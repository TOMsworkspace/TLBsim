/**
 * @file PTWalker.cpp
 * @author TOMsworkspace (2683322180@qq.com)
 * @brief 
 * @version 1.0
 * @date 2021-12-13
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "PTWalker.h"

PTW::PTW(const _u32 PTW_entry_num, enum cache_replacement_algo algo, const _u32& way):PTW_entry(PTW_entry_num)
{

    PTW_access_count = 0;

    if(way == 1){
        // 直接相联
        cache = new Direct_Cache(4096 * PTW_entry_num, 4096);
    }
    else
    {
        //全相联 / 组相联
        cache = new Set_Cache(4096 * PTW_entry_num, 4096, algo, way);
    }

    for(int i = 0; i < 3; ++i){
        Level_PTC_hit_count[i] = 0;
        Level_PTC_access_count[i] = 0;
    }
    
}

PTW::~PTW()
{
}

bool PTW::PTW_handle(_u64 v_addr){

    PTW_access_count++;

    int level;

    for(level = 1; level <= 3; ++level){
        Level_PTC_access_count[level - 1]++;
        if(cache->check_cache_hit(v_addr >> (9 * level)))
            break;
        
    }

    //level 是命中的一级，更新前1到level-1级
    
    for(int i = std::min(level, 3); i >= 1; --i){
        cache->do_cache_op(v_addr >> (9 * i));
    }

    if(level <= 3){
        Level_PTC_hit_count[level - 1]++;
        return true;
    }

    return false;
}

bool PTW::flush(){
    cache->flush_cache();

    for(int i = 0; i < 3; ++i){
        this->Level_PTC_hit_count[i] = 0;
        this->Level_PTC_access_count[i] = 0;
    }

    this->PTW_access_count = 0;

    return true;
}

_u64 PTW::get_op_count() const{
    return this->PTW_access_count;
}

_u64 PTW::get_overall_op_count() const{
    return this->cache->get_request_count();
}

_u64 PTW::get_hit_count() const{
    return this->cache->get_hit_count();
}

_u64 PTW::get_overall_hit_count() const{
    return this->cache->get_hit_count();
}


_u64 PTW::get_miss_count() const{
    return PTW_access_count - cache->get_hit_count();
}

_u64 PTW::get_overall_miss_count() const{
    return this->cache->get_request_count() - this->cache->get_hit_count();
}


double PTW::get_hit_rate() const{
    return 1.0 * cache->get_hit_count() / PTW_access_count;
}

double PTW::get_overall_hit_rate() const{
    return 1.0 * cache->get_hit_rate();
}


double PTW::get_L2_PTC_hit_rate() const{
    return 1.0 * Level_PTC_hit_count[0] / Level_PTC_access_count[0];
}

double PTW::get_L3_PTC_hit_rate() const{
    return 1.0 * Level_PTC_hit_count[1] / Level_PTC_access_count[1];
}

double PTW::get_L4_PTC_hit_rate() const{
    return 1.0 * Level_PTC_hit_count[2] / Level_PTC_access_count[2];
}

_u64 PTW::get_L2_PTC_hit_count() const{
    return Level_PTC_hit_count[0];
}

_u64 PTW::get_L3_PTC_hit_count() const{
    return Level_PTC_hit_count[1];
}

_u64 PTW::get_L4_PTC_hit_count() const{
    return Level_PTC_hit_count[2];
}

_u64 PTW::get_L2_PTC_miss_count() const{
    return Level_PTC_access_count[0] - Level_PTC_hit_count[0];
}

_u64 PTW::get_L3_PTC_miss_count() const{
    return Level_PTC_access_count[1] - Level_PTC_hit_count[1];
}

_u64 PTW::get_L4_PTC_miss_count() const{
    return Level_PTC_access_count[2] - Level_PTC_hit_count[2];
}