//
// Created by tom on 2020/9/4.
//

#include "Direct_Cache.h"


/**对一个指令进行读写处理*/
bool Direct_Cache::do_cache_op(const _u64 & addr,const enum operation & op )
{

    //std::cout << "do cache " << std::endl;
    this->tick_count++;
    this->cur_addr=addr;

    switch(op){
        case READ:
            this->cache_r_count++;
            break;
        case WRITE:
            this->cache_w_count++;
            break;
    }

    _u64 line=(addr>>cache_line_shifts)%cache_line_num;

    if(check_cache_hit(addr)){ //命中
        switch(op){
            case READ:
                this->read_cache(line);

                break;

            case WRITE:
                this->write_cache(line);

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
                break;
            }


            case WRITE: {

                _u64 victim_line = find_victim(addr);

                caches[victim_line].setVaild(true);

                caches[victim_line].setDirty(true);

                caches[victim_line].setTag(addr >> cache_line_shifts);

                caches[victim_line].setCount(this->tick_count);

                break;
            }

        }

        return false;


    }

    //std::cout << "do cache end" << std::endl;
}

/*
  * 检查是否命中
  * @args:
  * addr: 要判断的内存地址
  * @return:
 */
bool Direct_Cache::check_cache_hit(const _u64 & addr) const
{
    _u64 line=(addr>>cache_line_shifts)%cache_line_num;

    _u64 tag=addr >> cache_line_shifts;

    if(caches[line].isVaild()&&caches[line].getTag()==tag) { //数据有效，标签匹配，命中
        return true;
    }

    return false;
}


bool Direct_Cache::flush_cache(_u64 tag){
    _u64 line=tag % cache_line_num;

    if(caches[line].isVaild()&&caches[line].getTag()==tag) { //数据有效，标签匹配，命中
        caches[line].setVaild(false);
        caches[line].setDirty(false);

        return true;
    }

    return false;
}

/*
 * 更新读相关计数和状态*/

bool Direct_Cache::read_cache(const _u64 & line)
{
    if(caches[line].isVaild()){
        this->cache_hit_r_count++;

        caches[line].setCount(this->tick_count);
        return true;
    }

    else{
        return false;
    }
}

/*
 * 更新写相关状态
 * @return */

bool Direct_Cache::write_cache(const _u64 & line) {
    if(caches[line].isVaild()){
        this->cache_hit_w_count++;
        if(!caches[line].isDirty())
            caches[line].setDirty(true);

        caches[line].setCount(this->tick_count);

        return true;
    }

    else
    {
        return false;
    }
}

/**
 * 根据替换算法查找替换行
 * addr: 请求的地址
 * return: 返回受害者line号
 */

_u64 Direct_Cache::find_victim(const _u64& addr)
{

    _u64 line=(addr >> cache_line_shifts)%cache_line_num;

    _u64 tag=(addr>>cache_line_shifts);
    if(!caches[line].isVaild()) {
        this->cache_capacity_miss_count++;
        this->cur_victimtag = INVALID_TAG;
    }
    else if(caches[line].getTag()!=tag){
        this->cache_conflict_miss_count++;
        this->cur_victimtag=caches[line].getTag();
    }

    if(caches[line].isVaild()&&caches[line].isDirty())
        this->cache_to_next_mem++;

    return line;

}


std::ostream & operator << (std::ostream & os,const Direct_Cache& cache){
    os << "Direct Cache Result:\n" << std::endl;

    return cache.print(os);

}


