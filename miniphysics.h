#ifndef miniphysics_h
#define miniphysics_h

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h> /* For size_t. */

/* Sized types. Prefer built-in types. Fall back to stdint. */
#ifdef _MSC_VER
    #if defined(__clang__)
        #pragma GCC diagnostic push
        #pragma GCC diagnostic ignored "-Wlanguage-extension-token"
        #pragma GCC diagnostic ignored "-Wlong-long"        
        #pragma GCC diagnostic ignored "-Wc++11-long-long"
    #endif
    typedef   signed __int8  mp_int8;
    typedef unsigned __int8  mp_uint8;
    typedef   signed __int16 mp_int16;
    typedef unsigned __int16 mp_uint16;
    typedef   signed __int32 mp_int32;
    typedef unsigned __int32 mp_uint32;
    typedef   signed __int64 mp_int64;
    typedef unsigned __int64 mp_uint64;
    #if defined(__clang__)
        #pragma GCC diagnostic pop
    #endif
#else
    #define MP_HAS_STDINT
    #include <stdint.h>
    typedef int8_t   mp_int8;
    typedef uint8_t  mp_uint8;
    typedef int16_t  mp_int16;
    typedef uint16_t mp_uint16;
    typedef int32_t  mp_int32;
    typedef uint32_t mp_uint32;
    typedef int64_t  mp_int64;
    typedef uint64_t mp_uint64;
#endif

#ifdef MP_HAS_STDINT
    typedef uintptr_t mp_uintptr;
#else
    #if defined(_WIN32)
        #if defined(_WIN64)
            typedef mp_uint64 mp_uintptr;
        #else
            typedef mp_uint32 mp_uintptr;
        #endif
    #elif defined(__GNUC__)
        #if defined(__LP64__)
            typedef mp_uint64 mp_uintptr;
        #else
            typedef mp_uint32 mp_uintptr;
        #endif
    #else
        typedef mp_uint64 mp_uintptr;   /* Fallback. */
    #endif
#endif

typedef mp_uint8    mp_bool8;
typedef mp_uint32   mp_bool32;
#define MP_TRUE     1
#define MP_FALSE    0

typedef float     mp_float32;
typedef double    mp_float64;
typedef mp_uint32 mp_fixed32;   /* 16.16 fixed point. */
typedef mp_uint64 mp_fixed64;   /* 32.32 fixed point. */

typedef void* mp_handle;
typedef void* mp_ptr;
typedef void (* mp_proc)(void);

/* Define NULL for some compilers. */
#ifndef NULL
#define NULL 0
#endif

#if defined(SIZE_MAX)
    #define MP_SIZE_MAX    SIZE_MAX
#else
    #define MP_SIZE_MAX    0xFFFFFFFF  /* When SIZE_MAX is not defined by the standard library just default to the maximum 32-bit unsigned integer. */
#endif


#ifdef _MSC_VER
    #define MP_INLINE static __forceinline
#elif defined(__GNUC__)
    /*
    I've had a bug report where GCC is emitting warnings about functions possibly not being inlineable. This warning happens when
    the __attribute__((always_inline)) attribute is defined without an "inline" statement. I think therefore there must be some
    case where "__inline__" is not always defined, thus the compiler emitting these warnings. When using -std=c89 or -ansi on the
    command line, we cannot use the "inline" keyword and instead need to use "__inline__". In an attempt to work around this issue
    I am using "__inline__" only when we're compiling in strict ANSI mode.
    */
    #if defined(__STRICT_ANSI__)
        #define MP_INLINE static __inline__ __attribute__((always_inline))
    #else
        #define MP_INLINE static inline __attribute__((always_inline))
    #endif
#else
    #define MP_INLINE static
#endif


typedef int mp_result;
#define MP_SUCCESS                                      0
#define MP_ERROR                                       -1   /* A generic error. */
#define MP_INVALID_ARGS                                -2
#define MP_INVALID_OPERATION                           -3
#define MP_OUT_OF_MEMORY                               -4
#define MP_OUT_OF_RANGE                                -5
#define MP_ACCESS_DENIED                               -6
#define MP_DOES_NOT_EXIST                              -7
#define MP_ALREADY_EXISTS                              -8
#define MP_TOO_MANY_OPEN_FILES                         -9
#define MP_INVALID_FILE                                -10
#define MP_TOO_BIG                                     -11
#define MP_PATH_TOO_LONG                               -12
#define MP_NAME_TOO_LONG                               -13
#define MP_NOT_DIRECTORY                               -14
#define MP_IS_DIRECTORY                                -15
#define MP_DIRECTORY_NOT_EMPTY                         -16
#define MP_END_OF_FILE                                 -17
#define MP_NO_SPACE                                    -18
#define MP_BUSY                                        -19
#define MP_IO_ERROR                                    -20
#define MP_INTERRUPT                                   -21
#define MP_UNAVAILABLE                                 -22
#define MP_ALREADY_IN_USE                              -23
#define MP_BAD_ADDRESS                                 -24
#define MP_BAD_SEEK                                    -25
#define MP_BAD_PIPE                                    -26
#define MP_DEADLOCK                                    -27
#define MP_TOO_MANY_LINKS                              -28
#define MP_NOT_IMPLEMENTED                             -29
#define MP_NO_MESSAGE                                  -30
#define MP_BAD_MESSAGE                                 -31
#define MP_NO_DATA_AVAILABLE                           -32
#define MP_INVALID_DATA                                -33
#define MP_TIMEOUT                                     -34
#define MP_NO_NETWORK                                  -35
#define MP_NOT_UNIQUE                                  -36
#define MP_NOT_SOCKET                                  -37
#define MP_NO_ADDRESS                                  -38
#define MP_BAD_PROTOCOL                                -39
#define MP_PROTOCOL_UNAVAILABLE                        -40
#define MP_PROTOCOL_NOT_SUPPORTED                      -41
#define MP_PROTOCOL_FAMILY_NOT_SUPPORTED               -42
#define MP_ADDRESS_FAMILY_NOT_SUPPORTED                -43
#define MP_SOCKET_NOT_SUPPORTED                        -44
#define MP_CONNECTION_RESET                            -45
#define MP_ALREADY_CONNECTED                           -46
#define MP_NOT_CONNECTED                               -47
#define MP_CONNECTION_REFUSED                          -48
#define MP_NO_HOST                                     -49
#define MP_IN_PROGRESS                                 -50
#define MP_CANCELLED                                   -51
#define MP_MEMORY_ALREADY_MAPPED                       -52
#define MP_AT_END                                      -53


/**********************************************************************************************************************

Math
====

**********************************************************************************************************************/
#include <math.h>

#define MP_FIXED32_SHIFT    16
#define MP_FIXED64_SHIFT    32
#define MP_FIXED32_ONE      ((mp_fixed32)1 << MP_FIXED32_SHIFT)
#define MP_FIXED64_ONE      ((mp_fixed64)1 << MP_FIXED64_SHIFT)

MP_INLINE mp_float32 mp_float32_from_fixed32(mp_fixed32 x)
{
    return x / (mp_float32)MP_FIXED32_ONE;
}
MP_INLINE mp_float64 mp_float64_from_fixed64(mp_fixed64 x)
{
    return x / (mp_float64)MP_FIXED64_ONE;
}
MP_INLINE mp_fixed32 mp_fixed32_from_float32(mp_float32 x)
{
    return (mp_fixed32)(x * MP_FIXED32_ONE);
}
MP_INLINE mp_fixed64 mp_fixed64_from_float64(mp_float64 x)
{
    return (mp_fixed64)(x * MP_FIXED64_ONE);
}


MP_INLINE mp_float32 mp_sqrtf32(mp_float32 x)
{
    return sqrtf(x);
}
MP_INLINE mp_float64 mp_sqrtf64(mp_float64 x)
{
    return sqrt(x);
}

MP_INLINE mp_float32 mp_sinf32(mp_float32 x)
{
    return sinf(x);
}
MP_INLINE mp_float64 mp_sinf64(mp_float64 x)
{
    return sin(x);
}

MP_INLINE mp_float32 mp_cosf32(mp_float32 x)
{
    return cosf(x);
}
MP_INLINE mp_float64 mp_cosf64(mp_float64 x)
{
    return cos(x);
}

MP_INLINE mp_float32 mp_tanf32(mp_float32 x)
{
    return tanf(x);
}
MP_INLINE mp_float64 mp_tanf64(mp_float64 x)
{
    return tan(x);
}

MP_INLINE mp_float32 mp_atanf32(mp_float32 x)
{
    return atanf(x);
}
MP_INLINE mp_float64 mp_atanf64(mp_float64 x)
{
    return atan(x);
}

MP_INLINE mp_float32 mp_fabsf(mp_float32 x)
{
    return fabsf(x);
}
MP_INLINE mp_float64 mp_absf(mp_float64 x)
{
    return fabs(x);
}


MP_INLINE mp_float32 mp_float32_sin(mp_float32 x)
{
    return mp_sinf32(x);
}
MP_INLINE mp_float64 mp_float64_sin(mp_float64 x)
{
    return mp_sinf64(x);
}
MP_INLINE mp_fixed32 mp_fixed32_sin(mp_fixed32 x)
{
    /* TODO: Need a proper deterministic fixed point sin(). */
    return mp_fixed32_from_float32(mp_float32_sin(mp_float32_from_fixed32(x)));
}
MP_INLINE mp_fixed64 mp_fixed64_sin(mp_fixed64 x)
{
    /* TODO: Need a proper deterministic fixed point sin(). */
    return mp_fixed64_from_float64(mp_float64_sin(mp_float64_from_fixed64(x)));
}


MP_INLINE mp_float32 mp_float32_cos(mp_float32 x)
{
    return mp_cosf32(x);
}
MP_INLINE mp_float64 mp_float64_cos(mp_float64 x)
{
    return mp_cosf64(x);
}
MP_INLINE mp_fixed32 mp_fixed32_cos(mp_fixed32 x)
{
    /* TODO: Need a proper deterministic fixed point cos(). */
    return mp_fixed32_from_float32(mp_float32_cos(mp_float32_from_fixed32(x)));
}
MP_INLINE mp_fixed64 mp_fixed64_cos(mp_fixed64 x)
{
    /* TODO: Need a proper deterministic fixed point cos(). */
    return mp_fixed64_from_float64(mp_float64_cos(mp_float64_from_fixed64(x)));
}


MP_INLINE mp_float32 mp_float32_from_int32(mp_int32 x)
{
    return (mp_float32)x;
}
MP_INLINE mp_float64 mp_float64_from_int32(mp_int32 x)
{
    return (mp_float64)x;
}
MP_INLINE mp_fixed32 mp_fixed32_from_int32(mp_int32 x)
{
    return (mp_fixed32)x << MP_FIXED32_SHIFT;
}
MP_INLINE mp_fixed64 mp_fixed64_from_int32(mp_int32 x)
{
    return (mp_fixed64)x << MP_FIXED64_SHIFT;
}


#define DEFINE_FUNCTION_add(type) MP_INLINE type type##_add(type x, type y) { return x + y; }
DEFINE_FUNCTION_add(mp_float32)
DEFINE_FUNCTION_add(mp_float64)
DEFINE_FUNCTION_add(mp_fixed32)
DEFINE_FUNCTION_add(mp_fixed64)

#define DEFINE_FUNCTION_sub(type) MP_INLINE type type##_sub(type x, type y) { return x - y; }
DEFINE_FUNCTION_sub(mp_float32)
DEFINE_FUNCTION_sub(mp_float64)
DEFINE_FUNCTION_sub(mp_fixed32)
DEFINE_FUNCTION_sub(mp_fixed64)

#define DEFINE_FUNCTION_div(type) MP_INLINE type type##_div(type x, type y) { return x / y; }
DEFINE_FUNCTION_div(mp_float32)
DEFINE_FUNCTION_div(mp_float64)
DEFINE_FUNCTION_div(mp_fixed32)
DEFINE_FUNCTION_div(mp_fixed64)

MP_INLINE mp_float32 mp_float32_mul(mp_float32 x, mp_float32 y)
{
    return x * y;
}
MP_INLINE mp_float64 mp_float64_mul(mp_float64 x, mp_float64 y)
{
    return x * y;
}
MP_INLINE mp_fixed32 mp_fixed32_mul(mp_fixed32 x, mp_fixed32 y)
{
    return (mp_fixed32)(((mp_int64)x * (mp_int64)y) >> MP_FIXED32_SHIFT);
}
MP_INLINE mp_fixed64 mp_fixed64_mul(mp_fixed64 x, mp_fixed64 y)
{
    /* TODO: We need to do this manually since 128-bit integers are not standard. */
    return (x * y) >> MP_FIXED64_SHIFT;
}


#define DECLARE_STRUCT_x2(type) \
    typedef union               \
    {                           \
        struct                  \
        {                       \
            type x;             \
            type y;             \
        };                      \
        type v[2];              \
    } type##x2;

DECLARE_STRUCT_x2(mp_float32)
DECLARE_STRUCT_x2(mp_float64)
DECLARE_STRUCT_x2(mp_fixed32)
DECLARE_STRUCT_x2(mp_fixed64)


#define DECLARE_STRUCT_x3(type) \
    typedef union               \
    {                           \
        struct                  \
        {                       \
            type x;             \
            type y;             \
            type z;             \
        };                      \
        type v[3];              \
    } type##x3;

DECLARE_STRUCT_x3(mp_float32)
DECLARE_STRUCT_x3(mp_float64)
DECLARE_STRUCT_x3(mp_fixed32)
DECLARE_STRUCT_x3(mp_fixed64)


#define DECLARE_STRUCT_x4(type) \
    typedef union               \
    {                           \
        struct                  \
        {                       \
            type x;             \
            type y;             \
            type z;             \
            type w;             \
        };                      \
        type v[4];              \
    } type##x4;

DECLARE_STRUCT_x4(mp_float32)
DECLARE_STRUCT_x4(mp_float64)
DECLARE_STRUCT_x4(mp_fixed32)
DECLARE_STRUCT_x4(mp_fixed64)


/* mp_float32x2 mp_float32x2f(mp_float32 x, mp_float32 y); */
#define DEFINE_FUNCTION_x2f(type)                   \
    MP_INLINE type##x2 type##x2f(type x, type y)    \
    {                                               \
        type##x2 r;                                 \
        r.x = x;                                    \
        r.y = y;                                    \
        return r;                                   \
    }

DEFINE_FUNCTION_x2f(mp_float32)
DEFINE_FUNCTION_x2f(mp_float64)
DEFINE_FUNCTION_x2f(mp_fixed32)
DEFINE_FUNCTION_x2f(mp_fixed64)

/* mp_float32x3 mp_float32x3f(mp_float32 x, mp_float32 y, mp_float32 z); */
#define DEFINE_FUNCTION_x3f(type)                           \
    MP_INLINE type##x3 type##x3f(type x, type y, type z)    \
    {                                                       \
        type##x3 r;                                         \
        r.x = x;                                            \
        r.y = y;                                            \
        r.z = z;                                            \
        return r;                                           \
    }

DEFINE_FUNCTION_x3f(mp_float32)
DEFINE_FUNCTION_x3f(mp_float64)
DEFINE_FUNCTION_x3f(mp_fixed32)
DEFINE_FUNCTION_x3f(mp_fixed64)

/* mp_float32x4 mp_float32x4f(mp_float32 x, mp_float32 y, mp_float32 z, mp_float32 w); */
#define DEFINE_FUNCTION_x4f(type)                                   \
    MP_INLINE type##x4 type##x4f(type x, type y, type z, type w)    \
    {                                                               \
        type##x4 r;                                                 \
        r.x = x;                                                    \
        r.y = y;                                                    \
        r.z = z;                                                    \
        r.w = w;                                                    \
        return r;                                                   \
    }

DEFINE_FUNCTION_x4f(mp_float32)
DEFINE_FUNCTION_x4f(mp_float64)
DEFINE_FUNCTION_x4f(mp_fixed32)
DEFINE_FUNCTION_x4f(mp_fixed64)


#define DEFINE_FUNCTION_x2fv(type) MP_INLINE type##x2 type##x2fv(type* v) { return type##x2f(v[0], v[1]); }
DEFINE_FUNCTION_x2fv(mp_float32)
DEFINE_FUNCTION_x2fv(mp_float64)
DEFINE_FUNCTION_x2fv(mp_fixed32)
DEFINE_FUNCTION_x2fv(mp_fixed64)

#define DEFINE_FUNCTION_x3fv(type) MP_INLINE type##x3 type##x3fv(type* v) { return type##x3f(v[0], v[1], v[2]); }
DEFINE_FUNCTION_x3fv(mp_float32)
DEFINE_FUNCTION_x3fv(mp_float64)
DEFINE_FUNCTION_x3fv(mp_fixed32)
DEFINE_FUNCTION_x3fv(mp_fixed64)

#define DEFINE_FUNCTION_x4fv(type) MP_INLINE type##x4 type##x4fv(type* v) { return type##x4f(v[0], v[1], v[2], v[3]); }
DEFINE_FUNCTION_x4fv(mp_float32)
DEFINE_FUNCTION_x4fv(mp_float64)
DEFINE_FUNCTION_x4fv(mp_fixed32)
DEFINE_FUNCTION_x4fv(mp_fixed64)


/*
Returns a unit vector (1, 0) rotated by the specified angle in radians.
*/
#define DEFINE_FUNCTION_x2_from_angle(type)                                         \
    MP_INLINE type##x2 type##x2_from_angle(type angleInRadians)                     \
    {                                                                               \
        return type##x2f(type##_cos(angleInRadians), type##_sin(angleInRadians));   \
    }

DEFINE_FUNCTION_x2_from_angle(mp_float32)
DEFINE_FUNCTION_x2_from_angle(mp_float64)
DEFINE_FUNCTION_x2_from_angle(mp_fixed32)
DEFINE_FUNCTION_x2_from_angle(mp_fixed64)



#define DEFINE_FUNCTION_x2_add(type)                                        \
    MP_INLINE type##x2 type##x2_add(type##x2 v0, type##x2 v1)               \
    {                                                                       \
        return type##x2f(type##_add(v0.x, v1.x), type##_add(v0.y, v1.y));   \
    }

DEFINE_FUNCTION_x2_add(mp_float32)
DEFINE_FUNCTION_x2_add(mp_float64)
DEFINE_FUNCTION_x2_add(mp_fixed32)
DEFINE_FUNCTION_x2_add(mp_fixed64)

#define DEFINE_FUNCTION_x3_add(type)                                                                \
    MP_INLINE type##x3 type##x3_add(type##x3 v0, type##x3 v1)                                       \
    {                                                                                               \
        return type##x3f(type##_add(v0.x, v1.x), type##_add(v0.y, v1.y), type##_add(v0.z, v1.z));   \
    }

DEFINE_FUNCTION_x3_add(mp_float32)
DEFINE_FUNCTION_x3_add(mp_float64)
DEFINE_FUNCTION_x3_add(mp_fixed32)
DEFINE_FUNCTION_x3_add(mp_fixed64)

#define DEFINE_FUNCTION_x4_add(type)                                                                                        \
    MP_INLINE type##x4 type##x4_add(type##x4 v0, type##x4 v1)                                                               \
    {                                                                                                                       \
        return type##x4f(type##_add(v0.x, v1.x), type##_add(v0.y, v1.y), type##_add(v0.z, v1.z), type##_add(v0.w, v1.w));   \
    }

DEFINE_FUNCTION_x4_add(mp_float32)
DEFINE_FUNCTION_x4_add(mp_float64)
DEFINE_FUNCTION_x4_add(mp_fixed32)
DEFINE_FUNCTION_x4_add(mp_fixed64)



#define DEFINE_FUNCTION_x2_sub(type)                                        \
    MP_INLINE type##x2 type##x2_sub(type##x2 v0, type##x2 v1)               \
    {                                                                       \
        return type##x2f(type##_sub(v0.x, v1.x), type##_sub(v0.y, v1.y));   \
    }

DEFINE_FUNCTION_x2_sub(mp_float32)
DEFINE_FUNCTION_x2_sub(mp_float64)
DEFINE_FUNCTION_x2_sub(mp_fixed32)
DEFINE_FUNCTION_x2_sub(mp_fixed64)

#define DEFINE_FUNCTION_x3_sub(type)                                                                \
    MP_INLINE type##x3 type##x3_sub(type##x3 v0, type##x3 v1)                                       \
    {                                                                                               \
        return type##x3f(type##_sub(v0.x, v1.x), type##_sub(v0.y, v1.y), type##_sub(v0.z, v1.z));   \
    }

DEFINE_FUNCTION_x3_sub(mp_float32)
DEFINE_FUNCTION_x3_sub(mp_float64)
DEFINE_FUNCTION_x3_sub(mp_fixed32)
DEFINE_FUNCTION_x3_sub(mp_fixed64)

#define DEFINE_FUNCTION_x4_sub(type)                                                                                        \
    MP_INLINE type##x4 type##x4_sub(type##x4 v0, type##x4 v1)                                                               \
    {                                                                                                                       \
        return type##x4f(type##_sub(v0.x, v1.x), type##_sub(v0.y, v1.y), type##_sub(v0.z, v1.z), type##_sub(v0.w, v1.w));   \
    }

DEFINE_FUNCTION_x4_sub(mp_float32)
DEFINE_FUNCTION_x4_sub(mp_float64)
DEFINE_FUNCTION_x4_sub(mp_fixed32)
DEFINE_FUNCTION_x4_sub(mp_fixed64)



#define DEFINE_FUNCTION_x2_mul(type)                                        \
    MP_INLINE type##x2 type##x2_mul(type##x2 v0, type##x2 v1)               \
    {                                                                       \
        return type##x2f(type##_mul(v0.x, v1.x), type##_mul(v0.y, v1.y));   \
    }

DEFINE_FUNCTION_x2_mul(mp_float32)
DEFINE_FUNCTION_x2_mul(mp_float64)
DEFINE_FUNCTION_x2_mul(mp_fixed32)
DEFINE_FUNCTION_x2_mul(mp_fixed64)

#define DEFINE_FUNCTION_x3_mul(type)                                                                \
    MP_INLINE type##x3 type##x3_mul(type##x3 v0, type##x3 v1)                                       \
    {                                                                                               \
        return type##x3f(type##_mul(v0.x, v1.x), type##_mul(v0.y, v1.y), type##_mul(v0.z, v1.z));   \
    }

DEFINE_FUNCTION_x3_mul(mp_float32)
DEFINE_FUNCTION_x3_mul(mp_float64)
DEFINE_FUNCTION_x3_mul(mp_fixed32)
DEFINE_FUNCTION_x3_mul(mp_fixed64)

#define DEFINE_FUNCTION_x4_mul(type)                                                                                        \
    MP_INLINE type##x4 type##x4_mul(type##x4 v0, type##x4 v1)                                                               \
    {                                                                                                                       \
        return type##x4f(type##_mul(v0.x, v1.x), type##_mul(v0.y, v1.y), type##_mul(v0.z, v1.z), type##_mul(v0.w, v1.w));   \
    }

DEFINE_FUNCTION_x4_mul(mp_float32)
DEFINE_FUNCTION_x4_mul(mp_float64)
DEFINE_FUNCTION_x4_mul(mp_fixed32)
DEFINE_FUNCTION_x4_mul(mp_fixed64)


#define DEFINE_FUNCTION_x2_mul1(type)                               \
    MP_INLINE type##x2 type##x2_mul1(type##x2 v, type a)            \
    {                                                               \
        return type##x2f(type##_mul(v.x, a), type##_mul(v.y, a));   \
    }

DEFINE_FUNCTION_x2_mul1(mp_float32)
DEFINE_FUNCTION_x2_mul1(mp_float64)
DEFINE_FUNCTION_x2_mul1(mp_fixed32)
DEFINE_FUNCTION_x2_mul1(mp_fixed64)

#define DEFINE_FUNCTION_x3_mul1(type)                                                   \
    MP_INLINE type##x3 type##x3_mul1(type##x3 v, type a)                                \
    {                                                                                   \
        return type##x3f(type##_mul(v.x, a), type##_mul(v.y, a), type##_mul(v.z, a));   \
    }

DEFINE_FUNCTION_x3_mul1(mp_float32)
DEFINE_FUNCTION_x3_mul1(mp_float64)
DEFINE_FUNCTION_x3_mul1(mp_fixed32)
DEFINE_FUNCTION_x3_mul1(mp_fixed64)

#define DEFINE_FUNCTION_x4_mul1(type)                                                                       \
    MP_INLINE type##x4 type##x4_mul1(type##x4 v, type a)                                                    \
    {                                                                                                       \
        return type##x4f(type##_mul(v.x, a), type##_mul(v.y, a), type##_mul(v.z, a), type##_mul(v.z, a));   \
    }

DEFINE_FUNCTION_x4_mul1(mp_float32)
DEFINE_FUNCTION_x4_mul1(mp_float64)
DEFINE_FUNCTION_x4_mul1(mp_fixed32)
DEFINE_FUNCTION_x4_mul1(mp_fixed64)



#define DEFINE_FUNCTION_x2_div(type)                                        \
    MP_INLINE type##x2 type##x2_div(type##x2 v0, type##x2 v1)               \
    {                                                                       \
        return type##x2f(type##_div(v0.x, v1.x), type##_div(v0.y, v1.y));   \
    }

DEFINE_FUNCTION_x2_div(mp_float32)
DEFINE_FUNCTION_x2_div(mp_float64)
DEFINE_FUNCTION_x2_div(mp_fixed32)
DEFINE_FUNCTION_x2_div(mp_fixed64)

#define DEFINE_FUNCTION_x3_div(type)                                                                \
    MP_INLINE type##x3 type##x3_div(type##x3 v0, type##x3 v1)                                       \
    {                                                                                               \
        return type##x3f(type##_div(v0.x, v1.x), type##_div(v0.y, v1.y), type##_div(v0.z, v1.z));   \
    }

DEFINE_FUNCTION_x3_div(mp_float32)
DEFINE_FUNCTION_x3_div(mp_float64)
DEFINE_FUNCTION_x3_div(mp_fixed32)
DEFINE_FUNCTION_x3_div(mp_fixed64)

#define DEFINE_FUNCTION_x4_div(type)                                                                                        \
    MP_INLINE type##x4 type##x4_div(type##x4 v0, type##x4 v1)                                                               \
    {                                                                                                                       \
        return type##x4f(type##_div(v0.x, v1.x), type##_div(v0.y, v1.y), type##_div(v0.z, v1.z), type##_div(v0.w, v1.w));   \
    }

DEFINE_FUNCTION_x4_div(mp_float32)
DEFINE_FUNCTION_x4_div(mp_float64)
DEFINE_FUNCTION_x4_div(mp_fixed32)
DEFINE_FUNCTION_x4_div(mp_fixed64)




MP_INLINE mp_float32 mp_float32x2_dot(mp_float32x2 v0, mp_float32x2 v1)
{
    return v0.x*v1.x + v0.y*v1.y;
}

MP_INLINE mp_float32 mp_float32x3_dot(mp_float32x3 v0, mp_float32x3 v1)
{
    return v0.x*v1.x + v0.y*v1.y + v0.z*v1.z;
}

MP_INLINE mp_float32 mp_float32x4_dot(mp_float32x4 v0, mp_float32x4 v1)
{
    return v0.x*v1.x + v0.y*v1.y + v0.z*v1.z + v0.w*v1.w;
}


MP_INLINE mp_float32 mp_float32x2_length2(mp_float32x2 v)
{
    return mp_float32x2_dot(v, v);
}

MP_INLINE mp_float32 mp_float32x3_length2(mp_float32x3 v)
{
    return mp_float32x3_dot(v, v);
}

MP_INLINE mp_float32 mp_float32x4_length2(mp_float32x4 v)
{
    return mp_float32x4_dot(v, v);
}


MP_INLINE mp_float32 mp_float32x2_length(mp_float32x2 v)
{
    return mp_sqrtf32(mp_float32x2_length2(v));
}

MP_INLINE mp_float32 mp_float32x3_length(mp_float32x3 v)
{
    return mp_sqrtf32(mp_float32x3_length2(v));
}

MP_INLINE mp_float32 mp_float32x4_length(mp_float32x4 v)
{
    return mp_sqrtf32(mp_float32x4_length2(v));
}


MP_INLINE mp_float32 mp_float32x2_distance2(mp_float32x2 v0, mp_float32x2 v1)
{
    return mp_float32x2_length2(mp_float32x2_sub(v0, v1));
}

MP_INLINE mp_float32 mp_float32x3_distance2(mp_float32x3 v0, mp_float32x3 v1)
{
    return mp_float32x3_length2(mp_float32x3_sub(v0, v1));
}

MP_INLINE mp_float32 mp_float32x4_distance2(mp_float32x4 v0, mp_float32x4 v1)
{
    return mp_float32x4_length2(mp_float32x4_sub(v0, v1));
}


MP_INLINE mp_float32 mp_float32x2_distance(mp_float32x2 v0, mp_float32x2 v1)
{
    return mp_sqrtf32(mp_float32x2_distance2(v0, v1));
}

MP_INLINE mp_float32 mp_float32x3_distance(mp_float32x3 v0, mp_float32x3 v1)
{
    return mp_sqrtf32(mp_float32x3_distance2(v0, v1));
}

MP_INLINE mp_float32 mp_float32x4_distance(mp_float32x4 v0, mp_float32x4 v1)
{
    return mp_sqrtf32(mp_float32x4_distance2(v0, v1));
}


MP_INLINE mp_float32x2 mp_float32x2_normalize(mp_float32x2 v)
{
    return mp_float32x2_mul1(v, 1.0f / mp_float32x2_length(v));
}

MP_INLINE mp_float32x3 mp_float32x3_normalize(mp_float32x3 v)
{
    return mp_float32x3_mul1(v, 1.0f / mp_float32x3_length(v));
}

MP_INLINE mp_float32x4 mp_float32x4_normalize(mp_float32x4 v)
{
    return mp_float32x4_mul1(v, 1.0f / mp_float32x4_length(v));
}


MP_INLINE mp_float32x2 mp_float32x2_rotate(mp_float32x2 v, const mp_float32 angleInRadians)
{
    mp_float32 s = mp_sinf32(angleInRadians);
    mp_float32 c = mp_cosf32(angleInRadians);
    return mp_float32x2f(
        v.x*c - v.y*s,
        v.x*s + v.y*c
    );
}

MP_INLINE mp_float32x2 mp_float32x2_perpendicular(mp_float32x2 v)
{
    return mp_float32x2f(-v.y, v.x);
}
MP_INLINE mp_float32x2 mp_float32x2_perpendicular_rev(mp_float32x2 v)
{
    return mp_float32x2f(v.y, -v.x);
}

MP_INLINE mp_float32 mp_float32x2_angle(mp_float32x2 v0, mp_float32x2 v1)
{
    return mp_atanf32(v0.y / v0.x) - mp_atanf32(v1.y / v1.x);
}



#define DECLARE_STRUCT_x3x3(type)   \
    typedef union                   \
    {                               \
        type##x3 col[3];            \
    } type##x3x3;

DECLARE_STRUCT_x3x3(mp_float32)
DECLARE_STRUCT_x3x3(mp_float64)
DECLARE_STRUCT_x3x3(mp_fixed32)
DECLARE_STRUCT_x3x3(mp_fixed64)

#define DECLARE_STRUCT_x4x4(type)   \
    typedef union                   \
    {                               \
        type##x4 col[4];            \
    } type##x4x4;

DECLARE_STRUCT_x4x4(mp_float32)
DECLARE_STRUCT_x4x4(mp_float64)
DECLARE_STRUCT_x4x4(mp_fixed32)
DECLARE_STRUCT_x4x4(mp_fixed64)



/*
Performs a ray/plane intersection test.

Parameters
----------
`rayO` (in)
    The ray's origin.

`rayD` (in)
    The ray's direction.

`lineA` (in)
    The A in the plane equation.

`lineB` (in)
    The B in the plane equation.

`lineC` (in)
    The C in the plane equation.

`pIntersectionPoint` (out)
    A pointer to the vector that will receive the intersection point, if any.

Return Value
------------
True if the ray intersects with the plane; false otherwise.

Remarks
-------
A line is just a plane in 2D. It's equation is the following:

    `Ax + Bx + C = 0`

Where `A` and `B` are the x and y coordinates of the line's normal, and `C` is the distance from the origin. Below are the mappings for each
variable and their parameters for this function:

    `A` = `lineA`
    `B` = `lineB`
    `C` = `lineC`

You do not need to pass in values for the `x` and `y` variables in the line equation above. This function will determine the values to use for
these variables which are calculated based on the ray equation below:

    `R(t) = O + Dt`

For each coordiate:

    `Rx(t) = O.x + D.x*t`
    `Ry(t) = O.y + D.y*t`

Inserting the ray equation into the line equation:

    `A(Rx(t)) + B(Rx(t)) + C = 0`

We can then solve this for `t`:

    `                               0 = A(O.x + D.x*t) + B(O.y + D.y*t) + C`
    `                              -C = A(O.x + D.x*t) + B(O.y + D.y*t)`
    `                              -C = AO.x + AD.x*t + BO.y + BD.y*t`
    `                -C - AO.x - BO.y = AD.x*t + BD.y*t`
    `              -(C + AO.x + BO.y) = t(AD.x + BD.y)`
    `-(C + AO.x + BO.y)/(AD.x + BD.y) = t`

If `t` is < 0 then it means the point is behind the ray's origin and there is no intersection.
*/
MP_INLINE mp_bool32 mp_ray_line_intersection_float32x2(mp_float32x2 rayO, mp_float32x2 rayD, mp_float32 lineA, mp_float32 lineB, mp_float32 lineC, mp_float32x2* pIntersectionPoint)
{
    mp_float32 t;

    t = lineA*rayD.x + lineB*rayD.y;
    if (t != 0) {
        t = -(lineC + lineA*rayO.x + lineB*rayO.y) / t;
    } else {
        return MP_FALSE;
    }

    if (t < 0) {
        return MP_FALSE;    /* The line is behind the ray. */
    }

    /* Intersects. */
    if (pIntersectionPoint != NULL) {
        *pIntersectionPoint = mp_float32x2_add(rayO, mp_float32x2_mul1(rayD, t));
    }

    return MP_TRUE;
}


/*
Performs a ray/line segment intersection test.

Return Value
------------
True if the ray intersects with the line segment; false otherwise.

Remarks
-------
To check the intersection of a ray and a line segment, all we need to do is get the intersection point of the ray and the line
defined by the line segment and then compare the intersection point against the segment points.

Below is how the line parameters are calculated:

    `N = `


If `pIntersectionPoint` is not null, this will receive the intersection point if the ray intersects the line. If the ray does
not intersect with the line segment this is left unmodified.

The distance between the ray's origin and the intersection point can be calculated using mp_float32x2_distance().
*/
MP_INLINE mp_bool32 mp_ray_line_segment_intersection_float32x2(mp_float32x2 rayO, mp_float32x2 rayD, mp_float32x2 p0, mp_float32x2 p1, mp_float32x2* pIntersectionPoint)
{
    mp_float32x2 lineIntersection;
    mp_float32x2 d;
    mp_float32x2 n;
    mp_float32   c;

    d = mp_float32x2_sub(p1, p0);
    if (d.x == 0 && d.y == 0) {
        return MP_FALSE;    /* Line segment is of 0 length. */
    }

    n = mp_float32x2f(-d.y, d.x);
    c = d.y*p0.x - d.x*p0.y;

    if (mp_ray_line_intersection_float32x2(rayO, rayD, n.x, n.y, c, &lineIntersection)) {
        mp_float32 t;

        if (p0.x != p1.x) {
            t = (lineIntersection.x - p0.x) / (p1.x - p0.x);
        } else {
            t = (lineIntersection.y - p0.y) / (p1.y - p0.y);
        }

        if (t < 0 || t > 1) {
            return MP_FALSE;
        }

        if (pIntersectionPoint != NULL) {
            *pIntersectionPoint = mp_float32x2_add(p0, mp_float32x2_mul1(mp_float32x2_sub(p1, p0), t));
        }

        return MP_TRUE;
    } else {
        return MP_FALSE;
    }
}

/*
Retrieves the closest point on a 2D plane to another point.
*/
MP_INLINE mp_float32x2 mp_closest_point_on_plane_to_point(mp_float32x2 p, mp_float32 planeA, mp_float32 planeB, mp_float32 planeC)
{
    mp_float32x2 r;
    mp_float32 denom;

    denom = planeA*planeA + planeB*planeB;
    if (denom == 0) {
        return p;
    }

    r.x = (planeB * (planeB*p.x - planeA*p.y) - planeA*planeC) / denom;
    r.y = (planeA * (planeA*p.y - planeB*p.x) - planeB*planeC) / denom;

    return r;
}

/*
Retrieves the distance of a point to a 2D plane.
*/
MP_INLINE mp_float32 mp_distance_point_to_plane(mp_float32x2 p, mp_float32 planeA, mp_float32 planeB, mp_float32 planeC)
{
    return mp_fabsf(planeA*p.x + planeB*p.y + planeC) / mp_sqrtf32(planeA*planeA + planeB*planeB);
}



#if defined(MP_FIXED64)
    #define MP_USE_FIXED64
#elif defined(MP_FIXED32)
    #define MP_USE_FIXED32
#elif defined(MP_FLOAT64)
    #define MP_USE_FLOAT64
#else
    #define MP_USE_FLOAT32
#endif

#if defined(MP_USE_FLOAT32)
    typedef mp_float32         mp_real;
    typedef mp_float32x2       mp_vec2;
    typedef mp_float32x3       mp_vec3;
    typedef mp_float32x4       mp_vec4;
    typedef mp_float32x3x3     mp_mat3;
    typedef mp_float32x4x4     mp_mat4;
    #define mp_one             1.0f
    #define mp_real_from_int32 mp_float32_from_int32
    #define mp_vec2f           mp_float32x2f
    #define mp_vec3f           mp_float32x3f
    #define mp_vec4f           mp_float32x4f
    #define mp_vec2fv          mp_float32x2fv
    #define mp_vec3fv          mp_float32x3fv
    #define mp_vec4fv          mp_float32x4fv
    #define mp_add             mp_float32_add
    #define mp_vec2_add        mp_float32x2_add
    #define mp_vec3_add        mp_float32x3_add
    #define mp_vec4_add        mp_float32x4_add
    #define mp_sub             mp_float32_sub
    #define mp_vec2_sub        mp_float32x2_sub
    #define mp_vec3_sub        mp_float32x3_sub
    #define mp_vec4_sub        mp_float32x4_sub
    #define mp_mul             mp_float32_mul
    #define mp_vec2_mul        mp_float32x2_mul
    #define mp_vec3_mul        mp_float32x3_mul
    #define mp_vec4_mul        mp_float32x4_mul
    #define mp_vec2_mul1       mp_float32x2_mul1
    #define mp_vec3_mul1       mp_float32x3_mul1
    #define mp_vec4_mul1       mp_float32x4_mul1
    #define mp_div             mp_float32_div
    #define mp_vec2_div        mp_float32x2_div
    #define mp_vec3_div        mp_float32x3_div
    #define mp_vec4_div        mp_float32x4_div
#endif
#if defined(MP_USE_FLOAT64)
    typedef mp_float64         mp_real;
    typedef mp_float64x2       mp_vec2;
    typedef mp_float64x3       mp_vec3;
    typedef mp_float64x4       mp_vec4;
    typedef mp_float64x3x3     mp_mat3;
    typedef mp_float64x4x4     mp_mat4;
    #define mp_one             1.0
#endif
#if defined(MP_USE_FIXED32)
    typedef mp_fixed32         mp_real;
    typedef mp_fixed32x2       mp_vec2;
    typedef mp_fixed32x3       mp_vec3;
    typedef mp_fixed32x4       mp_vec4;
    typedef mp_fixed32x3x3     mp_mat3;
    typedef mp_fixed32x4x4     mp_mat4;
    #define mp_one             MP_FIXED32_ONE
#endif
#if defined(MP_USE_FIXED64)
    typedef mp_fixed64         mp_real;
    typedef mp_fixed64x2       mp_vec2;
    typedef mp_fixed64x3       mp_vec3;
    typedef mp_fixed64x4       mp_vec4;
    typedef mp_fixed64x3x3     mp_mat3;
    typedef mp_fixed64x4x4     mp_mat4;
    #define mp_one             MP_FIXED64_ONE
#endif


/**********************************************************************************************************************

Collision Detection
===================

**********************************************************************************************************************/
#ifndef MP_NO_COLLISION

typedef enum
{
    ma_shape_type_sphere,
    ma_shape_type_ellipsoid,
    ma_shape_type_box
} ma_shape_type;

typedef struct
{
    ma_shape_type type;
    union
    {
        struct
        {
            mp_real radius;
        } sphere;
        struct
        {
            mp_vec3 radius;
        } ellipsoid;
        struct
        {
            mp_vec3 dimensions;
        } box;
    } data;
} mp_shape;

mp_result mp_sphere_init(mp_real radius, mp_shape* pShape);
mp_result mp_box_init(mp_vec3 dimensions, mp_shape* pShape);


typedef struct
{
    mp_shape shape;
    mp_vec3 position;
    mp_mat3 rotation;
} mp_collision_object;

mp_result mp_collision_object_init(mp_shape shape, mp_collision_object* pCollisionObject);


typedef struct
{
    mp_real _unused;
} mp_collision_world_config;

mp_collision_world_config mp_collision_world_config_init();


typedef struct
{
    mp_real _unusedX;
    mp_vec3 _unusedV;
} mp_collision_world;

mp_result mp_collision_world_init(const mp_collision_world_config* pConfig, mp_collision_world* pCollisionWorld);
void mp_collision_world_uninit(mp_collision_world* pCollisionWorld);
mp_result mp_collision_world_add_object(mp_collision_world* pCollisionWorld, mp_collision_object* pCollisionObject);
mp_result mp_collision_world_remove_object(mp_collision_world* pCollisionWorld, mp_collision_object* pCollisionObject);

#endif  /* MP_NO_COLLISION_DETECTION */


/**********************************************************************************************************************

Dynamics
========

**********************************************************************************************************************/
#ifndef MP_NO_DYNAMICS

typedef struct
{
#ifndef MP_NO_COLLISION
    mp_collision_world_config collision;
#endif
    mp_real timestep;
    mp_vec3 gravity;
} mp_dynamics_world_config;

mp_dynamics_world_config mp_dynamics_world_config_init();



typedef struct
{
    mp_vec3 position;       /* World position. */
    mp_mat3 rotation;
    mp_vec3 linVelocity;    /* Linear velocity. */
    mp_vec3 angVelocity;    /* Angular velocity. */
    mp_real mass;           /* Static if mass = 0. */
    mp_bool32 isKinematic;
} mp_dynamics_body;

typedef struct
{
#ifndef MP_NO_COLLISION
    mp_collision_world collision;
#endif
    mp_real dt;         /* Used in ma_dynamics_world_step() to keep track of the delta time. */
    mp_real timestep;   /* Our fixed step time. */
    mp_vec3 gravity;
    mp_dynamics_body bodies[128];   /* Temp. Will be replaced with a dynamic array later. */
    mp_uint32 bodyCount;
} mp_dynamics_world;

mp_result mp_dynamics_world_init(const mp_dynamics_world_config* pConfig, mp_dynamics_world* pDynamicsWorld);
void mp_dynamics_world_uninit(mp_dynamics_world* pDynamicsWorld);
void mp_dynamics_world_set_fixed_timestep(mp_dynamics_world* pDynamicsWorld, mp_real timestep);
mp_real mp_dynamics_world_get_fixed_timestep(mp_dynamics_world* pDynamicsWorld);
void mp_dynamics_world_step(mp_dynamics_world* pDynamicsWorld, mp_real dt);
void mp_dynamics_world_set_gravity(mp_dynamics_world* pDynamicsWorld, mp_vec3 gravity);
mp_vec3 mp_dynamics_world_get_gravity(mp_dynamics_world* pDynamicsWorld);

void mp_dynamics_world_create_body(mp_dynamics_world* pDynamicsWorld, mp_dynamics_body** ppDynamicsBody);
void mp_dynamics_world_delete_body(mp_dynamics_world* pDynamicsWorld, mp_dynamics_body** ppDynamicsBody);
void mp_dynamics_world_insert_body(mp_dynamics_world* pDynamicsWorld, mp_dynamics_body** ppDynamicsBody);
void mp_dynamics_world_remove_body(mp_dynamics_world* pDynamicsWorld, mp_dynamics_body** ppDynamicsBody);

#endif /* MP_NO_DYNAMICS */


#ifdef __cplusplus
}
#endif
#endif  /* miniphysics_h */



/**********************************************************************************************************************
***********************************************************************************************************************

Implementation

***********************************************************************************************************************
**********************************************************************************************************************/
#if defined(MINIPHYSICS_IMPLEMENTATION)

#include <stdlib.h> /* For malloc(), free() */
#include <string.h> /* For memset() */
#include <assert.h>

#ifndef MP_MALLOC
#define MP_MALLOC(sz) malloc((sz))
#endif
#ifndef MP_REALLOC
#define MP_REALLOC(p, sz) realloc((p), (sz))
#endif
#ifndef MP_FREE
#define MP_FREE(p) free((p))
#endif
#ifndef MP_ZERO_MEMORY
#define MP_ZERO_MEMORY(p, sz) memset((p), 0, (sz))
#endif
#ifndef MP_COPY_MEMORY
#define MP_COPY_MEMORY(dst, src, sz) memcpy((dst), (src), (sz))
#endif
#ifndef MP_ASSERT
#define MP_ASSERT(condition) assert(condition)
#endif

#define MP_ZERO_OBJECT(p) MP_ZERO_MEMORY((p), sizeof(*(p)))

#define MP_COUNTOF(p)               (sizeof(p) / sizeof((p)[0]))
#define MP_MAX(x, y)                (((x) > (y)) ? (x) : (y))
#define MP_MIN(x, y)                (((x) < (y)) ? (x) : (y))
#define MP_CLAMP(x, lo, hi)         (MP_MAX(lo, MP_MIN(x, hi)))
#define MP_ABS(x)                   (((x) > 0) ? (x) : -(x))
#define MP_OFFSET_PTR(p, offset)    (((mp_uint8*)(p)) + (offset))



/**********************************************************************************************************************

Collision Detection
===================

**********************************************************************************************************************/
#ifndef MP_NO_COLLISION

mp_result mp_sphere_init(mp_real radius, mp_shape* pShape)
{
    if (pShape == NULL) {
        return MP_INVALID_ARGS;
    }

    pShape->type = ma_shape_type_sphere;
    pShape->data.sphere.radius = radius;

    return MP_SUCCESS;
}

mp_result mp_box_init(mp_vec3 dimensions, mp_shape* pShape)
{
    if (pShape == NULL) {
        return MP_INVALID_ARGS;
    }

    pShape->type = ma_shape_type_box;
    pShape->data.box.dimensions = dimensions;

    return MP_SUCCESS;
}


mp_collision_world_config mp_collision_world_config_init()
{
    mp_collision_world_config config;
    
    MP_ZERO_OBJECT(&config);
    config._unused = 0;

    return config;
}

mp_result mp_collision_world_init(const mp_collision_world_config* pConfig, mp_collision_world* pCollisionWorld)
{
    if (pCollisionWorld == NULL) {
        return MP_INVALID_ARGS;
    }

    MP_ZERO_OBJECT(pCollisionWorld);

    return MP_SUCCESS;
}

void mp_collision_world_uninit(mp_collision_world* pCollisionWorld)
{
    if (pCollisionWorld == NULL) {
        return;
    }

    /* TODO: Implement me. */
}

#endif


/**********************************************************************************************************************

Dynamics
========

**********************************************************************************************************************/
#ifndef MP_NO_DYNAMICS

mp_dynamics_world_config mp_dynamics_world_config_init()
{
    mp_dynamics_world_config config;

    MP_ZERO_OBJECT(&config);
    config.collision = mp_collision_world_config_init();
    config.timestep  = mp_div(mp_one, 144); /* 144 Hz */
    config.gravity   = mp_vec3f(0, -10 * mp_one, 0);

    return config;
}

mp_result mp_dynamics_world_init(const mp_dynamics_world_config* pConfig, mp_dynamics_world* pDynamicsWorld)
{
    mp_result result;

    if (pDynamicsWorld == NULL) {
        return MP_INVALID_ARGS;
    }

    MP_ZERO_OBJECT(pDynamicsWorld);
    pDynamicsWorld->dt       = 0;
    pDynamicsWorld->timestep = pConfig->timestep;
    pDynamicsWorld->gravity  = pConfig->gravity;

#ifndef MP_NO_COLLISION
    result = mp_collision_world_init(&pConfig->collision, &pDynamicsWorld->collision);
    if (result != MP_SUCCESS) {
        return result;
    }
#endif

    return MP_SUCCESS;
}

void mp_dynamics_world_uninit(mp_dynamics_world* pDynamicsWorld)
{
    if (pDynamicsWorld == NULL) {
        return;
    }

#ifndef MP_NO_COLLISION
    mp_collision_world_uninit(&pDynamicsWorld->collision);
#endif
}

void mp_dynamics_world_set_fixed_timestep(mp_dynamics_world* pDynamicsWorld, mp_real timestep)
{
    if (pDynamicsWorld == NULL) {
        return;
    }

    pDynamicsWorld->timestep = timestep;
}

mp_real mp_dynamics_world_get_fixed_timestep(mp_dynamics_world* pDynamicsWorld)
{
    if (pDynamicsWorld == NULL) {
        return 0;
    }

    return pDynamicsWorld->timestep;
}

static void mp_dynamics_world_step_fixed(mp_dynamics_world* pDynamicsWorld)
{
    mp_uint32 iBody;
    mp_vec3 gravityStep;

    MP_ASSERT(pDynamicsWorld != NULL);

    gravityStep = mp_vec3_mul1(pDynamicsWorld->gravity, pDynamicsWorld->timestep);

    for (iBody = 0; iBody < pDynamicsWorld->bodyCount; iBody += 1) {
        mp_dynamics_body* pBody = &pDynamicsWorld->bodies[iBody];
        if (pBody->isKinematic) {
            /* Kinematic body. */
        } else {
            if (pBody->mass > 0) {
                /* Apply gravity. */
                pBody->linVelocity = mp_vec3_add(pBody->linVelocity, gravityStep);

                /* Apply linear velocity. */
                pDynamicsWorld->bodies[iBody].position = mp_vec3_add(pDynamicsWorld->bodies[iBody].position, pDynamicsWorld->bodies[iBody].linVelocity);

                /* TODO: Apply angular velocity. */
            } else {
                /* It's a static body. */
            }
        }
    }
}

void mp_dynamics_world_step(mp_dynamics_world* pDynamicsWorld, mp_real dt)
{
    if (pDynamicsWorld == NULL) {
        return;
    }

    /* We need to do multiple steps, depending on `dt` and our fixed timestep. For stability, we can only update the physics simulation based on the fixed timestep. */
    pDynamicsWorld->dt = mp_add(pDynamicsWorld->dt, dt);

    while (pDynamicsWorld->dt >= pDynamicsWorld->timestep) {
        mp_dynamics_world_step_fixed(pDynamicsWorld);
        pDynamicsWorld->dt = mp_sub(pDynamicsWorld->dt, pDynamicsWorld->timestep);
    }
}

void mp_dynamics_world_set_gravity(mp_dynamics_world* pDynamicsWorld, mp_vec3 gravity)
{
    if (pDynamicsWorld == NULL) {
        return;
    }

    pDynamicsWorld->gravity = gravity;
}

mp_vec3 mp_dynamics_world_get_gravity(mp_dynamics_world* pDynamicsWorld)
{
    if (pDynamicsWorld == NULL) {
        return mp_vec3f(0, 0, 0);
    }

    return pDynamicsWorld->gravity;
}

#endif

#endif  /* MINIPHYSICS_IMPLEMENTATION */