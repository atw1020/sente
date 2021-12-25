"""

Author: Arthur Wesley

"""

from unittest import TestCase

import sente
from sente import gtp


class CustomGTPTester(gtp.Engine):

    def __init__(self):
        super().__init__("test")

        # register commands
        self.register_command(self.octopus)
        self.register_command(self.greeting)

        # register acceptable input types
        self.register_command(self.int_arg)
        self.register_command(self.vertex_arg)
        self.register_command(self.echo)
        self.register_command(self.color_arg)
        self.register_command(self.float_arg)
        self.register_command(self.move_arg)
        self.register_command(self.bool_arg)

        # register acceptable return types
        self.register_command(self.return_integer)
        self.register_command(self.return_vertex)
        self.register_command(self.return_raw_string)
        self.register_command(self.return_color)
        self.register_command(self.return_float)
        self.register_command(self.return_move)
        self.register_command(self.return_bool)

        # register genmove
        self.register_command(self.genmove)

    def echo(self, text: str):
        return True, text

    def octopus(self):
        return True, "https://github.com/atw1020/sente/blob/master/tests/invalid%20sgf/octopus.jpeg"

    def greeting(self):
        return True, "hello, my name is" + self.name

    def int_arg(self, arg: int):
        return True, str(arg)

    def vertex_arg(self, arg: sente.Vertex):
        return True, str(arg)

    def color_arg(self, arg: sente.stone):
        return True, str(arg)

    def float_arg(self, arg: float):
        return True, str(arg)

    def move_arg(self, arg: sente.Move):
        return True, str(arg)

    def bool_arg(self, arg: bool):
        return True, str(arg)

    def return_integer(self):
        return 5

    def return_vertex(self):
        return sente.Vertex(4, 4)

    def return_raw_string(self):
        return "hello hello (Hola!)"

    def return_color(self):
        return sente.stone.BLACK

    def return_float(self):
        return 3.14

    def return_move(self):
        return sente.Move(4, 4, sente.stone.BLACK)

    def return_bool(self):
        return False

    def genmove(self, stone: sente.stone):
        return sente.Move(4, 4, sente.stone.BLACK)


class CustomGTPCommands(TestCase):

    def test_basic_registration(self):
        """

        tests to see if commands can be registered

        :return:
        """

        engine = CustomGTPTester()

        self.assertEqual("= " + engine.octopus()[1] + "\n\n", engine.interpret("test-octopus"))

    def test_greeting(self):
        """

        checks to see if the custom greeting command works correclty

        :return:
        """

        engine = CustomGTPTester()

        self.assertEqual("= " + engine.greeting()[1] + "\n\n", engine.interpret("test-greeting"))

    def test_integer_arg(self):
        """

        tests to see if ingeger arguments cna be passed

        :return:
        """

        engine = CustomGTPTester()

        self.assertEqual("= 8\n\n", engine.interpret("test-int_arg 8"))

    def test_vertex_arg(self):
        """

        tests to see if ingeger arguments cna be passed

        :return:
        """

        engine = CustomGTPTester()

        self.assertEqual("= (3, 3)\n\n", engine.interpret("test-vertex_arg D4"))

    def test_echo(self):
        """

        tests to see if a custom echo command works correctly

        :return:
        """

        engine = CustomGTPTester()

        self.assertEqual("= " + engine.echo("silence")[1] + "\n\n",
                         engine.interpret("test-echo silence"))

    def test_multi_line_strings(self):
        """

        tests to see if echo works with multi line strings

        :return:
        """

        engine = CustomGTPTester()

        self.assertEqual("= this is two lines...\nof a string!\n\n",
                         engine.interpret("test-echo \"this is two lines...\nof a string!\""))

    def test_color_arg(self):
        """

        tests to see if colors can be passed correctly

        :return:
        """

        engine = CustomGTPTester()

        self.assertEqual("= " + engine.color_arg(sente.BLACK)[1] + "\n\n",
                         engine.interpret("test-color_arg BLACK"))

    def test_float_arg(self):
        """

        tests to see if floats can be passed correctly

        :return:
        """

        engine = CustomGTPTester()

        self.assertEqual("= 3.140000104904175\n\n",
                         engine.interpret("test-float_arg 3.14"))

    def test_move_arg(self):
        """

        tests to see if colors can be passed correctly

        :return:
        """

        engine = CustomGTPTester()

        self.assertEqual("= " + engine.move_arg(sente.Move(3, 3, sente.stone.BLACK))[1] + "\n\n",
                         engine.interpret("test-move_arg BLACK D4"))

    def test_bool_arg(self):
        """

        tests to see if colors can be passed correctly

        :return:
        """

        engine = CustomGTPTester()

        self.assertEqual("= " + engine.bool_arg(True)[1] + "\n\n",
                         engine.interpret("test-bool_arg true"))

    def test_return_integer(self):
        """

        checks to see if returning a raw integer is acceptable

        :return:
        """

        engine = CustomGTPTester()

        self.assertEqual("= 5\n\n", engine.interpret("test-return_integer"))

    def test_return_vertex(self):
        """

        checks to see if returning a vertex is acceptable

        :return:
        """

        engine = CustomGTPTester()

        self.assertEqual("= E5\n\n", engine.interpret("test-return_vertex"))

    def test_return_raw_string(self):
        """

        checks to see if returning a string is acceptable

        :return:
        """

        engine = CustomGTPTester()

        self.assertEqual("= hello hello (Hola!)\n\n", engine.interpret("test-return_raw_string"))

    def test_return_color(self):
        """

        checks to see if returning a color is acceptable

        :return:
        """

        engine = CustomGTPTester()

        self.assertEqual("= B\n\n", engine.interpret("test-return_color"))

    def test_return_float(self):
        """

        checks to see if returning a float is acceptable

        :return:
        """

        engine = CustomGTPTester()

        self.assertEqual("= 3.14\n\n", engine.interpret("test-return_float"))

    def test_return_move(self):
        """

        checks to see if returning a move is acceptable

        :return:
        """

        engine = CustomGTPTester()

        self.assertEqual("= B E5\n\n", engine.interpret("test-return_move"))

    def test_return_bool(self):
        """

        checks to see if returning a raw integer is acceptable

        :return:
        """

        engine = CustomGTPTester()

        self.assertEqual("= false\n\n", engine.interpret("test-return_bool"))

    def test_gen_move_override(self):
        """

        checks to see if we can override the genmove function

        :return:
        """

        engine = CustomGTPTester()

        self.assertEqual("= B E5\n\n", engine.interpret("genmove B"))


class InterpreterSyntaxChecking(TestCase):

    def test_echo_wrong_arguments(self):
        """

        makes sure that the echo command recognizes when an invalid argument type is passed to it

        :return:
        """

        engine = CustomGTPTester()

        self.assertEqual("? no viable argument pattern for command \"test-echo\"; "
                         "candidate pattern not valid: expected string in position 1, got integer\n\n",
                         engine.interpret("test-echo 53"))

    def test_echo_wrong_number_arguments(self):
        """

        makes sure that the echo command recognizes when it gets the wrong number of arguments

        :return:
        """

        engine = CustomGTPTester()

        self.assertEqual("? invalid number of arguments for command \"test-echo\"; expected 1, got 5\n\n",
                         engine.interpret("test-echo in the world of silence"))


class InvalidRegistration(gtp.Engine):

    def __init__(self):
        super().__init__("test")

    def unlabeled_arguments(self, arg):
        return True, "you shouldn't be able to use this!" + arg

    def non_tuple_response(self):
        return [False, "lists aren't acceptable"]

    def wrong_number_responses(self):
        return 1, 2, 3

    def test_tuple_first_item_wrong(self):
        return "false", "not real bool!"

    def test_tuple_second_item_wrong(self):
        return True, []

    def color_followed_by_intersection(self, color: sente.stone, point: sente.Vertex):
        return True, "this should be compressed to a move object"

    def dictionaries_argument(self, arg: dict):
        return False, "dictionaries are invalid arguments"


def my_message(arg: str):
    return True, "this isn't a method, so we can't register it!" + arg


class TestInvalidRegistration(TestCase):

    def test_unlabeled_arguments(self):
        """

        makes sure that unlabeled arguments are caught

        :return:
        """

        engine = InvalidRegistration()

        with self.assertRaises(ValueError):
            engine.register_command(engine.unlabeled_arguments)

    def test_non_method(self):
        """

        makes sure that passing in a function that is not a method causes an error

        :return:
        """

        engine = InvalidRegistration()

        with self.assertRaises(ValueError):
            engine.register_command(my_message)

    def test_bad_input_type(self):
        """



        :return:
        """

        engine = InvalidRegistration()

        with self.assertRaises(TypeError):
            engine.register_command(engine.dictionaries_argument)

    def test_non_tuple_response(self):
        """

        checks to make sure that methods that return non-tuples error out when called

        :return:
        """

        engine = InvalidRegistration()
        engine.register_command(engine.non_tuple_response)

        with self.assertRaises(TypeError):
            engine.interpret("test-non_tuple_response")

    def test_bad_tuple_response(self):
        """

        checks to see if a tuple that does not contain two elements raises an exception

        :return:
        """

        engine = InvalidRegistration()
        engine.register_command(engine.wrong_number_responses)

        with self.assertRaises(ValueError):
            engine.interpret("test-wrong_number_responses")

    def test_wrong_tuple_first_element(self):
        """

        checks to make sure that the sente detects that a response has the wrong type in the first element of it's tuple

        :return:
        """

        engine = InvalidRegistration()
        engine.register_command(engine.test_tuple_first_item_wrong)

        with self.assertRaises(TypeError):
            engine.interpret("test-test_tuple_first_item_wrong")

    def test_wrong_tuple_second_element(self):
        """

        checks to make sure that the sente detects that a response has the wrong type in the second element of it's tuple

        :return:
        """

        engine = InvalidRegistration()
        engine.register_command(engine.test_tuple_second_item_wrong)

        with self.assertRaises(TypeError):
            engine.interpret("test-test_tuple_second_item_wrong")

    def test_point_followed_by_color_illegal(self):
        """

        makes sure that sente detects when we have an intersection followed by a color

        :return:
        """

        engine = InvalidRegistration()

        with self.assertRaises(ValueError):
            engine.register_command(engine.color_followed_by_intersection)
