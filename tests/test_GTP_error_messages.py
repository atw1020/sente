"""

Author: Arthur Wesley

"""

from unittest import TestCase

from sente import GTP


class ErrorMessages(TestCase):

    def test_wrong_argument_count(self):
        """

        tests to see if the interpreter recognizes an incorrect number of arguments

        :return:
        """

        engine = GTP.Session()

        self.assertEqual("? invalid number of arguments for command \"play\"; expected 1, got 3\n",
                         engine.interpret("play BLACK 4 4"))
        self.assertEqual("? invalid number of arguments for command \"known_command\"; expected 1, got 2\n",
                         engine.interpret("known_command play B[dd]"))
        self.assertEqual("? invalid number of arguments for command \"loadsgf\"; expected 1 or 2, got 4\n",
                         engine.interpret("loadsgf tests/sgf/Lee Sedol ladder game.sgf"))
        self.assertEqual("? invalid number of arguments for command \"showboard\"; expected 0, got 1\n",
                         engine.interpret("showboard twice"))

    def test_invalid_argument_type(self):
        """

        checks to see that the interpreter recognizes arguments of an incorrect type

        :return:
        """

        engine = GTP.Session()

        self.assertEqual("? no viable argument pattern for command \"loadsgf\"; "
                         "candidate pattern not valid: expected integer in position 2, got string\n",
                         engine.interpret("loadsgf tests/sgf/ff4_ex.sgf one"))
        self.assertEqual("? no viable argument pattern for command \"play\"; "
                         "candidate pattern not valid: expected move in position 1, got string\n",
                         engine.interpret("play B[dd]"))
