#include "VectorFactory.hpp"
#include <sstream>
#include <random>
#include <algorithm>

namespace pgvector {

std::unique_ptr<Vector> VectorFactory::createVector(int16_t dim, const float* data) {
    std::vector<float> vec_data(data, data + dim);
    return std::make_unique<Vector>(dim, vec_data);
}

std::unique_ptr<Vector> VectorFactory::createVectorFromString(const std::string& str) {
    std::istringstream iss(str);
    std::vector<float> data;
    float value;
    
    // Parse comma-separated values
    while (iss >> value) {
        data.push_back(value);
        if (iss.peek() == ',') {
            iss.ignore();
        }
    }
    
    if (data.empty()) {
        throw std::invalid_argument("Empty vector string");
    }
    
    if (data.size() > INT16_MAX) {
        throw std::invalid_argument("Vector dimension too large");
    }
    
    int16_t dim = static_cast<int16_t>(data.size());
    return std::make_unique<Vector>(dim, data);
}

std::unique_ptr<Vector> VectorFactory::createZeroVector(int16_t dim) {
    return std::make_unique<Vector>(dim);
}

std::unique_ptr<Vector> VectorFactory::createRandomVector(int16_t dim, float min, float max) {
    auto vector = std::make_unique<Vector>(dim);
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(min, max);
    
    for (int i = 0; i < dim; ++i) {
        (*vector)[i] = dis(gen);
    }
    
    return vector;
}

} // namespace pgvector
