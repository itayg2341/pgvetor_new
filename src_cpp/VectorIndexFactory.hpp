#ifndef VECTOR_INDEX_FACTORY_HPP
#define VECTOR_INDEX_FACTORY_HPP

#include "VectorIndex.hpp"
#include <memory>
#include <string>

namespace pgvector {

class VectorIndexFactory {
public:
    // Create a vector index based on distance type
    static std::unique_ptr<VectorIndex> createIndex(DistanceType distance_type);
    
    // Create a vector index from string name
    static std::unique_ptr<VectorIndex> createIndex(const std::string& distance_type);
    
    // Get the string name for a distance type
    static std::string getDistanceTypeName(DistanceType distance_type);
    
    // Get the distance type from string name
    static DistanceType getDistanceTypeFromName(const std::string& name);
    
private:
    VectorIndexFactory() = delete; // Static class, no instantiation
};

} // namespace pgvector

#endif // VECTOR_INDEX_FACTORY_HPP
