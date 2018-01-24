#!/usr/bin/env python3
# Copyright (c) 2017 The Starwels developers
# Distributed under the MIT software license, see the accompanying
# file COPYING or http://www.opensource.org/licenses/mit-license.php.
"""Test starwels-cli"""
from test_framework.test_framework import StarwelsTestFramework
from test_framework.util import assert_equal

class TestStarwelsCli(StarwelsTestFramework):

    def set_test_params(self):
        self.setup_clean_chain = True
        self.num_nodes = 1

    def run_test(self):
        """Main test logic"""

        self.log.info("Compare responses from getinfo RPC and `starwels-cli getinfo`")
        cli_get_info = self.nodes[0].cli.getinfo()
        rpc_get_info = self.nodes[0].getinfo()

        assert_equal(cli_get_info, rpc_get_info)

if __name__ == '__main__':
    TestStarwelsCli().main()
