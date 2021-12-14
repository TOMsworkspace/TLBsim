/**
 * @file config.h
 * @author TOMsworkspace (2683322180@qq.com)
 * @brief 
 * @version 1.0
 * @date 2021-11-09
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef TLBSIM_CONFIG_H_
#define TLBSIM_CONFIG_H_

typedef unsigned long long _u64;
typedef unsigned int _u32;

const float sys_freq = 2200000000; //hz
const float cycle_to_time = 1e9 / sys_freq; // ns / cycles; 

const _u32 DRAM_READ_RAND = 101; //ns
const _u32 DRAM_READ_SEQ = 81; //
const _u32 DRAM_WRITE_SEQ = 86;
const _u32 DRAM_WRITE_RAND = 101; 
const _u32 NVM_READ_RAND = 165;
const _u32 NVM_READ_SEQ = 169;
const _u32 NVM_WRITE_RAND = 90;
const _u32 NVM_WRITE_SEQ = 62; 

const _u32 L1_TLB_latency=4; //3 cycles
const _u32 L2_TLB_latency=4;
const _u32 L3_PTC_latency=7;
const _u32 L3_unifiedTP_latency=7;
const _u32 DRAM_latency=80;

#endif