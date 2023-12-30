// Created by Jesse on 12/29/2023.
#include "Server.h"
#include "../Logger.h"

namespace Izuma
{
    Server* Server::s_CallbackInstance;
    void Server::Init(uint16 port)
    {
        SteamDatagramErrMsg errMsg;
        if (!GameNetworkingSockets_Init(nullptr, errMsg))
            IZ_LOG_CORE_ERROR("GameNetworkingSockets_Init failed.  %s", errMsg);

        m_Quit = false;
        m_Interface = SteamNetworkingSockets();

        SteamNetworkingIPAddr serverLocalAddr;
        serverLocalAddr.Clear();
        serverLocalAddr.m_port = port;
        SteamNetworkingConfigValue_t options;
        options.SetPtr(k_ESteamNetworkingConfig_Callback_ConnectionStatusChanged, (void*)ConnectionStatusChanged);
        m_ListenSocket = m_Interface->CreateListenSocketIP(serverLocalAddr, 1, &options);
        if(m_ListenSocket == k_HSteamNetPollGroup_Invalid)
        {
            IZ_LOG_CORE_ERROR("Failed to listen on port %d", port);
        }
        m_PollGroup = m_Interface->CreatePollGroup();
        if(m_PollGroup == k_HSteamNetPollGroup_Invalid)
        {
            IZ_LOG_CORE_ERROR("Failed to listen on port %d", port);
        }
        IZ_LOG_CORE_INFO("Server listening on port %d", port);
    }

    void Server::Update()
    {
        PollIncomingMessages();
        PollConnectionStateChanges();
        PollLocalUserInput();
    }

    void Server::Shutdown()
    {
        m_Quit = true;
        IZ_LOG_CORE_INFO("Server is shutting down");
        for(auto client : m_Clients)
        {
            //Send message
            m_Interface->CloseConnection(client.first, 0, "Server Shutdown", true);
        }
        m_Clients.clear();

        m_Interface->CloseListenSocket(m_ListenSocket);
        m_ListenSocket = k_HSteamListenSocket_Invalid;

        m_Interface->DestroyPollGroup(m_PollGroup);
        m_PollGroup = k_HSteamNetPollGroup_Invalid;

        GameNetworkingSockets_Kill();
    }

    void Server::PollIncomingMessages()
    {
        char temp[ 1024 ];

        while (!m_Quit)
        {
            ISteamNetworkingMessage* pIncomingMsg = nullptr;
            int numMsgs = m_Interface->ReceiveMessagesOnPollGroup( m_PollGroup, &pIncomingMsg, 1 );
            if ( numMsgs == 0 )
                break;
            if ( numMsgs < 0 )
                IZ_LOG_CORE_ERROR( "Error checking for messages. Messages less than 0" );
            IZ_CORE_ASSERT(numMsgs == 1 && pIncomingMsg != nullptr, "Error checking for messages");
            auto client = m_Clients.find( pIncomingMsg->m_conn );
            IZ_CORE_ASSERT(client != m_Clients.end(), "Message recieved from invalid client");

            //TO-DO Dispatch message to clients
            for(auto& c : m_Clients)
            {
                //if(c.first != client->first)
                //{
                    m_Interface->SendMessageToConnection(c.first, pIncomingMsg->m_pData, pIncomingMsg->m_cbSize, k_nSteamNetworkingSend_NoDelay, nullptr);
                //}
            }

            // We don't need this anymore.
            pIncomingMsg->Release();
        }
    }

    void Server::PollConnectionStateChanges()
    {
        s_CallbackInstance = this;
        m_Interface->RunCallbacks();
    }

    void Server::PollLocalUserInput()
    {

    }

    void Server::ConnectionStatusChanged(SteamNetConnectionStatusChangedCallback_t* info)
    {
        s_CallbackInstance->OnConnectionStatusChanged(info);
    }

    void Server::OnConnectionStatusChanged(SteamNetConnectionStatusChangedCallback_t* info)
    {
        char temp[1024];

        // What's the state of the connection?
        switch ( info->m_info.m_eState )
        {
            case k_ESteamNetworkingConnectionState_None:
                // NOTE: We will get callbacks here when we destroy connections.  You can ignore these.
                break;

            case k_ESteamNetworkingConnectionState_ClosedByPeer:
            case k_ESteamNetworkingConnectionState_ProblemDetectedLocally:
            {
                // Ignore if they were not previously connected.  (If they disconnected
                // before we accepted the connection.)
                if ( info->m_eOldState == k_ESteamNetworkingConnectionState_Connected )
                {

                    // Locate the client.  Note that it should have been found, because this
                    // is the only codepath where we remove clients (except on shutdown),
                    // and connection change callbacks are dispatched in queue order.
                    auto client = m_Clients.find(info->m_hConn);
                    IZ_CORE_ASSERT(client != m_Clients.end(), "Could not find client");

                    // Select appropriate log messages
                    const char *pszDebugLogAction;
                    if ( info->m_info.m_eState == k_ESteamNetworkingConnectionState_ProblemDetectedLocally )
                    {
                        pszDebugLogAction = "problem detected locally";
                        IZ_LOG_CORE_ERROR("Lost connection to client:%s", client->second);
                    }
                    else
                    {
                        // Note that here we could check the reason code to see if
                        // it was a "usual" connection or an "unusual" one.
                        pszDebugLogAction = "closed by peer";
                        IZ_LOG_CORE_INFO("%s has left", client->second);
                    }

                    // Spew something to our own log.  Note that because we put their nick
                    // as the connection description, it will show up, along with their
                    // transport-specific data (e.g. their IP address)
                    IZ_LOG_CORE_INFO( "Connection %s %s, reason %d: %s\n",
                            info->m_info.m_szConnectionDescription,
                            pszDebugLogAction,
                            info->m_info.m_eEndReason,
                            info->m_info.m_szEndDebug
                    );

                    m_Clients.erase(client);

                    //TO-DO send message to left everybody know someone left
                }
                else
                {
                    IZ_CORE_ASSERT(info->m_eOldState == k_ESteamNetworkingConnectionState_Connecting, "");
                }

                // Clean up the connection.  This is important!
                // The connection is "closed" in the network sense, but
                // it has not been destroyed.  We must close it on our end, too
                // to finish up.  The reason information do not matter in this case,
                // and we cannot linger because it's already closed on the other end,
                // so we just pass 0's.
                m_Interface->CloseConnection( info->m_hConn, 0, nullptr, false );
                break;
            }

            case k_ESteamNetworkingConnectionState_Connecting:
            {
                // This must be a new connection
                IZ_CORE_ASSERT(m_Clients.find(info->m_hConn) == m_Clients.end(), "Client connecting to invalid slot");

                IZ_LOG_CORE_INFO("Connection request from %s", info->m_info.m_szConnectionDescription);

                // A client is attempting to connect
                // Try to accept the connection.
                if (m_Interface->AcceptConnection(info->m_hConn) != k_EResultOK)
                {
                    // This could fail.  If the remote host tried to connect, but then
                    // disconnected, the connection may already be half closed.  Just
                    // destroy whatever we have on our side.
                    m_Interface->CloseConnection(info->m_hConn, 0, nullptr, false);
                    IZ_LOG_CORE_INFO("Can't accept connection.  (It was already closed?)");
                    break;
                }

                // Assign the poll group
                if (!m_Interface->SetConnectionPollGroup(info->m_hConn, m_PollGroup))
                {
                    m_Interface->CloseConnection(info->m_hConn, 0, nullptr, false );
                    IZ_LOG_CORE_ERROR("Failed to set poll group?");
                    break;
                }

                // Add them to the client list, using std::map wacky syntax
                m_Clients[info->m_hConn];
                break;
            }

            case k_ESteamNetworkingConnectionState_Connected:
                // We will get a callback immediately after accepting the connection.
                // Since we are the server, we can ignore this, it's not news to us.
                break;

            default:
                // Silences -Wswitch
                break;
        }
    }
} // Izuma