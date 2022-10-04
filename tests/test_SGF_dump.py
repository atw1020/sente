"""

Author: Arthur Wesley

"""

import random
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

        self.assertEqual(";B[dd];W[pd];B[dp];W[pp])", serialized[-25:])

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

        self.assertEqual("(;FF[4]SZ[19]RU[Chinese]\n;B[dd]\n(;W[pd])\n(;W[dp]))", serialized)

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

    def test_FF1_not_produced(self):
        """

        tests to see if a game in File format 1 does not add the parameter "FF[1]"

        :return:
        """

        game = sgf.load("tests/sgf/Lee Sedol ladder game.sgf")

        self.assertNotIn("FF", sgf.dumps(game))

    def test_backslashes_in_comments(self):
        """

        tests to see if backslashes automatically added to comments with brackets in them

        :return:
        """

        game = sente.Game()
        game.comment = "brackets! []"

        self.assertIn("C[brackets! [\\]]", sgf.dumps(game))

    def test_set_property_brackets(self):
        """

        tests to make sure that close

        :return:
        """

        game = sente.Game()
        game.set_property("C", "brackets! []")

        self.assertIn("C[brackets! [\\]", sgf.dumps(game))

    def test_access_bracket_backslash_non_destructive(self):
        """

        makes sure that if SGF puts a backslash into a SGF field

        :return:
        """

        game = sente.Game()
        game.set_property("C", "[]")

        self.assertEqual(game.comment, "[]")

        self.assertIn("[\\]", sgf.dumps(game))

    def test_load_dumps_backslashes(self):
        """

        tests to see if we can load and then dump backslashes

        :return:
        """

        game = sgf.load("tests/sgf/backslash at end of comment.sgf")

        self.assertIn("C[backslashes! \\\\]", sgf.dumps(game))

    def test_dump_double_pass(self):
        """

        makes sure that the dumps can handle games that end with double passes

        :return:
        """

        game = sente.Game()

        game.play(4, 4)
        game.play(15, 4)
        game.play(4, 15)

        game.pss()
        game.pss()

        # dump and load the game
        text = sgf.dumps(game)
        game = sgf.loads(text)

        last_two_moves = game.get_default_sequence()[-2:]


class SettingStones(TestCase):

    def test_set_black_black(self):
        """

        makes sure that adding a stone and then removing it leaves no record

        :return:
        """

        game = sente.Game()
        game.set_points([sente.Move(4, 4, sente.stone.BLACK)])
        self.assertIn("AB", sgf.dumps(game))
        game.set_points([sente.Move(4, 4, sente.stone.BLACK)])

        self.assertNotIn("AE", sgf.dumps(game))
        self.assertNotIn("AW", sgf.dumps(game))
        self.assertIn("AB", sgf.dumps(game))

    def test_set_black_white(self):
        """

        makes sure that adding a stone and then removing it leaves no record

        :return:
        """

        game = sente.Game()
        game.set_points([sente.Move(4, 4, sente.stone.BLACK)])
        self.assertIn("AB", sgf.dumps(game))
        game.set_points([sente.Move(4, 4, sente.stone.WHITE)])

        self.assertNotIn("AE", sgf.dumps(game))

    def test_set_black_empty(self):
        """

        makes sure that adding a stone and then removing it leaves no record

        :return:
        """

        game = sente.Game()
        game.set_points([sente.Move(4, 4, sente.stone.BLACK)])
        self.assertIn("AB", sgf.dumps(game))
        game.set_points([sente.Move(4, 4, sente.stone.EMPTY)])

        self.assertNotIn("AB", sgf.dumps(game))
        self.assertNotIn("AE", sgf.dumps(game))
        self.assertNotIn("AW", sgf.dumps(game))

    def test_set_white_black(self):
        """

        makes sure that adding a stone and then removing it leaves no record

        :return:
        """

        game = sente.Game()
        game.set_points([sente.Move(4, 4, sente.stone.WHITE)])
        self.assertIn("AW", sgf.dumps(game))
        game.set_points([sente.Move(4, 4, sente.stone.BLACK)])

        self.assertNotIn("AE", sgf.dumps(game))
        self.assertNotIn("AW", sgf.dumps(game))

    def test_set_white_white(self):
        """

        makes sure that adding a stone and then removing it leaves no record

        :return:
        """

        game = sente.Game()
        game.set_points([sente.Move(4, 4, sente.stone.WHITE)])
        self.assertIn("AW", sgf.dumps(game))
        game.set_points([sente.Move(4, 4, sente.stone.WHITE)])

        self.assertNotIn("AE", sgf.dumps(game))
        self.assertNotIn("AB", sgf.dumps(game))
        self.assertIn("AW", sgf.dumps(game))

    def test_set_white_empty(self):
        """

        makes sure that adding a stone and then removing it leaves no record

        :return:
        """

        game = sente.Game()
        game.set_points([sente.Move(4, 4, sente.stone.WHITE)])
        self.assertIn("AW", sgf.dumps(game))
        game.set_points([sente.Move(4, 4, sente.stone.EMPTY)])

        self.assertNotIn("AB", sgf.dumps(game))
        self.assertNotIn("AE", sgf.dumps(game))
        self.assertNotIn("AW", sgf.dumps(game))

    def test_set_empty_black(self):
        """

        makes sure that adding a stone and then removing it leaves no record

        :return:
        """

        game = sente.Game()
        game.set_points([sente.Move(4, 4, sente.stone.EMPTY)])
        self.assertNotIn("AE", sgf.dumps(game))
        game.set_points([sente.Move(4, 4, sente.stone.BLACK)])

        self.assertNotIn("AE", sgf.dumps(game))
        self.assertNotIn("AW", sgf.dumps(game))

    def test_set_empty_white(self):
        """

        makes sure that adding a stone and then removing it leaves no record

        :return:
        """

        game = sente.Game()
        game.set_points([sente.Move(4, 4, sente.stone.EMPTY)])
        self.assertNotIn("AE", sgf.dumps(game))
        game.set_points([sente.Move(4, 4, sente.stone.WHITE)])

        self.assertNotIn("AE", sgf.dumps(game))
        self.assertNotIn("AB", sgf.dumps(game))

    def test_set_empty_empty(self):
        """

        makes sure that adding a stone and then removing it leaves no record

        :return:
        """

        game = sente.Game()
        game.set_points([sente.Move(4, 4, sente.stone.EMPTY)])
        self.assertNotIn("AE", sgf.dumps(game))
        game.set_points([sente.Move(4, 4, sente.stone.EMPTY)])

        self.assertNotIn("AB", sgf.dumps(game))
        self.assertNotIn("AE", sgf.dumps(game))
        self.assertNotIn("AW", sgf.dumps(game))
