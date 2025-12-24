#include <iostream>
#include "../VectorIndex.hpp"
#include "../VectorIndexFactory.hpp"
extern void assert_equal(double expected, double actual, const std::string& test_name, double tolerance = 1e-6);
#include "../Vector.hpp"
#include "../VectorFactory.hpp"

extern std::vector<struct TestResult> test_results;
extern void assert_true(bool condition, const std::string& test_name, const std::string& message = "");

void test_vector_index() {
    std::cout << "\n=== Testing VectorIndex ===" << std::endl;
    
    // Test L2 distance index
    try {
        auto index = pgvector::VectorIndexFactory::createIndex(pgvector::DistanceType::L2);
        assert_true(index != nullptr, "VectorIndexFactory::createIndex returns non-null");
        assert_true(index->getDistanceType() == pgvector::DistanceType::L2, "Index has correct distance type");
    } catch (...) {
        assert_true(false, "VectorIndex creation", "Exception thrown");
    }
    
    // Test index with string name
    try {
        auto index = pgvector::VectorIndexFactory::createIndex("l2");
        assert_true(index != nullptr, "VectorIndexFactory::createIndex from string returns non-null");
        assert_true(index->getDistanceType() == pgvector::DistanceType::L2, "Index from string has correct distance type");
    } catch (...) {
        assert_true(false, "VectorIndex creation from string", "Exception thrown");
    }
    
    // Test adding vectors to index
    try {
        auto index = pgvector::VectorIndexFactory::createIndex(pgvector::DistanceType::L2);
        std::shared_ptr<pgvector::Vector> v1 = pgvector::VectorFactory::createVectorFromString("1.0, 2.0, 3.0");
        std::shared_ptr<pgvector::Vector> v2 = pgvector::VectorFactory::createVectorFromString("4.0, 5.0, 6.0");
        std::shared_ptr<pgvector::Vector> v3 = pgvector::VectorFactory::createVectorFromString("7.0, 8.0, 9.0");
        
        index->addVector(1, v1);
        index->addVector(2, v2);
        index->addVector(3, v3);
        
        assert_true(index->size() == 3, "Index has correct size after adding vectors");
    } catch (...) {
        assert_true(false, "Adding vectors to index", "Exception thrown");
    }
    
    // Test searching in index
    try {
        auto index = pgvector::VectorIndexFactory::createIndex(pgvector::DistanceType::L2);
        std::shared_ptr<pgvector::Vector> v1 = pgvector::VectorFactory::createVectorFromString("1.0, 2.0, 3.0");
        std::shared_ptr<pgvector::Vector> v2 = pgvector::VectorFactory::createVectorFromString("4.0, 5.0, 6.0");
        std::shared_ptr<pgvector::Vector> v3 = pgvector::VectorFactory::createVectorFromString("7.0, 8.0, 9.0");
        
        index->addVector(1, v1);
        index->addVector(2, v2);
        index->addVector(3, v3);
        
        std::shared_ptr<pgvector::Vector> query = pgvector::VectorFactory::createVectorFromString("1.0, 2.0, 3.0");
        auto results = index->search(*query, 2);
        
        assert_true(results.size() == 2, "Search returns correct number of results");
        assert_true(results[0].id == 1, "Search returns closest vector first");
        assert_equal(0.0, results[0].distance, "Search returns correct distance for closest vector");
    } catch (...) {
        assert_true(false, "Searching in index", "Exception thrown");
    }
    
    // Test removing vectors from index
    try {
        auto index = pgvector::VectorIndexFactory::createIndex(pgvector::DistanceType::L2);
        std::shared_ptr<pgvector::Vector> v1 = pgvector::VectorFactory::createVectorFromString("1.0, 2.0, 3.0");
        std::shared_ptr<pgvector::Vector> v2 = pgvector::VectorFactory::createVectorFromString("4.0, 5.0, 6.0");
        
        index->addVector(1, v1);
        index->addVector(2, v2);
        
        bool removed = index->removeVector(1);
        assert_true(removed, "Remove vector returns true when successful");
        assert_true(index->size() == 1, "Index has correct size after removing vector");
        
        bool not_removed = index->removeVector(999);
        assert_true(!not_removed, "Remove vector returns false when ID not found");
    } catch (...) {
        assert_true(false, "Removing vectors from index", "Exception thrown");
    }
    
    // Test different distance types
    try {
        auto l2_index = pgvector::VectorIndexFactory::createIndex("l2");
        auto ip_index = pgvector::VectorIndexFactory::createIndex("inner_product");
        auto cosine_index = pgvector::VectorIndexFactory::createIndex("cosine");
        
        assert_true(l2_index->getDistanceType() == pgvector::DistanceType::L2, "L2 distance type");
        assert_true(ip_index->getDistanceType() == pgvector::DistanceType::INNER_PRODUCT, "Inner product distance type");
        assert_true(cosine_index->getDistanceType() == pgvector::DistanceType::COSINE, "Cosine distance type");
    } catch (...) {
        assert_true(false, "Different distance types", "Exception thrown");
    }
    
    // Test invalid distance type
    try {
        auto index = pgvector::VectorIndexFactory::createIndex("invalid_type");
        assert_true(false, "Invalid distance type should throw exception", "Should have thrown exception");
    } catch (const std::invalid_argument&) {
        assert_true(true, "Invalid distance type throws invalid_argument");
    } catch (...) {
        assert_true(false, "Invalid distance type exception", "Wrong exception type");
    }
}
