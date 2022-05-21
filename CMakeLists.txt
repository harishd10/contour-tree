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
)

# Create library
add_library(ContourTree STATIC ${SOURCE_FILES} ${HEADER_FILES})

target_include_directories(ContourTree PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/ContourTree)
