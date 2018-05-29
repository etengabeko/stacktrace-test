Build project with CMake (minimum version 3.1).

Build dependencies:
```
    Qt >= 5.0
    boost >= 1.65.0
```

Build steps:
```
    cd <project_dir>
    cmake -B./build -H. -DBOOST_ROOT=<path-to-boost> -DCMAKE_BUILD_TYPE=Release
    cmake --build ./build
```

For build debug version replace:
```
    -DCMAKE_BUILD_TYPE=Debug
```

For build on Windows with MinGW add to call cmake:
```
    -G"MinGW Makefiles"
```

Build tested with `GCC 7.3.0` and `MinGW 6.3.0`.
