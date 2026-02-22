cmake_minimum_required(VERSION 4.0)

project(PracticaUNO_Flip)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

file(GLOB SOURCES "src/*.cpp")

add_executable(UNO_Flip ${SOURCES})