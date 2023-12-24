// Created by Jesse on 12/23/2023.
#include "Izuma.h"

int main()
{
    IZ_LOG_TRACE("Test Trace: %d", 312);
    IZ_LOG_DEBUG("Test Debug: %s", "true");
    IZ_LOG_INFO("Test Info: %f", 2.6f);
    IZ_LOG_WARN("Test Warn: %i", 26);
    IZ_LOG_ERROR("Test Error: %s", "ERROR");
    IZ_LOG_FATAL("Test Fatal: %f", 226.0f);
    IZ_ASSERT(false, "ASSERTION");

    return 0;
}