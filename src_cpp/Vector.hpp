#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <vector>
#include <memory>
#include <cmath>
#include <stdexcept>
#include <string>

namespace pgvector {

class Vector {
private:
    std::vector<float> data_;
    int16_t dim_;

public:
    // Constructor
    explicit Vector(int16_t dim) : data_(dim), dim_(dim) {}
    
    // Constructor with data
    Vector(int16_t dim, const std::vector<float>& data) : data_(data), dim_(dim) {
        if (data.size() != static_cast<size_t>(dim)) {
            throw std::invalid_argument("Data size does not match dimension");
        }
    }
    
    // Copy constructor
    Vector(const Vector& other) = default;
    
    // Move constructor
    Vector(Vector&& other) noexcept = default;
    
    // Destructor
    ~Vector() = default;
    
    // Assignment operators
    Vector& operator=(const Vector& other) = default;
    Vector& operator=(Vector&& other) noexcept = default;
    
    // Getters
    int16_t getDimension() const { return dim_; }
    const std::vector<float>& getData() const { return data_; }
    std::vector<float>& getData() { return data_; }
    
    // Element access
    float operator[](size_t index) const {
        if (index >= data_.size()) {
            throw std::out_of_range("Index out of range");
        }
        return data_[index];
    }
    
    float& operator[](size_t index) {
        if (index >= data_.size()) {
            throw std::out_of_range("Index out of range");
        }
        return data_[index];
    }
    
    // Distance functions
    double l2SquaredDistance(const Vector& other) const;
    double innerProduct(const Vector& other) const;
    double cosineSimilarity(const Vector& other) const;
    double cosineDistance(const Vector& other) const;
    
    // Utility functions
    void checkDimensions(const Vector& other) const;
    std::string toString() const;
    
    // Comparison operators
    bool operator==(const Vector& other) const;
    bool operator!=(const Vector& other) const;
    bool operator<(const Vector& other) const;
    bool operator>(const Vector& other) const;
    bool operator<=(const Vector& other) const;
    bool operator>=(const Vector& other) const;
};

} // namespace pgvector

#endif // VECTOR_HPP
