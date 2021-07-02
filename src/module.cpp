/**
 *
 * Author: Arthur Wesley
 *
 */
#include <pybind11/pybind11.h>

#include "include/GoGame.h"

int add(int x, int y){
    return x + y;
}

namespace py = pybind11;

PYBIND11_MODULE(sente, module){

    // m.doc() = ""; // todo: add a better description

    module.def("add", &add, py::arg("x"), py::arg("y"));

    module.doc() = "Sente: an open source API for go";
    module.def("opposite_player", &sente::getOpponent, "get the opponent of a particular stone color");

    py::register_exception<sente::IllegalMoveException>(module, "IllegalMoveException");

    py::enum_<sente::Stone>(module, "Stone")
            .value("BLACK", sente::BLACK)
            .value("WHITE", sente::WHITE)
            .value("EMPTY", sente::EMPTY)
            .export_values();

    py::enum_<sente::Rules>(module, "Rules")
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
            .def("__str__", [](const sente::Move& move){
                return std::string(move);
            });


    py::class_<sente::Board<19>>(module, "Board19")
            .def(py::init<>())
            .def("get_side", &sente::Board<19>::getSide,
                 "get the length of the side of the board")
            .def("play", &sente::Board<19>::playStone,
                 py::arg("move"),
                 "play a stone on the board")
            .def("get_stone", [](const sente::Board<19>& board, unsigned x, unsigned y){
                return board.getSpace(x, y).getStone();
            }, "get the stone ")
            .def("__str__", [](const sente::Board<19>& board){
                return std::string(board);
            });

    py::class_<sente::Board<13>>(module, "Board13")
            .def(py::init<>())
            .def("get_side", &sente::Board<13>::getSide,
                 "get the length of the side of the board")
            .def("play", &sente::Board<13>::playStone,
                 py::arg("move"),
                 "play a stone on the board")
            .def("get_stone", [](const sente::Board<13>& board, unsigned x, unsigned y){
                return board.getSpace(x, y).getStone();
            }, "get the stone ")
            .def("__str__", [](const sente::Board<13>& board){
                return std::string(board);
            });

    py::class_<sente::Board<9>>(module, "Board9")
            .def(py::init<>())
            .def("get_side", &sente::Board<9>::getSide,
                 "get the length of the side of the board")
            .def("play", &sente::Board<9>::playStone,
                 py::arg("move"),
                 "play a stone on the board")
            .def("get_stone", [](const sente::Board<9>& board, unsigned x, unsigned y){
                return board.getSpace(x, y).getStone();
            }, "get the stone ")
            .def("__str__", [](const sente::Board<9>& board){
                return std::string(board);
            });

    py::class_<sente::GoGame>(module, "GoGame")
            .def(py::init<sente::Rules, unsigned>(),
                py::arg("rules") = sente::Rules::CHINESE,
                py::arg("board_size") = 19)
            .def(py::init<std::string>(),
                py::arg("sgf_file_path"),
                "initializes a game of go from an sgf file")
            .def("is_legal", &sente::GoGame::isLegal,
                 py::arg("x"),
                 py::arg("y"),
                 "checks to see if a move is legal")
            .def("is_legal", &sente::GoGame::isLegal2,
                 py::arg("x"),
                 py::arg("y"),
                 py::arg("stone"),
                 "checks to see if a move is legal")
            .def("is_legal", &sente::GoGame::isLegal3,
                 py::arg("move"),
                 "checks to see if a move is legal")
            .def("is_legal", [](sente::GoGame& game, const py::object& obj){
                return obj.is_none();
            })
            .def("get_point", &sente::GoGame::getSpace,
                 py::arg("x"),
                 py::arg("y"),
                 "get move played at the specified position")
            .def("play", &sente::GoGame::playStone,
                 py::arg("x"),
                 py::arg("y"),
                 "plays a stone in the game and updates the board to remove any captured stones")
            .def("play", &sente::GoGame::playStone2,
                 py::arg("x"),
                 py::arg("y"),
                 py::arg("stone"),
                 "plays a stone in the game and updates the board to remove any captured stones")
            .def("play", &sente::GoGame::playStone3,
                 py::arg("move"),
                 "plays a stone in the game and updates the board to remove any captured stones")
            .def("play", [](sente::GoGame& game, const py::object& obj){
                 if (obj.is_none()){
                     // pass if the object is none
                     game.playStone3(sente::Move(game.getActivePlayer(), sente::PASS));
                 }
                 else {
                     throw std::domain_error("invalid python object");
                 }
             })
             .def("play_pass", [](sente::GoGame& game){
                 game.playStone3(sente::Move(game.getActivePlayer(), sente::PASS));
             })
             .def("play_resign", [](sente::GoGame& game){
                 game.playStone3(sente::Move(game.getActivePlayer(), sente::RESIGN));
             })
             .def("__str__", [](const sente::GoGame& game){
                 return std::string(game);
             });

}
