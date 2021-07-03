"""

Author: Arthur Wesley

"""

from unittest import TestCase

from sente import *


class BoardTest19(TestCase):

    def test_get_side(self):
        """

        tests the get_side function

        :return:
        """

        board = Board19()

        self.assertEqual(19, board.get_side())

    def test_board_equals(self):
        """

        checks to see if the equals operator works on the board

        :return:
        """

        board1 = Board19()

        board1.play(Move(3, 3, Stone.BLACK))
        board1.play(Move(8, 3, Stone.WHITE))
        board1.play(Move(3, 7, Stone.BLACK))

        board2 = Board19()

        board2.play(Move(3, 3, Stone.BLACK))
        board2.play(Move(8, 3, Stone.WHITE))

        self.assertNotEqual(board1, Board19())
        self.assertNotEqual(board1, board2)

        board2.play(Move(3, 7, Stone.BLACK))
        self.assertEqual(board1, board2)

    def test_get_stone(self):
        """

        checks to see if the board can obtain placed stones

        :return:
        """

        board = Board19()
        board.play(Move(3, 3, Stone.BLACK))

        self.assertEqual(board.get_stone(5, 5), Stone.EMPTY)
        self.assertEqual(board.get_stone(3, 3), Stone.BLACK)

    def test__str__(self):
        """

        checks string casting

        :return:
        """

        board = Board19()

        board.play(Move(0, 0, Stone.BLACK))
        board.play(Move(15, 3, Stone.WHITE))

        self.assertEqual(" 1  #  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .\n"
                         " 2  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .\n"
                         " 3  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .\n"
                         " 4  .  .  .  *  .  .  .  .  .  *  .  .  .  .  .  *  .  .  .\n"
                         " 5  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .\n"
                         " 6  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .\n"
                         " 7  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .\n"
                         " 8  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .\n"
                         " 9  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .\n"
                         "10  .  .  .  *  .  .  .  .  .  *  .  .  .  .  .  *  .  .  .\n"
                         "11  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .\n"
                         "12  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .\n"
                         "13  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .\n"
                         "14  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .\n"
                         "15  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .\n"
                         "16  .  .  .  O  .  .  .  .  .  *  .  .  .  .  .  *  .  .  .\n"
                         "17  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .\n"
                         "18  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .\n"
                         "19  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .\n"
                         "    A  B  C  D  E  F  G  H  I  J  K  L  M  N  O  P  Q  R  S\n",
                         str(board))


class BoardTest13(TestCase):

    def test_get_side(self):
        """

        tests the get_side function

        :return:
        """

        board = Board13()

        self.assertEqual(13, board.get_side())

    def test_board_equals(self):
        """

        checks to see if the equals operator works on the board

        :return:
        """

        board1 = Board19()

        board1.play(Move(3, 3, Stone.BLACK))
        board1.play(Move(8, 3, Stone.WHITE))
        board1.play(Move(3, 7, Stone.BLACK))

        board2 = Board19()

        board2.play(Move(3, 3, Stone.BLACK))
        board2.play(Move(8, 3, Stone.WHITE))

        self.assertNotEqual(board1, Board19())
        self.assertNotEqual(board1, board2)

        board2.play(Move(3, 7, Stone.BLACK))
        self.assertEqual(board1, board2)

    def test_get_stone(self):
        """

        checks to see if the board can obtain placed stones

        :return:
        """

        board = Board13()
        board.play(Move(3, 3, Stone.BLACK))

        self.assertEqual(board.get_stone(5, 5), Stone.EMPTY)
        self.assertEqual(board.get_stone(3, 3), Stone.BLACK)

    def test__str__(self):
        """

        checks string casting

        :return:
        """

        board = Board13()

        board.play(Move(0, 0, Stone.BLACK))
        board.play(Move(8, 3, Stone.WHITE))

        self.assertEqual(" 1  #  .  .  .  .  .  .  .  .  .  .  .  .\n"
                         " 2  .  .  .  .  .  .  .  .  .  .  .  .  .\n"
                         " 3  .  .  .  .  .  .  .  .  .  .  .  .  .\n"
                         " 4  .  .  .  *  .  .  .  .  .  *  .  .  .\n"
                         " 5  .  .  .  .  .  .  .  .  .  .  .  .  .\n"
                         " 6  .  .  .  .  .  .  .  .  .  .  .  .  .\n"
                         " 7  .  .  .  .  .  .  *  .  .  .  .  .  .\n"
                         " 8  .  .  .  .  .  .  .  .  .  .  .  .  .\n"
                         " 9  .  .  .  O  .  .  .  .  .  .  .  .  .\n"
                         "10  .  .  .  *  .  .  .  .  .  *  .  .  .\n"
                         "11  .  .  .  .  .  .  .  .  .  .  .  .  .\n"
                         "12  .  .  .  .  .  .  .  .  .  .  .  .  .\n"
                         "13  .  .  .  .  .  .  .  .  .  .  .  .  .\n"
                         "    A  B  C  D  E  F  G  H  I  J  K  L  M\n",
                         str(board))


class BoardTest9(TestCase):

    def test_get_side(self):
        """

        tests the get_side function

        :return:
        """

        board = Board9()

        self.assertEqual(9, board.get_side())

    def test_board_equals(self):
        """

        checks to see if the equals operator works on the board

        :return:
        """

        board1 = Board19()

        board1.play(Move(3, 3, Stone.BLACK))
        board1.play(Move(8, 3, Stone.WHITE))
        board1.play(Move(3, 7, Stone.BLACK))

        board2 = Board19()

        board2.play(Move(3, 3, Stone.BLACK))
        board2.play(Move(8, 3, Stone.WHITE))

        self.assertNotEqual(board1, Board19())
        self.assertNotEqual(board1, board2)

        board2.play(Move(3, 7, Stone.BLACK))
        self.assertEqual(board1, board2)

    def test_get_stone(self):
        """

        checks to see if the board can obtain placed stones

        :return:
        """

        board = Board9()
        board.play(Move(3, 3, Stone.BLACK))

        self.assertEqual(board.get_stone(5, 5), Stone.EMPTY)
        self.assertEqual(board.get_stone(3, 3), Stone.BLACK)

    def test__str__(self):
        """

        checks string casting

        :return:
        """

        board = Board9()

        board.play(Move(0, 0, Stone.BLACK))
        board.play(Move(8, 3, Stone.WHITE))

        self.assertEqual(
                         " 1  #  .  .  .  .  .  .  .  .\n"
                         " 2  .  .  .  .  .  .  .  .  .\n"
                         " 3  .  .  *  .  .  .  *  .  .\n"
                         " 4  .  .  .  .  .  .  .  .  .\n"
                         " 5  .  .  .  .  *  .  .  .  .\n"
                         " 6  .  .  .  .  .  .  .  .  .\n"
                         " 7  .  .  *  .  .  .  *  .  .\n"
                         " 8  .  .  .  .  .  .  .  .  .\n"
                         " 9  .  .  .  O  .  .  .  .  .\n"
                         "    A  B  C  D  E  F  G  H  I\n",
                         str(board))

