"""

Author: Arthur Wesley

"""
from unittest import TestCase

import sente


class TestMove(TestCase):

    def test_make_move(self):
        """

        tests to see if we can make a simple move

        :return:
        """

        game = sente.Game()

        game.play(3, 3, sente.stone.BLACK)

        self.assertEqual(sente.stone.BLACK, game.get_point(3, 3))

    def test_capture_stone(self):
        """

        tests to see if the board correctly removes stones

        :return:
        """

        game = sente.Game()

        game.play(2, 3, sente.stone.BLACK)
        game.play(3, 3, sente.stone.WHITE)

        game.play(4, 3, sente.stone.BLACK)
        game.play(18, 18, sente.stone.WHITE)

        game.play(3, 2, sente.stone.BLACK)
        game.play(18, 17, sente.stone.WHITE)

        game.play(3, 4, sente.stone.BLACK)

        self.assertEqual(sente.stone.EMPTY, game.get_point(3, 3))

    def test_capture_multiple_stones(self):
        """

        checks to see if a group of multiple stones can be captured

        :return:
        """

        game = sente.Game()

        game.play(2, 3, sente.stone.BLACK)
        game.play(3, 3, sente.stone.WHITE)

        game.play(4, 3, sente.stone.BLACK)
        game.play(3, 2, sente.stone.WHITE)

        game.play(2, 2, sente.stone.BLACK)
        game.play(18, 18, sente.stone.WHITE)

        game.play(4, 2, sente.stone.BLACK)
        game.play(18, 17, sente.stone.WHITE)

        game.play(3, 1, sente.stone.BLACK)
        game.play(18, 16, sente.stone.WHITE)

        game.play(3, 4, sente.stone.BLACK)

        self.assertEqual(sente.stone.EMPTY, game.get_point(3, 3))
        self.assertEqual(sente.stone.EMPTY, game.get_point(3, 2))

    def test_capture_multiple_groups(self):
        """

        tests to see if multiple stones can be captured with one move

        :return:
        """

        game = sente.Game()

        game.play(1, 3, sente.stone.BLACK)
        game.play(1, 2, sente.stone.WHITE)

        game.play(2, 2, sente.stone.BLACK)
        game.play(2, 1, sente.stone.WHITE)

        game.play(3, 1, sente.stone.BLACK)
        game.play(19, 19, sente.stone.WHITE)

        game.play(1, 1, sente.stone.BLACK)

        self.assertEqual(sente.stone.EMPTY, game.get_point(1, 2))
        self.assertEqual(sente.stone.EMPTY, game.get_point(2, 1))

    def test_capture_edge(self):
        """

        checks to see if stones on the edge of the board can be captured

        :return:
        """

        game = sente.Game()

        game.play(1, 1, sente.stone.BLACK)
        game.play(2, 2, sente.stone.WHITE)

        game.play(1, 2, sente.stone.BLACK)
        game.play(1, 3, sente.stone.WHITE)

        game.play(2, 1, sente.stone.BLACK)
        game.play(3, 1, sente.stone.WHITE)

        self.assertEqual(sente.stone.EMPTY, game.get_point(1, 1))
        self.assertEqual(sente.stone.EMPTY, game.get_point(2, 1))
        self.assertEqual(sente.stone.EMPTY, game.get_point(1, 2))

    def test_pass_none(self):
        """

        verifies that you can pass a move by passing none

        :return:
        """

        game = sente.Game()

        game.play(None)
        game.play(None)

        self.assertTrue(game.is_over())

    def test_pss_method(self):
        """

        makes sure that the pass method works

        :return: None
        """

        game = sente.Game()

        game.pss()
        game.pss()

        self.assertTrue(game.is_over())

    def test_pass_move(self):
        """

        checks to see if passing a pass move causes a pass

        :return:
        """

        game = sente.Game()

        game.play(sente.moves.Pass(sente.stone.BLACK))
        game.play(sente.moves.Pass(sente.stone.WHITE))

        self.assertTrue(game.is_over())

    def test_self_atari_legal(self):
        """

        makes sure that self atari is a legal move

        :return:
        """

        game = sente.Game()

        game.play(1, 2)

        self.assertTrue(game.is_legal(1, 1))

        try:
            game.play(1, 1)
        except Exception as E:
            self.fail(E)


class TestLegalMove(TestCase):

    def test_empty_correct_color(self):
        """

        tests to if the correct color is detected (assuming MakeMove does not work

        :return:
        """

        # create a 19x19 board
        game = sente.Game()

        self.assertTrue(game.is_legal(3, 3, sente.stone.BLACK))
        self.assertFalse(game.is_legal(3, 3, sente.stone.WHITE))

        self.assertTrue(game.is_legal(sente.Move(sente.stone.BLACK, 15, 15)))
        self.assertFalse(game.is_legal(sente.Move(sente.stone.WHITE, 15, 15)))

    def test_correct_color(self):
        """

        tests to see if the color is detected

        :return:
        """

        game = sente.Game()

        game.play(3, 3, sente.stone.BLACK)

        self.assertTrue(game.is_legal(15, 3, sente.stone.WHITE))
        self.assertFalse(game.is_legal(15, 3, sente.stone.BLACK))

        self.assertTrue(game.is_legal(sente.Move(sente.stone.WHITE, 15, 3)))
        self.assertFalse(game.is_legal(sente.Move(sente.stone.BLACK, 15, 3)))

    def test_empty_out_of_bounds(self):
        """

        checks to see if out of bounds coords are illegal

        :return:
        """

        game = sente.Game()

        self.assertTrue(game.is_legal(19, 19, sente.stone.BLACK))
        self.assertFalse(game.is_legal(20, 19, sente.stone.BLACK))
        self.assertFalse(game.is_legal(19, 20, sente.stone.BLACK))

        # internal indexing
        self.assertTrue(game.is_legal(sente.Move(sente.stone.BLACK, 19, 19)))
        self.assertFalse(game.is_legal(sente.Move(sente.stone.BLACK, 20, 19)))
        self.assertFalse(game.is_legal(sente.Move(sente.stone.BLACK, 19, 20)))

    def test_occupied_space(self):
        """

        checks to see if playing on an occupied space is illegal

        :return:
        """

        game = sente.Game()

        game.play(2, 3, sente.stone.BLACK)
        game.play(15, 3, sente.stone.WHITE)

        self.assertFalse(game.is_legal(2, 3, sente.stone.BLACK))
        self.assertFalse(game.is_legal(15, 3, sente.stone.BLACK))

    def test_self_capture(self):
        """

        checks to see if a self-capture move is illegal unless we're using
        Tromp-Taylor rules

        :return:
        """

        for ruleset in (sente.rules.CHINESE, sente.rules.TROMP_TAYLOR):
            game = sente.Game(19, ruleset)

            game.play(1, 2, sente.stone.BLACK)
            game.play(19, 19, sente.stone.WHITE)

            game.play(2, 1, sente.stone.BLACK)

            legal = game.is_legal(1, 1, sente.stone.WHITE)
            if ruleset == sente.rules.CHINESE:
                self.assertFalse(legal)
            else:
                self.assertTrue(legal)

    def test_group_self_capture(self):
        """

        checks to see self capture moves are illegal for groups of stones
        unless we're using Tromp-Taylor rules

        :return:
        """

        for ruleset in (sente.rules.CHINESE, sente.rules.TROMP_TAYLOR):
            game = sente.Game(19, ruleset)

            game.play(1, 3, sente.stone.BLACK)
            game.play(1, 2, sente.stone.WHITE)

            game.play(2, 3, sente.stone.BLACK)
            game.play(2, 2, sente.stone.WHITE)

            game.play(3, 2, sente.stone.BLACK)
            game.play(2, 1, sente.stone.WHITE)

            game.play(3, 1, sente.stone.BLACK)

            legal = game.is_legal(1, 1)
            if ruleset == sente.rules.CHINESE:
                self.assertFalse(legal)
            else:
                self.assertTrue(legal)

    def test_empty_triangle_liberties(self):
        """

        checks to see if a group's overlapping liberties matter

        :return:
        """

        game = sente.Game()

        game.play(1, 3)
        game.play(1, 2)

        game.play(2, 3)
        game.play(2, 2)

        game.play(3, 2)
        game.play(2, 1)

        game.play(3, 1)
        game.play(19, 19)

        self.assertTrue(game.is_legal(1, 1))

    def test_ko(self):
        """

        checks to see if the game correctly recognizes a Ko move as illegal

        :return:
        """

        game = sente.Game()

        game.play(2, 3, sente.stone.BLACK)
        game.play(3, 3, sente.stone.WHITE)

        game.play(4, 3, sente.stone.BLACK)
        game.play(1, 3, sente.stone.WHITE)

        game.play(3, 2, sente.stone.BLACK)
        game.play(2, 4, sente.stone.WHITE)

        game.play(3, 4, sente.stone.BLACK)
        game.play(2, 2, sente.stone.WHITE)

        # play away before taking the ko

        game.play(18, 18, sente.stone.BLACK)
        game.play(3, 3, sente.stone.WHITE)  # take the Ko

        self.assertFalse(game.is_legal(2, 3, sente.stone.BLACK))

    def test_inactive_ko(self):
        """

        checks to see if a ko gos inactive after making a ko threat

        :return:
        """

        game = sente.Game()

        game.play(3, 4, sente.stone.BLACK)
        game.play(4, 4, sente.stone.WHITE)

        game.play(5, 4, sente.stone.BLACK)
        game.play(2, 4, sente.stone.WHITE)

        game.play(4, 3, sente.stone.BLACK)
        game.play(3, 5, sente.stone.WHITE)

        game.play(4, 5, sente.stone.BLACK)
        game.play(3, 3, sente.stone.WHITE)

        # play away before taking the ko

        game.play(19, 19, sente.stone.BLACK)
        game.play(4, 4, sente.stone.WHITE)  # take the Ko

        # simulate a ko threat
        game.play(19, 1, sente.stone.BLACK)
        game.play(18, 1, sente.stone.WHITE)

        # the Ko should no longer be active
        self.assertTrue(game.is_legal(3, 4, sente.stone.BLACK))

        game.play(3, 4, sente.stone.BLACK)

        # it should now be illegal for white to play here
        self.assertFalse(game.is_legal(4, 4, sente.stone.WHITE))
        
    def test_zero_zero_illegal(self):
        """
        
        tests to see if playing on the zero zero point is illegal
        
        :return: 
        """
        
        game = sente.Game()
        
        self.assertFalse(game.is_legal(0, 0))


class IllegalMoveThrowsException(TestCase):
    """

    makes sure that making illegal moves throws an exception

    """

    def test_empty_correct_color(self):
        """

        tests to if the correct color is detected (assuming MakeMove does not work

        :return:
        """

        # create a 19x19 board
        game = sente.Game()

        with self.assertRaises(sente.exceptions.IllegalMoveException):
            game.play(3, 3, sente.stone.WHITE)

        with self.assertRaises(sente.exceptions.IllegalMoveException):
            game.play(15, 15, sente.stone.WHITE)

    def test_correct_color(self):
        """

        tests to see if the color is detected

        :return:
        """

        game = sente.Game()

        game.play(3, 3, sente.stone.BLACK)

        with self.assertRaises(sente.exceptions.IllegalMoveException):
            game.play(15, 3, sente.stone.BLACK)

        with self.assertRaises(sente.exceptions.IllegalMoveException):
            game.play(sente.Move(sente.stone.BLACK, 15, 3))

    def test_empty_out_of_bounds(self):
        """

        checks to see if out of bounds coords are illegal

        :return:
        """

        game = sente.Game()

        with self.assertRaises(sente.exceptions.IllegalMoveException):
            game.play(20, 19, sente.stone.BLACK)
            game.play(19, 20, sente.stone.BLACK)

        with self.assertRaises(sente.exceptions.IllegalMoveException):
            game.play(sente.Move(sente.stone.BLACK, 20, 19))
            game.play(sente.Move(sente.stone.BLACK, 19, 20))

    def test_occupied_space(self):
        """

        checks to see if playing on an occupied space is illegal

        :return:
        """

        game = sente.Game()

        game.play(2, 3, sente.stone.BLACK)
        game.play(15, 3, sente.stone.WHITE)

        with self.assertRaises(sente.exceptions.IllegalMoveException):
            game.play(2, 3, sente.stone.BLACK)
            game.play(15, 3, sente.stone.BLACK)

    def test_self_capture(self):
        """

        checks to see if a self-capture move is illegal

        :return:
        """

        for ruleset in (sente.rules.CHINESE, sente.rules.TROMP_TAYLOR):
            game = sente.Game(19, ruleset)

            game.play(1, 2, sente.stone.BLACK)
            game.play(19, 19, sente.stone.WHITE)

            game.play(2, 1, sente.stone.BLACK)

            if ruleset == sente.rules.CHINESE:
                with self.assertRaises(sente.exceptions.IllegalMoveException):
                    game.play(1, 1, sente.stone.WHITE)
            else:
                game.play(1, 1, sente.stone.WHITE)

                # Make sure the self-captured stone is actually captured
                self.assertTrue(game.get_board().get_stone(1, 1) == sente.stone.EMPTY)

    def test_group_self_capture(self):
        """

        checks to see self capture moves are illegal for groups of stones

        :return:
        """

        for ruleset in (sente.rules.CHINESE, sente.rules.TROMP_TAYLOR):
            game = sente.Game(19, ruleset)

            game.play(1, 3, sente.stone.BLACK)
            game.play(1, 2, sente.stone.WHITE)

            game.play(2, 3, sente.stone.BLACK)
            game.play(2, 2, sente.stone.WHITE)

            game.play(3, 2, sente.stone.BLACK)
            game.play(2, 1, sente.stone.WHITE)

            game.play(3, 1, sente.stone.BLACK)

            if ruleset == sente.rules.CHINESE:
                with self.assertRaises(sente.exceptions.IllegalMoveException):
                    game.play(1, 1)
            else:
                game.play(1, 1)

                # Make sure the self-captured stone is actually captured
                self.assertTrue(game.get_board().get_stone(1, 1) == sente.stone.EMPTY)

    def test_ko(self):
        """

        checks to see if the game correctly recognizes a Ko move as illegal

        :return:
        """

        game = sente.Game()

        game.play(2, 3, sente.stone.BLACK)
        game.play(3, 3, sente.stone.WHITE)

        game.play(4, 3, sente.stone.BLACK)
        game.play(1, 3, sente.stone.WHITE)

        game.play(3, 2, sente.stone.BLACK)
        game.play(2, 4, sente.stone.WHITE)

        game.play(3, 4, sente.stone.BLACK)
        game.play(2, 2, sente.stone.WHITE)

        # play away before taking the ko

        game.play(18, 18, sente.stone.BLACK)
        game.play(3, 3, sente.stone.WHITE)  # take the Ko

        with self.assertRaises(sente.exceptions.IllegalMoveException):
            game.play(2, 3, sente.stone.BLACK)

    def test_inactive_ko(self):
        """

        checks to see if a ko gos inactive after making a ko threat

        :return:
        """

        game = sente.Game()

        game.play(3, 4, sente.stone.BLACK)
        game.play(4, 4, sente.stone.WHITE)

        game.play(5, 4, sente.stone.BLACK)
        game.play(2, 4, sente.stone.WHITE)

        game.play(4, 3, sente.stone.BLACK)
        game.play(3, 5, sente.stone.WHITE)

        game.play(4, 5, sente.stone.BLACK)
        game.play(3, 3, sente.stone.WHITE)

        # play away before taking the ko

        game.play(19, 19, sente.stone.BLACK)
        game.play(4, 4, sente.stone.WHITE)  # take the Ko

        # simulate a ko threat
        game.play(19, 1, sente.stone.BLACK)
        game.play(18, 1, sente.stone.WHITE)

        # the Ko should no longer be active
        game.play(3, 4, sente.stone.BLACK)

        # it should now be illegal for white to play here
        with self.assertRaises(sente.exceptions.IllegalMoveException):
            game.play(4, 4, sente.stone.WHITE)

    def test_zero_zero_illegal(self):
        """
        
        tests to see if playing on the zero zero point is illegal
        
        :return: 
        """

        game = sente.Game()

        with self.assertRaises(sente.exceptions.IllegalMoveException):
            game.play(0, 0)
