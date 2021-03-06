add_subdirectory(util)

# AFL not yet supported by Windows
if(NOT CMAKE_SYSTEM_NAME MATCHES "^Windows")
    add_subdirectory(fuzzy)
endif()

add_subdirectory(benchmark-common-tasks)
add_subdirectory(benchmark-crud)
# FIXME: Add other benchmarks

set(NORMAL_TESTS
    test_all.cpp
    test_alloc.cpp
    test_array.cpp
    test_array_binary.cpp
    test_array_blob.cpp
    test_array_blobs_big.cpp
    test_array_float.cpp
    test_array_integer.cpp
    test_array_string.cpp
    test_array_string_long.cpp
    test_basic_utils.cpp
    test_binary_data.cpp
    test_column.cpp
    test_column_binary.cpp
    test_column_float.cpp
    test_column_mixed.cpp
    test_column_string.cpp
    test_column_timestamp.cpp
    test_descriptor.cpp
    test_destroy_guard.cpp
    test_destructor_thread_safety.cpp
    test_file.cpp
    test_file_locks.cpp
    test_group.cpp
    test_impl_simulated_failure.cpp
    test_index_string.cpp
    test_json.cpp
    test_lang_bind_helper.cpp
    test_link_query_view.cpp
    test_links.cpp
    test_metrics.cpp
    test_optional.cpp
    test_parser.cpp
    test_priority_queue.cpp
    test_query.cpp
    test_replication.cpp
    test_safe_int_ops.cpp
    test_self.cpp
    test_shared.cpp
    test_string_data.cpp
    test_table.cpp
    test_table_view.cpp
    test_thread.cpp
    test_transactions.cpp
    test_transactions_lasse.cpp
    test_upgrade_database.cpp
    test_utf8.cpp
    test_util_any.cpp
    test_util_base64.cpp
    test_util_error.cpp
    test_util_file.cpp
    test_util_inspect.cpp
    test_util_logger.cpp
    test_util_memory_stream.cpp
    test_util_overload.cpp
    test_util_scope_exit.cpp
    test_util_stringbuffer.cpp
    test_util_to_string.cpp
    test_util_type_list.cpp
    test_version.cpp)

if (REALM_ENABLE_ENCRYPTION)
	list(APPEND NORMAL_TESTS test_encrypted_file_mapping.cpp)
endif()

set(LARGE_TESTS
    large_tests/test_column_large.cpp
    large_tests/test_strings.cpp)

set(REALM_TEST_HEADERS
    fuzz_group.hpp
    pthread_test.hpp
    test.hpp
    test_all.hpp
    test_string_types.hpp
    test_table_helper.hpp
    testsettings.hpp
) # REALM_TEST_HEADERS

set(FUZZY_TESTS fuzz_group.cpp)

set(TESTS ${NORMAL_TESTS} ${LARGE_TESTS} ${FUZZY_TESTS})

if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux" AND NOT ANDROID)
    include(Findprocps)
    set(EXTRA_TEST_LIBS ${PROCPS_LIBRARY} rt)
    set(EXTRA_TEST_CFLAGS ${PROCPS_INCLUDE_DIR})
endif()

# Resources required for running the tests
file(GLOB REQUIRED_TEST_FILES
     "*.json"
     "*.realm"
     "expect_string.txt")
file(COPY ${REQUIRED_TEST_FILES}
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# FIXME: Benchmarks

if(CMAKE_GENERATOR STREQUAL Xcode)
    set(MAIN_FILE main.mm)
else()
    set(MAIN_FILE main.cpp)
endif()

if (MSVC)
    set_source_files_properties(test_query.cpp PROPERTIES COMPILE_FLAGS /bigobj)
endif()

add_executable(CoreTests ${TESTS} ${MAIN_FILE} ${REQUIRED_TEST_FILES} ${REALM_TEST_HEADERS})
set_target_properties(CoreTests PROPERTIES OUTPUT_NAME "realm-tests")

if(CMAKE_GENERATOR STREQUAL Xcode)
    set_target_properties(CoreTests PROPERTIES
                          MACOSX_BUNDLE YES
                          RESOURCE "${REQUIRED_TEST_FILES}")
endif()

target_link_libraries(CoreTests
                      TestUtil QueryParser ${EXTRA_TEST_LIBS} ${PLATFORM_LIBRARIES}
)

target_include_directories(CoreTests PUBLIC ${EXTRA_TEST_CFLAGS})

add_test(NAME RealmTests COMMAND realm-tests)

