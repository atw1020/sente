Game Tree Navigation
====================

Basic Navigation
----------------

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
    >>>
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

Other Utilities
---------------

In addition to the ``play()``, ``step_up()`` and ``get_branches()`` methods, sente also provides several other methods to make tree navigation easier.
These methods are the ``advance_to_root()``, ``play_sequence``, ``get_sequence()``

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

SGF files
---------