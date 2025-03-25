#include "LayoutCT.hpp"

#define _USE_MATH_DEFINES
#include <math.h>

#include <fstream>
#include <iostream>

namespace contourtree {

LayoutCT::LayoutCT(SimplifyCT *sim, std::vector<uint32_t>& order) : order(order), rootTwo (std::sqrt(2)) {
    this->simct = sim;
}

void LayoutCT::layoutTree(int simplifiedCount) {
//    lastIndex = lastIncludedBranch;
    for(int i = 0;i < simplifiedCount;i ++) {
        int in = order.size() - i - 1;
        int brid = order[in];
        brMap.push_back(brid);
        brSet.insert(brid);
        invMap[brid] = i;
    }

    assignBranchIds();

    maxLevel = 0;

    noNodes = simct->data->noNodes;;
    noBranches = simct->branches.size();

    nodeLocs.resize(noNodes);
    leafCount.resize(noBranches);
    startAngle.resize(noBranches);
    branches.resize(noBranches);

    noLeaves = countLeaves(0, 0);

    r.resize(maxLevel + 1);
    r[0] = 0;
    if(maxLevel > 0) {
        r[1] = 1;
    }
    for(int i = 2;i <= maxLevel;i ++) {
        r[i] = r[i - 1] * rootTwo;
    }
    branches[0].angle = (float) (2 * M_PI);
    startAngle[0] = 0;
    branches[0].x = 0;
    branches[0].z = 0;
    assignAngles(0);
    assignLocations();
}

std::unordered_map<uint32_t, Point> LayoutCT::getNodeLocations() {
    int ct = 0;
    std::unordered_map<int,int> nodeMap;
    std::vector<int> nids;
    for(int i = 0;i < branches.size();i ++) {
        if(branches[i].level != -1) {
            // valid branch
            int from = simct->branches[brMap[i]].from;
            int to = simct->branches[brMap[i]].to;

            if(nodeMap.find(from) == nodeMap.end()) {
                nids.push_back(from);
                nodeMap[from] = ct ++;
            }
            if(nodeMap.find(to) == nodeMap.end()) {
                nids.push_back(to);
                nodeMap[to] = ct ++;
            }
        }
    }

    std::unordered_map<uint32_t, Point> locations;
    for(int i = 0;i < nids.size();i ++) {
        locations[simct->data->nodeVerts[nids[i]]] = nodeLocs[nids[i]];
    }
    return locations;
}

int LayoutCT::countLeaves(int brNo, int level) {
    branches[brNo].level = level;
    Branch &br = simct->branches[brMap[brNo]];
    if(br.children.size() > 0) {
        for(uint32_t bno: br.children) {
            if(brSet.find(bno) != brSet.end()) {
                leafCount[brNo] += countLeaves(invMap[bno], level + 1);
            }
        }
        if(leafCount[brNo] == 0) {
            leafCount[brNo] = 1;
            maxLevel = std::max(level, maxLevel);
        }
        return leafCount[brNo];
    } else {
        leafCount[brNo] = 1;
        maxLevel = std::max(level, maxLevel);
        return 1;
    }
    return 0;
}

void LayoutCT::assignAngles(int brNo) {
    int i = brNo;
    float totAng = branches[i].angle;
    int totCh = leafCount[i];
    float add = startAngle[brNo];
    Branch &br = simct->branches[brMap[brNo]];
    for(auto cno: br.children) {
        if(brSet.find(cno) == brSet.end()) {
            continue;
        }

        int cid = invMap[cno];
        branches[cid].angle = totAng * leafCount[cid];
        branches[cid].angle /= totCh;
        if(i == 0) {
            if(branches[cid].angle > M_PI / 2) {
                branches[cid].angle = (float) (M_PI / 2);
            }
        }
        startAngle[cid] = add;
        add += branches[cid].angle;
        float ang = startAngle[cid] + branches[cid].angle/2;
        float rad = r[branches[cid].level];
        branches[cid].x = (float) (rad * std::sin(ang));
        branches[cid].z = (float) (rad * std::cos(ang));
        assignAngles(cid);
    }
}

void LayoutCT::assignLocations() {
    Branch &root = simct->branches[order[order.size() - 1]];
    float maxFn = simct->data->fnVals[root.to] - simct->data->fnVals[root.from];
    float minFn = simct->data->fnVals[0];
    float ratio;
    float valOfOne = 2;
    if(r.size() > 2) {
        valOfOne = r[r.size() - 1];
    }
    ratio = 2 * valOfOne / maxFn;
    std::unordered_set<uint32_t> nodeIds;
    for(int brno: brMap) {
        Branch &br = simct->branches[brno];
        nodeIds.insert(br.from);
        nodeIds.insert(br.to);
    }
    for(int n: nodeIds) {
        nodeLocs[n].y = (simct->data->fnVals[n] - minFn) * ratio - valOfOne;
        int bid = branchIds[n];
        nodeLocs[n].x = branches[bid].x;
        nodeLocs[n].z = branches[bid].z;
    }
    layoutExtent = valOfOne * 2;
}

void LayoutCT::assignBranchIds() {
    branchIds.clear();
    // start woth root
    std::deque<uint32_t> queue;
    queue.push_back(0);
    while(queue.size() > 0) {
        uint32_t brNo = queue.front();
        queue.pop_front();
        Branch &br = simct->branches[brMap[brNo]];
        if(brNo == 0) {
            branchIds[br.from] = brNo;
            branchIds[br.to] = brNo;
        } else {
            uint32_t parent = invMap[br.parent];
            if(simct->nodes[br.from].prev.size() == 0) {
                // minima - saddle branch
                branchIds[br.from] = brNo;
                branchIds[br.to] = parent;
            } else if(simct->nodes[br.to].next.size() == 0) {
                // saddle - maxima branch
                branchIds[br.from] = parent;
                branchIds[br.to] = brNo;
            } else {
                std::cerr << "is this possible???\n";
                exit(0);
            }
        }
        for(auto cno: br.children) {
            if(brSet.find(cno) == brSet.end()) {
                continue;
            }
            queue.push_back(invMap[cno]);
        }
    }
}

} // namespace contourtree
