#ifndef CACHESIM_MASTER_BASE_CACHE_H
#define CACHESIM_MASTER_BASE_CACHE_H

//
// Created by tom on 2020/9/3.
//

#include "CacheLine.h"
#include <cmath>
#include <iostream>
#include <iomanip>
#include <climits>


enum cacheAction{Hit='H',Miss='M'};

enum operation{READ='r',WRITE='w'};

//char OPERATION_READ = 'r';
//char OPERATION_WRITE = 'w';


class Cache {

protected:

    /**cache的总大小，单位byte*/
    _u64 cache_size;
    /**cache line(Cache block)cache块的大小*/
    _u32 cache_line_size;
    /**总的行数*/
    _u64 cache_line_num;

    /**2的多少次方是line的长度，用于匹配地址*/
    _u32 cache_line_shifts;

    /**真正的cache地址列。指针数组*/
    CacheLine *caches;

    /**cache缓冲区,由于并没有数据*/
//    _u8 *cache_buf[MAXLEVEL];


    /**访问计数器*/
    _u64 tick_count;

    /**读写计数*/
    _u64 cache_r_count, cache_w_count;

    /*读写命中的计数*/
    _u64 cache_hit_r_count, cache_hit_w_count;
    /**实际写内存的计数，cache --> memory */
    _u64 cache_to_next_mem;
    /**cache hit和miss的计数*/
    _u64 cache_hit_count, cache_miss_count;

    /**容量/冲突  未命中*/
    _u64 cache_capacity_miss_count;
    _u64 cache_conflict_miss_count;


    //runtime_msg;

    _u64 cur_addr;
    _u64 cur_victimtag;


public:

    //Cache(){init();}
    Cache(const _u64 & cache_size=128*1024,const _u32 & cache_line_size=64) //default 128KB-64B
    :cache_size(cache_size),cache_line_size(cache_line_size),tick_count(0),cache_r_count(0),cache_w_count(0),cache_hit_r_count(0),cache_hit_w_count(0),
    cache_to_next_mem(0),cache_hit_count(0),cache_miss_count(0),cache_capacity_miss_count(0),cache_conflict_miss_count(0)
    {
        cache_line_num=cache_size/cache_line_size;
        cache_line_shifts=log2(cache_line_size);

        caches = new CacheLine[cache_line_num];

        //for(_u32 i=0;i<cache_line_num;i++){
        //    caches[i] = new CacheLine();
        //}

    }
    
    virtual ~Cache() {
        //std::cout << "free mem" << std::endl;
        delete [] caches;
    }

    //计算总命中率
    double get_hit_rate() const{
        return 1.0 * cache_hit_count / (cache_miss_count + cache_hit_count);
    }

    //计算总未命中率
    double get_miss_rate() const {
        return 1.0 * cache_miss_count/ (cache_miss_count+ cache_hit_count);
    }

    //计算读命中率
    double get_hit_r_rate() const{
        return 1.0 * cache_hit_r_count / (cache_r_count);
    }

    //计算写命中率
    double get_hit_w_rate() const {
        return 1.0 * cache_hit_w_count/ cache_w_count;
    }

    _u64 get_r_count() const {
        return cache_r_count;
    }

    _u64 get_w_count() const {
        return cache_w_count;
    }

    _u64 get_request_count() const {
        return tick_count;
    }

    _u64 get_hit_count() const {
        return cache_hit_count;
    }

    _u64 get_miss_count() const{
        return cache_miss_count;
    }

    _u32 get_cache_size() const {
        return cache_size >> 27;
    }

    _u32 get_cacheline_size() const {
        return cache_line_size >> 6;
    }

    
    /**对一个指令进行读写处理*/
    
    virtual bool do_cache_op(const _u64& addr,const enum operation & op = operation::READ) = 0;
    
    /*
     * 检查是否命中
     * @args:
     * addr: 要判断的内存地址
     * @return: 是否命中
     * do: 更新命中、未命中计数、缓存行状态更新
    */
    virtual bool check_cache_hit(const _u64 & addr) const = 0;


    /**
     * 根据替换算法查找替换行
     * addr: 请求的地址
     * return: 返回受害者line号
     */

    virtual _u64 find_victim(const _u64 & addr) =0;

    virtual _u64 get_victim_tag() const {
        return cur_victimtag;
    }

    virtual _u64 get_cur_addr() const {
        return cur_addr;
    }

    /**lock a cache line*/
    virtual int lock_cache_line(_u64 addr)=0;
    /**unlock a cache line*/
    virtual int unlock_cache_line(_u64 addr)=0;
    /**@return 返回miss率*/

    /**
    * @brief flush cache(set every cacheline invalid and is not dirty)
    * 
    */

    virtual bool flush_cache(){
        for(int idx = 0; idx < cache_line_num; ++idx){
            caches[idx].setVaild(false);
            caches[idx].setDirty(false);
        }

        
        /**访问计数器清零*/
        this->tick_count = 0ll;

        this->cache_r_count = 0ll;
        this->cache_w_count = 0ll;

        this->cache_hit_r_count = 0ll;
        this->cache_hit_w_count = 0ll;

        this->cache_to_next_mem = 0ll;

        this->cache_hit_count = 0ll;
        this->cache_miss_count = 0ll;

        this->cache_capacity_miss_count = 0ll;
        this->cache_conflict_miss_count = 0ll;

        this->cur_addr = 0ull;
        this->cur_victimtag = 0ull;

        std::cout << "cache flush!!!" << std::endl;

        return true;
    }


    virtual std::ostream& print (std::ostream & os) const{

        os << std::dec ;

        os << std::setw(20) << "Cache size:(B)"<< std::setw(20) << cache_size
           << std::setw(20) <<"line size:(B)" << std::setw(20) << cache_line_size << std::endl;

        os << std::setw(20) << "operator number:" << std::setw(20) <<tick_count
           << std::setw(20) << "Read: " << std::setw(20) << cache_r_count
           << std::setw(20)  << "Write: " << cache_w_count << std::endl;

        os << std::setw(20) << "Read Hit:" << std::setw(20) << cache_hit_r_count
           << std::setw(20) <<"Write Hit: " << std::setw(20) << cache_hit_w_count
           << std::setw(20)<<"To next mem: "<< cache_to_next_mem << std::endl;

        os << std::setw(20) << "Hit:" << std::setw(20) << cache_hit_count
           << std::setw(20) << "Miss: " << std::setw(20) << cache_miss_count << std::endl;
        os << std::setw(20) << "Capacity miss:" << std::setw(20) << cache_capacity_miss_count
           << std::setw(20) << "Conflict Miss: " << std::setw(20) << cache_conflict_miss_count << std::endl;

        os << std::setw(20) << "Hit Rate:" << std::setw(20) << 100*get_hit_rate() << "%" << std::endl;

        return os;
    }

    friend std::ostream & operator << (std::ostream& os ,const Cache& cache){

        return cache.print(os);
    }

    void show () const {
        for(auto i=0;i<cache_line_num;i++){
            std::cout << caches[i] << std::endl;
        }
    }

protected:
    /*
 * 更新读相关计数和状态*/

    virtual bool read_cache(const _u64 & line)
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

    virtual bool write_cache(const _u64 & line){
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
};

#endif //CACHESIM_MASTER_BASE_CACHE_H
