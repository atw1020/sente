"""

Author: Arthur Wesley

"""
import typing

import sente
from sente import gtp


class OctopusGarden(gtp.Engine):

    @gtp.Command
    def echo(self, text: str) -> typing.Tuple[bool, str]:
        return True, text

    def fnord(self, fjord: float):
        return True, str(-fjord)


def main():
    """

    main method

    :return:
    """

    print("hello world!")

    """ceph = OctopusGarden()

    while ceph.active():
        response = ceph.interpret(input(">> "))
        print(response, end="")"""


if __name__ == "__main__":
    main()
