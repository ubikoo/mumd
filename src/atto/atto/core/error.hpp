/*
 * error.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef ATTO_CORE_ERROR_H_
#define ATTO_CORE_ERROR_H_

#include <iostream>
#include <sstream>
#include <exception>

/** ---- Error debug, throw and assert macros ---------------------------------
 * @brief These macros encapsulate debug information about a runtime error -
 * current file, current line number, name of the called function and error
 * message.
 * Alternatively, inherit from std::runtime class and create a custom
 * exception handler.
 * @note Enclose the compound statement inside a do {...} while(0)
 * to swallow the semicolon and the macro in one statement.
 * @see https://en.cppreference.com/w/cpp/error/exception
 *      https://en.cppreference.com/w/cpp/error/runtime_error
 *      https://stackoverflow.com/questions/8152720
 *      https://stackoverflow.com/questions/1569726
 */
#ifndef core_debug
#define core_debug(msg) \
do { \
    std::ostringstream ss; \
    ss << "\n" << __FILE__ \
        << ":" <<  __LINE__ \
        << ": " << (msg); \
    std::cerr << ss.str() << "\n"; \
} while(0)
#endif /* core_debug */

#ifndef core_throw
#define core_throw(msg) \
do { \
    std::ostringstream ss; \
    ss << "\n" << __FILE__ \
        << ":" <<  __LINE__ \
        << ": " << (msg); \
    throw std::runtime_error(ss.str()); \
} while(0)
#endif /* core_throw */

#ifndef core_assert
#define core_assert(expr,msg) \
do { \
    if (!(expr)) { \
        std::ostringstream ss; \
        ss << "\n" << __FILE__ \
           << ":" <<  __LINE__ \
           << ": " << (msg); \
        throw std::runtime_error(ss.str()); \
    } \
} while(0)
#endif /* core_assert */

#endif /* ATTO_CORE_ERROR_H_ */
