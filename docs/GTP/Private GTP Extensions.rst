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

        session = GTP.Session("echo_command", "0.0.1")

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

        session = GTP.Session("echo_command", "0.0.1")

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
oauth to be "echo_command-echo". We can conform this
by running the "list_commands" command

.. code-block:: bash

    $ python echo_command.py
    >> echo
    ? unknown command


    >> list_commands
    = play
    [...]
    echo_command-echo
    [...]
    loadsgf



    >> 


