#include "VectorQueryProcessor.hpp"
#include <chrono>
#include <algorithm>

namespace pgvector {

VectorQueryProcessor::VectorQueryProcessor(std::shared_ptr<VectorStorage> storage, 
                                         std::shared_ptr<VectorIndex> index)
    : storage_(storage), index_(index) {
    if (!storage_) {
        throw std::invalid_argument("Storage cannot be null");
    }
    if (!index_) {
        throw std::invalid_argument("Index cannot be null");
    }
}

int64_t VectorQueryProcessor::insertVector(std::shared_ptr<Vector> vector) {
    // Insert into storage
    int64_t id = storage_->addVector(vector);
    
    // Add to index
    index_->addVector(id, vector);
    
    return id;
}

bool VectorQueryProcessor::deleteVector(int64_t id) {
    // Remove from index first
    index_->removeVector(id);
    
    // Remove from storage
    return storage_->removeVector(id);
}

bool VectorQueryProcessor::updateVector(int64_t id, std::shared_ptr<Vector> vector) {
    // Remove old vector from index
    index_->removeVector(id);
    
    // Update in storage
    if (!storage_->updateVector(id, vector)) {
        return false;
    }
    
    // Add new vector to index
    index_->addVector(id, vector);
    
    return true;
}

std::vector<SearchResult> VectorQueryProcessor::searchSimilar(const Vector& query, int k) const {
    return index_->search(query, k);
}

std::vector<SearchResult> VectorQueryProcessor::searchWithinDistance(const Vector& query, double max_distance) const {
    auto all_results = index_->search(query, storage_->size());
    
    std::vector<SearchResult> filtered_results;
    for (const auto& result : all_results) {
        if (result.distance <= max_distance) {
            filtered_results.push_back(result);
        }
    }
    
    return filtered_results;
}

std::shared_ptr<Vector> VectorQueryProcessor::getVector(int64_t id) const {
    return storage_->getVector(id);
}

std::vector<int64_t> VectorQueryProcessor::insertVectors(const std::vector<std::shared_ptr<Vector>>& vectors) {
    std::vector<int64_t> ids;
    ids.reserve(vectors.size());
    
    for (const auto& vector : vectors) {
        ids.push_back(insertVector(vector));
    }
    
    return ids;
}

std::vector<SearchResult> VectorQueryProcessor::batchSearch(const std::vector<Vector>& queries, int k) const {
    std::vector<SearchResult> all_results;
    
    for (const auto& query : queries) {
        auto results = searchSimilar(query, k);
        all_results.insert(all_results.end(), results.begin(), results.end());
    }
    
    return all_results;
}

VectorQueryProcessor::ProcessorStats VectorQueryProcessor::getStats() const {
    ProcessorStats stats;
    stats.storage_size = storage_->size();
    stats.index_size = index_->size();
    stats.distance_type = index_->getDistanceType();
    stats.avg_query_time_ms = 0.0; // Will be calculated later with actual timing
    
    return stats;
}

void VectorQueryProcessor::rebuildIndex() {
    // Clear current index
    index_->clear();
    
    // Get all vectors from storage
    auto all_ids = storage_->getAllIds();
    
    // Re-add all vectors to index
    for (int64_t id : all_ids) {
        auto vector = storage_->getVector(id);
        if (vector) {
            index_->addVector(id, vector);
        }
    }
}

void VectorQueryProcessor::changeDistanceType(DistanceType new_type) {
    // Create new index with new distance type
    auto new_index = std::make_shared<VectorIndex>(new_type);
    
    // Get all vectors from storage
    auto all_ids = storage_->getAllIds();
    
    // Add all vectors to new index
    for (int64_t id : all_ids) {
        auto vector = storage_->getVector(id);
        if (vector) {
            new_index->addVector(id, vector);
        }
    }
    
    // Replace old index with new one
    index_ = new_index;
}

} // namespace pgvector
