#include "Vector.hpp"
#include <sstream>
#include <algorithm>
#include <numeric>

namespace pgvector {

void Vector::checkDimensions(const Vector& other) const {
    if (dim_ != other.dim_) {
        throw std::invalid_argument("Different vector dimensions " + 
                                   std::to_string(dim_) + " and " + 
                                   std::to_string(other.dim_));
    }
}

double Vector::l2SquaredDistance(const Vector& other) const {
    checkDimensions(other);
    
    double distance = 0.0;
    for (int i = 0; i < dim_; ++i) {
        double diff = data_[i] - other.data_[i];
        distance += diff * diff;
    }
    return distance;
}

double Vector::innerProduct(const Vector& other) const {
    checkDimensions(other);
    
    double product = 0.0;
    for (int i = 0; i < dim_; ++i) {
        product += data_[i] * other.data_[i];
    }
    return product;
}

double Vector::cosineSimilarity(const Vector& other) const {
    checkDimensions(other);
    
    double similarity = 0.0;
    double norma = 0.0;
    double normb = 0.0;
    
    for (int i = 0; i < dim_; ++i) {
        similarity += data_[i] * other.data_[i];
        norma += data_[i] * data_[i];
        normb += other.data_[i] * other.data_[i];
    }
    
    // Use sqrt(a * b) over sqrt(a) * sqrt(b)
    return similarity / std::sqrt(norma * normb);
}

double Vector::cosineDistance(const Vector& other) const {
    double similarity = cosineSimilarity(other);
    
    // Keep in range
    if (similarity > 1.0) similarity = 1.0;
    if (similarity < -1.0) similarity = -1.0;
    
    return 1.0 - similarity;
}

std::string Vector::toString() const {
    std::ostringstream oss;
    oss << "Vector(" << dim_ << "): [";
    for (int i = 0; i < dim_; ++i) {
        if (i > 0) oss << ", ";
        oss << data_[i];
    }
    oss << "]";
    return oss.str();
}

bool Vector::operator==(const Vector& other) const {
    if (dim_ != other.dim_) return false;
    return data_ == other.data_;
}

bool Vector::operator!=(const Vector& other) const {
    return !(*this == other);
}

bool Vector::operator<(const Vector& other) const {
    checkDimensions(other);
    return std::lexicographical_compare(data_.begin(), data_.end(), 
                                       other.data_.begin(), other.data_.end());
}

bool Vector::operator>(const Vector& other) const {
    return other < *this;
}

bool Vector::operator<=(const Vector& other) const {
    return !(other < *this);
}

bool Vector::operator>=(const Vector& other) const {
    return !(*this < other);
}

} // namespace pgvector
