"""

Author: Arthur Wesley

"""
from unittest import TestCase

import sente.utils

from assert_does_not_raise import DoesNotRaiseTestCase
from sente import *


class TestBasicMethods(DoesNotRaiseTestCase):

    def test_constructor(self):
        """

        checks that all possible constructors work

        :return:
        """

        with self.assertDoesNotRaise(Exception):
            Game()
            Game(13)
            Game(19)
            Game(9, CHINESE)
            Game(rules=CHINESE)
            Game(rules=JAPANESE)
            Game(rules=JAPANESE, board_size=13)

        with self.assertRaises(ValueError):
            Game(15, JAPANESE)

    def test_play(self):
        """

        checks to see that the syntax is valid

        :return:
        """

        game = Game()

        with self.assertDoesNotRaise(Exception):
            game.play(3, 15, BLACK)
            game.play(15, 15)

        with self.assertRaises(utils.IllegalMoveException):
            game.play(3, 15)

    def test_score_unfinished_game(self):
        """

        try to score an unfinished game

        :return:
        """

        game = Game()

        with self.assertRaises(ValueError):
            game.score()

        with self.assertRaises(ValueError):
            game.get_winner()

    def test_score_resigned_game(self):
        """

        try to score a game that has been resigned

        :return:
        """

        game = Game()
        game.resign()

        with self.assertRaises(ValueError):
            game.score()

        with self.assertDoesNotRaise(ValueError):
            game.get_winner()

    def test_get_legal_moves(self):
        """

        tests to see if the legal moves

        :return:
        """

        game = Game(9)

        self.assertEqual(83, len(game.get_legal_moves()))


class TestTreeNavigation(TestCase):

    def test_advance_to_root(self):
        """

        tests to see if the reset_to_root method works

        :return:
        """

        game = Game()

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

        game = Game()

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

        game = Game()

        # create a basic tree
        game.play(3, 3)
        game.play(3, 15)

        game.step_up()
        game.play(3, 15)

        self.assertEqual(stone.BLACK, game.get_point(3, 3))
        self.assertEqual(stone.WHITE, game.get_point(3, 15))

    def test_undo_multiple(self):
        """

        tests to see if multiple undos works

        :return:
        """

        game = Game()

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

        game = Game()

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

    def test_get_branches(self):
        """

        tests to see if the get_branches() method works

        :return:
        """

        game = Game()

        game.play(3, 3)
        game.play(3, 15)

        game.step_up()
        game.play(15, 3)

        game.step_up()
        game.play(15, 15)

        game.step_up()
        branches = game.get_branches()

        self.assertEqual(3, len(branches))

        self.assertIn(Move(14, 2, stone.WHITE), branches)
        self.assertIn(Move(14, 14, stone.WHITE), branches)
        self.assertIn(Move(2, 14, stone.WHITE), branches)

    def test_play_moves(self):
        """

        tests to see if the play_moves method is working

        :return:
        """

        game = Game()

        moves = [Move(2, 2, stone.BLACK), Move(4, 4, stone.WHITE), Move(6, 6, stone.BLACK)]

        game.play_sequence(moves)

        self.assertEqual(stone.BLACK, game.get_point(3, 3))
        self.assertEqual(stone.WHITE, game.get_point(5, 5))
        self.assertEqual(stone.BLACK, game.get_point(7, 7))

    def test_get_move_sequence(self):
        """

        checks to see if the game generates the correct sequence of moves

        :return:
        """

        game = Game()

        moves = [Move(3, 3, stone.BLACK), Move(5, 5, stone.WHITE), Move(7, 7, stone.BLACK)]

        game.play_sequence(moves)

        self.assertEqual(moves, game.get_sequence())

    def test_illegal_move_sequence(self):
        """

        checks to see if playing an illegal sequence of moves does not play a single move

        :return:
        """

        game = Game()

        moves = [Move(3, 3, stone.BLACK), Move(5, 5, stone.WHITE), Move(3, 3, stone.BLACK)]

        with self.assertRaises(sente.utils.IllegalMoveException):
            game.play_sequence(moves)
        self.assertEqual(stone.EMPTY, game.get_point(3, 3))

    def test_resign(self):
        """

        tests to see if resignation works

        :return:
        """

        game = Game()
        game.resign()

        self.assertEqual(stone.WHITE, game.get_winner())

    def test_get_children(self):
        """

        tests to see if the

        :return:
        """

        game = Game()

        self.assertEqual(game.get_branches(), [])

        game.play(3, 3)
        game.step_up()

        game.play(2, 3)
        game.step_up()

        self.assertEqual(game.get_branches(), [Move(2, 2, stone.BLACK), Move(1, 2, stone.BLACK)])
