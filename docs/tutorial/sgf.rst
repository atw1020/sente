The ``sgf`` (Smart Game Format) Module
======================================


The ``sente.sgf`` module contains functions for parsing SGF (Smart Game Format) Files.
SGF is a standard encoding for go games that is capable of encoding "forked" go matches in addition to a raw sequence of moves.
SGF files can be viewed in programs like `CGoban <https://www.gokgs.com/download.jsp>`_ or `Sabaki <https://sabaki.yichuanshen.de>`_.

.. note:: sente is a library for Go and cannot be used for other games.

Loading Games
-------------

SGF files can be loaded into sente using the ``sgf.load`` function.
``sgf.load`` returns a ``sente.Game`` object with a move tree that has been populated with the game tree in the SGF file.

.. code-block:: python

    >>> from sente import sgf
    >>> game = sgf.load("Lee Sedol ladder game.sgf")

.. doctest::
    :hide:

    >>> from sente import sgf
    >>> game = sgf.load("../tests/sgf/Lee Sedol ladder game.sgf")

.. Note:: The ``SGF.load()`` (and ``SGF.dump()``) functions can accept python path objects from the built in ``os`` and ``pathlib`` modules.

When sente loads an SGF file, it does not play out the sequence of moves in the game.
It populates an internal game tree with the moves played, but it does not play them on the board.
After the game is loaded, the game will still have an empty board.
However, if the ``get_children`` method is called, we can see that the opening move will be populated

.. doctest::

    >>> print(game) # the board is empty but the SGF file is loaded
     1  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .
     2  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .
     3  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .
     4  .  .  .  *  .  .  .  .  .  *  .  .  .  .  .  *  .  .  .
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
    16  .  .  .  *  .  .  .  .  .  *  .  .  .  .  .  *  .  .  .
    17  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .
    18  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .
    19  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .
        A  B  C  D  E  F  G  H  J  K  L  M  N  O  P  Q  R  S  T
    >>> game.get_branches() # if we use get_branches() we can see that a branch has been initiated
    [<sente.Move B Q4>]

It can be pretty tedious to play through every move by calling ``get_branches()`` and selecting the first branch every time, so sente provides the ``get_default_sequence()`` and ``play_default_sequence()`` methods to make it easier to play through games.
If we wish to play the game through Hong Jansik's resignation at move 212, we can use the ``play_default_sequence()`` method to play out the main branch of the game.

.. code-block:: python

    >>> game.play_default_sequence()
    >>> print(game)
     1  .  .  ⚪ .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .
     2  .  ⚪ .  ⚪ ⚫ .  .  .  ⚪ .  ⚫ .  .  .  .  .  .  ⚪ ⚫
     3  .  .  ⚪ ⚫ ⚫ .  .  ⚪ .  ⚪ ⚫ .  .  ⚫ ⚫ ⚪ ⚪ ⚫ .
     4  .  .  .  ⚪ .  ⚫ .  ⚪ ⚪ ⚫ ⚪ .  .  .  .  ⚫ ⚫ .  ⚫
     5  .  .  ⚪ ⚪ .  .  .  .  .  ⚫ .  .  .  .  .  .  ⚪ ⚫ ⚫
     6  .  .  ⚪ ⚫ ⚫ .  .  .  .  .  .  .  .  .  .  .  .  .  .
     7  .  .  ⚫ ⚪ ⚪ ⚫ .  ⚪ .  ⚫ .  .  .  .  .  .  .  .  .
     8  .  .  .  ⚫ ⚪ ⚪ ⚫ .  .  .  .  .  .  .  .  .  .  .  .
     9  .  ⚪ ⚪ ⚪ ⚫ ⚪ ⚪ ⚫ .  .  .  .  .  .  .  .  .  .  .
    10  .  ⚫ ⚪ ⚫ ⚫ ⚫ ⚪ ⚪ ⚫ *  ⚫ .  .  .  .  *  .  ⚫ .
    11  .  ⚪ ⚫ ⚫ ⚪ .  ⚫ ⚪ ⚪ ⚫ .  ⚫ ⚫ .  ⚫ .  ⚫ ⚪ .
    12  .  ⚪ ⚪ ⚫ .  ⚫ ⚫ ⚫ ⚪ ⚪ ⚫ ⚫ .  ⚫ .  ⚫ .  ⚪ ⚪
    13  .  ⚪ ⚪ ⚫ ⚫ ⚪ ⚪ ⚫ ⚫ ⚪ ⚪ ⚫ .  ⚫ ⚫ ⚪ ⚪ ⚪ .
    14  ⚫ ⚪ ⚫ ⚫ ⚪ ⚪ ⚫ ⚫ .  ⚫ ⚪ ⚫ .  ⚫ .  ⚫ ⚫ ⚫ ⚪
    15  .  ⚫ .  ⚫ ⚪ ⚫ ⚫ ⚪ ⚫ .  ⚪ ⚫ ⚫ ⚫ ⚫ ⚪ ⚪ ⚫ ⚪
    16  ⚫ ⚫ ⚫ ⚪ ⚪ ⚫ ⚪ ⚪ ⚫ ⚪ ⚪ .  ⚪ .  .  ⚪ ⚪ ⚫ .
    17  ⚫ ⚪ ⚫ ⚪ .  ⚪ ⚪ .  ⚫ ⚪ .  ⚪ .  ⚫ ⚫ ⚫ ⚪ ⚫ ⚫
    18  .  ⚪ ⚫ ⚪ .  ⚪ ⚫ ⚫ ⚫ ⚫ ⚪ .  .  .  .  .  ⚪ ⚪ ⚫
    19  ⚪ .  ⚪ ⚪ ⚪ .  ⚪ .  .  ⚪ .  .  .  .  .  .  .  ⚪ ⚪
        A  B  C  D  E  F  G  H  J  K  L  M  N  O  P  Q  R  S  T

.. doctest::
    :hide:

    >>> game.play_default_sequence()
    >>> print(game)
     1  .  .  ⚪ .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .
     2  .  ⚪ .  ⚪ ⚫ .  .  .  ⚪ .  ⚫ .  .  .  .  .  .  ⚪ ⚫
     3  .  .  ⚪ ⚫ ⚫ .  .  ⚪ .  ⚪ ⚫ .  .  ⚫ ⚫ ⚪ ⚪ ⚫ .
     4  .  .  .  ⚪ .  ⚫ .  ⚪ ⚪ ⚫ ⚪ .  .  .  .  ⚫ ⚫ .  ⚫
     5  .  .  ⚪ ⚪ .  .  .  .  .  ⚫ .  .  .  .  .  .  ⚪ ⚫ ⚫
     6  .  .  ⚪ ⚫ ⚫ .  .  .  .  .  .  .  .  .  .  .  .  .  .
     7  .  .  ⚫ ⚪ ⚪ ⚫ .  ⚪ .  ⚫ .  .  .  .  .  .  .  .  .
     8  .  .  .  ⚫ ⚪ ⚪ ⚫ .  .  .  .  .  .  .  .  .  .  .  .
     9  .  ⚪ ⚪ ⚪ ⚫ ⚪ ⚪ ⚫ .  .  .  .  .  .  .  .  .  .  .
    10  .  ⚫ ⚪ ⚫ ⚫ ⚫ ⚪ ⚪ ⚫ *  ⚫ .  .  .  .  *  .  ⚫ .
    11  .  ⚪ ⚫ ⚫ ⚪ .  ⚫ ⚪ ⚪ ⚫ .  ⚫ ⚫ .  ⚫ .  ⚫ ⚪ .
    12  .  ⚪ ⚪ ⚫ .  ⚫ ⚫ ⚫ ⚪ ⚪ ⚫ ⚫ .  ⚫ .  ⚫ .  ⚪ ⚪
    13  .  ⚪ ⚪ ⚫ ⚫ ⚪ ⚪ ⚫ ⚫ ⚪ ⚪ ⚫ .  ⚫ ⚫ ⚪ ⚪ ⚪ .
    14  ⚫ ⚪ ⚫ ⚫ ⚪ ⚪ ⚫ ⚫ .  ⚫ ⚪ ⚫ .  ⚫ .  ⚫ ⚫ ⚫ ⚪
    15  .  ⚫ .  ⚫ ⚪ ⚫ ⚫ ⚪ ⚫ .  ⚪ ⚫ ⚫ ⚫ ⚫ ⚪ ⚪ ⚫ ⚪
    16  ⚫ ⚫ ⚫ ⚪ ⚪ ⚫ ⚪ ⚪ ⚫ ⚪ ⚪ .  ⚪ .  .  ⚪ ⚪ ⚫ .
    17  ⚫ ⚪ ⚫ ⚪ .  ⚪ ⚪ .  ⚫ ⚪ .  ⚪ .  ⚫ ⚫ ⚫ ⚪ ⚫ ⚫
    18  .  ⚪ ⚫ ⚪ .  ⚪ ⚫ ⚫ ⚫ ⚫ ⚪ .  .  .  .  .  ⚪ ⚪ ⚫
    19  ⚪ .  ⚪ ⚪ ⚪ .  ⚪ .  .  ⚪ .  .  .  .  .  .  .  ⚪ ⚪
        A  B  C  D  E  F  G  H  J  K  L  M  N  O  P  Q  R  S  T

Alternatively, sente also provides the ``get_default_sequence()`` method, which returns the sequence of moves rather than playing them out.
Because the sequence of moves is a python list, it is subject to python list slicing, which can be used to play the first X moves of the game if desired.

.. code-block:: python

    >>> game.advance_to_root()
    >>> sequence = game.get_default_sequence()
    >>> game.play_sequence(sequence[:95]) # Lee Sedol's famous ladder sequence
    >>> print(game)
     1  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .
     2  .  ⚪ ⚫ ⚪ .  .  .  .  .  .  .  .  .  .  .  .  .  .  .
     3  .  .  ⚪ ⚫ ⚫ .  .  .  .  .  .  .  .  ⚫ .  .  .  .  .
     4  .  .  .  ⚪ .  ⚫ .  .  .  *  .  .  .  .  .  ⚫ .  .  .
     5  .  .  ⚪ ⚪ .  .  .  .  .  .  .  .  .  .  .  .  .  .  .
     6  .  .  ⚪ ⚫ ⚫ .  .  .  .  .  .  .  .  .  .  .  .  .  .
     7  .  .  ⚫ ⚪ ⚪ ⚫ .  .  .  .  .  .  .  .  .  .  .  .  .
     8  .  .  .  ⚫ ⚪ ⚪ ⚫ .  .  .  .  .  .  .  .  .  .  .  .
     9  .  .  .  .  ⚫ ⚪ ⚪ ⚫ .  .  .  .  .  .  .  .  .  .  .
    10  .  .  .  *  .  ⚫ ⚪ ⚪ ⚫ *  .  .  .  .  .  *  .  ⚫ .
    11  .  .  .  .  .  .  ⚫ ⚪ ⚪ ⚫ .  .  .  .  ⚫ .  ⚫ ⚪ .
    12  .  .  .  .  .  .  .  ⚫ ⚪ ⚪ ⚫ .  ⚪ ⚫ .  ⚫ .  ⚪ ⚪
    13  .  .  .  .  .  .  .  .  ⚫ ⚪ ⚪ ⚫ ⚪ ⚫ ⚫ ⚪ ⚪ ⚪ .
    14  .  .  .  .  .  .  .  .  .  ⚫ ⚪ ⚫ ⚪ ⚪ ⚪ ⚫ ⚫ ⚫ ⚪
    15  .  .  ⚪ .  .  .  .  .  .  .  .  ⚫ ⚫ ⚫ ⚫ ⚪ ⚪ ⚫ ⚪
    16  .  .  .  *  .  .  .  .  .  *  .  .  ⚪ .  .  ⚪ ⚪ ⚫ .
    17  .  .  .  ⚪ .  .  .  .  ⚫ .  .  .  .  ⚫ ⚫ ⚫ ⚪ ⚫ ⚫
    18  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  ⚪ ⚪ ⚫
    19  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  ⚪ .
        A  B  C  D  E  F  G  H  J  K  L  M  N  O  P  Q  R  S  T

.. doctest::
    :hide:

    >>> game.advance_to_root()
    >>> sequence = game.get_default_sequence()
    >>> game.play_sequence(sequence[:95]) # Lee Sedol's famous ladder sequence
    >>> print(game)
     1  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .
     2  .  ⚪ ⚫ ⚪ .  .  .  .  .  .  .  .  .  .  .  .  .  .  .
     3  .  .  ⚪ ⚫ ⚫ .  .  .  .  .  .  .  .  ⚫ .  .  .  .  .
     4  .  .  .  ⚪ .  ⚫ .  .  .  *  .  .  .  .  .  ⚫ .  .  .
     5  .  .  ⚪ ⚪ .  .  .  .  .  .  .  .  .  .  .  .  .  .  .
     6  .  .  ⚪ ⚫ ⚫ .  .  .  .  .  .  .  .  .  .  .  .  .  .
     7  .  .  ⚫ ⚪ ⚪ ⚫ .  .  .  .  .  .  .  .  .  .  .  .  .
     8  .  .  .  ⚫ ⚪ ⚪ ⚫ .  .  .  .  .  .  .  .  .  .  .  .
     9  .  .  .  .  ⚫ ⚪ ⚪ ⚫ .  .  .  .  .  .  .  .  .  .  .
    10  .  .  .  *  .  ⚫ ⚪ ⚪ ⚫ *  .  .  .  .  .  *  .  ⚫ .
    11  .  .  .  .  .  .  ⚫ ⚪ ⚪ ⚫ .  .  .  .  ⚫ .  ⚫ ⚪ .
    12  .  .  .  .  .  .  .  ⚫ ⚪ ⚪ ⚫ .  ⚪ ⚫ .  ⚫ .  ⚪ ⚪
    13  .  .  .  .  .  .  .  .  ⚫ ⚪ ⚪ ⚫ ⚪ ⚫ ⚫ ⚪ ⚪ ⚪ .
    14  .  .  .  .  .  .  .  .  .  ⚫ ⚪ ⚫ ⚪ ⚪ ⚪ ⚫ ⚫ ⚫ ⚪
    15  .  .  ⚪ .  .  .  .  .  .  .  .  ⚫ ⚫ ⚫ ⚫ ⚪ ⚪ ⚫ ⚪
    16  .  .  .  *  .  .  .  .  .  *  .  .  ⚪ .  .  ⚪ ⚪ ⚫ .
    17  .  .  .  ⚪ .  .  .  .  ⚫ .  .  .  .  ⚫ ⚫ ⚫ ⚪ ⚫ ⚫
    18  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  ⚪ ⚪ ⚫
    19  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  ⚪ .
        A  B  C  D  E  F  G  H  J  K  L  M  N  O  P  Q  R  S  T

Saving Games
------------

Once a game has been played out, the ``sgf.dump()`` function can save a SGF of the file.
If it is desired to add SGF metadata to the file, such as the player's names and ranks, it must be done before dumping the game.
See :ref:`SGF Metadata` for more details

.. code-block:: python

    >>> game.play_sequence(long_sequence)
    >>> sgf.dump(game, "my game.sgf")


``loads`` and ``dumps``
-----------------------

SGF files are a kind of `raw text file <https://en.wikipedia.org/wiki/Plain_text>`_ similarly to ``.py``, ``.csv`` and ``.json`` files.
Because of this, Sente's internal file reader can decode plain text, and the sgf module provides this utility in the form of the ``sgf.loads`` and ``sgf.dumps`` functions.
This is similar to how python's built-in `json library <https://docs.python.org/3/library/json.html>`_ works.
