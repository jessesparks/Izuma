// Created by Jesse on 12/26/2023.
#include "Application.h"
#include "Core/Logger.h"
#include "Core/Events/EventDispatcher.h"
#include "Core/Input/Input.h"

const uint16 DEFAULT_PORT = 27020;

namespace Izuma
{
    Application* Application::s_Instance = nullptr;
    bool Application::s_Running = true;
    Platform* Application::s_Platform = nullptr;
    std::list<void(*)(Event&)> EventDispatcher::s_Listeners;
    std::unordered_map<unsigned int, bool> Input::s_KeyStates;

    Application::Application()
    {
        if(s_Instance != nullptr)
        {
            IZ_LOG_CORE_ERROR("Application already exists");
            return;
        }
        s_Instance = this;
        s_Platform = Platform::GetPlatform();
        EventDispatcher::Register(Application::OnEvent);
        Input::Init();
        ip.Clear();
        ip.ParseString("68.48.170.187");
        IZ_LOG_CORE_INFO("Initialized");
    }

    Application::~Application()
    {
        
    }

    void Application::OnEvent(Event& e)
    {
        switch(e.GetID())
        {
            case PlatformEvents::WINDOW_CLOSE:
            {
                s_Running = false;
                break;
            }
            case ID("Health Pickup"):
            {
                const char* name = (const char*) e.GetArg("cName");

                IZ_LOG_CORE_TRACE("%s(iHealth = %i)", name, (int) e.GetArg("iHealth"));
                IZ_LOG_CORE_TRACE("%s(fRadius = %f)", name, (float) e.GetArg("fRadius"));
                IZ_LOG_CORE_TRACE("%s(bGrabbed = %s)", name, (bool) e.GetArg("bGrabbed") ? "true" : "false");
                break;
            }
            case PlatformEvents::WINDOW_RESIZE:
            {
                unsigned int width, height;
                width = e.GetArg("uiWidth");
                height = e.GetArg("uiHeight");
                IZ_LOG_CORE_INFO("WindowResizeEvent:%ix%ip", width, height);
                break;
            }
            case ID("SPACE"):
            {
                IZ_LOG_CORE_INFO("SPACE");
            }
        }
    }

    void Application::Run()
    {
        if(s_Platform->Init("Izuma Application", 600, 300, 1280, 720))
        {
            Event health("Health Pickup");
            health.AddArg("iHealth", {EventArg::EventType::INT, 50});
            health.AddArg("fRadius", {EventArg::EventType::FLOAT, 10.0f});
            health.AddArg("bGrabbed", {EventArg::EventType::BOOL, false});
            health.AddArg("cName", {EventArg::EventType::CHAR, "Basic Health Pickup"});
            EventDispatcher::Dispatch(health);

            while(s_Running)
            {
                if(Input::IsKeyDown(IZ_KEY_H) && !m_ServerRunning)
                {
                    m_ServerRunning = true;
                    m_Server.Init(DEFAULT_PORT);
                }
                if(Input::IsKeyDown(IZ_KEY_J) && !m_ClientRunning)
                {
                    m_ClientRunning = true;
                    SteamNetworkingIPAddr addr = ip;
                    addr.m_port = DEFAULT_PORT;
                    m_Client.Init(addr);
                }
                s_Platform->PumpMessages();

                if(m_ServerRunning)
                {
                    m_Server.Update();
                }
                if(m_ClientRunning)
                {
                    m_Client.Update();
                }
            }

            s_Platform->Shutdown();
        }
    }
} // Izuma