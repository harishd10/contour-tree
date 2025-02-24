#include "ContourTreeData.hpp"

#include <fstream>
#include <iostream>
#include <cassert>
#include "constants.h"

namespace contourtree {

#define EPSILON 1e-10

ContourTreeData::ContourTreeData() {}

void ContourTreeData::loadBinFile(std::string fileName) {
    // read meta data
    {
        std::ifstream ip(fileName + ".rg.dat");
        ip >> noNodes;
        ip >> noArcs;
        assert(noNodes == noArcs + 1);
        ip.close();
    }
    std::cout << noNodes << " " << noArcs << std::endl;

    std::vector<int64_t> nodeids(noNodes);
    std::vector<scalar_t> nodefns(noNodes);
    std::vector<char> nodeTypes(noNodes);
    std::vector<int64_t> arcs(noArcs * 2);

    // read the tree
    std::string rgFile = fileName + ".rg.bin";
    std::ifstream ip(rgFile, std::ios::binary);
    ip.read((char*)nodeids.data(), nodeids.size() * sizeof(int64_t));
    ip.read((char*)nodefns.data(), nodeids.size() * sizeof(scalar_t));
    ip.read((char*)nodeTypes.data(), nodeids.size());
    ip.read((char*)arcs.data(), arcs.size() * sizeof(int64_t));
    ip.close();

    std::cout << "finished reading data" << std::endl;
    this->loadData(nodeids, nodefns, nodeTypes, arcs);
}

void ContourTreeData::loadTxtFile(std::string fileName) {
    std::ifstream ip(fileName);
    ip >> noNodes;
    ip >> noArcs;

    std::vector<int64_t> nodeids(noNodes);
    std::vector<scalar_t> nodefns(noNodes);
    std::vector<char> nodeTypes(noNodes);
    std::vector<int64_t> arcs(noArcs * 2);

    for (size_t i = 0; i < noNodes; i++) {
        int64_t v;
        float fn;
        ip >> v;
        ip >> fn;
        char t;
        std::string type;
        ip >> type;
        if (type.compare("MINIMA") == 0) {
            t = MINIMUM;
        } else if (type.compare("MAXIMA") == 0) {
            t = MAXIMUM;
        } else if (type.compare("SADDLE") == 0) {
            t = SADDLE;
        } else {
            t = REGULAR;
        }
        nodeids[i] = v;
        nodefns[i] = (scalar_t)(fn);
        nodeTypes[i] = t;
    }
    for (size_t i = 0; i < noArcs; i++) {
        int v1, v2;
        ip >> v1 >> v2;
        arcs[i * 2 + 0] = v1;
        arcs[i * 2 + 1] = v2;
    }
    ip.close();
    std::cout << "finished reading data" << std::endl;
    this->loadData(nodeids, nodefns, nodeTypes, arcs);
}

void ContourTreeData::loadData(const std::vector<int64_t>& nodeids,
                               const std::vector<scalar_t>& nodefns,
                               const std::vector<char>& nodeTypes,
                               const std::vector<int64_t>& iarcs) {
    origNodes = noNodes;
    origArcs = noArcs;

    nodes.resize(noNodes);
    nodeVerts.resize(noNodes);
    fnVals.resize(noNodes);
    type.resize(noNodes);
    arcs.resize(noArcs);

    maxNodeId = 0;
    scalar_t minf = nodefns[0];
    scalar_t maxf = nodefns[noNodes - 1];
    for (uint32_t i = 0; i < noNodes; i++) {
        assert(nodefns[i] >= minf && nodefns[i] <= maxf);
        nodeVerts[i] = nodeids[i];
        fnVals[i] = (float)(nodefns[i] - minf) / (maxf - minf);
        type[i] = nodeTypes[i];
        nodeMap[nodeVerts[i]] = i;
        maxNodeId = std::max(maxNodeId,nodeVerts[i]);
    }

    for (uint32_t i = 0; i < noArcs; i++) {
        arcs[i].from = nodeMap[iarcs[i * 2 + 0]];
        arcs[i].to = nodeMap[iarcs[i * 2 + 1]];
        arcs[i].id = i;
        nodes[arcs[i].from].next.push_back(i);
        nodes[arcs[i].to].prev.push_back(i);
    }

    this->handleDegenerateExtrema();
}

void ContourTreeData::handleDegenerateExtrema() {
    uint32_t nn = noNodes;
    for(int i = 0;i < nn;i ++) {
        if(nodes[i].prev.size() == 0) {
            // minima
            if(nodes[i].next.size() > 1) {
                // degenerate
                int64_t newNode = noNodes;
                uint32_t newArc = noArcs;

                nodeVerts.push_back(maxNodeId+1);
                nodeMap[maxNodeId+1] = newNode;
                fnVals.push_back(fnVals[i] - EPSILON);
                type.push_back(MINIMUM);
                type[i] = SADDLE;

                arcs.push_back(Arc());
                arcs[newArc].from = newNode;
                arcs[newArc].to = i;
                arcs[newArc].id = newArc;

                nodes.push_back(Node());
                nodes[newNode].next.push_back(newArc);
                nodes[i].prev.push_back(newArc);

                noArcs ++;
                noNodes ++;
                maxNodeId ++;
            }
        }

        if(nodes[i].next.size() == 0) {
            // maxima
            if(nodes[i].prev.size() > 1) {
                // degenerate
                int64_t newNode = noNodes;
                uint32_t newArc = noArcs;

                nodeVerts.push_back(maxNodeId+1);
                nodeMap[maxNodeId+1] = newNode;
                fnVals.push_back(fnVals[i]);
                type.push_back(MAXIMUM);
                type[i] = SADDLE;

                arcs.push_back(Arc());
                arcs[newArc].from = i;
                arcs[newArc].to = newNode;
                arcs[newArc].id = newArc;

                nodes.push_back(Node());
                nodes[newNode].prev.push_back(newArc);
                nodes[i].next.push_back(newArc);

                noArcs ++;
                noNodes ++;
                maxNodeId ++;
            }
        }
    }
}

}  // namespace contourtree
