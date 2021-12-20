"""

Author: Arthur Wesley

"""


from unittest import TestCase

from sente import gtp


class CommandFunctionality(TestCase):

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

        self.assertEqual("= Engine using Sente GTP\n\n", engine.interpret("name\n"))

        engine2 = gtp.Engine("Ceph the octopus")
        self.assertEqual("= Ceph the octopus\n\n", engine2.interpret("name\n"))

    def test_version(self):
        """

        tests to see if the version command responds correctly

        :return:
        """

        with open("../version.txt") as file:
            version = file.read().strip()

        engine = gtp.Engine()

        self.assertEqual("= " + version + "\n\n", engine.interpret("version\n"))

    def test_known_command(self):
        """

        tests to see if "known_command" works properly for real commands

        :return:
        """

        engine = gtp.Engine()

        self.assertEqual("= true\n\n", engine.interpret("known_command name\n"))
        self.assertEqual("= true\n\n", engine.interpret("known_command known_command\n"))
        self.assertEqual("= true\n\n", engine.interpret("known_command list_commands\n"))
        self.assertEqual("= true\n\n", engine.interpret("known_command quit\n"))
        self.assertEqual("= true\n\n", engine.interpret("known_command clearboard\n"))
        self.assertEqual("= true\n\n", engine.interpret("known_command play\n"))

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

        # TODO: Fix Segault
        self.assertEqual("= \n\n", engine.interpret("boardsize 9"))

        board = engine.get_game().get_board()
        print("point 1")
        board.get_stone(4, 4)
        print("point 2")

        # engine.get_game().get_board().get_stone(4, 4)

    def test_play(self):
        """
        
        tests to see if playing a move works correclty
        
        :return: 
        """
        
        engine = gtp.Engine()

        engine.interpret("play B D4\n")


