Creating Private GTP Extensions
===============================

It is common to expand the Go Text Protocol with additional
commands that are not defined by the official spec.
Commands defined this way are called "private extensions"
and are outlined in `Section 2.13 <https://www.lysator.liu.se/~gunnar/gtp/gtp2-spec-draft2.pdf#page=8>`_
of the GTP 2 Spec.
`Leela zero <https://github.com/leela-zero/leela-zero>`_
for example defines a private extension command called
*analyze* that provides an analysis of the board. The
`Lizzie <https://github.com/featurecat/lizzie>`_ frontend for
leela zero then uses this command to generate it's
well-known analysis graphs.

The ``GTP.Session`` object provides utilities to define
private GTP extensions that can then be executed.

Making an Echo command
----------------------

GTP does not define an echo command as it has little
utility in GTP applications. However it makes a great
example of how to implement private GTP extensions in
sente.

To begin, we will use the basic GTP Code from the
previous section :ref:`GTP-shell-file-label`

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

        session = GTP.Session("engine", "0.0.1")

        while session.active():

            response = session.interpret(input(""))
            print(response)


    if __name__ == "__main__":
        main()

Private GTP extensions can be defined in sente using
decorators similar to those for ``Session.GenMove``.
Like ``Session.GenMove`` decorators, ``Session.Command``
decorators can only be applied to functions with python
type hints.

GTP Recognizes 7 different data types seen in the table
below. Sente maps each of these data types to a python
or sente data type seen in the second column. All
arguments and return types must match one of these for
sente to create a private GTP extension.

.. list-table:: GTP Variables
    :header-rows: 1

    * - **GTP Type**
      - **Python/Sente type**
    * - int
      - ``int``
    * - float
      - ``float``
    * - string
      - ``str``
    * - vertex
      - ``sente.Vertex``
    * - color
      - ``sente.stone``
    * - move
      - ``sente.Move``
    * - boolean
      - ``bool``

An echo command takes a string argument and returns
the same string. This is an extremely simple function
and we simply have to return the ``message`` argument

.. code-block:: python
    :linenos:
    :emphasize-lines: 19-28, 32

    """

    Author: Arthur Wesley

    """

    from sente import GTP


    def main():
        """

        main method

        """

        session = GTP.Session("engine", "0.0.1")

        @session.Command
        def echo(message: str) -> str:
            """

            a simple echo command

            :param message: message to echo
            :return: the message
            """
            return message

        while session.active():

            response = session.interpret(input(">> ")) # add the prompt back for debugging
            print(response)


    if __name__ == "__main__":
        main()

We can now run the program and and test our echo command

.. code-block:: bash

    $ python echo_command.py
    >> echo hello
    ? unknown command
    >>

Why wasn't the command echo recognized?

Naming conventions
------------------

`Section 2.13 <https://www.lysator.liu.se/~gunnar/gtp/gtp2-spec-draft2.pdf#page=8>`_
of the GTP spec advises that private GTP extensions
include the name of the engine followed by a dash ("-")
followed by the name of the command. Sente automatically
adds formats the names of it's commands to match this
pattern. Therefore, the echo command we have created
oauth to be "engine-echo". We can conform this
by running the "list_commands" command:

.. code-block:: bash

    $ python echo_command.py
    >> echo
    ? unknown command


    >> list_commands
    = play
    [...]
    engine-echo
    [...]
    loadsgf


    >>

therefore, we can run the echo command by
using the name "engine-echo"

.. code-block:: bash

    $ python echo_command.py
    >> engine-echo hello
    = hello


    >> engine-echo "hello world"
    = hello world

    >>

.. note:: Officially, GTP String literals are
    not allowed to have spaces in them. However
    the sente interpreter allows strings to with
    spaces in them so long as the strings are
    enclosed in quotes

Returning Error messages
------------------------

Sometimes it is desirable to add error messages when an
invalid variable is passed to a GTP command. For example,
the standard ``play`` command will error out if an
illegal move is requested.

GTP Commands that return custom error messages must
return a tuple containing two elements: a boolean
representing the status of the command and a GTP

For Example...

.. code-block:: python

    return True, "This is a successful status!"

...would indicate that the function has been completed
successfully. Meanwhile...

.. code-block:: python

    return False, "This is an unsuccessful status :("

...would indicate that the function has had an error.

Let's create a simple GTP command with error messages,
note that the return type is labeled as ``tuple``.

.. code-block:: python
    :emphasize-lines: 4

    session = GTP.Session("engine", "0.0.1")

    @session.Command
    def error_message(output: bool) -> tuple:
        if output:
            return True, "This is a successful status!"
        else:
            return False, "This is an unsuccessful status :("

When we run this code however, we get an error

.. code-block:: bash

    $ python error_message.py
    Traceback (most recent call last):
      File "error_message.py", line 37, in <module>
        main()
      File "error_message.py", line 20, in main
        def error_message(output: bool) -> tuple:
    TypeError: Custom GTP command returned invalid response, expected GTP compatible type, got <class 'tuple'>

As noted earlier, the ``error_message`` function is
labeled as returning a ``tuple``. By default, python
tuples are weakly typed, which violates the principle
that every
