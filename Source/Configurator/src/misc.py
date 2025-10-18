
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

import os
from pathlib import Path
import dearpygui.dearpygui as dpg
import common
import ini
import re
import installer as ins
from defaults import d
import ram as r
import settings as s

def get_ini_path():
    if not common.drive or common.drive == 0:
        common.show_popup("Error", "This feature requires a valid drive")
        return None
    return Path(str(common.drive)) / "atmosphere/config/system_settings.ini"


def set_psm_value(sender, app_data):
    ini_path = get_ini_path()
    if not ini_path:
        return
    value = next((x["value"] for x in s.PSM_OPTIONS if x["name"] == app_data), None)
    if value:
        ini.set_ini_values(str(ini_path), "psm", {"current_psm_mA": value})
        common.show_popup("Success", f"Charge Limit set to {app_data}")

def remove_tc_entries():
    ini_path = get_ini_path()
    if not ini_path:
        return

    try:
        with open(ini_path, "r") as f:
            lines = f.readlines()
    except FileNotFoundError:
        return

    output_lines = []
    current_section = None

    for line in lines:
        section_match = re.match(r"\s*\[([^\]]+)\]", line)
        if section_match:
            current_section = section_match.group(1)
            if current_section == "tc":
                current_section = "skip"
                continue
            else:
                output_lines.append(line)
                continue

        if current_section != "skip":
            output_lines.append(line)

    with open(ini_path, "w") as f:
        f.writelines(output_lines)
    common.show_popup("Success", "Reset to default fan curve")


def set_ini_from_profile(sender, app_data, user_data):
    profile_name = user_data
    if profile_name not in s.PROFILES:
        print(f"Profile '{profile_name}' not found.")
        return

    entries = s.PROFILES[profile_name]
    ini_path = get_ini_path()
    if not ini_path:
        return
    ini.set_ini_values(str(ini_path), "tc", entries)
    common.show_popup("Success", "Profile has been applied")
    print(f"Applied profile {profile_name} under [tc]")

def update_skin_target(sender, app_data):
    s.skinTarget = app_data
    print("skinTarget =", s.skinTarget)

    ini_path = get_ini_path()
    if not ini_path:
        return

    ini.set_ini_values(str(ini_path), "tc", s.skin_t_entries)

def toggle_battery_save(sender, user_data):

    if not common.drive:
        common.show_popup("Error", "No drive selected")
        return

    ini_path = Path(common.drive) / "atmosphere/config/system_settings.ini"

    for section, entries in s.BATTERY_SAVE_OPTIONS.items():
        if dpg.get_item_alias(sender) == "enable_battery_fix":  # hardcoded but otherwise it doesnt work :(
            ini.set_ini_values(str(ini_path), section, entries)
        else:
            remove_entries = {k: None for k in entries}
            ini.set_ini_values(str(ini_path), section, remove_entries)

    common.show_popup("Info", f"Sleep Mode Battery Fix {'enabled' if dpg.get_item_alias(sender) == "enable_battery_fix" else 'disabled'}")
def populate():
    dpg.add_separator(label="Fan")
    dpg.add_button(label="Optimize Fan Curve for V1", user_data="V1_Erista", callback=set_ini_from_profile)
    dpg.add_button(label="Optimize Fan Curve for V2", user_data="V2_Mariko", callback=set_ini_from_profile)
    dpg.add_button(label="Optimize Fan Curve for Lite", user_data="Lite_Mariko", callback=set_ini_from_profile)
    dpg.add_button(label="Optimize Fan Curve for OLED", user_data="OLED_Mariko", callback=set_ini_from_profile)

    dpg.add_slider_int(
        label="Skin Target (Recommended - 54°C)",
        min_value=50, max_value=60,
        default_value=s.skinTarget,
        callback=update_skin_target
    )
    dpg.add_button(label="Reset Fan Curve", callback=remove_tc_entries)

    dpg.add_separator(label="Battery")
    dpg.add_button(
        label="What is this?",
        callback=common.show_info_window,
        user_data="This option fixes battery life drain in sleep mode\nDO NOT use this if you are using online services",
        tag="battery_fix_info"
    )
    dpg.add_button(label="Enable sleep mode battery fix", tag="enable_battery_fix", callback=toggle_battery_save, user_data=1)
    dpg.add_button(label="Disable sleep mode battery fix", tag="disable_battery_fix",callback=toggle_battery_save, user_data=0)

    dpg.add_separator(label="Downloads")



    nx_ovlloader_link = "https://github.com/ppkantorski/nx-ovlloader/releases/latest/download/nx-ovlloader+.zip"
    dpg.add_button(label="Install nx-ovlloader", callback=lambda: ins.download_and_extract_zip(common, nx_ovlloader_link, zip_filename="ovll_temp.zip", success_message="Installed nx-ovlloader!"))

    ultrahand_link = "https://github.com/ppkantorski/Ultrahand-Overlay/releases/latest/download/ovlmenu.ovl"
    dpg.add_button(label="Install Ultrahand Overlay", callback=lambda: ins.download_and_install(c=common, install_path=common.drive + "switch/.overlays/", download_url=ultrahand_link, filename="ovlMenu.ovl", success_message="Installed Ultrahand!"))

    status_monitor_link = "https://github.com/ppkantorski/Status-Monitor-Overlay/releases/latest/download/ Status-Monitor-Overlay.ovl "
    dpg.add_button(label="Install Status Monitor", callback=lambda: ins.download_and_install(c=common, install_path=common.drive + "switch/.overlays/", download_url=status_monitor_link, filename=" Status-Monitor-Overlay.ovl", success_message="Installed Status Monitor!"))

    saltynx_link = "https://github.com/masagrator/SaltyNX/releases/latest/download/SaltyNX.zip"
    dpg.add_button(label="Install SaltyNX", callback=lambda: ins.download_and_extract_zip(common, saltynx_link, zip_filename="saltynx_temp.zip", success_message="Installed SaltyNX!"))

    reversenx_link = "https://github.com/masagrator/ReverseNX-RT/releases/latest/download/ReverseNX-RT-ovl.ovl"
    dpg.add_button(label="Install ReverseNX-RT", callback=lambda: ins.download_and_install(c=common, install_path=common.drive + "switch/.overlays/", download_url=reversenx_link, filename="ReverseNX-RT-ovl.ovl", success_message="Installed ReverseNX-RT!"))

    dpg.add_separator(label="Danger Zone")

    dpg.add_text("These options are known to cause hardware issues, PMIC issues or overheating.\nUse with extreme caution!", color=(255, 165, 0, 255))

    dpg.add_separator()

    dpg.add_spacer(height=10)

    psm_items = [x["name"] for x in s.PSM_OPTIONS]
    dpg.add_combo(items=psm_items, label="Battery Charge Limit", callback=set_psm_value, tag="psm_dropdown", default_value="2048mA (Default)")
    dpg.add_spacer(height=5)

    dpg.add_separator(label="EXTREME DANGER ZONE")
    dpg.add_text("THESE OPTIONS ARE KNOWN TO CAUSE IRREPARABLE HARDWARE DAMAGE THAT CAN AND WILL KILL YOUR CONSOLE\nUSE ONLY IF YOU ARE AN ADVANCED USER AND ARE AWARE OF THE RISKS AND POTENTIAL CONCEQUENCES", color=(255, 0, 0, 255))
    dpg.add_spacer(height=10)
    with dpg.group(horizontal=True):
        dpg.add_checkbox(
            label="",
            callback=set_white_tiger_clocks
        )
        dpg.add_image("coolerhd", width=20, height=20)

def set_white_tiger_clocks(app_data):
    # cooler aah clocks
    # 1400mv emc max
    # 1375mv cpu max
    # 750mv vdd2 mariko max
    base_vddq_uv = list(range(550000, 650001, 5000))

    vddq_uv = [0] + list(range(250000, 750001, 5000)) if app_data else base_vddq_uv
    vddq_mv = [v // 1000 for v in vddq_uv]
    vddq_mv_label = ["Default (600 mV)" if f == 0 else f"{f} mV" for f in vddq_mv]
    dpg.configure_item("marikoEmcVddqVolt", items=vddq_mv_label)

    base_voltages_uv = list(range(912500, 1212500 + 1, 12500))
    voltages_uv = base_voltages_uv + list(range(1225000, 1400000 + 1, 12500)) if app_data else base_voltages_uv
    voltages_mv = [v / 1000 for v in voltages_uv]
    voltages_mv_label = ["Default (1175 mV)" if f == 0 else f"{f} mV" for f in voltages_mv]
    dpg.configure_item("commonEmcMemVolt", items=voltages_mv_label)

    voltages = [0] + list(range(1130, 1375 + 1, 5)) if app_data else [0] + list(range(1000, 1160 + 1, 5))
    voltages_e = [0] + list(range(1180, 1375 + 1, 5)) if app_data else [0] + list(range(1120, 1255 + 1, 5)) + [1257]
    processed_voltages_e = ["Disabled" if v == 0 else f"{v}mV" for v in voltages_e]
    processed_voltages = ["Disabled" if v == 0 else f"{v}mV" for v in voltages]
    dpg.configure_item("marikoCpuMaxVolt", items=processed_voltages)
    dpg.configure_item("eristaCpuMaxVolt", items=processed_voltages_e)
