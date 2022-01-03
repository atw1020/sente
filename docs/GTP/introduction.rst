GTP Introduction
================

The `go text protocol <https://www.lysator.liu.se/~gunnar/gtp/gtp2-spec-draft2.pdf>`_
(GTP) is a way that go programs can
communicate with each other and is human readable. It can
be used by go programs to play each other, play a human or
communicate with a go program such as
`CGoban <https://www.gokgs.com/download.jsp>`_ or
`Sabaki <https://sabaki.yichuanshen.de>`_.

.. note:: Sente currently only provides resources to create
    the *engine* side of GTP, but not the *host* side. Users
    who wish to create a GTP host are encouraged to build
    their own system using the python
    `subprocess <https://docs.python.org/3/library/subprocess.html>`_
    library

GTP Overview
------------

GTP works similarly to a computer terminal or python
interactive shell, text commands
are issued to an interpreter which executes them upon
receiving a newline. A list of commands and their meanings
can be seen below:

.. list-table:: GTP Commands
    :widths: 30 70
    :header-rows: 1

    * - **Command**
      - **Description**
    * - play [Color: sente.Color] [Vertex: sente.Vertex]
      - Plays a [Color] stone at [Vertex]
    * - boardsize [size: int]
      - Sets the size of the board to be [size]
    * - name
      - Returns the name of the go engine (ie. "leela-zero")
    * - version
      - Returns the version of the engine (ie. "0.17")
    * - clear_board
      - Clears the board
    * - undo [moves: int] (optional)
      - Undoes the last played move
    * - list_commands
      - Lists all of the available commands
    * - known_command
      - checks to see if a given command is known
    * - komi [new komi: float]
      - Sets the komi of the match
    * - showboard
      - Print a representation of the board
    * - genmove
      - asks the engine to generate a move
    * - protocol_version
      - the version of GTP being used
    * - loadsgf [moves: int] (optional)
      - loads an SGF file and plays `moves` moves
    * - quit (also exit)
      - quits the session

.. doctest:: python
    :hide:

    >>> from sente import GTP
    >>> session = GTP.Session()
    >>> print(session.interpret("list_commands"))
    = play
    boardsize
    name
    exit
    clear_board
    undo
    list_commands
    komi
    quit
    showboard
    genmove
    known_command
    protocol_version
    version
    loadsgf
    <BLANKLINE>
    <BLANKLINE>
    <BLANKLINE>

Creating a simple GTP Interpreter
---------------------------------

The Sente GTP Module contains the `GTP.Session` object to
create Engines that can communicate using GTP.

.. code-block:: python

    >>> from sente import GTP
    >>> session = GTP.Session("my_engine_name", "0.0.1")

The `GTP.Session.interpret` method interprets GTP commands
that are passed to it. For example, we can call the `name`
and `version` commands from the table above

.. code-block:: python

    >>> print(session.interpret("name"))
    = my_engine_name


    >>> print(session.interpret("version"))
    = 0.0.1


    >>>

.. doctest::
    :hide:

    >>> from sente import GTP
    >>> session = GTP.Session("my_engine_name", "0.0.1")
    >>> print(session.interpret("name"))
    = my_engine_name
    <BLANKLINE>
    <BLANKLINE>
    >>> print(session.interpret("version"))
    = 0.0.1
    <BLANKLINE>
    <BLANKLINE>

*The extra newlines printed are an official part of the GTP
standard and should not be removed.*

Let's do some Go!

We can play some moves using the `play` command and
show the board using the `showboard` command.




