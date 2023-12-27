// Created by Jesse on 12/26/2023.
#pragma once
#include "Platform.h"

#ifdef __linux__
#include "../Izuma/Core/Logger.h"

#include <xcb/xcb.h>
#include <X11/keysym.h>
#include <X11/XKBlib.h>  // sudo apt-get install libx11-dev
#include <X11/Xlib.h>
#include <X11/Xlib-xcb.h>  // sudo apt-get install libxkbcommon-x11-dev
#include <sys/time.h>

struct internal_state
{
    Display* display;
    xcb_connection_t* connection;
    xcb_window_t window;
    xcb_screen_t* screen;
    xcb_atom_t wm_protocols;
    xcb_atom_t wm_delete_win;
};

namespace Izuma
{
    class LinuxPlatform : public Platform
    {
    public:
        LinuxPlatform() = default;

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