"""

Author: Arthur Wesley

"""

import inspect
from unittest import TestCase

import sente
from sente import gtp


class CustomGTPTester(gtp.Engine):

    def __init__(self):
        super().__init__("test")

        # register commands
        self.register_command(self.echo)
        self.register_command(self.octopus)
        self.register_command(self.greeting)

    def echo(self, text: str):
        return True, text

    def octopus(self):
        return True, "https://github.com/atw1020/sente/blob/master/tests/invalid%20sgf/octopus.jpeg"

    def greeting(self):
        return True, "hello, my name is" + self.name


class CustomGTPCommands(TestCase):

    def test_basic_registration(self):
        """



        :return:
        """

        engine = CustomGTPTester()

        self.assertEqual("= " + engine.octopus()[1] + "\n\n", engine.interpret("test-octopus"))

    def test_echo(self):
        """

        tests to see if a custom echo command works correctly

        :return:
        """

        engine = CustomGTPTester()

        self.assertEqual("= " + engine.echo("silence")[1] + "\n\n", engine.interpret("test-echo silence"))

    def test_greeting(self):
        """

        checks to see if the custom greeting command works correclty

        :return:
        """

        engine = CustomGTPTester()

        self.assertEqual("= " + engine.greeting()[1] + "\n\n", engine.interpret("test-greeting"))


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
        return False

    def wrong_number_responses(self):
        return 1, 2, 3

    def test_first_item_not_tuple(self):
        return "false", "not real bool!"

    def test_second_item_not_tuple(self):
        return True, 42


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

    def test_non_tuple_response(self):
        """

        checks to make sure that methods that return non-tuples error out when called

        :return:
        """

        engine = InvalidRegistration()
        engine.register_command(engine.non_tuple_response)

        print(engine.interpret("list_commands"))

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
        engine.register_command(engine.test_first_item_not_tuple)

        with self.assertRaises(TypeError):
            engine.interpret("test-test_first_item_not_tuple")

    def test_wrong_tuple_second_element(self):
        """

        checks to make sure that the sente detects that a response has the wrong type in the second element of it's tuple

        :return:
        """

        engine = InvalidRegistration()
        engine.register_command(engine.test_second_item_not_tuple)

        with self.assertRaises(TypeError):
            engine.interpret("test-test_second_item_not_tuple")
