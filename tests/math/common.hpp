/*
 * common.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef TEST_CORE_MATH_COMMON_H_
#define TEST_CORE_MATH_COMMON_H_

#include "atto/math/math.hpp"


/* Test Rng constants */
static const size_t numruns = 256;
static const size_t numsamples = 1048576;
static const size_t longsamples = 16777216;

/* Random bias used in geometry tests */
static const double rand_avg = 100.0;
static const double rand_sdev = 0.01;

/* Set the output format of type T */
static const std::string TestPrefix("/tmp");

#endif /* TEST_CORE_MATH_COMMON_H_ */
