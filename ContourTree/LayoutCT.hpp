#ifndef LAYOUTCT_HPP
#define LAYOUTCT_HPP

#include "SimplifyCT.hpp"
#include <unordered_map>
#include <unordered_set>
namespace contourtree {

struct Point {
    float x,y,z;
};

struct BranchLocation {
    int id;
    float angle;
    int level;
    float x,z;

    BranchLocation() : level(-1) {
    }
};

class LayoutCT
{
public:
    LayoutCT(SimplifyCT *sim, std::vector<uint32_t> &order);

    void layoutTree(int simplifiedCount);
    std::unordered_map<uint32_t, Point> getNodeLocations();

protected:
    int countLeaves(int brNo, int level);
    void assignAngles(int brNo);
    void assignLocations();
    void assignBranchIds();

public:
    SimplifyCT *simct;
    std::vector<uint32_t> &order;

    std::vector<Point> nodeLocs;
    std::vector<BranchLocation> branches;

    int noNodes;
    int noBranches;
    std::vector<int> leafCount;
    std::vector<float> startAngle;
    int noLeaves;
    int maxLevel;
//    int lastIndex;
    std::vector<float> r;
    std::vector<int> brMap;
    std::unordered_set<int> brSet;
    std::unordered_map<int,int> invMap;
    std::unordered_map<uint32_t,uint32_t> branchIds;
    float layoutExtent;

    const float rootTwo;
};

} // namespace contourtree
#endif // LAYOUTCT_HPP
