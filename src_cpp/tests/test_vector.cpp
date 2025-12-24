#include <iostream>
#include "../Vector.hpp"
#include <cmath>

extern std::vector<struct TestResult> test_results;
extern void assert_true(bool condition, const std::string& test_name, const std::string& message = "");
extern void assert_equal(double expected, double actual, const std::string& test_name, double tolerance = 1e-6);

void test_vector_operations() {
    std::cout << "\n=== Testing Vector Operations ===" << std::endl;
    
    // Test constructor
    try {
        pgvector::Vector v1(3);
        assert_true(v1.getDimension() == 3, "Vector constructor sets dimension");
        assert_true(v1.getData().size() == 3, "Vector constructor initializes data");
    } catch (...) {
        assert_true(false, "Vector constructor", "Exception thrown");
    }
    
    // Test constructor with data
    try {
        std::vector<float> data = {1.0f, 2.0f, 3.0f};
        pgvector::Vector v2(3, data);
        assert_true(v2.getDimension() == 3, "Vector constructor with data sets dimension");
        assert_equal(1.0f, v2[0], "Vector data access [0]");
        assert_equal(2.0f, v2[1], "Vector data access [1]");
        assert_equal(3.0f, v2[2], "Vector data access [2]");
    } catch (...) {
        assert_true(false, "Vector constructor with data", "Exception thrown");
    }
    
    // Test L2 distance
    try {
        std::vector<float> data1 = {1.0f, 2.0f, 3.0f};
        std::vector<float> data2 = {4.0f, 5.0f, 6.0f};
        pgvector::Vector v1(3, data1);
        pgvector::Vector v2(3, data2);
        
        double distance = v1.l2SquaredDistance(v2);
        double expected = 27.0; // (4-1)² + (5-2)² + (6-3)² = 9 + 9 + 9 = 27
        assert_equal(expected, distance, "L2 squared distance calculation");
    } catch (...) {
        assert_true(false, "L2 distance calculation", "Exception thrown");
    }
    
    // Test inner product
    try {
        std::vector<float> data1 = {1.0f, 2.0f, 3.0f};
        std::vector<float> data2 = {4.0f, 5.0f, 6.0f};
        pgvector::Vector v1(3, data1);
        pgvector::Vector v2(3, data2);
        
        double product = v1.innerProduct(v2);
        double expected = 32.0; // 1*4 + 2*5 + 3*6 = 4 + 10 + 18 = 32
        assert_equal(expected, product, "Inner product calculation");
    } catch (...) {
        assert_true(false, "Inner product calculation", "Exception thrown");
    }
    
    // Test cosine similarity
    try {
        std::vector<float> data1 = {1.0f, 0.0f, 0.0f};
        std::vector<float> data2 = {1.0f, 0.0f, 0.0f};
        pgvector::Vector v1(3, data1);
        pgvector::Vector v2(3, data2);
        
        double similarity = v1.cosineSimilarity(v2);
        assert_equal(1.0, similarity, "Cosine similarity (identical vectors)");
    } catch (...) {
        assert_true(false, "Cosine similarity calculation", "Exception thrown");
    }
    
    // Test dimension mismatch
    try {
        pgvector::Vector v1(3);
        pgvector::Vector v2(4);
        v1.l2SquaredDistance(v2);
        assert_true(false, "Dimension mismatch detection", "Should have thrown exception");
    } catch (const std::invalid_argument&) {
        assert_true(true, "Dimension mismatch detection");
    } catch (...) {
        assert_true(false, "Dimension mismatch detection", "Wrong exception type");
    }
    
    // Test comparison operators
    try {
        std::vector<float> data1 = {1.0f, 2.0f, 3.0f};
        std::vector<float> data2 = {1.0f, 2.0f, 3.0f};
        std::vector<float> data3 = {4.0f, 5.0f, 6.0f};
        
        pgvector::Vector v1(3, data1);
        pgvector::Vector v2(3, data2);
        pgvector::Vector v3(3, data3);
        
        assert_true(v1 == v2, "Vector equality operator");
        assert_true(v1 != v3, "Vector inequality operator");
        assert_true(v1 < v3, "Vector less than operator");
        assert_true(v3 > v1, "Vector greater than operator");
    } catch (...) {
        assert_true(false, "Vector comparison operators", "Exception thrown");
    }
}
