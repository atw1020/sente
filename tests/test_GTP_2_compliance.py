"""

Author: Arthur Wesley

"""

from unittest import TestCase

import sente
from sente import gtp


class MinimumGTPCommands(TestCase):

    def test_id(self):
        """

        makes sure that the id of the instruction is passed correctly

        :return:
        """

        engine = gtp.Engine()

        self.assertEqual("=1 2\n\n", engine.interpret("1 protocol_version\n"))
        self.assertEqual("=5 2\n\n", engine.interpret("5 protocol_version\n"))
        self.assertEqual("=2 2\n\n", engine.interpret("2 protocol_version\n"))
        self.assertEqual("=4294967295 2\n\n", engine.interpret("4294967295 protocol_version\n"))

    def test_protocol_version(self):
        """

        makes sure that the version command operates correctly

        :return:
        """

        engine = gtp.Engine()

        self.assertEqual("= 2\n\n", engine.interpret("protocol_version\n"))

    def test_name(self):
        """

        tests to see if the name command responds correctly

        :return:
        """

        engine = gtp.Engine()

        self.assertEqual("= unimplemented_engine\n\n", engine.interpret("name\n"))

        engine2 = gtp.Engine("Ceph_the_octopus")
        self.assertEqual("= Ceph_the_octopus\n\n", engine2.interpret("name\n"))

    def test_version(self):
        """

        tests to see if the version command responds correctly

        :return:
        """

        engine = gtp.Engine()

        self.assertEqual("= 0.0.0\n\n", engine.interpret("version\n"))

    def test_known_command(self):
        """

        tests to see if "known_command" works properly for real commands

        :return:
        """

        engine = gtp.Engine()

        self.assertEqual("= true\n\n", engine.interpret("known_command protocol_version\n"))
        self.assertEqual("= true\n\n", engine.interpret("known_command name\n"))
        self.assertEqual("= true\n\n", engine.interpret("known_command version\n"))
        self.assertEqual("= true\n\n", engine.interpret("known_command known_command\n"))
        self.assertEqual("= true\n\n", engine.interpret("known_command list_commands\n"))
        self.assertEqual("= true\n\n", engine.interpret("known_command quit\n"))
        self.assertEqual("= true\n\n", engine.interpret("known_command boardsize\n"))
        self.assertEqual("= true\n\n", engine.interpret("known_command clear_board\n"))
        self.assertEqual("= true\n\n", engine.interpret("known_command komi\n"))
        self.assertEqual("= true\n\n", engine.interpret("known_command play\n"))
        self.assertEqual("= true\n\n", engine.interpret("known_command genmove\n"))

    def test_known_command_false(self):
        """

        tests to see if known_command returns false for unkown commands

        :return:
        """

        engine = gtp.Engine()

        self.assertEqual("= false\n\n", engine.interpret("known_command there's_no_risky_thing\n"))
        self.assertEqual("= false\n\n", engine.interpret("known_command for_a_man_whose_determined_to_fall\n"))
        self.assertEqual("= false\n\n", engine.interpret("known_command octopus\n"))

    def test_list_commands(self):
        """

        tests to see if the known_command command responds correctly

        :return:
        """

        engine = gtp.Engine()

        # TODO: more precise testing
        commands = engine.interpret("list_commands")

        self.assertIn("protocol_version", commands)
        self.assertIn("name", commands)
        self.assertIn("version", commands)
        self.assertIn("known_command", commands)
        self.assertIn("list_commands", commands)
        self.assertIn("quit", commands)
        self.assertIn("boardsize", commands)
        self.assertIn("clear_board", commands)
        self.assertIn("komi", commands)
        self.assertIn("play", commands)
        self.assertIn("genmove", commands)

    def test_quit(self):
        """

        tests to see if the program quits

        :return:
        """

        engine = gtp.Engine()

        self.assertTrue(engine.active())
        self.assertEqual("= \n\n", engine.interpret("quit"))
        self.assertFalse(engine.active())

    def test_board_size(self):
        """

        tests to see if the boardSize command works properly

        :return:
        """

        engine = gtp.Engine()

        self.assertEqual("= \n\n", engine.interpret("boardsize 9"))
        self.assertEqual("= \n"
                         " 9  .  .  .  .  .  .  .  .  .\n"
                         " 8  .  .  .  .  .  .  .  .  .\n"
                         " 7  .  .  *  .  .  .  *  .  .\n"
                         " 6  .  .  .  .  .  .  .  .  .\n"
                         " 5  .  .  .  .  *  .  .  .  .\n"
                         " 4  .  .  .  .  .  .  .  .  .\n"
                         " 3  .  .  *  .  .  .  *  .  .\n"
                         " 2  .  .  .  .  .  .  .  .  .\n"
                         " 1  .  .  .  .  .  .  .  .  .\n"
                         "    A  B  C  D  E  F  G  H  J\n\n", engine.interpret("showboard"))

    def test_play(self):
        """

        tests to see if playing a move works correclty

        :return:
        """

        engine = gtp.Engine()

        engine.interpret("play B D4\n")

        self.assertEqual("= \n"
                         "19  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .\n"
                         "18  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .\n"
                         "17  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .\n"
                         "16  .  .  .  *  .  .  .  .  .  *  .  .  .  .  .  *  .  .  .\n"
                         "15  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .\n"
                         "14  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .\n"
                         "13  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .\n"
                         "12  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .\n"
                         "11  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .\n"
                         "10  .  .  .  *  .  .  .  .  .  *  .  .  .  .  .  *  .  .  .\n"
                         " 9  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .\n"
                         " 8  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .\n"
                         " 7  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .\n"
                         " 6  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .\n"
                         " 5  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .\n"
                         " 4  .  .  .  X  .  .  .  .  .  *  .  .  .  .  .  *  .  .  .\n"
                         " 3  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .\n"
                         " 2  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .\n"
                         " 1  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .\n"
                         "    A  B  C  D  E  F  G  H  J  K  L  M  N  O  P  Q  R  S  T\n\n",
                         engine.interpret("showboard"))

    def test_genmove(self):
        """

        tests to see if move generation is correct

        :return:
        """

        engine = gtp.Engine()

        with self.assertRaises(RuntimeError):
            engine.interpret("genmove B")

    def test_clear_board(self):
        """

        checks that the clear board command is working

        :return:
        """

        engine = gtp.Engine()

        engine.interpret("boardsize 9")
        engine.interpret("play B D4")
        engine.interpret("clear_board")

        self.assertEqual("= \n"
                         " 9  .  .  .  .  .  .  .  .  .\n"
                         " 8  .  .  .  .  .  .  .  .  .\n"
                         " 7  .  .  *  .  .  .  *  .  .\n"
                         " 6  .  .  .  .  .  .  .  .  .\n"
                         " 5  .  .  .  .  *  .  .  .  .\n"
                         " 4  .  .  .  .  .  .  .  .  .\n"
                         " 3  .  .  *  .  .  .  *  .  .\n"
                         " 2  .  .  .  .  .  .  .  .  .\n"
                         " 1  .  .  .  .  .  .  .  .  .\n"
                         "    A  B  C  D  E  F  G  H  J\n\n", engine.interpret("showboard"))


class ErrorMessages(TestCase):

    def test_unknown_command(self):
        """

        tests to see if the interpreter responds correctly to an unkown command

        :return:
        """

        engine = gtp.Engine()

        self.assertEqual("? unknown command\n\n", engine.interpret("please_tell_me_who_I_am"))
        self.assertEqual("? unknown command\n\n", engine.interpret("unknown_command"))
        self.assertEqual("? unknown command\n\n", engine.interpret("awfeawuehk"))

    def test_unacceptable_size(self):
        """

        tests to see if the interpreter rejects unacceptable board sizes

        :return:
        """

        engine = gtp.Engine()

        self.assertEqual("? unacceptable size\n\n", engine.interpret("boardsize 10"))

    def test_illegal_move(self):
        """

        tests to see if the interpreter recognizes an illegal move

        :return:
        """

        engine = gtp.Engine()

        engine.interpret("play B D4")
        self.assertEqual("? illegal move\n\n", engine.interpret("play W D4"))

    def test_wrong_color_not_rejected(self):
        """

        makes sure that moves that are of the wrong color can be played correctly

        :return:
        """

        engine = gtp.Engine()

        self.assertEqual("= \n\n", engine.interpret("play W D4"))


class OtherCompliance(TestCase):

    def test_ascii_compliance(self):
        """

        check to make sure that no unicode characters are used

        :return:
        """

        engine = gtp.Engine()

        engine.interpret("play B D4")

        self.assertNotIn("⚫", engine.interpret("showboard"))

    def test_move_co_ordinates(self):
        """

        tests to see if the board co-ordinates are correct

        :return:
        """

        engine = gtp.Engine()

        engine.interpret("boardsize 9")
        engine.interpret("play B D4")

        self.assertEqual("= \n"
                         " 9  .  .  .  .  .  .  .  .  .\n"
                         " 8  .  .  .  .  .  .  .  .  .\n"
                         " 7  .  .  *  .  .  .  *  .  .\n"
                         " 6  .  .  .  .  .  .  .  .  .\n"
                         " 5  .  .  .  .  *  .  .  .  .\n"
                         " 4  .  .  .  X  .  .  .  .  .\n"
                         " 3  .  .  *  .  .  .  *  .  .\n"
                         " 2  .  .  .  .  .  .  .  .  .\n"
                         " 1  .  .  .  .  .  .  .  .  .\n"
                         "    A  B  C  D  E  F  G  H  J\n\n", engine.interpret("showboard"))

        engine.interpret("resetboard")
        engine.interpret("play B D4")

        self.assertEqual("= \n"
                         " 9  .  .  .  .  .  .  .  .  .\n"
                         " 8  .  .  .  .  .  .  .  .  .\n"
                         " 7  .  .  *  .  .  .  *  .  .\n"
                         " 6  .  .  .  .  .  .  .  .  .\n"
                         " 5  .  .  .  .  *  .  .  .  .\n"
                         " 4  .  .  .  X  .  .  .  .  .\n"
                         " 3  .  .  *  .  .  .  *  .  .\n"
                         " 2  .  .  .  .  .  .  .  .  .\n"
                         " 1  .  .  .  .  .  .  .  .  .\n"
                         "    A  B  C  D  E  F  G  H  J\n\n", engine.interpret("showboard"))

    def test_I_cutoff(self):
        """

        makes sure that the offset of one letter is

        :return:
        """

        engine = gtp.Engine()

        engine.interpret("boardsize 9")
        engine.interpret("play B H8")

        self.assertEqual("= \n"
                         " 9  .  .  .  .  .  .  .  .  .\n"
                         " 8  .  .  .  .  .  .  .  X  .\n"
                         " 7  .  .  *  .  .  .  *  .  .\n"
                         " 6  .  .  .  .  .  .  .  .  .\n"
                         " 5  .  .  .  .  *  .  .  .  .\n"
                         " 4  .  .  .  .  .  .  .  .  .\n"
                         " 3  .  .  *  .  .  .  *  .  .\n"
                         " 2  .  .  .  .  .  .  .  .  .\n"
                         " 1  .  .  .  .  .  .  .  .  .\n"
                         "    A  B  C  D  E  F  G  H  J\n\n", engine.interpret("showboard"))

        engine.interpret("play W J9")

        self.assertEqual("= \n"
                         " 9  .  .  .  .  .  .  .  .  O \n"
                         " 8  .  .  .  .  .  .  .  X  .\n"
                         " 7  .  .  *  .  .  .  *  .  .\n"
                         " 6  .  .  .  .  .  .  .  .  .\n"
                         " 5  .  .  .  .  *  .  .  .  .\n"
                         " 4  .  .  .  .  .  .  .  .  .\n"
                         " 3  .  .  *  .  .  .  *  .  .\n"
                         " 2  .  .  .  .  .  .  .  .  .\n"
                         " 1  .  .  .  .  .  .  .  .  .\n"
                         "    A  B  C  D  E  F  G  H  J\n\n", engine.interpret("showboard"))

    def test_I_co_ord_invalid(self):
        """

        makes sure the interpreter fails when it receives a co-ordinate with an I in it

        """

        engine = gtp.Engine()

        with self.assertRaises(RuntimeError):
            engine.interpret("play B I9")

    def test_comments(self):
        """

        makes sure the protocol ignores comments

        :return:
        """

        engine = gtp.Engine()

        self.assertEqual("", engine.interpret("# this is a test"))
        self.assertEqual("= 2\n\n", engine.interpret("protocol_version # this is a test"))

    def test_multiple_commands_at_once(self):
        """

        tests to see if the interpreter can handle more than one command at a time

        :return:
        """

        engine = gtp.Engine()

        self.assertEqual(engine.interpret("boardsize 9") + engine.interpret("showboard"),
                         engine.interpret("boardsize 9\nshowboard"))

    def test_multiple_newlines_ignored(self):
        """

        checks to see if whitespace is ignored correctly

        :return:
        """

        engine = gtp.Engine()

        self.assertEqual("= \n\n= \n"
                         " 9  .  .  .  .  .  .  .  .  .\n"
                         " 8  .  .  .  .  .  .  .  .  .\n"
                         " 7  .  .  *  .  .  .  *  .  .\n"
                         " 6  .  .  .  .  .  .  .  .  .\n"
                         " 5  .  .  .  .  *  .  .  .  .\n"
                         " 4  .  .  .  .  .  .  .  .  .\n"
                         " 3  .  .  *  .  .  .  *  .  .\n"
                         " 2  .  .  .  .  .  .  .  .  .\n"
                         " 1  .  .  .  .  .  .  .  .  .\n"
                         "    A  B  C  D  E  F  G  H  J\n\n"
                         "= \n"
                         " 9  .  .  .  .  .  .  .  .  .\n"
                         " 8  .  .  .  .  .  .  .  .  .\n"
                         " 7  .  .  *  .  .  .  *  .  .\n"
                         " 6  .  .  .  .  .  .  .  .  .\n"
                         " 5  .  .  .  .  *  .  .  .  .\n"
                         " 4  .  .  .  .  .  .  .  .  .\n"
                         " 3  .  .  *  .  .  .  *  .  .\n"
                         " 2  .  .  .  .  .  .  .  .  .\n"
                         " 1  .  .  .  .  .  .  .  .  .\n"
                         "    A  B  C  D  E  F  G  H  J\n\n",
                         engine.interpret("boardsize 9\n\nshowboard\n\n\nshowboard"))

    def test_whitespace_ignored(self):
        """

        makes sure that whitespace between commands is ignored

        :return:
        """

        engine = gtp.Engine()

        self.assertEqual("= \n\n", engine.interpret("\t\tboardsize\t 9 "))
        self.assertEqual("=1 \n\n", engine.interpret("\t1 \t play    B D4   \t "))

        self.assertEqual("= \n"
                         " 9  .  .  .  .  .  .  .  .  .\n"
                         " 8  .  .  .  .  .  .  .  .  .\n"
                         " 7  .  .  *  .  .  .  *  .  .\n"
                         " 6  .  .  .  .  .  .  .  .  .\n"
                         " 5  .  .  .  .  *  .  .  .  .\n"
                         " 4  .  .  .  X  .  .  .  .  .\n"
                         " 3  .  .  *  .  .  .  *  .  .\n"
                         " 2  .  .  .  .  .  .  .  .  .\n"
                         " 1  .  .  .  .  .  .  .  .  .\n"
                         "    A  B  C  D  E  F  G  H  J\n\n", engine.interpret("showboard"))

    def test_whitespace_followed_by_newline(self):
        """

        tests to see if whitespace before a newline is ignored

        :return:
        """

        engine = gtp.Engine()

        self.assertEqual("= \n\n= \n"
                         " 9  .  .  .  .  .  .  .  .  .\n"
                         " 8  .  .  .  .  .  .  .  .  .\n"
                         " 7  .  .  *  .  .  .  *  .  .\n"
                         " 6  .  .  .  .  .  .  .  .  .\n"
                         " 5  .  .  .  .  *  .  .  .  .\n"
                         " 4  .  .  .  .  .  .  .  .  .\n"
                         " 3  .  .  *  .  .  .  *  .  .\n"
                         " 2  .  .  .  .  .  .  .  .  .\n"
                         " 1  .  .  .  .  .  .  .  .  .\n"
                         "    A  B  C  D  E  F  G  H  J\n\n",
                         engine.interpret("boardsize 9    \n\nshowboard"))
