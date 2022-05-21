#ifndef HYPERVOLUME_HPP
#define HYPERVOLUME_HPP

#include "SimFunction.hpp"
#include "ContourTreeData.hpp"

namespace contourtree {

class HyperVolume : public SimFunction {
public:
    HyperVolume(const ContourTreeData& ctData, std::string partFile);

    void init(std::vector<float>& fn, std::vector<Branch>& br);
    void update(const std::vector<Branch>& br, uint32_t brNo);
    void branchRemoved(std::vector<Branch>& br, uint32_t brNo, std::vector<bool>& invalid);
    float getBranchWeight(uint32_t brNo);

private:
    float volume(const std::vector<Branch>& br, int brNo);
    void initVolumes(const std::vector<uint32_t>& cols);

public:
    const float* fnVals;
    float* fn;
    std::vector<uint32_t> vol;
    std::vector<uint32_t> brVol;
};

}  // namespace contourtree

#endif  // HYPERVOLUME_HPP
