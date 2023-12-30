// Created by Jesse on 12/29/2023.
#pragma once
#include <steam/steamnetworkingsockets.h>
#include <steam/isteamnetworkingutils.h>

#include <map>

namespace Izuma
{
    class Server
    {
    public:
        void Init(uint16 port);
        void Update();
        void Shutdown();

        void OnConnectionStatusChanged(SteamNetConnectionStatusChangedCallback_t* info);
    private:
        bool m_Quit;
        ISteamNetworkingSockets* m_Interface;
        HSteamListenSocket m_ListenSocket;
        HSteamNetPollGroup m_PollGroup;

        struct Client_t
        {
            const char* name;
        };

        std::map<HSteamNetConnection, Client_t> m_Clients;

        void PollIncomingMessages();
        void PollConnectionStateChanges();
        void PollLocalUserInput();

        static Server* s_CallbackInstance;
        static void ConnectionStatusChanged(SteamNetConnectionStatusChangedCallback_t* info);
    };
} // Izuma