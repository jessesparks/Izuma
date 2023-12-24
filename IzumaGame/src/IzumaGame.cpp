// Created by Jesse on 12/23/2023.
#include "Izuma.h"

int main()
{
    IZ_LOG_CORE_FATAL("Test Error: %f", 226.0f);
    IZ_ASSERT(false, "ASSERTION");

    return 0;
}