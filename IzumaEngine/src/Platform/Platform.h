// Created by Jesse on 12/26/2023.
#pragma once
#include <cstdint>

namespace Izuma
{
    class Platform
    {
    public:
        struct PlatformState{
            void* internal_state;
        };

        Platform() = default;
        ~Platform() = default;

        virtual bool Init(const char* name, uint32_t xStart, uint32_t yStart, uint32_t width, uint32_t height) = 0;
        virtual void Shutdown() = 0;

        virtual bool PumpMessages() = 0;

        virtual void ConsoleWrite(const char* message, unsigned char level) = 0;
        virtual void ConsoleWriteError(const char* message, unsigned char level) = 0;

        virtual double GetTime() = 0;

        virtual void Sleep(uint32_t milliseconds) = 0;

        static Platform* GetPlatform();

    private:
        static Platform* s_Instance;
    };
}