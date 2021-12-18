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

    def test_name(self):
        """

        tests to see if the name command responds correctly

        :return:
        """

        host = gtp.GTPHost()

        self.assertEqual("name\n= Engine using Sente GTP\n\n", host.evaluate("name\n"))

        host2 = gtp.GTPHost("Ceph the octopus")
        self.assertEqual("name\n= Ceph the octopus\n\n", host2.evaluate("name\n"))

    def test_version(self):
        """

        tests to see if the version command responds correctly

        :return:
        """

        with open("../version.txt") as file:
            version = file.read().strip()

        host = gtp.GTPHost()

        self.assertEqual("version\n= " + version + "\n\n", host.evaluate("version\n"))

