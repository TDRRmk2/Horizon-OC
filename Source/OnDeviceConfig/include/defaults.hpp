/*
 * HOC Configurator - Default Values
 * Copyright (C) Dominatorul, Souldbminer
 */

#pragma once
#include <cstdint>

class KipHandler;

class Defaults {
public:
    template<typename T>
    static void initDefaults(T& data) {
        data.custRev = 0;
        data.mtcConf = 0;
        data.commonCpuBoostClock = 1785000;
        data.commonEmcMemVolt = 1175000;
        data.eristaCpuMaxVolt = 1235;
        data.eristaEmcMaxClock = 1862400;
        data.marikoCpuMaxVolt = 1120;
        data.marikoEmcMaxClock = 1996800;
        data.marikoEmcVddqVolt = 600000;
        data.marikoCpuUV = 0;
        data.marikoGpuUV = 0;
        data.eristaCpuUV = 0;
        data.eristaGpuUV = 0;
        data.commonGpuVoltOffset = 0;
        data.marikoEmcDvbShift = 0;
        
        // Memory timings
        data.t1_tRCD = 0;
        data.t2_tRP = 0;
        data.t3_tRAS = 0;
        data.t4_tRRD = 0;
        data.t5_tRFC = 0;
        data.t6_tRTW = 0;
        data.t7_tWTR = 0;
        data.t8_tREFI = 0;
        data.mem_burst_latency = 2;
        
        // Additional voltages
        data.marikoCpuVmin = 0;
        data.eristaGpuVmin = 0;
        data.marikoGpuVmin = 0;
        data.marikoGpuVmax = 0;
        
        // Initialize all GPU voltages to 600 (default safe value)
        data.g_volt_76800 = 600;
        data.g_volt_153600 = 600;
        data.g_volt_230400 = 600;
        data.g_volt_307200 = 600;
        data.g_volt_384000 = 600;
        data.g_volt_460800 = 600;
        data.g_volt_537600 = 600;
        data.g_volt_614400 = 600;
        data.g_volt_691200 = 600;
        data.g_volt_768000 = 600;
        data.g_volt_844800 = 605;
        data.g_volt_921600 = 635;
        data.g_volt_998400 = 665;
        data.g_volt_1075200 = 695;
        data.g_volt_1152000 = 730;
        data.g_volt_1228800 = 760;
        data.g_volt_1267200 = 785;
        data.g_volt_1305600 = 800;
        data.g_volt_1344000 = 0;
        data.g_volt_1382400 = 0;
        data.g_volt_1420800 = 0;
        data.g_volt_1459200 = 0;
        data.g_volt_1497600 = 0;
        data.g_volt_1536000 = 0;
        
        // Erista GPU voltages
        data.g_volt_e_76800 = 700;
        data.g_volt_e_153600 = 700;
        data.g_volt_e_230400 = 700;
        data.g_volt_e_307200 = 700;
        data.g_volt_e_384000 = 700;
        data.g_volt_e_460800 = 700;
        data.g_volt_e_537600 = 700;
        data.g_volt_e_614400 = 700;
        data.g_volt_e_691200 = 700;
        data.g_volt_e_768000 = 700;
        data.g_volt_e_844800 = 710;
        data.g_volt_e_921600 = 740;
        data.g_volt_e_998400 = 770;
        data.g_volt_e_1075200 = 800;
    }
};