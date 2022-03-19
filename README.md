# Process Controller Synthesis

![standard](https://img.shields.io/badge/c%2B%2B-20-blue.svg)
![ci.yml](https://github.com/nightly/pcs/actions/workflows/ci.yml/badge.svg)

## Requirements
- [CMake](https://cmake.org/) (>=3.8)
- [Git](https://git-scm.com/) (for submodule cloning)
- C++20 compiler
- [GraphViz](https://graphviz.org/) (*optional*: for visualisation purposes)

## Build instructions
Clone the repository alongside its submodules.
```
git clone --recurse-submodules https://github.com/nightly/pcs
```

Build the project using CMake, which will automatically invoke the tests unless otherwise specified.
```
cmake -S. -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

The tests can also be run with the following:
```
cd build
ctest -j 8 --output-on-failure --verbose
```

## Layout 
- `app`: contains an executable command line application
- `src/pcs`: contains the PCS library code
- `tests`: contains all tests & test data

## GraphViz visualisation support
The labelled transition systems parsed and generated can be visualised using the GraphViz tool.

The output generated in the `exports/` can be visualised entirely online [here](http://magjac.com/graphviz-visual-editor/) without installing GraphViz on your system.

Alternatively with GraphViz installed, the following command can be run for a given LTS file:
```
dot -Tpng lts1.txt -o lts1.png
```