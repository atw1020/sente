"""

Author: Arthur Wesley

"""


from unittest import TestCase

from sente import gtp


class CommandFunctionality(TestCase):

    def test_wierd_showboard(self):
        """



        :return:
        """

        engine = gtp.Engine()

        engine.interpret("list_commands")
        engine.interpret("loadsgf sgf/34839594-255-IDW64-noob_bot_3.sgf")
        engine.interpret("34839594-255-IDW64-noob_bot_3.sgf")
        engine.interpret("loadsgf tests/sgf/34839594-255-IDW64-noob_bot_3.sgf 20")
        print(engine.interpret("showboard"))
