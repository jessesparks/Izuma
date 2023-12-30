// Created by Jesse on 12/30/2023.
#pragma once
#include <steam/steamnetworkingsockets.h>
#include <steam/isteamnetworkingutils.h>

namespace Izuma
{
    class Client
    {
    public:
        void Init(const SteamNetworkingIPAddr& serverAddr);
        void Update();
        void Shutdown();

        void OnConnectionStatusChanged(SteamNetConnectionStatusChangedCallback_t* info);
    private:
        bool m_Quit;
        HSteamNetConnection m_Connection;
        ISteamNetworkingSockets* m_Interface;

        void PollIncomingMessages();
        void PollConnectionStateChanges();
        void PollLocalUserInput();

        static Client* s_CallbackInstance;
        static void ConnectionStatusChanged(SteamNetConnectionStatusChangedCallback_t* info);
    };
} // Izuma