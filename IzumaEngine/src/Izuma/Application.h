// Created by Jesse on 12/26/2023.
#pragma once
#include "../Platform/Platform.h"

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
    };
    //TO BE DEFINED IN CLIENT APP
    Application* CreateApplication();
} // Izuma