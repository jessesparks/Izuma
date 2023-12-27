// Created by Jesse on 12/26/2023.
#pragma once
#include "Application.h"

int main()
{
    auto application = Izuma::CreateApplication();
    application->Run();
    delete application;

    return 0;
}