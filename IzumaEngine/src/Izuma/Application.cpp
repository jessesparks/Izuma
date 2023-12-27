// Created by Jesse on 12/26/2023.
#include "Application.h"
#include "Core/Logger.h"

namespace Izuma
{
    Application* Application::s_Instance = nullptr;
    Platform* Application::s_Platform = nullptr;
    Logger* Logger::s_Instance = nullptr;
    Application::Application()
    {
        if(s_Instance != nullptr)
        {
            IZ_LOG_CORE_ERROR("Application already exists");
            return;
        }
        s_Instance = this;
        Logger::Init();
        s_Platform = Platform::GetPlatform();
        IZ_LOG_CORE_INFO("Initialized");
    }

    Application::~Application()
    {
        
    }

    void Application::Run()
    {
        if(s_Platform->Init("Izuma Application", 600, 300, 1280, 720))
        {
            while(true)
            {
                s_Platform->PumpMessages();
            }
            s_Platform->Shutdown();
        }
    }
} // Izuma