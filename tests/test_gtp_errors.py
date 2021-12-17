"""

Author: Arthur Wesley

"""


from unittest import TestCase

from sente import gtp


class CommandFunctionality(TestCase):

    def test_version(self):
        """

        makes sure that the version command operates correctly

        :return:
        """

        host = gtp.GTPHost()

        self.assertEqual("1 protocol_version\n=1 2\n\n", host.evaluate("1 protocol_version\n"))

