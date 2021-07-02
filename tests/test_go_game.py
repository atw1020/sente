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

        GoGame(os.path.join("sgf", "34839594-255-IDW64-noob_bot_3.sgf"))

        for file in files:
            with self.assertDoesNotRaise(Exception):
                sgf.load(os.path.join("sgf", file))

    def test_single_branch_file(self):
        """

        tests to see if we can import a sgf file with a single branch

        :return:
        """

        game = sgf.load("sgf/simple_sequence.sgf")
