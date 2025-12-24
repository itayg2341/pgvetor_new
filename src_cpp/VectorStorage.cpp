#include "VectorStorage.hpp"
#include <algorithm>

namespace pgvector {

int64_t VectorStorage::addVector(std::shared_ptr<Vector> vector) {
    std::unique_lock<std::shared_mutex> lock(mutex_);
    
    int64_t id = next_id_++;
    vectors_[id] = vector;
    return id;
}

bool VectorStorage::addVectorWithId(int64_t id, std::shared_ptr<Vector> vector) {
    std::unique_lock<std::shared_mutex> lock(mutex_);
    
    if (vectors_.find(id) != vectors_.end()) {
        return false; // ID already exists
    }
    
    vectors_[id] = vector;
    if (id >= next_id_) {
        next_id_ = id + 1;
    }
    return true;
}

std::shared_ptr<Vector> VectorStorage::getVector(int64_t id) const {
    std::shared_lock<std::shared_mutex> lock(mutex_);
    
    auto it = vectors_.find(id);
    if (it != vectors_.end()) {
        return it->second;
    }
    return nullptr;
}

bool VectorStorage::removeVector(int64_t id) {
    std::unique_lock<std::shared_mutex> lock(mutex_);
    
    return vectors_.erase(id) > 0;
}

bool VectorStorage::hasVector(int64_t id) const {
    std::shared_lock<std::shared_mutex> lock(mutex_);
    
    return vectors_.find(id) != vectors_.end();
}

std::vector<int64_t> VectorStorage::getAllIds() const {
    std::shared_lock<std::shared_mutex> lock(mutex_);
    
    std::vector<int64_t> ids;
    ids.reserve(vectors_.size());
    
    for (const auto& pair : vectors_) {
        ids.push_back(pair.first);
    }
    
    return ids;
}

size_t VectorStorage::size() const {
    std::shared_lock<std::shared_mutex> lock(mutex_);
    
    return vectors_.size();
}

void VectorStorage::clear() {
    std::unique_lock<std::shared_mutex> lock(mutex_);
    
    vectors_.clear();
    next_id_ = 1;
}

bool VectorStorage::updateVector(int64_t id, std::shared_ptr<Vector> vector) {
    std::unique_lock<std::shared_mutex> lock(mutex_);
    
    auto it = vectors_.find(id);
    if (it == vectors_.end()) {
        return false;
    }
    
    it->second = vector;
    return true;
}

VectorStorage::StorageStats VectorStorage::getStats() const {
    std::shared_lock<std::shared_mutex> lock(mutex_);
    
    StorageStats stats;
    stats.count = vectors_.size();
    stats.totalMemoryUsage = 0; // Will be calculated later
    
    if (!vectors_.empty()) {
        auto max_it = std::max_element(vectors_.begin(), vectors_.end(),
            [](const auto& a, const auto& b) { return a.first < b.first; });
        auto min_it = std::min_element(vectors_.begin(), vectors_.end(),
            [](const auto& a, const auto& b) { return a.first < b.first; });
        stats.maxId = max_it->first;
        stats.minId = min_it->first;
    } else {
        stats.maxId = 0;
        stats.minId = 0;
    }
    
    // Approximate memory usage calculation
    for (const auto& pair : vectors_) {
        stats.totalMemoryUsage += sizeof(pair.first) + sizeof(pair.second);
        if (pair.second) {
            stats.totalMemoryUsage += pair.second->getData().size() * sizeof(float);
        }
    }
    
    return stats;
}

} // namespace pgvector
