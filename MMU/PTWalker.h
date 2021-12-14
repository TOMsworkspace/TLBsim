/**
 * @file PTWalker.h
 * @author TOMsworkspace (2683322180@qq.com)
 * @brief 
 * @version 1.0
 * @date 2021-12-13
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef TLBSIM_PTWALKER_H_
#define TLBSIM_PTWALKER_H_

#include "Set_Cache.h"
#include "Direct_Cache.h"
#include "config.h"

class PTW
{

private:
    /* data */
    Cache * cache;

    // PTW项数目
    _u64 PTW_entry;

    //PTW延迟
    //int latency;

    _u64 Level_PTC_hit_count[3];

    _u64 Level_PTC_access_count[3];

    _u64 PTW_access_count;


public:
    PTW(const _u32 PTW_entry_num, enum cache_replacement_algo algo, const _u32& way);
    ~PTW();

    /**
     * @brief 
     * 
     * @param v_addr 虚拟地址
     * @return true : 命中
     * @return false ： 未命中
     */
    bool PTW_handle(_u64 v_addr);

    bool flush();

    _u64 get_op_count() const;

    _u64 get_overall_op_count() const;


    _u64 get_hit_count() const;

    _u64 get_overall_hit_count() const;

    
    _u64 get_miss_count() const;

    _u64 get_overall_miss_count() const;


    double get_hit_rate() const;

    double get_overall_hit_rate() const;


    double get_L2_PTC_hit_rate() const;

    double get_L3_PTC_hit_rate() const;

    double get_L4_PTC_hit_rate() const;

    _u64 get_L2_PTC_hit_count() const;

    _u64 get_L3_PTC_hit_count() const;

    _u64 get_L4_PTC_hit_count() const;

    _u64 get_L2_PTC_miss_count() const;

    _u64 get_L3_PTC_miss_count() const;

    _u64 get_L4_PTC_miss_count() const;
};



#endif