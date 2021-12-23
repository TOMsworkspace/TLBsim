//
// Created by tom on 2020/9/4.
//

#include "Set_Cache.h"
#include <ctime>


Set_Cache::Set_Cache(const _u64& cache_size,const _u32& cache_line_size, enum cache_replacement_algo algo,const _u32& way):
Cache(cache_size,cache_line_size),cache_ways(way),replacement_algo(algo)
{
    this->cache_set_num=cache_size/cache_line_size/way;
    this->cache_set_shifts=log2(cache_ways);
}



bool Set_Cache::do_cache_op(const _u64 & addr,const enum operation & op){

    this->tick_count++;
    this->cur_addr=addr;

    switch(op){
        case READ:
            this->cache_r_count++;
            break;
        case WRITE:
            this->cache_w_count++;
            break;
        default:
            break;
    }

    _u32 set=(addr>>cache_line_shifts)%cache_set_num;
    _u64 line=set*cache_ways;

    if(check_cache_hit(addr)){ //命中
        switch(op){
            case READ:
                this->read_cache(line);
                break;

            case WRITE:
                this->write_cache(line);
                break;

            default:
                break;
        }

        this->cache_hit_count++;

        return true;
    }
    else{ //未命中
        this->cache_miss_count++;

        switch(op){
            case READ: {
                _u64 victim_line = find_victim(addr);

                caches[victim_line].setVaild(true);
                caches[victim_line].setDirty(false);

                caches[victim_line].setTag(addr >> cache_line_shifts);

                caches[victim_line].setCount(this->tick_count);
            }
                break;

            case WRITE: {
                _u64 victim_line = find_victim(addr);

                caches[victim_line].setVaild(true);
                caches[victim_line].setDirty(true);

                caches[victim_line].setTag(addr >> cache_line_shifts);

                caches[victim_line].setCount(this->tick_count);

            }
                break;

            default:
                break;
        }

        return false;

    }

}

/*
      * 检查是否命中
      * @args:
      * addr: 要判断的内存地址
      * @return: true/false:命中/未命中
     */
bool Set_Cache::check_cache_hit(const _u64 & addr) const {
    _u32 set = (addr>>cache_line_shifts)%cache_set_num;
    _u64 line = set*cache_ways;

    _u64 tag = addr >> cache_line_shifts;

    _u64 bound = line + cache_ways;

    for(_u64 i = line;i < bound;i++) {

        if (caches[i].isVaild() && caches[i].getTag() == tag)  //数据有效，标签匹配，命中
            return true;

    }

    return false;
}

bool Set_Cache::flush_cache(_u64 tag){
    _u32 set = tag % cache_set_num;
    _u64 line = set*cache_ways;

    _u64 bound = line + cache_ways;

    for(_u64 i = line;i < bound;i++) {

        if (caches[i].isVaild() && caches[i].getTag() == tag){  //数据有效，标签匹配，命中
            caches[i].setDirty(false);
            caches[i].setVaild(false);
            return true;
        }

    }

    return false;
}


/**获取cache当前set中空余的line*/
_u64 Set_Cache::get_cache_free_line(const _u64 &addr) const {
    _u32 set = (addr>>cache_line_shifts) % cache_set_num;
    _u64 line = set * cache_ways;


    _u64 bound = line+cache_ways;
    for (_u64 i = line;i < bound;i++){
        if(!caches[i].isVaild())
            return i;
    }

    return ULONG_MAX;
}

/**
 * 根据替换算法查找替换行
 * addr: 请求的地址
 * return: 返回受害者line号 cache_line_num 无空行
 */

_u64 Set_Cache::find_victim(const _u64& addr) {

    _u64 line = get_cache_free_line(addr);

    if(line < cache_line_num){  //有空行
        this->cur_victimtag = INVALID_TAG;
        this->cache_capacity_miss_count++;
        return line;
    }

    _u32 set = (addr>>cache_line_shifts)%cache_set_num;
    line = set*cache_ways;
    int count = caches[line].getCount();

    _u64 bound  = line + cache_ways;

    switch (replacement_algo) {
        case CACHE_FIFO: {

            for(_u64 i = line;i < bound;i++){  //fifo 不更新 tick 选最小的
                if(caches[i].getCount() < count){
                    count = caches[i].getCount();
                    line = i;
                }
            }
            break;
        }


        case CACHE_LRU: {

            for(_u64 i = line;i < bound;i++){  //lru 更新 tick 选最小的
                if(caches[i].getCount() < count){
                    count = caches[i].getCount();
                    line = i;
                }
            }

            break;
        }


        case CACHE_RAND: {

            srand(time(nullptr));

            line=line+rand()%cache_ways;

            break;
        }

    }


    this->cache_conflict_miss_count++;
    this->cur_victimtag=caches[line].getTag();

    if(this->caches[line].isDirty()&&caches[line].isVaild())
        this->cache_to_next_mem++;
    return line;
}


/*
 * 更新读相关计数和状态*/

bool Set_Cache::read_cache(const _u64 & line)
{
    if(caches[line].isVaild()){
        this->cache_hit_r_count++;

        //更新计数

        switch (replacement_algo) {

            case cache_replacement_algo::CACHE_RAND:
            case cache_replacement_algo::CACHE_LRU:
                caches[line].setCount(this->tick_count);
                break;
            case cache_replacement_algo::CACHE_FIFO:
                break;

        }

        return true;
    }

    else{
        return false;
    }
}

/*
 * 更新写相关状态
 * @return */

bool Set_Cache::write_cache(const _u64 & line){
    if(caches[line].isVaild()){
        this->cache_hit_w_count++;
        if(!caches[line].isDirty())
            caches[line].setDirty(true);

        //更新计数

        switch (replacement_algo) {

            case cache_replacement_algo::CACHE_RAND:
            case cache_replacement_algo::CACHE_LRU:
                caches[line].setCount(this->tick_count);
                break;
            case cache_replacement_algo::CACHE_FIFO:
                break;

        }

        return true;
    }

    else
    {
        return false;
    }
}

std::ostream & operator << (std::ostream & os, const Set_Cache & set_cache)
{
    os << "Set Cache Result:\n" << std::endl;

    os << std::setw(20) << "Ways of Set: " << std::setw(20) << set_cache.cache_ways
       << std::setw(40) << "replacement:(0:fifo,1:lru,2:rand) " << std::setw(20) << set_cache.replacement_algo << std::endl;

    return set_cache.print(os);

}
