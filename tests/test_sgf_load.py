"""

Author: Arthur Wesley

"""

import os
from pathlib import Path
from unittest import TestCase

import sente.exceptions
from sente import sgf

from assert_does_not_raise import DoesNotRaiseTestCase


class BasicSGF(DoesNotRaiseTestCase):

    def test_all_sgfs(self):
        """

        tests to see if any of the sgf files in the directory raise any exceptions

        :return:
        """

        files = os.listdir("sgf")

        # sgf.load(os.path.join("sgf", "extra letter in move.sgf"))

        for file in files:
            game = sgf.load(str(Path("sgf")/file))
            game.play_default_sequence()

            with self.assertDoesNotRaise(Exception):
                game = sgf.load(str(Path("sgf")/file))
                game.play_default_sequence()

    def test_punctuation_ignored_inside_parens(self):
        """

        makes sure that punctuation is ignored inside parens

        """

        game = sgf.load("sgf/punctuation ignored inside parentheses.sgf")
        game.play_default_sequence()

        print(game)

    def test_simple_single_branch_file(self):
        """

        tests to see if we can import a sgf file with a single branch

        :return:
        """

        B = sente.stone.BLACK
        W = sente.stone.WHITE
        _ = sente.stone.EMPTY

        game = sgf.load("sgf/simple sequence.sgf")
        game.play_default_sequence()

        expected_game = sente.Board19([[_, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _],
                                 [_, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _],
                                 [_, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _],
                                 [_, _, _, B, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _],
                                 [_, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _],
                                 [_, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _],
                                 [_, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _],
                                 [_, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _],
                                 [_, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _],
                                 [_, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _],
                                 [_, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _],
                                 [_, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _],
                                 [_, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _],
                                 [_, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _],
                                 [_, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _],
                                 [_, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _],
                                 [_, _, _, B, _, _, _, _, _, _, _, _, _, _, _, _, B, W, W],
                                 [_, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, W, _, _],
                                 [_, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, W, W]])

        self.assertEqual(expected_game, game.get_board())

    def test_full_single_branch_game(self):
        """

        tests to see if the module can keep track of an entire game

        :return:
        """

        B = sente.stone.BLACK
        W = sente.stone.WHITE
        _ = sente.stone.EMPTY

        game = sgf.load("sgf/34839594-255-IDW64-noob_bot_3.sgf")
        game.play_default_sequence()

        expected_game = sente.Board19([[_, _, _, _, _, _, B, W, W, W, W, W, W, B, _, _, _, _, _],
                                 [_, _, _, _, W, B, B, B, W, B, B, B, W, B, _, _, _, _, _],
                                 [_, _, W, _, W, W, B, B, B, _, B, B, W, B, B, _, W, _, _],
                                 [_, _, _, W, _, _, W, B, B, _, B, W, W, W, B, B, W, _, _],
                                 [W, W, W, B, W, W, W, W, B, W, W, _, W, W, B, _, _, _, _],
                                 [B, B, B, B, B, W, B, B, W, _, W, _, _, _, W, W, _, _, _],
                                 [_, _, _, _, B, W, B, B, W, _, W, W, W, W, _, W, _, _, _],
                                 [W, W, W, W, W, B, B, W, W, _, _, _, W, _, W, W, B, _, _],
                                 [B, B, B, W, B, B, B, B, W, _, _, W, B, B, _, _, _, _, _],
                                 [_, _, B, W, W, B, _, B, W, _, _, W, _, _, _, W, W, W, _],
                                 [_, _, B, B, W, B, _, W, _, _, _, W, _, B, B, W, B, W, W],
                                 [W, W, B, B, W, B, _, W, _, _, W, B, B, B, W, B, B, B, W],
                                 [B, W, W, B, W, B, B, B, W, _, _, _, _, B, W, _, B, B, W],
                                 [B, _, _, W, W, B, B, B, W, _, _, W, W, W, W, _, _, _, B],
                                 [B, B, B, B, B, W, W, W, _, _, W, W, B, W, _, W, B, B, B],
                                 [W, W, W, W, W, W, B, B, W, W, B, B, B, B, B, W, W, W, W],
                                 [B, B, B, W, B, _, _, B, B, B, _, B, _, B, _, W, B, _, _],
                                 [B, B, _, W, B, B, B, _, _, _, B, W, B, _, _, W, B, B, B],
                                 [_, B, _, W, W, B, _, W, W, _, W, W, W, W, _, W, B, B, _]])

        self.assertEqual(str(expected_game), str(game))
        self.assertEqual(expected_game, game.get_board())


class StringLoad(DoesNotRaiseTestCase):
    """

    tests to see if SGFs can be loaded from strings and not just from files

    """

    def test_all_sgfs(self):
        """

        tests to see if any of the sgf files in the directory raise any exceptions

        :return:
        """

        files = os.listdir("sgf")

        # sgf.load(os.path.join("sgf", "extra letter in move.sgf"))

        for file in files:
            with self.assertDoesNotRaise(Exception):
                with open(str(Path("sgf")/file), encoding="utf-8") as sgf_file:
                    game = sgf.loads(sgf_file.read())
                    game.play_default_sequence()

    def test_simple_single_branch_file(self):
        """

        tests to see if we can import a sgf file with a single branch

        :return:
        """

        B = sente.stone.BLACK
        W = sente.stone.WHITE
        _ = sente.stone.EMPTY

        with open("sgf/simple sequence.sgf") as sgf_file:
            game = sgf.loads(sgf_file.read())
        game.play_default_sequence()

        expected_game = sente.Board19([[_, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _],
                                 [_, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _],
                                 [_, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _],
                                 [_, _, _, B, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _],
                                 [_, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _],
                                 [_, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _],
                                 [_, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _],
                                 [_, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _],
                                 [_, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _],
                                 [_, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _],
                                 [_, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _],
                                 [_, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _],
                                 [_, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _],
                                 [_, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _],
                                 [_, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _],
                                 [_, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _],
                                 [_, _, _, B, _, _, _, _, _, _, _, _, _, _, _, _, B, W, W],
                                 [_, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, W, _, _],
                                 [_, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, W, W]])

        self.assertEqual(expected_game, game.get_board())

    def test_full_single_branch_game(self):
        """

        tests to see if the module can keep track of an entire game

        :return:
        """

        B = sente.stone.BLACK
        W = sente.stone.WHITE
        _ = sente.stone.EMPTY

        with open("sgf/34839594-255-IDW64-noob_bot_3.sgf") as sgf_file:
            game = sgf.loads(sgf_file.read())
        game.play_default_sequence()

        expected_game = sente.Board19([[_, _, _, _, _, _, B, W, W, W, W, W, W, B, _, _, _, _, _],
                                 [_, _, _, _, W, B, B, B, W, B, B, B, W, B, _, _, _, _, _],
                                 [_, _, W, _, W, W, B, B, B, _, B, B, W, B, B, _, W, _, _],
                                 [_, _, _, W, _, _, W, B, B, _, B, W, W, W, B, B, W, _, _],
                                 [W, W, W, B, W, W, W, W, B, W, W, _, W, W, B, _, _, _, _],
                                 [B, B, B, B, B, W, B, B, W, _, W, _, _, _, W, W, _, _, _],
                                 [_, _, _, _, B, W, B, B, W, _, W, W, W, W, _, W, _, _, _],
                                 [W, W, W, W, W, B, B, W, W, _, _, _, W, _, W, W, B, _, _],
                                 [B, B, B, W, B, B, B, B, W, _, _, W, B, B, _, _, _, _, _],
                                 [_, _, B, W, W, B, _, B, W, _, _, W, _, _, _, W, W, W, _],
                                 [_, _, B, B, W, B, _, W, _, _, _, W, _, B, B, W, B, W, W],
                                 [W, W, B, B, W, B, _, W, _, _, W, B, B, B, W, B, B, B, W],
                                 [B, W, W, B, W, B, B, B, W, _, _, _, _, B, W, _, B, B, W],
                                 [B, _, _, W, W, B, B, B, W, _, _, W, W, W, W, _, _, _, B],
                                 [B, B, B, B, B, W, W, W, _, _, W, W, B, W, _, W, B, B, B],
                                 [W, W, W, W, W, W, B, B, W, W, B, B, B, B, B, W, W, W, W],
                                 [B, B, B, W, B, _, _, B, B, B, _, B, _, B, _, W, B, _, _],
                                 [B, B, _, W, B, B, B, _, _, _, B, W, B, _, _, W, B, B, B],
                                 [_, B, _, W, W, B, _, W, W, _, W, W, W, W, _, W, B, B, _]])

        self.assertEqual(str(expected_game), str(game))
        self.assertEqual(expected_game, game.get_board())

    def test_non_standard_komi(self):
        """

        tests to see if we can extract the komi from a game

        :return:
        """

        game = sgf.load("sgf/0.5 Komi.sgf")

        game.pss()
        game.pss()

        self.assertEqual(game.score().get_white_points(), 0.5)

    def test_semicolon_inside_brackets(self):
        """

        tests to see if semicolons inside a SGF's text field are ignored

        :return:
        """

        game = sgf.load("sgf/commented semicolon.sgf")

        with self.assertDoesNotRaise(sente.exceptions.InvalidSGFException):
            game.play_default_sequence()


class BranchedSGF(TestCase):

    def test_simple_branched_sgf(self):
        """

        tests to see if the SGF reader works for simple files

        :return:
        """

        game = sgf.load("sgf/simple fork.sgf")

        self.assertIn(sente.Move(3, 15, sente.stone.BLACK), game.get_branches())
        self.assertIn(sente.Move(3, 16, sente.stone.BLACK), game.get_branches())

    def test_long_branched_sgf(self):
        """

        tests to see if the SGF reader works for longer SGF files

        :return:
        """

        game = sgf.load("sgf/two josekis.sgf")

        self.assertEqual([sente.Move(3, 16, sente.stone.BLACK)], game.get_branches())
        game.play(4, 17)

        self.assertEqual([sente.Move(2, 14, sente.stone.WHITE), sente.Move(3, 14, sente.stone.WHITE)], game.get_branches())

    def test_complex_branched_sgf(self):
        """

        tests to see if the SGF parser can handle an SGF file with multiple branches

        :return:
        """

        game = sgf.load("sgf/3-4.sgf")  # a 3-4 joseki refrence with lots of branches

        # play all of the sequences
        game.play_default_sequence()
        game.advance_to_root()

        self.assertEqual([sente.Move(3, 16, sente.stone.BLACK)], game.get_branches())
        game.play(4, 17)

        self.assertEqual([sente.Move(3, 14, sente.stone.WHITE), sente.Move(2, 14, sente.stone.WHITE)], game.get_branches())


class InvalidSGF(TestCase):

    def test_all_invalid_sgf(self):
        """

        tests to see if all of the files in the invalid SGF directory raise exceptions

        :return:
        """

        files = os.listdir("invalid sgf")

        for file in files:
            with self.assertRaises(sente.exceptions.InvalidSGFException):
                game = sgf.load(str(Path("invalid sgf")/file))

    def test_invalid_moves(self):
        """

        tests to see if moves that have invalid formatting throw an exception

        :return:
        """

        with self.assertRaises(sente.exceptions.InvalidSGFException):
            sgf.load("invalid sgf/extra letter in move.sgf")
        with self.assertRaises(sente.exceptions.InvalidSGFException):
            sgf.load("invalid sgf/extra square bracket.sgf")
        with self.assertRaises(sente.exceptions.InvalidSGFException):
            sgf.load("invalid sgf/missing letter in move.sgf")
        with self.assertRaises(sente.exceptions.InvalidSGFException):
            sgf.load("invalid sgf/missing square bracket.sgf")

    def test_incorrect_parentheses(self):
        """

        tests to see if the

        :return:
        """

        with self.assertRaises(sente.exceptions.InvalidSGFException):
            sgf.load("invalid sgf/branched missing 2 parens.sgf")

    def test_nonexistent_file(self):
        """

        tests to see if if we raises a fileNotFound Exception

        :return:
        """

        with self.assertRaises(FileNotFoundError):
            sgf.load("invalid sgf/potato.sgf")

    def test_non_sgf_file(self):
        """

        tests to see if an exception is thrown with a non SGF file

        :return:
        """

        with self.assertRaises(sente.exceptions.InvalidSGFException):
            sgf.load("invalid sgf/module.cpp")

    def test_binary_file(self):
        """

        tests to see if an exception is thrown with a binary file

        :return:
        """

        with self.assertRaises(sente.exceptions.InvalidSGFException):
            # raise sente.exceptions.InvalidSGFException()
            sgf.load("invalid sgf/octopus.jpeg")

    def test_non_go_file(self):
        """

        tests to see if a non go file raises an exception

        :return:
        """

        with self.assertRaises(sente.exceptions.InvalidSGFException):
            sgf.load("invalid sgf/backgammon.sgf")
