#
# This file is part of LiteX.
#
# Copyright (c) 2019 Florent Kermarrec <florent@enjoy-digital.fr>
# SPDX-License-Identifier: BSD-2-Clause

import unittest

from migen import *

from litex.soc.cores.icap import *

iprog_sequence = [
    # csib rdwrb data
    "0 0 0xaa995566",
    "0 0 0x20000000",
    "0 0 0x20000000",
    "0 0 0x30008001",
    "0 0 0x0000000f",
    "0 0 0x20000000",
    "0 0 0x20000000",
    "0 0 0x30008001",
    "0 0 0x0000000d",
    "0 0 0x20000000",
    "0 0 0x20000000",
]

class TestICAP(unittest.TestCase):
    def test_icap_command_reload(self):
        def generator(dut):
            yield dut.addr.eq(ICAPRegisters.CMD)
            yield dut.data.eq(ICAPCMDs.IPROG)
            for i in range(8):
                yield
            yield dut.send.eq(1)
            yield
            yield dut.send.eq(0)

        def check(dut):
            while (yield dut._i) != ICAP_SYNC:
                yield
            for ref_w in iprog_sequence:
                cur_w = f"{(yield dut._csib)} {(yield dut._rdwrb)} 0x{(yield dut._i):08x}"
                self.assertEqual(ref_w, cur_w)
                #print(cur_w)
                yield


        dut = ICAP(with_csr=False, simulation=True)
        clocks = {"sys": 10, "icap": 10}
        run_simulation(dut, [generator(dut), check(dut)], clocks, vcd_name="icap.vcd")

    def test_icap_bitstream_syntax(self):
        dut = ICAPBitstream(simulation=True)
