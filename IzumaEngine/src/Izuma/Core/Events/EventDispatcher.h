// Created by Jesse on 12/28/2023.
#pragma once

#include "Event.h"

#include <list>

namespace Izuma
{
    class EventDispatcher
    {
    public:
        typedef void(*EventFunc)(Event&);

        EventDispatcher() = delete;
        ~EventDispatcher() = delete;

        static void Register(EventFunc func) { s_Listeners.push_back(func); }

        static void Dispatch(Event& e)
        {
            for(auto listener : s_Listeners)
            {
                if(!e.Handled)
                {
                    listener(e);
                }
                else
                {
                    break;
                }
            }
        }

    private:
        static std::list<EventFunc> s_Listeners;
    };
} // Izuma