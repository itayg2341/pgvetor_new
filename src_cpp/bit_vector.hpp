#ifndef BIT_VECTOR_HPP
#define BIT_VECTOR_HPP

#include <cstdint>
#include <cstddef>

class BitVector {
public:
    int32_t vl_len_;
    int32_t dim;
    uint8_t bits[1];

    static BitVector* init(int dim);
    int bit_size() const;
};

#endif // BIT_VECTOR_HPP
