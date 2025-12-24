#include "pgvector.hpp"
#include <iostream>
#include <memory>

using namespace pgvector;

int main() {
    std::cout << "pgvector C++ Library Example" << std::endl;
    std::cout << "Version: " << VERSION << std::endl;
    std::cout << "=============================" << std::endl;
    
    // Create a vector storage
    auto storage = std::make_shared<VectorStorage>();
    
    // Create an index with L2 distance
    std::shared_ptr<VectorIndex> index = VectorIndexFactory::createIndex(DistanceType::L2);
    
    // Create a query processor
    VectorQueryProcessor processor(storage, index);
    
    // Insert some vectors
    std::cout << "\nInserting vectors..." << std::endl;
    std::shared_ptr<Vector> v1 = VectorFactory::createVectorFromString("1.0, 2.0, 3.0");
    std::shared_ptr<Vector> v2 = VectorFactory::createVectorFromString("4.0, 5.0, 6.0");
    std::shared_ptr<Vector> v3 = VectorFactory::createVectorFromString("7.0, 8.0, 9.0");
    
    int64_t id1 = processor.insertVector(v1);
    int64_t id2 = processor.insertVector(v2);
    int64_t id3 = processor.insertVector(v3);
    
    std::cout << "Inserted vectors with IDs: " << id1 << ", " << id2 << ", " << id3 << std::endl;
    
    // Search for similar vectors
    std::cout << "\nSearching for similar vectors..." << std::endl;
    std::shared_ptr<Vector> query = VectorFactory::createVectorFromString("1.1, 2.1, 3.1");
    auto results = processor.searchSimilar(*query, 2);
    
    std::cout << "Top 2 similar vectors:" << std::endl;
    for (const auto& result : results) {
        std::cout << "  ID: " << result.id << ", Distance: " << result.distance << std::endl;
    }
    
    // Test different distance types
    std::cout << "\nTesting different distance types..." << std::endl;
    
    // Cosine distance
    processor.changeDistanceType(DistanceType::COSINE);
    auto cosine_results = processor.searchSimilar(*query, 2);
    std::cout << "Cosine distance results:" << std::endl;
    for (const auto& result : cosine_results) {
        std::cout << "  ID: " << result.id << ", Distance: " << result.distance << std::endl;
    }
    
    // Inner product
    processor.changeDistanceType(DistanceType::INNER_PRODUCT);
    auto ip_results = processor.searchSimilar(*query, 2);
    std::cout << "Inner product results:" << std::endl;
    for (const auto& result : ip_results) {
        std::cout << "  ID: " << result.id << ", Distance: " << result.distance << std::endl;
    }
    
    // Get statistics
    std::cout << "\nStatistics:" << std::endl;
    auto stats = processor.getStats();
    std::cout << "  Storage size: " << stats.storage_size << std::endl;
    std::cout << "  Index size: " << stats.index_size << std::endl;
    std::cout << "  Distance type: " << VectorIndexFactory::getDistanceTypeName(stats.distance_type) << std::endl;
    
    std::cout << "\nExample completed successfully!" << std::endl;
    return 0;
}
