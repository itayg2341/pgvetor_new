#ifndef SPARSE_VECTOR_HPP
#define SPARSE_VECTOR_HPP

#include <cstdint>
#include <cstddef>

#define SPARSEVEC_MAX_DIM 1000000000
#define SPARSEVEC_MAX_NNZ 16000

class SparseVector {
public:
    int32_t vl_len_;
    int32_t dim;
    int32_t nnz;
    int32_t unused;
    int32_t indices[1];

    static SparseVector* init(int dim, int nnz);
    float* values();
    const float* values() const;
    size_t size() const;
    void print(const char* msg);
    float inner_product(const SparseVector& other) const;
};

#endif // SPARSE_VECTOR_HPP
