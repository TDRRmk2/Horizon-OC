/*
 * HOC Configurator - KIP Handler
 * Copyright (C) Dominatorul, Souldbminer
 */

#pragma once
#include <string>
#include <cstdint>
#include <vector>
#include "defaults.hpp"

class KipHandler {
private:
    std::string kipPath;
    const uint8_t MAGIC[4] = {'C', 'U', 'S', 'T'};
    
    struct KipData {
        uint32_t custRev;
        uint32_t mtcConf;
        uint32_t commonCpuBoostClock;
        uint32_t commonEmcMemVolt;
        uint32_t eristaCpuMaxVolt;
        uint32_t eristaEmcMaxClock;
        uint32_t marikoCpuMaxVolt;
        uint32_t marikoEmcMaxClock;
        uint32_t marikoEmcVddqVolt;
        uint32_t marikoCpuUV;
        uint32_t marikoGpuUV;
        uint32_t eristaCpuUV;
        uint32_t eristaGpuUV;
        uint32_t commonGpuVoltOffset;
        uint32_t marikoEmcDvbShift;
        
        // Memory timings
        uint32_t t1_tRCD;
        uint32_t t2_tRP;
        uint32_t t3_tRAS;
        uint32_t t4_tRRD;
        uint32_t t5_tRFC;
        uint32_t t6_tRTW;
        uint32_t t7_tWTR;
        uint32_t t8_tREFI;
        uint32_t mem_burst_latency;
        
        // Additional voltages
        uint32_t marikoCpuVmin;
        uint32_t eristaGpuVmin;
        uint32_t marikoGpuVmin;
        uint32_t marikoGpuVmax;
        
        // GPU voltages for each frequency (Mariko)
        uint32_t g_volt_76800;
        uint32_t g_volt_153600;
        uint32_t g_volt_230400;
        uint32_t g_volt_307200;
        uint32_t g_volt_384000;
        uint32_t g_volt_460800;
        uint32_t g_volt_537600;
        uint32_t g_volt_614400;
        uint32_t g_volt_691200;
        uint32_t g_volt_768000;
        uint32_t g_volt_844800;
        uint32_t g_volt_921600;
        uint32_t g_volt_998400;
        uint32_t g_volt_1075200;
        uint32_t g_volt_1152000;
        uint32_t g_volt_1228800;
        uint32_t g_volt_1267200;
        uint32_t g_volt_1305600;
        uint32_t g_volt_1344000;
        uint32_t g_volt_1382400;
        uint32_t g_volt_1420800;
        uint32_t g_volt_1459200;
        uint32_t g_volt_1497600;
        uint32_t g_volt_1536000;
        
        // GPU voltages for each frequency (Erista)
        uint32_t g_volt_e_76800;
        uint32_t g_volt_e_153600;
        uint32_t g_volt_e_230400;
        uint32_t g_volt_e_307200;
        uint32_t g_volt_e_384000;
        uint32_t g_volt_e_460800;
        uint32_t g_volt_e_537600;
        uint32_t g_volt_e_614400;
        uint32_t g_volt_e_691200;
        uint32_t g_volt_e_768000;
        uint32_t g_volt_e_844800;
        uint32_t g_volt_e_921600;
        uint32_t g_volt_e_998400;
        uint32_t g_volt_e_1075200;
    };
    
    KipData data;
    
public:
    KipHandler(const std::string& path) : kipPath(path) {
        // Initialize with defaults
        Defaults::initDefaults(data);
    }
    
    bool readKip();
    bool writeKip();
    
    // Getters
    KipData& getData() { return data; }
    const KipData& getData() const { return data; }
    
    // Setters for common values
    void setCommonCpuBoostClock(uint32_t val) { data.commonCpuBoostClock = val; }
    void setCommonEmcMemVolt(uint32_t val) { data.commonEmcMemVolt = val; }
    void setMarikoCpuMaxVolt(uint32_t val) { data.marikoCpuMaxVolt = val; }
    void setMarikoEmcMaxClock(uint32_t val) { data.marikoEmcMaxClock = val; }
    void setMarikoEmcVddqVolt(uint32_t val) { data.marikoEmcVddqVolt = val; }
    
    // Utility
    std::string getKipPath() const { return kipPath; }
    void setKipPath(const std::string& path) { kipPath = path; }
};
