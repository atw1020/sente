cmake_minimum_required(VERSION 3.4)
project(sente)

set(CMAKE_CXX_STANDARD 11)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

if(MSVC)
    # MSVC's equivalent of -Wextra is buggy
    set(CMAKE_CXX_FLAGS "-Wall")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3")
else()
    set(CMAKE_CXX_FLAGS "-Wall -Wextra")
    set(CMAKE_CXX_FLAGS_RELEASE "-Ofast")
endif()

set(CMAKE_CXX_FLAGS_DEBUG "-g -DDEBUG_LOG")

add_subdirectory(pybind11)
# pybind11_add_module(sente src/module.cpp src/Include/Move.h src/Move.cpp)
pybind11_add_module(sente src/module.cpp src/Game/Group.cpp src/Include/Game/Group.h src/Include/Game/GoGame.h src/Game/GoGame.cpp
                    src/Game/Move.cpp src/Include/Game/Move.h src/Include/Game/Board.h src/Game/Board.cpp src/Include/Utils/Tree.h
                    src/Utils/SGF.cpp src/Include/Utils/SGF.h src/Include/Game/GoComponents.h src/Game/GoComponents.cpp
                    src/Utils/SenteExceptions.cpp src/Include/Utils/SenteExceptions.h src/Include/Game/LifeAndDeath.h
                    src/Game/LifeAndDeath.cpp src/Include/Utils/Numpy.h src/Utils/Numpy.cpp src/Include/Utils/SGFNode.h
                    src/Utils/SGFNode.cpp)
