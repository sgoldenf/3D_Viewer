# 3D_Viewer
This is team project of application for viewing 3D wireframe models. 
### Notes
- Uses MVC and design patterns
- Uses QT Framework and OpenGL, but for educational purposes we've written affine transformations using our own `Matrix` class (source code in `src/libs/matrix` directory)
- Tests for Model provided in `src/test_dir` directory and can be run with `make tests` command from `src` directory. `make gcov_report` creates web-page with Coverage Report in `src/lcov_report_dir` directory

## Build
To build from `src` directory use command `make`

## Install
### MacOS
To install from `src` directory use command `make install`
### Linux
`cp -r build/3DViewer.app path/to/app/3DViewer.app`
