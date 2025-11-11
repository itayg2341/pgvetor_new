#ifndef IVFFLAT_HPP
#define IVFFLAT_HPP

#include "vector.hpp"
#include <vector>

class IVFFlat {
public:
    int lists;

    IVFFlat(int lists);
    void build(const std::vector<Vector*>& data);
    std::vector<int> search(const Vector& query, int probes);
};

#endif // IVFFLAT_HPP
