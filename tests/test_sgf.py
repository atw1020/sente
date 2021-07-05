"""

Author: Arthur Wesley

"""

import os
from sente import *

from assert_does_not_raise import DoesNotRaiseTestCase


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

    def test_throws_invalid_sgf(self):
        """

        tests to see if the program throws an exception if an invalid SGF is provided

        :return:
        """

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

        expected_game = Board19([[_, _, _, _, _, _, B, W, W, W, W, W, W, B, _, _, _, _, _],
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

        print(expected_game)
        print(game)

        self.assertEqual(str(expected_game), str(game))
        self.assertEqual(expected_game, game.get_board())

    def test_simple_branched_sgf(self):
        """

        tests to see if the SGF reader works for simple files

        :return:
        """

    def test_complex_branched_sgf(self):
        """

        tests to see if the SGF parser can handle an SGF file with multiple branches

        :return:
        """

        game = sgf.load("sgf/3-4.sgf")  # a 3-4 joseki refrence with lots of branches

        print(len(game.get_branches()))

        game.play_default_branch()
        print(game)

        game.advance_to_root()

        self.assertEqual([Move(3, 16, stone.BLACK)], game.get_branches())
        game.play(3, 16)

        self.assertEqual([Move(3, 14, stone.WHITE), Move(2, 14, stone.WHITE)], game.get_branches())
