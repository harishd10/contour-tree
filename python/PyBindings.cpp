#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>
#include <vector>
#include "GraphScalarFunction.hpp"
#include "constants.h"
#include "TriMesh.hpp"
#include "Grid3D.hpp"

namespace py = pybind11;

PYBIND11_MODULE(contour_tree, m) {
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
    
    // Expose Vertex struct
    py::class_<contourtree::GraphScalarFunction::Vertex>(m, "Vertex")
        .def(py::init<>())
        .def_readwrite("adj", &contourtree::GraphScalarFunction::Vertex::adj);
    
    // Expose GraphScalarFunction class
    py::class_<contourtree::GraphScalarFunction, contourtree::ScalarFunction>(m, "GraphScalarFunction")
        .def(py::init<>())
        .def("getMaxDegree", &contourtree::GraphScalarFunction::getMaxDegree)
        .def("getVertexCount", &contourtree::GraphScalarFunction::getVertexCount)
        .def("getStar", [](contourtree::GraphScalarFunction& self, int64_t v) {
            std::vector<int64_t> star(self.getMaxDegree());
            int count = self.getStar(v, star);
            star.resize(count);
            return star;
        }, "Get star neighbors of vertex v")
        .def("lessThan", &contourtree::GraphScalarFunction::lessThan)
        .def("getFunctionValue", &contourtree::GraphScalarFunction::getFunctionValue)
        .def("initialize", &contourtree::GraphScalarFunction::initialize,
             "Initialize graph with specified number of nodes")
        .def("loadGraph", &contourtree::GraphScalarFunction::loadGraph,
             "Load graph from edge file")
        .def("updateFnValues", &contourtree::GraphScalarFunction::updateFnValues,
             "Update function values")
        .def_readwrite("nv", &contourtree::GraphScalarFunction::nv)
        .def_readwrite("fnVals", &contourtree::GraphScalarFunction::fnVals)
        .def_readwrite("vertices", &contourtree::GraphScalarFunction::vertices)
        .def_readwrite("maxStar", &contourtree::GraphScalarFunction::maxStar);

    // Expose TriMesh::Vertex struct
    py::class_<contourtree::TriMesh::Vertex>(m, "TriMeshVertex")
        .def(py::init<>())
        .def_readwrite("adj", &contourtree::TriMesh::Vertex::adj);

    // Expose TriMesh class
    py::class_<contourtree::TriMesh, contourtree::ScalarFunction>(m, "TriMesh")
        .def(py::init<>())
        .def("getMaxDegree", &contourtree::TriMesh::getMaxDegree)
        .def("getVertexCount", &contourtree::TriMesh::getVertexCount)
        .def("getStar", [](contourtree::TriMesh& self, int64_t v) {
            std::vector<int64_t> star(self.getMaxDegree());
            int count = self.getStar(v, star);
            star.resize(count);
            return star;
        }, "Get star neighbors of vertex v")
        .def("lessThan", &contourtree::TriMesh::lessThan)
        .def("getFunctionValue", &contourtree::TriMesh::getFunctionValue)
        .def("loadData", &contourtree::TriMesh::loadData, "Load mesh data from file")
        .def_readwrite("nv", &contourtree::TriMesh::nv)
        .def_readwrite("vertices", &contourtree::TriMesh::vertices)
        .def_readwrite("maxStar", &contourtree::TriMesh::maxStar);

    // Grid3D (template) bindings: instantiate for float and double
    using Grid3DFloat = contourtree::Grid3D<float>;
    using Grid3DDouble = contourtree::Grid3D<double>;

    py::class_<Grid3DFloat, contourtree::ScalarFunction>(m, "Grid3DFloat")
        .def(py::init<int,int,int>(), py::arg("resx"), py::arg("resy"), py::arg("resz"))
        .def("getMaxDegree", &Grid3DFloat::getMaxDegree)
        .def("getVertexCount", &Grid3DFloat::getVertexCount)
        .def("getStar", [](Grid3DFloat& self, int64_t v) {
            std::vector<int64_t> star(self.getMaxDegree());
            int count = self.getStar(v, star);
            star.resize(count);
            return star;
        }, "Get star neighbors of vertex v")
        .def("lessThan", &Grid3DFloat::lessThan)
        .def("getFunctionValue", &Grid3DFloat::getFunctionValue)
        .def("loadGrid", &Grid3DFloat::loadGrid, py::arg("fileName"), "Load scalar field from binary file")
        .def_readonly("dimx", &Grid3DFloat::dimx)
        .def_readonly("dimy", &Grid3DFloat::dimy)
        .def_readonly("dimz", &Grid3DFloat::dimz)
        .def_readonly("nv", &Grid3DFloat::nv);

    py::class_<Grid3DDouble, contourtree::ScalarFunction>(m, "Grid3DDouble")
        .def(py::init<int,int,int>(), py::arg("resx"), py::arg("resy"), py::arg("resz"))
        .def("getMaxDegree", &Grid3DDouble::getMaxDegree)
        .def("getVertexCount", &Grid3DDouble::getVertexCount)
        .def("getStar", [](Grid3DDouble& self, int64_t v) {
            std::vector<int64_t> star(self.getMaxDegree());
            int count = self.getStar(v, star);
            star.resize(count);
            return star;
        }, "Get star neighbors of vertex v")
        .def("lessThan", &Grid3DDouble::lessThan)
        .def("getFunctionValue", &Grid3DDouble::getFunctionValue)
        .def("loadGrid", &Grid3DDouble::loadGrid, py::arg("fileName"), "Load scalar field from binary file")
        .def_readonly("dimx", &Grid3DDouble::dimx)
        .def_readonly("dimy", &Grid3DDouble::dimy)
        .def_readonly("dimz", &Grid3DDouble::dimz)
        .def_readonly("nv", &Grid3DDouble::nv);
}
