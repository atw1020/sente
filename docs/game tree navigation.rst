Game Tree Navigation
====================

Playing Stones
--------------

The ``sente.game`` object records all of the moves played in a tree structure.
Sente provides utilities for navigating around this tree with the ``game.play()``, ``game.step_up()`` and ``game.get_branches()`` methods.

The ``game.play()`` method plays at a given point on the board.

.. code-block:: python

    >>> import sente
    >>> game = sente.Game(9) # creates a 9x9 board
    >>> game.play(4, 4) # plays a stone on the 4-4 point
    >>> print(game)
     1  .  .  .  .  .  .  .  .  .
     2  .  .  .  .  .  .  .  .  .
     3  .  .  *  .  .  .  *  .  .
     4  .  .  .  #  .  .  .  .  .
     5  .  .  .  .  *  .  .  .  .
     6  .  .  .  .  .  .  .  .  .
     7  .  .  *  .  .  .  *  .  .
     8  .  .  .  .  .  .  .  .  .
     9  .  .  .  .  .  .  .  .  .
        A  B  C  D  E  F  G  H  J


Undoing Moves
-------------

The ``game.step_up()`` method on the other hand, undoes the previous move played on the board.

.. code-block:: python

    >>> import sente
    >>> game = sente.Game()
    >>> game.play(4, 4)
    >>> game.step_up() # undo the previous move
    >>> print(game)
     1  .  .  .  .  .  .  .  .  .
     2  .  .  .  .  .  .  .  .  .
     3  .  .  *  .  .  .  *  .  .
     4  .  .  .  .  .  .  .  .  .
     5  .  .  .  .  *  .  .  .  .
     6  .  .  .  .  .  .  .  .  .
     7  .  .  *  .  .  .  *  .  .
     8  .  .  .  .  .  .  .  .  .
     9  .  .  .  .  .  .  .  .  .
        A  B  C  D  E  F  G  H  J

Repeatedly calling ``step_up()`` can be tedious and slow, so the number of steps that ``step_up()`` takes can be specified as an argument.

.. code-block:: python

    >>> import sente
    >>> game = sente.Game()
    >>> game.play(4, 4)
    >>> game.play(7, 7)
    >>> game.play(7, 4)
    >>> game.step_up(2) # undo two moves
    >>> print(game)
     1  .  .  .  .  .  .  .  .  .
     2  .  .  .  .  .  .  .  .  .
     3  .  .  *  .  .  .  *  .  .
     4  .  .  .  #  .  .  .  .  .
     5  .  .  .  .  *  .  .  .  .
     6  .  .  .  .  .  .  .  .  .
     7  .  .  *  .  .  .  *  .  .
     8  .  .  .  .  .  .  .  .  .
     9  .  .  .  .  .  .  .  .  .
        A  B  C  D  E  F  G  H  J

Another alternative to the ``step_up()`` method is the ``advance_to_root()`` method which simply undoes every move in the game leaving the board empty.

.. code-block:: python

    >>> import sente
    >>> game = sente.Game()
    >>> game.play(4, 4)
    >>> game.play(7, 7)
    >>> game.play(7, 4)
    >>> game.advance_to_root()
    >>> print(game)
     1  .  .  .  .  .  .  .  .  .
     2  .  .  .  .  .  .  .  .  .
     3  .  .  *  .  .  .  *  .  .
     4  .  .  .  .  .  .  .  .  .
     5  .  .  .  .  *  .  .  .  .
     6  .  .  .  .  .  .  .  .  .
     7  .  .  *  .  .  .  *  .  .
     8  .  .  .  .  .  .  .  .  .
     9  .  .  .  .  .  .  .  .  .
        A  B  C  D  E  F  G  H  J

Once moves have been undone, the move(s) played at a given node of the tree can be obtained using the ``get_branches()`` method.

.. code-block:: python

    >>> import sente
    >>> game = sente.Game(9)
    >>> game.play(4, 4)
    >>> game.play(7, 7)
    >>> game.step_up()
    >>> print(game.get_branches())
    [<sente.Move W[gg]>]

The ``sente.Move`` object is an object that can be passed to the ``game.play()`` method to play the specified move.
The ``get_branches()`` method returns a python list containing all of the moves that were played at the current node of the tree.

.. code-block:: python

    >>> import sente
    >>> game = sente.Game(9)
    >>> game.play(4, 4)
    >>> game.play(7, 7)
    >>> game.step_up()
    >>> branches = game.get_branches()
    >>> game.play(branches[0])
    >>> print(game)
     1  .  .  .  .  .  .  .  .  .
     2  .  .  .  .  .  .  .  .  .
     3  .  .  *  .  .  .  *  .  .
     4  .  .  .  #  .  .  .  .  .
     5  .  .  .  .  *  .  .  .  .
     6  .  .  .  .  .  .  .  .  .
     7  .  .  *  .  .  .  O  .  .
     8  .  .  .  .  .  .  .  .  .
     9  .  .  .  .  .  .  .  .  .
        A  B  C  D  E  F  G  H  J

If multiple branches exist from the current node, then ``get_branches()`` will return a list containing all of them.

Move Sequences
--------------

When undoing moves in sente it can be tricky to keep track of the sequence of moves that leads to a given board position.
To make game tree navigation easier, the ``sente.Game`` Object contains a method called ``get_sequence`` that generates a python list containing the sequence of moves that have been played in the game so far.

.. code-block:: python

    >>> game.play(4, 4)
    >>> game.play(15, 4)
    >>> game.play(15, 15)
    >>> print(game.get_sequence())
    [<sente.Move B[dd]>, <sente.Move W[od]>, <sente.Move B[oo]>]

If the moves in a sequence are undone, the board position can be restored by using the ``play_sequence()`` method.

.. code-block:: python

    >>> print(game)
     1  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .
     2  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .
     3  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .
     4  .  .  .  #  .  .  .  .  .  *  .  .  .  .  .  *  .  .  .
     5  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .
     6  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .
     7  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .
     8  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .
     9  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .
    10  .  .  .  *  .  .  .  .  .  *  .  .  .  .  .  *  .  .  .
    11  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .
    12  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .
    13  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .
    14  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .
    15  .  .  .  O  .  .  .  .  .  .  .  .  .  .  #  .  .  .  .
    16  .  .  .  *  .  .  .  .  .  *  .  .  .  .  .  *  .  .  .
    17  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .
    18  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .
    19  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .
        A  B  C  D  E  F  G  H  J  K  L  M  N  O  P  Q  R  S  T
    >>> move_sequence = game.get_sequence()
    >>> game.step_up(3) # alternatively, you could use the advance_to_root() method
    >>> game.play_sequence(move_sequence)
    >>> print(game)
     1  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .
     2  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .
     3  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .
     4  .  .  .  #  .  .  .  .  .  *  .  .  .  .  .  *  .  .  .
     5  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .
     6  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .
     7  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .
     8  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .
     9  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .
    10  .  .  .  *  .  .  .  .  .  *  .  .  .  .  .  *  .  .  .
    11  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .
    12  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .
    13  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .
    14  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .
    15  .  .  .  O  .  .  .  .  .  .  .  .  .  .  #  .  .  .  .
    16  .  .  .  *  .  .  .  .  .  *  .  .  .  .  .  *  .  .  .
    17  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .
    18  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .
    19  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .
        A  B  C  D  E  F  G  H  J  K  L  M  N  O  P  Q  R  S  T

Additionally, because the ``get_sequence`` method returns a python list, python list slicing is supported.
