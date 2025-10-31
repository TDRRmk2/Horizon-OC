/*
 * Copyright (C) Switch-OC-Suite
 *
 * Copyright (c) 2023 hanai3Bi
 *
 * Copyright (c) Souldbminer and Horizon OC Contributors
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "pcv.hpp"
#include "../mtc_timing_value.hpp"

namespace ams::ldr::oc::pcv::mariko
{

    Result GpuVmin(u32 *ptr)
    {
        if (!C.marikoGpuVmin)
            R_SKIP();
        PATCH_OFFSET(ptr, (int)C.marikoGpuVmin);
        R_SUCCEED();
    }

    Result GpuVmax(u32 *ptr)
    {
        if (!C.marikoGpuVmax)
            R_SKIP();
        PATCH_OFFSET(ptr, (int)C.marikoGpuVmax);
        R_SUCCEED();
    }

    Result CpuFreqVdd(u32 *ptr)
    {
        dvfs_rail *entry = reinterpret_cast<dvfs_rail *>(reinterpret_cast<u8 *>(ptr) - offsetof(dvfs_rail, freq));

        R_UNLESS(entry->id == 1, ldr::ResultInvalidCpuFreqVddEntry());
        R_UNLESS(entry->min_mv == 250'000, ldr::ResultInvalidCpuFreqVddEntry());
        R_UNLESS(entry->step_mv == 5000, ldr::ResultInvalidCpuFreqVddEntry());
        R_UNLESS(entry->max_mv == 1525'000, ldr::ResultInvalidCpuFreqVddEntry());
        if (C.enableMarikoCpuUnsafeFreqs)
        {
            PATCH_OFFSET(ptr, GetDvfsTableLastEntry(C.marikoCpuDvfsTable)->freq);
        }
        else
        {
            if (C.marikoCpuUV)
            {
                if (!C.enableMarikoCpuUnsafeFreqs)
                {
                    PATCH_OFFSET(ptr, GetDvfsTableLastEntry(C.marikoCpuDvfsTableSLT)->freq);
                }
                else
                {
                    PATCH_OFFSET(ptr, GetDvfsTableLastEntry(C.marikoCpuDvfsTableUnsafeFreqs)->freq);
                }
            }
        }
        R_SUCCEED();
    }

    Result CpuVoltRange(u32 *ptr)
    {
        u32 min_volt_got = *(ptr - 1);
        for (const auto &mv : CpuMinVolts)
        {
            if (min_volt_got != mv)
                continue;

            if (!C.marikoCpuMaxVolt)
                R_SKIP();

            PATCH_OFFSET(ptr, C.marikoCpuMaxVolt);
            // Patch vmin for slt
            if (C.marikoCpuUV)
            {
                if (*(ptr - 5) == 620)
                {
                    PATCH_OFFSET((ptr - 5), C.marikoCpuVmin);
                }
                if (*(ptr - 1) == 620)
                {
                    PATCH_OFFSET((ptr - 1), 600);
                }
            }
            R_SUCCEED();
        }
        R_THROW(ldr::ResultInvalidCpuMinVolt());
    }

    Result CpuVoltDfll(u32 *ptr)
    {
        cvb_cpu_dfll_data *entry = reinterpret_cast<cvb_cpu_dfll_data *>(ptr);

        R_UNLESS(entry->tune0_low == 0x0000FFCF, ldr::ResultInvalidCpuVoltDfllEntry());
        R_UNLESS(entry->tune0_high == 0x00000000, ldr::ResultInvalidCpuVoltDfllEntry());
        R_UNLESS(entry->tune1_low == 0x012207FF, ldr::ResultInvalidCpuVoltDfllEntry());
        R_UNLESS(entry->tune1_high == 0x03FFF7FF, ldr::ResultInvalidCpuVoltDfllEntry());
        switch (C.marikoCpuUV)
        {
        case 0:
            break;
        case 1:
            PATCH_OFFSET(&(entry->tune0_low), 0x0000FF90); // process_id 0 // EOS UV1
            PATCH_OFFSET(&(entry->tune0_high), 0x0000FFFF);
            PATCH_OFFSET(&(entry->tune1_low), 0x021107FF);
            PATCH_OFFSET(&(entry->tune1_high), 0x00000000);
            break;
        case 2:
            PATCH_OFFSET(&(entry->tune0_low), 0x0000FF92); /// EOS Uv2
            PATCH_OFFSET(&(entry->tune0_high), 0x0000FFFF);
            PATCH_OFFSET(&(entry->tune1_low), 0x021107FF);
            PATCH_OFFSET(&(entry->tune1_high), 0x00000000);
            break;
        case 3:
            PATCH_OFFSET(&(entry->tune0_low), 0x0000FF9A); // EOS UV3
            PATCH_OFFSET(&(entry->tune0_high), 0x0000FFFF);
            PATCH_OFFSET(&(entry->tune1_low), 0x021107FF);
            PATCH_OFFSET(&(entry->tune1_high), 0x00000000);
            break;
        case 4:
            PATCH_OFFSET(&(entry->tune0_low), 0x0000FFA2); // EOS Uv4
            PATCH_OFFSET(&(entry->tune0_high), 0x0000FFFF);
            PATCH_OFFSET(&(entry->tune1_low), 0x021107FF);
            PATCH_OFFSET(&(entry->tune1_high), 0x00000000);
            break;
        case 5:
            PATCH_OFFSET(&(entry->tune0_low), 0x0000FFFF); // EOS UV5
            PATCH_OFFSET(&(entry->tune0_high), 0x0000FFFF);
            PATCH_OFFSET(&(entry->tune1_low), 0x021107FF);
            PATCH_OFFSET(&(entry->tune1_high), 0x022217FF);
            break;
        case 6:
            PATCH_OFFSET(&(entry->tune0_low), 0x0000FFFF); // EOS UV6
            PATCH_OFFSET(&(entry->tune0_high), 0x0000FFFF);
            PATCH_OFFSET(&(entry->tune1_low), 0x021107FF);
            PATCH_OFFSET(&(entry->tune1_high), 0x024417FF);
            break;
        case 7:
            PATCH_OFFSET(&(entry->tune0_low), 0x0000FFFF); // EOS UV6
            PATCH_OFFSET(&(entry->tune0_high), 0x0000FFFF);
            PATCH_OFFSET(&(entry->tune1_low), 0x021107FF);
            PATCH_OFFSET(&(entry->tune1_high), 0x026617FF);
            break;
        case 8:
            PATCH_OFFSET(&(entry->tune0_low), 0x0000FFFF); // EOS UV6
            PATCH_OFFSET(&(entry->tune0_high), 0x0000FFFF);
            PATCH_OFFSET(&(entry->tune1_low), 0x021107FF);
            PATCH_OFFSET(&(entry->tune1_high), 0x028817FF);
            break;
        default:
            break;
        }
        R_SUCCEED();
    }

    Result GpuFreqMaxAsm(u32 *ptr32)
    {
        // Check if both two instructions match the pattern
        u32 ins1 = *ptr32, ins2 = *(ptr32 + 1);
        if (!(asm_compare_no_rd(ins1, asm_pattern[0]) && asm_compare_no_rd(ins2, asm_pattern[1])))
            R_THROW(ldr::ResultInvalidGpuFreqMaxPattern());

        // Both instructions should operate on the same register
        u8 rd = asm_get_rd(ins1);
        if (rd != asm_get_rd(ins2))
            R_THROW(ldr::ResultInvalidGpuFreqMaxPattern());

        u32 max_clock;
        switch (C.marikoGpuUV)
        {
        case 0:
            max_clock = GetDvfsTableLastEntry(C.marikoGpuDvfsTable)->freq;
            break;
        case 1:
            max_clock = GetDvfsTableLastEntry(C.marikoGpuDvfsTableSLT)->freq;
            break;
        case 2:
            max_clock = GetDvfsTableLastEntry(C.marikoGpuDvfsTableHiOPT)->freq;
            break;
        case 3:
            if (C.enableMarikoGpuUnsafeFreqs)
            {
                max_clock = GetDvfsTableLastEntry(C.marikoGpuDvfsTableUv3UnsafeFreqs)->freq;
            }
            else
            {
                max_clock = GetDvfsTableLastEntry(C.marikoGpuDvfsTable)->freq;
            }
            break;
        default:
            max_clock = GetDvfsTableLastEntry(C.marikoGpuDvfsTable)->freq;
            break;
        }
        u32 asm_patch[2] = {
            asm_set_rd(asm_set_imm16(asm_pattern[0], max_clock), rd),
            asm_set_rd(asm_set_imm16(asm_pattern[1], max_clock >> 16), rd)};
        PATCH_OFFSET(ptr32, asm_patch[0]);
        PATCH_OFFSET(ptr32 + 1, asm_patch[1]);

        R_SUCCEED();
    }

    Result GpuFreqPllLimit(u32 *ptr)
    {
        clk_pll_param *entry = reinterpret_cast<clk_pll_param *>(ptr);

        // All zero except for freq
        for (size_t i = 1; i < sizeof(clk_pll_param) / sizeof(u32); i++)
        {
            R_UNLESS(*(ptr + i) == 0, ldr::ResultInvalidGpuPllEntry());
        }

        // Double the max clk simply
        u32 max_clk = entry->freq * 2;
        entry->freq = max_clk;
        R_SUCCEED();
    }

    /* Get RAM vendor data, ty b0rd2death! */
    /* Note: I know this is horrible but I don't care atm. */
    bool IsMicron()
    {
        u64 packed_version;
        splGetConfig((SplConfigItem)2, &packed_version);

        switch (packed_version)
        {
        case 11:
        case 15:
        case 25:
        case 26:
        case 27:
        case 32:
        case 33:
        case 34:
            /* RAM is Micron. */
            return true;
        default:
            /* Not Micron. */
            return false;
        }
    }


    void MemMtcTableAutoAdjustBaseLatency(MarikoMtcTable *table) {
        #define WRITE_PARAM_ALL_REG(TABLE, PARAM, VALUE) \
            TABLE->burst_regs.PARAM = VALUE;             \
            TABLE->shadow_regs_ca_train.PARAM   = VALUE; \
            TABLE->shadow_regs_rdwr_train.PARAM = VALUE;

        WRITE_PARAM_ALL_REG(table, emc_cfg, 0xf3200000);
        WRITE_PARAM_ALL_REG(table, emc_rc, 0x00000070);
        WRITE_PARAM_ALL_REG(table, emc_rfc, 0x0000020b);
        WRITE_PARAM_ALL_REG(table, emc_ras, 0x0000004f);
        WRITE_PARAM_ALL_REG(table, emc_rp, 0x00000022);
        WRITE_PARAM_ALL_REG(table, emc_r2w, 0x0000002e);
        WRITE_PARAM_ALL_REG(table, emc_w2r, 0x00000025);
        WRITE_PARAM_ALL_REG(table, emc_r2p, 0x0000000e);
        WRITE_PARAM_ALL_REG(table, emc_w2p, 0x00000033);
        WRITE_PARAM_ALL_REG(table, emc_rd_rcd, 0x00000022);
        WRITE_PARAM_ALL_REG(table, emc_wr_rcd, 0x00000022);
        WRITE_PARAM_ALL_REG(table, emc_rrd, 0x00000013);
        WRITE_PARAM_ALL_REG(table, emc_rext, 0x0000001a);

        WRITE_PARAM_ALL_REG(table, emc_qsafe, 0x00000038);
        WRITE_PARAM_ALL_REG(table, emc_refresh, 0x00001c2d);
        WRITE_PARAM_ALL_REG(table, emc_burst_refresh_num, 0x00000000);
        WRITE_PARAM_ALL_REG(table, emc_pdex2wr, 0x00000013);
        WRITE_PARAM_ALL_REG(table, emc_pdex2rd, 0x00000013);
        WRITE_PARAM_ALL_REG(table, emc_pchg2pden, 0x00000004);
        WRITE_PARAM_ALL_REG(table, emc_act2pden, 0x0000001b);
        WRITE_PARAM_ALL_REG(table, emc_ar2pden, 0x00000004);
        WRITE_PARAM_ALL_REG(table, emc_rw2pden, 0x0000003f);
        WRITE_PARAM_ALL_REG(table, emc_txsr, 0x00000219);
        WRITE_PARAM_ALL_REG(table, emc_tcke, 0x00000010);
        WRITE_PARAM_ALL_REG(table, emc_tfaw, 0x0000004b);
        WRITE_PARAM_ALL_REG(table, emc_trpab, 0x00000028);
        WRITE_PARAM_ALL_REG(table, emc_tclkstable, 0x00000004);
        WRITE_PARAM_ALL_REG(table, emc_tclkstop, 0x00000017);
        WRITE_PARAM_ALL_REG(table, emc_trefbw, 0x00001c6d);
        WRITE_PARAM_ALL_REG(table, emc_tppd, 0x00000004);
        WRITE_PARAM_ALL_REG(table, emc_odt_write, 0x00000000);
        WRITE_PARAM_ALL_REG(table, emc_pdex2mrr, 0x00000037);
        WRITE_PARAM_ALL_REG(table, emc_wext, 0x00000016);
        WRITE_PARAM_ALL_REG(table, emc_rfc_slr, 0x00000000);
        WRITE_PARAM_ALL_REG(table, emc_mrs_wait_cnt2, 0x01d3001b);
        WRITE_PARAM_ALL_REG(table, emc_mrs_wait_cnt, 0x074a0034);
        table->emc_mrs = 0x00000000;
        table->emc_emrs = 0x00000000;
        table->emc_mrw = 0x00170040;
        WRITE_PARAM_ALL_REG(table, emc_fbio_spare, 0x00000012);
        WRITE_PARAM_ALL_REG(table, emc_fbio_cfg5, 0x9160a00d);
        WRITE_PARAM_ALL_REG(table, emc_pdex2cke, 0x00000002);
        WRITE_PARAM_ALL_REG(table, emc_cke2pden, 0x00000010);
        WRITE_PARAM_ALL_REG(table, emc_r2r, 0x00000000);
        WRITE_PARAM_ALL_REG(table, emc_einput, 0x00000015);
        WRITE_PARAM_ALL_REG(table, emc_einput_duration, 0x00000020);
        WRITE_PARAM_ALL_REG(table, emc_puterm_extra, 0x00000001);
        WRITE_PARAM_ALL_REG(table, emc_tckesr, 0x0000001c);
        WRITE_PARAM_ALL_REG(table, emc_tpd, 0x0000000e);
        table->emc_cfg_2 = 0x0011083d;
        WRITE_PARAM_ALL_REG(table, emc_cfg_dig_dll, 0x002c03a9);
        WRITE_PARAM_ALL_REG(table, emc_cfg_dig_dll_period, 0x00008000);
        WRITE_PARAM_ALL_REG(table, emc_rdv_mask, 0x00000040);
        WRITE_PARAM_ALL_REG(table, emc_wdv_mask, 0x00000010);
        WRITE_PARAM_ALL_REG(table, emc_rdv_early_mask, 0x0000003e);
        WRITE_PARAM_ALL_REG(table, emc_rdv_early, 0x0000003c);
        WRITE_PARAM_ALL_REG(table, emc_fdpd_ctrl_dq, 0x8020221f);
        WRITE_PARAM_ALL_REG(table, emc_fdpd_ctrl_cmd, 0x0220f40f);
        table->emc_sel_dpd_ctrl = 0x0004000c;
        WRITE_PARAM_ALL_REG(table, emc_pre_refresh_req_cnt, 0x0000070b);
        WRITE_PARAM_ALL_REG(table, emc_dyn_self_ref_control, 0x80003873);
        WRITE_PARAM_ALL_REG(table, emc_txsrdll, 0x00000219);
        WRITE_PARAM_ALL_REG(table, emc_ibdly, 0x1000001f);
        WRITE_PARAM_ALL_REG(table, emc_obdly, 0x10000004);
        WRITE_PARAM_ALL_REG(table, emc_txdsrvttgen, 0x00000000);
        WRITE_PARAM_ALL_REG(table, emc_we_duration, 0x0000000e);
        WRITE_PARAM_ALL_REG(table, emc_ws_duration, 0x00000008);
        WRITE_PARAM_ALL_REG(table, emc_wev, 0x0000000c);
        WRITE_PARAM_ALL_REG(table, emc_cfg_3, 0x00000040);
        WRITE_PARAM_ALL_REG(table, emc_wdv_chk, 0x00000006);
        WRITE_PARAM_ALL_REG(table, emc_cfg_pipe_2, 0x00000000);
        WRITE_PARAM_ALL_REG(table, emc_cfg_pipe_1, 0x0fff0000);
        WRITE_PARAM_ALL_REG(table, emc_cfg_pipe, 0x0fff0000);
        WRITE_PARAM_ALL_REG(table, emc_quse_width, 0x00000009);
        WRITE_PARAM_ALL_REG(table, emc_puterm_width, 0x80000000);
        WRITE_PARAM_ALL_REG(table, emc_fbio_cfg7, 0x00003bff);
        WRITE_PARAM_ALL_REG(table, emc_rfcpb, 0x00000106);
        WRITE_PARAM_ALL_REG(table, emc_ccdmw, 0x00000020);
        WRITE_PARAM_ALL_REG(table, emc_config_sample_delay, 0x00000020);
        table->dram_timings.t_rp = 0x00000106;
        table->dram_timings.t_rfc = 0x0000020b;

        table->dram_timings.rl = 32;                      /*                                                    */
        WRITE_PARAM_ALL_REG(table, emc_wdv, 0x00000010);  /*                                                    */
        WRITE_PARAM_ALL_REG(table, emc_quse, 0x00000028); /*                                                    */
        WRITE_PARAM_ALL_REG(table, emc_qrst, 0x0007000c); /* These timings cause issues and I have no idea why. */
        WRITE_PARAM_ALL_REG(table, emc_rdv, 0x0000003e);  /*                                                    */
        WRITE_PARAM_ALL_REG(table, emc_wsv, 0x0000000e);  /*                                                    */
        WRITE_PARAM_ALL_REG(table, emc_qpop, 0x00000030); /*                                                    */

        table->burst_mc_regs.mc_emem_arb_cfg = 0x0000000E;
        table->burst_mc_regs.mc_emem_arb_outstanding_req = 0x80000080;
        table->burst_mc_regs.mc_emem_arb_timing_rcd = 0x00000007;
        table->burst_mc_regs.mc_emem_arb_timing_rp = 0x00000008;
        table->burst_mc_regs.mc_emem_arb_timing_rc = 0x0000001C;
        table->burst_mc_regs.mc_emem_arb_timing_ras = 0x00000012;
        table->burst_mc_regs.mc_emem_arb_timing_faw = 0x00000012;
        table->burst_mc_regs.mc_emem_arb_timing_rrd = 0x00000004;
        table->burst_mc_regs.mc_emem_arb_timing_rap2pre = 0x00000004;
        table->burst_mc_regs.mc_emem_arb_timing_wap2pre = 0x0000000F;
        table->burst_mc_regs.mc_emem_arb_timing_r2r = 0x00000001;
        table->burst_mc_regs.mc_emem_arb_timing_w2w = 0x00000001;
        table->burst_mc_regs.mc_emem_arb_timing_r2w = 0x0000000D;
        table->burst_mc_regs.mc_emem_arb_timing_w2r = 0x0000000B;
        table->burst_mc_regs.mc_emem_arb_da_turns = 0x05060000;
        table->burst_mc_regs.mc_emem_arb_da_covers = 0x000F0A0E;
        table->burst_mc_regs.mc_emem_arb_misc0 = 0x726E2A1D;
        table->burst_mc_regs.mc_emem_arb_misc1 = 0x70000F0F;
        table->burst_mc_regs.mc_emem_arb_misc2 = 0x00000000;
        table->burst_mc_regs.mc_emem_arb_ring1_throttle = 0x001F0000;
        table->burst_mc_regs.mc_emem_arb_timing_rfcpb = 0x00000041;
        table->burst_mc_regs.mc_emem_arb_timing_ccdmw = 0x00000008;
        table->burst_mc_regs.mc_emem_arb_dhyst_ctrl = 0x00000002;
        table->burst_mc_regs.mc_emem_arb_dhyst_timeout_util_0 = 0x0000001A;
        table->burst_mc_regs.mc_emem_arb_dhyst_timeout_util_1 = 0x0000001A;
        table->burst_mc_regs.mc_emem_arb_dhyst_timeout_util_2 = 0x0000001A;
        table->burst_mc_regs.mc_emem_arb_dhyst_timeout_util_3 = 0x0000001A;
        table->burst_mc_regs.mc_emem_arb_dhyst_timeout_util_4 = 0x0000001A;
        table->burst_mc_regs.mc_emem_arb_dhyst_timeout_util_5 = 0x0000001A;
        table->burst_mc_regs.mc_emem_arb_dhyst_timeout_util_6 = 0x0000001A;
        table->burst_mc_regs.mc_emem_arb_dhyst_timeout_util_7 = 0x0000001A;
        table->la_scale_regs.mc_mll_mpcorer_ptsa_rate = 0x000000F2;
        table->la_scale_regs.mc_ftop_ptsa_rate = 0x0000001B;
        table->la_scale_regs.mc_ptsa_grant_decrement = 0x00001501;
        table->la_scale_regs.mc_latency_allowance_avpc_0 = 0x006D0004;
        table->la_scale_regs.mc_latency_allowance_sdmmcaa_0 = 0x006D0005;
        table->la_scale_regs.mc_latency_allowance_sdmmca_0 = 0x006D0014;
        table->la_scale_regs.mc_latency_allowance_isp2_0 = 0x0000002C;
        table->la_scale_regs.mc_latency_allowance_isp2_1 = 0x006D006D;
        table->la_scale_regs.mc_latency_allowance_vic_0 = 0x006D0019;
        table->la_scale_regs.mc_latency_allowance_nvdec_0 = 0x006D0095;
        table->la_scale_regs.mc_latency_allowance_tsec_0 = 0x006D0041;
        table->la_scale_regs.mc_latency_allowance_ppcs_1 = 0x006D0080;
        table->la_scale_regs.mc_latency_allowance_xusb_0 = 0x006D003D;
        table->la_scale_regs.mc_latency_allowance_ppcs_0 = 0x00340049;
        table->la_scale_regs.mc_latency_allowance_gpu2_0 = 0x006D0016;
        table->la_scale_regs.mc_latency_allowance_hc_1 = 0x0000006D;
        table->la_scale_regs.mc_latency_allowance_sdmmc_0 = 0x006D0090;
        table->la_scale_regs.mc_latency_allowance_mpcore_0 = 0x006D0004;
        table->la_scale_regs.mc_latency_allowance_vi2_0 = 0x0000006D;
        table->la_scale_regs.mc_latency_allowance_hc_0 = 0x00080013;
        table->la_scale_regs.mc_latency_allowance_gpu_0 = 0x006D0016;
        table->la_scale_regs.mc_latency_allowance_sdmmcab_0 = 0x006D0005;
        table->la_scale_regs.mc_latency_allowance_nvenc_0 = 0x006D0018;
    }

    void MemMtcTableAutoAdjust(MarikoMtcTable *table) {
        /* Official Tegra X1 TRM, sign up for nvidia developer program (free) to download:
         *     https://developer.nvidia.com/embedded/dlc/tegra-x1-technical-reference-manual
         *     Section 18.11: MC Registers
         *
         * Retail Mariko: 200FBGA 16Gb DDP LPDDR4X SDRAM x 2
         * x16/Ch, 1Ch/die, Double-die, 2Ch, 1CS(rank), 8Gb density per die
         * 64Mb x 16DQ x 8banks x 2channels = 2048MB (x32DQ) per package
         *
         * Devkit Mariko: 200FBGA 32Gb DDP LPDDR4X SDRAM x 2
         * x16/Ch, 1Ch/die, Quad-die,   2Ch, 2CS(rank), 8Gb density per die
         * X1+ EMC can R/W to both ranks at the same time, resulting in doubled DQ
         * 64Mb x 32DQ x 8banks x 2channels = 4096MB (x64DQ) per package
         *
         * If you have access to LPDDR4(X) specs or datasheets (from manufacturers or Google),
         * you'd better calculate timings yourself rather than relying on following algorithm.
         */

        #define WRITE_PARAM_ALL_REG(TABLE, PARAM, VALUE) \
            TABLE->burst_regs.PARAM = VALUE;             \
            TABLE->shadow_regs_ca_train.PARAM   = VALUE; \
            TABLE->shadow_regs_rdwr_train.PARAM = VALUE;

        #define GET_CYCLE(PARAM) ((u32)((double)(PARAM) / tCK_avg))

        WRITE_PARAM_ALL_REG(table, emc_rc, 0x60);
//        WRITE_PARAM_ALL_REG(table, emc_rfc, GET_CYCLE_CEIL(tRFCab));
//        WRITE_PARAM_ALL_REG(table, emc_rfcpb, GET_CYCLE_CEIL(tRFCpb));
//        WRITE_PARAM_ALL_REG(table, emc_ras, GET_CYCLE_CEIL(tRAS));
//        WRITE_PARAM_ALL_REG(table, emc_rp, GET_CYCLE_CEIL(tRPpb));
//        WRITE_PARAM_ALL_REG(table, emc_r2p, GET_CYCLE_CEIL(tRTP));
//        WRITE_PARAM_ALL_REG(table, emc_rd_rcd, GET_CYCLE_CEIL(tRCD));
//        WRITE_PARAM_ALL_REG(table, emc_wr_rcd, GET_CYCLE_CEIL(tRCD));
//        WRITE_PARAM_ALL_REG(table, emc_rrd, GET_CYCLE_CEIL(tRRD));
//        WRITE_PARAM_ALL_REG(table, emc_refresh, REFRESH);
//        WRITE_PARAM_ALL_REG(table, emc_pre_refresh_req_cnt, REFRESH / 4);
//
//        WRITE_PARAM_ALL_REG(table, emc_r2w, R2W);
//        WRITE_PARAM_ALL_REG(table, emc_w2r, W2R);
//        WRITE_PARAM_ALL_REG(table, emc_w2p, WTP);
//
//        /* May or may not have to be patched in Micron; let's skip for now. */
//        if (!IsMicron())
//        {
//            WRITE_PARAM_ALL_REG(table, emc_pdex2wr, GET_CYCLE_CEIL(tXP));
//            WRITE_PARAM_ALL_REG(table, emc_pdex2rd, GET_CYCLE_CEIL(tXP));
//        }
//
//        WRITE_PARAM_ALL_REG(table, emc_txsr, MIN(GET_CYCLE_CEIL(tXSR), (u32)0x3fe));
//        WRITE_PARAM_ALL_REG(table, emc_txsrdll, MIN(GET_CYCLE_CEIL(tXSR), (u32)0x3fe));
//        WRITE_PARAM_ALL_REG(table, emc_tckesr, GET_CYCLE_CEIL(tSR));
//        WRITE_PARAM_ALL_REG(table, emc_tfaw, GET_CYCLE_CEIL(tFAW));
//        WRITE_PARAM_ALL_REG(table, emc_trpab, GET_CYCLE_CEIL(tRPab));
//        WRITE_PARAM_ALL_REG(table, emc_trefbw, REFBW);
//
///* Worth replacing with l4t dumps at some point. */
//// Burst MC Regs
//#define WRITE_PARAM_BURST_MC_REG(TABLE, PARAM, VALUE) TABLE->burst_mc_regs.PARAM = VALUE;
//
//        constexpr u32 MC_ARB_DIV = 4;
//        constexpr u32 MC_ARB_SFA = 2;
//
//        WRITE_PARAM_BURST_MC_REG(table, mc_emem_arb_cfg, C.marikoEmcMaxClock / (33.3 * 1000) / MC_ARB_DIV); // CYCLES_PER_UPDATE: The number of mcclk cycles per deadline timer update
//        WRITE_PARAM_BURST_MC_REG(table, mc_emem_arb_timing_rcd, CEIL(GET_CYCLE_CEIL(tRCD) / MC_ARB_DIV) - 2)
//        WRITE_PARAM_BURST_MC_REG(table, mc_emem_arb_timing_rp, CEIL(GET_CYCLE_CEIL(tRPpb) / MC_ARB_DIV) - 1 + MC_ARB_SFA)
//        WRITE_PARAM_BURST_MC_REG(table, mc_emem_arb_timing_rc, CEIL(GET_CYCLE_CEIL(tRC) / MC_ARB_DIV) - 1)
//        WRITE_PARAM_BURST_MC_REG(table, mc_emem_arb_timing_ras, CEIL(GET_CYCLE_CEIL(tRAS) / MC_ARB_DIV) - 2)
//        WRITE_PARAM_BURST_MC_REG(table, mc_emem_arb_timing_faw, CEIL(GET_CYCLE_CEIL(tFAW) / MC_ARB_DIV) - 1)
//        WRITE_PARAM_BURST_MC_REG(table, mc_emem_arb_timing_rrd, CEIL(GET_CYCLE_CEIL(tRRD) / MC_ARB_DIV) - 1)
//        WRITE_PARAM_BURST_MC_REG(table, mc_emem_arb_timing_rap2pre, CEIL(GET_CYCLE_CEIL(tRTP) / MC_ARB_DIV))
//        WRITE_PARAM_BURST_MC_REG(table, mc_emem_arb_timing_wap2pre, CEIL((WTP) / MC_ARB_DIV))
//        WRITE_PARAM_BURST_MC_REG(table, mc_emem_arb_timing_r2r, CEIL(table->burst_regs.emc_rext / MC_ARB_DIV) - 1 + MC_ARB_SFA)
//        WRITE_PARAM_BURST_MC_REG(table, mc_emem_arb_timing_r2w, CEIL((R2W) / MC_ARB_DIV) - 1 + MC_ARB_SFA)
//        WRITE_PARAM_BURST_MC_REG(table, mc_emem_arb_timing_w2r, CEIL((W2R) / MC_ARB_DIV) - 1 + MC_ARB_SFA)
//        WRITE_PARAM_BURST_MC_REG(table, mc_emem_arb_timing_rfcpb, CEIL(GET_CYCLE_CEIL(tRFCpb) / MC_ARB_DIV))
    }

    void MemMtcPllmbDivisor(MarikoMtcTable *table)
    {
        // Calculate DIVM and DIVN (clock divisors)
        // Common PLL oscillator is 38.4 MHz
        // PLLMB_OUT = 38.4 MHz / PLLLMB_DIVM * PLLMB_DIVN
        typedef struct
        {
            u8 numerator : 4;
            u8 denominator : 4;
        } pllmb_div;

        constexpr pllmb_div div[] = {
            {3, 4}, {2, 3}, {1, 2}, {1, 3}, {1, 4}, {0, 2}};

        constexpr u32 pll_osc_in = 38'400;
        u32 divm{}, divn{};
        const u32 remainder = C.marikoEmcMaxClock % pll_osc_in;
        for (const auto &index : div)
        {
            // Round down
            if (remainder >= pll_osc_in * index.numerator / index.denominator)
            {
                divm = index.denominator;
                divn = C.marikoEmcMaxClock / pll_osc_in * divm + index.numerator;
                break;
            }
        }

        table->pllmb_divm = divm;
        table->pllmb_divn = divn;
    }

    Result MemFreqMtcTable(u32 *ptr)
    {
        u32 khz_list[] = {1600000, 1331200, 204000};
        u32 khz_list_size = sizeof(khz_list) / sizeof(u32);

        // Generate list for mtc table pointers
        MarikoMtcTable *table_list[khz_list_size];
        for (u32 i = 0; i < khz_list_size; i++)
        {
            u8 *table = reinterpret_cast<u8 *>(ptr) - offsetof(MarikoMtcTable, rate_khz) - i * sizeof(MarikoMtcTable);
            table_list[i] = reinterpret_cast<MarikoMtcTable *>(table);
            R_UNLESS(table_list[i]->rate_khz == khz_list[i], ldr::ResultInvalidMtcTable());
            R_UNLESS(table_list[i]->rev == MTC_TABLE_REV, ldr::ResultInvalidMtcTable());
        }

        if (C.marikoEmcMaxClock <= EmcClkOSLimit)
            R_SKIP();

        MarikoMtcTable *table_alt = table_list[1], *table_max = table_list[0];
        MarikoMtcTable *tmp = new MarikoMtcTable;

        // Copy unmodified 1600000 table to tmp
        std::memcpy(reinterpret_cast<void *>(tmp), reinterpret_cast<void *>(table_max), sizeof(MarikoMtcTable));
        // Adjust max freq mtc timing parameters with reference to 1331200 table
        /* TODO: Implement mariko */

        if (C.mtcConf == AUTO_ADJ) {
            MemMtcTableAutoAdjust(table_max);
        } else {
            MemMtcTableAutoAdjustBaseLatency(table_max);
        }
        MemMtcPllmbDivisor(table_max);
        // Overwrite 13312000 table with unmodified 1600000 table copied back
        std::memcpy(reinterpret_cast<void *>(table_alt), reinterpret_cast<void *>(tmp), sizeof(MarikoMtcTable));

        delete tmp;

        PATCH_OFFSET(ptr, C.marikoEmcMaxClock);

        // Handle customize table replacement
        // if (C.mtcConf == CUSTOMIZED_ALL) {
        //    MemMtcCustomizeTable(table_list[0], reinterpret_cast<MarikoMtcTable *>(reinterpret_cast<u8 *>(C.marikoMtcTable)));
        // }

        R_SUCCEED();
    }

    Result MemFreqDvbTable(u32 *ptr)
    {
        emc_dvb_dvfs_table_t *default_end = reinterpret_cast<emc_dvb_dvfs_table_t *>(ptr);
        emc_dvb_dvfs_table_t *new_start = default_end + 1;

        // Validate existing table
        void *mem_dvb_table_head = reinterpret_cast<u8 *>(new_start) - sizeof(EmcDvbTableDefault);
        bool validated = std::memcmp(mem_dvb_table_head, EmcDvbTableDefault, sizeof(EmcDvbTableDefault)) == 0;
        R_UNLESS(validated, ldr::ResultInvalidDvbTable());

        if (C.marikoEmcMaxClock <= EmcClkOSLimit)
            R_SKIP();

        int32_t voltAdd = 25 * C.EmcDvbShift;

#define DVB_VOLT(zero, one, two) std::min(zero + voltAdd, 1050), std::min(one + voltAdd, 1025), std::min(two + voltAdd, 1000),

        if (C.marikoEmcMaxClock < 1862400)
        {
            std::memcpy(new_start, default_end, sizeof(emc_dvb_dvfs_table_t));
        }
        else if (C.marikoEmcMaxClock < 2131200)
        {
            emc_dvb_dvfs_table_t oc_table = {1862400, {
                                                          700,
                                                          675,
                                                          650,
                                                      }};
            std::memcpy(new_start, &oc_table, sizeof(emc_dvb_dvfs_table_t));
        }
        else if (C.marikoEmcMaxClock < 2400000)
        {
            emc_dvb_dvfs_table_t oc_table = {2131200, {
                                                          725,
                                                          700,
                                                          675,
                                                      }};
            std::memcpy(new_start, &oc_table, sizeof(emc_dvb_dvfs_table_t));
        }
        else if (C.marikoEmcMaxClock < 2665600)
        {
            emc_dvb_dvfs_table_t oc_table = {2400000, {DVB_VOLT(750, 725, 700)}};
            std::memcpy(new_start, &oc_table, sizeof(emc_dvb_dvfs_table_t));
        }
        else if (C.marikoEmcMaxClock < 2931200)
        {
            emc_dvb_dvfs_table_t oc_table = {2665600, {DVB_VOLT(775, 750, 725)}};
            std::memcpy(new_start, &oc_table, sizeof(emc_dvb_dvfs_table_t));
        }
        else if (C.marikoEmcMaxClock < 3200000)
        {
            emc_dvb_dvfs_table_t oc_table = {2931200, {DVB_VOLT(800, 775, 750)}};
            std::memcpy(new_start, &oc_table, sizeof(emc_dvb_dvfs_table_t));
        }
        else
        {
            emc_dvb_dvfs_table_t oc_table = {3200000, {DVB_VOLT(800, 800, 775)}};
            std::memcpy(new_start, &oc_table, sizeof(emc_dvb_dvfs_table_t));
        }
        new_start->freq = C.marikoEmcMaxClock;
        /* Max dvfs entry is 32, but HOS doesn't seem to boot if exact freq doesn't exist in dvb table,
           reason why it's like this
        */

        R_SUCCEED();
    }

    Result MemFreqMax(u32 *ptr)
    {
        if (C.marikoEmcMaxClock <= EmcClkOSLimit)
            R_SKIP();

        PATCH_OFFSET(ptr, C.marikoEmcMaxClock);
        R_SUCCEED();
    }

    Result I2cSet_U8(I2cDevice dev, u8 reg, u8 val)
    {
        struct
        {
            u8 reg;
            u8 val;
        } __attribute__((packed)) cmd;

        I2cSession _session;
        Result res = i2cOpenSession(&_session, dev);
        if (R_FAILED(res))
            return res;

        cmd.reg = reg;
        cmd.val = val;
        res = i2csessionSendAuto(&_session, &cmd, sizeof(cmd), I2cTransactionOption_All);
        i2csessionClose(&_session);
        return res;
    }

    Result EmcVddqVolt(u32 *ptr)
    {
        regulator *entry = reinterpret_cast<regulator *>(reinterpret_cast<u8 *>(ptr) - offsetof(regulator, type_2_3.default_uv));

        constexpr u32 uv_step = 5'000;
        constexpr u32 uv_min = 250'000;

        auto validator = [entry]()
        {
            R_UNLESS(entry->id == 2, ldr::ResultInvalidRegulatorEntry());
            R_UNLESS(entry->type == 3, ldr::ResultInvalidRegulatorEntry());
            R_UNLESS(entry->type_2_3.step_uv == uv_step, ldr::ResultInvalidRegulatorEntry());
            R_UNLESS(entry->type_2_3.min_uv == uv_min, ldr::ResultInvalidRegulatorEntry());
            R_SUCCEED();
        };

        R_TRY(validator());

        u32 emc_uv = C.marikoEmcVddqVolt;
        if (!emc_uv)
            R_SKIP();

        if (emc_uv % uv_step)
            emc_uv = (emc_uv + uv_step - 1) / uv_step * uv_step; // rounding

        PATCH_OFFSET(ptr, emc_uv);

        i2cInitialize();
        I2cSet_U8(I2cDevice_Max77812_2, 0x25, (emc_uv - uv_min) / uv_step);
        i2cExit();

        R_SUCCEED();
    }

    void Patch(uintptr_t mapped_nso, size_t nso_size)
    {
        u32 CpuCvbDefaultMaxFreq = static_cast<u32>(GetDvfsTableLastEntry(CpuCvbTableDefault)->freq);
        u32 GpuCvbDefaultMaxFreq = static_cast<u32>(GetDvfsTableLastEntry(GpuCvbTableDefault)->freq);

        PatcherEntry<u32> patches[] = {
            {"CPU Freq Vdd", &CpuFreqVdd, 1, nullptr, CpuClkOSLimit},
            {"CPU Freq Table", CpuFreqCvbTable<true>, 1, nullptr, CpuCvbDefaultMaxFreq},
            {"CPU Volt Limit", &CpuVoltRange, 13, nullptr, CpuVoltOfficial},
            {"CPU Volt Dfll", &CpuVoltDfll, 1, nullptr, 0x0000FFCF},
            {"GPU Freq Table", GpuFreqCvbTable<true>, 1, nullptr, GpuCvbDefaultMaxFreq},
            {"GPU Freq Asm", &GpuFreqMaxAsm, 2, &GpuMaxClockPatternFn},
            {"GPU Freq PLL", &GpuFreqPllLimit, 1, nullptr, GpuClkPllLimit},
            {"MEM Freq Mtc", &MemFreqMtcTable, 0, nullptr, EmcClkOSLimit},
            {"MEM Freq Dvb", &MemFreqDvbTable, 1, nullptr, EmcClkOSLimit},
            {"MEM Freq Max", &MemFreqMax, 0, nullptr, EmcClkOSLimit},
            {"MEM Freq PLLM", &MemFreqPllmLimit, 2, nullptr, EmcClkPllmLimit},
            {"MEM Vddq", &EmcVddqVolt, 2, nullptr, EmcVddqDefault},
            {"MEM Vdd2", &MemVoltHandler, 2, nullptr, MemVdd2Default},
            {"GPU Vmin", &GpuVmin, 0, nullptr, gpuVmin},
            {"GPU Vmax", &GpuVmax, 0, nullptr, gpuVmax},
        };

        for (uintptr_t ptr = mapped_nso;
             ptr <= mapped_nso + nso_size - sizeof(MarikoMtcTable);
             ptr += sizeof(u32))
        {
            u32 *ptr32 = reinterpret_cast<u32 *>(ptr);
            for (auto &entry : patches)
            {
                if (R_SUCCEEDED(entry.SearchAndApply(ptr32)))
                    break;
            }
        }

        for (auto &entry : patches)
        {
            LOGGING("%s Count: %zu", entry.description, entry.patched_count);
            if (R_FAILED(entry.CheckResult()))
                CRASH(entry.description);
        }
    }

}
