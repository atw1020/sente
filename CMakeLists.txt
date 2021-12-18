cmake_minimum_required(VERSION 3.4)
project(sente)

set(CMAKE_CXX_STANDARD 17)

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
pybind11_add_module(sente src/module.cpp src/Game/Group.cpp src/Include/Game/Group.h src/Include/Game/GoGame.h src/Game/GoGame.cpp
                    src/Game/Move.cpp src/Include/Game/Move.h src/Include/Game/Board.h src/Game/Board.cpp src/Include/Utils/Tree.h
                    src/Utils/SGF/SGF.cpp src/Include/Utils/SGF/SGF.h src/Include/Game/GoComponents.h src/Game/GoComponents.cpp
                    src/Utils/SenteExceptions.cpp src/Include/Utils/SenteExceptions.h src/Include/Game/LifeAndDeath.h
                    src/Game/LifeAndDeath.cpp src/Include/Utils/Numpy.h src/Utils/Numpy.cpp src/Include/Utils/SGF/SGFNode.h
                    src/Utils/SGF/SGFNode.cpp src/Include/Utils/SGF/SGFProperty.h src/Utils/SGF/SGFProperty.cpp
                    src/Include/Utils/GTP/Tokens/Token.h src/Utils/GTP/Tokens/Token.cpp
                    src/Include/Utils/GTP/Tokens/Seperator.h src/Utils/GTP/Tokens/Seperator.cpp
                    src/Include/Utils/GTP/Tokens/Literal.h src/Utils/GTP/Tokens/Literal.cpp
                    src/Include/Utils/GTP/Parser.h src/Utils/GTP/Parser.cpp
                    src/Include/Utils/GTP/Host.h src/Utils/GTP/Host.cpp)
