// Created by Jesse on 12/23/2023.
#include "Izuma.h"

class IzumaEditor : public Izuma::Application
{
public:
    IzumaEditor()
    {
        IZ_LOG_INFO("Initialized");
    }
};

Izuma::Application* Izuma::CreateApplication()
{
    return new IzumaEditor;
}