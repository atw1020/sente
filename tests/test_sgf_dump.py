"""

Author: Arthur Wesley

"""

from unittest import TestCase

import sente
from sente import sgf


class TestBasics(TestCase):

    def test_single_move(self):
        """

        tests the serialization of a single move

        :return:
        """

        game = sente.Game()

        game.play(4, 4)

        self.assertEqual("(;FF[4]\nSZ[19]\nRU[Chinese]\n;B[dd])", sgf.dumps(game))

    def test_multiple_moves(self):
        """

        tests to see if we can dump a sequence of mvoes

        :return:
        """

        game = sente.Game()

        game.play(4, 4)
        game.play(16, 4)

        game.play(4, 16)
        game.play(16, 16)

        self.assertEqual("(;FF[4]\nSZ[19]\nRU[Chinese]\n;B[dd]\n(;W[pd]\n(;B[dp]\n(;W[pp]))))", sgf.dumps(game))

    def test_multiple_branches(self):
        """

        tests to see if we can dump multiple branches of moves

        :return:
        """

        game = sente.Game()

        game.play(4, 4)
        game.play(16, 4)

        game.step_up()

        game.play(4, 16)

        self.assertEqual("(;FF[4]\nSZ[19]\nRU[Chinese]\n;B[dd]\n(;W[pd];W[dp]))", sgf.dumps(game))

    def test_resigned_game(self):
        """

        tests to see if the game records resignation

        :return:
        """

        game = sente.Game()

        game.resign()

        self.assertEqual("(;FF[4]\nRE[W+R]\nSZ[19]\nRU[Chinese])", sgf.dumps(game))

    def test_dumps_loads(self):
        """

        tests to see if a dumps followed by a loads generates the same board

        :return:
        """

        game = sente.Game()

        game.play(4, 4)
        game.play(16, 4)

        game.play(4, 16)
        game.play(16, 16)

        serial = sgf.dumps(game)

        new_game = sgf.loads(serial)
        new_game.play_default_sequence()

        print(game)
        print(new_game)

        self.assertEqual(sente.BLACK, new_game.get_point(4, 4))
        self.assertEqual(sente.WHITE, new_game.get_point(16, 4))
        self.assertEqual(sente.BLACK, new_game.get_point(4, 16))
        self.assertEqual(sente.WHITE, new_game.get_point(16, 16))

