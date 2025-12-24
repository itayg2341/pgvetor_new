#include "VectorIndex.hpp"
#include <algorithm>
#include <limits>
#include <queue>

namespace pgvector {

std::function<double(const Vector&, const Vector&)> VectorIndex::getDistanceFunction() const {
    switch (distance_type_) {
        case DistanceType::L2:
            return [](const Vector& a, const Vector& b) {
                return std::sqrt(a.l2SquaredDistance(b));
            };
        case DistanceType::INNER_PRODUCT:
            return [](const Vector& a, const Vector& b) {
                return -a.innerProduct(b); // Negative because we want to minimize
            };
        case DistanceType::COSINE:
            return [](const Vector& a, const Vector& b) {
                return a.cosineDistance(b);
            };
        default:
            throw std::runtime_error("Unknown distance type");
    }
}

void VectorIndex::addVector(int64_t id, std::shared_ptr<Vector> vector) {
    vectors_.emplace_back(id, vector);
}

bool VectorIndex::removeVector(int64_t id) {
    auto it = std::remove_if(vectors_.begin(), vectors_.end(),
                            [id](const VectorWithId& v) { return v.id == id; });
    if (it != vectors_.end()) {
        vectors_.erase(it, vectors_.end());
        return true;
    }
    return false;
}

std::vector<SearchResult> VectorIndex::search(const Vector& query, int k) const {
    if (vectors_.empty()) {
        return {};
    }
    
    auto distance_func = getDistanceFunction();
    std::vector<SearchResult> results;
    results.reserve(vectors_.size());
    
    // Calculate distances to all vectors
    for (const auto& vec_with_id : vectors_) {
        double distance = distance_func(query, *vec_with_id.vector);
        results.emplace_back(vec_with_id.id, distance);
    }
    
    // Sort by distance and take top k
    std::partial_sort(results.begin(), 
                     results.begin() + std::min(k, static_cast<int>(results.size())),
                     results.end(),
                     [](const SearchResult& a, const SearchResult& b) {
                         return a.distance < b.distance;
                     });
    
    // Resize to k elements
    if (results.size() > static_cast<size_t>(k)) {
        results.resize(k);
    }
    
    return results;
}

std::vector<SearchResult> VectorIndex::searchOptimized(const Vector& query, int k) const {
    // Default implementation just calls the regular search
    // Derived classes can override this with optimized implementations
    return search(query, k);
}

} // namespace pgvector
