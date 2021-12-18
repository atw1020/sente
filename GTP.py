"""

Author: Arthur Wesley

"""

import sente
from sente import gtp


def main():
    """

    main method

    :return:
    """

    engine = gtp.Engine("Octopus' garden")

    while engine.active():
        response = engine.interpret(input(">> "))
        print(response, end="")


if __name__ == "__main__":
    main()
