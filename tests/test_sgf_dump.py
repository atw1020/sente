"""

Author: Arthur Wesley

"""

import sente
from sente import sgf
from assert_does_not_raise import DoesNotRaiseTestCase


class TestBasics(DoesNotRaiseTestCase):

    def test_single_move(self):
        """

        tests the serialization of a single move

        :return:
        """

        game = sente.Game()

        game.play(4, 4)

        serialized = sgf.dumps(game)

        self.assertIn("B[dd]", serialized)

    def test_default_parameters(self):
        """

        tests to see if the game adds the default parameters

        :return:
        """

        game = sente.Game()

        game.play(4, 4)

        serialized = sgf.dumps(game)

        self.assertIn("FF[4]", serialized)
        self.assertIn("SZ[19]", serialized)
        self.assertIn("RU[Chinese]", serialized)

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

        self.assertEqual(";B[dd];W[dp];B[pd];W[pp])", serialized[-25:])

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

        self.assertEqual("(;FF[4]SZ[19]RU[Chinese]\n;B[dd]\n(;W[dp])\n(;W[pd]))", serialized)

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

    def test_dump_complex_game(self):
        """

        tests to see if we can dump a game with lots and lots of variations

        :return:
        """
        game = sgf.load("sgf/3-4.sgf")

        with self.assertDoesNotRaise(ValueError):
            sgf.dumps(game)

    def test_FF1_not_produced(self):
        """

        tests to see if a game in File format 1 does not add the parameter "FF[1]"

        :return:
        """

        game = sgf.load("sgf/Lee Sedol ladder game.sgf")

        self.assertNotIn("FF", sgf.dumps(game))
