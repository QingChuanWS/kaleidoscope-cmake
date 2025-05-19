# Kaleidoscope-cmake

## Development Environment

This project has been tested on Ubuntu 22.04 and Mac OS X 11.7. In order to build this repo you will need:

- LLVM > 14
- C++ compiler that supports C++17
- CMake 3.10 or higher

## Build & Testing

Firstly, you need to clone this repo:

```
git clone --recursive https://github.com/QingChuanWS/kaleidoscope-cmake.git
```

```
mkdir build && cd build
cmake ..
make
```

run the unit-test:

```
(in build folder)
./test/kaleidoscope_tests
```
