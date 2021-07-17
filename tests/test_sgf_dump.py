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

        serialized = sgf.dumps(game)

        self.assertEqual("(;FF[4]", serialized[:7])
        self.assertIn("SZ[19]", serialized)
        self.assertIn("RU[Chinese]", serialized)

        self.assertEqual(";B[dd])", serialized[-7:])

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

        serialized = sgf.dumps(game)

        self.assertEqual("(;FF[4]", serialized[:7])
        self.assertIn("SZ[19]", serialized)
        self.assertIn("RU[Chinese]", serialized)

        self.assertEqual(";B[dd]\n(;W[dp]\n(;B[pd]\n(;W[pp]))))", serialized[-34:])

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

        serialized = sgf.dumps(game)

        self.assertEqual("(;FF[4]", serialized[:7])
        self.assertIn("SZ[19]", serialized)
        self.assertIn("RU[Chinese]", serialized)

        self.assertEqual(";B[dd]\n(;W[dp];W[pd]))", serialized[-22:])

    def test_resigned_game(self):
        """

        tests to see if the game records resignation

        :return:
        """

        game = sente.Game()

        game.resign()

        serialized = sgf.dumps(game)

        self.assertEqual("(;FF[4]", serialized[:7])
        self.assertIn("RE[W+R]", serialized)
        self.assertIn("SZ[19]", serialized)
        self.assertIn("RU[Chinese]", serialized)

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

        self.assertEqual(sente.BLACK, new_game.get_point(4, 4))
        self.assertEqual(sente.WHITE, new_game.get_point(16, 4))
        self.assertEqual(sente.BLACK, new_game.get_point(4, 16))
        self.assertEqual(sente.WHITE, new_game.get_point(16, 16))

    def test_attribute(self):
        """

        tests to see if attributes can be successfully added to the SGF

        :return:
        """

        game = sente.Game()

        params = {
            "BR": "8k",
            "WR": "7k"
        }

        serialized = sgf.dumps(game, params)

        self.assertEqual("(;FF[4]", serialized[:7])
        self.assertIn("SZ[19]", serialized)
        self.assertIn("RU[Chinese]", serialized)
        self.assertIn("BR[8k]", serialized)
        self.assertIn("WR[7k]", serialized)

    def test_invalid_attribute(self):
        """

        tests to see if passing an invalid attribute results in an exception

        :return:
        """

        game = sente.Game()

        params = {
            "this is invalid": "video killed the radio star"
        }

        with self.assertRaises(ValueError):
            sgf.dumps(game, params)

    def test_override_params(self):
        """

        tests to see if Rules can be overridden

        :return:
        """

        game = sente.Game()

        params = {
            "RU": "Japanese"
        }

        serialized = sgf.dumps(game, params)

        self.assertIn("RU[Japanese]", serialized)

    def test_override_board_size(self):
        """

        tests ensures that we can't just override the board size

        :return:
        """

        game = sente.Game()

        params = {
            "SZ": "19"
        }

        with self.assertRaises(ValueError):
            sgf.dumps(game, params)

