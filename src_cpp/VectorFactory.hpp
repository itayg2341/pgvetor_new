#ifndef VECTOR_FACTORY_HPP
#define VECTOR_FACTORY_HPP

#include "Vector.hpp"
#include <memory>
#include <string>

namespace pgvector {

class VectorFactory {
public:
    // Create a vector from raw data
    static std::unique_ptr<Vector> createVector(int16_t dim, const float* data);
    
    // Create a vector from string representation
    static std::unique_ptr<Vector> createVectorFromString(const std::string& str);
    
    // Create a zero vector
    static std::unique_ptr<Vector> createZeroVector(int16_t dim);
    
    // Create a random vector (for testing)
    static std::unique_ptr<Vector> createRandomVector(int16_t dim, float min = -1.0f, float max = 1.0f);
    
private:
    VectorFactory() = delete; // Static class, no instantiation
};

} // namespace pgvector

#endif // VECTOR_FACTORY_HPP
