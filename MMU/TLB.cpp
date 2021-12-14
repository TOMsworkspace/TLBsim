/**
 * @file TLB.cpp
 * @author TOMsworkspace (2683322180@qq.com)
 * @brief 
 * @version 1.0
 * @date 2021-12-13
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "TLB.h"

TLB::TLB(const _u32 TLB_entry_num, enum cache_replacement_algo algo, const _u32& way):TLB_entry(TLB_entry_num)
{
    if(way == 1){
        // 直接相联
        cache = new Direct_Cache(4096 * TLB_entry_num, 4096);
    }
    else
    {
        //全相联 / 组相联
        cache = new Set_Cache(4096 * TLB_entry_num, 4096, algo, way);
    }
    
}

TLB::~TLB()
{

}

bool TLB::TLB_handle(_u64 v_addr){
    return cache->do_cache_op(v_addr);
}

bool TLB::flush(){
    cache->flush_cache();
    return true;
}

_u64 TLB::get_op_count() const{
    return this->cache->get_request_count();
}

_u64 TLB::get_hit_count() const{
    return this->cache->get_hit_count();
}

_u64 TLB::get_miss_count() const{
    return this->cache->get_miss_count();
}

double TLB::get_TLB_hit_rate() const{
    return this->cache->get_hit_rate();
}