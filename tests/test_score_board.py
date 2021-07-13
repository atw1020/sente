"""

Author: Arthur Wesley

"""

from unittest import TestCase

import sente


class TestNoDeadStones(TestCase):

    def play_simple_game(self, game):
        """

        plays out a simple game in a cordner

        :return:
        """

        game.play(3, 1)
        game.play(1, 3)

        game.play(3, 2)
        game.play(2, 3)

        game.play(4, 3)
        game.play(3, 4)

        game.play(5, 3)
        game.play(3, 5)

        game.play(6, 2)
        game.play(2, 6)

        game.play(6, 1)
        game.play(1, 6)

        return game

    def test_simple_chinese(self):
        """



        :return:
        """

        game = self.play_simple_game(sente.Game(19, sente.CHINESE))
        result = game.score()

        self.assertEqual(sente.stone.WHITE, game.get_winner())
        self.assertEqual(10, result.get_black_score())
        self.assertEqual(17.5, result.get_white_score())

    def test_simple_japanese(self):
        """



        :return:
        """

        game = self.play_simple_game(sente.Game(19, sente.JAPANESE))
        result = game.score()

        self.assertEqual(sente.stone.WHITE, game.get_winner())
        self.assertEqual(4, result.get_black_score())
        self.assertEqual(10.5, result.get_white_score())
