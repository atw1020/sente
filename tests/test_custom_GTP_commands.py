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
        # self.register_command(self.echo)
        self.register_command(self.octopus)

    def echo(self, text: str):
        return True, text

    def octopus(self):
        return True, "https://github.com/atw1020/sente/blob/master/tests/invalid%20sgf/octopus.jpeg"


class CustomGTPCommands(TestCase):

    def test_basic_registration(self):
        """



        :return:
        """

        engine = CustomGTPTester()

        print(isinstance(engine, gtp.Engine))
        print(engine.interpret("test-octopus"))
