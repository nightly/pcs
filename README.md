# Process Controller Synthesis

![standard](https://img.shields.io/badge/c%2B%2B-20-blue.svg)
![ci.yml](https://github.com/nightly/pcs/actions/workflows/ci.yml/badge.svg)

## Build instructions
### Requirements
- [CMake](https://cmake.org/) (>=3.22)
- [Git](https://git-scm.com/) (for submodule cloning)
- C++20 compiler

And *optionally*:
- [GraphViz](https://graphviz.org/) (for visualisation images, `dot` must be in system path)

### Cloning & updating
Clone the repository alongside its submodules (shallow submodule cloning is optional).
```
git clone --recurse-submodules --shallow-submodules https://github.com/nightly/pcs
```
To correctly pull any newly added Git submodules to your local repository (`git pull` alone doesn't suffice):
```
git submodule update --recursive
```

### CMake
Build the project using CMake or simply open it using an IDE that has CMake support.
```
cmake -S. -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

All of the tests can also be run with the following:
```
cd build
ctest --output-on-failure --verbose
```

## Layout 
- `app`: contains a CLI executable application. The `examples/` subdirectory has pre-made examples to run.
- `benchmarks`: contains all the project's benchmarks
- `data`: has some sample recipes and modelled resources#
- `external`: 3rd party dependencies added as Git submodules
- `src/pcs`: PCS library code
- `tests`: holds all tests & test data

## GraphViz visualisation support
The labelled transition systems parsed and generated can be visualised using the GraphViz tool.

The output generated in `exports/` can be visualised entirely online [here](http://magjac.com/graphviz-visual-editor/) or [here](https://sketchviz.com/new) without installing GraphViz on your system.

Alternatively with GraphViz installed, the following command can be run for a given LTS file `lts1.gv`:
```
dot -Tpng lts1.gv -o lts1.png
```
Or for SVG format:
```
dot -Tsvg lts1.gv -o lts1.svg
```