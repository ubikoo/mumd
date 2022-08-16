/*
 * random.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef ATTO_MATH_RANDOM_H_
#define ATTO_MATH_RANDOM_H_

#include <cmath>
#include <cstring>
#include <vector>

namespace atto {
namespace math {
namespace rng {

/** ---- Base random number generator engine ----------------------------------
 * Engine
 * @brief Base random number generator engine. It implements a device random
 * number generator and a random prime number generator. Engine is a crtp base
 * class. The derived children are random number generator engines used as the
 * template parameter T of the Engine base class.
 *
 * Each derived random number generator engine implements the following:
 *  init     initialise the random number generator engine state.
 *  rand32   sample a 32-bit random number.
 *  rand64   sample a 64-bit random number.
 */
template<typename T>
struct Engine {
    /* Random number generator engine. */
    T *m_engine;

    /* Random number generator engine derived functions. */
    T *engine(void) { return m_engine; }
    void init(void) { engine()->init(); }
    uint32_t rand32 (void) { return engine()->rand32(); }
    uint64_t rand64 (void) { return engine()->rand64(); }

    /* Random number generator engine functions. */
    uint32_t randev(void);
    uint32_t randprime(void);
    bool isprime(const uint32_t n);
    uint32_t nearbyprime(const uint32_t num);

    /* Constructor/destructor. */
    Engine() { m_engine = static_cast<T *>(this); }
    ~Engine() = default;

    /* Copy constructor/assignment. */
    Engine(const Engine &other) = default;
    Engine &operator=(const Engine &other) = default;
};

/**
 * Engine<T>::randev
 * @brief Read a 32-bit random number from the random device.
 */
template<typename T>
uint32_t Engine<T>::randev(void)
{
    uint32_t number;

    core::FileIn fp;
    try {
        fp.open("/dev/urandom", core::FileIn::Binary);
        core_assert(fp.read(&number, sizeof(uint32_t)), "I/O error");
        fp.close();
    } catch (std::exception& e) {
        fp.close();
        core_throw(e.what());
    }

    return number;
}

/**
 * Engine<T>::randprime
 * @brief Compute the nearest prime smaller than a 32-bit random number.
 */
template<typename T>
uint32_t Engine<T>::randprime(void)
{
    return nearbyprime(randev());
}

/**
 * Engine<T>::isprime
 *
 * @brief Is the specified number n a prime number?
 * A prime number is a natural number greater than 1 that cannot be
 * formed by multiplying two smaller natural numbers.
 * Search for any divisor p < n. If none found then n is a prime.
 * We only need to test i < sqrt(n) (+1 to ensure correct counting).
 *
 * If n = p*q, then p ≤ sqrt(n) or q ≤ sqrt(n), otherwise p*q > n.
 * In the case where p = q, then
 *      n = p*p and p = sqrt(n).
 * In the case where p != q, then either
 *      p > sqrt(n) and q < sqrt(n)
 * or conversely
 *      p < sqrt(n) and q > sqrt(n)
 *
 * If there is no divisor p < sqrt(n) : n%p == 0, then n must be a prime.
 * Every even number larger than 2 is not prime, because 2 divides all even
 * numbers. If 2 does not divide n, then every other even number smaller
 * than n will also not divide n.
 */
template<typename T>
bool Engine<T>::isprime(const uint32_t num)
{
    /* If num is even, then it's not a prime */
    if (num%2 == 0) {
        return false;
    }

    uint32_t sqrt_num = (uint32_t) std::sqrt((double) num) + 1;
    for (uint32_t p = 3; p < sqrt_num; p += 2) {
        /* If p is a divisor, num is not prime */
        if (num%p == 0) {
            return false;
        }
    }

    /* Otherwise, num is necessarily prime */
    return true;
}

/**
 * Engine<T>::nearbyprime
 * @brief Find the nearest prime smaller or equal than n.
 */
template<typename T>
uint32_t Engine<T>::nearbyprime(const uint32_t num)
{
    /* Go over all the numbers smaller than n and test their primality. */
    for (uint32_t n = num; n >= 3; --n) {
        if (isprime(n)) {
            return n;
        }
    }

    /* If num = 1 or num = 2 or num = 3, then num is a prime. */
    return num;
}

/** ---- Kiss random number generator engine ----------------------------------
 * Kiss
 * @brief Random number generator engine based on the 64-bit variant of
 * George Marsaglia's KISS generator, and designed by David Jones.
 *
 * @see http://www.cs.ucl.ac.uk/staff/d.jones/GoodPracticeRNG.pdf
 */
struct Kiss : Engine<Kiss> {
    /*
     * Random number generator engine constants.
     * Linear congruential generator is defined by
     *  Xn+1 = (m1*Xn + m2) mod m, where m = 2^64-1.
     *
     * Multiply-with-carry generator 1
     *  Xn+1 = (m3*Xn + c) mod m, and c = Xn+1 >> 32
     *
     * Multiply-with-carry generator 2
     *  Xn+1 = (m4*Xn + c) mod m, and c = Xn+1 >> 32
     */
    const uint64_t m_m1 = 1490024343005336237ULL;
    const uint64_t m_m2 = 123456789ULL;
    const uint64_t m_m3 = 4294584393ULL;
    const uint64_t m_m4 = 4246477509ULL;

    const uint64_t m_seed_x  = 123456789123ULL;
    const uint64_t m_seed_y  = 987654321987ULL;
    const uint32_t m_seed_z1 = 43219876U;
    const uint32_t m_seed_c1 = 6543217U;
    const uint32_t m_seed_z2 = 21987643U;
    const uint32_t m_seed_c2 = 1732654U;
    const uint32_t m_seed_cmax = 698769069U;

    const uint32_t m_warmup_steps = 1024;

    /* Random number generator engine state. */
    uint64_t m_x;           /* linear congruential generator */
    uint64_t m_y;           /* xor-shift generator */
    uint32_t m_z1;          /* multiply-with-carry generator */
    uint32_t m_c1;
    uint32_t m_z2;          /* multiply-with-carry generator */
    uint32_t m_c2;

    /* Random number generator engine initialisation and sampling functions. */
    void init(void);
    uint32_t rand32 (void);
    uint64_t rand64 (void);

    /* Constructor/destructor. */
    Kiss(const bool do_init = false)
        : m_x(m_seed_x)
        , m_y(m_seed_y)
        , m_z1(m_seed_z1)
        , m_c1(m_seed_c1)
        , m_z2(m_seed_z2)
        , m_c2(m_seed_c2) {
        /* Initialize the random number generator engine */
        if (do_init) { init(); }
    }
    ~Kiss() = default;

    /* Copy constructor/assignment. */
    Kiss(const Kiss &other)
        : m_x(other.m_x)
        , m_y(other.m_y)
        , m_z1(other.m_z1)
        , m_c1(other.m_c1)
        , m_z2(other.m_z2)
        , m_c2(other.m_c2) {}
    Kiss &operator=(const Kiss &other) {
        if (this == &other) {
            return *this;
        }
        m_x  = other.m_x;
        m_y  = other.m_y;
        m_z1 = other.m_z1;
        m_c1 = other.m_c1;
        m_z2 = other.m_z2;
        m_c2 = other.m_c2;
        return *this;
    }
};

/**
 * Kiss::init
 * @brief Initialize the KISS random number generator engine.
 *
 * Four random seeds(x, y, z1, c2), are necessary for the 32-bit
 * generator and six(x, y, z1, c1, z2, c2) for the 64-bit generator.
 *      0 < x  < 2^32,       linear congruential generator
 *      0 < y  < 2^32,       xor-shift register generator
 *      0 < z1 < 2^32,       multiply-with-carry generator 1
 *      0 < c1 < 698769069
 *      0 < z2 < 2^32,       multiply-with-carry generator 2
 *      0 < c2 < 698769069
 * @note  Do not set y = 0 and avoid x = z1 = c1 = z2 = c2 = 0.
 */
core_inline
void Kiss::init(void)
{
    /* Seed the linear congruential generator state. */
    do {
        m_x = ((uint64_t) randprime() << 0  & 0x00000000FFFFFFFFULL) |
              ((uint64_t) randprime() << 32 & 0xFFFFFFFF00000000ULL);
    } while (m_x < 3);

    /* Seed the xor-shift register generator state. */
    do {
        m_y = ((uint64_t) randprime() << 0  & 0x00000000FFFFFFFFULL) |
              ((uint64_t) randprime() << 32 & 0xFFFFFFFF00000000ULL);
    } while (m_y < 3);

    /* Seed the first multiply-with-carry generator state. */
    do {
        m_z1 = randprime();
    } while (m_z1 < 3);

    do {
        m_c1 = randprime();
    } while (m_c1 < 3 || m_c1 >= m_seed_cmax);

    /* Seed the second multiply-with-carry generator state. */
    do {
        m_z2 = randprime();
    } while (m_z2 < 3);

    do {
        m_c2 = randprime();
    } while (m_c2 < 3 || m_c2 >= m_seed_cmax);

    /* Warmup the random number generator engine. */
    for (uint32_t i = 0; i < m_warmup_steps; ++i) {
        (void) rand64();    /* uint64_t core_unused(n) = rand64(); */
    }
}

/**
 * Kiss::rand32
 * @brief Long period 32-bit KISS random number generator engine.
 */
core_inline
uint32_t Kiss::rand32 (void)
{
    /* Linear congruential generator */
    m_x =  m_m1 * m_x + m_m2;

    /* Xorshift register generator - do not set y = 0*/
    m_y ^= m_y << 21;
    m_y ^= m_y >> 17;
    m_y ^= m_y << 30;

    /* First multiply-with-carry generator - avoid z1 = c1 = 0 */
    uint64_t t1 = m_m3 * m_z1 + m_c1;
    m_c1 = (uint32_t) (t1 >> 8) + (uint32_t) (t1 >> 24); /* or (t1 >> 32) */
    m_z1 = (uint32_t) (t1 >> 8);

    /*
     * Mix the multiply-with-carry values into a 32-bit word and return.
     */
    uint32_t m_z = (uint32_t) (m_z1 >> 8) + (uint32_t) (m_z1 >> 24);

    return ((uint32_t) (m_x >> 8) +
            (uint32_t) (m_x >> 24) +
            (uint32_t) (m_y) + m_z);
}

/**
 * Kiss::rand64
 * @brief Long period 64-bit KISS random number generator engine.
 */
core_inline
uint64_t Kiss::rand64 (void)
{
    /* Linear congruential generator */
    m_x =  m_m1 * m_x + m_m2;

    /* Xorshift register generator - do not set y = 0*/
    m_y ^= m_y << 21;
    m_y ^= m_y >> 17;
    m_y ^= m_y << 30;

    /* First multiply-with-carry generator - avoid z1 = c1 = 0 */
    uint64_t t1 = m_m3 * m_z1 + m_c1;
    m_c1 = (uint32_t) (t1 >> 8) + (uint32_t) (t1 >> 24); /* or (t1 >> 32) */
    m_z1 = (uint32_t) (t1 >> 8);

    /* Second multiply-with-carry generator - avoid z2 = c2 = 0 */
    uint64_t t2 = m_m4 * m_z2 + m_c2;
    m_c2 = (uint32_t) (t2 >> 8) + (uint32_t) (t2 >> 24); /* or (t2 >> 32) */
    m_z2 = (uint32_t) (t2 >> 8);

    /*
     * Mix the multiply-with-carry values into a 64-bit word and return.
     */
    uint32_t r1 = (uint32_t) (m_z1 >> 8) + (uint32_t) (m_z1 >> 24);
    uint32_t r2 = (uint32_t) (m_z2 >> 8) + (uint32_t) (m_z2 >> 24);

    uint64_t m_z = (((uint64_t) (r1) << 0  & 0x00000000FFFFFFFFULL) |
                    ((uint64_t) (r2) << 32 & 0xFFFFFFFF00000000ULL));

    return (m_x + m_y + m_z);
}

/** ---- Complementary multiply-with-carry random number engine ---------------
 * Cmwc
 * @brief Random number generator engine based on the 32-bit variant of
 * George Marsaglia's complementary multiply-with-carry CMWC256 generator,
 * period = 2^8222.
 *
 * @see
 * https://groups.google.com/d/msg/sci.math/k3kVM8KwR-s/jxPdZl8XWZkJ
 * https://groups.google.com/d/msg/comp.lang.c/qZFQgKRCQGg/rmPkaRHqxOMJ
 * http://groups.google.co.uk/group/sci.math.num-analysis/msg/eb4ddde782b17051
 * http://groups.google.co.uk/group/sci.math/msg/9959175f66dd138f
 * http://groups.google.co.uk/group/sci.math/msg/5d891ca5727b97d2
 */
struct Cmwc : Engine<Cmwc> {
    /* Random number generator engine constants. */
    const uint32_t m_seed_c = 362436U;
    const uint32_t m_seed_cmax = 809430660U;
    const uint32_t m_pool_size = 256;
    const uint32_t m_seed_Q[256] = {
        303367, 978269, 743111, 993977, 709157, 205519, 247889, 705097,
        872479, 508349, 598051, 493541, 795203, 355891, 643681, 125687,
        328357, 346397, 363683, 128413, 937429, 916583, 508931, 485959,
        841931, 864679, 519539, 401813, 777251, 901211, 724853, 208111,
        765587, 969179, 193031,  33617, 878021, 186037, 799363, 851041,
        324673, 474163, 886069, 972119, 527563, 391817, 773251, 151273,
        744389, 443701, 513419,  21487, 101963, 988459,  68371, 331523,
        911527, 538823, 237151, 554759, 837079, 432743, 328249, 728489,
        111317, 182711, 467317, 677321, 722363, 836951,  39863, 921031,
        683821, 963847, 473197, 793927, 107339, 544007, 258319, 709273,
        527699, 585043, 854213, 616547, 225223, 781889, 441937, 151537,
        208277, 263429, 745753, 202667, 986333, 169691, 406067, 602431,
        950039,  93407, 591743, 369353, 349373, 627611, 684109, 134287,
        787207,  91411, 324791, 384691, 650449, 339071, 885359, 338237,
        346651,  19697, 914891, 166021, 996409, 203869,  28393, 760847,
        327127, 899893, 966617, 353737, 888917, 702349, 831709, 986471,
        711629, 993611, 474659, 178601, 933883, 104543, 388459, 672169,
        987293,  93281, 159059, 364909, 744221, 808603, 103333, 315521,
        591937, 642547, 416963, 905923, 122477, 124363, 700433,  42787,
        777191, 802031, 958339, 824723, 504061, 463693, 107137, 887203,
        581369, 660067, 917971, 810343, 585691, 826883, 777437, 681293,
        585917, 323879,  36761,  49919, 188369, 501089, 203657,   7673,
        178757, 988591, 306739, 356567, 354439, 187597, 742009,  70769,
        183047,  38677, 465797, 814927, 709531, 242863, 894667,  36269,
        115823, 236111,  74887, 751001, 950791, 228799, 454721, 948139,
         75983, 995833, 121321, 770459, 256423, 463343,  93871, 144887,
        533927, 173263, 199657, 508159,  86371, 731447, 755309, 857867,
        586493, 896717, 343433, 897349,  18899, 277531,  92251, 245753,
        520451, 178513, 517411, 839731, 547951, 656603, 486943, 695371,
        666697, 505201, 243613, 921143, 670507, 325541,  49367, 205619,
        636763, 694567, 988243, 365611, 966997,  13499, 611887, 454709,
        877181, 207923, 577867, 657091, 382777, 153523, 955193, 360509};
    const uint32_t m_warmup_steps = 1024;

    /* Random number generator engine state. */
    uint32_t m_Q[256];              /* pool state with a 256 cycle */
    uint32_t m_c = m_seed_c;        /* carry state */
    uint8_t m_ix = m_pool_size - 1; /* pool index */

    /* Random number generator engine initialisation and sampling functions. */
    void init(void);
    uint32_t rand32 (void);
    uint64_t rand64 (void);

    /* Constructor/destructor. */
    Cmwc(const bool do_init = false) {
        /* Seed the random number generator engine */
        std::memcpy(m_Q, m_seed_Q, sizeof(m_seed_Q));

        /* Initialize the random number generator engine */
        if (do_init) { init(); }
    }
    ~Cmwc() = default;

    /* Copy constructor/assignment. */
    Cmwc(const Cmwc &other)
        : m_c(other.m_c)
        , m_ix(other.m_ix) {
        std::memcpy(
            (void *) m_Q,
            (const void *) other.m_Q,
            m_pool_size * sizeof(uint32_t));
    }
    Cmwc &operator=(const Cmwc &other) {
        if (this == &other) {
            return *this;
        }
        m_c  = other.m_c;
        m_ix = other.m_ix;
        std::memcpy(
            (void *) m_Q,
            (const void *) other.m_Q,
            m_pool_size * sizeof(uint32_t));
        return *this;
    }
};

/**
 * Cmwc::init
 * @brief Initialize the MWC256 random number generator engine.
 * Choose a random initial carry value c < 809430660 and 256 random 32-bit
 * prime integers for the state Q[].
 */
core_inline
void Cmwc::init(void)
{
    /* Seed the carry state. */
    do {
        m_c = randprime();
    } while (m_c < 3 || m_c >= m_seed_cmax);

    /* Seed the pool state. */
    for (uint32_t i = 0; i < m_pool_size; ++i) {
        do {
            m_Q[i] = randprime();
        } while (m_Q[i] < 3);
    }

    /* Warmup the random number generator engine. */
    for (uint32_t i = 0; i < m_warmup_steps; ++i) {
       (void) rand64();     /* uint64_t core_unused(n) = rand64(); */
    }
}

/**
 * Cmwc::rand32
 * @brief Long period 32-bit MWC256 random number generator engine
 * based on usenet posting by G. Marsaglia.
 */
core_inline
uint32_t Cmwc::rand32 (void)
{
    static constexpr uint64_t mul = 1540315826ULL;  /* or 809430660ULL */

    uint64_t t = mul * m_Q[++m_ix] + m_c;
    m_c = (uint32_t) (t >> 8) + (uint32_t) (t >> 24);  /* or c = (t >> 32) */

    uint32_t x = t + m_c;
    if (x < m_c) {
        x++;
        m_c++;
    }

    return (m_Q[m_ix] = 0xFFFFFFFEU - x);           /* Q[ix] = (2^32 - 1) - x */
}

/**
 * Cmwc::rand64
 * @brief Long period 64-bit MWC256 random number generator engine
 * represented as bitwise pair of two 32-bit random samples.
 */
core_inline
uint64_t Cmwc::rand64 (void)
{
    static constexpr uint64_t m1 = 4294584393ULL;   /* 2^32 */
    static constexpr uint64_t m2 = 4246477509ULL;

    uint64_t z1 = m1 * rand32() + m_c;
    uint32_t carry = (uint32_t) (z1 >> 8) + (uint32_t) (z1 >> 24);
    uint64_t z2 = m2 * rand32() + carry;

    return (((uint64_t) (z1) << 0  & 0x00000000FFFFFFFFULL) |
            ((uint64_t) (z2) << 32 & 0xFFFFFFFF00000000ULL));
}

/** ---- Random number generator samplers -------------------------------------
 * uniform
 * @brief Sample a random number from a uniform distribution in interval [a,b].
 */
template<typename Type>
struct uniform {};

template<>
struct uniform<float> {
    template<typename T = Kiss>
    float operator()(
        Engine<T> engine, float lo = (float) 0, float hi = (float) 1) {
        float r = (float) engine.rand32() / UINT32_MAX;
        return (lo + (hi - lo) * r);
    }
};

template<>
struct uniform<double> {
    template<typename T = Kiss>
    double operator()(
        Engine<T> engine, double lo = (double) 0, double hi = (double) 1) {
        double r = (double) engine.rand64() / UINT64_MAX;
        return (lo + (hi - lo) * r);
    }
};

template<>
struct uniform<uint32_t> {
    uniform<float> rand;
    template<typename T = Kiss>
    uint32_t operator()(
        Engine<T> engine, uint32_t lo = 0, uint32_t hi = UINT32_MAX) {
        float delta = (float) (hi - lo + 1);
        float n = (float) lo + std::floor(rand(engine) * delta);
        return ((uint32_t) n);
    }
};

template<>
struct uniform<uint64_t> {
    uniform<double> rand;
    template<typename T = Kiss>
    uint64_t operator()(
        Engine<T> engine, uint64_t lo = 0, uint64_t hi = UINT64_MAX) {
        double delta = (double) (hi - lo + 1);
        double n = (double) lo + std::floor(rand(engine) * delta);
        return ((uint64_t) n);
    }
};

template<>
struct uniform<int32_t> {
    uniform<float> rand;
    template<typename T = Kiss>
    int32_t operator()(
        Engine<T> engine, int32_t lo = INT32_MIN, int32_t hi = INT32_MAX) {
        float delta = (float) (hi - lo + 1);
        float n = (float) lo + std::floor(rand(engine) * delta);
        return ((int32_t) n);
    }
};

template<>
struct uniform<int64_t> {
    uniform<double> rand;
    template<typename T = Kiss>
    int64_t operator()(
        Engine<T> engine, int64_t lo = INT64_MIN, int64_t hi = INT64_MAX) {
        double delta = (double) (hi - lo + 1);
        double n = (double) lo + std::floor(rand(engine) * delta);
        return ((int64_t) n);
    }
};

/**
 * gauss
 * @brief Generate a floating point random number from a Gaussian distribution
 * with average mu and standard deviation sig.
 *
 * If Z is a standard normal deviate with zero mean and unit variance, then
 * X = sig*Z + mu will be a normal deviate with mean mu and variance sig^2.
 * Sample a random deviate Z from a standard normal distribution, i.e., a
 * Gaussian distribution with zero mean and unit variance using Box-Muller
 * algorithm.
 *
 * @see http://en.literateprograms.org/Box-Muller_transform_(C)
 * Compute the normal deviate X using the corresponding linear relation.
 */
template<typename Type>
struct gauss {};

template<>
struct gauss<float> {
    static constexpr float zero = (float) 0;
    static constexpr float one  = (float) 1;
    static constexpr float two  = (float) 2;

    bool has_cache = false;     /* normal random deviate cache state */
    float cache = zero;         /* stored normal random deviate */
    uniform<float> rand;

    template<typename T = Kiss>
    float operator()(Engine<T> engine, float mu = zero, float sig = one) {
        float value = zero;
        if (has_cache) {
            /* Get the stored random deviate and reset cache state. */
            value = cache;
            has_cache = false;
        } else {
            /*
             * Sample a random point in the unit circle from two samples
             * in the interval [-one, one] using the Box-Muller transform.
             */
            float x, y, r;
            do {
                x = rand(engine, -one, one);
                y = rand(engine, -one, one);
                r = x*x + y*y;
            } while (math::isequal(r, zero) || !math::isless(r, one));

            float d = std::sqrt(-two * std::log(r) / r);
            value = x * d;      /* use first random deviate */
            cache = y * d;      /* cache the second random deviate */
            has_cache = true;
        }
        return (sig * value + mu);
    }
};

template<>
struct gauss<double> {
    static constexpr double zero = (double) 0;
    static constexpr double one  = (double) 1;
    static constexpr double two  = (double) 2;

    bool has_cache = false;     /* normal random deviate cache state */
    double cache = zero;        /* stored normal random deviate */
    uniform<double> rand;

    template<typename T = Kiss>
    double operator()(Engine<T> engine, double mu = zero, double sig = one) {
        double value = zero;
        if (has_cache) {
            /* Get the stored random deviate and reset cache state. */
            value = cache;
            has_cache = false;
        } else {
            /*
             * Sample a random point in the unit circle from two samples
             * in the interval [-one, one] using the Box-Muller transform.
             */
            double x, y, r;
            do {
                x = rand(engine, -one, one);
                y = rand(engine, -one, one);
                r = x*x + y*y;
            } while (math::isequal(r, zero) || !math::isless(r, one));

            double d = std::sqrt(-two * std::log(r) / r);
            value = x * d;      /* use first random deviate */
            cache = y * d;      /* cache the second random deviate */
            has_cache = true;
        }
        return (sig * value + mu);
    }
};

} /* rng */
} /* math */
} /* atto */

#endif /* ATTO_MATH_RANDOM_H_ */
