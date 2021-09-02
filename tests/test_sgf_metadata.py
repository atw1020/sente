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

        self.assertEqual(correct, game.metadata)

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

        self .assertEqual(correct, game.metadata)

    def test_added_stones_are_not_metadata(self):
        """

        tests to see if the get_metadata() function ignores "AW" and "AB" parameters

        :return:
        """

        game = sgf.load("sgf/multiple stones at once.sgf")

        self.assertNotIn("AB", game.metadata)
        self.assertNotIn("AW", game.metadata)


class StoreMetadata(TestCase):

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

    def test_default_params_ignored(self):
        """

        tests to see if the sgf.dump ignores default parameters

        :return:
        """

        game = sente.Game()

        params = {
            "FF": "3",
            "GM": "2",
            "CA": "UTF-16"
        }
        game.add_metadata("FF", 3)

        serialized = sgf.dumps(game)[22:]

        self.assertNotIn("FF[3]", serialized)
        self.assertNotIn("GM[2]", serialized)
        self.assertNotIn("CA[UTF-8]", serialized)



