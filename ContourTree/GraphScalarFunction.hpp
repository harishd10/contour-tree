#ifndef GRAPHSCALARFUNCTION_HPP
#define GRAPHSCALARFUNCTION_HPP

#include "ScalarFunction.hpp"
#include <vector>
#include <set>
#include <string>

namespace contourtree {

class GraphScalarFunction : public ScalarFunction
{
public:
    struct Vertex {
        std::set<uint32_t> adj;
    };

public:
    GraphScalarFunction();


    int getMaxDegree();
    int getVertexCount();
    int getStar(int64_t v, std::vector<int64_t> &star);
    bool lessThan(int64_t v1, int64_t v2);
    scalar_t getFunctionValue(int64_t v);

public:
    void initialize(uint32_t noNodes);
    void loadGraph(std::string edgeFile);
    void updateFnValues(const std::vector<scalar_t> &fn);

public:
    int nv;
    std::vector<scalar_t> fnVals;
    std::vector<Vertex> vertices;
    int maxStar;
};


}
#endif // GRAPHSCALARFUNCTION_HPP
