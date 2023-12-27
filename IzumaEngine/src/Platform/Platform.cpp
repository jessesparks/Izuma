// Created by Jesse on 12/26/2023.
#include "Platform.h"
#include "LinuxPlatform.h"
#include "WindowsPlatform.h"
#include "../Izuma/Core/Logger.h"

namespace Izuma
{
    Platform* Platform::s_Instance = nullptr;
    Platform* Platform::GetPlatform()
    {
        if(s_Instance != nullptr)
            return s_Instance;
        else
        {
            #ifdef __linux__
                        s_Instance = new LinuxPlatform;
                        return s_Instance;
            #elif defined _WIN32
            #ifdef _WIN64
                        s_Instance = new WindowsPlatform;
                        return s_Instance;
            #else
            #error Izuma only supports 64 bit version of Windows
            #endif
            #endif
        }
    }
}