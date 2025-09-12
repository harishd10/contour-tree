# TDA using Contour Trees

This is a generalized version of the code used in the project TopoAngler:

* Paper: http://www.harishd.com/home/assets/papers/topoangler.pdf

* Video: https://www.youtube.com/watch?v=gz22DjMRJjk

* Code: https://github.com/ViDA-NYU/Segmentangling

It consists of two parts:

1. it implements the sweep algorithm by Carr et al. ("Computing contour trees in all dimensions", CGTA 2003) 
to compute the contour tree of an input scalar function. 

2. it supports simplification of the contour tree using either topoloigcal persistence or 
hyper volume as the importance measure, and obtain a mapping between the input data and the simplified features. 
This is typically used when analyzing the data (like it was used in TopoAngler)

An example of using the code on a structured 3D grid is provided in main.cpp. 
A 2D grid input can be used by setting the value of *dimz* = 1. The *TriMesh* class can instead be used for a triangle mesh input.
Note that, support for other input types can be added by implementing a subclass of the *ScalarFunction* interface.

I would appreciate it if you can let me know (http://www.harishd.com/home/contact/) if you use this code, 
and optionally, how you are using it (this is purely to keep approximate count of the users of this code).

### 🛠️ Building the C Library

The C library can be built using CMake. Run the following commands:

1. `$ mkdir build-main`
1. `$ cd build-main`
1. `$ cmake ..` OR `cmake -DBUILD_PYCT=ON ..` if you'd like to build the python module as well.
1. `$ cmake --build . --config Release` OR `cmake --build . --config Debug`

The library (and any other built components) should now be available in `build-main/Release` or `build-main/Debug`.

### 🎲 Installing via Pip

1. Create a virtual env (or conda env) with python 3.7 or later
1. Activate the virtual environment and run `pip install .`

### 🐍 Building Python Wheels
To build an installable python wheel, follow these steps:

1. Create a virtual env (or conda env) with python 3.7 or later
1. Activate the virtual environment and run `pip install numpy build`
1. Run `python -m build .`