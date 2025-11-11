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

void HalfVector::print(const char* msg) {
    // Implementation requires converting half to float for printing
}

float HalfVector::inner_product(const HalfVector& other) const {
    float result = 0.0f;
    for (int i = 0; i < dim; ++i) {
        result += (float)x[i] * (float)other.x[i];
    }
    return result;
}

