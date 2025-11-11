#include "vector.hpp"
#include <iostream>
#include <cstdlib>
#include <cstring>

Vector* Vector::init(int dim) {
    if (dim <= 0 || dim > VECTOR_MAX_DIM) {
        return nullptr;
    }
    size_t size = offsetof(Vector, x) + sizeof(float) * dim;
    Vector* vec = (Vector*)malloc(size);
    if (vec) {
        memset(vec, 0, size);
        vec->dim = dim;
    }
    return vec;
}

void Vector::print(const char* msg) {
    std::cout << msg << ": [";
    for (int i = 0; i < dim; ++i) {
        std::cout << x[i] << (i == dim - 1 ? "" : ", ");
    }
    std::cout << "]" << std::endl;
}

int Vector::compare(const Vector& other) const {
    int min_dim = dim < other.dim ? dim : other.dim;
    for (int i = 0; i < min_dim; ++i) {
        if (x[i] != other.x[i]) {
            return x[i] < other.x[i] ? -1 : 1;
        }
    }
    if (dim != other.dim) {
        return dim - other.dim;
    }
    return 0;
}

float Vector::inner_product(const Vector& other) const {
    float result = 0.0;
    for (int i = 0; i < dim; ++i) {
        result += x[i] * other.x[i];
    }
    return result;
}

