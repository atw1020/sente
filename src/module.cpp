/**
 *
 * Author: Arthur Wesley
 *
 */
#include <fstream>

#include <pybind11/stl.h>
#include <pybind11/pybind11.h>

#include "Utils/SGF/SGF.h"
#include "Game/GoGame.h"
#include "Utils/Numpy.h"
#include "Utils/SenteExceptions.h"
#include "Utils/GTP/Engine.h"

namespace py = pybind11;

PYBIND11_MODULE(sente, module){

    module.doc() = R"pbdoc(
        Sente library
        =============

        .. currentmodule:: sente

        Documentation for the Contents of the ``sente`` library

        .. toctree::
            :maxdepth: 2
            :caption: Basic Components

            objects/stone
            objects/rules
            objects/results
            objects/Move
            objects/Boards
            objects/Game

    )pbdoc";

    module.def("opposite_player", &sente::getOpponent, "get the opponent of a particular stone color");


    py::enum_<sente::Stone>(module, "stone", R"pbdoc(
            An enumeration for a Go stone.

            .. code-block:: python

                >>> stone = sente.stone.BLACK
                >>> print(stone)
                stone.BLACK

        )pbdoc")
        .value("BLACK", sente::BLACK, R"pbdoc(
            Represents a Black Go Stone.
        )pbdoc")
        .value("WHITE", sente::WHITE, R"pbdoc(
            Represents a White Go Stone.
        )pbdoc")
        .value("EMPTY", sente::EMPTY, R"pbdoc(
            Represents an empty point on the grid.
        )pbdoc")
        .export_values();

    py::enum_<sente::Rules>(module, "rules", R"pbdoc(
            An enumeration for a Go Rules Set.

            .. code-block:: python

                >>> rules = sente.rules.CHINESE
                >>> print(rules)
                rules.CHINESE

        )pbdoc")
        .value("CHINESE", sente::Rules::CHINESE, R"pbdoc(
            The `Chinese rules <https://senseis.xmp.net/?ChineseRules>`_ for go.
        )pbdoc")
        .value("JAPANESE", sente::Rules::JAPANESE, R"pbdoc(
            The `Japanese rules <https://senseis.xmp.net/?JapaneseRules>`_ for go.
        )pbdoc")
            .value("KOREAN", sente::Rules::KOREAN, R"pbdoc(
            The `Korean rules <https://senseis.xmp.net/?KoreanRules>`_ for go.
        )pbdoc")
        .export_values();

    py::class_<sente::Move>(module, "Move", R"pbdoc(
            A class that represents a move that can be played on a go board, consisting of a position and a stone.

            :members:
        )pbdoc")
        .def(py::init<unsigned, unsigned, sente::Stone>(),
                py::arg("x"),
                py::arg("y"),
                py::arg("stone"))
        .def("get_x", &sente::Move::getX, R"pbdoc(
            get the x-coordinate of the move (internal indices)

            :return: x-coordinate of the move
        )pbdoc")
        .def("get_y", &sente::Move::getY, R"pbdoc(
            get the y-coordinate of the move (internal indices) AFWEIJOIEJFUIF

            :return: y-coordinate of the move
        )pbdoc")
        .def("get_stone", &sente::Move::getStone, R"pbdoc(
            get the stone that the player will place on the board

            :return: :ref:`sente.stone <stone>` object that the move contains
        )pbdoc")
        .def("__eq__", &sente::Move::operator==)
        .def("__ne__", &sente::Move::operator!=)
        .def("__repr__", [](const sente::Move& move){
            return "<sente.Move " + std::string(move) + ">";
        });

    py::class_<sente::Results>(module, "results")
        .def("get_winner", &sente::Results::winner,
            R"pbdoc(
                gets the winner of the game

                :return: :ref:`sente.stone <stone>` of the winner of the game
            )pbdoc")
        .def("get_black_points", [](const sente::Results& results){
                return results.blackScore;
            },
            R"pbdoc(

                gets the number of points that black scored

                :return: the number of points that black scored
            )pbdoc")
        .def("get_white_points", [](const sente::Results& results){
                return results.whiteScore + results.komi;
            },
            R"pbdoc(

              gets the number of points that white scored

              :return: the number of points that black scored
            )pbdoc")
        .def("__repr__", [](const sente::Results& results){
                return "<sente.result \"" + std::string(results) + "\">";
            });

    py::class_<sente::Board<19>>(module, "Board19")
        .def(py::init<>())
        .def(py::init<std::array<std::array<sente::Stone, 19>, 19>>())
        .def("get_side", &sente::Board<19>::getSide,
            R"pbdoc(
                get the length of the side of the board

                :return: the length of the side of the board (19)
            )pbdoc")
        .def("play", &sente::Board<19>::playStone,
            py::arg("move"),
            R"pbdoc(
                play a stone on the board

                :param move: the move object to play
            )pbdoc")
        .def("get_stone", [](const sente::Board<19>& board, unsigned x, unsigned y){
                return board.getSpace(x - 1, y - 1).getStone();
            }, R"pbdoc(
                get the stone located on the specified point.

                :param x: The x co-ordinate to get the stone for.
                :param y: The y co-ordinate to get the stone for.
                :return: the stone located at specified point
            )pbdoc")
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
            R"pbdoc(
                get the length of the side of the board

                :return: the length of the side of the board (13)
            )pbdoc")
        .def("play", &sente::Board<13>::playStone,
            py::arg("move"),
            R"pbdoc(
                play a stone on the board

                :param move: the move object to play
            )pbdoc")
        .def("get_stone", [](const sente::Board<13>& board, unsigned x, unsigned y){
                return board.getSpace(x - 1, y - 1).getStone();
            },
            R"pbdoc(
                get the stone located on the specified point.

                :param x: The x co-ordinate to get the stone for.
                :param y: The y co-ordinate to get the stone for.
                :return: the stone located at specified point
            )pbdoc")
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
            R"pbdoc(
                get the length of the side of the board

                :return: the length of the side of the board (9)
            )pbdoc")
        .def("play", &sente::Board<9>::playStone,
            py::arg("move"),
            R"pbdoc(
                play a stone on the board

                :param move: the move object to play
            )pbdoc")
        .def("get_stone", [](const sente::Board<9>& board, unsigned x, unsigned y){
                return board.getSpace(x - 1, y - 1).getStone();
            },
            R"pbdoc(
                get the stone located on the specified point.

                :param x: The x co-ordinate to get the stone for.
                :param y: The y co-ordinate to get the stone for.
                :return: the stone located at specified point
            )pbdoc")
        .def("__str__", [](const sente::Board<9>& board){
                return std::string(board);
            })
        .def("__eq__", &sente::Board<9>::operator==,
             "equality operator")
        .def("__ne__", [](const sente::Board<9>& us, const sente::Board<9>& other){
                return not (us == other);
            });

    py::class_<sente::GoGame>(module, "Game", R"pbdoc(

            The Sente Game object.

            The ``sente.Game`` Object differs from the ``sente.Board`` object in that it accounts for the rules of Go and is capable of capturing stones and deeming ko moves invalid.
            For more on the difference between ``sente.Game`` and ``sente.Board`` see :ref:`Boards vs Games`.

        )pbdoc")
        .def(py::init<unsigned, sente::Rules, double>(),
            py::arg("board_size") = 19,
            py::arg("rules") = sente::Rules::CHINESE,
            py::arg("komi") = INFINITY,
            R"pbdoc(
                initializes a go game with a specified board size and rules

                :param: board_size
                :param: rules to play the game by
                :param: komi of the game
            )pbdoc")
        .def("get_active_player", &sente::GoGame::getActivePlayer,
            R"pbdoc(
                get the stone color of the active player (the player whose turn it is right now).

                :return: the stone color of the active player.
            )pbdoc")
        .def("is_legal", [](sente::GoGame& game, unsigned x, unsigned y){
                return game.isLegal(x - 1, y - 1);
            },
            py::arg("x"),
            py::arg("y"),
            R"pbdoc(
                Checks to see if a move is legal.

                Sente checks five conditions to see if a move is illegal

                #. Are the coordinates of the move located on the board?
                #. Does the move lie on an occupied point?
                #. Is it the person playing the stone's turn?
                #. Does the move result in self-capture?
                #. Is the move illegal because of a Ko?

                :param x: The x co-ordinate of the move.
                :param y: The y co-ordinate of the move.
                :return: whether or not the move satisfies the above conditions.
            )pbdoc")
        .def("is_legal", [](sente::GoGame& game, unsigned x, unsigned y, sente::Stone stone){
                return game.isLegal(x - 1, y - 1, stone);
            },
            py::arg("x"),
            py::arg("y"),
            py::arg("stone"),
            R"pbdoc(
                Checks to see if a move is legal.

                Sente checks five conditions to see if a move is illegal (see above).

                :param x: The x co-ordinate of the move.
                :param y: The y co-ordinate of the move.
                :param stone: The color of the player making the move.
                :return: whether or not the move satisfies the above conditions.
            )pbdoc")
        .def("is_legal", [](sente::GoGame& game, const sente::Move& move){
                return game.isLegal(move);
            },
            py::arg("move"),
            R"pbdoc(
                Checks to see if a move is legal.

                Sente checks five conditions to see if a move is illegal (see above).

                :param move: A move object to play
                :return: whether or not the move satisfies the above conditions.
            )pbdoc")
        .def("is_legal", [](sente::GoGame& game, const py::object& obj){
            return obj.is_none();
        },
            R"pbdoc(
                An overloaded extension of the ``is_legal`` method accepts ``None`` as an argument.
                Using ``game.play(None)`` is interpreted as passing, and this method ensures that such a move is legal.

                :param move: A move object to play
                :return: whether or not the move satisfies the above conditions.
            )pbdoc")
        .def("get_point", [](const sente::GoGame& game, unsigned x, unsigned y){
                return game.getSpace(x - 1, y - 1);
            },
            py::arg("x"),
            py::arg("y"),
             R"pbdoc(
                get move played at the specified position.

                :param x: x co-ordinate of the point to locate.
                :param y: y co-ordinate of the point to locate.
                :return: a :ref:`sente.stone <stone>` object representing the specified point
            )pbdoc")
        .def("play", [](sente::GoGame& game, unsigned x, unsigned y){
                game.playStone(x - 1, y - 1);
            },
            py::arg("x"),
            py::arg("y"),
            R"pbdoc(

                Plays a stone on the board at the specified location and Captures and stones

                :param x: The x co-ordinate of the move to play.
                :param y: The y co-ordinate of the move to play:
                :raises IllegalMoveException: If the move is illegal. (see ``Game.is_legal``)
            )pbdoc")
        .def("play", [](sente::GoGame& game, unsigned x, unsigned y, sente::Stone stone){
                return game.playStone(x - 1, y - 1, stone);
            },
            py::arg("x"),
            py::arg("y"),
            py::arg("stone"),
            R"pbdoc(

                Plays a stone on the board at the specified location and Captures and stones

                :param x: The x co-ordinate of the move to play.
                :param y: The y co-ordinate of the move to play:
                :param stones: The color of the stone to play.
                :raises IllegalMoveException: If the move is illegal. (see ``Game.is_legal``)

            )pbdoc")
        .def("play", [](sente::GoGame& game, const sente::Move& move){
                game.playStone(move);
            },
            py::arg("move"),
            R"pbdoc(

                Plays a stone on the board at the specified location and Captures and stones

                :param move: The Move object to play
                :raises IllegalMoveException: If the move is illegal. (see ``Game.is_legal``)

            )pbdoc")
        .def("play", [](sente::GoGame& game, const py::object& obj){
                if (obj.is_none()){
                    // pass if the object is none
                    game.playStone(sente::Move::pass(game.getActivePlayer()));
                }
                else {
                    throw std::domain_error("cannot play " + std::string(py::str(obj)));
                }
            },
            R"pbdoc(

                Plays a stone on the board at the specified location and Captures and stones

                :param move: The Move object to play
                :raises IllegalMoveException: If the move is illegal. (see ``Game.is_legal``)
                :raises ValueError: If a valid Move object is not passed

            )pbdoc")
        .def("pss", [](sente::GoGame& game){
            game.playStone(sente::Move::pass(game.getActivePlayer()));
        },
        R"pbdoc(
            causes the current active player to pass.
        )pbdoc")
        .def("resign", [](sente::GoGame& game){
            game.playStone(sente::Move::resign(game.getActivePlayer()));
        },
        R"pbdoc(
            causes the current active player to resign.
        )pbdoc")
        .def("get_results", &sente::GoGame::getResults,
            R"pbdoc(
                returns a results object for a game.

                .. Warning:: This method does not remove dead stones

                :return: :ref:`sente.results <results>` object.

            )pbdoc")
        .def("score", &sente::GoGame::score,
            R"pbdoc(
                scores a game and returns the results.

                .. Warning:: This method does not remove dead stones

                :return: :ref:`sente.results <results>` object.

            )pbdoc")
        .def("get_winner", [](const sente::GoGame& game){
                return game.getResults().winner();
            },
            R"pbdoc(
                determines the winner of the game.

                .. Warning:: This method does not remove dead stones.

                :return: :ref:`sente.stone <stone>` of the winner of the game.

            )pbdoc")
        .def("is_at_root", &sente::GoGame::isAtRoot,
            R"pbdoc(
                Determine if the board is currently at the root of the tree.

                :return: whether or not the board is at the root of the tree.
            )pbdoc")
        .def("advance_to_root", &sente::GoGame::advanceToRoot,
            R"pbdoc(
                Advance the board tree position to the root of the tree (ie. an empty board).
            )pbdoc")
        .def("step_up", &sente::GoGame::stepUp,
            py::arg("steps") = 1,
            R"pbdoc(

                step up the tree the specified number of steps.
                ie. undo the specified number of moves

                :param steps: the number to steps to step up
            )pbdoc")
        .def("get_branches", &sente::GoGame::getBranches,
            R"pbdoc(

                generates a list of the branches at the current node of the game tree.

                :return: list of branches at the current node of the tree
            )pbdoc")
        .def("get_default_sequence", &sente::GoGame::getDefaultSequence,
            R"pbdoc(
                generates a list of the moves in the default branch.

                :return: the sequence of moves that leads to the current board position.
            )pbdoc")
        .def("get_current_sequence", &sente::GoGame::getMoveSequence,
             R"pbdoc(
                generate the sequence of moves that leads to the current board position

                :return: a python list containing the moves that lead to this position.
             )pbdoc")
        .def("get_all_sequences", [](sente::GoGame& game){
                return game.getSequences({});
            },
             R"pbdoc(
                generates a list of all variations currently in the game

                the "default" sequences is the first element in this list

                :return: a list of lists of moves where each move is the move sequence.
             )pbdoc")
        .def("play_sequence", &sente::GoGame::playMoveSequence,
                 py::arg("moves"),
                 R"pbdoc(
                plays all of the moves in a given list of moves

                :param moves: a list of move objects to play
                :raises IllegalMoveException: If any move in the sequence is illegal
            )pbdoc")
        .def("play_default_sequence", &sente::GoGame::playDefaultSequence,
            R"pbdoc(
                plays out the moves in the default (first) branch of the tree
            )pbdoc")
        .def("get_legal_moves", &sente::GoGame::getLegalMoves, py::return_value_policy::reference_internal,
            R"pbdoc(
                generates a list of all legal moves

                :return: list of legal moves on the current board
            )pbdoc")
        .def("is_over", &sente::GoGame::isOver,
             R"pbdoc(
                determine if the game is over yet

                :return: whether or not the game has ended
            )pbdoc")
        .def("get_board", &sente::GoGame::getBoard,
             py::return_value_policy::reference,
             R"pbdoc(
                Get the board object that the game is updating internally.

                :return: a ``sente.Board`` object that represents the board to be played.
            )pbdoc")
        .def("numpy", &sente::utils::getFeatures)
        .def("numpy", [](const sente::GoGame& game){
            return sente::utils::getFeatures(game, {"Black Stones", "White Stones", "Empty Points", "Ko Points"});
        })
        .def("get_properties", [](const sente::GoGame& game) -> py::dict{
                py::dict response;
                std::unordered_map<std::string, std::vector<std::string>> metadata = game.getProperties();

                for (const auto& item : metadata){
                    if (item.second.size() == 1){
                        response[py::str(item.first)] = item.second[0];
                    }
                    else {
                        response[py::str(item.first)] = item.second;
                    }
                }

                return response;

            },
            R"pbdoc(
                Get all of the properties from the SGF file.

                :return: a python dictionary that maps from metadata parameters (ie. SZ[], FF[]) to their values
            )pbdoc")
        .def("set_property", [](sente::GoGame& game, const std::string& property, double value){
                game.setProperty(property, std::to_string(value));
            }, R"pbdoc(
                Adds the specified property to the game

                :param property: SGF property to set the value of
                :param value: value to set the metadata to
                :return: None
            )pbdoc")
        .def("set_property", [](sente::GoGame& game, const std::string& property, const std::string& value){
                game.setProperty(property, value);
            }, R"pbdoc(
                Adds the specified property to the game

                :param property: SGF property to set the value of
                :param value: value to set the metadata to
                :return: None
            )pbdoc")
        .def("set_property", [](sente::GoGame& game, const std::string& property, const std::vector<std::string>& values){
                game.setProperty(property, values);
            }, R"pbdoc(
                Adds the specified property to the game

                :param property: SGF property to set the value of
                :param value: value to set the metadata to
                :return: None
            )pbdoc")
        .def_property("comment", &sente::GoGame::getComment, &sente::GoGame::setComment,
            R"pbdoc(
                The comment associated with the given node
            )pbdoc")
        .def("__str__", [](const sente::GoGame& game){
            return std::string(game);
        });

    auto sgf = module.def_submodule("sgf", "utilities for parsing SGF (Smart Game Format) files")
        .def("load", [](const std::string& fileName, bool disableWarnings,
                                                     bool ignoreIllegalProperties,
                                                     bool fixFileFormat) -> sente::GoGame {

                std::string SGFText;

                // load the text from the file
                std::ifstream filePointer(fileName);

                if (not filePointer.good()){
                    throw sente::utils::FileNotFoundException(fileName);
                }
                else {
                    SGFText = std::string((std::istreambuf_iterator<char>(filePointer)),
                                          std::istreambuf_iterator<char>());
                    auto tree = sente::sgf::loadSGF(SGFText, disableWarnings, ignoreIllegalProperties, fixFileFormat);
                    return sente::GoGame(tree);
                }
            },
            py::arg("filename"),
            py::arg("disable_warnings") = false,
            py::arg("ignore_illegal_properties") = true,
            py::arg("fix_file_format") = true,
            R"pbdoc(
                Loads a go game from an SGF file.

                :param filename: the name of the file
                :param disable_warnings: whether to ignore warnings when loading an illegal SGF file
                :param ignore_illegal_properties: whether or not to ignore illegal SGF properties
                :param fix_file_format: whether or not to fix the file format if it is wrong
                :return: a ``sente.Game`` object populated with data from the SGF file
            )pbdoc")
        .def("dump", [](const sente::GoGame& game, const std::string& fileName){
                std::ofstream output(fileName);
                output << sente::sgf::dumpSGF(game);
            },
             py::arg("game"),
             py::arg("file_name"),
             "saves a game as an SGF")
        .def("loads", [](const std::string& SGFText, bool disableWarnings,
                                                     bool ignoreIllegalProperties,
                                                     bool fixFileFormat) -> sente::GoGame {
                auto tree = sente::sgf::loadSGF(SGFText, disableWarnings, ignoreIllegalProperties, fixFileFormat);
                return sente::GoGame(tree);
            },
            py::arg("sgf_text"),
            py::arg("disable_warnings") = false,
            py::arg("ignore_illegal_properties") = true,
            py::arg("fix_file_format") = true,
            R"pbdoc(
                Loads a go game from an SGF file.

                :param sgf_text: the text of the SGF file to read from
                :param disable_warnings: whether to ignore warnings when loading an illegal SGF file
                :param ignore_illegal_properties: whether or not to ignore illegal SGF properties
                :param fix_file_format: whether or not to fix the file format if it is wrong
                :return: a ``sente.Game`` object populated with data from the SGF file
            )pbdoc")
        .def("dumps", [](const sente::GoGame& game){
                return sente::sgf::dumpSGF(game);
            },
            py::arg("game"),
            "Serialize a string as an SGF");

    auto exceptions = module.def_submodule("exceptions", "various exceptions used by sente");

    py::register_exception<sente::utils::InvalidSGFException>(exceptions, "InvalidSGFException");
    py::register_exception<sente::utils::IllegalMoveException>(exceptions, "IllegalMoveException");

#if PY_MAJOR_VERSION == 3
    py::register_exception<sente::utils::FileNotFoundException>(exceptions, "FileNotFoundError", PyExc_FileNotFoundError);
#else
    py::register_exception<sente::utils::FileNotFoundException>(exceptions, "IOError", PyExc_IOError);
#endif

    auto gtp = module.def_submodule("gtp", "utilities for implementing the go text protocol (GTP)");
    py::class_<sente::GTP::Engine>(gtp, "Engine")
            .def(py::init<std::string, std::string>(),
                    py::arg("engine_version") = "Engine using Sente GTP",
                    py::arg("engine_version") = "0.4.0")
            .def("interpret", &sente::GTP::Engine::interpret)
            .def("active", [](sente::GTP::Engine& engine){
                return engine.active;
            });

}
