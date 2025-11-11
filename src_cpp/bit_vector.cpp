#include "bit_vector.hpp"
#include <cstdlib>
#include <cstring>

// Corresponding to VARBITTOTALLEN in PostgreSQL `varbit.h`
static inline size_t total_len(int32_t bitlen) {
    return offsetof(BitVector, bits) + ((bitlen + 7) / 8);
}

BitVector* BitVector::init(int dim) {
    if (dim <= 0) {
        return nullptr;
    }
    size_t size = total_len(dim);
    BitVector* vec = (BitVector*)malloc(size);
    if (vec) {
        memset(vec, 0, size);
        vec->dim = dim;
        // Set varlena length
        vec->vl_len_ = size << 2;
    }
    return vec;
}

int BitVector::bit_size() const {
    return dim;
}
