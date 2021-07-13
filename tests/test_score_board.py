"""

Author: Arthur Wesley

"""

from unittest import TestCase

import sente


class TestNoDeadStones(TestCase):

    def play_simple_game(self):
        """

        plays out a simple game in a cordner

        :return:
        """

        game = sente.Game()

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

        game = self.play_simple_game()

        print(game.score())
