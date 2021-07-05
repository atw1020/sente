"""

Author: Arthur Wesley

"""
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
            GoGame(13)
            GoGame(19)
            GoGame(9, CHINESE)
            GoGame(rules=CHINESE)
            GoGame(rules=JAPANESE)
            GoGame(rules=JAPANESE, board_size=13)

        with self.assertRaises(ValueError):
            GoGame(15, JAPANESE)

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


class TestTreeNavigation(TestCase):

    def test_advance_to_root(self):
        """

        tests to see if the reset_to_root method works

        :return:
        """

        game = GoGame()

        game.play(3, 3)
        game.play(3, 4)
        game.play(4, 3)
        game.play(4, 4)
        game.play(3, 15)
        game.play(15, 3)
        game.play(15, 15)

        game.advance_to_root()

        self.assertEqual(stone.EMPTY, game.get_point(3, 3))
        self.assertEqual(stone.EMPTY, game.get_point(3, 4))
        self.assertEqual(stone.EMPTY, game.get_point(4, 3))
        self.assertEqual(stone.EMPTY, game.get_point(4, 4))
        self.assertEqual(stone.EMPTY, game.get_point(3, 15))
        self.assertEqual(stone.EMPTY, game.get_point(15, 3))
        self.assertEqual(stone.EMPTY, game.get_point(15, 15))

    def test_undo_move(self):
        """

        tests to see if moves can be undone

        :return:
        """

        game = GoGame()

        # create a basic tree
        game.play(3, 3)
        game.play(3, 15)

        self.assertEqual(stone.BLACK, game.get_point(3, 3))
        self.assertEqual(stone.WHITE, game.get_point(3, 15))
        game.step_up()
        self.assertEqual(stone.BLACK, game.get_point(3, 3))
        self.assertEqual(stone.EMPTY, game.get_point(3, 15))

    def test_undo_redo(self):
        """

        tests to see if a game

        :return:
        """

        game = GoGame()

        # create a basic tree
        game.play(3, 3)
        game.play(3, 15)

        game.step_up()
        game.play(3, 15)

        print(game)

        self.assertEqual(stone.BLACK, game.get_point(3, 3))
        self.assertEqual(stone.WHITE, game.get_point(3, 15))

    def test_undo_multiple(self):
        """

        tests to see if multiple undos works

        :return:
        """

        game = GoGame()

        game.play(3, 3)
        game.play(3, 15)
        game.play(15, 3)
        game.play(15, 15)

        self.assertEqual(stone.BLACK, game.get_point(3, 3))
        self.assertEqual(stone.WHITE, game.get_point(3, 15))
        self.assertEqual(stone.BLACK, game.get_point(15, 3))
        self.assertEqual(stone.WHITE, game.get_point(15, 15))

        game.step_up(3)

        self.assertEqual(stone.BLACK, game.get_point(3, 3))
        self.assertEqual(stone.EMPTY, game.get_point(3, 15))
        self.assertEqual(stone.EMPTY, game.get_point(15, 3))
        self.assertEqual(stone.EMPTY, game.get_point(15, 15))

    def test_simple_fork(self):
        """

        tests to see if the tree can navigate a simple fork

        :return:
        """

        game = GoGame()

        game.play(3, 3)
        game.play(3, 15)

        game.step_up()
        game.play(15, 3)

        self.assertEqual(stone.BLACK, game.get_point(3, 3))
        self.assertEqual(stone.WHITE, game.get_point(15, 3))
        self.assertEqual(stone.EMPTY, game.get_point(3, 15))

        # switch back to the other branch
        game.step_up()
        game.play(3, 15)

        self.assertEqual(stone.BLACK, game.get_point(3, 3))
        self.assertEqual(stone.EMPTY, game.get_point(15, 3))
        self.assertEqual(stone.WHITE, game.get_point(3, 15))

    def test_play_moves(self):
        """

        tests to see if the play_moves method is working

        :return:
        """

        game = GoGame()

        moves = [Move(3, 3, stone.BLACK), Move(5, 5, stone.WHITE), Move(7, 7, stone.BLACK)]

        game.play_moves(moves)

        self.assertEqual(stone.BLACK, game.get_point(3, 3))
        self.assertEqual(stone.WHITE, game.get_point(5, 5))
        self.assertEqual(stone.BLACK, game.get_point(7, 7))

    def test_get_move_sequence(self):
        """

        checks to see if the game generates the correct sequence of moves

        :return:
        """

        game = GoGame()

        moves = [Move(3, 3, stone.BLACK), Move(5, 5, stone.WHITE), Move(7, 7, stone.BLACK)]

        game.play_moves(moves)

        self.assertEqual(moves, game.get_moves())
