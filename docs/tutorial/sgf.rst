The ``sgf`` (Smart Game Format) Module
======================================

.. warning:: the Sente SGF parser is currently highly specialized **only stone placement data** and ignores lots of data included in SGF files including game messages, triangle, circle and other markers.

The ``sente.sgf`` module contains functions for parsing SGF (Smart Game Format) Files.
SGF is a standard encoding for go games that is capable of encoding "forked" go games in addition to a raw sequence of moves.
SGF files can be viewed in programs like `CGoban <https://www.gokgs.com/download.jsp>`_ or `Sabaki <https://sabaki.yichuanshen.de>`_.

Loading Metadata
-------------------------

Typically SGF files begin with a section of metadata that contains information not directly relevant to the file itself.
This metadata includes information such as the names of the players, the date that the game was played and the file encoding.
Most of this information is not needed to load a game into sente, but may be of interest to someone using sente.
Sente provides the ``sgf.get_metadata`` function to extract this data from a file.

.. code-block:: python

    >>> from sente import sgf
    >>> metadata = sgf.get_metadata("3-4.sgf") # a simple 3-4 joseki
    >>> print(metadata) # note: the following output was produced using the python pretty-print library
    {   'AP': 'CGoban:3',
        'CA': 'UTF-8',
        'FF': '4',
        'GM': '1',
        'KM': '0.00',
        'PB': 'Black',
        'PW': 'White',
        'RU': 'Japanese',
        'ST': '2',
        'SZ': '19'}

We can see that each field in the metadata is represented by a two-capitol letter code such as ``FF`` or ``KM``.
The table gives a summary of some of the more common metadata parameters.
A full list of metadata parameters can be found on the `sgf specifications page <https://www.red-bean.com/sgf/>`_.

.. list-table:: Metadata Parameters
    :widths: 10 90
    :header-rows: 1

    * - **Code**
      - **Meaning**
    * - FF
      - SGF format standard (usually SGF 4)
    * - CA
      - File encoding (usually UTF-8)
    * - GM
      - The kind of game (Go = 1, Backgammon = 6, etc.)
    * - GN
      - The name of the game ("ie. Lee Sedol ladder game")
    * - KM
      - The Komi of the game
    * - PB
      - Player Black (the name of the black player ie. "Honinbo Shusaku"
    * - BR
      - The black player's rank (ie. 7k)
    * - PW
      - Player White (the name of the white player ie. "Gennan Inseki")
    * - WR
      - The white player's rank (ie. 9p)
    * - RU
      - The ruleset to use (ie. "Chinese")
    * - SZ
      - The size of the board the game was played on (ie. 19)
    * - RE
      - The result of the match (ie. "B+3.5")

Loading Games
-------------

SGF files can be loaded into sente using the ``sgf.load`` function.
``sgf.load`` returns a ``sente.Game`` object with a move tree that has been populated with the game tree in the SGF file.

.. code-block:: python

    >>> from sente import sgf
    >>> game = sgf.load("Lee Sedol ladder game.sgf")

.. Note:: The ``SGF.load()`` function takes in a file path.

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

Saving Games
------------

Once a game has been played out, the ``sgf.dump()`` function can be used to save a SGF of the file.

.. code-block:: python

    >>> game.play_sequence(long_sequence)
    >>> sgf.dump(game, "my game.sgf")

As mentioned earlier, SGF files may contain metadata about the game such as Player names and the date that the match was played on.
Sente will automatically add some parameters such as the File encoding, Game result and game type.
Other metadata can be added using the ``metadata`` keyword argument which accepts a python dictionary that maps from Metadata codes described above to their values.

.. code-block:: python

    >>> game.play_sequence(long_sequence)
    >>> metadata = {
    ...     "PB": "AlphaGo"
    ...     "PW": "Ke Jie"
    ...     "DT": "2017-05-23"
    ... }
    >>> sgf.dump(game, "my game.sgf", metadata=metadata)


``loads`` and ``dumps``
-----------------------

SGF files are a kind of `raw text file <https://en.wikipedia.org/wiki/Plain_text>`_ similarly to ``.py``, ``.csv`` and ``.json`` files.
Because of this, Sente's internal file reader is capable of decoding plain text and the sgf module provides this utility in the from of the ``sgf.loads`` and ``sgf.dumps`` functions.
This is similar to how python's built-in `json library <https://docs.python.org/3/library/json.html>`_ works.

