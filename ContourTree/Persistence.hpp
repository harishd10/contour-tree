#ifndef PERSISTENCE_HPP
#define PERSISTENCE_HPP

#include "SimFunction.hpp"
#include "ContourTreeData.hpp"

namespace contourtree {

class Persistence : public SimFunction
{
public:
    Persistence(const ContourTreeData& ctData);

    void init(std::vector<float> &fn, std::vector<Branch> &br);
    void update(const std::vector<Branch> &br, uint32_t brNo);
    void branchRemoved(std::vector<Branch>& br, uint32_t brNo, std::vector<bool>& invalid);
    float getBranchWeight(uint32_t brNo);

public:
    const float *fnVals;
    float *fn;
};

}

#endif // PERSISTENCE_HPP
