#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>
#include <vector>
#include "GraphScalarFunction.hpp"
#include "constants.h"
#include "TriMesh.hpp"
#include "Grid3D.hpp"
#include "MergeTree.hpp"
#include "ContourTreeData.hpp"
#include "SimplifyCT.hpp"
#include "SimFunction.hpp"
#include "Persistence.hpp"
#include "HyperVolume.hpp"
#include "TopologicalFeatures.hpp"
#include "LayoutCT.hpp"

namespace py = pybind11;

PYBIND11_MODULE(pyct, m) {
    m.doc() = "Python bindings for Contour Tree library";
    
    // Expose constants
    m.attr("REGULAR") = contourtree::REGULAR;
    m.attr("MINIMUM") = contourtree::MINIMUM;
    m.attr("MAXIMUM") = contourtree::MAXIMUM;
    m.attr("SADDLE") = contourtree::SADDLE;
    
    // Expose TreeType enum
    py::enum_<contourtree::TreeType>(m, "TreeType")
        .value("TypeJoinTree", contourtree::TypeJoinTree)
        .value("TypeSplitTree", contourtree::TypeSplitTree)
        .value("TypeContourTree", contourtree::TypeContourTree);
    
    // Expose ScalarFunction base class
    py::class_<contourtree::ScalarFunction>(m, "ScalarFunction")
        .def("getMaxDegree", &contourtree::ScalarFunction::getMaxDegree)
        .def("getVertexCount", &contourtree::ScalarFunction::getVertexCount)
        .def("getStar", [](contourtree::ScalarFunction& self, int64_t v) {
            std::vector<int64_t> star(self.getMaxDegree());
            int count = self.getStar(v, star);
            star.resize(count);
            return star;
        }, "Get star neighbors of vertex v")
        .def("lessThan", &contourtree::ScalarFunction::lessThan)
        .def("getFunctionValue", &contourtree::ScalarFunction::getFunctionValue);
    
    // Expose GraphScalarFunction class
    py::class_<contourtree::GraphScalarFunction, contourtree::ScalarFunction>(m, "GraphScalarFunction")
        .def(py::init<>())
        .def("getVertexCount", &contourtree::GraphScalarFunction::getVertexCount)
        .def("initialize", &contourtree::GraphScalarFunction::initialize,
             "Initialize graph with specified number of nodes")
        .def("loadGraph", &contourtree::GraphScalarFunction::loadGraph,
             "Load graph from edge file")
        .def("updateFnValues", &contourtree::GraphScalarFunction::updateFnValues,
             "Update function values");

    // Expose TriMesh class
    py::class_<contourtree::TriMesh, contourtree::ScalarFunction>(m, "TriMesh")
        .def(py::init<>())
        .def("getVertexCount", &contourtree::TriMesh::getVertexCount)
        .def("getFunctionValue", &contourtree::TriMesh::getFunctionValue)
        .def("loadData", &contourtree::TriMesh::loadData, "Load mesh data from file");

    // Grid3D (template) bindings: instantiate for float and double
    using Grid3DUint8 = contourtree::Grid3D<uint8_t>;
    py::class_<Grid3DUint8, contourtree::ScalarFunction>(m, "Grid3DUint8")
        .def(py::init<int,int,int>(), py::arg("resx"), py::arg("resy"), py::arg("resz"))
        .def("getVertexCount", &Grid3DUint8::getVertexCount)
        .def("getFunctionValue", &Grid3DUint8::getFunctionValue)
        .def("loadGrid", &Grid3DUint8::loadGrid, py::arg("fileName"), "Load scalar field from binary file")
        .def_readonly("dimx", &Grid3DUint8::dimx)
        .def_readonly("dimy", &Grid3DUint8::dimy)
        .def_readonly("dimz", &Grid3DUint8::dimz);

    using Grid3DFloat = contourtree::Grid3D<float>;
    py::class_<Grid3DFloat, contourtree::ScalarFunction>(m, "Grid3DFloat")
        .def(py::init<int,int,int>(), py::arg("resx"), py::arg("resy"), py::arg("resz"))
        .def("getVertexCount", &Grid3DFloat::getVertexCount)
        .def("getFunctionValue", &Grid3DFloat::getFunctionValue)
        .def("loadGrid", &Grid3DFloat::loadGrid, py::arg("fileName"), "Load scalar field from binary file")
        .def_readonly("dimx", &Grid3DFloat::dimx)
        .def_readonly("dimy", &Grid3DFloat::dimy)
        .def_readonly("dimz", &Grid3DFloat::dimz);

    using Grid3DDouble = contourtree::Grid3D<double>;
    py::class_<Grid3DDouble, contourtree::ScalarFunction>(m, "Grid3DDouble")
        .def(py::init<int,int,int>(), py::arg("resx"), py::arg("resy"), py::arg("resz"))
        .def("getVertexCount", &Grid3DDouble::getVertexCount)
        .def("getFunctionValue", &Grid3DDouble::getFunctionValue)
        .def("loadGrid", &Grid3DDouble::loadGrid, py::arg("fileName"), "Load scalar field from binary file")
        .def_readonly("dimx", &Grid3DDouble::dimx)
        .def_readonly("dimy", &Grid3DDouble::dimy)
        .def_readonly("dimz", &Grid3DDouble::dimz);

    // Expose MergeTree class
    py::class_<contourtree::MergeTree>(m, "MergeTree")
        .def(py::init<>())
        .def("computeTree", &contourtree::MergeTree::computeTree,
                py::arg("data"), py::arg("treeType"),
                "Compute the specified tree using the provided ScalarFunction")
        .def("output", &contourtree::MergeTree::output,
                py::arg("fileName"), py::arg("treeType"),
                "Write tree data to files with the given base name");

    // Expose ContourTreeData class (minimal)
    py::class_<contourtree::ContourTreeData>(m, "ContourTreeData")
        .def(py::init<>())
        .def("loadBinFile", &contourtree::ContourTreeData::loadBinFile, py::arg("filename"), "Load data from a binary file");

    // Base class for similarity functions (no Python overrides needed right now)
    py::class_<contourtree::SimFunction>(m, "SimFunction");

    // Persistence and HyperVolume constructors
    py::class_<contourtree::Persistence, contourtree::SimFunction>(m, "Persistence")
        .def(py::init<const contourtree::ContourTreeData&>(), py::arg("ctData"));

    py::class_<contourtree::HyperVolume, contourtree::SimFunction>(m, "HyperVolume")
        .def(py::init<const contourtree::ContourTreeData&, std::string>(),
             py::arg("ctData"), py::arg("partFile"));

    // Expose SimplifyCT class
    py::class_<contourtree::SimplifyCT>(m, "SimplifyCT")
        .def(py::init<>())
        .def("setInput", &contourtree::SimplifyCT::setInput, py::arg("data"))
        .def(
            "simplify",
            static_cast<void (contourtree::SimplifyCT::*)(contourtree::SimFunction*)>(&contourtree::SimplifyCT::simplify),
            py::arg("simFn"),
            "Simplify using Persistence or HyperVolume")
        .def("outputOrder", &contourtree::SimplifyCT::outputOrder, py::arg("fileName"), py::arg("normalize"), "Write the branch removal order to disk");

    // Expose TopologicalFeatures::Feature class
    py::class_<contourtree::Feature>(m, "Feature")
        .def(py::init<>())
        .def_readwrite("arcs", &contourtree::Feature::arcs)
        .def_readwrite("frm", &contourtree::Feature::from)
        .def_readwrite("to", &contourtree::Feature::to);

    // Expose TopologicalFeatures class
    py::class_<contourtree::TopologicalFeatures>(m, "TopologicalFeatures")
        .def(py::init<>())
        .def("loadData", &contourtree::TopologicalFeatures::loadData, py::arg("filename"))
        .def("getArcFeatures", [](contourtree::TopologicalFeatures& self, int topk, float th) {
            int topk_copy = topk;
            auto features = self.getArcFeatures(topk_copy, th);
            return py::make_tuple(features, topk_copy);
        }, py::arg("topk"), py::arg("th") = 0.0f, "Returns tuple of (features, updated_topk)")
        .def("getPartitionedExtremaFeatures", [](contourtree::TopologicalFeatures& self, int topk, float th) {
            int topk_copy = topk;
            auto features = self.getPartitionedExtremaFeatures(topk_copy, th);
            return py::make_tuple(features, topk_copy);
        }, py::arg("topk"), py::arg("th") = 0.0f, "Returns tuple of (features, updated_topk)");

    // Expose Point struct
    py::class_<contourtree::Point>(m, "Point")
        .def(py::init<>())
        .def_readwrite("x", &contourtree::Point::x)
        .def_readwrite("y", &contourtree::Point::y)
        .def_readwrite("z", &contourtree::Point::z);

    // Expose LayoutCT class
    py::class_<contourtree::LayoutCT>(m, "LayoutCT")
        .def(py::init<contourtree::TopologicalFeatures*>(), py::arg("tf"))
        .def("layoutTree", &contourtree::LayoutCT::layoutTree, py::arg("simplifiedCount"))
        .def("getNodeLocations", &contourtree::LayoutCT::getNodeLocations);

}
