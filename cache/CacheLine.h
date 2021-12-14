//
// Created by tom on 2020/9/3.
//

#ifndef CACHESIM_MASTER_CACHELINE_H
#define CACHESIM_MASTER_CACHELINE_H

#include <iostream>
#include <iomanip>
#include "config.h"

class CacheLine
{
private:
    _u64 tag;

    union {
        _u64 count;
        _u64 lru_count;
        _u64 fifo_count;
    };

    bool valid;
    bool dirty;


public:
    CacheLine():tag(0x00),valid(false),dirty(false),count(0){
        //std::cout << "dafault instructor !" << std::endl;
    }

    CacheLine(const _u64 & tag,const bool & vaild= false,const bool & dirty= false ,const int & count =0):tag(tag),valid(vaild),dirty(dirty),count(0){}

    ~CacheLine(){
        //std::cout << "dafault constructor !" << std::endl;
    }

    _u64 getTag() const{
        return tag;
    }

    void setTag( const _u64 & tag){
        this->tag=tag;
    }

    bool isVaild() const{
        return valid;
    }

    void setVaild(const bool& vaild){
        this->valid=vaild;
    }

    bool isDirty() const{
        return dirty;
    }

    void setDirty(const bool& dirty){
        this->dirty=dirty;
    }

     _u64 getCount( ) const
    {
        return count;
    }

    void setCount(const _u64& tick){
        this->count=tick;
    }

    friend std::ostream& operator << (std::ostream& os,const CacheLine& cacheline)
    {
        os << "Tag: " << std::hex << std::setw(10) << cacheline.getTag() << std::dec << "   " << (cacheline.isVaild() ? "vaild" : "invaild") << "  " << (cacheline.isDirty() ?
        "dirty" : "no dirty") << std::endl;
        return os;
    }


    CacheLine(CacheLine *pLine) {
        this->tag = pLine->tag;
        this->dirty = pLine->dirty;
        this->valid = valid;
        this->count = pLine->count;
    }
};

#endif //CACHESIM_MASTER_CACHELINE_H
