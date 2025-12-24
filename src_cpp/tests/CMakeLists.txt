# Test executable
add_executable(vector_tests
    test_main.cpp
    test_vector.cpp
    test_vector_factory.cpp
    test_vector_index.cpp
    test_vector_storage.cpp
    test_vector_query_processor.cpp
)

target_link_libraries(vector_tests pgvector_cpp_static)

# Add test
add_test(NAME vector_tests COMMAND vector_tests)
