#include "half_vector.hpp"
#include <cstdlib>
#include <cstring>
#include <cstddef>

HalfVector* HalfVector::init(int dim) {
    if (dim <= 0 || dim > HALFVEC_MAX_DIM) {
        return nullptr;
    }
    size_t size = offsetof(HalfVector, x) + sizeof(half) * dim;
    HalfVector* vec = (HalfVector*)malloc(size);
    if (vec) {
        memset(vec, 0, size);
        vec->dim = dim;
    }
    return vec;
}
