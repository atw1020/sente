Gotchas
=======

This page contains some easy-to-make mistakes that come up when writing code using sente.

Boards vs Games
---------------

Sente provides two different constructs that represent go boards: the ``sente.Game`` object and the various ``sente.Board<9, 13, 19>`` objects.
On the surface, it seems like these objects are similar to each other because both represent a position on a go board and both have a ``play`` method.

TL;DR just use ``sente.Game``.
******************************

The difference between the two is that a ``sente.Board`` contains only the location of the stones on the board and nothing else.
The ``sente.Board`` Objects do not know anything about capturing stones, Whose turn it is, or Kos.
``sente.Board`` objects only exist in sente if for some reason you need to ignore the rules of the game.

Internal vs external Co-ordinates
---------------------------------

Internally, Sente represents coordinates on a go board with `zero-based indexing <https://en.wikipedia.org/wiki/Zero-based_numbering>`_.
This means that internally, a stone on the 1-1 corner point would actually be located at the index ``(0, 0)`` rather than ``(1, 1)``.
The ``Game.play()`` method hides this from you by automatically subtracting 1 from the coordinates that you specify.
However ``sente.Move`` objects use `Internal` Indexing and thus if you wish to instantiate a ``sente.Move`` Object, the coordinates must zero-indexed.

.. code-block:: python

    >>> move = sente.Move(0, 0, sente.stone.WHITE) # move is actually on the 1-1 point
    >>> game.play(1, 1)
    >>> game.play(move)
    Traceback (most recent call last):
      File "<stdin>", line 1, in <module>
    sente.exceptions.IllegalMoveException: The Desired move W[aa] lies on an occupied point


Because of this, Instantiating the ``sente.Move`` object directly is inadvisable and using ``sente.play(x, y)`` is preferred.
