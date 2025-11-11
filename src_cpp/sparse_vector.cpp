#include "sparse_vector.hpp"
#include <cstdlib>
#include <cstring>
#include <cstddef>

SparseVector* SparseVector::init(int dim, int nnz) {
    if (dim <= 0 || dim > SPARSEVEC_MAX_DIM || nnz < 0 || nnz > SPARSEVEC_MAX_NNZ) {
        return nullptr;
    }
    size_t size = offsetof(SparseVector, indices) + (nnz * sizeof(int32_t)) + (nnz * sizeof(float));
    SparseVector* vec = (SparseVector*)malloc(size);
    if (vec) {
        memset(vec, 0, size);
        vec->dim = dim;
        vec->nnz = nnz;
    }
    return vec;
}

float* SparseVector::values() {
    return (float*)(((char*)this) + offsetof(SparseVector, indices) + (this->nnz * sizeof(int32_t)));
}

const float* SparseVector::values() const {
    return (const float*)(((const char*)this) + offsetof(SparseVector, indices) + (this->nnz * sizeof(int32_t)));
}

size_t SparseVector::size() const {
    return offsetof(SparseVector, indices) + (nnz * sizeof(int32_t)) + (nnz * sizeof(float));
}
