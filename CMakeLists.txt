cmake_minimum_required(VERSION 3.20)
project(XboxUIPrototype LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(XboxUIPrototype WIN32
    src/main.cpp
)

target_include_directories(XboxUIPrototype PRIVATE src)

target_link_libraries(XboxUIPrototype
    d3d11
    dxgi
    d3dcompiler
    xinput
)
