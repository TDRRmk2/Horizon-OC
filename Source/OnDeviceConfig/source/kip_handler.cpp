/*
 * HOC Configurator - KIP Handler Implementation
 * Copyright (C) Dominatorul, Souldbminer
 */

#include "kip_handler.hpp"
#include <fstream>
#include <cstring>
#include <vector>

bool KipHandler::readKip() {
    std::ifstream file(kipPath, std::ios::binary);
    if (!file.is_open()) {
        return false;
    }
    
    // Read entire file
    file.seekg(0, std::ios::end);
    size_t fileSize = file.tellg();
    file.seekg(0, std::ios::beg);
    
    std::vector<uint8_t> buffer(fileSize);
    file.read(reinterpret_cast<char*>(buffer.data()), fileSize);
    file.close();
    
    // Find CUST magic
    size_t magicPos = 0;
    bool found = false;
    for (size_t i = 0; i < fileSize - 4; i++) {
        if (memcmp(&buffer[i], MAGIC, 4) == 0) {
            magicPos = i + 4;
            found = true;
            break;
        }
    }
    
    if (!found) {
        return false;
    }
    
    // Read structure (assuming packed uint32_t structure)
    size_t offset = magicPos;
    auto readU32 = [&]() -> uint32_t {
        uint32_t val;
        memcpy(&val, &buffer[offset], sizeof(uint32_t));
        offset += sizeof(uint32_t);
        return val;
    };
    
    data.custRev = readU32();
    data.mtcConf = readU32();
    data.commonCpuBoostClock = readU32();
    data.commonEmcMemVolt = readU32();
    data.eristaCpuMaxVolt = readU32();
    data.eristaEmcMaxClock = readU32();
    data.marikoCpuMaxVolt = readU32();
    data.marikoEmcMaxClock = readU32();
    data.marikoEmcVddqVolt = readU32();
    data.marikoCpuUV = readU32();
    data.marikoGpuUV = readU32();
    data.eristaCpuUV = readU32();
    data.eristaGpuUV = readU32();
    data.commonGpuVoltOffset = readU32();
    data.marikoEmcDvbShift = readU32();
    
    // Memory timings
    data.t1_tRCD = readU32();
    data.t2_tRP = readU32();
    data.t3_tRAS = readU32();
    data.t4_tRRD = readU32();
    data.t5_tRFC = readU32();
    data.t6_tRTW = readU32();
    data.t7_tWTR = readU32();
    data.t8_tREFI = readU32();
    data.mem_burst_latency = readU32();
    
    // Additional voltages
    data.marikoCpuVmin = readU32();
    data.eristaGpuVmin = readU32();
    data.marikoGpuVmin = readU32();
    data.marikoGpuVmax = readU32();
    
    // GPU voltages Mariko
    data.g_volt_76800 = readU32();
    data.g_volt_153600 = readU32();
    data.g_volt_230400 = readU32();
    data.g_volt_307200 = readU32();
    data.g_volt_384000 = readU32();
    data.g_volt_460800 = readU32();
    data.g_volt_537600 = readU32();
    data.g_volt_614400 = readU32();
    data.g_volt_691200 = readU32();
    data.g_volt_768000 = readU32();
    data.g_volt_844800 = readU32();
    data.g_volt_921600 = readU32();
    data.g_volt_998400 = readU32();
    data.g_volt_1075200 = readU32();
    data.g_volt_1152000 = readU32();
    data.g_volt_1228800 = readU32();
    data.g_volt_1267200 = readU32();
    data.g_volt_1305600 = readU32();
    data.g_volt_1344000 = readU32();
    data.g_volt_1382400 = readU32();
    data.g_volt_1420800 = readU32();
    data.g_volt_1459200 = readU32();
    data.g_volt_1497600 = readU32();
    data.g_volt_1536000 = readU32();
    
    // GPU voltages Erista
    data.g_volt_e_76800 = readU32();
    data.g_volt_e_153600 = readU32();
    data.g_volt_e_230400 = readU32();
    data.g_volt_e_307200 = readU32();
    data.g_volt_e_384000 = readU32();
    data.g_volt_e_460800 = readU32();
    data.g_volt_e_537600 = readU32();
    data.g_volt_e_614400 = readU32();
    data.g_volt_e_691200 = readU32();
    data.g_volt_e_768000 = readU32();
    data.g_volt_e_844800 = readU32();
    data.g_volt_e_921600 = readU32();
    data.g_volt_e_998400 = readU32();
    data.g_volt_e_1075200 = readU32();
    
    return true;
}

bool KipHandler::writeKip() {
    std::fstream file(kipPath, std::ios::in | std::ios::out | std::ios::binary);
    if (!file.is_open()) {
        return false;
    }
    
    // Read entire file
    file.seekg(0, std::ios::end);
    size_t fileSize = file.tellg();
    file.seekg(0, std::ios::beg);
    
    std::vector<uint8_t> buffer(fileSize);
    file.read(reinterpret_cast<char*>(buffer.data()), fileSize);
    
    // Find CUST magic
    size_t magicPos = 0;
    bool found = false;
    for (size_t i = 0; i < fileSize - 4; i++) {
        if (memcmp(&buffer[i], MAGIC, 4) == 0) {
            magicPos = i + 4;
            found = true;
            break;
        }
    }
    
    if (!found) {
        file.close();
        return false;
    }
    
    // Write structure
    size_t offset = magicPos;
    auto writeU32 = [&](uint32_t val) {
        memcpy(&buffer[offset], &val, sizeof(uint32_t));
        offset += sizeof(uint32_t);
    };
    
    writeU32(data.custRev);
    writeU32(data.mtcConf);
    writeU32(data.commonCpuBoostClock);
    writeU32(data.commonEmcMemVolt);
    writeU32(data.eristaCpuMaxVolt);
    writeU32(data.eristaEmcMaxClock);
    writeU32(data.marikoCpuMaxVolt);
    writeU32(data.marikoEmcMaxClock);
    writeU32(data.marikoEmcVddqVolt);
    writeU32(data.marikoCpuUV);
    writeU32(data.marikoGpuUV);
    writeU32(data.eristaCpuUV);
    writeU32(data.eristaGpuUV);
    writeU32(data.commonGpuVoltOffset);
    writeU32(data.marikoEmcDvbShift);
    
    // Memory timings
    writeU32(data.t1_tRCD);
    writeU32(data.t2_tRP);
    writeU32(data.t3_tRAS);
    writeU32(data.t4_tRRD);
    writeU32(data.t5_tRFC);
    writeU32(data.t6_tRTW);
    writeU32(data.t7_tWTR);
    writeU32(data.t8_tREFI);
    writeU32(data.mem_burst_latency);
    
    // Additional voltages
    writeU32(data.marikoCpuVmin);
    writeU32(data.eristaGpuVmin);
    writeU32(data.marikoGpuVmin);
    writeU32(data.marikoGpuVmax);
    
    // GPU voltages Mariko
    writeU32(data.g_volt_76800);
    writeU32(data.g_volt_153600);
    writeU32(data.g_volt_230400);
    writeU32(data.g_volt_307200);
    writeU32(data.g_volt_384000);
    writeU32(data.g_volt_460800);
    writeU32(data.g_volt_537600);
    writeU32(data.g_volt_614400);
    writeU32(data.g_volt_691200);
    writeU32(data.g_volt_768000);
    writeU32(data.g_volt_844800);
    writeU32(data.g_volt_921600);
    writeU32(data.g_volt_998400);
    writeU32(data.g_volt_1075200);
    writeU32(data.g_volt_1152000);
    writeU32(data.g_volt_1228800);
    writeU32(data.g_volt_1267200);
    writeU32(data.g_volt_1305600);
    writeU32(data.g_volt_1344000);
    writeU32(data.g_volt_1382400);
    writeU32(data.g_volt_1420800);
    writeU32(data.g_volt_1459200);
    writeU32(data.g_volt_1497600);
    writeU32(data.g_volt_1536000);
    
    // GPU voltages Erista
    writeU32(data.g_volt_e_76800);
    writeU32(data.g_volt_e_153600);
    writeU32(data.g_volt_e_230400);
    writeU32(data.g_volt_e_307200);
    writeU32(data.g_volt_e_384000);
    writeU32(data.g_volt_e_460800);
    writeU32(data.g_volt_e_537600);
    writeU32(data.g_volt_e_614400);
    writeU32(data.g_volt_e_691200);
    writeU32(data.g_volt_e_768000);
    writeU32(data.g_volt_e_844800);
    writeU32(data.g_volt_e_921600);
    writeU32(data.g_volt_e_998400);
    writeU32(data.g_volt_e_1075200);
    
    // Write back to file
    file.seekp(0, std::ios::beg);
    file.write(reinterpret_cast<char*>(buffer.data()), fileSize);
    file.close();
    
    return true;
}