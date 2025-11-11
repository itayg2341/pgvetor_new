#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <cstdint>
#include <cstddef>

#define VECTOR_MAX_DIM 16000

class Vector {
public:
    int32_t vl_len_;    // varlena header
    int16_t dim;        // number of dimensions
    int16_t unused;     // reserved
    float x[1];       // flexible array member

    static Vector* init(int dim);
    void print(const char* msg);
    int compare(const Vector& other) const;
    float inner_product(const Vector& other) const;
};

#endif // VECTOR_HPP
