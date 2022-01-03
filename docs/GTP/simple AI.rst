Building a Simple AI with the Sente GTP Module
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
section :ref:`GTP-shell-label`.

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
file will open a GTP session.

.. code-block::

    (venv) $ python random_move.py
    starting up random move...
    >>


However, so far we haven't changed the functionality of
the ``genmove`` command. Trying to call the genmove command
with a GTP session where no GenMove has been registered
will cause an exception:

.. code-block::

    (venv) $ python random_move.py
    starting up random move...
    >> genmove B
    Traceback (most recent call last):
      File "random_move.py", line 28, in <module>
        main()
      File "random_move.py", line 23, in main
        response = session.interpret(input(">> "))
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
the ``random.choice`` function to choose a random move.

.. code-block:: python
    :linenos:
    :emphasize-lines: 7, 23-31

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
the `GTP Specifications, page 19 <https://www.lysator.liu.se/~gunnar/gtp/gtp2-spec-draft2.pdf#page=20>`_
requires that the genmove command the following signature:

:param color: Color of the player to generate a move for
:return: Move object containing the desired move

.. note:: the color argument is ignored here for
    the sake of simplicity, but in general it
    should not be ignored.

Adding the decorators and type hints we get the following code:

.. code-block:: python
    :linenos:
    :emphasize-lines: 9, 24, 25, 30

    """

    Author: Arthur Wesley

    """

    import random

    import sente
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

We can now run our program and watch our AI generate hillariously
awful moves

.. code-block::

    (venv) $ python random_move.py
    starting up random move...
    >> genmove B
    = K18

.. note:: you do not need to play the generated move into
    the session's ``sente.Game`` object. Sente does this
    automatically

Connecting the AI to Sabaki
---------------------------

.. note:: the following instructions are based on a Unix
    environment. If using windows, adapt accordingly
    (use backslashes, batch files, etc. instead)

In order for another Go program to talk to this AI, the
host program needs to spawn our program as a subprocess.
In this example, we will see how this is done for
`Sabaki <https://sabaki.yichuanshen.de>`_, a popular
Go GUI program.

Sabaki only executes one system command to spawn the
engine subprocess. Unfortunately, if your engine's
libraries are stored in a virtual environment it will
take multiple system commands to activate the environment
and run the program. a simple way around this is to write
a short shell script that activates and runs the engine.

.. code-block:: bash
    :linenos:

    #!/bin/bash

    cd "~/path/to/your/project" # absolute path, since the current directory of Sabaki is arbitrary
    source venv/bin/activate
    python random_move.py

With this shell script created, the steps to run the
engine in Sabaki are as follows.

1. Open the ``Engines Sidebar`` from the ``Engines Menu``

.. image:: ../_static/sabaki\ screenshots/Open\ "Engines\ Sidebar".png

2. Click on the ``Attach Engine...`` button and select "Manage Engines"

.. image:: ../_static/sabaki\ screenshots/Open\ "Manage\ Engines".png

3. Click "Add"

.. image:: ../_static/sabaki\ screenshots/Add\ Engine.png

4. Name the AI ``Random Move`` and for the path, put the path to your shell script.

.. image:: ../_static/sabaki\ screenshots/Add\ Random\ Move.png

5. Close the "Manage Engines" Window. "Random Move" should now appear under the options for ``Attach Engine``

.. image:: ../_static/sabaki\ screenshots/Random\ Move\ Under\ "Attatch\ Engine...".png

6. Run "Random Move"

.. image:: ../_static/sabaki\ screenshots/Run\ Random\ Move.png

7. Assign Random Move to be the White Player

.. image:: ../_static/sabaki\ screenshots/Set\ as\ White\ Player.png

8. Begin Play

.. image:: ../_static/sabaki\ screenshots/Play.png
