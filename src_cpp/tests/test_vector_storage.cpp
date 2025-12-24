#include <algorithm>
#include <iostream>
#include "../VectorStorage.hpp"
#include "../VectorFactory.hpp"
extern void assert_equal(double expected, double actual, const std::string& test_name, double tolerance = 1e-6);

extern std::vector<struct TestResult> test_results;
extern void assert_true(bool condition, const std::string& test_name, const std::string& message = "");

void test_vector_storage() {
    std::cout << "\n=== Testing VectorStorage ===" << std::endl;
    
    // Test adding vectors
    try {
        pgvector::VectorStorage storage;
        std::shared_ptr<pgvector::Vector> v1 = pgvector::VectorFactory::createVectorFromString("1.0, 2.0, 3.0");
        std::shared_ptr<pgvector::Vector> v2 = pgvector::VectorFactory::createVectorFromString("4.0, 5.0, 6.0");
        
        int64_t id1 = storage.addVector(v1);
        int64_t id2 = storage.addVector(v2);
        
        assert_true(id1 == 1, "First vector gets ID 1");
        assert_true(id2 == 2, "Second vector gets ID 2");
        assert_true(storage.size() == 2, "Storage has correct size");
    } catch (...) {
        assert_true(false, "Adding vectors to storage", "Exception thrown");
    }
    
    // Test getting vectors
    try {
        pgvector::VectorStorage storage;
        std::shared_ptr<pgvector::Vector> v1 = pgvector::VectorFactory::createVectorFromString("1.0, 2.0, 3.0");
        int64_t id = storage.addVector(v1);
        
        auto retrieved = storage.getVector(id);
        assert_true(retrieved != nullptr, "getVector returns non-null for existing ID");
        assert_true(retrieved->getDimension() == 3, "Retrieved vector has correct dimension");
        assert_equal(1.0f, (*retrieved)[0], "Retrieved vector has correct data");
        
        auto not_found = storage.getVector(999);
        assert_true(not_found == nullptr, "getVector returns null for non-existing ID");
    } catch (...) {
        assert_true(false, "Getting vectors from storage", "Exception thrown");
    }
    
    // Test removing vectors
    try {
        pgvector::VectorStorage storage;
        std::shared_ptr<pgvector::Vector> v1 = pgvector::VectorFactory::createVectorFromString("1.0, 2.0, 3.0");
        int64_t id = storage.addVector(v1);
        
        bool removed = storage.removeVector(id);
        assert_true(removed, "removeVector returns true for existing ID");
        assert_true(storage.size() == 0, "Storage is empty after removal");
        
        bool not_removed = storage.removeVector(999);
        assert_true(!not_removed, "removeVector returns false for non-existing ID");
    } catch (...) {
        assert_true(false, "Removing vectors from storage", "Exception thrown");
    }
    
    // Test hasVector
    try {
        pgvector::VectorStorage storage;
        std::shared_ptr<pgvector::Vector> v1 = pgvector::VectorFactory::createVectorFromString("1.0, 2.0, 3.0");
        int64_t id = storage.addVector(v1);
        
        assert_true(storage.hasVector(id), "hasVector returns true for existing ID");
        assert_true(!storage.hasVector(999), "hasVector returns false for non-existing ID");
    } catch (...) {
        assert_true(false, "hasVector check", "Exception thrown");
    }
    
    // Test getAllIds
    try {
        pgvector::VectorStorage storage;
        std::shared_ptr<pgvector::Vector> v1 = pgvector::VectorFactory::createVectorFromString("1.0, 2.0, 3.0");
        std::shared_ptr<pgvector::Vector> v2 = pgvector::VectorFactory::createVectorFromString("4.0, 5.0, 6.0");
        
        int64_t id1 = storage.addVector(v1);
        int64_t id2 = storage.addVector(v2);
        
        auto ids = storage.getAllIds();
        assert_true(ids.size() == 2, "getAllIds returns correct number of IDs");
        assert_true(std::find(ids.begin(), ids.end(), id1) != ids.end(), "getAllIds contains first ID");
        assert_true(std::find(ids.begin(), ids.end(), id2) != ids.end(), "getAllIds contains second ID");
    } catch (...) {
        assert_true(false, "getAllIds", "Exception thrown");
    }
    
    // Test addVectorWithId
    try {
        pgvector::VectorStorage storage;
        std::shared_ptr<pgvector::Vector> v1 = pgvector::VectorFactory::createVectorFromString("1.0, 2.0, 3.0");
        
        bool added = storage.addVectorWithId(100, v1);
        assert_true(added, "addVectorWithId returns true for new ID");
        assert_true(storage.hasVector(100), "Vector is added with specified ID");
        
        bool not_added = storage.addVectorWithId(100, v1);
        assert_true(!not_added, "addVectorWithId returns false for existing ID");
    } catch (...) {
        assert_true(false, "addVectorWithId", "Exception thrown");
    }
    
    // Test updateVector
    try {
        pgvector::VectorStorage storage;
        std::shared_ptr<pgvector::Vector> v1 = pgvector::VectorFactory::createVectorFromString("1.0, 2.0, 3.0");
        std::shared_ptr<pgvector::Vector> v2 = pgvector::VectorFactory::createVectorFromString("4.0, 5.0, 6.0");
        
        int64_t id = storage.addVector(v1);
        bool updated = storage.updateVector(id, v2);
        
        assert_true(updated, "updateVector returns true for existing ID");
        auto retrieved = storage.getVector(id);
        assert_equal(4.0f, (*retrieved)[0], "Updated vector has new data");
        
        bool not_updated = storage.updateVector(999, v2);
        assert_true(!not_updated, "updateVector returns false for non-existing ID");
    } catch (...) {
        assert_true(false, "updateVector", "Exception thrown");
    }
    
    // Test clear
    try {
        pgvector::VectorStorage storage;
        std::shared_ptr<pgvector::Vector> v1 = pgvector::VectorFactory::createVectorFromString("1.0, 2.0, 3.0");
        storage.addVector(v1);
        
        storage.clear();
        assert_true(storage.size() == 0, "Storage is empty after clear");
        assert_true(storage.getAllIds().empty(), "getAllIds returns empty after clear");
    } catch (...) {
        assert_true(false, "clear storage", "Exception thrown");
    }
    
    // Test getStats
    try {
        pgvector::VectorStorage storage;
        std::shared_ptr<pgvector::Vector> v1 = pgvector::VectorFactory::createVectorFromString("1.0, 2.0, 3.0");
        storage.addVector(v1);
        
        auto stats = storage.getStats();
        assert_true(stats.count == 1, "Stats has correct count");
        assert_true(stats.minId == 1, "Stats has correct minId");
        assert_true(stats.maxId == 1, "Stats has correct maxId");
        assert_true(stats.totalMemoryUsage > 0, "Stats has memory usage > 0");
    } catch (...) {
        assert_true(false, "getStats", "Exception thrown");
    }
}
