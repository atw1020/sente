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

The Basic element in Sente is the ``sente.Game`` object which represents a Simple Game.

.. code-block:: python

    >>> import sente
    >>> game = sente.Game()

By default, sente creates a 19x19 game with Chinese Rules.
9x9 and 13x13 board sizes and Japanese rules can be specified if desired.

.. code-block:: python

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


The Sente board display is designed to be similar to the `gomill ascii boards output <https://mjw.woodcraft.me.uk/gomill/doc/0.7/ascii_boards.html>`_.
A hash (``#``) denotes a Black stone and an "O" (``O``) denotes a white stone.
One difference between sente and gomill is that in sente, the star points are denoted using an asterisk (``*``)

Finally, Sente also provides the ability to check if a move is legal using the `is_legal()` method.

.. code-block:: python

    >>> import sente
    >>> game = sente.Game()
    >>> game.play(4, 4)
    >>> game.is_legal(4, 4)
    False

