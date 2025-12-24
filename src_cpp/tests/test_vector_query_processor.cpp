#include <algorithm>
#include <iostream>
#include "../VectorQueryProcessor.hpp"
#include "../VectorFactory.hpp"
extern void assert_equal(double expected, double actual, const std::string& test_name, double tolerance = 1e-6);
#include "../VectorIndexFactory.hpp"
#include "../VectorStorage.hpp"

extern std::vector<struct TestResult> test_results;
extern void assert_true(bool condition, const std::string& test_name, const std::string& message = "");

void test_vector_query_processor() {
    std::cout << "\n=== Testing VectorQueryProcessor ===" << std::endl;
    
    // Test constructor
    try {
        auto storage = std::make_shared<pgvector::VectorStorage>();
        std::shared_ptr<pgvector::VectorIndex> index = pgvector::VectorIndexFactory::createIndex(pgvector::DistanceType::L2);
        pgvector::VectorQueryProcessor processor(storage, index);
        assert_true(true, "VectorQueryProcessor constructor");
    } catch (...) {
        assert_true(false, "VectorQueryProcessor constructor", "Exception thrown");
    }
    
    // Test insertVector
    try {
        auto storage = std::make_shared<pgvector::VectorStorage>();
        std::shared_ptr<pgvector::VectorIndex> index = pgvector::VectorIndexFactory::createIndex(pgvector::DistanceType::L2);
        pgvector::VectorQueryProcessor processor(storage, index);
        
        std::shared_ptr<pgvector::Vector> v1 = pgvector::VectorFactory::createVectorFromString("1.0, 2.0, 3.0");
        int64_t id = processor.insertVector(v1);
        
        assert_true(id == 1, "insertVector returns correct ID");
        assert_true(storage->size() == 1, "Storage has vector after insert");
        assert_true(index->size() == 1, "Index has vector after insert");
    } catch (...) {
        assert_true(false, "insertVector", "Exception thrown");
    }
    
    // Test deleteVector
    try {
        auto storage = std::make_shared<pgvector::VectorStorage>();
        std::shared_ptr<pgvector::VectorIndex> index = pgvector::VectorIndexFactory::createIndex(pgvector::DistanceType::L2);
        pgvector::VectorQueryProcessor processor(storage, index);
        
        std::shared_ptr<pgvector::Vector> v1 = pgvector::VectorFactory::createVectorFromString("1.0, 2.0, 3.0");
        int64_t id = processor.insertVector(v1);
        
        bool deleted = processor.deleteVector(id);
        assert_true(deleted, "deleteVector returns true for existing ID");
        assert_true(storage->size() == 0, "Storage is empty after delete");
        assert_true(index->size() == 0, "Index is empty after delete");
    } catch (...) {
        assert_true(false, "deleteVector", "Exception thrown");
    }
    
    // Test updateVector
    try {
        auto storage = std::make_shared<pgvector::VectorStorage>();
        std::shared_ptr<pgvector::VectorIndex> index = pgvector::VectorIndexFactory::createIndex(pgvector::DistanceType::L2);
        pgvector::VectorQueryProcessor processor(storage, index);
        
        std::shared_ptr<pgvector::Vector> v1 = pgvector::VectorFactory::createVectorFromString("1.0, 2.0, 3.0");
        std::shared_ptr<pgvector::Vector> v2 = pgvector::VectorFactory::createVectorFromString("4.0, 5.0, 6.0");
        int64_t id = processor.insertVector(v1);
        
        bool updated = processor.updateVector(id, v2);
        assert_true(updated, "updateVector returns true for existing ID");
        
        auto retrieved = processor.getVector(id);
        assert_equal(4.0f, (*retrieved)[0], "Updated vector has new data");
    } catch (...) {
        assert_true(false, "updateVector", "Exception thrown");
    }
    
    // Test searchSimilar
    try {
        auto storage = std::make_shared<pgvector::VectorStorage>();
        std::shared_ptr<pgvector::VectorIndex> index = pgvector::VectorIndexFactory::createIndex(pgvector::DistanceType::L2);
        pgvector::VectorQueryProcessor processor(storage, index);
        
        std::shared_ptr<pgvector::Vector> v1 = pgvector::VectorFactory::createVectorFromString("1.0, 2.0, 3.0");
        std::shared_ptr<pgvector::Vector> v2 = pgvector::VectorFactory::createVectorFromString("4.0, 5.0, 6.0");
        std::shared_ptr<pgvector::Vector> v3 = pgvector::VectorFactory::createVectorFromString("7.0, 8.0, 9.0");
        
        processor.insertVector(v1);
        processor.insertVector(v2);
        processor.insertVector(v3);
        
        std::shared_ptr<pgvector::Vector> query = pgvector::VectorFactory::createVectorFromString("1.0, 2.0, 3.0");
        auto results = processor.searchSimilar(*query, 2);
        
        assert_true(results.size() == 2, "searchSimilar returns correct number of results");
        assert_true(results[0].id == 1, "searchSimilar returns closest vector first");
    } catch (...) {
        assert_true(false, "searchSimilar", "Exception thrown");
    }
    
    // Test searchWithinDistance
    try {
        auto storage = std::make_shared<pgvector::VectorStorage>();
        std::shared_ptr<pgvector::VectorIndex> index = pgvector::VectorIndexFactory::createIndex(pgvector::DistanceType::L2);
        pgvector::VectorQueryProcessor processor(storage, index);
        
        std::shared_ptr<pgvector::Vector> v1 = pgvector::VectorFactory::createVectorFromString("1.0, 2.0, 3.0");
        std::shared_ptr<pgvector::Vector> v2 = pgvector::VectorFactory::createVectorFromString("1.1, 2.1, 3.1");
        std::shared_ptr<pgvector::Vector> v3 = pgvector::VectorFactory::createVectorFromString("10.0, 20.0, 30.0");
        
        processor.insertVector(v1);
        processor.insertVector(v2);
        processor.insertVector(v3);
        
        std::shared_ptr<pgvector::Vector> query = pgvector::VectorFactory::createVectorFromString("1.0, 2.0, 3.0");
        auto results = processor.searchWithinDistance(*query, 1.0);
        
        assert_true(results.size() >= 2, "searchWithinDistance returns vectors within distance");
    } catch (...) {
        assert_true(false, "searchWithinDistance", "Exception thrown");
    }
    
    // Test getVector
    try {
        auto storage = std::make_shared<pgvector::VectorStorage>();
        std::shared_ptr<pgvector::VectorIndex> index = pgvector::VectorIndexFactory::createIndex(pgvector::DistanceType::L2);
        pgvector::VectorQueryProcessor processor(storage, index);
        
        std::shared_ptr<pgvector::Vector> v1 = pgvector::VectorFactory::createVectorFromString("1.0, 2.0, 3.0");
        int64_t id = processor.insertVector(v1);
        
        auto retrieved = processor.getVector(id);
        assert_true(retrieved != nullptr, "getVector returns non-null for existing ID");
        assert_equal(1.0f, (*retrieved)[0], "getVector returns correct vector");
    } catch (...) {
        assert_true(false, "getVector", "Exception thrown");
    }
    
    // Test insertVectors (batch)
    try {
        auto storage = std::make_shared<pgvector::VectorStorage>();
        std::shared_ptr<pgvector::VectorIndex> index = pgvector::VectorIndexFactory::createIndex(pgvector::DistanceType::L2);
        pgvector::VectorQueryProcessor processor(storage, index);
        
        std::vector<std::shared_ptr<pgvector::Vector>> vectors;
        vectors.push_back(pgvector::VectorFactory::createVectorFromString("1.0, 2.0, 3.0"));
        vectors.push_back(pgvector::VectorFactory::createVectorFromString("4.0, 5.0, 6.0"));
        
        auto ids = processor.insertVectors(vectors);
        assert_true(ids.size() == 2, "insertVectors returns correct number of IDs");
        assert_true(storage->size() == 2, "Storage has correct size after batch insert");
        assert_true(index->size() == 2, "Index has correct size after batch insert");
    } catch (...) {
        assert_true(false, "insertVectors batch", "Exception thrown");
    }
    
    // Test getStats
    try {
        auto storage = std::make_shared<pgvector::VectorStorage>();
        std::shared_ptr<pgvector::VectorIndex> index = pgvector::VectorIndexFactory::createIndex(pgvector::DistanceType::L2);
        pgvector::VectorQueryProcessor processor(storage, index);
        
        std::shared_ptr<pgvector::Vector> v1 = pgvector::VectorFactory::createVectorFromString("1.0, 2.0, 3.0");
        processor.insertVector(v1);
        
        auto stats = processor.getStats();
        assert_true(stats.storage_size == 1, "Stats has correct storage size");
        assert_true(stats.index_size == 1, "Stats has correct index size");
        assert_true(stats.distance_type == pgvector::DistanceType::L2, "Stats has correct distance type");
    } catch (...) {
        assert_true(false, "getStats", "Exception thrown");
    }
    
    // Test rebuildIndex
    try {
        auto storage = std::make_shared<pgvector::VectorStorage>();
        std::shared_ptr<pgvector::VectorIndex> index = pgvector::VectorIndexFactory::createIndex(pgvector::DistanceType::L2);
        pgvector::VectorQueryProcessor processor(storage, index);
        
        std::shared_ptr<pgvector::Vector> v1 = pgvector::VectorFactory::createVectorFromString("1.0, 2.0, 3.0");
        processor.insertVector(v1);
        
        processor.rebuildIndex();
        assert_true(index->size() == 1, "Index has correct size after rebuild");
    } catch (...) {
        assert_true(false, "rebuildIndex", "Exception thrown");
    }
    
    // Test changeDistanceType
    try {
        auto storage = std::make_shared<pgvector::VectorStorage>();
        std::shared_ptr<pgvector::VectorIndex> index = pgvector::VectorIndexFactory::createIndex(pgvector::DistanceType::L2);
        pgvector::VectorQueryProcessor processor(storage, index);
        
        std::shared_ptr<pgvector::Vector> v1 = pgvector::VectorFactory::createVectorFromString("1.0, 2.0, 3.0");
        processor.insertVector(v1);
        
        processor.changeDistanceType(pgvector::DistanceType::COSINE);
        auto stats = processor.getStats();
        assert_true(stats.distance_type == pgvector::DistanceType::COSINE, "Distance type changed correctly");
        assert_true(stats.storage_size == 1, "Vectors preserved after distance type change");
    } catch (...) {
        assert_true(false, "changeDistanceType", "Exception thrown");
    }
}
