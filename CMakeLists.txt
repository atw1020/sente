cmake_minimum_required(VERSION 3.4)
project(sente)

set(CMAKE_CXX_STANDARD 20)

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
pybind11_add_module(sente src/module.cpp src/Game/Group.cpp src/Game/Group.h src/Game/GoGame.h src/Game/GoGame.cpp
                    src/Game/Move.cpp src/Game/Move.h src/Game/Board.h src/Game/Board.cpp src/Utils/Tree.h
                    src/Utils/SGF/SGF.cpp src/Utils/SGF/SGF.h src/Game/GoComponents.h src/Game/GoComponents.cpp
                    src/Utils/SenteExceptions.cpp src/Utils/SenteExceptions.h src/Game/LifeAndDeath.h
                    src/Game/LifeAndDeath.cpp src/Utils/Numpy.h src/Utils/Numpy.cpp src/Utils/SGF/SGFNode.h
                    src/Utils/SGF/SGFNode.cpp src/Utils/SGF/SGFProperty.h src/Utils/SGF/SGFProperty.cpp
                    src/Utils/GTP/Tokens/Token.h src/Utils/GTP/Tokens/Token.cpp
                    src/Utils/GTP/Tokens/Seperator.h src/Utils/GTP/Tokens/Seperator.cpp
                    src/Utils/GTP/Tokens/Literal.h src/Utils/GTP/Tokens/Literal.cpp
                    src/Utils/GTP/Parser.h src/Utils/GTP/Parser.cpp
                    src/Utils/GTP/Operators.h src/Utils/GTP/Operators.cpp
                    src/Utils/GTP/Controller.h src/Utils/GTP/Controller.cpp
                    src/Utils/GTP/Engine.h src/Utils/GTP/Engine.cpp
                    src/Utils/GTP/CustomRegistration.h src/Utils/GTP/CustomRegistration.cpp)
