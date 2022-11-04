/*
 * ortho.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef ATTO_MATH_ORTHO_H_
#define ATTO_MATH_ORTHO_H_

#include "atto/math/geometry/vec3.hpp"
#include "atto/math/geometry/mat3.hpp"
#include "atto/math/geometry/algebra.hpp"
#include "atto/math/geometry/io.hpp"

namespace atto {
namespace math {

/** ---------------------------------------------------------------------------
 * Ortho
 * @brief Ortho is a plain class representing an orthonormal basis.
 */
template<typename Type>
struct Ortho {
    static_assert(std::is_floating_point<Type>::value, "non floating point");

    /* Orthonormal basis vectors. */
    vec3<Type> u;//{(Type) 1.0, (Type) 0.0, (Type) 0.0};
    vec3<Type> v;//{(Type) 0.0, (Type) 1.0, (Type) 0.0};
    vec3<Type> w;//{(Type) 0.0, (Type) 0.0, (Type) 1.0};

    /* Create an orthonormal basis from a single fixed vector. */
    static Ortho<Type> create_from_u(const vec3<Type> &e_u);
    static Ortho<Type> create_from_v(const vec3<Type> &e_v);
    static Ortho<Type> create_from_w(const vec3<Type> &e_w);

    /*
     * Create an orthonormal basis from two vectors. The first vector is fixed,
     * the second is projected in a direction orthogonal to the first vector.
     */
    static Ortho<Type> create_from_uv(const vec3<Type> &e_u, const vec3<Type> &e_v);
    static Ortho<Type> create_from_vu(const vec3<Type> &e_v, const vec3<Type> &e_u);

    static Ortho<Type> create_from_vw(const vec3<Type> &e_v, const vec3<Type> &e_w);
    static Ortho<Type> create_from_wv(const vec3<Type> &e_w, const vec3<Type> &e_v);

    static Ortho<Type> create_from_wu(const vec3<Type> &e_w, const vec3<Type> &e_u);
    static Ortho<Type> create_from_uw(const vec3<Type> &e_u, const vec3<Type> &e_w);

    /* Return the projection of the vector from world to local space. */
    vec3<Type> world_to_local(const vec3<Type> &a) const;

    /* Return the projection of the vector from local to world space. */
    vec3<Type> local_to_world(const vec3<Type> &a) const;
};

/** ---- Geometry operators ---------------------------------------------------
 * Ortho<Type>::create_from_u
 * @brief Create an orthonormal basis set given the fixed vector u.
 */
template<typename Type>
core_inline
Ortho<Type> Ortho<Type>::create_from_u(const vec3<Type> &e_u)
{
    static constexpr Type eps = std::sqrt(std::numeric_limits<Type>::epsilon());
    static const vec3<Type> e_1{1.0, 0.0, 0.0};
    static const vec3<Type> e_2{0.0, 1.0, 0.0};

    Ortho<Type> o;
    o.u = normalize(e_u);               /* u = u / |u| */
    o.v = cross(o.u, e_1);              /* v = u x e_1 */
    if (norm(o.v) < eps) {
        o.v = cross(o.u, e_2);          /* v = u x e_2 */
    }
    o.v = normalize(o.v);
    o.w = normalize(cross(o.u, o.v));   /* w = <u x v> */
    return o;
}

/**
 * Ortho<Type>::create_from_v
 * @brief Create an orthonormal basis set given the fixed vector v.
 */
template<typename Type>
core_inline
Ortho<Type> Ortho<Type>::create_from_v(const vec3<Type> &e_v)
{
    static constexpr Type eps = std::sqrt(std::numeric_limits<Type>::epsilon());
    static const vec3<Type> e_1{1.0, 0.0, 0.0};
    static const vec3<Type> e_2{0.0, 1.0, 0.0};

    Ortho<Type> o;
    o.v = normalize(e_v);               /* v = v / |v| */
    o.w = cross(o.v, e_1);              /* w = v x e_1 */
    if (norm(o.w) < eps) {
        o.w = cross(o.v, e_2);          /* w = v x e_2 */
    }
    o.w = normalize(o.w);
    o.u = normalize(cross(o.v, o.w));   /* u = <v x w> */
    return o;
}

/**
 * Ortho<Type>::create_from_w
 * @brief Create an orthonormal basis set given the fixed vector w.
 */
template<typename Type>
core_inline
Ortho<Type> Ortho<Type>::create_from_w(const vec3<Type> &e_w)
{
    static constexpr Type eps = std::sqrt(std::numeric_limits<Type>::epsilon());
    static const vec3<Type> e_1{1.0, 0.0, 0.0};
    static const vec3<Type> e_2{0.0, 1.0, 0.0};

    Ortho<Type> o;
    o.w = normalize(e_w);               /* w = w / |w| */
    o.u = cross(o.w, e_1);              /* u = w x e_1 */
    if (norm(o.u) < eps) {
        o.u = cross(o.w, e_2);          /* u = w x e_2 */
    }
    o.u = normalize(o.u);
    o.v = normalize(cross(o.w, o.u));   /* v = <w x u> */
    return o;
}

/** ---------------------------------------------------------------------------
 * Ortho<Type>::create_from_uv
 * @brief Create an orthonormal basis set from two vectors:
 *  u is fixed.
 *  v is orthogonalized using a Gram-Schmidt process.
 */
template<typename Type>
core_inline
Ortho<Type> Ortho<Type>::create_from_uv(const vec3<Type> &e_u, const vec3<Type> &e_v)
{
    Ortho<Type> o;
    o.u = normalize(e_u);                           /* u = u / |u| */
    o.v = normalize(e_v - (dot(e_v, o.u) * o.u));   /* v = <v - (v.u)*u> */
    o.w = normalize(cross(o.u, o.v));               /* w = u x v */
    return o;
}

/**
 * Ortho<Type>::create_from_vu
 * @brief Create an orthonormal basis set from two vectors:
 *  v is fixed.
 *  u is orthogonalized using a Gram-Schmidt process.
 */
template<typename Type>
core_inline
Ortho<Type> Ortho<Type>::create_from_vu(const vec3<Type> &e_v, const vec3<Type> &e_u)
{
    Ortho<Type> o;
    o.v = normalize(e_v);                           /* v = v / |v| */
    o.u = normalize(e_u - (dot(e_u, o.v) * o.v));   /* u = <u - (u.v)*v> */
    o.w = normalize(cross(o.u, o.v));               /* w = u x v */
    return o;
}

/** ---------------------------------------------------------------------------
 * Ortho<Type>::create_from_vw
 * @brief Create an orthonormal basis set from two vectors:
 *  v is fixed.
 *  w is orthogonalized using a Gram-Schmidt process.
 */
template<typename Type>
core_inline
Ortho<Type> Ortho<Type>::create_from_vw(const vec3<Type> &e_v, const vec3<Type> &e_w)
{
    Ortho<Type> o;
    o.v = normalize(e_v);                           /* v = v / |v| */
    o.w = normalize(e_w - (dot(e_w, o.v) * o.v));   /* w = <w - (w.v)*v> */
    o.u = normalize(cross(o.v, o.w));               /* u = <v x w> */
    return o;
}

/**
 * Ortho<Type>::create_from_wv
 * @brief Create an orthonormal basis set from two vectors:
 *  w is fixed.
 *  v is orthogonalized using a Gram-Schmidt process.
 */
template<typename Type>
core_inline
Ortho<Type> Ortho<Type>::create_from_wv(const vec3<Type> &e_w, const vec3<Type> &e_v)
{
    Ortho<Type> o;
    o.w = normalize(e_w);                           /* w = w / |w| */
    o.v = normalize(e_v - (dot(e_v, o.w) * o.w));   /* v = <v - (v.w)*w> */
    o.u = normalize(cross(o.v, o.w));               /* u = <v x w> */
    return o;
}

/** ---------------------------------------------------------------------------
 * Ortho<Type>::create_from_wu
 * @brief Create an orthonormal basis set from two vectors:
 *  w is fixed.
 *  u is orthogonalized using a Gram-Schmidt process.
 */
template<typename Type>
core_inline
Ortho<Type> Ortho<Type>::create_from_wu(const vec3<Type> &e_w, const vec3<Type> &e_u)
{
    Ortho<Type> o;
    o.w = normalize(e_w);                           /* w = w / |w| */
    o.u = normalize(e_u - (dot(e_u, o.w) * o.w));   /* u = <u - (u.w)*w> */
    o.v = normalize(cross(o.w, o.u));               /* v = <w x u> */
    return o;
}

/**
 * Ortho<Type>::create_from_uw
 * @brief Create an orthonormal basis set from two vectors:
 *  u is fixed.
 *  w is orthogonalized using a Gram-Schmidt process.
 */
template<typename Type>
core_inline
Ortho<Type> Ortho<Type>::create_from_uw(const vec3<Type> &e_u, const vec3<Type> &e_w)
{
    Ortho<Type> o;
    o.u = normalize(e_u);                           /* u = u / |u| */
    o.w = normalize(e_w - (dot(e_w, o.u) * o.u));   /* w = <w - (u.w)*u> */
    o.v = normalize(cross(o.w, o.u));               /* v = <w x u> */
    return o;
}

/** ---------------------------------------------------------------------------
 * Ortho<Type>::world_to_local
 * @brief Project a vector coordinates from world to local
 * reference frame defined by the orthonormal basis set.
 */
template<typename Type>
core_inline
vec3<Type> Ortho<Type>::world_to_local(const vec3<Type> &a) const
{
    return vec3<Type>{
        u.x * a.x + u.y * a.y + u.z * a.z,
        v.x * a.x + v.y * a.y + v.z * a.z,
        w.x * a.x + w.y * a.y + w.z * a.z
    };
}

/**
 * Ortho<Type>::local_to_world
 * @brief Project a vector coordinates from local to world
 * reference frame defined by the orthonormal basis set.
 */
template<typename Type>
core_inline
vec3<Type> Ortho<Type>::local_to_world(const vec3<Type> &a) const
{
    return vec3<Type>{
        u.x * a.x + v.x * a.y + w.x * a.z,
        u.y * a.x + v.y * a.y + w.y * a.z,
        u.z * a.x + v.z * a.y + w.z * a.z
    };
}

/** ---------------------------------------------------------------------------
 * @brief Write a string representation of the orthonormal vectors
 * using the appropriate format specifier.
 */
template<typename Type>
core_inline
std::string to_string(const Ortho<Type> &o, const char *format)
{
    std::ostringstream ss;
    ss << to_string(o.u, format) << "\n";
    ss << to_string(o.v, format) << "\n";
    ss << to_string(o.w, format) << "\n";
    return ss.str();
}

} /* math */
} /* atto */

#endif /* ATTO_MATH_ORTHO_H_ */
