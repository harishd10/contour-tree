QT += core

CONFIG += c++11

TARGET = ContourTree
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    MergeTree.cpp \
    SimplifyCT.cpp \
    ContourTreeData.cpp \
    Persistence.cpp \
    TriMesh.cpp \
    TopologicalFeatures.cpp \
    HyperVolume.cpp \
    ContourTree.cpp

HEADERS += \
    DisjointSets.hpp \
    MergeTree.hpp \
    ScalarFunction.hpp \
    Grid3D.hpp \
    SimplifyCT.hpp \
    ContourTreeData.hpp \
    constants.h \
    SimFunction.hpp \
    Persistence.hpp \
    TriMesh.hpp \
    TopologicalFeatures.hpp \
    HyperVolume.hpp \
    ContourTree.hpp

# Unix configuration
unix:!macx{
    QMAKE_CXXFLAGS += -fopenmp
    QMAKE_LFLAGS   += -fopenmp
}

win32{
    CONFIG += console
    QMAKE_CXXFLAGS += -openmp
    QMAKE_LFLAGS   += -openmp
}
