"""

Author: Arthur Wesley

"""
import opcode
import os
from unittest import TestCase

from assert_does_not_raise import DoesNotRaiseTestCase
from sente import *


class TestBasicMethods(DoesNotRaiseTestCase):

    def test_constructor(self):
        """

        checks that all possible constructors work

        :return:
        """

        with self.assertDoesNotRaise(Exception):
            GoGame()
            GoGame(CHINESE)
            GoGame(JAPANESE)
            GoGame(CHINESE, 9)
            GoGame(rules=CHINESE)
            GoGame(rules=JAPANESE, board_size=13)

        with self.assertRaises(TypeError):
            GoGame(19)

        with self.assertRaises(ValueError):
            GoGame(JAPANESE, 15)

    def test_play(self):
        """

        checks to see that the syntax is valid

        :return:
        """

        game = GoGame()

        print("test section A")

        with self.assertDoesNotRaise(Exception):
            game.play(3, 15, BLACK)
            game.play(15, 15)

        with self.assertRaises(IllegalMoveException):
            game.play(3, 15)


class TestSGF(DoesNotRaiseTestCase):

    def test_all_sgfs(self):
        """

        tests to see if any of the sgf files in the directory raise any exceptions

        :return:
        """

        files = os.listdir("sgf")

        # sgf.load(os.path.join("sgf", "34839594-255-IDW64-noob_bot_3.sgf"))

        for file in files:
            with self.assertDoesNotRaise(Exception):
                game = sgf.load(os.path.join("sgf", file))
                game.play_default_branch()

    def test_simple_single_branch_file(self):
        """

        tests to see if we can import a sgf file with a single branch

        :return:
        """

        B = BLACK
        W = WHITE
        _ = EMPTY

        game = sgf.load("sgf/simple_sequence.sgf")
        game.play_default_branch()

        expected_game = Board19([[_, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _],
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

        B = BLACK
        W = WHITE
        _ = EMPTY

        game = sgf.load("sgf/34839594-255-IDW64-noob_bot_3.sgf")
        game.play_default_branch()

        expected_game = Board19([[_, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _],
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
                                 [_, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _],
                                 [_, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _],
                                 [_, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _],
                                 [_, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _],
                                 [_, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _],
                                 [_, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _]])

        print(game)
