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

    move1 = sente.Move(sente.stone.BLACK, 5, 5)
    move2 = sente.Move(sente.stone.BLACK, 5, 5)
    move3 = sente.Move(sente.stone.BLACK, 4, 5)
    move4 = sente.Move(sente.stone.WHITE, 4, 5)

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

        self.assertEqual("B E4", str(self.move1))
        self.assertEqual("B D4", str(self.move3))
        self.assertEqual("W D4", str(self.move4))

