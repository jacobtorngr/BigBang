cmake_minimum_required(VERSION 3.17)
project(BigBang C)

set(CMAKE_C_STANDARD 99)

add_executable(BigBang BigBang.c BigBang.h GravityNstuff.c tga.c tga.h PlotFunctions.c FileName.c)