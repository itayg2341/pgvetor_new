#include "vector.hpp"

extern "C" {

    Vector* init_vector(int dim) {
        return Vector::init(dim);
    }

    void print_vector(const char* msg, Vector* vec) {
        if (vec) {
            vec->print(msg);
        }
    }

    int vector_cmp_internal(Vector* a, Vector* b) {
        if (a && b) {
            return a->compare(*b);
        }
        return a ? 1 : (b ? -1 : 0);
    }

}
