"""

Author: Arthur Wesley

"""

from unittest import TestCase

import sente
from sente import sgf


class LoadMetadata(TestCase):

    def test_basic_metadata(self):
        """



        :return:
        """

        game = sgf.load("sgf/0.5 Komi.sgf")

        correct = {
            "FF": "4",
            "CA": "UTF-8",
            "GM": "1",
            "DT": "2021-06-27",
            "PC": "OGS: https://online-go.com/game/34839594",
            "GN": "Friendly Match",
            "PB": "noob_bot_3",
            "PW": "IDW64",
            "BR": "32k",
            "WR": "6k",
            "TM": "259200",
            'C':  "noob_bot_3: Hi! This is bot. Join 'noob_bot' group and have fun! Undo "
                  "will be accepted. You can send undo message if you need.\n",
            "OT": "86400 fischer",
            "RE": "W+368.5",
            "SZ": "19",
            "KM": "0.5",
            "RU": "Chinese"
        }

        self.assertEqual(correct, game.get_properties())

    def test_remove_labels(self):
        """



        :return:
        """

        game = sgf.load("sgf/3-4.sgf")

        correct = {
            "FF": "4",
            "GM": "1",
            "CA": "UTF-8",
            "AP": "CGoban:3",
            "ST": "2",
            "RU": "Japanese",
            "SZ": "19",
            "KM": "0.00",
            "PW": "White",
            "PB": "Black",
        }

        self .assertEqual(correct, game.get_properties())

    def test_metadata_list(self):
        """

        tests to make sure that metadata may be in list format

        :return:
        """

        game = sgf.load("sgf/metadata list.sgf")
        metadata = game.get_properties()

        self.assertEqual(["dd", "qd", "dq", "pp"], metadata["TR"])

    def test_added_stones_are_not_metadata(self):
        """

        tests to see if the get_properties() function ignores "AW" and "AB" parameters

        :return:
        """

        game = sgf.load("sgf/multiple stones at once.sgf")

        self.assertNotIn("AB", game.get_properties())
        self.assertNotIn("AW", game.get_properties())

    def test_load_backslashes(self):
        """

        tests to see if backslashes are properly loaded into comments

        :return:
        """

        game = sgf.load("sgf/backslash at end of comment.sgf")

        self.assertEqual("C[backslashes! \\]", game.comment)


class StoreMetadata(TestCase):

    def test_set_property_inline(self):
        """

        tests to see if attributes can be successfully added to the SGF

        :return:
        """

        game = sente.Game()

        game.set_property("BR", "8k")
        game.set_property("WR", "7k")

        serialized = sgf.dumps(game)

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

        with self.assertRaises(sente.exceptions.InvalidSGFException):
            game.set_property("this is invalid", "video killed the radio star")

    def test_override_params(self):
        """

        tests to see if Rules can be overridden

        :return:
        """

        game = sente.Game()
        game.set_property("RU", "Japanese")

        serialized = sgf.dumps(game)

        self.assertIn("RU[Japanese]", serialized)

    def test_override_board_size(self):
        """

        tests ensures that we can't just override the board size

        :return:
        """

        game = sente.Game()

        with self.assertRaises(ValueError):
            game.set_property("SZ", "13")

    def test_default_params_ignored(self):
        """

        tests to see if the sgf.dump ignores default parameters

        :return:
        """

        game = sente.Game()

        game.set_property("FF", 3)

        serialized = sgf.dumps(game)[22:]

        self.assertNotIn("FF[3]", serialized)
        self.assertNotIn("GM[2]", serialized)
        self.assertNotIn("CA[UTF-8]", serialized)

    def test_adding_unsupported_FF(self):
        """

        tests to see if adding an unsupported command for a particular version

        :return:
        """

        game = sente.Game()

        with self.assertRaises(sente.exceptions.InvalidSGFException):
            game.set_property("EX", "bb")



