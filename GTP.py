"""

Author: Arthur Wesley

"""
import typing
import inspect

import sente
from sente import GTP


def test() -> int:
    return "hi"


def main():
    """

    main method

    :return:
    """

    session = GTP.Session("OctalPus", "0.0.1")

    @session.GenMove
    def genmove(color: sente.stone) -> sente.Vertex:

        # obtain the current game state
        game = sente.Game()
        game.play_sequence(session.get_sequence())

    while session.active():
        print(session.interpret(input(">> ")))

    print(session.get_sequence())


if __name__ == "__main__":
    main()
