"""

Author: Arthur Wesley

"""
import inspect

import sente
from sente import gtp


class OctopusGarden(gtp.Engine):

    def __init__(self):
        super().__init__("Ceph")
        print(inspect.getfullargspec(self.echo))
        self.register_command(self.echo)

    def echo(self, text: str):
        return True, text


def main():
    """

    main method

    :return:
    """

    ceph = OctopusGarden()

    while ceph.active():
        response = ceph.interpret(input(">> "))
        print(response, end="")


if __name__ == "__main__":
    main()
