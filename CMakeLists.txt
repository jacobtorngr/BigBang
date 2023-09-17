cmake_minimum_required(VERSION 3.16)
project(BigBang)

set(CMAKE_C_STANDARD 99)

set(${PROJECT_SOURCE_DIR} src)

add_executable(${PROJECT_NAME} 
    src/BigBang.c
    src/GravityNstuff.c
    src/tga.c 
    src/PlotFunctions.c
    src/FileName.c
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)

include_directories(${PYTHON_INCLUDE_DIRS})
find_package(PythonLibs REQUIRED)

target_link_libraries(${PROJECT_NAME} 
    m 
    ${PYTHON_LIBRARIES} 
)