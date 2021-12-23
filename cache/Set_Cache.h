#ifndef CACHESIM_MASTER_SET_CACHE_H
#define CACHESIM_MASTER_SET_CACHE_H

//
// Created by tom on 2020/9/3.
//

#include "Base_Cache.h"

enum cache_replacement_algo {
    CACHE_FIFO,
    CACHE_LRU,
    CACHE_RAND
};


class Set_Cache : public Cache
{
private:
    /**每个set有多少way*/
    _u32 cache_ways;
    /**整个cache有多少组*/
    _u32 cache_set_num;

    /**2的多少次方是set的数量，用于匹配地址时，进行位移比较*/
    _u32 cache_set_shifts;

    /**缓存替换算法*/
    int replacement_algo;



public:

    Set_Cache():Cache(),cache_ways(4),cache_set_num(512),cache_set_shifts(2),replacement_algo(cache_replacement_algo::CACHE_LRU) {} //default LRU

    Set_Cache(const _u64 & cache_size,const _u32 & cache_line_size, enum cache_replacement_algo algo,const _u32& way);

    ~Set_Cache() {}
    
    /**
     * @brief 对一个指令进行读写处理
     * 
     * @param addr 地址
     * @param op 操作类型: read / write
     * @return true :命中
     * @return false 未命中
     */
    virtual bool do_cache_op(const _u64 & addr,const enum operation & op = operation::READ) override;

    /*
      * 检查是否命中
      * @args:
      * addr: 要判断的内存地址
      * @return:
     */
    virtual bool check_cache_hit(const _u64 & addr) const override;

    virtual bool flush_cache(_u64 addr) override;


    /**获取cache当前set中空余的line*/
    virtual _u64 get_cache_free_line(const _u64 &addr) const;

    /**
     * 根据替换算法查找替换行
     * addr: 请求的地址
     * return: 返回受害者line号
     */

    virtual _u64 find_victim(const _u64 & addr) override;

    //  template<_u64 n>
  //  friend _u64 BloomFilterPredictor<n>::get_victim_tag(const _u64 & addr,const Set_Cache& cache);


    friend std::ostream & operator << (std::ostream & os, const Set_Cache & set_cache);

protected:

    virtual bool read_cache(const _u64 & line) override;

    virtual bool write_cache(const _u64 & line) override;

    /**lock a cache line*/
    virtual int lock_cache_line(_u64 addr) override {return addr;}
    /**unlock a cache line*/
    virtual int unlock_cache_line(_u64 addr) override {return addr;}
    /**@return 返回miss率 */


};

#endif //CACHESIM_MASTER_SET_CACHE_H
