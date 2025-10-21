/*
 * Copyright (c) 2023 hanai3Bi
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

#pragma once

#include "oc_common.hpp"

namespace ams::ldr::oc {
    #define MAX(A, B)   std::max(A, B)
    #define MIN(A, B)   std::min(A, B)
    #define CEIL(A)     std::ceil(A)
    #define FLOOR(A)    std::floor(A)

    /* Primary timings. */
    const std::array<u32,  8> tRCD_values  =  {18, 17, 16, 15, 14, 13, 12, 11};
    const std::array<u32,  8> tRP_values   =  {18, 17, 16, 15, 14, 13, 12, 11};
    const std::array<u32, 10> tRAS_values  =  {42, 36, 34, 32, 30, 28, 26, 24, 22, 20};

    /* Secondary timings. */
    const std::array<double,    8>  tRRD_values   = {10.0, 7.5, 6.0, 5.0, 4.0, 3.0, 2.0, 1.0};
    const std::array<u32,       6>  tRFC_values   = {90, 80, 70, 60, 50, 40};
    const std::array<u32,      10>  tWTR_values   = {10, 9, 8, 7, 6, 5, 4, 3, 3, 1};
    const std::array<u32,       6>  tREFpb_values = {3900, 5850, 7800, 11700, 15600, 99999};

    /* Set to 4 read and 2 write for 1866bl. */
    /* For 2131bl: 8 read and 4 write. */
    const u32 latency_offset_read = 0;
    const u32 latency_offset_write = 0;

    const u32 BL = 16;
    const u32 RL = 28 + latency_offset_read;
    const u32 WL = 14 + latency_offset_write;

    /* Precharge to Precharge Delay. (Cycles) */
    /* Don't touch! */
    const u32 tPPD = 4;

    /* DQS output access time from CK_t/CK_c. */
    const double tDQSCK_max = 3.5;
    const double tWPRE = 2.0;

    /* tCK Read postamble. */
    const double tRPST = 0.5;

    namespace pcv::erista {
        /* tCK_avg may have to be improved... */
        const double tCK_avg = 1000'000.0 / C.eristaEmcMaxClock;

        /* Primary timings. */
        const u32 tRCD  = tRCD_values[C.t1_tRCD];
        const u32 tRPpb = tRP_values[C.t2_tRP];
        const u32 tRAS  = tRAS_values[C.t3_tRAS];

        /* Secondary timings. */
        const double tRRD = tRRD_values[C.t4_tRRD];
        const u32 tRFCpb = tRFC_values[C.t5_tRFC];
        const u32 tWTR   = tWTR_values[C.t7_tWTR];
        const u32 tREFpb = tREFpb_values[C.t8_tREFI];

        /* Four-bank ACTIVATE Window */
        const u32 tFAW = (u32) (tRRD * 4.0);

        /* Latency stuff. */
        const u32 tR2W = (u32)(((((double)(long)(3.5 / tCK_avg) + 32.0 + (BL / 2)) - 14.0) + tWPRE + 12.0) - (double)(C.t6_tRTW * 3));
        const u32 tW2R = static_cast<u32>((static_cast<long>(tWTR / tCK_avg) + 23.0) - (BL / 2.0));
        const u32 tRW2PDEN = (u32) ((double) (u64)(1.25 / tCK_avg) + 46.0 + (double) (u64)(0.8 / tCK_avg) + 6.0);

        /* Refresh Cycle time. (All Banks) */
        const u32 tRFCab = tRFCpb * 2;

        /* Do not touch stuff. */
        /* ACTIVATE-to-ACTIVATE command period. (same bank) */
        const u32 tRC = tRAS + tRPpb;

        /* Minimum Self-Refresh Time. (Entry to Exit) */
        const double tSR = 15.0;
        /* SELF REFRESH exit to next valid command delay. */
        const double tXSR = (double) (tRFCab + 7.5);

        /* Exit power down to next valid command delay. */
        const double tXP = 7.5; // I assume this is correct.

        /* u32ernal READ to PRECHARGE command delay. */
        const u32 pdex2mrr = (u32) (tCK_avg * 3.0 + (double) tRCD_values[C.t1_tRCD] + 10.0);

        /* Row Precharge Time. (all banks) */
        const double tRPab = tRPpb + 3;
    }

    /* TODO. */
    namespace pcv::mariko {

    }

}
