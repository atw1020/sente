"""

Author: Arthur Wesley

"""


from unittest import TestCase

from sente import gtp


class CommandFunctionality(TestCase):

    def test_load_sgf(self):
        """



        :return:
        """

        engine = gtp.Engine()

        engine.interpret("loadsgf sgf/34839594-255-IDW64-noob_bot_3.sgf")
        self.assertEqual("= \n"
                         "19  .  .  .  .  .  .  X  O  O  O  O  O  O  X  .  .  .  .  .\n"
                         "18  .  .  .  .  O  X  X  X  O  X  X  X  O  X  .  .  .  .  .\n"
                         "17  .  .  O  .  O  O  X  X  X  .  X  X  O  X  X  .  O  .  .\n"
                         "16  .  .  .  O  .  .  O  X  X  *  X  O  O  O  X  X  O  .  .\n"
                         "15  O  O  O  X  O  O  O  O  X  O  O  .  O  O  X  .  .  .  .\n"
                         "14  X  X  X  X  X  O  X  X  O  .  O  .  .  .  O  O  .  .  .\n"
                         "13  .  .  .  .  X  O  X  X  O  .  O  O  O  O  .  O  .  .  .\n"
                         "12  O  O  O  O  O  X  X  O  O  .  .  .  O  .  O  O  X  .  .\n"
                         "11  X  X  X  O  X  X  X  X  O  .  .  O  X  X  .  .  .  .  .\n"
                         "10  .  .  X  O  O  X  .  X  O  *  .  O  .  .  .  O  O  O  .\n"
                         " 9  .  .  X  X  O  X  .  O  .  .  .  O  .  X  X  O  X  O  O \n"
                         " 8  O  O  X  X  O  X  .  O  .  .  O  X  X  X  O  X  X  X  O \n"
                         " 7  X  O  O  X  O  X  X  X  O  .  .  .  .  X  O  .  X  X  O \n"
                         " 6  X  .  .  O  O  X  X  X  O  .  .  O  O  O  O  .  .  .  X \n"
                         " 5  X  X  X  X  X  O  O  O  .  .  O  O  X  O  .  O  X  X  X \n"
                         " 4  O  O  O  O  O  O  X  X  O  O  X  X  X  X  X  O  O  O  O \n"
                         " 3  X  X  X  O  X  .  .  X  X  X  .  X  .  X  .  O  X  .  .\n"
                         " 2  X  X  .  O  X  X  X  .  .  .  X  O  X  .  .  O  X  X  X \n"
                         " 1  .  X  .  O  O  X  .  O  O  .  O  O  O  O  .  O  X  X  .\n"
                         "    A  B  C  D  E  F  G  H  J  K  L  M  N  O  P  Q  R  S  T\n"
                         "noob_bot_3: Thanks for playing. If you want a weaker/stronger bot match with you, recommend "
                         "try to play with 'ELOtest'. It can calculate & match your rank after few games.\n"
                         "noob_bot_3: Final score: W+368.5 (upper bound: 368.5, lower: 368.5)\n\n\n",
                         engine.interpret("showboard"))
