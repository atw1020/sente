"""

Author: Arthur Wesley

"""
import typing
import inspect

import sente
from sente import gtp


def main():
    """

    main method

    :return:
    """

    session = gtp.Session("OctalPus", "0.0.1")

    @session.Command
    def echo(text: str) -> typing.Union[typing.Tuple[bool, str], str]:
        return True, text

    @session.Command
    def fnord(fjord: float) -> typing.Tuple[bool, str]:
        return True, str(-fjord)

    """@session.genmove
    def genmove(color: sente.stone) -> sente.Vertex:
        return sente.Vertex(4, 4)"""

    while session.active():
        print(session.interpret(input(">> ")))

    print(session.get_sequence())


if __name__ == "__main__":
    main()
