"""

Author: Arthur Wesley

"""
import typing
import inspect

import sente
from sente import gtp


@gtp.Engine("octopus", "0.0.1")
class OctopusGarden:

    @gtp.Command
    def echo(self, text: str) -> typing.Union[typing.Tuple[bool, str], str]:
        return True, text

    @gtp.Command
    def fnord(self, fjord: float) -> typing.Tuple[bool, str]:
        return True, str(-fjord)

    @gtp.command
    def genmove(self, color: sente.stone) -> sente.Vertex:
        return sente.Vertex(4, 4)


def main():
    """

    main method

    :return:
    """

    octalpus = OctopusGarden()
    print(octalpus.interpret("name"))
    print(octalpus.interpret("version"))


if __name__ == "__main__":
    main()
