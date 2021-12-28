"""

Author: Arthur Wesley

"""
import typing
import inspect

import sente
from sente import gtp


def Engine(engine):

    # get a list containing all the attributes
    commands = [name for name, value in engine.__dict__.items() if hasattr(value, "_sente_gtp_command")]
    print(commands)

    engine.interpret = lambda self, x: "= " + str(x)

    return engine


def Command(function):

    # add the attribute and return
    function._sente_gtp_command = True

    return function


@gtp.Engine
class OctopusGarden:

    def echo(self, text: str) -> typing.Union[typing.Tuple[bool, str], str]:
        return True, text

    @gtp.Command
    def fnord(self, fjord: float) -> typing.Tuple[bool, str]:
        return True, str(-fjord)


def main():
    """

    main method

    :return:
    """

    octalpus = OctopusGarden()
    print(octalpus.interpret("list_commands"))


if __name__ == "__main__":
    main()
