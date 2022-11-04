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
     * Get a list of available platforms.
     */
    std::vector<cl_platform_id> platforms = cl::Platform::get_platform_ids();
    for (auto &it : platforms) {
        std::cout << cl::Platform::get_info_string(it) << "\n";
    }

    /*
     * Get a list of devices on the first available platform.
     */
    std::vector<cl_device_id> devices = cl::Device::get_device_ids(
        platforms[0], CL_DEVICE_TYPE_ALL);
    for (auto &it : devices) {
        std::cout << cl::Device::get_info_string(it) << "\n";
    }

    /*
     * Release the list of devices before closing.
     */
    for (auto &it : devices) {
        cl::Device::release(it);
    }

    exit(EXIT_SUCCESS);
}