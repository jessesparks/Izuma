// Created by Jesse on 12/26/2023.
#pragma once
#include "Platform.h"

#ifdef _WIN64
#include "../Izuma/Core/Logger.h"

#include <Windows.h>
#include <Windowsx.h>

struct internal_state
{
    HINSTANCE hInstance;
    HWND hwnd;
};

namespace Izuma
{
    class WindowsPlatform : public Platform
    {
    public:
        WindowsPlatform() = default;

        virtual bool Init(const char* name, uint32_t xStart, uint32_t yStart, uint32_t width, uint32_t height) override;
        virtual void Shutdown() override;

        virtual bool PumpMessages() override;

        virtual void ConsoleWrite(const char* message, unsigned char level) override;
        virtual void ConsoleWriteError(const char* message, unsigned char level) override;

        virtual double GetTime() override;

        virtual void Sleep(uint32_t milliseconds) override;

    private:
        internal_state* m_State;
    };
}
#endif