// Created by Jesse on 12/23/2023.
#pragma once

#include "../../Platform/Platform.h"
#include "../Application.h"

#include <cstdio>
#include <cstring>
#include <stdarg.h>
#include <memory>
#include <string>

#define CORE_LOG_WARN_ENABLED true
#define CORE_LOG_INFO_ENABLED true
#define CORE_LOG_DEBUG_ENABLED true
#define CORE_LOG_TRACE_ENABLED true

#define CLIENT_LOG_WARN_ENABLED true
#define CLIENT_LOG_INFO_ENABLED true
#define CLIENT_LOG_DEBUG_ENABLED true
#define CLIENT_LOG_TRACE_ENABLED true

#ifdef IZ_RELEASE
#define CORE_LOG_DEBUG_ENABLED false
#define CORE_LOG_TRACE_ENABLED false
#define CLIENT_LOG_DEBUG_ENABLED false
#define CLIENT_LOG_TRACE_ENABLED false
#endif

namespace Izuma
{
    class Logger
    {
    public:
        enum LogLevel{
            LOG_LEVEL_TRACE = 0,
            LOG_LEVEL_DEBUG = 1,
            LOG_LEVEL_INFO = 2,
            LOG_LEVEL_WARN = 3,
            LOG_LEVEL_ERROR = 4,
            LOG_LEVEL_FATAL = 5

        };

        Logger() = default;

        static inline void Init()
        {
            s_Instance = new Logger;
        }

        inline static Logger* GetLogger() { return s_Instance; }

        inline void Log(LogLevel level, const char* core, const char* file, int line, const char* msg, ...)
        {
            const char* level_strings[6] = { "\x1B[0;36;49m[TRACE]", "\x1B[0;32;49m[DEBUG]", "\x1B[1;39;49m[INFO]", "\x1B[1;33;49m[WARN]", "\x1B[1;31;49m[ERROR]",   "\x1B[1;31;40m[FATAL]",};

            bool is_error = level > LogLevel::LOG_LEVEL_WARN;

            const uint32_t max_length = 32000;

            char out_message[max_length];
            std::memset(out_message, 0, sizeof(out_message));

            __builtin_va_list arg_ptr;
            va_start(arg_ptr, msg);
            std::vsnprintf(out_message, max_length, msg, arg_ptr);
            va_end(arg_ptr);

            char out_message2[max_length];
            std::sprintf(out_message2, "%s%s%s%s%s%s%s\n", level_strings[level], core, file, "(", std::to_string(line).c_str(), "): ", out_message);

            if(is_error)
            {
                Application::GetPlatform()->ConsoleWriteError(out_message2, level);
            }else
            {
                Application::GetPlatform()->ConsoleWrite(out_message2, level);
            }

            //std::printf("%s", out_message2);
        }
    private:
        static Logger* s_Instance;
    };
} // Izuma

//CORE MACROS
#define IZ_LOG_CORE_FATAL(message, ...) Izuma::Logger::GetLogger()->Log(Izuma::Logger::LOG_LEVEL_FATAL, "[CORE]: ", __FILE__ , __LINE__ , message, ##__VA_ARGS__);
#define IZ_LOG_CORE_ERROR(message, ...) Izuma::Logger::GetLogger()->Log(Izuma::Logger::LOG_LEVEL_ERROR, "[CORE]: ", __FILE__ , __LINE__ , message, ##__VA_ARGS__);

#if CORE_LOG_WARN_ENABLED
#define IZ_LOG_CORE_WARN(message, ...) Izuma::Logger::GetLogger()->Log(Izuma::Logger::LOG_LEVEL_WARN, "[CORE]: ", __FILE__ , __LINE__ , message, ##__VA_ARGS__);
#else
#define IZ_LOG_CORE_WARN(message, ...)
#endif

#if CORE_LOG_INFO_ENABLED
#define IZ_LOG_CORE_INFO(message, ...) Izuma::Logger::GetLogger()->Log(Izuma::Logger::LOG_LEVEL_INFO, "[CORE]: ", __FILE__ , __LINE__ , message, ##__VA_ARGS__);
#else
#define IZ_LOG_CORE_INFO(message, ...)
#endif

#if CORE_LOG_DEBUG_ENABLED
#define IZ_LOG_CORE_DEBUG(message, ...) Izuma::Logger::GetLogger()->Log(Izuma::Logger::LOG_LEVEL_DEBUG, "[CORE]: ", __FILE__ , __LINE__ , message, ##__VA_ARGS__);
#else
#define IZ_LOG_CORE_DEBUG(message, ...)
#endif

#if CORE_LOG_TRACE_ENABLED
#define IZ_LOG_CORE_TRACE(message, ...) Izuma::Logger::GetLogger()->Log(Izuma::Logger::LOG_LEVEL_TRACE, "[CORE]: ", __FILE__ , __LINE__ , message, ##__VA_ARGS__);
#else
#define IZ_LOG_CORE_TRACE(message, ...)
#endif


//CLIENT MACROS
#define IZ_LOG_FATAL(message, ...) Izuma::Logger::GetLogger()->Log(Izuma::Logger::LOG_LEVEL_FATAL, "[CLIENT]: ", __FILE__ , __LINE__ , message, ##__VA_ARGS__);
#define IZ_LOG_ERROR(message, ...) Izuma::Logger::GetLogger()->Log(Izuma::Logger::LOG_LEVEL_ERROR, "[CLIENT]: ", __FILE__ , __LINE__ , message, ##__VA_ARGS__);

#if CLIENT_LOG_WARN_ENABLED
#define IZ_LOG_WARN(message, ...) Izuma::Logger::GetLogger()->Log(Izuma::Logger::LOG_LEVEL_WARN, "[CLIENT]: ", __FILE__ , __LINE__ , message, ##__VA_ARGS__);
#else
#define IZ_LOG_WARN(message, ...)
#endif

#if CLIENT_LOG_INFO_ENABLED
#define IZ_LOG_INFO(message, ...) Izuma::Logger::GetLogger()->Log(Izuma::Logger::LOG_LEVEL_INFO, "[CLIENT]: ", __FILE__ , __LINE__ , message, ##__VA_ARGS__);
#else
#define IZ_LOG_INFO(message, ...)
#endif

#if CLIENT_LOG_DEBUG_ENABLED
#define IZ_LOG_DEBUG(message, ...) Izuma::Logger::GetLogger()->Log(Izuma::Logger::LOG_LEVEL_DEBUG, "[CLIENT]: ", __FILE__ , __LINE__ , message, ##__VA_ARGS__);
#else
#define IZ_LOG_DEBUG(message, ...)
#endif

#if CLIENT_LOG_TRACE_ENABLED
#define IZ_LOG_TRACE(message, ...) Izuma::Logger::GetLogger()->Log(Izuma::Logger::LOG_LEVEL_TRACE, "[CLIENT]: ", __FILE__ , __LINE__ , message, ##__VA_ARGS__);
#else
#define IZ_LOG_TRACE(message, ...)
#endif

#if IZ_ENABLE_ASSERTS
#define IZ_CORE_ASSERT(x, ...) { if(!(x)) { IZ_LOG_CORE_ERROR("Assertion Failed: %s", __VA_ARGS__); } }
#define IZ_ASSERT(x, ...) { if(!(x)) { IZ_LOG_ERROR("Assertion Failed: %s", __VA_ARGS__); } }
#else
#define IZ_CORE_ASSERT(x, ...)
#define IZ_ASSERT(x, ...)
#endif