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
# pybind11_add_module(sente src/module.cpp src/include/Move.h src/Move.cpp)
pybind11_add_module(sente src/module.cpp src/Game/Group.cpp src/include/Game/Group.h src/include/Game/GoGame.h src/Game/GoGame.cpp
                    src/Game/Move.cpp src/include/Game/Move.h src/include/Game/Board.h src/Game/Board.cpp src/include/Utils/Tree.h
                    src/Utils/SGF.cpp src/include/Utils/SGF.h src/include/Game/GoComponents.h src/Game/GoComponents.cpp
                    src/Utils/SenteExceptions.cpp src/include/Utils/SenteExceptions.h src/include/Game/LifeAndDeath.h
                    src/Game/LifeAndDeath.cpp src/include/Utils/Numpy.h src/Utils/Numpy.cpp)
