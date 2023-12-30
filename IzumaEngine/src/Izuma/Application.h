// Created by Jesse on 12/26/2023.
#pragma once
#include "../Platform/Platform.h"
#include "Core/Events/Event.h"
#include "Core/Network/Server.h"
#include "Core/Network/Client.h"

namespace Izuma
{
    class Application
    {
    public:
        Application();
        virtual ~Application();

        void Run();

        inline static Application* GetApplication() { return s_Instance; }
        inline static Platform* GetPlatform() { return s_Platform; }
    private:
        static Application* s_Instance;
        static Platform* s_Platform;
        static bool s_Running;
        static void OnEvent(Event& e);
        bool m_ServerRunning = false;
        bool m_ClientRunning = false;
        Server m_Server;
        Client m_Client;
        SteamNetworkingIPAddr ip;
    };
    //TO BE DEFINED IN CLIENT APP
    Application* CreateApplication();
} // Izuma