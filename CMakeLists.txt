cmake_minimum_required(VERSION 3.15)
project(imgRender)

set(CMAKE_CXX_STANDARD 14)

add_executable(imgRender main.cpp origin/tgaimage.cpp origin/tgaimage.h origin/Model.cpp origin/Model.h origin/GlobalFunctions.h origin/Geomotry.h origin/Geomotry.cpp origin/render.cpp origin/render.h)