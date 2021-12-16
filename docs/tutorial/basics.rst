Basics
======

Installation
------------

Sente runs on any operating system with python 3.x and can be installed via pip.

.. code-block:: bash

    $ pip install sente

-- or --

.. code-block:: bash

    $ python3 -m install sente

Getting Started
---------------

.. testsetup::

    import sente

The Basic element in Sente is the ``sente.Game`` object which represents a Simple Game.

.. code-block:: python

    >>> import sente
    >>> game = sente.Game()

.. doctest::
    :hide:

    >>> game = sente.Game()

By default, sente creates a 19x19 game with Chinese Rules.
9x9 and 13x13 board sizes and Japanese rules can be specified if desired.

.. code-block:: python

    >>> game = sente.Game(13)
    >>> game = sente.Game(19, sente.rules.JAPANESE)

.. doctest::
    :hide:

    >>> game = sente.Game(13)
    >>> game = sente.Game(19, sente.rules.JAPANESE)

.. warning:: Japanese rules may not be advisable because sente has no means of automatic dead stone detection (at present)

Moves can be played on the game using the ``play()`` method, and the board can be printed using the python ``print()`` function.

.. code-block:: python

    >>> game.play(4, 4)
    >>> game.play(16, 4)
    >>> game.play(4, 17)
    >>> print(game)
     1  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .
     2  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .
     3  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .
     4  .  .  .  ⚫ .  .  .  .  .  *  .  .  .  .  .  *  ⚫ .  .
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
    16  .  .  .  ⚪ .  .  .  .  .  *  .  .  .  .  .  *  .  .  .
    17  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .
    18  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .
    19  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .
        A  B  C  D  E  F  G  H  J  K  L  M  N  O  P  Q  R  S  T

The text created by printing a Sente board is designed to be similar to the [gomill ascii boards output](https://mjw.woodcraft.me.uk/gomill/doc/0.7/ascii_boards.html).
Unlike Gomill however, Sente uses Unicode characters to represent black and white stones to make complex board positions more visible and plot star points.

Finally, Sente also provides utilities for reading and saving SGF files using the `sente.SGF` module

.. code-block:: python

    >>> from sente import sgf
    >>> game = sgf.load("Lee Sedol ladder game.sgf")
    >>> move_sequence = game.get_default_sequence()[:95]
    >>> game.play_sequence(move_sequence)
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

