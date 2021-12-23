//
// Created by tom on 2020/9/4.
//

#ifndef CACHESIM_MASTER_DIRECT_CACHE_H
#define CACHESIM_MASTER_DIRECT_CACHE_H

#include "Base_Cache.h"

class Direct_Cache : public Cache
{
public:

    Direct_Cache():Cache() {} //default

    Direct_Cache(const _u64& cache_size,const _u32& cache_line_size):Cache(cache_size,cache_line_size) {}

    ~Direct_Cache() {}


    /**
     * @brief 对一个指令进行读写处理
     * 
     * @param addr 地址
     * @param op 操作类型: read / write
     * @return true :命中
     * @return false 未命中
     */
    virtual bool do_cache_op(const _u64 & addr,const enum operation& op = operation::READ) override;

    /*
      * 检查是否命中
      * @args:
      * addr: 要判断的内存地址
      * @return:
     */
    virtual bool check_cache_hit(const _u64 & addr) const;

    /**
     * 根据替换算法查找替换行
     * addr: 请求的地址
     * return: 返回受害者line号
     */

    virtual _u64 find_victim(const _u64& addr) override;

    
    virtual bool flush_cache(_u64 tag) override;

    // std::ostream & print (std::ostream & os) const;

    friend std::ostream & operator << (std::ostream & os,const Direct_Cache& cache);


protected:



    virtual bool read_cache(const _u64 & line) override;

    virtual bool write_cache(const _u64 & line) override;

    virtual int lock_cache_line(_u64 addr) override {return addr;}
    virtual int unlock_cache_line(_u64 addr) override{return addr;}



};

#endif //CACHESIM_MASTER_DIRECT_CACHE_H
