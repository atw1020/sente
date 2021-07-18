The ``sgf`` (Smart Game Format) Module
======================================

.. warning:: the Sente SGF parser is currently highly specialized **only stone placement data** and ignores lots of data included in SGF files including messages and metadata.

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

    * - Code
      - Meaning
    * - FF
      - SGF format standard (usually SGF 4)
    * - CA
      - File encoding (usually UTF-8)
    * - GM
      - The game specification (SGF is used for
    * - FF
      - SGF format standard (usually SGF 4)
    * - FF
      - SGF format standard (usually SGF 4)



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

    >>> game.get_branches()
    [<sente.Move B[dp]>]

It can be quite tedious to play through every move by calling ``get_branches()`` and selecting the first branch every time, so sente procides the ``get_default_sequence()`` and ``play_default_sequence()`` methods to make it easier to play through games.
If we wish to play the game all the way through to Hong Jansik's resignation at move 212, we can use the ``play_default_sequence()`` method to play out the main branch of the game.

.. code-block:: python

    >>> game.play_default_sequence()
    >>> print(game)
     1  .  .  O  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .
     2  .  O  .  O  #  .  .  .  O  .  #  .  .  .  .  .  .  O  #
     3  .  .  O  #  #  .  .  O  .  O  #  .  .  #  #  O  O  #  .
     4  .  .  .  O  .  #  .  O  O  #  O  .  .  .  .  #  #  .  #
     5  .  .  O  O  .  .  .  .  .  #  .  .  .  .  .  .  O  #  #
     6  .  .  O  #  #  .  .  .  .  .  .  .  .  .  .  .  .  .  .
     7  .  .  #  O  O  #  .  O  .  #  .  .  .  .  .  .  .  .  .
     8  .  .  .  #  O  O  #  .  .  .  .  .  .  .  .  .  .  .  .
     9  .  O  O  O  #  O  O  #  .  .  .  .  .  .  .  .  .  .  .
    10  .  #  O  #  #  #  O  O  #  *  #  .  .  .  .  *  .  #  .
    11  .  O  #  #  O  .  #  O  O  #  .  #  #  .  #  .  #  O  .
    12  .  O  O  #  .  #  #  #  O  O  #  #  .  #  .  #  .  O  O
    13  .  O  O  #  #  O  O  #  #  O  O  #  .  #  #  O  O  O  .
    14  #  O  #  #  O  O  #  #  .  #  O  #  .  #  .  #  #  #  O
    15  .  #  .  #  O  #  #  O  #  .  O  #  #  #  #  O  O  #  O
    16  #  #  #  O  O  #  O  O  #  O  O  .  O  .  .  O  O  #  .
    17  #  O  #  O  .  O  O  .  #  O  .  O  .  #  #  #  O  #  #
    18  .  O  #  O  .  O  #  #  #  #  O  .  .  .  .  .  O  O  #
    19  O  .  O  O  O  .  O  .  .  O  .  .  .  .  .  .  .  O  O
        A  B  C  D  E  F  G  H  J  K  L  M  N  O  P  Q  R  S  T

Alternatively, sente also provides the ``get_default_sequence()`` method which returns the sequence of moves rather than playing them out.
Because the sequence of moves is a python list, it is subject to python list slicing which can be used to play the first X moves of the game if desired.

.. code-block:: python

    >>> game.advance_to_root()
    >>> sequence = game.get_default_sequence()
    >>> game.play_sequence(sequence[:95]) # Lee Sedol's famous ladder sequence
    >>> print(game)
     1  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .
     2  .  O  #  O  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .
     3  .  .  O  #  #  .  .  .  .  .  .  .  .  #  .  .  .  .  .
     4  .  .  .  O  .  #  .  .  .  *  .  .  .  .  .  #  .  .  .
     5  .  .  O  O  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .
     6  .  .  O  #  #  .  .  .  .  .  .  .  .  .  .  .  .  .  .
     7  .  .  #  O  O  #  .  .  .  .  .  .  .  .  .  .  .  .  .
     8  .  .  .  #  O  O  #  .  .  .  .  .  .  .  .  .  .  .  .
     9  .  .  .  .  #  O  O  #  .  .  .  .  .  .  .  .  .  .  .
    10  .  .  .  *  .  #  O  O  #  *  .  .  .  .  .  *  .  #  .
    11  .  .  .  .  .  .  #  O  O  #  .  .  .  .  #  .  #  O  .
    12  .  .  .  .  .  .  .  #  O  O  #  .  O  #  .  #  .  O  O
    13  .  .  .  .  .  .  .  .  #  O  O  #  O  #  #  O  O  O  .
    14  .  .  .  .  .  .  .  .  .  #  O  #  O  O  O  #  #  #  O
    15  .  .  O  .  .  .  .  .  .  .  .  #  #  #  #  O  O  #  O
    16  .  .  .  *  .  .  .  .  .  *  .  .  O  .  .  O  O  #  .
    17  .  .  .  O  .  .  .  .  #  .  .  .  .  #  #  #  O  #  #
    18  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  O  O  #
    19  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  O  .
        A  B  C  D  E  F  G  H  J  K  L  M  N  O  P  Q  R  S  T

Saving Games
------------

Once a game has been played out, the ``sgf.dump()`` function can be used to save a SGF of the file.

.. code-block:: python

    >>> game.play_sequence(long_sequence)
    >>> sgf.dump(game, "my game.sgf")
