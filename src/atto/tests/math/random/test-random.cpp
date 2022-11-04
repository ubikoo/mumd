/*
 * test-rng.cpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#include "Catch2/catch.hpp"

#include "../common.hpp"
using namespace atto;


/* ---- Test Kiss Random Number Engine ----------------------------------------
 */
TEST_CASE("Kiss-Random-Number-Engine") {
    /*
     * Create an array of kiss rng engines.
     */
    std::vector<math::rng::Kiss> engine;
    core_pragma_omp(parallel default(none) shared(engine))
    {
        core_pragma_omp(master)
        {
            engine.resize(omp_get_num_threads());
            for (auto &e : engine) { e.init(); }
        }
    } /* omp parallel */

    /*
     * 32-bit random number engine
     */
    SECTION("32-bit-random-number-engine") {
        std::vector<uint32_t> samples32(numsamples);
        for (size_t ir = 0; ir < numruns; ++ir) {
            /*
             * Generate the sample data
             */
            std::fill(samples32.begin(), samples32.end(), 0.0);

            core_pragma_omp(parallel default(none) shared(samples32, engine))
            {
                int tid = omp_get_thread_num();
                core_pragma_omp(for schedule(static))
                for (size_t i = 0; i < samples32.size(); ++i) {
                    samples32[i] = engine[tid].rand32();
                }
            } /* omp parallel */

            /*
             * Write the data to the output file
             */
            core::FileOut fp(
                std::string(TestPrefix + "/out.kiss32." + std::to_string(ir)),
                core::FileOut::Binary);
            fp.write(samples32.data(), samples32.size()*sizeof(uint32_t));
            REQUIRE(!fp.is_error());
            fp.close();
        }

        /*
         * 32-bit long run
         */
        {
            samples32.resize(longsamples);
            std::fill(samples32.begin(), samples32.end(), 0.0);

            core_pragma_omp(parallel default(none) shared(samples32, engine))
            {
                int tid = omp_get_thread_num();
                core_pragma_omp(for schedule(static))
                for (size_t i = 0; i < samples32.size(); ++i) {
                    samples32[i] = engine[tid].rand32();
                }
            } /* omp parallel */

            core::FileOut fp(
                std::string(TestPrefix + "/out.kiss32.long"),
                core::FileOut::Binary);
            fp.write(samples32.data(), samples32.size()*sizeof(uint32_t));
            REQUIRE(!fp.is_error());
            fp.close();
        }
    }

    /*
     * 64-bit random number engine
     */
    SECTION("64-bit-random-number-engine") {
        std::vector<uint64_t> samples64(numsamples);
        for (size_t ir = 0; ir < numruns; ++ir) {
            /*
             * Generate the sample data
             */
            std::fill(samples64.begin(), samples64.end(), 0.0);

            core_pragma_omp(parallel default(none) shared(samples64, engine))
            {
                int tid = omp_get_thread_num();
                core_pragma_omp(for schedule(static))
                for (size_t i = 0; i < samples64.size(); ++i) {
                    samples64[i] = engine[tid].rand64();
                }
            } /* omp parallel */

            /*
             * Write the data to the output file
             */
            core::FileOut fp(
                std::string(TestPrefix + "/out.kiss64." + std::to_string(ir)),
                core::FileOut::Binary);
            fp.write(samples64.data(), samples64.size()*sizeof(uint64_t));
            REQUIRE(!fp.is_error());
            fp.close();
        }

        /*
         * 64-bit long run
         */
        {
            samples64.resize(longsamples);
            std::fill(samples64.begin(), samples64.end(), 0.0);

            core_pragma_omp(parallel default(none) shared(samples64, engine))
            {
                int tid = omp_get_thread_num();
                core_pragma_omp(for schedule(static))
                for (size_t i = 0; i < samples64.size(); ++i) {
                    samples64[i] = engine[tid].rand64();
                }
            } /* omp parallel */

            core::FileOut fp(
                std::string(TestPrefix + "/out.kiss64.long"),
                core::FileOut::Binary);
            fp.write(samples64.data(), samples64.size()*sizeof(uint64_t));
            REQUIRE(!fp.is_error());
            fp.close();
        }
    }
}


/* ---- Test Cmwc Random Number Engine ----------------------------------------
 */
TEST_CASE("Cmwc-Random-Number-Engine") {
    /*
     * Create an array of cmwc rng engines.
     */
    std::vector<math::rng::Cmwc> engine;
    core_pragma_omp(parallel default(none) shared(engine))
    {
        core_pragma_omp(master)
        {
            engine.resize(omp_get_num_threads());
            for (auto &e : engine) { e.init(); }
        }
    } /* omp parallel */

    /*
     * 32-bit random number engine
     */
    SECTION("32-bit-random-number-engine") {
        std::vector<uint32_t> samples32(numsamples);
        for (size_t ir = 0; ir < numruns; ++ir) {
            /*
             * Generate the sample data
             */
            std::fill(samples32.begin(), samples32.end(), 0.0);

            core_pragma_omp(parallel default(none) shared(samples32, engine))
            {
                int tid = omp_get_thread_num();
                core_pragma_omp(for schedule(static))
                for (size_t i = 0; i < samples32.size(); ++i) {
                    samples32[i] = engine[tid].rand32();
                }
            } /* omp parallel */

            /*
             * Write the data to the output file
             */
            core::FileOut fp(
                std::string(TestPrefix + "/out.cmwc32." + std::to_string(ir)),
                core::FileOut::Binary);
            fp.write(samples32.data(), samples32.size()*sizeof(uint32_t));
            REQUIRE(!fp.is_error());
            fp.close();
        }

        /*
         * 32-bit long run
         */
        {
            samples32.resize(longsamples);
            std::fill(samples32.begin(), samples32.end(), 0.0);

            core_pragma_omp(parallel default(none) shared(samples32, engine))
            {
                int tid = omp_get_thread_num();
                core_pragma_omp(for schedule(static))
                for (size_t i = 0; i < samples32.size(); ++i) {
                    samples32[i] = engine[tid].rand32();
                }
            } /* omp parallel */

            core::FileOut fp(
                std::string(TestPrefix + "/out.cmwc32.long"),
                core::FileOut::Binary);
            fp.write(samples32.data(), samples32.size()*sizeof(uint32_t));
            REQUIRE(!fp.is_error());
            fp.close();
        }
    }

    /*
     * 64-bit random number engine
     */
    SECTION("64-bit-random-number-engine") {
        std::vector<uint64_t> samples64(numsamples);
        for (size_t ir = 0; ir < numruns; ++ir) {
            /*
             * Generate the sample data
             */
            std::fill(samples64.begin(), samples64.end(), 0.0);

            core_pragma_omp(parallel default(none) shared(samples64, engine))
            {
                int tid = omp_get_thread_num();
                core_pragma_omp(for schedule(static))
                for (size_t i = 0; i < samples64.size(); ++i) {
                    samples64[i] = engine[tid].rand64();
                }
            } /* omp parallel */

            /*
             * Write the data to the output file
             */
            core::FileOut fp(
                std::string(TestPrefix + "/out.cmwc64." + std::to_string(ir)),
                core::FileOut::Binary);
            fp.write(samples64.data(), samples64.size()*sizeof(uint64_t));
            REQUIRE(!fp.is_error());
            fp.close();
        }

        /*
         * 64-bit long run
         */
        {
            samples64.resize(longsamples);
            std::fill(samples64.begin(), samples64.end(), 0.0);

            core_pragma_omp(parallel default(none) shared(samples64, engine))
            {
                int tid = omp_get_thread_num();
                core_pragma_omp(for schedule(static))
                for (size_t i = 0; i < samples64.size(); ++i) {
                    samples64[i] = engine[tid].rand64();
                }
            } /* omp parallel */

            core::FileOut fp(
                std::string(TestPrefix + "/out.cmwc64.long"),
                core::FileOut::Binary);
            fp.write(samples64.data(), samples64.size()*sizeof(uint64_t));
            REQUIRE(!fp.is_error());
            fp.close();
        }
    }
}
