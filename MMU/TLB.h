/**
 * @file TLB.h
 * @author TOMsworkspace (2683322180@qq.com)
 * @brief 
 * @version 1.0
 * @date 2021-12-13
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef TLBSIM_TLB_H_
#define TLBSIM_TLB_H_

#include "Set_Cache.h"
#include "Direct_Cache.h"
#include "config.h"

class TLB
{

private:
    /* data */
    Cache * cache;

    // TLB项数目
    _u64 TLB_entry;

    //TLB延迟
    //int latency;


public:
    TLB(const _u32 TLB_entry_num, enum cache_replacement_algo algo, const _u32& way);
    ~TLB();

    /**
     * @brief 
     * 
     * @param v_addr 虚拟地址
     * @return true : 命中
     * @return false ： 未命中
     */
    bool TLB_handle(_u64 v_addr);

    bool flush();

    _u64 get_op_count() const;

    _u64 get_hit_count() const;

    _u64 get_miss_count() const;

    double get_TLB_hit_rate() const;
};

#endif