"""

Author: Arthur Wesley

"""

from unittest import TestCase

import sente


class StoneTest(TestCase):

    def test_equals(self):
        """

        checks to see if equivalence is defined for stones

        :return:
        """

        self.assertEqual(sente.stone.BLACK, sente.stone.BLACK)
        self.assertEqual(sente.stone.WHITE, sente.stone.WHITE)
        self.assertEqual(sente.stone.EMPTY, sente.stone.EMPTY)

    def test_opposite(self):
        """

        tests the opposite function

        :return:
        """

        stone = sente.stone.BLACK
        self.assertEqual(sente.opposite_player(stone), sente.stone.WHITE)

        stone = sente.stone.WHITE
        self.assertEqual(sente.opposite_player(stone), sente.stone.BLACK)


class MoveTest(TestCase):

    move1 = sente.Move(4, 4, sente.stone.BLACK)
    move2 = sente.Move(4, 4, sente.stone.BLACK)
    move3 = sente.Move(3, 4, sente.stone.BLACK)
    move4 = sente.Move(3, 4, sente.stone.WHITE)
    move5 = sente.Move(0, 0, sente.stone.EMPTY)

    def test_equals(self):
        """

        checks to see if two moves

        :return:
        """

        self.assertEqual(self.move1, self.move2)
        self.assertNotEqual(self.move1, self.move3)
        self.assertNotEqual(self.move3, self.move4)

    def test_get_methods(self):
        """

        test the get x method

        :return:
        """

        self.assertEqual(4, self.move1.get_x())
        self.assertEqual(3, self.move3.get_x())
        self.assertEqual(4, self.move3.get_y())

        self.assertEqual(sente.stone.BLACK, self.move1.get_stone())
        self.assertEqual(sente.stone.WHITE, self.move4.get_stone())

    def test__repr__(self):
        """

        checks to see if the string casting works correctly

        :return:
        """

        game = sente.Game()
        game.play(self.move3)

        self.assertEqual("<sente.Move B[ee]>", str(self.move1))
        self.assertEqual("<sente.Move B[de]>", str(self.move3))
        self.assertEqual("<sente.Move W[de]>", str(self.move4))
        self.assertEqual("<sente.Move [Placeholder Move]>", str(self.move5))

