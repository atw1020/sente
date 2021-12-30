"""

Author: Arthur Wesley

"""

from unittest import TestCase
from typing import Tuple, List

import sente
from sente import GTP


class CustomGTPCommands(TestCase):

    def test_basic_registration(self):
        """

        tests to see if commands can be registered

        :return:
        """

        session = GTP.Session("test", "0.0.1")

        @session.Command
        def octopus() -> Tuple[bool, str]:
            return True, "https://github.com/atw1020/sente/blob/master/tests/invalid%20sgf/octopus.jpeg"

        self.assertEqual("= " + octopus()[1] + "\n\n", session.interpret("test-octopus"))

    def test_greeting(self):
        """

        checks to see if the custom greeting command works correclty

        :return:
        """

        session = GTP.Session("test", "0.0.1")

        @session.Command
        def greeting() -> Tuple[bool, str]:
            return True, "hello, my name is" + session.name

        self.assertEqual("= " + greeting()[1] + "\n\n", session.interpret("test-greeting"))

    def test_integer_arg(self):
        """

        tests to see if ingeger arguments cna be passed

        :return:
        """

        session = GTP.Session("test", "0.0.1")

        @session.Command
        def int_arg(arg: int) -> Tuple[bool, str]:
            return True, str(arg)

        self.assertEqual("= 8\n\n", session.interpret("test-int_arg 8"))

    def test_vertex_arg(self):
        """

        tests to see if ingeger arguments cna be passed

        :return:
        """

        session = GTP.Session("test", "0.0.1")

        @session.Command
        def vertex_arg(arg: sente.Vertex) -> Tuple[bool, str]:
            return True, str(arg)

        self.assertEqual("= " + vertex_arg(sente.Vertex(3, 3))[1] + "\n\n", session.interpret("test-vertex_arg D4"))

    def test_echo(self):
        """

        tests to see if a custom echo command works correctly

        :return:
        """

        session = GTP.Session("test", "0.0.1")

        @session.Command
        def echo(text: str) -> Tuple[bool, str]:
            return True, text

        self.assertEqual("= " + echo("silence")[1] + "\n\n",
                         session.interpret("test-echo silence"))

    def test_multi_line_strings(self):
        """

        tests to see if echo works with multi line strings

        :return:
        """

        session = GTP.Session("test", "0.0.1")

        @session.Command
        def echo(text: str) -> Tuple[bool, str]:
            return True, text

        self.assertEqual("= this is two lines...\nof a string!\n\n",
                         session.interpret("test-echo \"this is two lines...\nof a string!\""))

    def test_color_arg(self):
        """

        tests to see if colors can be passed correctly

        :return:
        """

        session = GTP.Session("test", "0.0.1")

        @session.Command
        def color_arg(arg: sente.stone) -> Tuple[bool, str]:
            return True, str(arg)

        self.assertEqual("= " + color_arg(sente.BLACK)[1] + "\n\n",
                         session.interpret("test-color_arg BLACK"))

    def test_float_arg(self):
        """

        tests to see if floats can be passed correctly

        :return:
        """

        session = GTP.Session("test", "0.0.1")

        @session.Command
        def float_arg(arg: float) -> Tuple[bool, str]:
            return True, str(arg)

        self.assertEqual("= 3.140000104904175\n\n",
                         session.interpret("test-float_arg 3.14"))

    def test_move_arg(self):
        """

        tests to see if colors can be passed correctly

        :return:
        """

        session = GTP.Session("test", "0.0.1")

        @session.Command
        def move_arg(arg: sente.Move) -> Tuple[bool, str]:
            return True, str(arg)

        self.assertEqual("= " + move_arg(sente.Move(3, 3, sente.stone.BLACK))[1] + "\n\n",
                         session.interpret("test-move_arg BLACK D4"))

    def test_bool_arg(self):
        """

        tests to see if colors can be passed correctly

        :return:
        """

        session = GTP.Session("test", "0.0.1")

        @session.Command
        def bool_arg(arg: bool) -> Tuple[bool, str]:
            return True, str(arg)

        self.assertEqual("= " + bool_arg(True)[1] + "\n\n",
                         session.interpret("test-bool_arg true"))

    def test_return_integer(self):
        """

        checks to see if returning a raw integer is acceptable

        :return:
        """

        session = GTP.Session("test", "0.0.1")

        @session.Command
        def return_integer() -> int:
            return 5

        self.assertEqual("= 5\n\n", session.interpret("test-return_integer"))

    def test_return_vertex(self):
        """

        checks to see if returning a vertex is acceptable

        :return:
        """

        session = GTP.Session("test", "0.0.1")

        @session.Command
        def return_vertex() -> sente.Vertex:
            return sente.Vertex(4, 4)

        self.assertEqual("= E5\n\n", session.interpret("test-return_vertex"))

    def test_return_raw_string(self):
        """

        checks to see if returning a string is acceptable

        :return:
        """

        session = GTP.Session("test", "0.0.1")

        @session.Command
        def return_raw_string() -> str:
            return "hello hello (Hola!)"

        self.assertEqual("= hello hello (Hola!)\n\n", session.interpret("test-return_raw_string"))

    def test_return_color(self):
        """

        checks to see if returning a color is acceptable

        :return:
        """

        session = GTP.Session("test", "0.0.1")

        @session.Command
        def return_color() -> sente.stone:
            return sente.stone.BLACK

        self.assertEqual("= B\n\n", session.interpret("test-return_color"))

    def test_return_float(self):
        """

        checks to see if returning a float is acceptable

        :return:
        """

        session = GTP.Session("test", "0.0.1")

        @session.Command
        def return_float() -> float:
            return 3.14

        self.assertEqual("= 3.14\n\n", session.interpret("test-return_float"))

    def test_return_move(self):
        """

        checks to see if returning a move is acceptable

        :return:
        """

        session = GTP.Session("test", "0.0.1")

        @session.Command
        def return_move() -> sente.Move:
            return sente.Move(4, 4, sente.stone.BLACK)

        self.assertEqual("= B E5\n\n", session.interpret("test-return_move"))

    def test_return_bool(self):
        """

        checks to see if returning a raw integer is acceptable

        :return:
        """

        session = GTP.Session("test", "0.0.1")

        @session.Command
        def return_bool() -> bool:
            return False

        self.assertEqual("= false\n\n", session.interpret("test-return_bool"))

    def test_gen_move_override(self):
        """

        checks to see if we can override the genmove function

        :return:
        """

        session = GTP.Session("test", "0.0.1")

        @session.GenMove
        def genmove(stone: sente.stone) -> sente.Vertex:
            return sente.Vertex(4, 4)

        # check that the move is generated
        self.assertEqual("= E5\n\n", session.interpret("genmove B"))

    def test_gen_move_plays_move(self):
        """

        tests to see if the genmove command plays the move that it generates

        :return:
        """

        session = GTP.Session("test", "0.0.1")

        @session.GenMove
        def genmove(stone: sente.stone) -> sente.Vertex:
            return sente.Vertex(4, 4)

        session.interpret("boardsize 9")
        session.interpret("genmove B")

        self.assertEqual("= \n"
                         " 9  .  .  .  .  .  .  .  .  .\n"
                         " 8  .  .  .  .  .  .  .  .  .\n"
                         " 7  .  .  *  .  .  .  *  .  .\n"
                         " 6  .  .  .  .  .  .  .  .  .\n"
                         " 5  .  .  .  .  X  .  .  .  .\n"
                         " 4  .  .  .  .  .  .  .  .  .\n"
                         " 3  .  .  *  .  .  .  *  .  .\n"
                         " 2  .  .  .  .  .  .  .  .  .\n"
                         " 1  .  .  .  .  .  .  .  .  .\n"
                         "    A  B  C  D  E  F  G  H  J\n\n", session.interpret("showboard"))


class InterpreterSyntaxChecking(TestCase):

    def test_echo_wrong_arguments(self):
        """

        makes sure that the echo command recognizes when an invalid argument type is passed to it

        :return:
        """

        session = GTP.Session("test", "0.0.1")

        @session.Command
        def echo(text: str) -> Tuple[bool, str]:
            return True, text

        self.assertEqual("? no viable argument pattern for command \"test-echo\"; "
                         "candidate pattern not valid: expected string in position 1, got integer\n\n",
                         session.interpret("test-echo 53"))

    def test_echo_wrong_number_arguments(self):
        """

        makes sure that the echo command recognizes when it gets the wrong number of arguments

        :return:
        """

        session = GTP.Session("test", "0.0.1")

        @session.Command
        def echo(text: str) -> Tuple[bool, str]:
            return True, text

        self.assertEqual("? invalid number of arguments for command \"test-echo\"; expected 1, got 5\n\n",
                         session.interpret("test-echo in the world of silence"))

    def test_letter_i_skipped_vertex(self):
        """

        makes sure that vertex responses can't

        :return:
        """

        session = GTP.Session("test", "0.0.1")

        @session.Command
        def letter_i_skipped_vertex() -> sente.Vertex:
            return sente.Vertex(8, 8)

        self.assertEqual("= J9\n\n", session.interpret("test-letter_i_skipped_vertex"))

    def test_letter_i_skipped_move(self):
        """

        makes sure that move responses skip the letter i

        :return:
        """

        session = GTP.Session("test", "0.0.1")

        @session.Command
        def letter_i_skipped_move() -> sente.Move:
            return sente.Move(8, 8, sente.stone.BLACK)

        self.assertEqual("= B J9\n\n", session.interpret("test-letter_i_skipped_move"))


def my_message(arg: str):
    return True, "this isn't a method, so we can't register it!" + arg


class TestInvalidRegistration(TestCase):

    def test_unlabeled_arguments(self):
        """

        makes sure that unlabeled arguments are caught

        :return:
        """

        session = GTP.Session("test", "0.0.1")

        with self.assertRaises(ValueError):

            @session.Command
            def unlabeled_arguments(arg) -> Tuple[bool, str]:
                return True, "you shouldn't be able to use this!" + arg

    def test_bad_input_type(self):
        """



        :return:
        """

        session = GTP.Session("test", "0.0.1")

        with self.assertRaises(TypeError):

            @session.Command
            def dictionaries_argument(arg: dict) -> Tuple[bool, str]:
                return False, "dictionaries are invalid arguments"

    def test_non_tuple_response(self):
        """

        checks to make sure that methods that return non-tuples error out when called

        :return:
        """

        session = GTP.Session("test", "0.0.1")

        with self.assertRaises(TypeError):

            @session.Command
            def non_tuple_response() -> List[bool, str]:
                return [False, "lists aren't acceptable"]

    def test_bad_tuple_response(self):
        """

        checks to see if a tuple that does not contain two elements raises an exception

        :return:
        """

        session = GTP.Session("test", "0.0.1")

        with self.assertRaises(ValueError):

            @session.Command
            def wrong_number_responses() -> Tuple[int, int, int]:
                return 1, 2, 3

    def test_wrong_tuple_first_element(self):
        """

        checks to make sure that the sente detects that a response has the wrong type in the first element of it's tuple

        :return:
        """

        session = GTP.Session("test", "0.0.1")

        with self.assertRaises(TypeError):

            @session.Command
            def test_tuple_first_item_wrong() -> Tuple[str, str]:
                return "false", "not real bool!"

    def test_wrong_tuple_second_element(self):
        """

        checks to make sure that the sente detects that a response has the wrong type in the second element of it's tuple

        :return:
        """

        session = GTP.Session("test", "0.0.1")

        with self.assertRaises(TypeError):

            @session.Command
            def test_tuple_second_item_wrong() -> Tuple[bool, list]:
                return True, []

    def test_point_followed_by_color_illegal(self):
        """

        makes sure that sente detects when we have an intersection followed by a color

        :return:
        """

        session = GTP.Session("test", "0.0.1")

        with self.assertRaises(ValueError):

            @session.Command
            def color_followed_by_intersection(color: sente.stone, point: sente.Vertex) -> Tuple[bool, str]:
                return True, "this should be compressed to a move object"

    def test_wrong_response_type(self):
        """

        checks to see if the functions with incorrectly labeled response types are caught

        :return:
        """

        session = GTP.Session("test", "0.0.0")

        @session.Command
        def wrong_response_type() -> str:
            return None

        with self.assertRaises(TypeError):
            session.interpret("test-wrong_response_type")

    def test_wrong_response_type_genmove(self):
        """

        checks to see if the genmove commands with incorrectly labeled response types are caught

        :return:
        """

        session = GTP.Session("test", "0.0.0")

        @session.GenMove
        def genmove(stone: sente.stone) -> sente.Vertex:
            return None

        with self.assertRaises(TypeError):
            print(session.interpret("genmove B"))
