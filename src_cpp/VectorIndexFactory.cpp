#include "VectorIndexFactory.hpp"
#include <algorithm>
#include <cctype>

namespace pgvector {

std::unique_ptr<VectorIndex> VectorIndexFactory::createIndex(DistanceType distance_type) {
    return std::make_unique<VectorIndex>(distance_type);
}

std::unique_ptr<VectorIndex> VectorIndexFactory::createIndex(const std::string& distance_type) {
    DistanceType type = getDistanceTypeFromName(distance_type);
    return createIndex(type);
}

std::string VectorIndexFactory::getDistanceTypeName(DistanceType distance_type) {
    switch (distance_type) {
        case DistanceType::L2:
            return "l2";
        case DistanceType::INNER_PRODUCT:
            return "inner_product";
        case DistanceType::COSINE:
            return "cosine";
        default:
            throw std::runtime_error("Unknown distance type");
    }
}

DistanceType VectorIndexFactory::getDistanceTypeFromName(const std::string& name) {
    std::string lower_name = name;
    std::transform(lower_name.begin(), lower_name.end(), lower_name.begin(), ::tolower);
    
    if (lower_name == "l2" || lower_name == "euclidean") {
        return DistanceType::L2;
    } else if (lower_name == "inner_product" || lower_name == "dot_product") {
        return DistanceType::INNER_PRODUCT;
    } else if (lower_name == "cosine") {
        return DistanceType::COSINE;
    } else {
        throw std::invalid_argument("Unknown distance type: " + name);
    }
}

} // namespace pgvector
