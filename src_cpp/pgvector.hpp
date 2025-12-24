#ifndef PGVECTOR_HPP
#define PGVECTOR_HPP

// Main header file for pgvector C++ library

#include "Vector.hpp"
#include "VectorFactory.hpp"
#include "VectorIndex.hpp"
#include "VectorIndexFactory.hpp"
#include "VectorStorage.hpp"
#include "VectorQueryProcessor.hpp"

namespace pgvector {

// Version information
constexpr const char* VERSION = "1.0.0";
constexpr int VERSION_MAJOR = 1;
constexpr int VERSION_MINOR = 0;
constexpr int VERSION_PATCH = 0;

} // namespace pgvector

#endif // PGVECTOR_HPP
