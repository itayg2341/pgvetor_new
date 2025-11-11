# Plan for Converting C to C++

This document outlines the plan for converting the existing C codebase to C++. The goal is to create a modern, object-oriented C++ implementation while maintaining the functionality of the original code.

## 1. Project Structure

- The new C++ code will be placed in the `src_cpp` directory.
- The original C code in the `src` directory will be preserved for reference.
- The build system will be updated to compile the C++ code and link it with the existing codebase.

## 2. C++ Conversion Strategy

The conversion will be done in a phased approach, starting with the core data structures and then moving to the indexing and search algorithms.

### Phase 1: Core Data Structures

1. **Vector:**
   - Create a `Vector` class in `src_cpp/vector.hpp` and `src_cpp/vector.cpp`.
   - The `Vector` class will encapsulate the data and provide methods for operations like distance calculations, normalization, and comparisons.
   - The `Vector` class will replace the `Vector` struct in `src/vector.h`.
   - Create a C-style wrapper API to maintain backward compatibility with the existing C code.

2. **HalfVector:**
   - Create a `HalfVector` class in `src_cpp/half_vector.hpp` and `src_cpp/half_vector.cpp`.
   - The `HalfVector` class will handle the half-precision floating-point data.
   - It will provide similar methods to the `Vector` class.

3. **SparseVector:**
   - Create a `SparseVector` class in `src_cpp/sparse_vector.hpp` and `src_cpp/sparse_vector.cpp`.
   - The `SparseVector` class will manage the sparse vector data, including indices and values.
   - It will provide methods for sparse vector operations.

4. **BitVector:**
   - Create a `BitVector` class in `src_cpp/bit_vector.hpp` and `src_cpp/bit_vector.cpp`.
   - The `BitVector` class will handle bit-level data and operations.

### Phase 2: Indexing and Search Algorithms

1. **HNSW:**
   - Create an `HNSW` class in `src_cpp/hnsw.hpp` and `src_cpp/hnsw.cpp`.
   - The `HNSW` class will encapsulate the HNSW graph, search state, and build state.
   - It will provide methods for building the index, inserting elements, and searching.
   - This class will replace the HNSW-related functions in `src/hnsw.c`.

2. **IVFFlat:**
   - Create an `IVFFlat` class in `src_cpp/ivfflat.hpp` and `src_cpp/ivfflat.cpp`.
   - The `IVFFlat` class will manage the IVFFlat index, including the lists, centers, and build state.
   - It will provide methods for building the index, inserting elements, and searching.
   - This class will replace the IVFFlat-related functions in `src/ivfflat.c`.

### Phase 3: Integration and Testing

1. **C Wrapper API:**
   - Create a C-style wrapper API for the C++ classes.
   - This API will be used to integrate the new C++ code with the existing C code.
   - The wrapper functions will be placed in separate files, such as `src_cpp/c_api.cpp`.

2. **Build System:**
   - Update the `Makefile` to compile the C++ code and create a library.
   - The existing C code will be linked with the new C++ library.

3. **Testing:**
   - Create a new set of tests for the C++ code using a C++ testing framework like Google Test.
   - The existing tests will be used to verify the functionality of the C++ code through the C wrapper API.

## 3. Detailed Steps

1. **Create the `src_cpp` directory.** (Done)
2. **Create the `Vector` class.**
3. **Create the `HalfVector` class.**
4. **Create the `SparseVector` class.**
5. **Create the `BitVector` class.**
6. **Create the `HNSW` class.**
7. **Create the `IVFFlat` class.**
8. **Create the C wrapper API.**
9. **Update the `Makefile`.**
10. **Create new C++ tests.**
11. **Run existing tests to ensure backward compatibility.**
