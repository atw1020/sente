Gotchas
=======

This page contains some easy-to-make mistakes that come up when writing code using sente.
These Issues may lead to sleeper

Sente says my SGF file is "Invalid" but I can open it without errors in CGoban/Sabaki
-------------------------------------------------------------------------------------

.. code-block:: python

    >>> from sente import sgf
    >>> game = sgf.load("some SGF file.sgf")
    Traceback (most recent call last):
      File "<stdin>", line 1, in <module>
    sente.exceptions.InvalidSGFException: The Property "AP" is not supported on this version of SGF (FF[3])
    >>> game = sgf.load("some other SGF file.sgf")
    Traceback (most recent call last):
      File "<stdin>", line 1, in <module>
    sente.exceptions.InvalidSGFException: Unknown SGF Property: "JD"

Sente is a Stricter enforcer of the SGF standard than most go programs which means there may be some SGF files which it refuses to open dispute other programs being able to.
Future versions of Sente will include functionality that allows sente to open such files regardless.

At Present, the best thing to do is to find the problematic SGF property (the one listed in the error) and remove it from the file.

SGF files are a kind of `plain text file<https://en.wikipedia.org/wiki/Plain_text>`_ and so can be viewed using text editing programs like TextEdit (OSx) or Notepad (Windows).
For example, ``some SGF file.sgf`` Contains the following text:

.. code-block::
    :emphasize-lines: 11

    FF[3]
    EV[3rd Siptan tournament 1st round]
    PB[Lee Changho]
    BR[Wangwi]
    PW[Kang Dongyun]
    WR[7d]
    KM[6.5]
    RE[B+R]
    DT[2007-09-20]
    SZ[19]
    AP[Free Climber 0 . 8 . 11 . 61] # delete this line

The Error says that the "AP" property is illegal, so if we delete it, Sente will be able to parse the file.

.. code-block::

    >>> from sente import sgf
    >>> game = sgf.load("some SGF file.sgf")
    >>>

Boards vs Games
---------------

Sente provides two different constructs that represent go boards: the ``sente.Game`` object and the various ``sente.Board<9, 13, 19>`` objects.
On the surface it seems like these objects are similar to each other because both represent a position on a go board and both have a ``play`` method.

TL;DR just use ``sente.Game``.
******************************

The difference between the two is that a ``sente.Board`` contains only the location of the stones on the board and nothing else.
The ``sente.Board`` Objects do not know anything about capturing stones, Whose turn it is or Kos.
``sente.Board`` objects only exist in sente if for some reason you need to ignore the rules of the game for some reason which is rarely the case.

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
