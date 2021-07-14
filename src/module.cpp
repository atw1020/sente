/**
 *
 * Author: Arthur Wesley
 *
 */
#include <fstream>

#include <pybind11/stl.h>
#include <pybind11/eval.h>
#include <pybind11/pybind11.h>

#include "include/SGF.h"
#include "include/GoGame.h"
#include "include/SenteExceptions.h"

int add(int x, int y){
    return x + y;
}

namespace py = pybind11;

PYBIND11_MODULE(sente, module){

    // m.doc() = ""; // todo: add a better description

    module.def("add", &add, py::arg("x"), py::arg("y"));

    module.doc() = "Sente: an open source API for go";
    module.def("opposite_player", &sente::getOpponent, "get the opponent of a particular stone color");


    py::enum_<sente::Stone>(module, "stone")
            .value("BLACK", sente::BLACK)
            .value("WHITE", sente::WHITE)
            .value("EMPTY", sente::EMPTY)
            .export_values();

    py::enum_<sente::Rules>(module, "rules")
            .value("CHINESE", sente::Rules::CHINESE)
            .value("JAPANESE", sente::Rules::JAPANESE)
            .export_values();

    py::class_<sente::Move>(module, "Move")
            .def(py::init<unsigned, unsigned, sente::Stone>(),
                    py::arg("x"),
                    py::arg("y"),
                    py::arg("stone"))
            .def("get_x", &sente::Move::getX,
                 "get the x-coordinate of the move (zero based indices)")
            .def("get_y", &sente::Move::getY,
                 "get the y-coordinate of the move (zero based indices)")
            .def("get_stone", &sente::Move::getStone, "get the stone that the player will place on the board")
            .def("__eq__", &sente::Move::operator==)
            .def("__ne__", &sente::Move::operator!=)
            .def("__repr__", [](const sente::Move& move){
                return std::string(move);
            });

    py::class_<sente::Results>(module, "result")
            .def("get_winner", &sente::Results::winner,
                 "gets the winner of the game")
            .def("get_black_score", [](const sente::Results& results){
                return results.blackScore;
            })
            .def("get_white_score", [](const sente::Results& results){
                return results.whiteScore + results.komi;
            })
            .def("__repr__", [](const sente::Results& results){
                return "<sente.result \"" + std::string(results) + "\">";
            });

    py::class_<sente::Board<19>>(module, "Board19")
            .def(py::init<>())
            .def(py::init<std::array<std::array<sente::Stone, 19>, 19>>())
            .def("get_side", &sente::Board<19>::getSide,
                 "get the length of the side of the board")
            .def("play", &sente::Board<19>::playStone,
                 py::arg("move"),
                 "play a stone on the board")
            .def("get_stone", [](const sente::Board<19>& board, unsigned x, unsigned y){
                return board.getSpace(x - 1, y - 1).getStone();
            }, "get the stone ")
            .def("__str__", [](const sente::Board<19>& board){
                return std::string(board);
            })
            .def("__eq__", &sente::Board<19>::operator==,
                "equality operator")
            .def("__ne__", [](const sente::Board<19>& us, const sente::Board<19>& other){
                return not (us == other);
            });

    py::class_<sente::Board<13>>(module, "Board13")
            .def(py::init<>())
            .def(py::init<std::array<std::array<sente::Stone, 13>, 13>>())
            .def("get_side", &sente::Board<13>::getSide,
                 "get the length of the side of the board")
            .def("play", &sente::Board<13>::playStone,
                 py::arg("move"),
                 "play a stone on the board")
            .def("get_stone", [](const sente::Board<13>& board, unsigned x, unsigned y){
                return board.getSpace(x - 1, y - 1).getStone();
            }, "get the stone ")
            .def("__str__", [](const sente::Board<13>& board){
                return std::string(board);
            })
            .def("__eq__", &sente::Board<13>::operator==,
                 "equality operator")
            .def("__ne__", [](const sente::Board<13>& us, const sente::Board<13>& other){
                return not (us == other);
            });

    py::class_<sente::Board<9>>(module, "Board9")
            .def(py::init<>())
            .def(py::init<std::array<std::array<sente::Stone, 9>, 9>>())
            .def("get_side", &sente::Board<9>::getSide,
                 "get the length of the side of the board")
            .def("play", &sente::Board<9>::playStone,
                 py::arg("move"),
                 "play a stone on the board")
            .def("get_stone", [](const sente::Board<9>& board, unsigned x, unsigned y){
                return board.getSpace(x - 1, y - 1).getStone();
            }, "get the stone ")
            .def("__str__", [](const sente::Board<9>& board){
                return std::string(board);
            })
            .def("__eq__", &sente::Board<9>::operator==,
                 "equality operator")
            .def("__ne__", [](const sente::Board<9>& us, const sente::Board<9>& other){
                return not (us == other);
            });

    py::class_<sente::GoGame>(module, "Game")
            .def(py::init<unsigned, sente::Rules, double>(),
                py::arg("board_size") = 19,
                py::arg("rules") = sente::Rules::CHINESE,
                py::arg("komi") = INFINITY,
                "initializes a go game with a specified board size and rules")
            .def("is_legal", [](const sente::GoGame& game, unsigned x, unsigned y){
                    return game.isLegal(x - 1, y - 1);
                },
                py::arg("x"),
                py::arg("y"),
                "checks to see if a move is legal")
            .def("is_legal", [](const sente::GoGame& game, unsigned x, unsigned y, sente::Stone stone){
                    return game.isLegal(x - 1, y - 1, stone);
                },
                py::arg("x"),
                py::arg("y"),
                py::arg("stone"),
                "checks to see if a move is legal")
            .def("is_legal", [](const sente::GoGame& game, const sente::Move& move){
                    return game.isLegal(move);
                },
                py::arg("move"),
                "checks to see if a move is legal")
            .def("is_legal", [](sente::GoGame& game, const py::object& obj){
                return obj.is_none();
            })
            .def("get_point", [](const sente::GoGame& game, unsigned x, unsigned y){
                    return game.getSpace(x - 1, y - 1);
                },
                py::arg("x"),
                py::arg("y"),
                "get move played at the specified position")
            .def("play", [](sente::GoGame& game, unsigned x, unsigned y){
                    game.playStone(x - 1, y - 1);
                },
                py::arg("x"),
                py::arg("y"),
                "plays a stone in the game and updates the board to remove any captured stones")
            .def("play", [](sente::GoGame& game, unsigned x, unsigned y, sente::Stone stone){
                    return game.playStone(x - 1, y - 1, stone);
                },
                py::arg("x"),
                py::arg("y"),
                py::arg("stone"),
                "plays a stone in the game and updates the board to remove any captured stones")
            .def("play", [](sente::GoGame& game, const sente::Move& move){
                    game.playStone(move);
                },
                py::arg("move"),
                "plays a stone in the game and updates the board to remove any captured stones")
            .def("play", [](sente::GoGame& game, const py::object& obj){
                if (obj.is_none()){
                    // pass if the object is none
                    game.playStone(sente::Move(game.getActivePlayer(), sente::PASS));
                }
                else {
                    throw std::domain_error("cannot play " + std::string(py::str(obj)));
                }
            })
            .def("pss", [](sente::GoGame& game){
                game.playStone(sente::Move(game.getActivePlayer(), sente::PASS));
            })
            .def("resign", [](sente::GoGame& game){
                game.playStone(sente::Move(game.getActivePlayer(), sente::RESIGN));
            })
            .def("get_results", &sente::GoGame::getResults,
                 "returns a results object for a game WITHOUT removing dead stones")
            .def("score", &sente::GoGame::score,
                 "scores the game WITHOUT removing dead stones")
            .def("get_winner", [](const sente::GoGame& game){
                    return game.getResults().winner();
                },
                "determines the winner of the game")
            .def("advance_to_root", &sente::GoGame::advanceToRoot,
                 "advance to the root node")
            .def("step_up", &sente::GoGame::stepUp,
                py::arg("steps") = 1,
                "step up the tree the specified number of steps")
            .def("get_branches", &sente::GoGame::getBranches,
                 "generates a list of the branches at the curren node of the game tree")
            .def("play_default_branch", &sente::GoGame::playDefaultBranch,
                 "plays out the moves in the default (first) branch of the tree")
            .def("play_moves", &sente::GoGame::playMoveSequence,
                 py::arg("moves"),
                 "plays all of the moves in a given list of moves")
            .def("get_legal_moves", &sente::GoGame::getLegalMoves,
                 "generates a list of all legal moves")
            .def("is_over", &sente::GoGame::isOver,
                 "determine if the game is over yet")
            .def("get_previous_moves", &sente::GoGame::getMoveSequence,
                 "get a list containing all of the moves on the current branch of the tree")
            .def("get_branches", &sente::GoGame::getBranches,
                 "get a list of the next moves that are currently in the move tree")
            .def("get_board", &sente::GoGame::getBoard,
                 py::return_value_policy::reference,
                 "Get the board that the game is being played on")
            .def("__str__", [](const sente::GoGame& game){
                return std::string(game);
            });

    auto sgf = module.def_submodule("sgf", "utilities for parsing SGF (Smart Game Format) files")
        .def("load", [](const std::string& fileName){

                std::string SGFText;

                try {
                    // load the text from the file
                    std::ifstream filePointer(fileName);
                    SGFText = std::string((std::istreambuf_iterator<char>(filePointer)),
                                           std::istreambuf_iterator<char>());
                }
                catch (const std::domain_error& E){
                    py::print("hit exception", E.what());
                    py::eval("raise FileNotFoundError(" + fileName + ")");
                }
                return sente::GoGame(SGFText);
            },
            py::arg("filename"),
            "Loads a go game from an SGF file")
        .def("dump", [](const sente::GoGame& game, const std::string& fileName, std::unordered_map<std::string, std::string> params){
                std::ofstream output(fileName);
                output << game.toSGF(params);
            },
             py::arg("game"),
             py::arg("file_name"),
             py::arg("params") = py::dict(),
             "saves a game as an SGF")
        .def("loads", [](const std::string& SGFText){
            return sente::GoGame(SGFText);
        })
        .def("dumps", [](const sente::GoGame& game, std::unordered_map<std::string, std::string> params){
            return game.toSGF(params);
        });

    auto utils = module.def_submodule("utils", "various utilities used by sente");

    py::register_exception<sente::utils::InvalidSGFException>(utils, "InvalidSGFException");
    py::register_exception<sente::utils::IllegalMoveException>(utils, "IllegalMoveException");

}
