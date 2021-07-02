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

        self.assertEqual(sente.Stone.BLACK, sente.Stone.BLACK)
        self.assertEqual(sente.Stone.WHITE, sente.Stone.WHITE)
        self.assertEqual(sente.Stone.EMPTY, sente.Stone.EMPTY)

    def test_opposite(self):
        """

        tests the opposite function

        :return:
        """

        stone = sente.Stone.BLACK
        self.assertEqual(sente.opposite_player(stone), sente.Stone.WHITE)

        stone = sente.Stone.WHITE
        self.assertEqual(sente.opposite_player(stone), sente.Stone.BLACK)


class MoveTest(TestCase):

    move1 = sente.Move(4, 4, sente.Stone.BLACK)
    move2 = sente.Move(4, 4, sente.Stone.BLACK)
    move3 = sente.Move(3, 4, sente.Stone.BLACK)
    move4 = sente.Move(3, 4, sente.Stone.WHITE)
    move5 = sente.Move(0, 0, sente.Stone.EMPTY)

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

        self.assertEqual(sente.Stone.BLACK, self.move1.get_stone())
        self.assertEqual(sente.Stone.WHITE, self.move4.get_stone())

    def test__str__(self):
        """

        checks to see if the string casting works correctly

        :return:
        """

        self.assertEqual("B[ee]", str(self.move1))
        self.assertEqual("B[de]", str(self.move3))
        self.assertEqual("W[de]", str(self.move4))
        self.assertEqual("[Placeholder Move]", str(self.move5))
