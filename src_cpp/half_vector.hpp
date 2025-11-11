#ifndef HALF_VECTOR_HPP
#define HALF_VECTOR_HPP

#include <cstdint>

#define HALFVEC_MAX_DIM 16000

// Define 'half' if not supported
#ifndef __FLT16_MAX__
using half = uint16_t;
#else
using half = _Float16;
#endif

class HalfVector {
public:
    int32_t vl_len_;
    int16_t dim;
    int16_t unused;
    half x[1];

    static HalfVector* init(int dim);
    void print(const char* msg);
    float inner_product(const HalfVector& other) const;
};

#endif // HALF_VECTOR_HPP
