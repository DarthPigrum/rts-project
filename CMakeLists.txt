cmake_minimum_required(VERSION 3.17)
project(rts)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5Widgets)
find_package(Qt5PrintSupport)
find_package(Qt5Qml)

add_executable(rts.out rts.cpp rts/gui.h rts/gui.ui 3rdparty/qcustomplot/qcustomplot.h 3rdparty/qcustomplot/qcustomplot.cpp)
qt5_use_modules(rts.out Widgets PrintSupport)
