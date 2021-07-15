Gotchas
=======

This page contains some easy-to-make mistakes that come up when writing code using sente.
These Issues may lead to sleeper

Internal vs external Co-ordinates
---------------------------------

Internally, Sente represents co-ordinates on a go board with `zero based indexing <https://en.wikipedia.org/wiki/Zero-based_numbering>`_.
This means that internally, a stone on the 1-1 corner point would actually be located at the index ``(0, 0)`` rather than ``(1, 1)``.
The ``Game.play()`` method hides this from you by automatically subtracting 1 from the co-ordinates that you specify.
However ``sente.Move`` objects use `Internal` Indexing and thus, if you wish to instantiate a ``sente.Move`` Object, the co-ordinates must zero-indexed.

.. code-block:: python

    >>> move = sente.Move(0, 0, sente.stone.WHITE) # move is actually on the 1-1 point
    >>> game.play(1, 1)
    >>> game.play(move)
    Traceback (most recent call last):
      File "<stdin>", line 1, in <module>
    sente.exceptions.IllegalMoveException: The Desired move W[aa] lies on an occupied point


Because of this, Instantiating the ``sente.Move`` object directly is inadvisable and using ``sente.play(x, y)`` is prefered.
