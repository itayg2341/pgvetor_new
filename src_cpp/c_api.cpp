#include "vector.hpp"
#include "half_vector.hpp"
#include "sparse_vector.hpp"

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

extern "C" {
    float vector_inner_product(Vector* a, Vector* b) {
        if (a && b) {
            return a->inner_product(*b);
        }
        return 0.0;
    }
}

extern "C" {
    float half_vector_inner_product(HalfVector* a, HalfVector* b) {
        if (a && b) {
            return a->inner_product(*b);
        }
        return 0.0;
    }
}

extern "C" {
    float sparse_vector_inner_product(SparseVector* a, SparseVector* b) {
        if (a && b) {
            return a->inner_product(*b);
        }
        return 0.0;
    }
}


extern "C" {
    float vector_l2_distance(Vector* a, Vector* b) {
        if (a && b) {
            return a->l2_distance(*b);
        }
        return 0.0;
    }
}


extern "C" {
    float half_vector_l2_distance(HalfVector* a, HalfVector* b) {
        if (a && b) {
            return a->l2_distance(*b);
        }
        return 0.0;
    }
}


extern "C" {
    float sparse_vector_l2_distance(SparseVector* a, SparseVector* b) {
        if (a && b) {
            return a->l2_distance(*b);
        }
        return 0.0;
    }
}



