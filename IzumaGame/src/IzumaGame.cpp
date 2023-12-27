// Created by Jesse on 12/23/2023.
#include "Izuma.h"

class IzumaGame : public Izuma::Application
{
public:
    IzumaGame()
    {
        IZ_LOG_INFO("Initialized");
    }

    ~IzumaGame() override = default;
};

Izuma::Application* Izuma::CreateApplication()
{
    return new IzumaGame;
}