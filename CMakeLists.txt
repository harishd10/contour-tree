cmake_minimum_required(VERSION 3.1)

project(ContourTree)

set (CMAKE_CXX_STANDARD 17)

# Add header files
set(HEADER_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/ContourTree/constants.h
    ${CMAKE_CURRENT_SOURCE_DIR}/ContourTree/ContourTree.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ContourTree/ContourTreeData.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ContourTree/DisjointSets.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ContourTree/Grid3D.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ContourTree/HyperVolume.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ContourTree/MergeTree.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ContourTree/Persistence.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ContourTree/ScalarFunction.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ContourTree/SimFunction.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ContourTree/SimplifyCT.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ContourTree/TopologicalFeatures.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ContourTree/TriMesh.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ContourTree/GraphScalarFunction.hpp
)

# Add source files
set(SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/ContourTree/ContourTree.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ContourTree/ContourTreeData.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ContourTree/HyperVolume.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ContourTree/MergeTree.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ContourTree/Persistence.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ContourTree/SimplifyCT.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ContourTree/TopologicalFeatures.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ContourTree/TriMesh.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ContourTree/GraphScalarFunction.cpp
)

set(MAIN_SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/ContourTree/main.cpp
)


# Create library
add_library(ContourTree STATIC ${SOURCE_FILES} ${HEADER_FILES})

add_executable(TestContourTree ${MAIN_SOURCE_FILES})
target_link_libraries(TestContourTree ContourTree)

target_include_directories(ContourTree PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/ContourTree)
target_include_directories(TestContourTree PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/ContourTree)

# Python bindings (pybind11)
find_package(pybind11 REQUIRED)
find_package(Python3 COMPONENTS Interpreter Development NumPy REQUIRED)

add_library(pyct MODULE
    python/PyBindings.cpp
)
target_link_libraries(pyct PRIVATE pybind11::module ContourTree Python3::Python Python3::NumPy)
target_include_directories(pyct PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/ContourTree)
set_target_properties(pyct PROPERTIES
    PREFIX ""
    OUTPUT_NAME "pyct"
    SUFFIX ".pyd"
)
