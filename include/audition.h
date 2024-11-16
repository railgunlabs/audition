/*
 *  Audition - an xUnit Testing Framework for C11 and beyond
 *  Copyright (c) 2021-2024 Railgun Labs, LLC
 *
 *  This library is available under an end-user license agreement (EULA) for
 *  personal and demo use. For business use or other specific licensing needs,
 *  a separate licensing agreement must be obtained from Railgun Labs, LLC.
 *  By using this library, you agree to comply with the terms and conditions
 *  of the license.
 *
 *  For more information on obtaining a personal use or business license,
 *  please visit <https://RailgunLabs.com/audition/license>.
 */

// Documentation is available at <https://RailgunLabs.com/audition/manual>.

#pragma once

// Audition requires a compiler supporting C11 or newer.
// Audition does not support C++ at this time.
#if !defined(__STDC_VERSION__) || (__STDC_VERSION__ < 201112L)
    #if defined(__cplusplus)
        #error "Audition is built for C11. It does not support C++ at this time."
    #elif defined(__GNUC__) || defined(__clang__)
        #error "Audition requires a C11 compiler. Try compiling again with the -std=c11 flag."
    #elif defined(_MSC_VER)
        #error "Audition requires a C11 compiler. Try compiling again with the /std:c11 flag."
    #else
        #error "Audition requires a C11 compiler."
    #endif
#endif

#include <stdint.h>
#include <stdbool.h>
#include <wchar.h>

// As of Xcode 14.2 and Clang 14.0.0 on macOS Ventura, Apple does not provide
// the C11 uchar.h header. Implement the required character types here.
#if defined(__has_include)
    #if __has_include(<uchar.h>)
        #include <uchar.h>
    #else
        typedef uint_least16_t char16_t;
        typedef uint_least32_t char32_t;
    #endif
#else
    #include <uchar.h>
#endif

#ifndef DOXYGEN
    #if defined(_WIN32) || defined(__CYGWIN__)
        #if defined(DLL_EXPORT)
            #define XAPI __declspec(dllexport)
        #elif defined(AUDITION_STATIC)
            #define XAPI
        #else
            #define XAPI __declspec(dllimport)
        #endif
    #elif __GNUC__ >= 4 || defined(__clang__)
        #define XAPI __attribute__ ((visibility ("default")))
    #else
        #define XAPI
    #endif
#endif

#define TEST(SUITE_NAME, TEST_NAME, ...)                                    \
    AUDITION_TEST_WARNINGS_PUSH                                             \
    static void SUITE_NAME ## _ ## TEST_NAME ## _func (void);               \
    DATA_PUSH                                                               \
    const struct xUnitEntryPoint SUITE_NAME ## _ ## TEST_NAME ## _test = {  \
        .magic = XUNIT_HEADER_MAGIC,                                        \
        .tag = XUNIT_ENTRY_TEST,                                            \
        .c = &audition__libc,                                               \
        .u.test.function = SUITE_NAME ## _ ## TEST_NAME ## _func,           \
        .u.test.suite = #SUITE_NAME,                                        \
        .u.test.name = #TEST_NAME,                                          \
        .u.test.file = __FILE__,                                            \
        .u.test.line = __LINE__,                                            \
        .u.test.options = {                                                 \
            .iterations = 1,                                                \
            .exit_status = -1,                                              \
            .signal = -1,                                                   \
            __VA_ARGS__                                                     \
        },                                                                  \
    };                                                                      \
    DATA_POP                                                                \
    static void SUITE_NAME ## _ ## TEST_NAME ## _func (void)                \
    AUDITION_TEST_WARNINGS_POP

#define SET_TEST_ITERATIONS(TEST_NAME, COUNT) audit_set_test_iterations(#TEST_NAME, COUNT, __FILE__, __LINE__)

#if defined(DOXYGEN)
extern int32_t TEST_ITERATION;
extern const int32_t DYNAMIC_ITERATIONS;
#else
#define TEST_ITERATION audit_iteration_index()
#define DYNAMIC_ITERATIONS 0
#endif

#define ABORT(...) audit_fail(__FILE__, __LINE__, true, __VA_ARGS__)
#define FAIL(...) audit_fail(__FILE__, __LINE__, false, __VA_ARGS__)

typedef int64_t StatusCode;

typedef struct TestOptions
{
    int32_t iterations;
    StatusCode exit_status;
    StatusCode signal;
    int timeout;
    bool sandbox;
} TestOptions;

#define RUNNER_SETUP()                                                      \
    AUDITION_TEST_WARNINGS_PUSH                                             \
    static void runner_setup_func (void);                                   \
    DATA_PUSH                                                               \
    const struct xUnitEntryPoint runner_setup_fixture = {                   \
        .magic = XUNIT_HEADER_MAGIC,                                        \
        .tag = XUNIT_ENTRY_RUNNER_SETUP,                                    \
        .c = &audition__libc,                                               \
        .u.fixture.function = runner_setup_func,                            \
        .u.fixture.file = __FILE__,                                         \
        .u.fixture.line = __LINE__,                                         \
    };                                                                      \
    DATA_POP                                                                \
    static void runner_setup_func (void)                                    \
    AUDITION_TEST_WARNINGS_POP

#define RUNNER_TEARDOWN()                                                   \
    AUDITION_TEST_WARNINGS_PUSH                                             \
    static void runner_teardown_func (void);                                \
    DATA_PUSH                                                               \
    const struct xUnitEntryPoint runner_teardown_fixture = {                \
        .magic = XUNIT_HEADER_MAGIC,                                        \
        .tag = XUNIT_ENTRY_RUNNER_TEARDOWN,                                 \
        .c = &audition__libc,                                               \
        .u.fixture.function = runner_teardown_func,                         \
        .u.fixture.file = __FILE__,                                         \
        .u.fixture.line = __LINE__,                                         \
    };                                                                      \
    DATA_POP                                                                \
    static void runner_teardown_func (void)                                 \
    AUDITION_TEST_WARNINGS_POP

#define SUITE_SETUP(SUITE_NAME)                                             \
    AUDITION_TEST_WARNINGS_PUSH                                             \
    static void SUITE_NAME ## _beforeAll_func (void);                       \
    DATA_PUSH                                                               \
    const struct xUnitEntryPoint SUITE_NAME ## _beforeAll_fixture = {       \
        .magic = XUNIT_HEADER_MAGIC,                                        \
        .tag = XUNIT_ENTRY_SUITE_SUITE_SETUP,                               \
        .c = &audition__libc,                                               \
        .u.fixture.function = SUITE_NAME ## _beforeAll_func,                \
        .u.fixture.suite = #SUITE_NAME,                                     \
        .u.fixture.file = __FILE__,                                         \
        .u.fixture.line = __LINE__,                                         \
    };                                                                      \
    DATA_POP                                                                \
    static void SUITE_NAME ## _beforeAll_func (void)                        \
    AUDITION_TEST_WARNINGS_POP

#define SUITE_TEARDOWN(SUITE_NAME)                                          \
    AUDITION_TEST_WARNINGS_PUSH                                             \
    static void SUITE_NAME ## _afterAll_func (void);                        \
    DATA_PUSH                                                               \
    const struct xUnitEntryPoint SUITE_NAME ## _afterAll_fixture = {        \
        .magic = XUNIT_HEADER_MAGIC,                                        \
        .tag = XUNIT_ENTRY_SUITE_SUITE_TEARDOWN,                            \
        .c = &audition__libc,                                               \
        .u.fixture.function = SUITE_NAME ## _afterAll_func,                 \
        .u.fixture.suite = #SUITE_NAME,                                     \
        .u.fixture.file = __FILE__,                                         \
        .u.fixture.line = __LINE__,                                         \
    };                                                                      \
    DATA_POP                                                                \
    static void SUITE_NAME ## _afterAll_func (void)                         \
    AUDITION_TEST_WARNINGS_POP

#define TEST_SETUP(SUITE_NAME)                                              \
    AUDITION_TEST_WARNINGS_PUSH                                             \
    static void SUITE_NAME ## _beforeEach_func (void);                      \
    DATA_PUSH                                                               \
    const struct xUnitEntryPoint SUITE_NAME ## _beforeEach_fixture = {      \
        .magic = XUNIT_HEADER_MAGIC,                                        \
        .tag = XUNIT_ENTRY_SUITE_TEST_SETUP,                                \
        .c = &audition__libc,                                               \
        .u.fixture.function = SUITE_NAME ## _beforeEach_func,               \
        .u.fixture.suite = #SUITE_NAME,                                     \
        .u.fixture.file = __FILE__,                                         \
        .u.fixture.line = __LINE__,                                         \
    };                                                                      \
    DATA_POP                                                                \
    static void SUITE_NAME ## _beforeEach_func (void)                       \
    AUDITION_TEST_WARNINGS_POP

#define TEST_TEARDOWN(SUITE_NAME)                                           \
    AUDITION_TEST_WARNINGS_PUSH                                             \
    static void SUITE_NAME ## _afterEach_func (void);                       \
    DATA_PUSH                                                               \
    const struct xUnitEntryPoint SUITE_NAME ## _afterEach_fixture = {       \
        .magic = XUNIT_HEADER_MAGIC,                                        \
        .tag = XUNIT_ENTRY_SUITE_TEST_TEARDOWN,                             \
        .c = &audition__libc,                                               \
        .u.fixture.function = SUITE_NAME ## _afterEach_func,                \
        .u.fixture.suite = #SUITE_NAME,                                     \
        .u.fixture.file = __FILE__,                                         \
        .u.fixture.line = __LINE__,                                         \
    };                                                                      \
    DATA_POP                                                                \
    static void SUITE_NAME ## _afterEach_func (void)                        \
    AUDITION_TEST_WARNINGS_POP

#define ASSERT_EQ(X, Y, ...) XUNIT_ASSERT_EQ(X, Y, #X, #Y, true, "" __VA_ARGS__)
#define ASSERT_EQ_APPROX(X, Y, T, ...) XUNIT_ASSERT_EQ_APPROX(X, Y, T, #X, #Y, true, "" __VA_ARGS__)
#define ASSERT_NE(X, Y, ...) XUNIT_ASSERT_NE(X, Y, #X, #Y, true, "" __VA_ARGS__)
#define ASSERT_NE_APPROX(X, Y, T, ...) XUNIT_ASSERT_NE_APPROX(X, Y, T, #X, #Y, true, "" __VA_ARGS__)
#define ASSERT_LT(X, Y, ...) XUNIT_ASSERT_LT(X, Y, #X, #Y, true, "" __VA_ARGS__)
#define ASSERT_LTEQ(X, Y, ...) XUNIT_ASSERT_LTEQ(X, Y, #X, #Y, true, "" __VA_ARGS__)
#define ASSERT_GT(X, Y, ...) XUNIT_ASSERT_GT(X, Y, #X, #Y, true, "" __VA_ARGS__)
#define ASSERT_GTEQ(X, Y, ...) XUNIT_ASSERT_GTEQ(X, Y, #X, #Y, true, "" __VA_ARGS__)
#define ASSERT_NULL(X, ...) audit_expect_null(X, #X, __FILE__, __LINE__, true, "" __VA_ARGS__)
#define ASSERT_NONNULL(X, ...) audit_expect_nonnull(X, #X, __FILE__, __LINE__, true, "" __VA_ARGS__)
#define ASSERT_TRUE(X, ...) \
    AUDITION_EXPECT_WARNINGS_PUSH \
    audit_expect_true((X) ? 1 : 0, #X, __FILE__, __LINE__, true, "" __VA_ARGS__) \
    AUDITION_EXPECT_WARNINGS_POP
#define ASSERT_FALSE(X, ...) \
    AUDITION_EXPECT_WARNINGS_PUSH \
    audit_expect_false((X) ? 1 : 0, #X, __FILE__, __LINE__, true, "" __VA_ARGS__) \
    AUDITION_EXPECT_WARNINGS_POP

#define EXPECT_EQ(X, Y, ...) XUNIT_ASSERT_EQ(X, Y, #X, #Y, false, "" __VA_ARGS__)
#define EXPECT_EQ_APPROX(X, Y, T, ...) XUNIT_ASSERT_EQ_APPROX(X, Y, T, #X, #Y, false, "" __VA_ARGS__)
#define EXPECT_NE(X, Y, ...) XUNIT_ASSERT_NE(X, Y, #X, #Y, false, "" __VA_ARGS__)
#define EXPECT_NE_APPROX(X, Y, T, ...) XUNIT_ASSERT_NE_APPROX(X, Y, T, #X, #Y, false, "" __VA_ARGS__)
#define EXPECT_LT(X, Y, ...) XUNIT_ASSERT_LT(X, Y, #X, #Y, false, "" __VA_ARGS__)
#define EXPECT_LTEQ(X, Y, ...) XUNIT_ASSERT_LTEQ(X, Y, #X, #Y, false, "" __VA_ARGS__)
#define EXPECT_GT(X, Y, ...) XUNIT_ASSERT_GT(X, Y, #X, #Y, false, "" __VA_ARGS__)
#define EXPECT_GTEQ(X, Y, ...) XUNIT_ASSERT_GTEQ(X, Y, #X, #Y, false, "" __VA_ARGS__)
#define EXPECT_NULL(X, ...) audit_expect_null(X, #X, __FILE__, __LINE__, false, "" __VA_ARGS__)
#define EXPECT_NONNULL(X, ...) audit_expect_nonnull(X, #X, __FILE__, __LINE__, false, "" __VA_ARGS__)
#define EXPECT_TRUE(X, ...) \
    AUDITION_EXPECT_WARNINGS_PUSH \
    audit_expect_true((X) ? 1 : 0, #X, __FILE__, __LINE__, false, "" __VA_ARGS__) \
    AUDITION_EXPECT_WARNINGS_POP
#define EXPECT_FALSE(X, ...) \
    AUDITION_EXPECT_WARNINGS_PUSH \
    audit_expect_false((X) ? 1 : 0, #X, __FILE__, __LINE__, false, "" __VA_ARGS__) \
    AUDITION_EXPECT_WARNINGS_POP

#define ASSERT_STR_EQ(X, Y, ...) XUNIT_ASSERT_EQ_STR(X, Y, #X, #Y, true, "" __VA_ARGS__)
#define ASSERT_STR_NE(X, Y, ...) XUNIT_ASSERT_NE_STR(X, Y, #X, #Y, true, "" __VA_ARGS__)
#define ASSERT_STR_LT(X, Y, ...) XUNIT_ASSERT_LT_STR(X, Y, #X, #Y, true, "" __VA_ARGS__)
#define ASSERT_STR_LTEQ(X, Y, ...) XUNIT_ASSERT_LTEQ_STR(X, Y, #X, #Y, true, "" __VA_ARGS__)
#define ASSERT_STR_GT(X, Y, ...) XUNIT_ASSERT_GT_STR(X, Y, #X, #Y, true, "" __VA_ARGS__)
#define ASSERT_STR_GTEQ(X, Y, ...) XUNIT_ASSERT_GTEQ_STR(X, Y, #X, #Y, true, "" __VA_ARGS__)

#define EXPECT_STR_EQ(X, Y, ...) XUNIT_ASSERT_EQ_STR(X, Y, #X, #Y, false, "" __VA_ARGS__)
#define EXPECT_STR_NE(X, Y, ...) XUNIT_ASSERT_NE_STR(X, Y, #X, #Y, false, "" __VA_ARGS__)
#define EXPECT_STR_LT(X, Y, ...) XUNIT_ASSERT_LT_STR(X, Y, #X, #Y, false, "" __VA_ARGS__)
#define EXPECT_STR_LTEQ(X, Y, ...) XUNIT_ASSERT_LTEQ_STR(X, Y, #X, #Y, false, "" __VA_ARGS__)
#define EXPECT_STR_GT(X, Y, ...) XUNIT_ASSERT_GT_STR(X, Y, #X, #Y, false, "" __VA_ARGS__)
#define EXPECT_STR_GTEQ(X, Y, ...) XUNIT_ASSERT_GTEQ_STR(X, Y, #X, #Y, false, "" __VA_ARGS__)

#define ASSERT_MEM_EQ(X, Y, SX, SY, ...) audit_expect_eq_mem(X, Y, SX, SY, #X, #Y, __FILE__, __LINE__, true, "" __VA_ARGS__)
#define ASSERT_MEM_NE(X, Y, SX, SY, ...) audit_expect_ne_mem(X, Y, SX, SY, #X, #Y, __FILE__, __LINE__, true, "" __VA_ARGS__)

#define EXPECT_MEM_EQ(X, Y, SX, SY, ...) audit_expect_eq_mem(X, Y, SX, SY, #X, #Y, __FILE__, __LINE__, false, "" __VA_ARGS__)
#define EXPECT_MEM_NE(X, Y, SX, SY, ...) audit_expect_ne_mem(X, Y, SX, SY, #X, #Y, __FILE__, __LINE__, false, "" __VA_ARGS__)

#define FAKE(FUNC, FAKE)                                                            \
    do {                                                                            \
        XUNIT_EXPECT_TYPES_EQUAL(FUNC, FAKE, "function signatures do not match");   \
        audit_fake((FUNC), (FAKE), #FUNC, #FAKE, __FILE__, __LINE__);               \
    } while (0)

#define STUB(FUNC, VALUE)                             \
    _Generic((VALUE),                                 \
        _Bool: audit_stub_bool,                       \
        signed char: audit_stub_char,                 \
        unsigned char: audit_stub_uchar,              \
        signed short: audit_stub_short,               \
        unsigned short: audit_stub_ushort,            \
        signed int: audit_stub_int,                   \
        unsigned int: audit_stub_uint,                \
        signed long: audit_stub_long,                 \
        unsigned long: audit_stub_ulong,              \
        signed long long: audit_stub_longlong,        \
        unsigned long long: audit_stub_ulonglong,     \
        float: audit_stub_float,                      \
        double: audit_stub_double,                    \
        long double: audit_stub_longdouble,           \
        default: audit_stub_pointer                   \
    )(FUNC, #FUNC, VALUE, #VALUE, __FILE__, __LINE__)

#define CALL(FUNC, ...)                             \
    do {                                            \
        const int _id = audit_suspend(FUNC, #FUNC); \
        (FUNC)(__VA_ARGS__);                        \
        audit_reinstate(_id);                       \
    } while (0)

#define CALL_GET(FUNC, RESULT, ...)                 \
    do {                                            \
        const int _id = audit_suspend(FUNC, #FUNC); \
        *(RESULT) = (FUNC)(__VA_ARGS__);            \
        audit_reinstate(_id);                       \
    } while (0)

#define RESTORE(FUNC) audit_restore(FUNC, #FUNC, __FILE__, __LINE__)
#define SUSPEND_MOCKS() audit_suspend_mocks()
#define RESTORE_MOCKS() audit_restore_mocks()

typedef unsigned long long audit_time;
typedef void(*audit_listdir_cb)(void *cb_data, const char *filename, bool directory);

XAPI audit_time audit_now(void);
XAPI void audit_sleep(audit_time duration);

XAPI int audit_read(FILE *stream, void *buf, int count);
XAPI int audit_write(FILE *stream, const void *buf, int count);
XAPI int audit_close(FILE *stream);

XAPI bool audit_mkdir(const char *path);
XAPI bool audit_rmdir(const char *path);
XAPI bool audit_isdir(const char *path);
XAPI bool audit_isfile(const char *path);
XAPI int audit_listdir(const char *dir, void *cb_data, audit_listdir_cb cb);

XAPI int audit_main(int argc, char *argv[]);
XAPI bool audit_isboxd(void);

/************************************************************************************
 *                                                                                  *
 *                           !!!!!!! WARNING !!!!!!                                 *
 *                                                                                  *
 *    All code beyond this comment block is PRIVATE API and is subject to           *
 *    modification without incrementing the major version number of the library.    *
 *    Please do not access any definitions after this point. They are to be         *
 *    only indirectly through the API defined before this comment block.            *
 *                                                                                  *
 ************************************************************************************/

#ifndef DOXYGEN

// The following functions are compiled by the user against their local C runtime
// library. They are called by the Audition implementation to ensure that IO
// interactions use the users IO routines. This ensures IO problems, like
// interleaving output are a non-issue.

#include <stdio.h>

#if defined(_WIN32)
#include <io.h>
#include <fcntl.h>
#else
#include <unistd.h>
#endif

static FILE *audition__stdany(int fd) {
    switch (fd) {
        case 0: return stdout;
        case 1: return stderr;
        case 2: return stdin;
        default: return (void *)0;
    }
}

static FILE *audition__tmpfile(void) {
#if defined(_MSC_VER)
    FILE *fp = NULL;
    if (tmpfile_s(&fp) == 0)
    {
        return fp;
    }
    return NULL;
#else
    return tmpfile();
#endif
}

static FILE *audition__fopen(const char *filename, const char *mode) {
#if defined(_MSC_VER)
    FILE *fp = NULL;
    if (fopen_s(&fp, filename, mode) == 0)
    {
        return fp;
    }
    return NULL;
#else
    return fopen(filename, mode);
#endif
}

static int audition__fputs(const char *buffer, FILE *stream) {
    return fputs(buffer, stream);
}

static int audition__fputc(int character, FILE *stream) {
    return fputc(character, stream);
}

static int audition__fclose(FILE *stream) {
    return fclose(stream);
}

static int audition__fseek(FILE *stream, long offset, int origin) {
    return fseek(stream, offset, origin);
}

static size_t audition__fread(void *buffer, size_t size, size_t count, FILE *stream) {
    return fread(buffer, size, count, stream);
}

static size_t audition__fwrite(const void *buffer, size_t size, size_t count, FILE *stream) {
    return fwrite(buffer, size, count, stream);
}

static long audition__ftell(FILE *stream) {
    return ftell(stream);
}

static int audition__fflush(FILE *stream) {
    return fflush(stream);
}

static int audition__setmode(int fd, int mode) {
#if defined(_MSC_VER)
    return _setmode(fd, mode);
#elif defined(_WIN32)
    return setmode(fd, mode);
#else
    return -1;
#endif
}

static FILE *audition__fdopen(int fd, const char *format) {
#if defined(_MSC_VER)
    return _fdopen(fd, format);
#else
    return fdopen(fd, format);
#endif
}

static int audition__setvbuf(FILE *stream, char *buffer, int mode, size_t size) {
    return setvbuf(stream, buffer, mode, size);
}

static int audition__fileno(FILE *stream) {
#if defined(_MSC_VER)
    return _fileno(stream);
#else
    return fileno(stream);
#endif
}

static int audition__dup(int fd) {
#if defined(_MSC_VER)
    return _dup(fd);
#else
    return dup(fd);
#endif
}

static int audition__dup2(int src, int dst) {
#if defined(_MSC_VER)
    return _dup2(src, dst);
#else
    return dup2(src, dst);
#endif
}

static const struct xUnitLibC
{
    FILE *(*std)(int fd);
    FILE *(*tmpfile)(void);
    FILE *(*fopen)(const char *filename, const char *mode);
    int (*fputs)(const char *buffer, FILE *stream);
    int (*fputc)(int character, FILE *stream);
    size_t (*fwrite)(const void *buffer, size_t size, size_t count, FILE *stream);
    size_t (*fread)(void *buffer, size_t size, size_t count, FILE *stream);
    long (*ftell)(FILE *stream);
    int (*fclose)(FILE *stream);
    int (*fseek)(FILE *stream, long offset, int origin);
    int(*fflush)(FILE *stream);
    int(*setmode)(int fd, int mode);
    FILE *(*fdopen)(int fd, const char *format);
    int (*setvbuf)(FILE *stream, char *buffer, int mode, size_t size);
    int (*fileno)(FILE *stream);
    int (*dup)(int fd);
    int (*dup2)(int src, int dst);
} audition__libc = {
    .std = audition__stdany,
    .tmpfile = audition__tmpfile,
    .fopen = audition__fopen,
    .fputs = audition__fputs,
    .fputc = audition__fputc,
    .fwrite = audition__fwrite,
    .fread = audition__fread,
    .ftell = audition__ftell,
    .fclose = audition__fclose,
    .fseek = audition__fseek,
    .fflush = audition__fflush,
    .setmode = audition__setmode,
    .fdopen = audition__fdopen,
    .setvbuf = audition__setvbuf,
    .fileno = audition__fileno,
    .dup = audition__dup,
    .dup2 = audition__dup2,
};

typedef void(*xUnitCallback)(void);

enum xUnitEntryPointTag
{
    XUNIT_ENTRY_INVALID,
    XUNIT_ENTRY_RUNNER_SETUP,
    XUNIT_ENTRY_RUNNER_TEARDOWN,
    XUNIT_ENTRY_SUITE_SUITE_SETUP,
    XUNIT_ENTRY_SUITE_SUITE_TEARDOWN,
    XUNIT_ENTRY_SUITE_TEST_SETUP,
    XUNIT_ENTRY_SUITE_TEST_TEARDOWN,
    XUNIT_ENTRY_TEST,
};

struct xUnitEntryPoint
{
    uint64_t magic;
    enum xUnitEntryPointTag tag;
    const struct xUnitLibC *c;
    union
    {
        struct
        {
            xUnitCallback function;
            const char *suite;
            const char *name;
            const char *file;
            int line;
            struct TestOptions options;
        } test;

        struct
        {
            xUnitCallback function;
            const char *suite;
            const char *file;
            int line;
        } fixture;
    } u;
};

#if defined(__GNUC__) || defined(__clang__)
    #define XUNIT_EXPECT_TYPES_EQUAL(T1, T2, MSG) \
        _Static_assert(__builtin_types_compatible_p(typeof(T2), typeof(T1)), MSG);
#else
    #define XUNIT_EXPECT_TYPES_EQUAL(T1, T2, MSG)
#endif

#if defined(_MSC_VER)
    #define DATA_PUSH                             \
        __pragma(data_seg(push))                  \
        __pragma(section("xunit", read))          \
        __declspec(allocate("xunit"))
    #define DATA_POP                              \
        __pragma(data_seg(pop))
#elif defined(__APPLE__)
    #define DATA_PUSH __attribute__((section("__DATA," "xunit")))
    #define DATA_POP
#else
    #define DATA_PUSH __attribute__((section("xunit")))
    #define DATA_POP
#endif

#if defined(__clang__)
#define AUDITION_TEST_WARNINGS_PUSH \
    _Pragma("clang diagnostic push") \
    _Pragma("clang diagnostic ignored \"-Winitializer-overrides\"")
#define AUDITION_TEST_WARNINGS_POP \
    _Pragma("clang diagnostic pop")

#define AUDITION_EXPECT_WARNINGS_PUSH \
    _Pragma("clang diagnostic push") \
    _Pragma("clang diagnostic ignored \"-Wliteral-conversion\"")
#define AUDITION_EXPECT_WARNINGS_POP \
    _Pragma("clang diagnostic pop")
#else
#define AUDITION_TEST_WARNINGS_PUSH
#define AUDITION_TEST_WARNINGS_POP
#define AUDITION_EXPECT_WARNINGS_PUSH
#define AUDITION_EXPECT_WARNINGS_POP
#endif

#define COMPARE_FUNCS_DEFS(NAME)                                                                            \
    XAPI void audit_expect_## NAME ##_schar(signed char x_val, signed char y_val,                           \
        const char *x_var, const char *y_var, const char *file, int line, bool fail, const char *msg, ...); \
    XAPI void audit_expect_## NAME ##_uchar(unsigned char x_val, unsigned char y_val,                       \
        const char *x_var, const char *y_var, const char *file, int line, bool fail, const char *msg, ...); \
    XAPI void audit_expect_## NAME ##_sshort(signed short x_val, signed short y_val,                        \
        const char *x_var, const char *y_var, const char *file, int line, bool fail, const char *msg, ...); \
    XAPI void audit_expect_## NAME ##_ushort(unsigned short x_val, unsigned short y_val,                    \
        const char *x_var, const char *y_var, const char *file, int line, bool fail, const char *msg, ...); \
    XAPI void audit_expect_## NAME ##_sint(signed int x_val, signed int y_val,                              \
        const char *x_var, const char *y_var, const char *file, int line, bool fail, const char *msg, ...); \
    XAPI void audit_expect_## NAME ##_uint(unsigned int x_val, unsigned int y_val,                          \
        const char *x_var, const char *y_var, const char *file, int line, bool fail, const char *msg, ...); \
    XAPI void audit_expect_## NAME ##_slong(signed long x_val, signed long y_val,                           \
        const char *x_var, const char *y_var, const char *file, int line, bool fail, const char *msg, ...); \
    XAPI void audit_expect_## NAME ##_ulong(unsigned long x_val, unsigned long y_val,                       \
        const char *x_var, const char *y_var, const char *file, int line, bool fail, const char *msg, ...); \
    XAPI void audit_expect_## NAME ##_slonglong(signed long long x_val, signed long long y_val,             \
        const char *x_var, const char *y_var, const char *file, int line, bool fail, const char *msg, ...); \
    XAPI void audit_expect_## NAME ##_ulonglong(unsigned long long x_val, unsigned long long y_val,         \
        const char *x_var, const char *y_var, const char *file, int line, bool fail, const char *msg, ...); \
    XAPI void audit_expect_## NAME ##_float(float x_val, float y_val,                                       \
        const char *x_var, const char *y_var, const char *file, int line, bool fail, const char *msg, ...); \
    XAPI void audit_expect_## NAME ##_double(double x_val, double y_val,                                    \
        const char *x_var, const char *y_var, const char *file, int line, bool fail, const char *msg, ...); \
    XAPI void audit_expect_## NAME ##_longdouble(long double x_val, long double y_val,                      \
        const char *x_var, const char *y_var, const char *file, int line, bool fail, const char *msg, ...); \
    XAPI void audit_expect_## NAME ##_ascii(const char *x_val, const char *y_val,                           \
        const char *x_var, const char *y_var, const char *file, int line, bool fail, const char *msg, ...); \
    XAPI void audit_expect_## NAME ##_string(const signed char *x_val, const signed char *y_val,            \
        const char *x_var, const char *y_var, const char *file, int line, bool fail, const char *msg, ...); \
    XAPI void audit_expect_## NAME ##_string8(const unsigned char *x_val, const unsigned char *y_val,       \
        const char *x_var, const char *y_var, const char *file, int line, bool fail, const char *msg, ...); \
    XAPI void audit_expect_## NAME ##_string16(const char16_t *x_val, const char16_t *y_val,                \
        const char *x_var, const char *y_var, const char *file, int line, bool fail, const char *msg, ...); \
    XAPI void audit_expect_## NAME ##_string32(const char32_t *x_val, const char32_t *y_val,                \
        const char *x_var, const char *y_var, const char *file, int line, bool fail, const char *msg, ...); \
    XAPI void audit_expect_## NAME ##_wstring(const wchar_t *x_val, const wchar_t *y_val,                   \
        const char *x_var, const char *y_var, const char *file, int line, bool fail, const char *msg, ...); \
    XAPI void audit_expect_## NAME ##_ptr(const void *x_val, const void *y_val,                             \
        const char *x_var, const char *y_var, const char *file, int line, bool fail, const char *msg, ...);

COMPARE_FUNCS_DEFS(eq)
COMPARE_FUNCS_DEFS(ne)
COMPARE_FUNCS_DEFS(lt)
COMPARE_FUNCS_DEFS(lteq)
COMPARE_FUNCS_DEFS(gt)
COMPARE_FUNCS_DEFS(gteq)

XAPI void audit_expect_eq_float_approx(float x_val, float y_val, float tolerance,
    const char *x_var, const char *y_var, const char *file, int line, bool fail, const char *msg, ...);
XAPI void audit_expect_eq_double_approx(double x_val, double y_val, double tolerance,
    const char *x_var, const char *y_var, const char *file, int line, bool fail, const char *msg, ...);
XAPI void audit_expect_eq_longdouble_approx(long double x_val, long double y_val, long double tolerance,
    const char *x_var, const char *y_var, const char *file, int line, bool fail, const char *msg, ...);
XAPI void audit_expect_ne_float_approx(float x_val, float y_val, float tolerance,
    const char *x_var, const char *y_var, const char *file, int line, bool fail, const char *msg, ...);
XAPI void audit_expect_ne_double_approx(double x_val, double y_val, double tolerance,
    const char *x_var, const char *y_var, const char *file, int line, bool fail, const char *msg, ...);
XAPI void audit_expect_ne_longdouble_approx(long double x_val, long double y_val, long double tolerance,
    const char *x_var, const char *y_var, const char *file, int line, bool fail, const char *msg, ...);

XAPI void audit_expect_eq_bool(_Bool x_val, _Bool y_val,
    const char *x_var, const char *y_var, const char *file, int line, bool fail, const char *msg, ...);
XAPI void audit_expect_ne_bool(_Bool x_val, _Bool y_val,
    const char *x_var, const char *y_var, const char *file, int line, bool fail, const char *msg, ...);

XAPI void audit_expect_null(const void *value, const char *variable, const char *file, int line, bool fail, const char *msg, ...);
XAPI void audit_expect_nonnull(const void *value, const char *variable, const char *file, int line, bool fail, const char *msg, ...);

XAPI void audit_expect_true(_Bool value, const char *variable, const char *file, int line, bool fail, const char *msg, ...);
XAPI void audit_expect_false(_Bool value, const char *variable, const char *file, int line, bool fail, const char *msg, ...);

XAPI void audit_expect_eq_mem(const void *x_value, const void *y_value, size_t x_size, size_t y_size,
    const char *x_variable, const char *y_variable, const char *file, int line, bool fail, const char *msg, ...);
XAPI void audit_expect_ne_mem(const void *x_value, const void *y_value, size_t x_size, size_t y_size,
    const char *x_variable, const char *y_variable, const char *file, int line, bool fail, const char *msg, ...);

XAPI void audit_fail(const char *file, int line, bool fail, const char *fmt, ...);

XAPI void audit_set_test_iterations(const char *name, int count, const char *file, int line);
XAPI int audit_iteration_index(void);

XAPI void audit_fake(const void *src, const void *dst, const char *src_name, const char *dst_name, const char *file, int line);
XAPI void audit_restore(const void *address, const char *name, const char *file, int line);
XAPI int audit_suspend(const void *func, const char *name);
XAPI void audit_reinstate(int id);
XAPI void audit_suspend_mocks(void);
XAPI void audit_restore_mocks(void);

XAPI void audit_stub_bool(const void *src, const char *src_name, _Bool value, const char *value_name, const char *file, int line);
XAPI void audit_stub_char(const void *src, const char *src_name, signed char value, const char *value_name, const char *file, int line);
XAPI void audit_stub_uchar(const void *src, const char *src_name, unsigned char value, const char *value_name, const char *file, int line);
XAPI void audit_stub_short(const void *src, const char *src_name, signed short value, const char *value_name, const char *file, int line);
XAPI void audit_stub_ushort(const void *src, const char *src_name, unsigned short value, const char *value_name, const char *file, int line);
XAPI void audit_stub_int(const void *src, const char *src_name, signed int value, const char *value_name, const char *file, int line);
XAPI void audit_stub_uint(const void *src, const char *src_name, unsigned int value, const char *value_name, const char *file, int line);
XAPI void audit_stub_long(const void *src, const char *src_name, signed long value, const char *value_name, const char *file, int line);
XAPI void audit_stub_ulong(const void *src, const char *src_name, unsigned long value, const char *value_name, const char *file, int line);
XAPI void audit_stub_longlong(const void *src, const char *src_name, signed long long value, const char *value_name, const char *file, int line);
XAPI void audit_stub_ulonglong(const void *src, const char *src_name, unsigned long long value, const char *value_name, const char *file, int line);
XAPI void audit_stub_float(const void *src, const char *src_name, float value, const char *file, const char *value_name, int line);
XAPI void audit_stub_double(const void *src, const char *src_name, double value, const char *file, const char *value_name, int line);
XAPI void audit_stub_longdouble(const void *src, const char *src_name, long double value, const char *value_name, const char *file, int line);
XAPI void audit_stub_pointer(const void *src, const char *src_name, const void *value, const char *value_name, const char *file, int line);


// There are multiple bugs with the implementation of _Generic in the Visual Studio C compiler:
// 
//   - https://developercommunity.visualstudio.com/t/-generic-char-signed-char-unsigned-char-are-not-di/1228885
//   - https://developercommunity.visualstudio.com/t/C11C17-_Generic-selection-incorrectly-h/10560768
//
// Fortunately, some can be worked around. Unfortunately, the workarounds require Audition to define a different
// set of macros specifically appease for Visual Studio. Note that this workaround is not needed for Clang-CL.
#if defined(_MSC_VER) && !defined(__clang__)
#define XUNIT_ASSERT_EQ_STR(X, Y, XS, YS, FAIL, ...)         \
    _Generic((X),                                            \
        signed char *: audit_expect_eq_string,               \
        const signed char *: audit_expect_eq_string,         \
        unsigned char *: audit_expect_eq_string8,            \
        const unsigned char *: audit_expect_eq_string8,      \
        uint_least16_t *: audit_expect_eq_string16,          \
        const uint_least16_t *: audit_expect_eq_string16,    \
        uint_least32_t *: audit_expect_eq_string32,          \
        const uint_least32_t *: audit_expect_eq_string32,    \
        default: audit_expect_eq_wstring                     \
    )(X,Y,XS,YS,__FILE__,__LINE__,FAIL,__VA_ARGS__)

#define XUNIT_ASSERT_NE_STR(X, Y, XS, YS, FAIL, ...)         \
    _Generic((X),                                            \
        signed char *: audit_expect_ne_string,               \
        const signed char *: audit_expect_ne_string,         \
        unsigned char *: audit_expect_ne_string8,            \
        const unsigned char *: audit_expect_ne_string8,      \
        uint_least16_t *: audit_expect_ne_string16,          \
        const uint_least16_t *: audit_expect_ne_string16,    \
        uint_least32_t *: audit_expect_ne_string32,          \
        const uint_least32_t *: audit_expect_ne_string32,    \
        default: audit_expect_ne_wstring                     \
    )(X,Y,XS,YS,__FILE__,__LINE__,FAIL,__VA_ARGS__)

#define XUNIT_ASSERT_LT_STR(X, Y, XS, YS, FAIL, ...)         \
    _Generic((X),                                            \
        signed char *: audit_expect_lt_string,               \
        const signed char *: audit_expect_lt_string,         \
        unsigned char *: audit_expect_lt_string8,            \
        const unsigned char *: audit_expect_lt_string8,      \
        uint_least16_t *: audit_expect_lt_string16,          \
        const uint_least16_t *: audit_expect_lt_string16,    \
        uint_least32_t *: audit_expect_lt_string32,          \
        const uint_least32_t *: audit_expect_lt_string32,    \
        default: audit_expect_lt_wstring                     \
    )(X,Y,XS,YS,__FILE__,__LINE__,FAIL,__VA_ARGS__)

#define XUNIT_ASSERT_LTEQ_STR(X, Y, XS, YS, FAIL, ...)       \
    _Generic((X),                                            \
        signed char *: audit_expect_lteq_string,             \
        const signed char *: audit_expect_lteq_string,       \
        unsigned char *: audit_expect_lteq_string8,          \
        const unsigned char *: audit_expect_lteq_string8,    \
        uint_least16_t *: audit_expect_lteq_string16,        \
        const uint_least16_t *: audit_expect_lteq_string16,  \
        uint_least32_t *: audit_expect_lteq_string32,        \
        const uint_least32_t *: audit_expect_lteq_string32,  \
        default: audit_expect_lteq_wstring                   \
    )(X,Y,XS,YS,__FILE__,__LINE__,FAIL,__VA_ARGS__)

#define XUNIT_ASSERT_GT_STR(X, Y, XS, YS, FAIL, ...)         \
    _Generic((X),                                            \
        signed char *: audit_expect_gt_string,               \
        const signed char *: audit_expect_gt_string,         \
        unsigned char *: audit_expect_gt_string8,            \
        const unsigned char *: audit_expect_gt_string8,      \
        uint_least16_t *: audit_expect_gt_string16,          \
        const uint_least16_t *: audit_expect_gt_string16,    \
        uint_least32_t *: audit_expect_gt_string32,          \
        const uint_least32_t *: audit_expect_gt_string32,    \
        default: audit_expect_gt_wstring                     \
    )(X,Y,XS,YS,__FILE__,__LINE__,FAIL,__VA_ARGS__)

#define XUNIT_ASSERT_GTEQ_STR(X, Y, XS, YS, FAIL, ...)       \
    _Generic((X),                                            \
        signed char *: audit_expect_gteq_string,             \
        const signed char *: audit_expect_gteq_string,       \
        unsigned char *: audit_expect_gteq_string8,          \
        const unsigned char *: audit_expect_gteq_string8,    \
        uint_least16_t *: audit_expect_gteq_string16,        \
        const uint_least16_t *: audit_expect_gteq_string16,  \
        uint_least32_t *: audit_expect_gteq_string32,        \
        const uint_least32_t *: audit_expect_gteq_string32,  \
        default: audit_expect_gteq_wstring                   \
    )(X,Y,XS,YS,__FILE__,__LINE__,FAIL,__VA_ARGS__)
#else
#define XUNIT_ASSERT_EQ_STR(X, Y, XS, YS, FAIL, ...)         \
    _Generic((X),                                            \
        char *: audit_expect_eq_ascii,                       \
        const char *: audit_expect_eq_ascii,                 \
        signed char *: audit_expect_eq_string,               \
        const signed char *: audit_expect_eq_string,         \
        unsigned char *: audit_expect_eq_string8,            \
        const unsigned char *: audit_expect_eq_string8,      \
        uint_least16_t *: audit_expect_eq_string16,          \
        const uint_least16_t *: audit_expect_eq_string16,    \
        uint_least32_t *: audit_expect_eq_string32,          \
        const uint_least32_t *: audit_expect_eq_string32,    \
        default: audit_expect_eq_wstring                     \
    )(X,Y,XS,YS,__FILE__,__LINE__,FAIL,__VA_ARGS__)

#define XUNIT_ASSERT_NE_STR(X, Y, XS, YS, FAIL, ...)         \
    _Generic((X),                                            \
        char *: audit_expect_ne_ascii,                       \
        const char *: audit_expect_ne_ascii,                 \
        signed char *: audit_expect_ne_string,               \
        const signed char *: audit_expect_ne_string,         \
        unsigned char *: audit_expect_ne_string8,            \
        const unsigned char *: audit_expect_ne_string8,      \
        uint_least16_t *: audit_expect_ne_string16,          \
        const uint_least16_t *: audit_expect_ne_string16,    \
        uint_least32_t *: audit_expect_ne_string32,          \
        const uint_least32_t *: audit_expect_ne_string32,    \
        default: audit_expect_ne_wstring                     \
    )(X,Y,XS,YS,__FILE__,__LINE__,FAIL,__VA_ARGS__)

#define XUNIT_ASSERT_LT_STR(X, Y, XS, YS, FAIL, ...)         \
    _Generic((X),                                            \
        char *: audit_expect_lt_ascii,                       \
        const char *: audit_expect_lt_ascii,                 \
        signed char *: audit_expect_lt_string,               \
        const signed char *: audit_expect_lt_string,         \
        unsigned char *: audit_expect_lt_string8,            \
        const unsigned char *: audit_expect_lt_string8,      \
        uint_least16_t *: audit_expect_lt_string16,          \
        const uint_least16_t *: audit_expect_lt_string16,    \
        uint_least32_t *: audit_expect_lt_string32,          \
        const uint_least32_t *: audit_expect_lt_string32,    \
        default: audit_expect_lt_wstring                     \
    )(X,Y,XS,YS,__FILE__,__LINE__,FAIL,__VA_ARGS__)

#define XUNIT_ASSERT_LTEQ_STR(X, Y, XS, YS, FAIL, ...)       \
    _Generic((X),                                            \
        char *: audit_expect_lteq_ascii,                     \
        const char *: audit_expect_lteq_ascii,               \
        signed char *: audit_expect_lteq_string,             \
        const signed char *: audit_expect_lteq_string,       \
        unsigned char *: audit_expect_lteq_string8,          \
        const unsigned char *: audit_expect_lteq_string8,    \
        uint_least16_t *: audit_expect_lteq_string16,        \
        const uint_least16_t *: audit_expect_lteq_string16,  \
        uint_least32_t *: audit_expect_lteq_string32,        \
        const uint_least32_t *: audit_expect_lteq_string32,  \
        default: audit_expect_lteq_wstring                   \
    )(X,Y,XS,YS,__FILE__,__LINE__,FAIL,__VA_ARGS__)

#define XUNIT_ASSERT_GT_STR(X, Y, XS, YS, FAIL, ...)         \
    _Generic((X),                                            \
        char *: audit_expect_gt_ascii,                       \
        const char *: audit_expect_gt_ascii,                 \
        signed char *: audit_expect_gt_string,               \
        const signed char *: audit_expect_gt_string,         \
        unsigned char *: audit_expect_gt_string8,            \
        const unsigned char *: audit_expect_gt_string8,      \
        uint_least16_t *: audit_expect_gt_string16,          \
        const uint_least16_t *: audit_expect_gt_string16,    \
        uint_least32_t *: audit_expect_gt_string32,          \
        const uint_least32_t *: audit_expect_gt_string32,    \
        default: audit_expect_gt_wstring                     \
    )(X,Y,XS,YS,__FILE__,__LINE__,FAIL,__VA_ARGS__)

#define XUNIT_ASSERT_GTEQ_STR(X, Y, XS, YS, FAIL, ...)       \
    _Generic((X),                                            \
        char *: audit_expect_gteq_ascii,                     \
        const char *: audit_expect_gteq_ascii,               \
        signed char *: audit_expect_gteq_string,             \
        const signed char *: audit_expect_gteq_string,       \
        unsigned char *: audit_expect_gteq_string8,          \
        const unsigned char *: audit_expect_gteq_string8,    \
        uint_least16_t *: audit_expect_gteq_string16,        \
        const uint_least16_t *: audit_expect_gteq_string16,  \
        uint_least32_t *: audit_expect_gteq_string32,        \
        const uint_least32_t *: audit_expect_gteq_string32,  \
        default: audit_expect_gteq_wstring                   \
    )(X,Y,XS,YS,__FILE__,__LINE__,FAIL,__VA_ARGS__)
#endif

#define XUNIT_ASSERT_EQ(X, Y, XS, YS, FAIL, ...)             \
    _Generic((X),                                            \
        _Bool: audit_expect_eq_bool,                         \
        signed char: audit_expect_eq_schar,                  \
        unsigned char: audit_expect_eq_uchar,                \
        signed short: audit_expect_eq_sshort,                \
        unsigned short: audit_expect_eq_ushort,              \
        signed int: audit_expect_eq_sint,                    \
        unsigned int: audit_expect_eq_uint,                  \
        signed long: audit_expect_eq_slong,                  \
        unsigned long: audit_expect_eq_ulong,                \
        signed long long: audit_expect_eq_slonglong,         \
        unsigned long long: audit_expect_eq_ulonglong,       \
        float: audit_expect_eq_float,                        \
        double: audit_expect_eq_double,                      \
        long double: audit_expect_eq_longdouble,             \
        default: audit_expect_eq_ptr                         \
    )(X,Y,XS,YS,__FILE__,__LINE__,FAIL,__VA_ARGS__)

#define XUNIT_ASSERT_EQ_APPROX(X, Y, T, XS, YS, FAIL, ...)   \
    _Generic((X),                                            \
        float: audit_expect_eq_float_approx,                 \
        double: audit_expect_eq_double_approx,               \
        long double: audit_expect_eq_longdouble_approx       \
    )(X,Y,T,XS,YS,__FILE__,__LINE__,FAIL,__VA_ARGS__)

#define XUNIT_ASSERT_NE(X, Y, XS, YS, FAIL, ...)             \
    _Generic((X),                                            \
        _Bool: audit_expect_ne_bool,                         \
        signed char: audit_expect_ne_schar,                  \
        unsigned char: audit_expect_ne_uchar,                \
        signed short: audit_expect_ne_sshort,                \
        unsigned short: audit_expect_ne_ushort,              \
        signed int: audit_expect_ne_sint,                    \
        unsigned int: audit_expect_ne_uint,                  \
        signed long: audit_expect_ne_slong,                  \
        unsigned long: audit_expect_ne_ulong,                \
        signed long long: audit_expect_ne_slonglong,         \
        unsigned long long: audit_expect_ne_ulonglong,       \
        float: audit_expect_ne_float,                        \
        double: audit_expect_ne_double,                      \
        long double: audit_expect_ne_longdouble,             \
        default: audit_expect_ne_ptr                         \
    )(X,Y,XS,YS,__FILE__,__LINE__,FAIL,__VA_ARGS__)

#define XUNIT_ASSERT_NE_APPROX(X, Y, T, XS, YS, FAIL, ...)   \
    _Generic((X),                                            \
        float: audit_expect_ne_float_approx,                 \
        double: audit_expect_ne_double_approx,               \
        long double: audit_expect_ne_longdouble_approx       \
    )(X,Y,T,XS,YS,__FILE__,__LINE__,FAIL,__VA_ARGS__)

#define XUNIT_ASSERT_LT(X, Y, XS, YS, FAIL, ...)             \
    _Generic((X),                                            \
        signed char: audit_expect_lt_schar,                  \
        unsigned char: audit_expect_lt_uchar,                \
        signed short: audit_expect_lt_sshort,                \
        unsigned short: audit_expect_lt_ushort,              \
        signed int: audit_expect_lt_sint,                    \
        unsigned int: audit_expect_lt_uint,                  \
        signed long: audit_expect_lt_slong,                  \
        unsigned long: audit_expect_lt_ulong,                \
        signed long long: audit_expect_lt_slonglong,         \
        unsigned long long: audit_expect_lt_ulonglong,       \
        float: audit_expect_lt_float,                        \
        double: audit_expect_lt_double,                      \
        long double: audit_expect_lt_longdouble,             \
        default: audit_expect_lt_ptr                         \
    )(X,Y,XS,YS,__FILE__,__LINE__,FAIL,__VA_ARGS__)

#define XUNIT_ASSERT_LTEQ(X, Y, XS, YS, FAIL, ...)           \
    _Generic((X),                                            \
        signed char: audit_expect_lteq_schar,                \
        unsigned char: audit_expect_lteq_uchar,              \
        signed short: audit_expect_lteq_sshort,              \
        unsigned short: audit_expect_lteq_ushort,            \
        signed int: audit_expect_lteq_sint,                  \
        unsigned int: audit_expect_lteq_uint,                \
        signed long: audit_expect_lteq_slong,                \
        unsigned long: audit_expect_lteq_ulong,              \
        signed long long: audit_expect_lteq_slonglong,       \
        unsigned long long: audit_expect_lteq_ulonglong,     \
        float: audit_expect_lteq_float,                      \
        double: audit_expect_lteq_double,                    \
        long double: audit_expect_lteq_longdouble,           \
        default: audit_expect_lteq_ptr                       \
    )(X,Y,XS,YS,__FILE__,__LINE__,FAIL,__VA_ARGS__)

#define XUNIT_ASSERT_GT(X, Y, XS, YS, FAIL, ...)             \
    _Generic((X),                                            \
        signed char: audit_expect_gt_schar,                  \
        unsigned char: audit_expect_gt_uchar,                \
        signed short: audit_expect_gt_sshort,                \
        unsigned short: audit_expect_gt_ushort,              \
        signed int: audit_expect_gt_sint,                    \
        unsigned int: audit_expect_gt_uint,                  \
        signed long: audit_expect_gt_slong,                  \
        unsigned long: audit_expect_gt_ulong,                \
        signed long long: audit_expect_gt_slonglong,         \
        unsigned long long: audit_expect_gt_ulonglong,       \
        float: audit_expect_gt_float,                        \
        double: audit_expect_gt_double,                      \
        long double: audit_expect_gt_longdouble,             \
        default: audit_expect_gt_ptr                         \
    )(X,Y,XS,YS,__FILE__,__LINE__,FAIL,__VA_ARGS__)

#define XUNIT_ASSERT_GTEQ(X, Y, XS, YS, FAIL, ...)           \
    _Generic((X),                                            \
        signed char: audit_expect_gteq_schar,                \
        unsigned char: audit_expect_gteq_uchar,              \
        signed short: audit_expect_gteq_sshort,              \
        unsigned short: audit_expect_gteq_ushort,            \
        signed int: audit_expect_gteq_sint,                  \
        unsigned int: audit_expect_gteq_uint,                \
        signed long: audit_expect_gteq_slong,                \
        unsigned long: audit_expect_gteq_ulong,              \
        signed long long: audit_expect_gteq_slonglong,       \
        unsigned long long: audit_expect_gteq_ulonglong,     \
        float: audit_expect_gteq_float,                      \
        double: audit_expect_gteq_double,                    \
        long double: audit_expect_gteq_longdouble,           \
        default: audit_expect_gteq_ptr                       \
    )(X,Y,XS,YS,__FILE__,__LINE__,FAIL,__VA_ARGS__)

#define XUNIT_HEADER_MAGIC 0x7E57C0DE4D3FEC75

#endif
