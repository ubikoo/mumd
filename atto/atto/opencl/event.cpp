/*
 * event.cpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#include "atto/opencl/event.hpp"

namespace atto {
namespace cl {
namespace Event {

/**
 * create
 * @brief Creates a NULL initialized event.
 */
cl_event create(void)
{
    cl_event event = NULL;
    return event;
}

/**
 * create_user_event
 * @brief Wrapper for clCreateUserEvent. Creates a user event object.
 */
cl_event create_user_event(const cl_context &context)
{
    cl_int err;
    cl_event event = clCreateUserEvent(context, &err);
    core_assert(err == CL_SUCCESS, "clCreateUserEvent");
    return event;
}

/**
 * release
 * @brief Wrapper for clReleaseEvent. Decrements the event reference count.
 */
cl_int release(const cl_event &event)
{
    cl_int err = CL_INVALID_VALUE;
    if (event != NULL) {
        err = clReleaseEvent(event);
        core_assert(err == CL_SUCCESS, "clReleaseEvent");
    }
    return err;
}

/**
 * wait_for_events
 * @brief Wrapper for clWaitForEvents. Waits on the host thread for commands
 * identified by event objects to complete.
 */
cl_int wait_for_events(const std::vector<cl_event> &events)
{
    cl_int err = clWaitForEvents(
        (cl_uint) events.size(),
        (!events.empty()) ? events.data() : NULL);
    core_assert(err == CL_SUCCESS, "clWaitForEvents");
    return err;
}

/**
 * wait_for_event
 * @brief Wrapper for clWaitForEvents. Waits on the host thread for commands
 * identified by the event object to complete.
 */
cl_int wait_for_event(const cl_event &event)
{
    cl_int err = clWaitForEvents(1, &event);
    core_assert(err == CL_SUCCESS, "clWaitForEvents");
    return err;
}

/**
 * set_event_callback
 * @brief Wrapper for clSetEventCallback. Registers a user callback function
 * for a specific command execution status.
 */
cl_int set_event_callback(
    const cl_event &event,
    cl_int command_exec_callback_type,
  	void (CL_CALLBACK *pfn_event_notify) (
        cl_event event,
        cl_int event_command_exec_status,
        void *user_data),
  	void *user_data)
{
    cl_int err = clSetEventCallback(
        event,
        command_exec_callback_type,
        pfn_event_notify,
        user_data);
    core_assert(err == CL_SUCCESS, "clSetEventCallback");
    return err;
}

/**
 * get_command_time_start
 * @brief Wrapper for clGetEventProfilingInfo. Return a 64-bit value describing
 * the current device time counter in nanoseconds when the command identified
 * by event starts execution on the device.
 */
cl_ulong get_command_start(const cl_event &event)
{
    cl_ulong time_start;
    cl_int err = clGetEventProfilingInfo(
        event,
        CL_PROFILING_COMMAND_START,
        sizeof(time_start),
        &time_start,
        NULL);
    core_assert(err == CL_SUCCESS, "clGetEventProfilingInfo");
    return time_start;
}

/**
 * get_command_end
 * @brief Wrapper for clGetEventProfilingInfo. Return a 64-bit value describing
 * the current device time counter in nanoseconds when the command identified
 * by event finished execution on the device.
 */
cl_ulong get_command_end(const cl_event &event)
{
    cl_ulong time_end;
    cl_int err = clGetEventProfilingInfo(
        event,
        CL_PROFILING_COMMAND_END,
        sizeof(time_end),
        &time_end,
        NULL);
    core_assert(err == CL_SUCCESS, "clGetEventProfilingInfo");
    return time_end;
}

} /* Event */
} /* cl */
} /* atto */
