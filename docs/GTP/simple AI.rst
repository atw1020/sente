Building a Simple AI with the sente GTP module
==============================================

One of the required GTP commands that all programs that
implement GTP are required to implement is the ``genmove``
command. This command asks the associated engine to make
a move. Sente provides utilities that allow you to change
the functionality of the ``genmove`` command with custom
code.

In this tutorial, we will learn how to override ``genmove``
and use it to create a simple GTP AI that plays random
moves using sente.

Setting Up the Python file
--------------------------

To begin, let's set up a GTP interpreter in a python
file. We will copy most of the code from the previous
section :ref:`GTP-shell-label`

.. code-block:: python
    :linenos:

    """

    Author: Arthur Wesley

    """

    from sente import GTP

    def main():
        """

        main method

        """

        session = GTP.Session("random_move", "0.0.1")

        print("starting up random move...")

        while session.active():

            response = session.interpret(input(">> "))
            print(response)


    if __name__ == "__main__":
        main()

The above is equivalent to the code in the aforementioned
section, only with proper python formatting. Running the
file will open a

.. code-block::

    $ python random_move.py
    starting up random move...
    >>


However, so far we haven't changed the functionality of
the ``genmove`` command. Trying to call the genmove command
with a GTP session where no GenMove has been registered
will cause an exception

.. code-block::

    $ python random_move.py
    starting up random move...
    >> genmove B
    Traceback (most recent call last):
      File "random_move.py", line 27, in <module>
        main()
      File "random_move.py", line 22, in main
        print(session.interpret(input(">> "))
    RuntimeError: genmove has not been implemented by this engine, please register a valid function

Overwriting Genmove
-------------------

The ``genmove`` command is overwritten by adding the
``Session.GenMove`` decorator to the function, which
registers it internally to be called whenever the GTP
interpreter receives the ``genmove`` commmand.

To begin, we define a function that returns a random legal
move. The ``sente.game`` object has a handy method that
generates a list of all legal moves. We can obtain the
current game object the GTP Engine uses and then use
the ``random.choice`` function to choose a random move

.. code-block:: python
    :linenos:
    :emphasize-lines: 7, 22-30

    """

    Author: Arthur Wesley

    """

    import random

    from sente import GTP

    def main():
        """

        main method

        """

        session = GTP.Session("random_move", "0.0.1")

        print("starting up random move...")

        def gen_move():
            """

            generates a random move

            :return: random move
            """

            return random.choice(session.game.get_legal_moves())

        while session.active():

            response = session.interpret(input(">> "))
            print(response)


    if __name__ == "__main__":
        main()

Next, we need to add the decorator to the ``gen_move``
method to register it and override the default ``genmove``
command.

Importantly, the ``Session.GenMove`` decorator only
accepts functions that have
`typing hints <https://docs.python.org/3/library/typing.html>`_.
This is because GTP is a strongly typed protocol meaning that
only predefined data types can be transferred. Additionally,
the `GTP Specifications <https://www.lysator.liu.se/~gunnar/gtp/gtp2-spec-draft2.pdf#page=20>`_
requires that the genmove command the following signature:

:param color: Color of the player to generate a move for
:return: Move object containing the desired move

Adding the decorators and type hints we get the following code:

.. code-block:: python
    :linenos:
    :emphasize-lines: 22, 23, 28

    """

    Author: Arthur Wesley

    """

    import random

    from sente import GTP

    def main():
        """

        main method

        """

        session = GTP.Session("random_move", "0.0.1")

        print("starting up random move...")

        @session.GenMove
        def gen_move(color: sente.stone) -> sente.Move:
            """

            generates a random move

            :param color: Color of the player to generate a move for
            :return: Move object containing the desired move
            """

            return random.choice(session.game.get_legal_moves())

        while session.active():

            response = session.interpret(input(">> "))
            print(response)


    if __name__ == "__main__":
        main()

.. note:: you do not need to play the generated move into
    the session's ``sente.Game`` object. Sente does this
    automatically

Connecting the AI to Sabaki
---------------------------


