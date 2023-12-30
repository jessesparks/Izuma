// Created by Jesse on 12/30/2023.
#include "Client.h"
#include "../Logger.h"
#include "../Events/EventDispatcher.h"
#include "../Input/Input.h"

namespace Izuma
{
    Client* Client::s_CallbackInstance;
    void Client::Init(const SteamNetworkingIPAddr& serverAddr)
    {
        SteamDatagramErrMsg errMsg;
        if (!GameNetworkingSockets_Init(nullptr, errMsg))
            IZ_LOG_CORE_ERROR("GameNetworkingSockets_Init failed.  %s", errMsg);

        m_Quit = false;
        m_Interface = SteamNetworkingSockets();

        // Start connecting
        char szAddr[ SteamNetworkingIPAddr::k_cchMaxString ];
        serverAddr.ToString( szAddr, sizeof(szAddr), true );
        IZ_LOG_CORE_INFO("Connecting to chat server at %s", szAddr );
        SteamNetworkingConfigValue_t options;
        options.SetPtr(k_ESteamNetworkingConfig_Callback_ConnectionStatusChanged, (void*)ConnectionStatusChanged);
        m_Connection = m_Interface->ConnectByIPAddress(serverAddr, 1, &options);
        if (m_Connection == k_HSteamNetConnection_Invalid)
            IZ_LOG_CORE_ERROR("Failed to create connection");
    }

    void Client::Update()
    {
        PollIncomingMessages();
        PollConnectionStateChanges();
        PollLocalUserInput();
    }

    void Client::Shutdown()
    {
        m_Quit = true;

        GameNetworkingSockets_Kill();
    }

    void Client::PollIncomingMessages()
    {
        while (!m_Quit)
        {
            ISteamNetworkingMessage *pIncomingMsg = nullptr;
            int numMsgs = m_Interface->ReceiveMessagesOnConnection( m_Connection, &pIncomingMsg, 1 );
            if (numMsgs == 0)
                break;
            if (numMsgs < 0)
                IZ_LOG_CORE_ERROR("Error checking for messages");

            //This is where you could handle messages from the server
            EventSerializer ser;
            Event event = ser.Deserialize((char*)pIncomingMsg->m_pData);
            EventDispatcher::Dispatch(event);

            // We don't need this anymore.
            pIncomingMsg->Release();
        }
    }

    void Client::PollConnectionStateChanges()
    {
        s_CallbackInstance = this;
        m_Interface->RunCallbacks();
    }

    void Client::PollLocalUserInput()
    {
        if(Input::IsKeyDown(IZ_KEY_SPACE))
        {
            Event event("SPACE");
            EventSerializer ser;
            ser.Serialize(event);
            m_Interface->SendMessageToConnection(m_Connection, ser, strlen(ser.data), k_nSteamNetworkingSend_NoDelay, nullptr);
        }
    }

    void Client::ConnectionStatusChanged(SteamNetConnectionStatusChangedCallback_t* info)
    {
        s_CallbackInstance->OnConnectionStatusChanged(info);
    }

    void Client::OnConnectionStatusChanged(SteamNetConnectionStatusChangedCallback_t* info)
    {
        //IZ_CORE_ASSERT(info->m_hConn == m_Connection || m_Connection == k_HSteamNetConnection_Invalid, "Connection was invalid");

        // What's the state of the connection?
        switch (info->m_info.m_eState)
        {
            case k_ESteamNetworkingConnectionState_None:
                // NOTE: We will get callbacks here when we destroy connections.  You can ignore these.
                break;

            case k_ESteamNetworkingConnectionState_ClosedByPeer:
            case k_ESteamNetworkingConnectionState_ProblemDetectedLocally:
            {
                // Print an appropriate message
                if (info->m_eOldState == k_ESteamNetworkingConnectionState_Connecting)
                {
                    // Note: we could distinguish between a timeout, a rejected connection,
                    // or some other transport problem.
                    IZ_LOG_CORE_ERROR("Could not connect to host");
                }
                else if (info->m_info.m_eState == k_ESteamNetworkingConnectionState_ProblemDetectedLocally)
                {
                    IZ_LOG_CORE_ERROR("Could not connect to host (%s)", info->m_info.m_szEndDebug);
                }
                else
                {
                    // NOTE: We could check the reason code for a normal disconnection
                    IZ_LOG_CORE_ERROR("Could not connect to host (%s)", info->m_info.m_szEndDebug );
                }

                // Clean up the connection.  This is important!
                // The connection is "closed" in the network sense, but
                // it has not been destroyed.  We must close it on our end, too
                // to finish up.  The reason information do not matter in this case,
                // and we cannot linger because it's already closed on the other end,
                // so we just pass 0's.
                m_Interface->CloseConnection(info->m_hConn, 0, nullptr, false);
                m_Connection = k_HSteamNetConnection_Invalid;
                Shutdown();
                break;
            }

            case k_ESteamNetworkingConnectionState_Connecting:
                // We will get this callback when we start connecting.
                // We can ignore this.
                break;

            case k_ESteamNetworkingConnectionState_Connected:
                IZ_LOG_CORE_INFO("Connected to server OK");
                break;

            default:
                // Silences -Wswitch
                break;
        }
    }
} // Izuma