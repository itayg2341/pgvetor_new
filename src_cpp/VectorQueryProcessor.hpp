#ifndef VECTOR_QUERY_PROCESSOR_HPP
#define VECTOR_QUERY_PROCESSOR_HPP

#include "Vector.hpp"
#include "VectorIndex.hpp"
#include "VectorStorage.hpp"
#include <memory>
#include <vector>
#include <string>

namespace pgvector {

class VectorQueryProcessor {
private:
    std::shared_ptr<VectorStorage> storage_;
    std::shared_ptr<VectorIndex> index_;
    
public:
    VectorQueryProcessor(std::shared_ptr<VectorStorage> storage, 
                        std::shared_ptr<VectorIndex> index);
    ~VectorQueryProcessor() = default;
    
    // Insert a vector and update index
    int64_t insertVector(std::shared_ptr<Vector> vector);
    
    // Delete a vector and update index
    bool deleteVector(int64_t id);
    
    // Update a vector and update index
    bool updateVector(int64_t id, std::shared_ptr<Vector> vector);
    
    // Search for similar vectors
    std::vector<SearchResult> searchSimilar(const Vector& query, int k) const;
    
    // Search with distance threshold
    std::vector<SearchResult> searchWithinDistance(const Vector& query, double max_distance) const;
    
    // Get vector by ID
    std::shared_ptr<Vector> getVector(int64_t id) const;
    
    // Batch operations
    std::vector<int64_t> insertVectors(const std::vector<std::shared_ptr<Vector>>& vectors);
    std::vector<SearchResult> batchSearch(const std::vector<Vector>& queries, int k) const;
    
    // Statistics
    struct ProcessorStats {
        size_t storage_size;
        size_t index_size;
        DistanceType distance_type;
        double avg_query_time_ms; // Will be calculated later
    };
    
    ProcessorStats getStats() const;
    
    // Rebuild index (useful after many updates/deletes)
    void rebuildIndex();
    
    // Change distance type
    void changeDistanceType(DistanceType new_type);
};

} // namespace pgvector

#endif // VECTOR_QUERY_PROCESSOR_HPP
