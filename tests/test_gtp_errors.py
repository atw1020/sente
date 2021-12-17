"""

Author: Arthur Wesley

"""


from unittest import TestCase

from sente import gtp


class CommandFunctionality(TestCase):

    def test_id(self):
        """

        makes sure that the id of the instruction is passed correctly

        :return:
        """

        host = gtp.GTPHost()

        self.assertEqual("1 protocol_version\n=1 2\n\n", host.evaluate("1 protocol_version\n"))
        self.assertEqual("5 protocol_version\n=5 2\n\n", host.evaluate("5 protocol_version\n"))
        self.assertEqual("2 protocol_version\n=2 2\n\n", host.evaluate("2 protocol_version\n"))
        self.assertEqual("4294967295 protocol_version\n=4294967295 2\n\n", host.evaluate("4294967295 protocol_version\n"))

    def test_protocol_version(self):
        """

        makes sure that the version command operates correctly

        :return:
        """

        host = gtp.GTPHost()

        self.assertEqual("protocol_version\n= 2\n\n", host.evaluate("protocol_version\n"))

