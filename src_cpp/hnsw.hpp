#ifndef HNSW_HPP
#define HNSW_HPP

#include "vector.hpp"
#include <vector>
#include <mutex>

class HNSW {
public:
    // Parameters
    int m;
    int ef_construction;

    // Graph structure
    // Additional implementation details will be added here.

    HNSW(int m, int ef_construction);
    void build(const std::vector<Vector*>& data);
    std::vector<int> search(const Vector& query, int ef_search, int k);
};

#endif // HNSW_HPP
