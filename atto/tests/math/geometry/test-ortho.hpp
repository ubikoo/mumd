/*
 * test-ortho.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef TEST_CORE_GEOMETRY_ORTHO_H_
#define TEST_CORE_GEOMETRY_ORTHO_H_

#include "../common.hpp"

/**
 * check_ortho
 * @brief ortho test client.
 */
template<typename Type>
core_inline
bool check_ortho(const atto::math::Ortho<Type> &ortho)
{
    using namespace atto;

    /*
     * Check norm
     */
    Type norm_u = std::fabs(math::norm(ortho.u));
    if (!math::isequal(norm_u, (Type) 1)) {
        std::cerr << core::str_format("norm_u %lf\n", norm_u) << "\n";
        return false;
    }

    Type norm_v = std::fabs(math::norm(ortho.v));
    if (!math::isequal(norm_v, (Type) 1)) {
        std::cerr << core::str_format("norm_v %lf\n", norm_v) << "\n";
        return false;
    }

    Type norm_w = std::fabs(math::norm(ortho.w));
    if (!math::isequal(norm_w, (Type) 1)) {
        std::cerr << core::str_format("norm_w %lf\n", norm_w) << "\n";
        return false;
    }

    /*
     * Check orthogonality
     */
    Type dot_uv = std::fabs(math::dot(ortho.u, ortho.v));
    if (!math::isequal(dot_uv, (Type) 0)) {
        std::cerr << core::str_format("dot_uv %lf\n", dot_uv) << "\n";
        return false;
    }

    Type dot_uw = std::fabs(math::dot(ortho.u, ortho.w));
    if (!math::isequal(dot_uw, (Type) 0)) {
        std::cerr << core::str_format("dot_uw %lf\n", dot_uw) << "\n";
        return false;
    }

    Type dot_vw = std::fabs(math::dot(ortho.v, ortho.w));
    if (!math::isequal(dot_vw, (Type) 0)) {
        std::cerr << core::str_format("dot_vw %lf\n", dot_vw) << "\n";
        return false;
    }

    /*
     * Check cross products
     */
    Type dot_wuv = math::dot(ortho.w, cross(ortho.u, ortho.v));
    if (!math::isequal(dot_wuv, (Type) 1)) {
        std::cerr << core::str_format("dot_wuv %lf\n", dot_wuv) << "\n";
        return false;
    }

    Type dot_uvw = math::dot(ortho.u, cross(ortho.v, ortho.w));
    if (!math::isequal(dot_uvw, (Type) 1)) {
        std::cerr << core::str_format("dot_uvw %lf\n", dot_uvw) << "\n";
        return false;
    }

    Type dot_vwu = math::dot(ortho.v, cross(ortho.w, ortho.u));
    if (!math::isequal(dot_vwu, (Type) 1)) {
        std::cerr << core::str_format("dot_vwu %lf\n", dot_vwu) << "\n";
        return false;
    }

    return true;
}

template<typename Type>
core_inline
bool check_isnan(const atto::math::vec3<Type> &v)
{
    using namespace atto;

    if (std::isnan(v(0)) || std::isnan(v(1)) || std::isnan(v(2))) {
        std::cerr << "isnan " << v << "\n";
        return false;
    }
    return true;
}

/*
 * test_ortho
 */
template<typename Type>
core_inline
void test_ortho(const size_t n_items)
{
    using namespace atto;

    /*
     * Create an array of vectors and orthonormal axis
     */
    std::vector<math::vec3<Type>> arr_u(n_items);
    std::vector<math::vec3<Type>> arr_v(n_items);
    std::vector<math::vec3<Type>> arr_w(n_items);
    std::vector<math::Ortho<Type>> arr_o(n_items);

    /*
     * Create an array of random number generators
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
     * Test orthonormal operators
     */
    core_pragma_omp(parallel \
        default(none) \
        shared(n_items, arr_u, arr_v, arr_w, arr_o, engine, std::cerr))
    {
        size_t tid = omp_get_thread_num();
        math::rng::gauss<Type> rand;

        core_pragma_omp(for schedule(static))
        for (size_t ix = 0; ix < n_items; ++ix) {
            arr_u[ix] = math::vec3<Type>(
                rand(engine[tid], rand_sdev, rand_avg),
                rand(engine[tid], rand_sdev, (Type) 0),
                rand(engine[tid], rand_sdev, (Type) 0));

            arr_v[ix] = math::vec3<Type>(
                rand(engine[tid], rand_sdev, (Type) 0),
                rand(engine[tid], rand_sdev, rand_avg),
                rand(engine[tid], rand_sdev, (Type) 0));

            arr_w[ix] = math::vec3<Type>(
                rand(engine[tid], rand_sdev, (Type) 0),
                rand(engine[tid], rand_sdev, (Type) 0),
                rand(engine[tid], rand_sdev, rand_avg));

            /* Check vector samples */
            REQUIRE(check_isnan(arr_u[ix]));
            REQUIRE(check_isnan(arr_v[ix]));
            REQUIRE(check_isnan(arr_w[ix]));

            /*
             * create_from_u
             */
            arr_o[ix] = math::Ortho<Type>::create_from_u(arr_u[ix]);
            if (!check_ortho(arr_o[ix])) {
                std::cerr << "create_from_u\n";
                std::cerr << to_string(arr_o[ix], "%lf") << "\n";
                std::cerr << to_string(arr_u[ix], "%lf") << "\n";
            }
            REQUIRE(check_ortho(arr_o[ix]));

            /*
             * create_from_v
             */
            arr_o[ix] = math::Ortho<Type>::create_from_v(arr_v[ix]);
            if (!check_ortho(arr_o[ix])) {
                std::cerr << "create_from_v\n";
                std::cerr << to_string(arr_o[ix], "%lf") << "\n";
                std::cerr << to_string(arr_v[ix], "%lf") << "\n";
            }
            REQUIRE(check_ortho(arr_o[ix]));

            /*
             * create_from_w
             */
            arr_o[ix] = math::Ortho<Type>::create_from_w(arr_w[ix]);
            if (!check_ortho(arr_o[ix])) {
                std::cerr << "create_from_w\n";
                std::cerr << to_string(arr_o[ix], "%lf") << "\n";
                std::cerr << to_string(arr_w[ix], "%lf") << "\n";
            }
            REQUIRE(check_ortho(arr_o[ix]));

            /*
             * create_from_uv
             */
            arr_o[ix] = math::Ortho<Type>::create_from_uv(arr_u[ix], arr_v[ix]);
            if (!check_ortho(arr_o[ix])) {
                std::cerr << "create_from_uv\n";
                std::cerr << to_string(arr_o[ix], "%lf") << "\n";
            }
            REQUIRE(check_ortho(arr_o[ix]));

            /*
             * create_from_vu
             */
            arr_o[ix] = math::Ortho<Type>::create_from_vu(arr_v[ix], arr_u[ix]);
            if (!check_ortho(arr_o[ix])) {
                std::cerr << "create_from_vu\n";
                std::cerr << to_string(arr_o[ix], "%lf") << "\n";
            }
            REQUIRE(check_ortho(arr_o[ix]));

            /*
             * create_from_vw
             */
            arr_o[ix] = math::Ortho<Type>::create_from_vw(arr_v[ix], arr_w[ix]);
            if (!check_ortho(arr_o[ix])) {
                std::cerr << "create_from_vw\n";
                std::cerr << to_string(arr_o[ix], "%lf") << "\n";
            }
            REQUIRE(check_ortho(arr_o[ix]));

            /*
             * create_from_wv
             */
            arr_o[ix] = math::Ortho<Type>::create_from_wv(arr_w[ix], arr_v[ix]);
            if (!check_ortho(arr_o[ix])) {
                std::cerr << "create_from_wv\n";
                std::cerr << to_string(arr_o[ix], "%lf") << "\n";
            }
            REQUIRE(check_ortho(arr_o[ix]));

            /*
             * create_from_wu
             */
            arr_o[ix] = math::Ortho<Type>::create_from_wu(arr_w[ix], arr_u[ix]);
            if (!check_ortho(arr_o[ix])) {
                std::cerr << "create_from_wu\n";
                std::cerr << to_string(arr_o[ix], "%lf") << "\n";
            }
            REQUIRE(check_ortho(arr_o[ix]));

            /*
             * create_from_uw
             */
            arr_o[ix] = math::Ortho<Type>::create_from_uw(arr_u[ix], arr_w[ix]);
            if (!check_ortho(arr_o[ix])) {
                std::cerr << "create_from_uw\n";
                std::cerr << to_string(arr_o[ix], "%lf") << "\n";
            }
            REQUIRE(check_ortho(arr_o[ix]));

            /*
             * world_to_local and local_to_world
             */
            math::vec3<Type> a(
                rand(engine[tid], rand_sdev, rand_avg),
                rand(engine[tid], rand_sdev, rand_avg),
                rand(engine[tid], rand_sdev, rand_avg));
            math::vec3<Type> b = arr_o[ix].world_to_local(a);
            math::vec3<Type> e = a - arr_o[ix].local_to_world(b);

            if (!math::isequal(norm(e), (Type) 0)) {
                std::cerr << "world_to_local -> local_to_world\n";
                std::cerr << "o:\n" << to_string(arr_o[ix], "%lf") << "\n";
                std::cerr << "a: " << to_string(a, "%lf") << "\n";
                std::cerr << "b: " << to_string(b, "%lf") << "\n";
                std::cerr << "e: " << to_string(e, "%lf") << "\n";
                std::cerr << "norm e: " << core::str_format("%lf", norm(e)) << "\n";
                std::cerr << "" << "\n";
            }
            REQUIRE(math::isequal(norm(e), (Type) 0));
        }
    }
}

#endif /* TEST_CORE_GEOMETRY_ORTHO_H_ */
