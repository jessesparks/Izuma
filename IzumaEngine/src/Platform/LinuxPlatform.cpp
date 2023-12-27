// Created by jesse on 12/26/23.
#include "LinuxPlatform.h"

#ifdef __linux__

#if _POSIX_C_SOURCE >= 199309L
#include <time.h> //nanosleep
#else
#include <unistd.h> //usleep
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string>

namespace Izuma
{
    bool Izuma::LinuxPlatform::Init(const char* name, uint32_t xStart, uint32_t yStart, uint32_t width, uint32_t height)
    {
        m_State = new internal_state;

        //Connect to X
        m_State->display = XOpenDisplay(NULL);

        //Turn off key repeats
        XAutoRepeatOff(m_State->display);

        //Retrieve the connection from the display
        m_State->connection = XGetXCBConnection(m_State->display);

        if(xcb_connection_has_error(m_State->connection))
        {
            IZ_LOG_CORE_FATAL("Failed to connect to X server via XCB");
            return false;
        }

        //Get data from X server
        const struct xcb_setup_t* setup = xcb_get_setup(m_State->connection);

        //Loop through screens using iterator
        xcb_screen_iterator_t it = xcb_setup_roots_iterator(setup);
        int screen_p = 0;
        for(int s = screen_p; s > 0; s--)
        {
            xcb_screen_next(&it);
        }

        //After screens have been looped through, assign it
        m_State->screen = it.data;

        //Allocate a XID for the window to be created
        m_State->window = xcb_generate_id(m_State->connection);

        //Register event types
        //XCB_CW_BACK_PIXEL = filling the window background with a single color
        //XCB_CW_EVENT_MASK is required
        uint32_t event_mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;

        //Listen for keyboard and mouse buttons
        uint32_t event_values = XCB_EVENT_MASK_BUTTON_PRESS | XCB_EVENT_MASK_BUTTON_RELEASE |
                                XCB_EVENT_MASK_KEY_PRESS | XCB_EVENT_MASK_KEY_RELEASE |
                                XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_POINTER_MOTION |
                                XCB_EVENT_MASK_STRUCTURE_NOTIFY;

        //Values to be sent over XCB (bg color, events)
        uint32_t value_list[] = {m_State->screen->black_pixel, event_values};

        //Create the window
        xcb_void_cookie_t cookie = xcb_create_window(
                m_State->connection,
                XCB_COPY_FROM_PARENT,   //Depth
                m_State->window,
                m_State->screen->root,  //Parent
                xStart,
                yStart,
                width,
                height,
                0,                      //No border
                XCB_WINDOW_CLASS_INPUT_OUTPUT,
                m_State->screen->root_visual,
                event_mask,
                value_list);

        //Change the title
        xcb_change_property(
                m_State->connection,
                XCB_PROP_MODE_REPLACE,
                m_State->window,
                XCB_ATOM_WM_NAME,
                XCB_ATOM_STRING,
                8,              //View data 8 bits at a time
                strlen(name),
                name);

        //Tell the server to notify when the window manager attempts to destroy the window
        xcb_intern_atom_cookie_t wm_delete_cookie = xcb_intern_atom(
                m_State->connection,
                0,
                strlen("WM_DELETE_WINDOW"),
                "WM_DELETE_WINDOW");
        xcb_intern_atom_cookie_t wm_protocols_cookie = xcb_intern_atom(
                m_State->connection,
                0,
                strlen("WM_PROTOCOLS"),
                "WM_PROTOCOLS");
        xcb_intern_atom_reply_t* wm_delete_reply = xcb_intern_atom_reply(
                m_State->connection,
                wm_delete_cookie,
                NULL);
        xcb_intern_atom_reply_t* wm_protocols_reply = xcb_intern_atom_reply(
                m_State->connection,
                wm_protocols_cookie,
                NULL);
        m_State->wm_delete_win = wm_delete_reply->atom;
        m_State->wm_protocols = wm_protocols_reply->atom;

        xcb_change_property(
                m_State->connection,
                XCB_PROP_MODE_REPLACE,
                m_State->window,
                wm_protocols_reply->atom,
                4, 31, 1,
                &wm_delete_reply->atom);

        //Map the window to the screen
        xcb_map_window(m_State->connection, m_State->window);

        //Flush the stream
        uint32_t stream_result = xcb_flush(m_State->connection);
        if(stream_result <= 0)
        {
            IZ_LOG_CORE_FATAL("An error occured when flushing the XCB stream");
            return false;
        }

        return true;
    }

    void LinuxPlatform::Shutdown()
    {
        XAutoRepeatOn(m_State->display);

        xcb_destroy_window(m_State->connection, m_State->window);
    }

    bool LinuxPlatform::PumpMessages()
    {
        xcb_generic_event_t* event;
        xcb_client_message_event_t* cm;

        bool quit_flagged = false;

        // Poll for events until null is returned.
        while (xcb_poll_for_event(m_State->connection))
        {
            if (event == 0)
            {
                break;
            }
            // Input events
            switch (event->response_type & ~0x80)
            {
                case XCB_KEY_PRESS:
                case XCB_KEY_RELEASE:
                {
                    // TODO: Key presses and releases
                } break;
                case XCB_BUTTON_PRESS:
                case XCB_BUTTON_RELEASE:
                {
                    // TODO: Mouse button presses and releases
                }
                case XCB_MOTION_NOTIFY:
                    // TODO: mouse movement
                    break;
                case XCB_CONFIGURE_NOTIFY:
                {
                    // TODO: Resizing
                }

                case XCB_CLIENT_MESSAGE:
                {
                    cm = (xcb_client_message_event_t*)event;

                    // Window close
                    if (cm->data.data32[0] == m_State->wm_delete_win)
                    {
                        quit_flagged = true;
                    }
                } break;
                default:
                    // Something else
                    break;
            }
        }
        return !quit_flagged;
    }

    void LinuxPlatform::ConsoleWrite(const char* message, unsigned char level)
    {
        //TRACE, DEBUG, INFO, WARN, ERROR, FATAL
        const char* colour_strings[] = {"1;30", "1;34", "1;32", "1;33", "1;31", "0;41"};
        printf("\033[%sm%s\033[0m", colour_strings[level], message);
    }

    void LinuxPlatform::ConsoleWriteError(const char* message, unsigned char level)
    {
        //TRACE, DEBUG, INFO, WARN, ERROR, FATAL
        const char* colour_strings[] = {"1;30", "1;34", "1;32", "1;33", "1;31", "0;41"};
        printf("\033[%sm%s\033[0m", colour_strings[level], message);
    }

    double LinuxPlatform::GetTime()
    {
        struct timespec current;
        clock_gettime(CLOCK_MONOTONIC, &current);
        return (double)(current.tv_sec + (current.tv_nsec * 0.000000001));
    }

    void LinuxPlatform::Sleep(uint32_t milliseconds)
    {
        #if _POSIX_C_SOURCE >= 199309L
            struct timespec ts;
            ts.tv_sec = milliseconds / 1000;
            ts.tv_nsec = (milliseconds % 1000) * 1000 * 1000;
            nanosleep(&ts, 0);
        #else
            if (ms >= 1000)
            {
                sleep(ms / 1000);
            }
            usleep((ms % 1000) * 1000);
        #endif
    }
}

#endif
