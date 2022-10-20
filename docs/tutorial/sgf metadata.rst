``sgf`` Metadata
============

In addition to containing a record of the sequence of moves in a game, SGF files contain metadata associated with the games.
This metadata might include things like the name of the black player, the Komi the match was played with, or a point on the board marked with a circle.
Each such piece of metadata is called a "property" in the SGF file format.
Each property has a two-capitol-letter code that uniquely identifies that property called the "property code."
For example, the "KM" property code refers to the place where the komi of the game is recorded.

A complete list of all legal properties and their descriptions may be found at `this red-bean archive <https://www.red-bean.com/sgf/properties.html>`_.
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

Reading metadata
----------------

The ``Game.get_properties()`` method can obtain metadata from an SGF file.

For example, say we have an SGF file containing a game record of the Honinbo Shusaku's famous "Ear-Reddening Game."

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
This value is typically a string, but in some cases, it may be a list of strings if the property is associated with multiple values.

The ``get_properties()`` method will return all of the root properties of the game as well as properties associated with the currently active node in the SGF tree.
Thus, while the root properties will be the same no matter what move of the game you are on, the node properties will change as you step through a game.

Setting metadata
----------------

Sente can set the metadata properties of a ``Game`` object using the ``set_property()`` method.

.. code-block::

    >>> game = sente.Game()
    >>> game.set_property("PB", "Arthur Wesley")
    >>> game.set_property("PW", "Lucas Wesley")
    >>> game.get_properties()
    {'SZ': '19',
     'PB': 'Arthur Wesley',
     'FF': '4',
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

As mentioned above, Sente strictly enforces conformity to the official SGF file format, and custom SGF properties are not permitted as metadata.
Thus, setting a property not defined by the standard will result in an error.

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

