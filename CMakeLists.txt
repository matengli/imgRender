cmake_minimum_required(VERSION 3.15)
project(imgRender)

set(CMAKE_CXX_STANDARD 14)

add_executable(imgRender main.cpp origin/tgaimage.cpp origin/tgaimage.h)