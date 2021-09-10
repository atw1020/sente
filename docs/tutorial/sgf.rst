The ``sgf`` (Smart Game Format) Module
======================================


The ``sente.sgf`` module contains functions for parsing SGF (Smart Game Format) Files.
SGF is a standard encoding for go games that is capable of encoding "forked" go games in addition to a raw sequence of moves.
SGF files can be viewed in programs like `CGoban <https://www.gokgs.com/download.jsp>`_ or `Sabaki <https://sabaki.yichuanshen.de>`_.

.. note:: Relative to other SGF parsing tools, sente is a relatively strict enforcer of the SGF format and may reject SGF files that can be opened using other tools.

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
It populates It's internal game tree with the moves played but it does not actually play them on the board.
This means that after the game is loaded, the game will still have an empty board.
However, if the ``get_children`` method is called we can see that the opening move will be populated

.. code-block:: python

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
    [<sente.Move B[dp]>]

.. doctest::
    :hide:

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
    [<sente.Move B[pd]>]

It can be quite tedious to play through every move by calling ``get_branches()`` and selecting the first branch every time, so sente procides the ``get_default_sequence()`` and ``play_default_sequence()`` methods to make it easier to play through games.
If we wish to play the game all the way through to Hong Jansik's resignation at move 212, we can use the ``play_default_sequence()`` method to play out the main branch of the game.

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

Alternatively, sente also provides the ``get_default_sequence()`` method which returns the sequence of moves rather than playing them out.
Because the sequence of moves is a python list, it is subject to python list slicing which can be used to play the first X moves of the game if desired.

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

Once a game has been played out, the ``sgf.dump()`` function can be used to save a SGF of the file.
If it is desired to add SGF metadata to the file such as the player's names and ranks, it must be done before dumping the game.
See `Setting metadata`_ for more details

.. code-block:: python

    >>> game.play_sequence(long_sequence)
    >>> sgf.dump(game, "my game.sgf")


``loads`` and ``dumps``
-----------------------

SGF files are a kind of `raw text file <https://en.wikipedia.org/wiki/Plain_text>`_ similarly to ``.py``, ``.csv`` and ``.json`` files.
Because of this, Sente's internal file reader is capable of decoding plain text and the sgf module provides this utility in the from of the ``sgf.loads`` and ``sgf.dumps`` functions.
This is similar to how python's built-in `json library <https://docs.python.org/3/library/json.html>`_ works.

Metadata
--------

In addition to containing a record of the sequence of moves in a game, SGF files contain metadata associated with the games.
This metadata might include things like the name of the black player, the Komi the match was played with or a point on the board marked with a circle.
Each such piece of metadata is called a "property" in the SGF file format.
Each property has a two-capitol-letter code associated with it that uniquely identifies that property called the "property code".
For example, the "KM" property code refers to the place where the komi of the game is recorded.

A full list of all legal properties and descriptions of them may be found at `this red-bean archive <https://www.red-bean.com/sgf/properties.html>`_.
A partial list of properties is given below.

.. list-table:: SGF Properties
    :widths: 10 20 70
    :header-rows: 1

    * - **Code**
      - **Root or Node**
      - **Meaning**
    * - FF
      - Root
      - SGF format standard (usually SGF 4)
    * - KM
      - Root
      - The Komi of the game
    * - PB
      - Root
      - Player Black (the name of the black player ie. "Honinbo Shusaku"
    * - PW
      - Root
      - Player White (the name of the white player ie. "Gennan Inseki")
    * - RU
      - Root
      - The ruleset to use (ie. "Chinese")
    * - SZ
      - Root
      - The size of the board the game was played on (ie. 19)
    * - C
      - Node
      - The comment associated with the current SGF node.
    * - CR
      - Node
      - Mark a point with a Circle
    * - TR
      - Node
      - mark a point with a Triangle
    * - AB
      - Node
      - Add a Black stone at the specified point.
    * - AW
      - Node
      - Add a White stone at the specified point.

Sente divides properties into two categories: Root properties and Node properties

- **Root Properties:** information associated with the game as a whole.
    - KM (the amount of Komi associated with the game)
    - PB (the name of the person playing with the black stones)
    - RU (the ruleset of the game)
- **Node Properties:** information associated with the current node of the game tree.
    - C (puts a comment on the node)
    - CR (marks a point on the board with a circle)
    - AB (adds a white stone to the board, regardless of whose turn it currently is)

To obtain metadata properties from a ``sente.Game`` Object, simply call the ``get_properties``

Reading metadata
----------------

The ``Game.get_properties()`` method can be used to obtain metadata from a SGF file.

For example, say we have a sgf file containing a game record of the Honinbo Shusaku's famous "Ear-Reddening Game".

.. code-block:: python

    >>> game = sgf.load("ear reddening game.sgf")
    >>> game.get_properties()
    {'PB': 'Yasuda Shusaku',        # PB: Name of the black player
     'BR': '4d',                    # BR: Rank of the black player
     'PW': 'Inoue Gennan Inseki',   # PW: Name of the white player
     'RE': 'B+2',                   # RE: Result of the game (Black wins by 2 points)
     'WR': '8d',                    # WR: Rank of the white player
     'DT': '1846-09-11,14,15'}      # DT: Date(s) the game was played

As seen above, the resulting dictionary will map from the property code to the value associated with the property.
This value is typically a string, but in some cases may be a list of strings if the property is associated with multiple values.

The ``get_properties()`` method will return all of the root properties of the game as well as properties associated with the current active node in the SGF tree.
Thus, while the root properties will be the same no matter what move of the game you are on, the node properties will change as you step through a game.

Setting metadata
----------------

Sente can set the metadata properties of a ``Game`` object using the ``set_property()`` method.

.. code-block::

    >>> game = sente.Game()
    >>> game.set_property("PB", "Arthur Wesley")
    >>> game.set_property("PW", "Lucas Wesley")
    >>> game.get_properties()
    {'FF': '4',
     'SZ': '19',
     'PB': 'Arthur Wesley',
     'RU': 'Chinese',
     'PW': 'Lucas Wesley'}

.. doctest::
    :hide:

    >>> import sente
    >>> game = sente.Game()
    >>> game.set_property("PB", "Arthur Wesley")
    >>> game.set_property("PW", "Lucas Wesley")
    >>> game.get_properties()
    {'FF': '4', 'SZ': '19', 'PB': 'Arthur Wesley', 'RU': 'Chinese', 'PW': 'Lucas Wesley'}

As mentioned above, Sente strictly enforces conformity to the official SGF file format and custom SGF properties are not permitted as metadata.
Thus, trying to set a property not defined by the standard will result in an error.

.. code-block::

    >>> game = sente.Game()
    >>> game.set_property("JD", "Kaei 5-11-17")
    Traceback (most recent call last):
      File "<stdin>", line 1, in <module>
    sente.exceptions.InvalidSGFException: unknown SGF Property "JD"

.. doctest::
    :hide:

    >>> game = sente.Game()
    >>> game.set_property("JD", "Kaei 5-11-17")
    Traceback (most recent call last):
      File "<stdin>", line 1, in <module>
    sente.exceptions.InvalidSGFException: unknown SGF Property "JD"

.. note:: Although SGF files from the internet frequently contain the "JD" property (Japanese Date) it is not officially documented.

