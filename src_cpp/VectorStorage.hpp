#ifndef VECTOR_STORAGE_HPP
#define VECTOR_STORAGE_HPP

#include "Vector.hpp"
#include <vector>
#include <memory>
#include <unordered_map>
#include <mutex>
#include <shared_mutex>

namespace pgvector {

class VectorStorage {
private:
    std::unordered_map<int64_t, std::shared_ptr<Vector>> vectors_;
    mutable std::shared_mutex mutex_;
    int64_t next_id_;

public:
    VectorStorage() : next_id_(1) {}
    ~VectorStorage() = default;
    
    // Add a vector and return its ID
    int64_t addVector(std::shared_ptr<Vector> vector);
    
    // Add a vector with specific ID
    bool addVectorWithId(int64_t id, std::shared_ptr<Vector> vector);
    
    // Get a vector by ID
    std::shared_ptr<Vector> getVector(int64_t id) const;
    
    // Remove a vector by ID
    bool removeVector(int64_t id);
    
    // Check if a vector exists
    bool hasVector(int64_t id) const;
    
    // Get all vector IDs
    std::vector<int64_t> getAllIds() const;
    
    // Get the number of vectors
    size_t size() const;
    
    // Clear all vectors
    void clear();
    
    // Update a vector
    bool updateVector(int64_t id, std::shared_ptr<Vector> vector);
    
    // Get statistics
    struct StorageStats {
        size_t count;
        size_t totalMemoryUsage; // Approximate
        int64_t maxId;
        int64_t minId;
    };
    
    StorageStats getStats() const;
};

} // namespace pgvector

#endif // VECTOR_STORAGE_HPP
