"""

Author: Arthur Wesley

"""

import inspect
from unittest import TestCase

import sente
from sente import gtp


class MyClass:
    def add(self, a: int, b: tuple, c: str, d: sente.stone, e: float, f: sente.Move, g: bool):
        return True, "success"


def register_command(func):
    arg_spec = inspect.getfullargspec(func)
    print(arg_spec)
    print(func.__name__)


class CustomGTPCommands(TestCase):

    def test_basic_registration(self):
        """



        :return:
        """

        engine = gtp.Engine()
        instance = MyClass()
        engine.register_command(instance.add)

        self.assertTrue(True)
