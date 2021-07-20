"""

Author: Arthur Wesley

"""
from unittest import TestCase

from assert_does_not_raise import DoesNotRaiseTestCase
import sente


class TestBasicMethods(DoesNotRaiseTestCase):

    def test_constructor(self):
        """

        checks that all possible constructors work

        :return:
        """

        with self.assertDoesNotRaise(Exception):
            sente.Game()
            sente.Game(13)
            sente.Game(19)
            sente.Game(9, sente.rules.CHINESE)
            sente.Game(rules=sente.rules.CHINESE)
            sente.Game(rules=sente.rules.JAPANESE)
            sente.Game(rules=sente.rules.JAPANESE, board_size=13)

        with self.assertRaises(ValueError):
            sente.Game(15, sente.rules.JAPANESE)

    def test_play(self):
        """

        checks to see that the syntax is valid

        :return:
        """

        game = sente.Game()

        with self.assertDoesNotRaise(Exception):
            game.play(3, 15, sente.stone.BLACK)
            game.play(15, 15)

        with self.assertRaises(sente.exceptions.IllegalMoveException):
            game.play(3, 15)

    def test_get_active_player(self):
        """

        tests to see if the get_active_player() method works

        :return:
        """

        game = sente.Game()

        self.assertEqual(sente.stone.BLACK, game.get_active_player())

        game.play(4, 4)

        self.assertEqual(sente.stone.WHITE, game.get_active_player())

    def test_score_unfinished_game(self):
        """

        try to score an unfinished game

        :return:
        """

        game = sente.Game()

        with self.assertRaises(ValueError):
            game.score()

        with self.assertRaises(ValueError):
            game.get_winner()

    def test_score_resigned_game(self):
        """

        try to score a game that has been resigned

        :return:
        """

        game = sente.Game()
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

        game = sente.Game(9)

        self.assertEqual(83, len(game.get_legal_moves()))

    def test_score_empty_game(self):
        """

        tests to see if we can score an empty game

        :return:
        """

        game = sente.Game()

        game.pss()
        game.pss()

        with self.assertDoesNotRaise(Exception):
            game.score()

    def test_get_point(self):
        """

        tests to see if the get point() method works

        :return:
        """

        game = sente.Game()

        game.play(4, 5)

        self.assertEqual(game.get_point(4, 5), sente.stone.BLACK)

        with self.assertRaises(IndexError):
            game.get_point(20, 20)

        with self.assertRaises(IndexError):
            game.get_point(30, 30)


class TestTreeNavigation(TestCase):

    def test_advance_to_root(self):
        """

        tests to see if the reset_to_root method works

        :return:
        """

        game = sente.Game()

        game.play(3, 3)
        game.play(3, 4)
        game.play(4, 3)
        game.play(4, 4)
        game.play(3, 15)
        game.play(15, 3)
        game.play(15, 15)

        game.advance_to_root()

        self.assertEqual(sente.stone.EMPTY, game.get_point(3, 3))
        self.assertEqual(sente.stone.EMPTY, game.get_point(3, 4))
        self.assertEqual(sente.stone.EMPTY, game.get_point(4, 3))
        self.assertEqual(sente.stone.EMPTY, game.get_point(4, 4))
        self.assertEqual(sente.stone.EMPTY, game.get_point(3, 15))
        self.assertEqual(sente.stone.EMPTY, game.get_point(15, 3))
        self.assertEqual(sente.stone.EMPTY, game.get_point(15, 15))

    def test_undo_move(self):
        """

        tests to see if moves can be undone

        :return:
        """

        game = sente.Game()

        # create a basic tree
        game.play(3, 3)
        game.play(3, 15)

        self.assertEqual(sente.stone.BLACK, game.get_point(3, 3))
        self.assertEqual(sente.stone.WHITE, game.get_point(3, 15))
        game.step_up()
        self.assertEqual(sente.stone.BLACK, game.get_point(3, 3))
        self.assertEqual(sente.stone.EMPTY, game.get_point(3, 15))

    def test_undo_redo(self):
        """

        tests to see if a game

        :return:
        """

        game = sente.Game()

        # create a basic tree
        game.play(3, 3)
        game.play(3, 15)

        game.step_up()
        game.play(3, 15)

        self.assertEqual(sente.stone.BLACK, game.get_point(3, 3))
        self.assertEqual(sente.stone.WHITE, game.get_point(3, 15))

    def test_undo_multiple(self):
        """

        tests to see if multiple undos works

        :return:
        """

        game = sente.Game()

        game.play(3, 3)
        game.play(3, 15)
        game.play(15, 3)
        game.play(15, 15)

        self.assertEqual(sente.stone.BLACK, game.get_point(3, 3))
        self.assertEqual(sente.stone.WHITE, game.get_point(3, 15))
        self.assertEqual(sente.stone.BLACK, game.get_point(15, 3))
        self.assertEqual(sente.stone.WHITE, game.get_point(15, 15))

        game.step_up(3)

        self.assertEqual(sente.stone.BLACK, game.get_point(3, 3))
        self.assertEqual(sente.stone.EMPTY, game.get_point(3, 15))
        self.assertEqual(sente.stone.EMPTY, game.get_point(15, 3))
        self.assertEqual(sente.stone.EMPTY, game.get_point(15, 15))

    def test_simple_fork(self):
        """

        tests to see if the tree can navigate a simple fork

        :return:
        """

        game = sente.Game()

        game.play(3, 3)
        game.play(3, 15)

        game.step_up()
        game.play(15, 3)

        self.assertEqual(sente.stone.BLACK, game.get_point(3, 3))
        self.assertEqual(sente.stone.WHITE, game.get_point(15, 3))
        self.assertEqual(sente.stone.EMPTY, game.get_point(3, 15))

        # switch back to the other branch
        game.step_up()
        game.play(3, 15)

        self.assertEqual(sente.stone.BLACK, game.get_point(3, 3))
        self.assertEqual(sente.stone.EMPTY, game.get_point(15, 3))
        self.assertEqual(sente.stone.WHITE, game.get_point(3, 15))

    def test_get_branches(self):
        """

        tests to see if the get_branches() method works

        :return:
        """

        game = sente.Game()

        game.play(3, 3)
        game.play(3, 15)

        game.step_up()
        game.play(15, 3)

        game.step_up()
        game.play(15, 15)

        game.step_up()
        branches = game.get_branches()

        self.assertEqual(3, len(branches))

        self.assertIn(sente.Move(14, 2, sente.stone.WHITE), branches)
        self.assertIn(sente.Move(14, 14, sente.stone.WHITE), branches)
        self.assertIn(sente.Move(2, 14, sente.stone.WHITE), branches)

    def test_play_moves(self):
        """

        tests to see if the play_moves method is working

        :return:
        """

        game = sente.Game()

        moves = [sente.Move(2, 2, sente.stone.BLACK), sente.Move(4, 4, sente.stone.WHITE), sente.Move(6, 6, sente.stone.BLACK)]

        game.play_sequence(moves)

        self.assertEqual(sente.stone.BLACK, game.get_point(3, 3))
        self.assertEqual(sente.stone.WHITE, game.get_point(5, 5))
        self.assertEqual(sente.stone.BLACK, game.get_point(7, 7))

    def test_get_move_sequence(self):
        """

        checks to see if the game generates the correct sequence of moves

        :return:
        """

        game = sente.Game()

        moves = [sente.Move(3, 3, sente.stone.BLACK), sente.Move(5, 5, sente.stone.WHITE), sente.Move(7, 7, sente.stone.BLACK)]

        game.play_sequence(moves)

        self.assertEqual(moves, game.get_sequence())

    def test_illegal_move_sequence(self):
        """

        checks to see if playing an illegal sequence of moves does not play a single move

        :return:
        """

        game = sente.Game()

        moves = [sente.Move(3, 3, sente.stone.BLACK), sente.Move(5, 5, sente.stone.WHITE), sente.Move(3, 3, sente.stone.BLACK)]

        with self.assertRaises(sente.exceptions.IllegalMoveException):
            game.play_sequence(moves)
        self.assertEqual(sente.stone.EMPTY, game.get_point(3, 3))

    def test_get_default_sequence(self):
        """

        tests to see if the game correctly obtains the default sequence of moves

        :return:
        """

        game = sente.Game()

        game.play(4, 4)
        game.play(16, 4)
        game.play(4, 16)

        game.step_up()

        default_branch = game.get_default_sequence()

        self.assertEqual(game.get_point(4, 16), sente.stone.EMPTY)
        self.assertEqual(game.get_point(4, 4), sente.stone.BLACK)
        self.assertEqual(game.get_point(16, 4), sente.stone.WHITE)

        moves = [sente.Move(3, 3, sente.stone.BLACK), sente.Move(15, 3, sente.stone.WHITE), sente.Move(3, 15, sente.stone.BLACK)]

        self.assertEqual(moves, default_branch)

    def test_is_at_root(self):
        """

        tests to see if the is at root method works

        :return:
        """

        game = sente.Game()
        self.assertTrue(game.is_at_root())

        game.play(4, 4)
        self.assertFalse(game.is_at_root())

        game.play(3, 3)
        game.step_up()

        self.assertFalse(game.is_at_root())

        game.step_up()
        self.assertTrue(game.is_at_root())

    def test_resign(self):
        """

        tests to see if resignation works

        :return:
        """

        game = sente.Game()
        game.resign()

        self.assertEqual(sente.stone.WHITE, game.get_winner())

    def test_get_children(self):
        """

        tests to see if the

        :return:
        """

        game = sente.Game()

        self.assertEqual(game.get_branches(), [])

        game.play(3, 3)
        game.step_up()

        game.play(2, 3)
        game.step_up()

        self.assertEqual(game.get_branches(), [sente.Move(2, 2, sente.stone.BLACK), sente.Move(1, 2, sente.stone.BLACK)])

    def test_undo_resign(self):
        """

        tests to see if resignation can be undone

        TODO: implement

        :return:
        """
