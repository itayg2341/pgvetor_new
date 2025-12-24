# pgvector C++ Library

A C++ implementation of the pgvector PostgreSQL extension for vector similarity search.

## Overview

This library provides a complete C++ implementation of vector operations, storage, indexing, and query processing for high-dimensional vector similarity search. It supports multiple distance metrics including L2 (Euclidean), inner product, and cosine distance.

## Features

- **Vector Operations**: Complete vector arithmetic with L2, inner product, and cosine distance calculations
- **Vector Storage**: Thread-safe storage with automatic ID management
- **Vector Indexing**: Efficient indexing for similarity search
- **Query Processing**: High-level interface for vector operations
- **Multiple Distance Types**: Support for L2, inner product, and cosine distance
- **Factory Pattern**: Easy creation of vectors and indexes
- **Thread Safety**: Safe concurrent access to storage
- **Comprehensive Testing**: Full test suite with 94 test cases

## Architecture

The library is organized into the following main components:

### Core Classes

1. **Vector**: Represents a high-dimensional vector with distance calculations
2. **VectorFactory**: Factory for creating vectors from various sources
3. **VectorIndex**: Index for efficient similarity search
4. **VectorIndexFactory**: Factory for creating indexes with different distance types
5. **VectorStorage**: Thread-safe storage for vectors with automatic ID management
6. **VectorQueryProcessor**: High-level interface for vector operations

### Distance Types

- **L2 (Euclidean)**: Standard Euclidean distance
- **Inner Product**: Negative inner product (for similarity search)
- **Cosine**: Cosine distance (1 - cosine similarity)

## Building

### Requirements

- C++17 compatible compiler (GCC 7+, Clang 5+, MSVC 2017+)
- CMake 3.10+ (optional, for CMake build)
- Make (for Makefile build)

### Build with Makefile

```bash
cd src_cpp
make all
make test  # Run tests
```

### Build with CMake

```bash
cd src_cpp
mkdir build
cd build
cmake ..
make
make test
```

## Usage

### Basic Example

```cpp
#include "pgvector.hpp"
#include <iostream>
#include <memory>

using namespace pgvector;

int main() {
    // Create storage and index
    auto storage = std::make_shared<VectorStorage>();
    auto index = VectorIndexFactory::createIndex(DistanceType::L2);
    VectorQueryProcessor processor(storage, index);
    
    // Insert vectors
    auto v1 = VectorFactory::createVectorFromString("1.0, 2.0, 3.0");
    auto v2 = VectorFactory::createVectorFromString("4.0, 5.0, 6.0");
    
    int64_t id1 = processor.insertVector(v1);
    int64_t id2 = processor.insertVector(v2);
    
    // Search for similar vectors
    auto query = VectorFactory::createVectorFromString("1.1, 2.1, 3.1");
    auto results = processor.searchSimilar(*query, 1);
    
    std::cout << "Most similar vector: ID=" << results[0].id 
              << ", Distance=" << results[0].distance << std::endl;
    
    return 0;
}
```

### Advanced Usage

```cpp
// Create custom vectors
std::vector<float> data = {1.0f, 2.0f, 3.0f};
auto vector = std::make_shared<Vector>(3, data);

// Manual distance calculations
double l2_dist = vector1->l2SquaredDistance(vector2);
double ip = vector1->innerProduct(vector2);
double cosine_sim = vector1->cosineSimilarity(vector2);

// Batch operations
std::vector<std::shared_ptr<Vector>> vectors = {v1, v2, v3};
auto ids = processor.insertVectors(vectors);

// Search within distance threshold
auto nearby = processor.searchWithinDistance(*query, 5.0);
```

## API Reference

### Vector Class

- `Vector(int16_t dim)`: Create zero vector
- `Vector(int16_t dim, const std::vector<float>& data)`: Create vector with data
- `l2SquaredDistance(const Vector& other)`: Calculate L2 squared distance
- `innerProduct(const Vector& other)`: Calculate inner product
- `cosineSimilarity(const Vector& other)`: Calculate cosine similarity
- `cosineDistance(const Vector& other)`: Calculate cosine distance

### VectorFactory Class

- `createVector(int16_t dim, const float* data)`: Create from raw data
- `createVectorFromString(const std::string& str)`: Create from string
- `createZeroVector(int16_t dim)`: Create zero vector
- `createRandomVector(int16_t dim, float min, float max)`: Create random vector

### VectorIndex Class

- `addVector(int64_t id, std::shared_ptr<Vector> vector)`: Add vector to index
- `removeVector(int64_t id)`: Remove vector from index
- `search(const Vector& query, int k)`: Search for k nearest neighbors

### VectorStorage Class

- `addVector(std::shared_ptr<Vector> vector)`: Add vector, return auto-generated ID
- `getVector(int64_t id)`: Retrieve vector by ID
- `removeVector(int64_t id)`: Remove vector by ID
- `updateVector(int64_t id, std::shared_ptr<Vector> vector)`: Update vector

### VectorQueryProcessor Class

- `insertVector(std::shared_ptr<Vector> vector)`: Insert vector
- `deleteVector(int64_t id)`: Delete vector
- `searchSimilar(const Vector& query, int k)`: Search for similar vectors
- `searchWithinDistance(const Vector& query, double max_distance)`: Search within distance
- `changeDistanceType(DistanceType new_type)`: Change distance metric

## Thread Safety

- `VectorStorage` is thread-safe for all operations
- `VectorIndex` is thread-safe for read operations, but not for concurrent read/write
- `VectorQueryProcessor` is thread-safe for read operations

## Performance

The library is designed for performance with:
- Efficient vector operations using optimized algorithms
- Minimal memory overhead
- Fast similarity search with early termination
- Support for future optimizations (e.g., SIMD, GPU acceleration)

## Testing

The library includes a comprehensive test suite with 94 test cases covering:
- Vector operations and distance calculations
- Factory pattern implementations
- Index operations and search
- Storage operations
- Query processor functionality
- Error handling and edge cases

Run tests with:
```bash
make test
```

## Future Enhancements

- SIMD optimizations for vector operations
- GPU acceleration support
- Advanced indexing algorithms (HNSW, IVF)
- Persistent storage backend
- Integration with existing PostgreSQL extension

## License

This implementation follows the same license as the original pgvector project.
