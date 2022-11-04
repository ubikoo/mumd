/*
 * event.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef ATTO_OPENCL_EVENT_H_
#define ATTO_OPENCL_EVENT_H_

#include <vector>
#include "atto/opencl/base.hpp"
#include "atto/opencl/context.hpp"

namespace atto {
namespace cl {
namespace Event {

/**
 * create
 * @brief Creates a NULL initialized event.
 */
cl_event create(void);

/**
 * create_user_event
 * @brief Wrapper for clCreateUserEvent. Creates a user event object.
 */
cl_event create_user_event(const cl_context &context);

/**
 * release
 * @brief Wrapper for clReleaseEvent. Decrements the event reference count.
 */
cl_int release(const cl_event &event);

/**
 * wait_for_events
 * @brief Wrapper for clWaitForEvents. Waits on the host thread for commands
 * identified by event objects to complete.
 */
cl_int wait_for_events(const std::vector<cl_event> &events);

/**
 * wait_for_event
 * @brief Wrapper for clWaitForEvents. Waits on the host thread for commands
 * identified by the event object to complete.
 */
cl_int wait_for_event(const cl_event &event);

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
  	void *user_data = NULL);

/**
 * get_command_time_start
 * @brief Wrapper for clGetEventProfilingInfo. Return a 64-bit value describing
 * the current device time counter in nanoseconds when the command identified
 * by event starts execution on the device.
 */
cl_ulong get_command_start(const cl_event &event);

/**
 * get_command_end
 * @brief Wrapper for clGetEventProfilingInfo. Return a 64-bit value describing
 * the current device time counter in nanoseconds when the command identified
 * by event finished execution on the device.
 */
cl_ulong get_command_end(const cl_event &event);

} /* Event */
} /* cl */
} /* atto */

#endif /* ATTO_OPENCL_EVENT_H_ */
