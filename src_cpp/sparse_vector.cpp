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

void SparseVector::print(const char* msg) {
    // Implementation requires iterating through sparse data
}

float SparseVector::inner_product(const SparseVector& other) const {
    float result = 0.0f;
    int i = 0;
    int j = 0;
    while (i < nnz && j < other.nnz) {
        if (indices[i] == other.indices[j]) {
            result += values()[i] * other.values()[j];
            i++;
            j++;
        } else if (indices[i] < other.indices[j]) {
            i++;
        } else {
            j++;
        }
    }
    return result;
}

