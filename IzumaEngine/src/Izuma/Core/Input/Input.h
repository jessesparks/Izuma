// Created by Jesse on 12/29/2023.
#pragma once
#include "../Events/Event.h"
#include "../Events/EventDispatcher.h"

#include <unordered_map>

namespace Izuma
{
    class Input
    {
    public:
        inline static void Init()
        {
            EventDispatcher::Register(Input::OnEvent);
        }

        inline static bool IsKeyDown(unsigned int keycode)
        {
            return s_KeyStates[keycode];
        }

    private:
        static std::unordered_map<unsigned int, bool> s_KeyStates;
        inline static void OnEvent(Event& e)
        {
            switch(e.GetID())
            {
                case InputEvents::KEY_PRESSED:
                {
                    s_KeyStates[(unsigned int)e.GetArg("uiKeycode")] = true;
                    break;
                }
                case InputEvents::KEY_RELEASED:
                {
                    s_KeyStates[(unsigned int)e.GetArg("uiKeycode")] = false;
                    break;
                }
            }
        }
    };
}