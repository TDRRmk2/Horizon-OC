
"""

HOC Configurator

Copyright (C) Souldbminer

This program is free software; you can redistribute it and/or modify it
under the terms and conditions of the GNU General Public License,
version 2, as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

"""


class Defaults: # This almost always never needs to be updated as pulling from the kip takes priority
    def __init__(self):
        self.autosave = 0
        self.custrev = 0
        self.mtc = 0
        self.commonCpuBoostClock = 1785000
        self.commonEmcMemVolt = 1175000
        self.eristaCpuMaxVolt = 1235
        self.eristaEmcMaxClock = 1862400
        self.marikoCpuMaxVolt = 1120
        self.marikoEmcMaxClock = 1996800
        self.marikoEmcVddqVolt = 600000
        self.marikoCpuUV = 0
        self.marikoGpuUV = 0
        self.eristaCpuUV = 0
        self.eristaGpuUV = 0
        self.commonGpuVoltOffset = 0
        self.marikoEmcDvbShift = 0
        self.t1_tRCD = 0
        self.t2_tRP = 0
        self.t3_tRAS = 0
        self.t4_tRRD = 0
        self.t5_tRFC = 0
        self.t6_tRTW = 0
        self.t7_tWTR = 0
        self.t8_tREFI = 0
        self.mem_burst_latency = 2
        self.m_freq_76800 = 600
        self.m_freq_153600 = 600
        self.m_freq_230400 = 600
        self.m_freq_307200 = 600
        self.m_freq_384000 = 600
        self.m_freq_460800 = 600
        self.m_freq_537600 = 600
        self.m_freq_614400 = 600
        self.m_freq_691200 = 600
        self.m_freq_768000 = 600
        self.m_freq_844800 = 605
        self.m_freq_921600 = 635
        self.m_freq_998400 = 665
        self.m_freq_1075200 = 695
        self.m_freq_1152000 = 730
        self.m_freq_1228800 = 760
        self.m_freq_1267200 = 785
        self.m_freq_1305600 = 800
        self.m_freq_1344000 = 0
        self.m_freq_1382400 = 0
        self.m_freq_1420800 = 0
        self.m_freq_1459200 = 0
        self.m_freq_1497600 = 0
        self.m_freq_1536000 = 0
        self.high_vddq = 0
        self.high_vdd2 = 0

d = Defaults()