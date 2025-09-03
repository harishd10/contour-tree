#include "GraphScalarFunction.hpp"

#include <iostream>
#include <fstream>
#include <algorithm>
#include <cassert>
#include <sstream>

namespace contourtree {

GraphScalarFunction::GraphScalarFunction() {
}

int GraphScalarFunction::getMaxDegree() {
    return maxStar;
}

int GraphScalarFunction::getVertexCount() {
    return nv;
}

int GraphScalarFunction::getStar(int64_t v, std::vector<int64_t> &star) {
    int ct = 0;
    for(uint32_t vv: vertices[v].adj) {
        star[ct] = vv;
        ct ++;
    }
    return ct;
}

bool GraphScalarFunction::lessThan(int64_t v1, int64_t v2) {
    if(fnVals[v1] < fnVals[v2]) {
        return true;
    } else if(fnVals[v1] == fnVals[v2]) {
        return (v1 < v2);
    }
    return false;
}

scalar_t GraphScalarFunction::getFunctionValue(int64_t v) {
    return this->fnVals[v];
}

void GraphScalarFunction::initialize(uint32_t noNodes) {
    this->nv = noNodes;

    this->vertices.clear();
    this->vertices.resize(nv);

    this->fnVals.clear();
    this->fnVals.resize(nv);

}


inline std::vector<std::string> splitString(std::string s, char delim) {
    std::vector<std::string> ret;
    std::string t;
    std::stringstream ss(s);
    while (std::getline(ss, t, delim)) {
        ret.push_back(t);
    }
    return ret;
}

void GraphScalarFunction::loadGraph(std::string edgeFile) {
    std::ifstream ip(edgeFile);
    std::string s;

    int v1 = 0;
    while(std::getline(ip,s)) {
        std::vector<std::string> adj = splitString(s,',');
        for(std::string &e: adj) {
            int v2 = std::atoi(e.c_str());
            if(v1 != v2) {
                this->vertices[v1].adj.insert(v2);
                this->vertices[v2].adj.insert(v1);
            }
        }
        v1 ++;
    }

    maxStar = 0;
    for(int i = 0;i < nv;i ++) {
        maxStar = std::max(maxStar,(int)vertices[i].adj.size());
    }
}

void GraphScalarFunction::updateFnValues(const std::vector<scalar_t> &fn) {
    assert(fn.size() == nv);

    for(int i = 0;i < nv;i ++) {
        this->fnVals[i] = fn[i];
    }
}

}
