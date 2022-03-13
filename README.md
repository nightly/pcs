# Process Controller Synthesis

![standard](https://img.shields.io/badge/c%2B%2B-23-blue.svg)
![ci.yml](https://github.com/nightly/fedes/actions/workflows/ci.yml/badge.svg)

# Build
Clone the repository alongside its submodules.
```
git clone --recurse-submodules https://github.com/nightly/pcs
```

Build the project using CMake, which will automatically invoke the tests unless otherwise specified.
```
cmake -S . -B build
cmake --build build
```

The tests can also be run with the following:
```
cd build
ctest -j 8 --output-on-failure --verbose
```