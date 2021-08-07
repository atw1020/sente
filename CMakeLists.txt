cmake_minimum_required(VERSION 3.4)
project(sente)

set(CMAKE_CXX_STANDARD 11)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# set compiler to be g++ on windows
if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(CMAKE_CXX_COMPILER "g++")
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_RELEASE "-Ofast")

set(CMAKE_CXX_FLAGS_DEBUG "-g -DDEBUG_LOG")

add_subdirectory(pybind11)
# pybind11_add_module(sente src/module.cpp src/include/Move.h src/Move.cpp)
pybind11_add_module(sente src/module.cpp src/Group.cpp src/include/Group.h src/include/GoGame.h src/GoGame.cpp
                          src/Move.cpp src/include/Move.h src/include/Board.h src/Board.cpp src/include/Tree.h
                          src/SGF.cpp src/include/SGF.h src/include/GoComponents.h src/GoComponents.cpp
                          src/SenteExceptions.cpp src/include/SenteExceptions.h src/include/LifeAndDeath.h
                          src/LifeAndDeath.cpp)
