/**
 * @file UnifiedTP.cpp
 * @author TOMsworkspace (2683322180@qq.com)
 * @brief 
 * @version 1.0
 * @date 2021-12-13
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "UnifiedTP.h"

unifiedTP::unifiedTP(const _u32 unifiedTP_entry_num, enum cache_replacement_algo algo, const _u32& way):unifiedTP_entry(unifiedTP_entry_num)
{

    unifiedTP_access_count = 0;

    PTW_access_count = 0; //PTW整体访问次数，多级查询算一次访问

    TLB_access_count = 0; //TLB访问次数

    TLB_hit_count = 0; //TLB命中次数

    unifiedTP_access_count = 0; //Vaddr to addr请求次数

    if(way == 1){
        // 直接相联
        cache = new Direct_Cache(4096 * unifiedTP_entry_num, 4096);
    }
    else
    {
        //全相联 / 组相联
        cache = new Set_Cache(4096 * unifiedTP_entry_num, 4096, algo, way);
    }

    for(int i = 0; i < 3; ++i){
        Level_PTC_hit_count[i] = 0;
        Level_PTC_access_count[i] = 0;
    }
    
}

unifiedTP::~unifiedTP()
{
}

bool unifiedTP::unifiedTP_update(_u64 v_addr, enum unifiedTP_access_type type){
    

    switch (type)
    {
        case u_TLB:
            cache->do_cache_op(v_addr);
            break;
        case u_L2PTC:
            cache->do_cache_op(v_addr >> 9);
            break;
        case u_L3PTC:
            cache->do_cache_op(v_addr >> 18);
            break;
        case u_L4PTC:
            cache->do_cache_op(v_addr >> 27);
            break;
        default:
            return false;
    }

    return true;
    
}

bool unifiedTP::unifiedTP_victim_tag_update(_u64 victim_tag){
    if(victim_tag != INVALID_TAG){
        unifiedTP_update(victim_tag << PAGE_SIZE_SHIFT, u_TLB);
    }
    else
    {
        return false;
    }
}


unifiedTP_access_result_type unifiedTP::unifiedTP_check_hit(_u64 v_addr, enum unifiedTP_access_type type){
    unifiedTP_access_count++;

    //check TLB
    if(type == u_TLB){
        TLB_access_count++;

        if(cache->check_cache_hit(v_addr)){ //TLB命中
            TLB_hit_count++;

            return u_TLB_HIT;
        }
        else{
            return u_TLB_MISS;
        }
    }
    
    
    if(type == u_PTC){
    
        //check PTW

        PTW_access_count++;

        int level;

        for(level = 1; level <= 3; ++level){
            Level_PTC_access_count[level - 1]++;
            if(cache->check_cache_hit(v_addr >> (9 * level)))
                break;    
        }

        if(level <= 3){
            Level_PTC_hit_count[level - 1]++;
        }

        switch (level)
        {
        case 1:
            return u_L2PTC_HIT;
            break;
        case 2:
            return u_L3PTC_HIT;
            break;
        case 3:
            return u_L4PTC_HIT;
            break;    
        default:
            return u_L4PTC_MISS;
            break;
        }
    }
}

_u64 unifiedTP::get_victim_tag() const {
    return cache->get_victim_tag();
}

/**
bool unifiedTP::unifiedTP_handle(_u64 v_addr, enum::unifiedTP_access_type type){

    unifiedTP_access_count++;

    if(type == unifiedTP_type::u_TLB){
        TLB_access_count++;

        if(cache->check_cache_hit(v_addr)){ //TLB命中
            TLB_hit_count++;

            cache->do_cache_op(v_addr);

            return true;
        }
        else{
            return false;
        }
    }
    else{

        PTW_access_count++;

        int level;

        for(level = 1; level <= 3; ++level){
            Level_PTC_access_count[level - 1]++;
            if(cache->check_cache_hit(v_addr >> (9 * level)))
                break;    
        }

        //level 是命中的一级PTC，更新前1到level-1级
        
        for(int i = std::min(level, 3); i >= 1; --i){
            cache->do_cache_op(v_addr >> (9 * i));
        }

        cache->do_cache_op(v_addr); //更新TLB

        if(level <= 3){
            Level_PTC_hit_count[level - 1]++;
            return true;
        }

        return false;
    }
}
**/

bool unifiedTP::flush(_u64 tag){
    return cache->flush_cache(tag);
}


bool unifiedTP::flush(){
    cache->flush_cache();

    for(int i = 0; i < 3; ++i){
        this->Level_PTC_hit_count[i] = 0;
        this->Level_PTC_access_count[i] = 0;
    }

    this->PTW_access_count = 0; 

    this->TLB_access_count = 0;

    this->TLB_hit_count = 0;

    this->unifiedTP_access_count = 0;

    return true;
}

_u64 unifiedTP::get_op_count() const{
    return this->unifiedTP_access_count;
}

_u64 unifiedTP::get_TLB_op_count() const {
    return this->TLB_access_count;
}

_u64 unifiedTP::get_PTW_op_count() const {
    return this->PTW_access_count;
}

_u64 unifiedTP::get_overall_op_count() const{
    return this->cache->get_request_count();
}


double unifiedTP::get_hit_rate() const{
    return 1.0 * cache->get_hit_count() / unifiedTP_access_count;
}

double unifiedTP::get_TLB_hit_rate() const{
    return 1.0 * TLB_hit_count / TLB_access_count;
}

double unifiedTP::get_PTW_hit_rate() const{
    return 1.0 * (Level_PTC_hit_count[0] + Level_PTC_hit_count[1] + Level_PTC_hit_count[2]) / PTW_access_count;
}

double unifiedTP::get_overall_hit_rate() const{
    return cache->get_hit_rate();
}



_u64 unifiedTP::get_hit_count() const{
    return cache->get_hit_count();
}

_u64 unifiedTP::get_TLB_hit_count() const{
    return TLB_hit_count;
}

_u64 unifiedTP::get_PTW_hit_count() const{
    return Level_PTC_hit_count[0] + Level_PTC_hit_count[1] + Level_PTC_hit_count[2];
}

_u64 unifiedTP::get_overall_hit_count() const{
    return cache->get_hit_count();
}


_u64 unifiedTP::get_miss_count() const{
    return unifiedTP_access_count - cache->get_hit_count(); 
}

_u64 unifiedTP::get_TLB_miss_count() const{
    return TLB_access_count - TLB_hit_count;
}

_u64 unifiedTP::get_PTW_miss_count() const{
    return PTW_access_count - get_PTW_hit_count();
}

_u64 unifiedTP::get_overall_miss_count() const{
    return cache->get_miss_count();
}



double unifiedTP::get_L2_PTC_hit_rate() const{
    return 1.0 * Level_PTC_hit_count[0] / Level_PTC_access_count[0];
}

double unifiedTP::get_L3_PTC_hit_rate() const{
    return 1.0 * Level_PTC_hit_count[1] / Level_PTC_access_count[1];
}

double unifiedTP::get_L4_PTC_hit_rate() const{
    return 1.0 * Level_PTC_hit_count[2] / Level_PTC_access_count[2];
}

_u64 unifiedTP::get_L2_PTC_hit_count() const{
    return Level_PTC_hit_count[0];
}

_u64 unifiedTP::get_L3_PTC_hit_count() const{
    return Level_PTC_hit_count[1];
}

_u64 unifiedTP::get_L4_PTC_hit_count() const{
    return Level_PTC_hit_count[2];
}

_u64 unifiedTP::get_L2_PTC_miss_count() const{
    return Level_PTC_access_count[0] - Level_PTC_hit_count[0];
}

_u64 unifiedTP::get_L3_PTC_miss_count() const{
    return Level_PTC_access_count[1] - Level_PTC_hit_count[1];
}

_u64 unifiedTP::get_L4_PTC_miss_count() const{
    return Level_PTC_access_count[2] - Level_PTC_hit_count[2];
}