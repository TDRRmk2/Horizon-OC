/*
 * HOC Configurator - Complete UI Implementation
 * Copyright (C) Dominatorul, Souldbminer
 */

#include "ui.hpp"
#include "kip_handler.hpp"
#include "value_editor.hpp"
#include "constants.hpp"
#include <cstdio>
#include <vector>
#include <algorithm>

UI::UI() : currentState(MenuState::MAIN), selectedIndex(0), scrollOffset(0), 
           kipLoaded(false), autoSave(false), kipHandler(nullptr), editor(nullptr) {
    statusMessage = "Welcome to HOC Configurator";
    editor = new ValueEditor();
}

UI::~UI() {
    if (editor) delete editor;
}

void UI::drawHeader() {
    printf("\x1b[2J\x1b[1;1H");
    printf("\x1b[47;30m");
    printf("================================================================================\n");
    printf(" HOC Configurator v%s | Made by Dominatorul                          \n", Constants::APP_VERSION);
    printf("================================================================================\n");
    printf("\x1b[0m");
}

void UI::drawFooter() {
    printf("\x1b[42;1H");
    printf("\x1b[47;30m");
    printf("================================================================================\n");
    std::string truncStatus = statusMessage;
    if (truncStatus.length() > 50) {
        truncStatus = truncStatus.substr(0, 47) + "...";
    }
    printf(" [A] Select [B] Back [+] Exit | %s\n", truncStatus.c_str());
    printf("================================================================================\n");
    printf("\x1b[0m");
}

void UI::drawMenuItem(const std::string& text, bool selected, int y) {
    printf("\x1b[%d;1H", y);
    std::string displayText = text;
    if (displayText.length() > 74) {
        displayText = displayText.substr(0, 71) + "...";
    }
    if (selected) {
        printf("\x1b[44;37m > %-76s\x1b[0m\n", displayText.c_str());
    } else {
        printf("   %-76s\n", displayText.c_str());
    }
}

void UI::drawText(const std::string& text, int x, int y) {
    printf("\x1b[%d;%dH%s", y, x, text.c_str());
}

void UI::showValueEditor(const std::string& title, EditorType type, int currentValue,
                         std::function<void(int)> callback, const std::vector<std::string>& options,
                         int min, int max, int step) {
    if (!editor) return;
    EditorConfig cfg;
    cfg.title = title;
    cfg.type = type;
    cfg.currentValue = currentValue;
    cfg.minValue = min;
    cfg.maxValue = max;
    cfg.step = step;
    cfg.options = options;
    cfg.onValueChange = callback;
    editor->show(cfg);
}

void UI::renderMainMenu() {
    drawHeader();
    printf("\x1b[5;1H");
    printf("+- Main Menu -------------------------------------------------------------------+\n");
    std::vector<std::string> menuItems = {
        "GPU Settings", "CPU Settings", "RAM Settings",
        "Misc Settings", "Settings", "About"
    };
    int startY = 7;
    for (size_t i = 0; i < menuItems.size(); i++) {
        drawMenuItem(menuItems[i], (int)i == selectedIndex, startY + (int)i);
    }
    printf("\x1b[%d;1H", startY + (int)menuItems.size() + 1);
    printf("+-------------------------------------------------------------------------------+\n");
    printf("\n");
    printf("  KIP Path: %s\n", kipPath.c_str());
    printf("  KIP Status: %s\n", kipLoaded ? "\x1b[32mLoaded\x1b[0m" : "\x1b[31mNot Loaded\x1b[0m");
    printf("  Auto-save: %s\n", autoSave ? "\x1b[32mEnabled\x1b[0m" : "\x1b[33mDisabled\x1b[0m");
    drawFooter();
}

void UI::renderGPUMenu() {
    drawHeader();
    printf("\x1b[5;1H");
    printf("+- GPU Settings ----------------------------------------------------------------+\n");
    
    if (!kipHandler || !kipLoaded) {
        printf("\n  \x1b[31mNo KIP loaded! Go to Settings to load a KIP file.\x1b[0m\n\n");
        printf("+-------------------------------------------------------------------------------+\n");
        drawFooter();
        return;
    }
    
    auto& data = kipHandler->getData();
    std::vector<std::string> menuItems = {
        "Mariko GPU vMin: " + (data.marikoGpuVmin == 0 ? "Disabled" : std::to_string(data.marikoGpuVmin) + "mV"),
        "Mariko GPU vMax: " + (data.marikoGpuVmax == 0 ? "Disabled" : std::to_string(data.marikoGpuVmax) + "mV"),
        "Erista GPU vMin: " + (data.eristaGpuVmin == 0 ? "Disabled" : std::to_string(data.eristaGpuVmin) + "mV"),
        "Mariko Undervolt: UV" + std::to_string(data.marikoGpuUV),
        "Erista Undervolt: UV" + std::to_string(data.eristaGpuUV),
        "GPU Volt Offset: " + (data.commonGpuVoltOffset == 0 ? "Disabled" : "-" + std::to_string(data.commonGpuVoltOffset) + "mV")
    };
    
    int startY = 7;
    for (size_t i = 0; i < menuItems.size(); i++) {
        drawMenuItem(menuItems[i], (int)i == selectedIndex, startY + (int)i);
    }
    printf("\x1b[%d;1H", startY + (int)menuItems.size() + 1);
    printf("+-------------------------------------------------------------------------------+\n");
    drawFooter();
}

void UI::renderCPUMenu() {
    drawHeader();
    printf("\x1b[5;1H");
    printf("+- CPU Settings ----------------------------------------------------------------+\n");
    
    if (!kipHandler || !kipLoaded) {
        printf("\n  \x1b[31mNo KIP loaded! Go to Settings to load a KIP file.\x1b[0m\n\n");
        printf("+-------------------------------------------------------------------------------+\n");
        drawFooter();
        return;
    }
    
    auto& data = kipHandler->getData();
    std::vector<std::string> menuItems = {
        "CPU Boost Frequency: " + std::to_string(data.commonCpuBoostClock / 1000) + " MHz",
        "Mariko CPU vMin: " + (data.marikoCpuVmin == 0 ? "Default" : std::to_string(data.marikoCpuVmin) + "mV"),
        "Mariko CPU vMax: " + (data.marikoCpuMaxVolt == 0 ? "Disabled" : std::to_string(data.marikoCpuMaxVolt) + "mV"),
        "Erista CPU vMax: " + (data.eristaCpuMaxVolt == 0 ? "Disabled" : std::to_string(data.eristaCpuMaxVolt) + "mV"),
        "Mariko CPU Undervolt: UV" + std::to_string(data.marikoCpuUV),
        "Erista CPU Undervolt: UV" + std::to_string(data.eristaCpuUV)
    };
    
    int startY = 7;
    for (size_t i = 0; i < menuItems.size(); i++) {
        drawMenuItem(menuItems[i], (int)i == selectedIndex, startY + (int)i);
    }
    printf("\x1b[%d;1H", startY + (int)menuItems.size() + 1);
    printf("+-------------------------------------------------------------------------------+\n");
    drawFooter();
}

void UI::renderRAMMenu() {
    drawHeader();
    printf("\x1b[5;1H");
    printf("+- RAM Settings ----------------------------------------------------------------+\n");
    
    if (!kipHandler || !kipLoaded) {
        printf("\n  \x1b[31mNo KIP loaded! Go to Settings to load a KIP file.\x1b[0m\n\n");
        printf("+-------------------------------------------------------------------------------+\n");
        drawFooter();
        return;
    }
    
    auto& data = kipHandler->getData();
    std::vector<std::string> menuItems = {
        "RAM Max Frequency (Mariko): " + std::to_string(data.marikoEmcMaxClock / 1000) + " MHz",
        "RAM Max Frequency (Erista): " + std::to_string(data.eristaEmcMaxClock / 1000) + " MHz",
        "RAM Primary Voltage (VDD2): " + std::to_string(data.commonEmcMemVolt / 1000) + " mV",
        "RAM Secondary Voltage (VDDQ): " + std::to_string(data.marikoEmcVddqVolt / 1000) + " mV",
        "SoC DVB Shift: " + std::to_string(data.marikoEmcDvbShift),
        "Base Latency: " + std::to_string(data.mem_burst_latency),
        "t1 tRCD: " + std::to_string(data.t1_tRCD),
        "t2 tRP: " + std::to_string(data.t2_tRP),
        "t3 tRAS: " + std::to_string(data.t3_tRAS),
        "t4 tRRD: " + std::to_string(data.t4_tRRD),
        "t5 tRFC: " + std::to_string(data.t5_tRFC)
    };
    
    int startY = 7;
    int visibleStart = scrollOffset;
    int visibleEnd = std::min((int)menuItems.size(), scrollOffset + MAX_VISIBLE_ITEMS);
    
    for (int i = visibleStart; i < visibleEnd; i++) {
        drawMenuItem(menuItems[i], i == selectedIndex, startY + (i - visibleStart));
    }
    
    printf("\x1b[%d;1H", startY + (visibleEnd - visibleStart) + 1);
    printf("+-------------------------------------------------------------------------------+\n");
    drawFooter();
}

void UI::renderMiscMenu() {
    drawHeader();
    printf("\x1b[5;1H");
    printf("+- Misc Settings ---------------------------------------------------------------+\n");
    std::vector<std::string> menuItems = {
        "Optimize Fan Curve (V1 Erista)",
        "Optimize Fan Curve (V2 Mariko)",
        "Optimize Fan Curve (Lite)",
        "Optimize Fan Curve (OLED)",
        "Reset Fan Curve to Default",
        "Sleep Mode Battery Fix: Toggle",
        "Battery Charge Limit \x1b[31m(DANGEROUS)\x1b[0m"
    };
    
    int startY = 7;
    for (size_t i = 0; i < menuItems.size(); i++) {
        drawMenuItem(menuItems[i], (int)i == selectedIndex, startY + (int)i);
    }
    printf("\x1b[%d;1H", startY + (int)menuItems.size() + 1);
    printf("+-------------------------------------------------------------------------------+\n");
    drawFooter();
}

void UI::renderAboutMenu() {
    drawHeader();
    printf("\x1b[5;1H");
    printf("+- About -----------------------------------------------------------------------+\n");
    printf("|                                                                              |\n");
    printf("|  Horizon OC Configurator v%-50s  |\n", Constants::APP_VERSION);
    printf("|  Nintendo Switch Homebrew Edition                                            |\n");
    printf("|                                                                              |\n");
    printf("|  Contributors:                                                               |\n");
    printf("|    * Dominatorul - Homebrew port and development                             |\n");
    printf("|    * Souldbminer - Original PC configurator                                  |\n");
    printf("|    * Lightos - L4T timings research                                          |\n");
    printf("|    * Lightos, Samybigio, Flopsider - Testing                                 |\n");
    printf("|                                                                              |\n");
    printf("|  License: GNU General Public License v2.0 or later                           |\n");
    printf("|  Source: github.com/souldbminersmwc/Horizon-OC                               |\n");
    printf("|                                                                              |\n");
    printf("|  \x1b[33mWARNING: Improper overclocking can damage your console!\x1b[0m      |\n");
    printf("|  Use at your own risk. Always test stability before daily use.               |\n");
    printf("|                                                                              |\n");
    printf("+------------------------------------------------------------------------------+\n");
    drawFooter();
}

void UI::renderSettingsMenu() {
    drawHeader();
    printf("\x1b[5;1H");
    printf("+- Settings --------------------------------------------------------------------+\n");
    std::vector<std::string> menuItems = {
        "Toggle Auto-save: " + std::string(autoSave ? "\x1b[32mON\x1b[0m" : "\x1b[33mOFF\x1b[0m"),
        "Save KIP Now",
        "Reload KIP",
        "Back to Main Menu"
    };
    
    int startY = 7;
    for (size_t i = 0; i < menuItems.size(); i++) {
        drawMenuItem(menuItems[i], (int)i == selectedIndex, startY + (int)i);
    }
    printf("\x1b[%d;1H", startY + (int)menuItems.size() + 1);
    printf("+-------------------------------------------------------------------------------+\n");
    printf("\n");
    printf("  Current KIP Path: %s\n", kipPath.c_str());
    printf("  Auto-save Status: %s\n", autoSave ? "\x1b[32mEnabled\x1b[0m" : "\x1b[33mDisabled\x1b[0m");
    drawFooter();
}

void UI::render() {
    if (editor && editor->isActive()) {
        editor->render();
        return;
    }
    
    switch (currentState) {
        case MenuState::MAIN: renderMainMenu(); break;
        case MenuState::GPU: renderGPUMenu(); break;
        case MenuState::CPU: renderCPUMenu(); break;
        case MenuState::RAM: renderRAMMenu(); break;
        case MenuState::MISC: renderMiscMenu(); break;
        case MenuState::ABOUT: renderAboutMenu(); break;
        case MenuState::SETTINGS: renderSettingsMenu(); break;
    }
}

void UI::handleInput(u64 kDown) {
    if (editor && editor->isActive()) {
        editor->handleInput(kDown);
        return;
    }
    
    switch (currentState) {
        case MenuState::MAIN: handleMainMenuInput(kDown); break;
        case MenuState::GPU: handleGPUMenuInput(kDown); break;
        case MenuState::CPU: handleCPUMenuInput(kDown); break;
        case MenuState::RAM: handleRAMMenuInput(kDown); break;
        case MenuState::MISC: handleMiscMenuInput(kDown); break;
        case MenuState::ABOUT: handleAboutMenuInput(kDown); break;
        case MenuState::SETTINGS: handleSettingsMenuInput(kDown); break;
    }
}

void UI::handleMainMenuInput(u64 kDown) {
    if (kDown & HidNpadButton_Down) selectedIndex = (selectedIndex + 1) % 6;
    if (kDown & HidNpadButton_Up) selectedIndex = (selectedIndex - 1 + 6) % 6;
    
    if (kDown & HidNpadButton_A) {
        switch (selectedIndex) {
            case 0: currentState = MenuState::GPU; break;
            case 1: currentState = MenuState::CPU; break;
            case 2: currentState = MenuState::RAM; break;
            case 3: currentState = MenuState::MISC; break;
            case 4: currentState = MenuState::SETTINGS; break;
            case 5: currentState = MenuState::ABOUT; break;
        }
        selectedIndex = 0;
        scrollOffset = 0;
    }
}

void UI::handleGPUMenuInput(u64 kDown) {
    if (!kipHandler || !kipLoaded) {
        if (kDown & HidNpadButton_B) {
            currentState = MenuState::MAIN;
            selectedIndex = 0;
        }
        return;
    }
    
    int maxItems = 8;
    if (kDown & HidNpadButton_Down) selectedIndex = (selectedIndex + 1) % maxItems;
    if (kDown & HidNpadButton_Up) selectedIndex = (selectedIndex - 1 + maxItems) % maxItems;
    if (kDown & HidNpadButton_B) {
        currentState = MenuState::MAIN;
        selectedIndex = 0;
    }
    
    if (kDown & HidNpadButton_A) {
        auto& data = kipHandler->getData();
        
        switch (selectedIndex) {
            case 5: { // Mariko GPU UV
                std::vector<std::string> opts = {"UV0 (No Table)", "UV1 (Regular)", "UV2 (High)", "UV3 (Custom)"};
                showValueEditor("Mariko GPU Undervolt Mode", EditorType::LIST,
                    data.marikoGpuUV, 
                    [this, &data](int val) {
                        data.marikoGpuUV = val;
                        if (autoSave && kipHandler) kipHandler->writeKip();
                        setStatus("Mariko GPU UV mode set to UV" + std::to_string(val));
                    }, opts);
                break;
            }
            case 6: { // Erista GPU UV
                std::vector<std::string> opts = {"UV0 (No Table)", "UV1 (Regular)", "UV2 (High)", "UV3 (Custom)"};
                showValueEditor("Erista GPU Undervolt Mode", EditorType::LIST,
                    data.eristaGpuUV, 
                    [this, &data](int val) {
                        data.eristaGpuUV = val;
                        if (autoSave && kipHandler) kipHandler->writeKip();
                        setStatus("Erista GPU UV mode set to UV" + std::to_string(val));
                    }, opts);
                break;
            }
            default:
                setStatus("Feature in development");
                break;
        }
    }
}

void UI::handleCPUMenuInput(u64 kDown) {
    if (!kipHandler || !kipLoaded) {
        if (kDown & HidNpadButton_B) {
            currentState = MenuState::MAIN;
            selectedIndex = 0;
        }
        return;
    }
    
    int maxItems = 8;
    if (kDown & HidNpadButton_Down) selectedIndex = (selectedIndex + 1) % maxItems;
    if (kDown & HidNpadButton_Up) selectedIndex = (selectedIndex - 1 + maxItems) % maxItems;
    if (kDown & HidNpadButton_B) {
        currentState = MenuState::MAIN;
        selectedIndex = 0;
    }
    
    if (kDown & HidNpadButton_A) {
        auto& data = kipHandler->getData();
        
        switch (selectedIndex) {
            default:
                setStatus("Feature in development");
                break;
        }
    }
}

void UI::handleRAMMenuInput(u64 kDown) {
    if (!kipHandler || !kipLoaded) {
        if (kDown & HidNpadButton_B) {
            currentState = MenuState::MAIN;
            selectedIndex = 0;
        }
        return;
    }
    
    int maxItems = 11;
    
    if (kDown & HidNpadButton_Down) {
        selectedIndex++;
        if (selectedIndex >= maxItems) selectedIndex = 0;
        if (selectedIndex >= scrollOffset + MAX_VISIBLE_ITEMS) {
            scrollOffset = selectedIndex - MAX_VISIBLE_ITEMS + 1;
        }
    }
    
    if (kDown & HidNpadButton_Up) {
        selectedIndex--;
        if (selectedIndex < 0) selectedIndex = maxItems - 1;
        if (selectedIndex < scrollOffset) scrollOffset = selectedIndex;
    }
    
    if (kDown & HidNpadButton_B) {
        currentState = MenuState::MAIN;
        selectedIndex = 0;
        scrollOffset = 0;
    }
    
    if (kDown & HidNpadButton_A) {
        setStatus("RAM setting (in development)");
    }
}

void UI::handleMiscMenuInput(u64 kDown) {
    int maxItems = 7;
    if (kDown & HidNpadButton_Down) selectedIndex = (selectedIndex + 1) % maxItems;
    if (kDown & HidNpadButton_Up) selectedIndex = (selectedIndex - 1 + maxItems) % maxItems;
    if (kDown & HidNpadButton_B) {
        currentState = MenuState::MAIN;
        selectedIndex = 0;
    }
    
    if (kDown & HidNpadButton_A) {
        setStatus("Misc feature (in development)");
    }
}

void UI::handleAboutMenuInput(u64 kDown) {
    if (kDown & HidNpadButton_B) {
        currentState = MenuState::MAIN;
        selectedIndex = 0;
    }
}

void UI::handleSettingsMenuInput(u64 kDown) {
    int maxItems = 4;
    if (kDown & HidNpadButton_Down) selectedIndex = (selectedIndex + 1) % maxItems;
    if (kDown & HidNpadButton_Up) selectedIndex = (selectedIndex - 1 + maxItems) % maxItems;
    if (kDown & HidNpadButton_B) {
        currentState = MenuState::MAIN;
        selectedIndex = 0;
    }
    
    if (kDown & HidNpadButton_A) {
        switch (selectedIndex) {
            case 0: // Toggle Auto-save
                autoSave = !autoSave;
                setStatus(autoSave ? "Auto-save ENABLED" : "Auto-save DISABLED");
                break;
            case 1: // Save KIP Now
                if (kipHandler && kipLoaded) {
                    if (kipHandler->writeKip()) {
                        setStatus("KIP saved successfully!");
                    } else {
                        setStatus("ERROR: Failed to save KIP!");
                    }
                } else {
                    setStatus("ERROR: No KIP loaded!");
                }
                break;
            case 2: // Reload KIP
                if (kipHandler) {
                    if (kipHandler->readKip()) {
                        setStatus("KIP reloaded successfully!");
                        kipLoaded = true;
                    } else {
                        setStatus("ERROR: Failed to reload KIP!");
                        kipLoaded = false;
                    }
                } else {
                    setStatus("ERROR: No KIP handler initialized!");
                }
                break;
            case 3: // Back to Main
                currentState = MenuState::MAIN;
                selectedIndex = 0;
                break;
        }
    }
}