#include <iostream>
#include "../VectorFactory.hpp"
#include "../Vector.hpp"
extern void assert_equal(double expected, double actual, const std::string& test_name, double tolerance = 1e-6);

extern std::vector<struct TestResult> test_results;
extern void assert_true(bool condition, const std::string& test_name, const std::string& message = "");

void test_vector_factory() {
    std::cout << "\n=== Testing VectorFactory ===" << std::endl;
    
    // Test createVector
    try {
        float data[] = {1.0f, 2.0f, 3.0f};
        auto vector = pgvector::VectorFactory::createVector(3, data);
        assert_true(vector != nullptr, "VectorFactory::createVector returns non-null");
        assert_true(vector->getDimension() == 3, "Factory created vector has correct dimension");
        assert_equal(1.0f, (*vector)[0], "Factory created vector has correct data [0]");
    } catch (...) {
        assert_true(false, "VectorFactory::createVector", "Exception thrown");
    }
    
    // Test createVectorFromString
    try {
        auto vector = pgvector::VectorFactory::createVectorFromString("1.0, 2.0, 3.0");
        assert_true(vector != nullptr, "VectorFactory::createVectorFromString returns non-null");
        assert_true(vector->getDimension() == 3, "Factory created vector from string has correct dimension");
        assert_equal(1.0f, (*vector)[0], "Factory created vector from string has correct data [0]");
        assert_equal(2.0f, (*vector)[1], "Factory created vector from string has correct data [1]");
        assert_equal(3.0f, (*vector)[2], "Factory created vector from string has correct data [2]");
    } catch (...) {
        assert_true(false, "VectorFactory::createVectorFromString", "Exception thrown");
    }
    
    // Test createZeroVector
    try {
        auto vector = pgvector::VectorFactory::createZeroVector(5);
        assert_true(vector != nullptr, "VectorFactory::createZeroVector returns non-null");
        assert_true(vector->getDimension() == 5, "Factory created zero vector has correct dimension");
        
        bool all_zero = true;
        for (int i = 0; i < 5; ++i) {
            if ((*vector)[i] != 0.0f) {
                all_zero = false;
                break;
            }
        }
        assert_true(all_zero, "Factory created zero vector has all zeros");
    } catch (...) {
        assert_true(false, "VectorFactory::createZeroVector", "Exception thrown");
    }
    
    // Test createRandomVector
    try {
        auto vector = pgvector::VectorFactory::createRandomVector(3, -1.0f, 1.0f);
        assert_true(vector != nullptr, "VectorFactory::createRandomVector returns non-null");
        assert_true(vector->getDimension() == 3, "Factory created random vector has correct dimension");
        
        bool all_in_range = true;
        for (int i = 0; i < 3; ++i) {
            float val = (*vector)[i];
            if (val < -1.0f || val > 1.0f) {
                all_in_range = false;
                break;
            }
        }
        assert_true(all_in_range, "Factory created random vector values are in range");
    } catch (...) {
        assert_true(false, "VectorFactory::createRandomVector", "Exception thrown");
    }
    
    // Test invalid string
    try {
        auto vector = pgvector::VectorFactory::createVectorFromString("");
        assert_true(false, "Empty string should throw exception", "Should have thrown exception");
    } catch (const std::invalid_argument&) {
        assert_true(true, "Empty string throws invalid_argument");
    } catch (...) {
        assert_true(false, "Empty string exception type", "Wrong exception type");
    }
}
