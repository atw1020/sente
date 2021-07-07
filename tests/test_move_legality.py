"""

Author: Arthur Wesley

"""
from unittest import TestCase

from sente import *
from assert_does_not_raise import DoesNotRaiseTestCase


class TestMakeMove(DoesNotRaiseTestCase):

    def test_make_move(self):
        """

        tests to see if we can make a simple move

        :return:
        """

        game = Game()

        game.play(3, 3, BLACK)

        self.assertEqual(BLACK, game.get_point(3, 3))

    def test_capture_stone(self):
        """

        tests to see if the board correctly removes stones

        :return:
        """

        game = Game()

        game.play(2, 3, BLACK)
        game.play(3, 3, WHITE)

        game.play(4, 3, BLACK)
        game.play(18, 18, WHITE)

        game.play(3, 2, BLACK)
        game.play(18, 17, WHITE)

        game.play(3, 4, BLACK)

        self.assertEqual(EMPTY, game.get_point(3, 3))

    def test_capture_multiple_stones(self):
        """

        checks to see if a group of multiple stones can be captured

        :return:
        """

        game = Game()

        game.play(2, 3, BLACK)
        game.play(3, 3, WHITE)

        game.play(4, 3, BLACK)
        game.play(3, 2, WHITE)

        game.play(2, 2, BLACK)
        game.play(18, 18, WHITE)

        game.play(4, 2, BLACK)
        game.play(18, 17, WHITE)

        game.play(3, 1, BLACK)
        game.play(18, 16, WHITE)

        game.play(3, 4, BLACK)

        self.assertEqual(EMPTY, game.get_point(3, 3))
        self.assertEqual(EMPTY, game.get_point(3, 2))

    def test_capture_multiple_groups(self):
        """

        tests to see if multiple stones can be captured with one move

        :return:
        """

        game = Game()

        game.play(0, 2, BLACK)
        game.play(0, 1, WHITE)

        game.play(1, 1, BLACK)
        game.play(1, 0, WHITE)

        game.play(2, 0, BLACK)
        game.play(18, 18, WHITE)

        game.play(0, 0, BLACK)

        self.assertEqual(EMPTY, game.get_point(0, 1))
        self.assertEqual(EMPTY, game.get_point(1, 0))

    def test_capture_edge(self):
        """

        checks to see if stones on the edge of the board can be captured

        :return:
        """

        game = Game()

        game.play(0, 0, BLACK)
        game.play(1, 1, WHITE)

        game.play(0, 1, BLACK)
        game.play(0, 2, WHITE)

        game.play(1, 0, BLACK)
        game.play(2, 0, WHITE)

        self.assertEqual(EMPTY, game.get_point(0, 0))
        self.assertEqual(EMPTY, game.get_point(1, 0))
        self.assertEqual(EMPTY, game.get_point(0, 1))

    def test_pass(self):
        """

        verifies that all methods of passing are accepted as valid

        :return:
        """

        game = Game()

        with self.assertDoesNotRaise(utils.IllegalMoveException):
            game.play(None)
            game.play_pass()


class TestLegalMove(TestCase):

    def test_empty_correct_color(self):
        """

        tests to if the correct color is detected (assuming MakeMove does not work

        :return:
        """

        # create a 19x19 board
        game = Game()

        self.assertTrue(game.is_legal(3, 3, BLACK))
        self.assertFalse(game.is_legal(3, 3, WHITE))

        self.assertTrue(game.is_legal(Move(15, 15, BLACK)))
        self.assertFalse(game.is_legal(Move(15, 15, WHITE)))

    def test_correct_color(self):
        """

        tests to see if the color is detected

        :return:
        """

        game = Game()

        game.play(3, 3, BLACK)

        self.assertTrue(game.is_legal(15, 3, WHITE))
        self.assertFalse(game.is_legal(15, 3, BLACK))

        self.assertTrue(game.is_legal(Move(15, 3, WHITE)))
        self.assertFalse(game.is_legal(Move(15, 3, BLACK)))

    def test_empty_out_of_bounds(self):
        """

        checks to see if out of bounds coords are illegal

        :return:
        """

        game = Game()

        self.assertTrue(game.is_legal(19, 19, BLACK))
        self.assertFalse(game.is_legal(20, 19, BLACK))
        self.assertFalse(game.is_legal(19, 20, BLACK))

        self.assertTrue(game.is_legal(Move(19, 19, BLACK)))
        self.assertFalse(game.is_legal(Move(20, 19, BLACK)))
        self.assertFalse(game.is_legal(Move(19, 20, BLACK)))

    def test_occupied_space(self):
        """

        checks to see if playing on an occupied space is illegal

        :return:
        """

        game = Game()

        game.play(2, 3, BLACK)
        game.play(15, 3, WHITE)

        self.assertFalse(game.is_legal(2, 3, BLACK))
        self.assertFalse(game.is_legal(15, 3, BLACK))

    def test_self_capture(self):
        """

        checks to see if a self-capture move is illegal

        :return:
        """

        game = Game()

        game.play(0, 1, BLACK)
        game.play(18, 18, WHITE)

        game.play(1, 0, BLACK)

        self.assertFalse(game.is_legal(0, 0, WHITE))

    def test_group_self_capture(self):
        """

        checks to see self capture moves are illegal for groups of stones

        :return:
        """

        # TODO: Implement

        game = Game()

        game.play(0, 1, BLACK)

    def test_empty_triangle_liberties(self):
        """

        checks to see if a group's overlapping liberties matter

        :return:
        """

        game = Game()

        print("starting game!")

        game.play(0, 2)
        game.play(0, 1)

        print(game)

        game.play(1, 2)
        game.play(1, 1)

        print(game)

        game.play(2, 1)
        game.play(1, 0)

        print(game)

        game.play(2, 0)
        game.play(18, 18)

        self.assertTrue(game.is_legal(0, 0))

    def test_ko(self):
        """

        checks to see if the game correctly recognizes a Ko move as illegal

        :return:
        """

        game = Game()

        game.play(2, 3, BLACK)
        game.play(3, 3, WHITE)

        game.play(4, 3, BLACK)
        game.play(1, 3, WHITE)

        game.play(3, 2, BLACK)
        game.play(2, 4, WHITE)

        game.play(3, 4, BLACK)
        game.play(2, 2, WHITE)

        # play away before taking the ko

        game.play(18, 18, BLACK)
        game.play(3, 3, WHITE)  # take the Ko

        self.assertFalse(game.is_legal(2, 3, BLACK))

    def test_inactive_ko(self):
        """

        checks to see if a ko gos inactive after making a ko threat

        :return:
        """

        game = Game()

        game.play(2, 3, BLACK)
        game.play(3, 3, WHITE)

        game.play(4, 3, BLACK)
        game.play(1, 3, WHITE)

        game.play(3, 2, BLACK)
        game.play(2, 4, WHITE)

        game.play(3, 4, BLACK)
        game.play(2, 2, WHITE)

        # play away before taking the ko

        game.play(18, 18, BLACK)
        game.play(3, 3, WHITE)  # take the Ko

        # simulate a ko threat
        game.play(18, 0, BLACK)
        game.play(17, 0, WHITE)

        # the Ko should no longer be active
        self.assertTrue(game.is_legal(2, 3, BLACK))

        game.play(2, 3, BLACK)

        # it should now be illegal for white to play here
        self.assertFalse(game.is_legal(3, 3, WHITE))


class IllegalMoveThrowsException(DoesNotRaiseTestCase):
    """

    makes sure that making illegal moves throws an exception

    """

    def test_empty_correct_color(self):
        """

        tests to if the correct color is detected (assuming MakeMove does not work

        :return:
        """

        # create a 19x19 board
        game = Game()

        with self.assertRaises(utils.IllegalMoveException):
            game.play(3, 3, WHITE)

        with self.assertRaises(utils.IllegalMoveException):
            game.play(15, 15, WHITE)

    def test_correct_color(self):
        """

        tests to see if the color is detected

        :return:
        """

        game = Game()

        game.play(3, 3, BLACK)

        with self.assertRaises(utils.IllegalMoveException):
            game.play(15, 3, BLACK)

        with self.assertRaises(utils.IllegalMoveException):
            game.play(Move(15, 3, BLACK))

    def test_empty_out_of_bounds(self):
        """

        checks to see if out of bounds coords are illegal

        :return:
        """

        game = Game()

        with self.assertRaises(utils.IllegalMoveException):
            game.play(20, 19, BLACK)
            game.play(19, 20, BLACK)

        with self.assertRaises(utils.IllegalMoveException):
            game.play(Move(20, 19, BLACK))
            game.play(Move(19, 20, BLACK))

    def test_occupied_space(self):
        """

        checks to see if playing on an occupied space is illegal

        :return:
        """

        game = Game()

        game.play(2, 3, BLACK)
        game.play(15, 3, WHITE)

        with self.assertRaises(utils.IllegalMoveException):
            game.play(2, 3, BLACK)
            game.play(15, 3, BLACK)

    def test_self_capture(self):
        """

        checks to see if a self-capture move is illegal

        :return:
        """

        game = Game()

        game.play(1, 2, BLACK)
        game.play(19, 19, WHITE)

        game.play(2, 1, BLACK)

        with self.assertRaises(utils.IllegalMoveException):
            game.play(1, 1, WHITE)

    def test_group_self_capture(self):
        """

        checks to see self capture moves are illegal for groups of stones

        :return:
        """

        # TODO: Implement

        game = Game()

        game.play(0, 1, BLACK)

    def test_ko(self):
        """

        checks to see if the game correctly recognizes a Ko move as illegal

        :return:
        """

        game = Game()

        game.play(2, 3, BLACK)
        game.play(3, 3, WHITE)

        game.play(4, 3, BLACK)
        game.play(1, 3, WHITE)

        game.play(3, 2, BLACK)
        game.play(2, 4, WHITE)

        game.play(3, 4, BLACK)
        game.play(2, 2, WHITE)

        # play away before taking the ko

        game.play(18, 18, BLACK)
        game.play(3, 3, WHITE)  # take the Ko

        with self.assertRaises(utils.IllegalMoveException):
            game.play(2, 3, BLACK)

    def test_inactive_ko(self):
        """

        checks to see if a ko gos inactive after making a ko threat

        :return:
        """

        game = Game()

        game.play(2, 3, BLACK)
        game.play(3, 3, WHITE)

        game.play(4, 3, BLACK)
        game.play(1, 3, WHITE)

        game.play(3, 2, BLACK)
        game.play(2, 4, WHITE)

        game.play(3, 4, BLACK)
        game.play(2, 2, WHITE)

        # play away before taking the ko

        game.play(18, 18, BLACK)
        game.play(3, 3, WHITE)  # take the Ko

        # simulate a ko threat
        game.play(18, 1, BLACK)
        game.play(17, 1, WHITE)

        # the Ko should no longer be active
        with self.assertDoesNotRaise(utils.IllegalMoveException):
            game.play(2, 3, BLACK)

        game.play(2, 3, BLACK)

        # it should now be illegal for white to play here
        with self.assertRaises(utils.IllegalMoveException):
            game.play(3, 3, WHITE)
