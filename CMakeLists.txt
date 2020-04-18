cmake_minimum_required(VERSION 2.8)

project(two-square-cipher)
add_executable(${PROJECT_NAME} "main.cpp" mainwindow.cpp mainwindow.hpp)
set_target_properties(two-square-cipher  PROPERTIES AUTOMOC ON AUTOUIC ON)
find_package(Qt5 COMPONENTS Gui Core Widgets REQUIRED)
target_link_libraries(two-square-cipher  Qt5::Core Qt5::Widgets Qt5::Gui)
