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

        board1.play(Move(stone.BLACK, 4, 4))
        board1.play(Move(stone.WHITE, 9, 4))
        board1.play(Move(stone.BLACK, 4, 8))

        board2 = Board19()

        board2.play(Move(stone.BLACK, 4, 4))
        board2.play(Move(stone.WHITE, 9, 4))

        self.assertNotEqual(board1, Board19())
        self.assertNotEqual(board1, board2)

        board2.play(Move(stone.BLACK, 4, 8))
        self.assertEqual(board1, board2)

    def test_get_stone(self):
        """

        checks to see if the board can obtain placed stones

        :return:
        """

        board = Board19()
        board.play(Move(stone.BLACK, 4, 4))

        self.assertEqual(board.get_stone(5, 5), stone.EMPTY)
        self.assertEqual(board.get_stone(4, 4), stone.BLACK)

    def test__str__(self):
        """

        checks string casting

        :return:
        """

        board = Board19()

        board.play(Move(stone.BLACK, 19, 1))
        board.play(Move(stone.WHITE, 4, 16))

        correct = " 1  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  ⚫\n" \
                  " 2  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .\n" \
                  " 3  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .\n" \
                  " 4  .  .  .  *  .  .  .  .  .  *  .  .  .  .  .  *  .  .  .\n" \
                  " 5  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .\n" \
                  " 6  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .\n" \
                  " 7  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .\n" \
                  " 8  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .\n" \
                  " 9  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .\n" \
                  "10  .  .  .  *  .  .  .  .  .  *  .  .  .  .  .  *  .  .  .\n" \
                  "11  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .\n" \
                  "12  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .\n" \
                  "13  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .\n" \
                  "14  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .\n" \
                  "15  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .\n" \
                  "16  .  .  .  ⚪ .  .  .  .  .  *  .  .  .  .  .  *  .  .  .\n" \
                  "17  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .\n" \
                  "18  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .\n" \
                  "19  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .\n" \
                  "    A  B  C  D  E  F  G  H  J  K  L  M  N  O  P  Q  R  S  T"

        self.assertEqual(correct, str(board))


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

        board1.play(Move(stone.BLACK, 4, 4))
        board1.play(Move(stone.WHITE, 9, 4))
        board1.play(Move(stone.BLACK, 4, 8))

        board2 = Board19()

        board2.play(Move(stone.BLACK, 4, 4))
        board2.play(Move(stone.WHITE, 9, 4))

        self.assertNotEqual(board1, Board19())
        self.assertNotEqual(board1, board2)

        board2.play(Move(stone.BLACK, 4, 8))
        self.assertEqual(board1, board2)

    def test_get_stone(self):
        """

        checks to see if the board can obtain placed stones

        :return:
        """

        board = Board13()
        board.play(Move(stone.BLACK, 4, 4))

        self.assertEqual(board.get_stone(5, 5), stone.EMPTY)
        self.assertEqual(board.get_stone(4, 4), stone.BLACK)

    def test__str__(self):
        """

        checks string casting

        :return:
        """

        board = Board13()

        board.play(Move(stone.BLACK, 1, 1))
        board.play(Move(stone.WHITE, 4, 9))

        correct = " 1  ⚫ .  .  .  .  .  .  .  .  .  .  .  .\n" \
                  " 2  .  .  .  .  .  .  .  .  .  .  .  .  .\n" \
                  " 3  .  .  .  .  .  .  .  .  .  .  .  .  .\n" \
                  " 4  .  .  .  *  .  .  .  .  .  *  .  .  .\n" \
                  " 5  .  .  .  .  .  .  .  .  .  .  .  .  .\n" \
                  " 6  .  .  .  .  .  .  .  .  .  .  .  .  .\n" \
                  " 7  .  .  .  .  .  .  *  .  .  .  .  .  .\n" \
                  " 8  .  .  .  .  .  .  .  .  .  .  .  .  .\n" \
                  " 9  .  .  .  ⚪ .  .  .  .  .  .  .  .  .\n" \
                  "10  .  .  .  *  .  .  .  .  .  *  .  .  .\n" \
                  "11  .  .  .  .  .  .  .  .  .  .  .  .  .\n" \
                  "12  .  .  .  .  .  .  .  .  .  .  .  .  .\n" \
                  "13  .  .  .  .  .  .  .  .  .  .  .  .  .\n" \
                  "    A  B  C  D  E  F  G  H  J  K  L  M  N"

        """if platform == "win32":
            correct = correct.replace("⚪", "O ")
            correct = correct.replace("⚫", "# ")"""

        self.assertEqual(correct, str(board))


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

        board1.play(Move(stone.BLACK, 4, 4))
        board1.play(Move(stone.WHITE, 9, 4))
        board1.play(Move(stone.BLACK, 4, 8))

        board2 = Board19()

        board2.play(Move(stone.BLACK, 4, 4))
        board2.play(Move(stone.WHITE, 9, 4))

        self.assertNotEqual(board1, Board19())
        self.assertNotEqual(board1, board2)

        board2.play(Move(stone.BLACK, 4, 8))
        self.assertEqual(board1, board2)

    def test_get_stone(self):
        """

        checks to see if the board can obtain placed stones

        :return:
        """

        board = Board9()
        board.play(Move(stone.BLACK, 4, 4))

        self.assertEqual(board.get_stone(5, 5), stone.EMPTY)
        self.assertEqual(board.get_stone(4, 4), stone.BLACK)

    def test__str__(self):
        """

        checks string casting

        :return:
        """

        board = Board9()

        board.play(Move(stone.BLACK, 1, 1))
        board.play(Move(stone.WHITE, 4, 9))

        correct = " 1  ⚫ .  .  .  .  .  .  .  .\n" \
                  " 2  .  .  .  .  .  .  .  .  .\n" \
                  " 3  .  .  *  .  .  .  *  .  .\n" \
                  " 4  .  .  .  .  .  .  .  .  .\n" \
                  " 5  .  .  .  .  *  .  .  .  .\n" \
                  " 6  .  .  .  .  .  .  .  .  .\n" \
                  " 7  .  .  *  .  .  .  *  .  .\n" \
                  " 8  .  .  .  .  .  .  .  .  .\n" \
                  " 9  .  .  .  ⚪ .  .  .  .  .\n" \
                  "    A  B  C  D  E  F  G  H  J"

        """if platform == "win32":
            correct = correct.replace("⚪", "O ")
            correct = correct.replace("⚫", "# ")"""

        self.assertEqual(correct, str(board))
