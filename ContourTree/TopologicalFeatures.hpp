#ifndef TOPOLOGICALFEATURES_HPP
#define TOPOLOGICALFEATURES_HPP

#include "SimplifyCT.hpp"
#include <string>
#include <set>

namespace contourtree {

struct Feature {
    std::vector<uint32_t> arcs;
    uint32_t from, to;
};

class TopologicalFeatures {
public:
    TopologicalFeatures();

    void loadData(std::string dataLocation);
    std::vector<Feature> getArcFeatures(int topk = -1, float th = 0);
    std::vector<Feature> getPartitionedExtremaFeatures(int topk = -1, float th = 0);

public:
    ContourTreeData ctdata;
    std::vector<uint32_t> order;
    std::vector<float> wts;

    // when completely partitioning branch decomposition
    std::vector<std::vector<uint32_t>> featureArcs;
    SimplifyCT gsim;

private:
    void addFeature(SimplifyCT& sim, uint32_t bno, std::vector<Feature>& features,
                    std::set<size_t>& featureSet);
};

}  // namespace contourtree

#endif  // TOPOLOGICALFEATURES_HPP
