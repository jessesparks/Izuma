// Created by Jesse on 12/27/2023.
#pragma once
#include <type_traits>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>
#include <variant>

namespace Izuma
{
    struct ID
    {
    public:
        constexpr ID(const std::string_view& string): m_ID(Hash(string)) {}

        constexpr operator std::size_t() const { return m_ID; }
    private:
        std::size_t m_ID;

        template <typename Str>
        constexpr size_t Hash(const Str& toHash)
        {
            static_assert(sizeof(size_t) == 8);
            // fnv-1 64 bit
            size_t result = 0xcbf29ce484222325;

            for (char c : toHash) {
                result *= 1099511628211;
                result ^= c;
            }

            return result;
        }

        template <size_t N>
        constexpr size_t Hash(char const (&toHash)[N])
        {
            return Hash(std::string_view(toHash));
        }
    };



    struct EventArg
    {
        enum
        {
            INT,
            FLOAT,
            BOOL,
            CHAR,
            UINT,
            COUNT       //Number of unique types
        } Type;

        std::variant<int, float, bool, const char*, unsigned int> value;

        operator unsigned int() const { return std::get<unsigned int>(value); }
        operator int() const { return std::get<int>(value); }
        operator float() const { return std::get<float>(value); }
        operator bool() const { return std::get<bool>(value); }
        operator const char*() const { return std::get<const char*>(value); }
    };

    enum class IZStaticTypes
    {
        None = 0, WindowResize, WindowClose, KeyPressed, KeyReleased, MouseMove, MouseScrolled
    };

    struct Event
    {
        const static int MAX_ARGS = 16;
    public:
        std::size_t GetID() const { return m_ID; }
        bool Handled = false;

        Event() = delete;
        explicit Event(const std::string& name)
                : m_ID(ID(name))
        {
            args.reserve(MAX_ARGS);
        }
        explicit Event(ID id)
                : m_ID(id)
        {
            args.reserve(MAX_ARGS);
        }
        explicit Event(std::size_t id)
                : m_ID(id)
        {
            args.reserve(MAX_ARGS);
        }

        int GetNumberOfArgs() { return (int)args.size(); }

        void AddArg(const std::string& name, EventArg arg)
        {
            args[name] = arg;
        }

        EventArg GetArg(const std::string& name)
        {
            return args[name];
        }

        //bool operator==(const Event other) const { if(m_ID == other.GetID()) return true; else return false; }
        constexpr bool operator==(const std::size_t other) const { if(m_ID == other) return true; else return false; }

    private:
        std::unordered_map<std::string, EventArg> args;
        std::size_t m_ID;
    };
}

namespace Izuma::PlatformEvents
{
    const static std::size_t WINDOW_RESIZE =  ID("Window Resize");
    const static std::size_t WINDOW_CLOSE =  ID("Window Close");
}

namespace Izuma::InputEvents
{
    const static std::size_t KEY_PRESSED = ID("Key Pressed");
    const static std::size_t KEY_RELEASED = ID("Key Released");
    const static std::size_t MOUSE_MOVE = ID("Mouse Moved");
    const static std::size_t MOUSE_SCROLLED = ID("Mouse Scrolled");
}