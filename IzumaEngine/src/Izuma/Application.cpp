// Created by Jesse on 12/26/2023.
#include "Application.h"
#include "Core/Logger.h"
#include "Core/Events/EventDispatcher.h"
#include "Core/Input/Input.h"

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
        }
    }

    void Application::Run()
    {
        if(s_Platform->Init("Izuma Application", 600, 300, 1280, 720))
        {
            Event health("Health Pickup");
            health.AddArg("iHealth", {EventArg::INT, 50});
            health.AddArg("fRadius", {EventArg::FLOAT, 10.0f});
            health.AddArg("bGrabbed", {EventArg::BOOL, false});
            health.AddArg("cName", {EventArg::CHAR, "Basic Health Pickup"});
            EventDispatcher::Dispatch(health);

            while(s_Running)
            {
                s_Platform->PumpMessages();
            }

            s_Platform->Shutdown();
        }
    }
} // Izuma