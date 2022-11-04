/*
 * main.cpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#include "../base.hpp"
using namespace atto;

/** ---------------------------------------------------------------------------
 * main
 */
int main(int argc, char const *argv[])
{
    /*
     * Create an OpenCL context with all devices of the specified type in the
     * first available platform.
     */
    cl_context context = cl::Context::create(CL_DEVICE_TYPE_ALL);
    for (auto &it : cl::Context::get_devices(context)) {
        std::cout << cl::Device::get_info_string(it) << "\n";
    }

    /* Release the context before exit. */
    cl::Context::release(context);

    exit(EXIT_SUCCESS);
}