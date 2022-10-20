Handicaps & setting up the Board
==================================

In many circumstances, it is necessary to set specific points
on the board to set up a go problem, or else to add a handicap.
In these circumstances, adding stones via ``sente.play(x, y)``
is overly cumbersome, since moves must be played in the
correct order. To assist setting up the board, ``sente.play()``
can accept a python set of stones.

.. doctest:: python

    >>> import sente
    >>> game = sente.Game()
    >>> game.play({sente.Move(sente.WHITE, 4, 4),
    ...            sente.Move(sente.BLACK, 16, 16)})
    >>> print(game)
     1  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .
     2  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .
     3  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .
     4  .  .  .  ⚪ .  .  .  .  .  *  .  .  .  .  .  *  .  .  .
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
    15  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .
    16  .  .  .  *  .  .  .  .  .  *  .  .  .  .  .  ⚫ .  .  .
    17  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .
    18  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .
    19  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .
        A  B  C  D  E  F  G  H  J  K  L  M  N  O  P  Q  R  S  T
    >>>


As seen above, moves added to the board need not be in the correct
order. This functionality can also be used to set up a Go problem.

.. doctest:: python

    >>> import sente
    >>> game = sente.Game(9)
    >>> game.play({ # a simple go problem
    ...     sente.Move(sente.stone.WHITE, 1, 4),
    ...     sente.Move(sente.stone.WHITE, 2, 4),
    ...     sente.Move(sente.stone.WHITE, 2, 3),
    ...     sente.Move(sente.stone.WHITE, 2, 2),
    ...     sente.Move(sente.stone.WHITE, 2, 1),
    ...     sente.Move(sente.stone.BLACK, 2, 6),
    ...     sente.Move(sente.stone.BLACK, 2, 5),
    ...     sente.Move(sente.stone.BLACK, 3, 4),
    ...     sente.Move(sente.stone.BLACK, 3, 3),
    ...     sente.Move(sente.stone.BLACK, 3, 2),
    ...     sente.Move(sente.stone.BLACK, 5, 2),
    ... })
    >>> print(game)
     1  .  ⚪ .  .  .  .  .  .  .
     2  .  ⚪ ⚫ .  ⚫ .  .  .  .
     3  .  ⚪ ⚫ .  .  .  *  .  .
     4  ⚪ ⚪ ⚫ .  .  .  .  .  .
     5  .  ⚫ .  .  *  .  .  .  .
     6  .  ⚫ .  .  .  .  .  .  .
     7  .  .  *  .  .  .  *  .  .
     8  .  .  .  .  .  .  .  .  .
     9  .  .  .  .  .  .  .  .  .
        A  B  C  D  E  F  G  H  J
    >>>



Handicaps
---------

The most common way that a board might be set up is using
a "Handicap" in which stones are placed on the board to
give one player an advantage. Handicaps can be added in
the ``sente.Game()`` constructor as follows

.. doctest:: python

    >>> import sente
    >>> game = sente.Game(handicap=sente.get_handicap(4))
    >>> print(game)
     1  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .
     2  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .
     3  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .
     4  .  .  .  ⚫ .  .  .  .  .  *  .  .  .  .  .  ⚫ .  .  .
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
    15  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .
    16  .  .  .  ⚫ .  .  .  .  .  *  .  .  .  .  .  ⚫ .  .  .
    17  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .
    18  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .
    19  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .
        A  B  C  D  E  F  G  H  J  K  L  M  N  O  P  Q  R  S  T



