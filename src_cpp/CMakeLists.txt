cmake_minimum_required(VERSION 3.10)
project(pgvector_cpp VERSION 1.0.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

# Source files
set(SOURCES
    Vector.cpp
    VectorFactory.cpp
    VectorIndex.cpp
    VectorIndexFactory.cpp
    VectorStorage.cpp
    VectorQueryProcessor.cpp
)

# Header files
set(HEADERS
    Vector.hpp
    VectorFactory.hpp
    VectorIndex.hpp
    VectorIndexFactory.hpp
    VectorStorage.hpp
    VectorQueryProcessor.hpp
    pgvector.hpp
)

# Create static library
add_library(pgvector_cpp_static STATIC ${SOURCES})
target_include_directories(pgvector_cpp_static PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
set_target_properties(pgvector_cpp_static PROPERTIES OUTPUT_NAME pgvector_cpp)

# Create shared library
add_library(pgvector_cpp_shared SHARED ${SOURCES})
target_include_directories(pgvector_cpp_shared PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
set_target_properties(pgvector_cpp_shared PROPERTIES OUTPUT_NAME pgvector_cpp)

# Installation
install(TARGETS pgvector_cpp_static pgvector_cpp_shared
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)

install(FILES ${HEADERS}
    DESTINATION include/pgvector
)

# Tests
option(BUILD_TESTS "Build tests" ON)
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
