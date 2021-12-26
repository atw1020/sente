"""

Author: Arthur Wesley

"""
import typing

import sente
from sente import gtp


def my_decorator(function):

    annotations = function.__annotations__
    return_type = annotations["return"]

    attrs = typing.get_args(return_type)

    print(attrs[1])
    print(typing.get_args(attrs[0]))


class OctopusGarden(gtp.Engine):

    @gtp.Command
    # @my_decorator
    def echo(self, text: str) -> typing.Union[typing.Tuple[bool, str], str]:
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
