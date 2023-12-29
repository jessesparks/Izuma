// Created by Jesse on 12/26/2023.
#include "WindowsPlatform.h"
#include "../Izuma/Core/Events/Event.h"
#include "../Izuma/Core/Events/EventDispatcher.h"

#ifdef _WIN64

static double clock_frequency;
static LARGE_INTEGER start_time;

namespace Izuma
{
    LRESULT CALLBACK ProcessMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

    bool WindowsPlatform::Init(const char* name, uint32_t xStart, uint32_t yStart, uint32_t width, uint32_t height)
    {
        IZ_LOG_CORE_INFO("Creating Windows Platform");

        m_State = new internal_state;

        m_State->hInstance = GetModuleHandleA(0);

        //Setup and register window class
        WNDCLASSA wc;
        wc.style = CS_DBLCLKS;//Gets double-clicks
        wc.lpfnWndProc = ProcessMessage;
        wc.cbClsExtra = 0;
        wc.cbWndExtra = 0;
        wc.hInstance = m_State->hInstance;
        wc.hIcon = LoadIcon(m_State->hInstance, IDI_APPLICATION);
        wc.hCursor = LoadCursor(NULL, IDC_ARROW);
        wc.hbrBackground = NULL;
        wc.lpszClassName = "Izuma Application";

        if(!RegisterClassA(&wc))
        {
            MessageBoxA(0 , "Window registration failed", "Error", MB_ICONEXCLAMATION | MB_OK);
            return false;
        }

        uint32_t client_x = xStart;
        uint32_t client_y = yStart;
        uint32_t client_width = width;
        uint32_t client_height = height;

        uint32_t window_x = client_x;
        uint32_t window_y = client_y;
        uint32_t window_width = client_width;
        uint32_t window_height = client_height;

        uint32_t window_style = WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION;
        uint32_t window_ex_style = WS_EX_APPWINDOW;

        window_style |= WS_MAXIMIZEBOX;
        window_style |= WS_MINIMIZEBOX;
        window_style |= WS_THICKFRAME;

        //Get the size of the border
        RECT border_rect = {0, 0, 0, 0};
        AdjustWindowRectEx(&border_rect, window_style, 0, window_ex_style);

        //The border rectangle will be negative
        window_x += border_rect.left;
        window_y += border_rect.top;

        window_width += (border_rect.right - border_rect.left);
        window_height += (border_rect.bottom - border_rect.top);

        HWND handle = CreateWindowExA(
            window_ex_style, "Izuma Application", name, window_style,
            window_x, window_y, window_width, window_height, 0, 0,
            m_State->hInstance, 0);
        
        if(handle == 0)
        {
            MessageBoxA(NULL, "Window Creation Failed", "Error", MB_ICONEXCLAMATION | MB_OK);
            IZ_LOG_CORE_FATAL("Window creation failed");
            return false;
        }else
        {
            m_State->hwnd = handle;
        }

        //Show the window
        bool should_show = true;
        uint32_t show_window_command_flags = should_show ? SW_SHOW : SW_SHOWNOACTIVATE;
        //If initially minimized, use SW_MINIMIZE : SW_SHOWMINNOACTIVE;
        //If initially maximized, use SW_SHOWMAXIMIZED : SW_MAXIMIZE;
        ShowWindow(m_State->hwnd, show_window_command_flags);

        //Setup clock
        LARGE_INTEGER frequency;
        QueryPerformanceFrequency(&frequency);
        clock_frequency = 1.0f / (double)frequency.QuadPart;
        QueryPerformanceCounter(&start_time);

        return true;
    }

    void WindowsPlatform::Shutdown()
    {
        if(m_State->hwnd)
        {
            DestroyWindow(m_State->hwnd);
            m_State->hwnd = 0;
        }
    }

    bool WindowsPlatform::PumpMessages()
    {
        MSG message;
        while(PeekMessageA(&message, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&message);
            DispatchMessage(&message);
        }

        return true;
    }

    void WindowsPlatform::ConsoleWrite(const char* message, unsigned char level)
    {
        HANDLE console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
        //FATAL, ERROR, WARN, INFO, DEBUG, TRACE
        static unsigned char levels[6] = {64, 4, 6, 2, 1, 8};

        SetConsoleTextAttribute(console_handle, levels[level]);
        OutputDebugStringA(message);
        double length = strlen(message);
        LPDWORD number_written = 0;
        WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), message, (DWORD)length, number_written, 0);
    }

    void WindowsPlatform::ConsoleWriteError(const char* message, unsigned char level)
    {
        HANDLE console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
        //FATAL, ERROR, WARN, INFO, DEBUG, TRACE
        static unsigned char levels[6] = {64, 4, 6, 2, 1, 8};

        SetConsoleTextAttribute(console_handle, levels[level]);
        OutputDebugStringA(message);
        double length = strlen(message);
        LPDWORD number_written = 0;
        WriteConsoleA(GetStdHandle(STD_ERROR_HANDLE), message, (DWORD)length, number_written, 0);
    }

    double WindowsPlatform::GetTime()
    {
        LARGE_INTEGER current_time;
        QueryPerformanceCounter(&current_time);
        return (double)current_time.QuadPart * clock_frequency;
    }

    void WindowsPlatform::Sleep(uint32_t milliseconds)
    {
        Sleep(milliseconds);
    }

    LRESULT CALLBACK ProcessMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        switch (msg) {
            case WM_ERASEBKGND:
                // Notify the OS that erasing will be handled by the application to prevent flicker.
                return 1;
            case WM_CLOSE:
            {
                Event event(PlatformEvents::WINDOW_CLOSE);
                EventDispatcher::Dispatch(event);
                return 0;
            }
            case WM_DESTROY:
                PostQuitMessage(0);
                return 0;
            case WM_SIZE:
            {
                RECT r;
                GetClientRect(hwnd, &r);
                unsigned int width = r.right - r.left;
                unsigned int height = r.bottom - r.top;

                Event event(PlatformEvents::WINDOW_RESIZE);
                event.AddArg("uiWidth", {EventArg::UINT, width});
                event.AddArg("uiHeight", {EventArg::UINT, height});
                EventDispatcher::Dispatch(event);
                break;
            }
            case WM_KEYDOWN:
            case WM_SYSKEYDOWN:
            case WM_KEYUP:
            case WM_SYSKEYUP: {
                // Key pressed/released
                //bool pressed = (msg == WM_KEYDOWN || msg == WM_SYSKEYDOWN);
                // TODO: input processing

            } break;
            case WM_MOUSEMOVE: {
                // Mouse move
                //uint32_t x_position = GET_X_LPARAM(l_param);
                //uint32_t y_position = GET_Y_LPARAM(l_param);
                // TODO: input processing.
            } break;
            case WM_MOUSEWHEEL: {
                // uint32_t z_delta = GET_WHEEL_DELTA_WPARAM(w_param);
                // if (z_delta != 0) {
                //     // Flatten the input to an OS-independent (-1, 1)
                //     z_delta = (z_delta < 0) ? -1 : 1;
                //     // TODO: input processing.
                // }
            } break;
            case WM_LBUTTONDOWN:
            case WM_MBUTTONDOWN:
            case WM_RBUTTONDOWN:
            case WM_LBUTTONUP:
            case WM_MBUTTONUP:
            case WM_RBUTTONUP: {
                //bool pressed = msg == WM_LBUTTONDOWN || msg == WM_RBUTTONDOWN || msg == WM_MBUTTONDOWN;
                // TODO: input processing.
            } break;
        }

        return DefWindowProcA(hwnd, msg, wParam, lParam);
    }
}
#endif
