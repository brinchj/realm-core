set(TEST_UTIL_SOURCES
    benchmark_results.cpp
    demangle.cpp
    mem.cpp
    timer.cpp
    random.cpp
    quote.cpp
    wildcard.cpp
    unit_test.cpp
    test_path.cpp
    test_only.cpp
    crypt_key.cpp
    misc.cpp
    verified_integer.cpp
    verified_string.cpp
    resource_limits.cpp
    jsmn.cpp
) # TEST_UTIL_SOURCES

add_library(TestUtil ${TEST_UTIL_SOURCES})

target_link_libraries(TestUtil Core)

