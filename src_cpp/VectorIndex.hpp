#ifndef VECTOR_INDEX_HPP
#define VECTOR_INDEX_HPP

#include "Vector.hpp"
#include <vector>
#include <memory>
#include <functional>
#include <unordered_map>

namespace pgvector {

enum class DistanceType {
    L2,          // Euclidean distance
    INNER_PRODUCT, // Inner product
    COSINE       // Cosine distance
};

struct VectorWithId {
    int64_t id;
    std::shared_ptr<Vector> vector;
    
    VectorWithId(int64_t id, std::shared_ptr<Vector> vec) : id(id), vector(vec) {}
};

struct SearchResult {
    int64_t id;
    double distance;
    
    SearchResult(int64_t id, double distance) : id(id), distance(distance) {}
    SearchResult() = default;
};

class VectorIndex {
protected:
    DistanceType distance_type_;
    std::vector<VectorWithId> vectors_;
    
    // Distance function based on distance type
    std::function<double(const Vector&, const Vector&)> getDistanceFunction() const;
    
public:
    explicit VectorIndex(DistanceType distance_type) : distance_type_(distance_type) {}
    virtual ~VectorIndex() = default;
    
    // Add a vector to the index
    virtual void addVector(int64_t id, std::shared_ptr<Vector> vector);
    
    // Remove a vector from the index
    virtual bool removeVector(int64_t id);
    
    // Search for k nearest neighbors
    virtual std::vector<SearchResult> search(const Vector& query, int k) const;
    
    // Get the number of vectors in the index
    size_t size() const { return vectors_.size(); }
    
    // Clear all vectors
    void clear() { vectors_.clear(); }
    
    // Get distance type
    DistanceType getDistanceType() const { return distance_type_; }
    
    // Virtual function for derived classes to implement optimized search
    virtual std::vector<SearchResult> searchOptimized(const Vector& query, int k) const;
};

} // namespace pgvector

#endif // VECTOR_INDEX_HPP
