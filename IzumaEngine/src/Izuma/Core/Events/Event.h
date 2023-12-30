// Created by Jesse on 12/27/2023.
#pragma once
#include <type_traits>
#include <string>
#include <cstring>
#include <string_view>
#include <sstream>
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
        enum class EventType : int
        {
            INT,
            FLOAT,
            BOOL,
            CHAR,
            UINT,
            COUNT       //Number of unique types
        };
        EventType Type;

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
        friend class EventSerializer;
        std::size_t GetID() const { return m_ID; }
        bool Handled = false;

        Event() = delete;
        explicit Event(const std::string& name)
                : m_ID(ID(name))
        {
            m_Args.reserve(MAX_ARGS);
        }
        explicit Event(ID id)
                : m_ID(id)
        {
            m_Args.reserve(MAX_ARGS);
        }
        explicit Event(std::size_t id)
                : m_ID(id)
        {
            m_Args.reserve(MAX_ARGS);
        }

        int GetNumberOfArgs() const { return (int)m_Args.size(); }

        void AddArg(const std::string& name, EventArg arg)
        {
            m_Args[name] = arg;
        }

        EventArg GetArg(const std::string& name)
        {
            return m_Args.at(name);
        }

        //bool operator==(const Event other) const { if(m_ID == other.GetID()) return true; else return false; }
        constexpr bool operator==(const std::size_t other) const { if(m_ID == other) return true; else return false; }

    private:
        std::unordered_map<std::string, EventArg> m_Args;
        std::size_t m_ID;
    };

    struct EventSerializer
    {
        char data[1024];

        void Serialize(const Event& other)
        {
            std::stringstream out;
            out << other.GetID();
            out << ',';
            out << other.Handled;
            out << ',';
            out << other.GetNumberOfArgs();
            out << ',';
            for (const auto& arg : other.m_Args)
            {
                out << arg.first.size();
                out << ',';
                out << arg.first;
                out << ',';
                out << (int)arg.second.Type;
                out << ',';
                switch(arg.second.Type)
                {
                    case EventArg::EventType::INT:
                    {
                        out << (int)arg.second;
                        break;
                    }
                    case EventArg::EventType::FLOAT:
                    {
                        out << (float)arg.second;
                        break;
                    }
                    case EventArg::EventType::BOOL:
                    {
                        out << (bool)arg.second;
                        break;
                    }
                    case EventArg::EventType::CHAR:
                    {
                        out << (const char*)arg.second;
                        break;
                    }
                    case EventArg::EventType::UINT:
                    case EventArg::EventType::COUNT:
                    {
                        out << (unsigned int)arg.second;
                        break;
                    }
                }
                out << ',';
            }
            out.seekp(-1, out.cur);
            out << '\0';
            std::string tmp;
            out >> tmp;
            for (int i = 0; i < tmp.length(); i++)
            {
                data[i] = tmp[i];
            }
        }

        Event Deserialize(char* packet) const
        {
            std::stringstream in;
            in << packet;
            in.seekp(in.beg);
            if(in)
            {
                int len = 0;
                char comma;
                int nmbrargs;
                std::size_t id;
                in >> id;
                Event event(id);

                in >> comma;
                in >> event.Handled;
                in >> comma;
                in >> nmbrargs;
                in >> comma;
                for(int i = 0; i < nmbrargs; i++)
                {
                    in >> len;
                    in >> comma;
                    std::string temp;
                    if (in && len) {
                        std::vector<char> tmp(len);
                        in.read(tmp.data() , len); //deserialize characters of string
                        temp.assign(tmp.data(), len);
                    }
                    in >> comma;
                    int type;
                    in >> type;
                    in >>comma;
                    EventArg::EventType t = static_cast<EventArg::EventType>(type);
                    switch(t)
                    {
                        case EventArg::EventType::INT:
                        {
                            int value;
                            in >> value;
                            event.AddArg(temp, {t, value});
                            break;
                        }
                        case EventArg::EventType::FLOAT:
                        {
                            float value;
                            in >> value;
                            event.AddArg(temp, {t, value});
                            break;
                        }
                        case EventArg::EventType::BOOL:
                        {
                            bool value;
                            in >> value;
                            event.AddArg(temp, {t, value});
                            break;
                        }
                        case EventArg::EventType::CHAR:
                        {
                            std::string tmp;
                            in >> tmp;
                            event.AddArg(temp, {t, tmp.c_str()});
                            break;
                        }
                        case EventArg::EventType::UINT:
                        case EventArg::EventType::COUNT:
                        {
                            unsigned int value;
                            in >> value;
                            event.AddArg(temp, {t, value});
                            break;
                        }
                    }
                    in >> comma;
                }
                return event;
            }
            return Event("-1");
        }

        operator char*() { return data; }
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