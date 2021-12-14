/**
 * @file UnifiedTP.h
 * @author TOMsworkspace (2683322180@qq.com)
 * @brief 
 * @version 1.0
 * @date 2021-12-13
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef TLBSIM_UNIFIEDTP_H_
#define TLBSIM_UNIFIEDTP_H_

#include "Set_Cache.h"
#include "Direct_Cache.h"
#include "config.h"

enum unifiedTP_type{u_PTC, u_TLB};

class unifiedTP
{

private:
    /* data */
    Cache * cache;

    // unified_tp项数目
    _u64 unifiedTP_entry;

    //unifiedTP延迟
    //int latency;

    _u64 Level_PTC_hit_count[3]; //每层PTC命中次数

    _u64 Level_PTC_access_count[3]; //每层PTC访问次数

    _u64 PTW_access_count; //PTW整体访问次数，多级查询算一次访问

    _u64 TLB_access_count; //TLB访问次数

    _u64 TLB_hit_count; //TLB命中次数

    _u64 unifiedTP_access_count; //Vaddr to addr请求次数


public:
    unifiedTP(const _u32 unifiedTP_entry_num, enum cache_replacement_algo algo, const _u32& way);
    ~unifiedTP();

    /**
     * @brief 
     * 
     * @param v_addr 虚拟地址
     * @return true : 命中
     * @return false ： 未命中
     */
    bool unifiedTP_handle(_u64 v_addr, enum::unifiedTP_type type);

    bool flush();

    _u64 get_op_count() const;

    _u64 get_TLB_op_count() const;

    _u64 get_PTW_op_count() const;
    
    _u64 get_overall_op_count() const;


    double get_hit_rate() const;

    double get_TLB_hit_rate() const;

    double get_PTW_hit_rate() const;

    double get_overall_hit_rate() const;


    _u64 get_hit_count() const;

    _u64 get_TLB_hit_count() const;

    _u64 get_PTW_hit_count() const;

    _u64 get_overall_hit_count() const;

    
    _u64 get_miss_count() const;

    _u64 get_TLB_miss_count() const;

    _u64 get_PTW_miss_count() const;

    _u64 get_overall_miss_count() const;


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