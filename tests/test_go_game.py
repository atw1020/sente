"""

Author: Arthur Wesley

"""
from unittest import TestCase

import sente
import numpy as np


class TestBasicMethods(TestCase):

    def test_constructor(self):
        """

        checks that all possible constructors work

        :return:
        """

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
            game.get_result()

        self.assertEqual(sente.EMPTY, game.get_winner())

    def test_score_resigned_game(self):
        """

        try to score a game that has been resigned

        :return:
        """

        game = sente.Game()
        game.resign()

        self.assertEqual(sente.WHITE, game.get_winner())

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

        self.assertEqual("W+7.5", str(game.get_result()))

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

    def test__str__(self):
        """

        tests to see if __str__ can be called on the object

        :return: None
        """

        game = sente.Game()

        self.assertEqual(str(game.get_board()), str(game))


class TestMetadata(TestCase):

    def test_add_metadata(self):
        """

        tests to see if metadata can be added to a game

        :return:
        """

        game = sente.Game()

        game.set_property("C", "This is a comment")

        self.assertEqual(game.comment, "This is a comment")

    def test_non_root_metadata(self):
        """

        tests to see if non root metadata is only available at the node it is put at

        :return:
        """

        game = sente.Game()

        game.comment = "TILL THEN I WALK ALONG"

        game.play(4, 4)
        self.assertEqual(game.comment, "")
        game.step_up()
        self.assertEqual(game.comment, "TILL THEN I WALK ALONG")

    def test_add_not_at_root(self):
        """

        tests to see if root attributes can be added at places other than the root

        :return:
        """

        game = sente.Game()

        game.play(4, 4)
        game.set_property("AP", "Sente")

        self.assertEqual(game.get_properties()["AP"], "Sente")
        game.step_up()
        self.assertEqual(game.get_properties()["AP"], "Sente")

    def test_closing_bracket_backslash_invisible(self):
        """

        makes sure that if SGF puts a backslash into a SGF field

        :return:
        """

        game = sente.Game()
        game.set_property("C", "[]")

        self.assertEqual(game.comment, "[]")


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

        moves = [sente.Move(2, 2, sente.stone.BLACK),
                 sente.Move(4, 4, sente.stone.WHITE), sente.Move(6, 6, sente.stone.BLACK)]

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

        moves = [sente.Move(3, 3, sente.stone.BLACK), sente.Move(5, 5, sente.stone.WHITE),
                 sente.Move(7, 7, sente.stone.BLACK)]

        game.play_sequence(moves)

        self.assertEqual(moves, game.get_current_sequence())

    def test_illegal_move_sequence(self):
        """

        checks to see if playing an illegal sequence of moves does not play a single move

        :return:
        """

        game = sente.Game()

        moves = [sente.Move(3, 3, sente.stone.BLACK), sente.Move(5, 5, sente.stone.WHITE),
                 sente.Move(3, 3, sente.stone.BLACK)]

        with self.assertRaises(sente.exceptions.IllegalMoveException):
            game.play_sequence(moves)
        self.assertEqual(sente.stone.EMPTY, game.get_point(3, 3))

    def test_get_default_sequence_root(self):
        """

        tests to see if the game correctly obtains the default sequence of moves

        :return:
        """

        game = sente.Game()

        game.play(4, 4)
        game.play(16, 4)
        game.play(4, 16)

        game.advance_to_root()

        default_branch = game.get_default_sequence()

        moves = [sente.Move(3, 3, sente.stone.BLACK), sente.Move(15, 3, sente.stone.WHITE),
                 sente.Move(3, 15, sente.stone.BLACK)]

        self.assertEqual(moves, default_branch)

    def test_get_default_sequence_child(self):
        """

        makes sure that the "default sequence" refers to the sequence of moves *continuing* from the current node

        :return:
        """

        game = sente.Game()

        game.play(4, 4)
        game.play(16, 4)
        game.play(4, 16)

        game.step_up()

        default_branch = game.get_default_sequence()

        moves = [sente.Move(3, 15, sente.stone.BLACK)]

        self.assertEqual(moves, default_branch)

    def test_get_all_sequences(self):
        """

        tests the get_all_sequences() method

        :return:
        """

        game = sente.Game()

        game.play(4, 4)
        game.play(4, 16)

        game.advance_to_root()

        game.play(16, 4)
        game.play(16, 16)

        game.advance_to_root()

        sequences = game.get_all_sequences()

        self.assertEqual([[sente.Move(3, 3, sente.stone.BLACK), sente.Move(3, 15, sente.stone.WHITE)],
                          [sente.Move(15, 3, sente.stone.BLACK), sente.Move(15, 15, sente.stone.WHITE)]], sequences)

    def test_get_all_sequences_does_not_move(self):
        """

        makes sure that calling the get_all_sequences() method does not move the current posisiton in the game tree

        :return:
        """

        game = sente.Game()

        game.play(4, 4)
        game.play(4, 16)

        game.advance_to_root()

        game.play(16, 4)
        game.play(16, 16)

        game.step_up()

        sequences = game.get_all_sequences()

        # we should only get one branch
        self.assertEqual([[sente.Move(15, 15, sente.stone.WHITE)]], sequences)

        # and that branch should not have been played
        self.assertEqual(sente.stone.BLACK, game.get_point(16, 4))
        self.assertEqual(sente.stone.EMPTY, game.get_point(16, 16))

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

    def test_step_up_0_steps(self):
        """

        check to see if we can step up once

        :return:
        """

        game = sente.Game()

        game.play(3, 3)
        game.step_up(0)

        self.assertEqual(sente.stone.BLACK, game.get_point(3, 3))

    def test_illegal_step_up(self):
        """

        tests to see if we get a value error when we try to step up past the root node

        :return:
        """

        game = sente.Game()

        game.play(3, 3)

        with self.assertRaises(ValueError):
            game.step_up(2)

    def test_undo_resign(self):
        """

        tests to see if resignation can be undone

        :return:
        """

        game = sente.Game()

        game.resign()

        self.assertTrue(game.is_legal(3, 3))

    def test_undo_double_pass(self):
        """

        tests to see if resignation can be undone

        :return:
        """

        game = sente.Game()

        game.pss()
        game.pss()

        game.advance_to_root()
        self.assertTrue(game.is_legal(3, 3))

    def test_resign_move(self):
        """

        checks to see if the resign move object causes resignation

        :return:
        """

        game = sente.Game()

        game.play(sente.moves.Resign(sente.stone.BLACK))

        self.assertTrue(game.is_over())

    def test_comment_write(self):
        """

        tests to see if comments can be added and stored

        :return:
        """

        game = sente.Game()

        game.comment = "This is the Root"

        self.assertEqual(game.comment, "This is the Root")

    def test_comment_long_term(self):
        """

        tests to see if comments persist after moves are undone and other comments are set

        :return:
        """

        game = sente.Game()

        game.comment = "This is the Root"

        game.play(4, 4)
        game.comment = "this is the first branch"
        game.step_up()
        game.play(16, 4)
        game.comment = "this is the second branch"

        game.advance_to_root()
        self.assertEqual(game.comment, "This is the Root")

        game.play(4, 4)
        self.assertEqual(game.comment, "this is the first branch")
        game.step_up()

        game.play(16, 4)
        self.assertEqual(game.comment, "this is the second branch")

    def test_comment_override(self):
        """

        tests to see if comments can be overridden

        :return:
        """

        game = sente.Game()

        game.comment = "this is the original string"
        game.comment = "this is the new string"

        self.assertEqual("this is the new string", game.comment)

    def test_comment_brackets(self):
        """

        tests to see if comments can have brackets in them

        :return:
        """

        game = sente.Game()

        game.comment = "here are some brackets [[]] ]["

        self.assertEqual("here are some brackets [[]] ][", game.comment)

    def test_backslashes_not_ignored(self):
        """

        tests to see if backslashes in comments are legal

        :return:
        """

        game = sente.Game()

        game.comment = "here is a backslash \\"

        self.assertEqual("here is a backslash \\", game.comment)


class TestNumpy(TestCase):

    def test_9x9_numpy(self):

        game = sente.Game(9)

        game.play(4, 4)
        game.play(9, 4)

        game.play(4, 9)
        game.play(9, 9)

        correct_board = np.zeros((9, 9, 4), dtype=np.uint8)

        for i in range(9):
            for j in range(9):
                if game.get_point(i + 1, j + 1) == sente.stone.BLACK:
                    correct_board[i][j][0] = 1
                elif game.get_point(i + 1, j + 1) == sente.stone.WHITE:
                    correct_board[i][j][1] = 1
                elif game.get_point(i + 1, j + 1) == sente.stone.EMPTY:
                    correct_board[i][j][2] = 1

    def test_13x13_numpy(self):

        game = sente.Game(13)

        game.play(4, 4)
        game.play(9, 4)

        game.play(4, 9)
        game.play(9, 9)

        correct_board = np.zeros((13, 13, 4), dtype=np.uint8)

        for i in range(13):
            for j in range(13):
                if game.get_point(i + 1, j + 1) == sente.stone.BLACK:
                    correct_board[i][j][0] = 1
                elif game.get_point(i + 1, j + 1) == sente.stone.WHITE:
                    correct_board[i][j][1] = 1
                elif game.get_point(i + 1, j + 1) == sente.stone.EMPTY:
                    correct_board[i][j][2] = 1

    def test_ko(self):
        """

        tests to see if ko points are accurately recorded

        :return:
        """

        game = sente.Game()

        game.play(2, 2)
        game.play(2, 1)

        game.play(3, 1)
        game.play(1, 2)

        game.play(1, 1)

        ko_numpy = game.numpy(["ko_points"])

        correct = np.zeros((19, 19, 1), dtype=np.uint8)
        correct[1, 0, 0] = 1

        self.assertTrue(np.array_equal(correct, ko_numpy))

    def test_full_game(self):
        """

        tests to see if a fully completed game matches the expected numpy array

        :return:
        """

        game = sente.sgf.load("tests/sgf/Lee Sedol ladder game.sgf")
        game.play_default_sequence()

        numpy = game.numpy()

        correct_board = np.zeros((19, 19, 4), dtype=np.uint8)

        for i in range(19):
            for j in range(19):
                if game.get_point(i + 1, j + 1) == sente.stone.BLACK:
                    correct_board[i][j][0] = 1
                elif game.get_point(i + 1, j + 1) == sente.stone.WHITE:
                    correct_board[i][j][1] = 1
                elif game.get_point(i + 1, j + 1) == sente.stone.EMPTY:
                    correct_board[i][j][2] = 1

        self.assertTrue(np.array_equal(correct_board, numpy))


class TestSetPoints(TestCase):

    def test_handicap_1(self):
        """

        checks to see if a handicap with 1 stone works

        :return:
        """

        game = sente.Game(handicap=sente.get_handicap_stones(1))
        self.assertEqual(game.get_point(16, 4), sente.stone.BLACK)
        self.assertEqual(game.get_active_player(), sente.stone.WHITE)

    def test_handicap_2(self):
        """

        checks to see if a handicap with 2 stones works

        :return:
        """

        game = sente.Game(handicap=sente.get_handicap_stones(2))

        self.assertEqual(game.get_point(16, 4), sente.stone.BLACK)
        self.assertEqual(game.get_point(4, 16), sente.stone.BLACK)
        self.assertEqual(game.get_active_player(), sente.stone.WHITE)

    def test_handicap_3(self):
        """

        checks to see if a handicap with 2 stones works

        :return:
        """

        game = sente.Game(handicap=sente.get_handicap_stones(3))

        self.assertEqual(game.get_point(16, 4), sente.stone.BLACK)
        self.assertEqual(game.get_point(4, 16), sente.stone.BLACK)
        self.assertEqual(game.get_point(16, 16), sente.stone.BLACK)
        self.assertEqual(game.get_active_player(), sente.stone.WHITE)

    def test_handicap_4(self):
        """

        checks to see if a handicap with 2 stones works

        :return:
        """

        game = sente.Game(handicap=sente.get_handicap_stones(4))

        self.assertEqual(game.get_point(4, 4), sente.stone.BLACK)
        self.assertEqual(game.get_point(16, 4), sente.stone.BLACK)
        self.assertEqual(game.get_point(4, 16), sente.stone.BLACK)
        self.assertEqual(game.get_point(16, 16), sente.stone.BLACK)
        self.assertEqual(game.get_active_player(), sente.stone.WHITE)

    def test_handicap_5(self):
        """

        checks to see if a handicap with 2 stones works

        :return:
        """

        game = sente.Game(handicap=sente.get_handicap_stones(5))

        self.assertEqual(game.get_point(4, 4), sente.stone.BLACK)
        self.assertEqual(game.get_point(16, 4), sente.stone.BLACK)
        self.assertEqual(game.get_point(10, 10), sente.stone.BLACK)
        self.assertEqual(game.get_point(4, 16), sente.stone.BLACK)
        self.assertEqual(game.get_point(16, 16), sente.stone.BLACK)
        self.assertEqual(game.get_active_player(), sente.stone.WHITE)

    def test_handicap_6(self):
        """

        checks to see if a handicap with 2 stones works

        :return:
        """

        game = sente.Game(handicap=sente.get_handicap_stones(6))

        self.assertEqual(game.get_point(4, 4), sente.stone.BLACK)
        self.assertEqual(game.get_point(16, 4), sente.stone.BLACK)
        self.assertEqual(game.get_point(4, 16), sente.stone.BLACK)
        self.assertEqual(game.get_point(16, 16), sente.stone.BLACK)
        self.assertEqual(game.get_point(16, 10), sente.stone.BLACK)
        self.assertEqual(game.get_point(4, 10), sente.stone.BLACK)
        self.assertEqual(game.get_active_player(), sente.stone.WHITE)

    def test_handicap_7(self):
        """

        checks to see if a handicap with 2 stones works

        :return:
        """

        game = sente.Game(handicap=sente.get_handicap_stones(7))

        self.assertEqual(game.get_point(4, 4), sente.stone.BLACK)
        self.assertEqual(game.get_point(16, 4), sente.stone.BLACK)
        self.assertEqual(game.get_point(4, 16), sente.stone.BLACK)
        self.assertEqual(game.get_point(16, 16), sente.stone.BLACK)
        self.assertEqual(game.get_point(16, 10), sente.stone.BLACK)
        self.assertEqual(game.get_point(4, 10), sente.stone.BLACK)
        self.assertEqual(game.get_point(10, 10), sente.stone.BLACK)
        self.assertEqual(game.get_active_player(), sente.stone.WHITE)

    def test_handicap_8(self):
        """

        checks to see if a handicap with 2 stones works

        :return:
        """

        game = sente.Game(handicap=sente.get_handicap_stones(8))

        self.assertEqual(game.get_point(4, 4), sente.stone.BLACK)
        self.assertEqual(game.get_point(16, 4), sente.stone.BLACK)
        self.assertEqual(game.get_point(4, 16), sente.stone.BLACK)
        self.assertEqual(game.get_point(16, 16), sente.stone.BLACK)
        self.assertEqual(game.get_point(16, 10), sente.stone.BLACK)
        self.assertEqual(game.get_point(4, 10), sente.stone.BLACK)
        self.assertEqual(game.get_point(10, 4), sente.stone.BLACK)
        self.assertEqual(game.get_point(10, 16), sente.stone.BLACK)
        self.assertEqual(game.get_active_player(), sente.stone.WHITE)

    def test_handicap_9(self):
        """

        checks to see if a handicap with 2 stones works

        :return:
        """

        game = sente.Game(handicap=sente.get_handicap_stones(9))

        self.assertEqual(game.get_point(4, 4), sente.stone.BLACK)
        self.assertEqual(game.get_point(16, 4), sente.stone.BLACK)
        self.assertEqual(game.get_point(4, 16), sente.stone.BLACK)
        self.assertEqual(game.get_point(16, 16), sente.stone.BLACK)
        self.assertEqual(game.get_point(16, 10), sente.stone.BLACK)
        self.assertEqual(game.get_point(4, 10), sente.stone.BLACK)
        self.assertEqual(game.get_point(10, 4), sente.stone.BLACK)
        self.assertEqual(game.get_point(10, 10), sente.stone.BLACK)
        self.assertEqual(game.get_active_player(), sente.stone.WHITE)

    def test_play_after_set(self):
        """

        checks to see if we can continue normal play after we set some points

        :return:
        """

    def test_setup_problem(self):
        """

        tests to see if we can add many moves

        :return:
        """

        game = sente.Game()

        game.set_points([sente.Move(1, 0, sente.stone.BLACK),
                         sente.Move(1, 1, sente.stone.BLACK),
                         sente.Move(1, 2, sente.stone.BLACK),
                         sente.Move(1, 3, sente.stone.BLACK),
                         sente.Move(0, 3, sente.stone.BLACK)])

        game.set_points([sente.Move(2, 0, sente.stone.WHITE),
                         sente.Move(2, 1, sente.stone.WHITE),
                         sente.Move(2, 2, sente.stone.WHITE),
                         sente.Move(2, 3, sente.stone.WHITE),
                         sente.Move(2, 4, sente.stone.WHITE),
                         sente.Move(1, 4, sente.stone.WHITE),
                         sente.Move(0, 4, sente.stone.WHITE)])

        self.assertEqual(game.get_point(2, 1), sente.stone.BLACK)
        self.assertEqual(game.get_point(2, 2), sente.stone.BLACK)
        self.assertEqual(game.get_point(2, 3), sente.stone.BLACK)
        self.assertEqual(game.get_point(2, 4), sente.stone.BLACK)
        self.assertEqual(game.get_point(1, 4), sente.stone.BLACK)

        self.assertEqual(game.get_point(3, 1), sente.stone.WHITE)
        self.assertEqual(game.get_point(3, 2), sente.stone.WHITE)
        self.assertEqual(game.get_point(3, 3), sente.stone.WHITE)
        self.assertEqual(game.get_point(3, 4), sente.stone.WHITE)
        self.assertEqual(game.get_point(3, 5), sente.stone.WHITE)
        self.assertEqual(game.get_point(2, 5), sente.stone.WHITE)
        self.assertEqual(game.get_point(1, 5), sente.stone.WHITE)

        self.assertEqual(game.get_active_player(), sente.stone.BLACK)

    def test_reset_points(self):
        """

        makes sure we can set points to be empty

        :return:
        """

        game = sente.Game()

        game.set_points([sente.Move(4, 4, sente.stone.BLACK)])
        game.set_points([sente.Move(4, 4, sente.stone.EMPTY)])

        self.assertEqual(sente.stone.EMPTY, game.get_point(4, 4))
