# Process Controller Synthesis

![standard](https://img.shields.io/badge/c%2B%2B-23-blue.svg)
![ci.yml](https://github.com/nightly/pcs/actions/workflows/ci.yml/badge.svg)

## Requirements
- CMake (>=3.8)
- Git (for submodule cloning)

## Build instructions
Clone the repository alongside its submodules.
```
git clone --recurse-submodules https://github.com/nightly/pcs
```

Build the project using CMake, which will automatically invoke the tests unless otherwise specified.
```
cmake -S. -B build/x64/release -DCMAKE_BUILD_TYPE=Release
cmake --build build/x64/release
```

The tests can also be run with the following:
```
cd build/x64/release
ctest -j 8 --output-on-failure --verbose
```

## Layout 
- `app`: contains an executable command line application
- `src/pcs`: contains the PCS library code
- `tests`: contains all tests & test data