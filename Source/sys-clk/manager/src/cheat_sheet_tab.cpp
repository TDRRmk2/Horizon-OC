/*
    sys-clk manager, a sys-clk frontend homebrew
    Copyright (C) 2019-2020  natinusala
    Copyright (C) 2019  p-sam
    Copyright (C) 2019  m4xw
    Copyright (C) 2025  Souldbminer

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "cheat_sheet_tab.h"

#include <borealis.hpp>

CheatSheetTab::CheatSheetTab()
{
    // CPU
    this->addView(new brls::Header("CPU Clocks"));
    brls::Table *cpuTable = new brls::Table();

    cpuTable->addRow(brls::TableRowType::BODY, "Mariko Absolute Max", "2601 MHz");

    cpuTable->addRow(brls::TableRowType::BODY, "Erista Absolute Max", "2295 MHz");


    cpuTable->addRow(brls::TableRowType::BODY, "Maximum (Mariko Safe)", "1963 MHz");
    cpuTable->addRow(brls::TableRowType::BODY, "Maximum (Erista Safe)", "1785 MHz");

    cpuTable->addRow(brls::TableRowType::BODY, "Boost Mode", "1785 MHz");

    cpuTable->addRow(brls::TableRowType::BODY, "Official Docked and Handheld", "1020 MHz");

    this->addView(cpuTable);

    // GPU
    this->addView(new brls::Header("GPU Clocks"));
    brls::Table *gpuTable = new brls::Table();

    gpuTable->addRow(brls::TableRowType::BODY, "Mariko Absolute Max", "1536 MHz");

    gpuTable->addRow(brls::TableRowType::BODY, "Maximum (Mariko Safe)", "UV0: 1075 MHz, SLT: 1152 MHz, HiOPT: 1228 MHz");

    gpuTable->addRow(brls::TableRowType::BODY, "Erista Absolute Max", "1075 MHz");

    gpuTable->addRow(brls::TableRowType::BODY, "Maximum (Erista Safe)", "UV0: 844 MHz, SLT: 921 MHz, HiOPT: 998 MHz");

    gpuTable->addRow(brls::TableRowType::BODY, "Official Docked", "768 MHz");
    gpuTable->addRow(brls::TableRowType::BODY, "Maximum Mariko Handheld", "614 MHz");
    gpuTable->addRow(brls::TableRowType::BODY, "Maximum Erista Handheld", "460 MHz");
    gpuTable->addRow(brls::TableRowType::BODY, "Official Handheld", "307-460 MHz (depends per game)");

    gpuTable->addRow(brls::TableRowType::BODY, "Boost Mode", "76 MHz");

    this->addView(gpuTable);

    // MEM
    this->addView(new brls::Header("MEM Clocks"));
    brls::Table *memTable = new brls::Table();

    memTable->addRow(brls::TableRowType::BODY, "Mariko Absolute Max", "3200 MHz");

    memTable->addRow(brls::TableRowType::BODY, "Erista Absolute Max", "2360 MHz");

    memTable->addRow(brls::TableRowType::BODY, "Mariko Official Max", "2133 MHz");
    memTable->addRow(brls::TableRowType::BODY, "AM-MGCJ / Hynix NME Official Max", "1862 MHz");

    memTable->addRow(brls::TableRowType::BODY, "Official Docked, Erista Official Max", "1600 MHz");
    memTable->addRow(brls::TableRowType::BODY, "Official Handheld", "1331 MHz");

    this->addView(memTable);

}

void CheatSheetTab::customSpacing(brls::View* current, brls::View* next, int* spacing)
{
    if (dynamic_cast<brls::Table*>(current))
        *spacing = 0;
    else
        List::customSpacing(current, next, spacing);
}
