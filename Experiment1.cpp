/**
 * @file Experiment1.cpp
 * @author TOMsworkspace (2683322180@qq.com)
 * @brief 
 * @version 1.0
 * @date 2021-12-13
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <iostream>
#include <fstream>
//#include <intrin.h>
#include <string>

using namespace std;


#include "TLB.h"
#include "UnifiedTP.h"
#include "PTWalker.h"

#include "bench.h"
#include "config.h"
#include "Perform.h"

//
const _u32 L1_TLB_ENTRY = 64;
const _u32 L1_TLB_WAY = 4;

const _u32 L2_TLB_ENTRY = 1536;
const _u32 L2_TLB_WAY = 4;

const _u32 L3_PTC_ENTRY = 24;
const _u32 L3_PTC_WAY = 24;

const _u32 L2_UNIFIEDTP_ENTRY = 1536;
const _u32 L2_UNIFIEDTP_WAY = 4;

/**
 * @brief 架构1分析 l1_tlb, l2 tlb, PTC
 * 
 * @param benchname 
 * @param L1_TLB 
 * @param L2_TLB 
 * @param PTC 
 * @param outfile 
 */
void load_trace_arch1(const string & benchname, TLB & L1_TLB, TLB & L2_TLB, PTW & L3_PTC, ofstream& outfile);

/**
 * @brief 架构2分析 l1_tlb, unifiedTP
 * 
 * @param benchname 
 * @param L1_TLB 
 * @param UTP 
 * @param outfile 
 */
void load_trace_arch2(const string & benchname, TLB & L1_TLB, unifiedTP & L2_UTP, ofstream & outfile);


int main(int argc, char* argv[]) {

    if(argc < 2)
    {
        std::cerr <<  "Without input bench name!!" << endl;
        return 1;
    }

    TLB L1_TLB = TLB(L1_TLB_ENTRY, cache_replacement_algo::CACHE_LRU, L1_TLB_WAY);
    TLB L2_TLB = TLB(L2_TLB_ENTRY, cache_replacement_algo::CACHE_LRU, L2_TLB_WAY);
    PTW L3_PTC = PTW(L3_PTC_ENTRY, cache_replacement_algo::CACHE_LRU, L3_PTC_ENTRY);
    unifiedTP L2_UTP = unifiedTP(L2_UNIFIEDTP_ENTRY, cache_replacement_algo::CACHE_LRU, L2_UNIFIEDTP_WAY);


    for(int i = 1; i < argc; ++i){
        
        string benchname = string(argv[i]); // bench 

        if(!BENCHS.count(benchname)){
            std::cerr <<  "Input bench name " << benchname << " is not exist!!" << endl;
            return 1;
        }

        ofstream outFile;
        outFile.open("./result/" + benchname + "_Experiment1_results.txt", ios::out | ios::trunc); //输出结果

        load_trace_arch1(BENCHS.at(benchname), L1_TLB, L2_TLB, L3_PTC, outFile);

        L1_TLB.flush();
        L2_TLB.flush();
        L3_PTC.flush();

        load_trace_arch2(BENCHS.at(benchname), L1_TLB, L2_UTP, outFile);

        L1_TLB.flush();
        L2_UTP.flush();

        outFile.flush();
        outFile.close();
    }

    return 0;
}


void load_trace_arch1(const string &benchname, TLB & L1_TLB, TLB & L2_TLB, PTW & L3_PTC, ofstream& outfile) {

    std::ifstream inf(benchname, std::ios_base::in);

    if (!inf.is_open()) {
        cout << "load_trace " << benchname <<  " failed\n" <<endl;
        return;
    }

    _u64 cycle;
    _u64 v_addr;

    int op;

    cout << "Load trace:   " << benchname  << endl;

    while (inf.is_open() && !inf.eof() && !inf.bad() && !inf.fail()) {

        //inf >> cycle >> std::hex >> v_addr >> std::dec;
        inf >> cycle >> op >> std::hex >> v_addr >> std::dec;

        if(L1_TLB.TLB_handle(v_addr)){ //l1 tlb hit
            continue;
        }
        else{
            if(L2_TLB.TLB_handle(v_addr)){ //l2 tlb hit
                continue;
            }
            else{
                L3_PTC.PTW_handle(v_addr);
            }
        }
    }

    outfile << "Architectual 1 result!!!" <<  endl;
    outfile << "            hit count     " << "  miss count" << endl;
    outfile << "L1 TLB   " << L1_TLB.get_hit_count() << "     " << L1_TLB.get_miss_count() << endl;
    outfile << "L2 TLB   " << L2_TLB.get_hit_count() << "     " << L2_TLB.get_miss_count() << endl;
    outfile << "L2 PTC   " << L3_PTC.get_L2_PTC_hit_count() << "     " << L3_PTC.get_L2_PTC_miss_count() << endl;
    outfile << "L3 PTC   " << L3_PTC.get_L3_PTC_hit_count() << "     " << L3_PTC.get_L3_PTC_miss_count() << endl;
    outfile << "L4 PTC   " << L3_PTC.get_L4_PTC_hit_count() << "     " << L3_PTC.get_L4_PTC_miss_count() << endl;

    outfile << endl;

    inf.close();
}

void load_trace_arch2(const string & benchname, TLB & L1_TLB, unifiedTP & L2_UTP, ofstream& outfile) {
    
    std::ifstream inf(benchname,std::ios_base::in);

    if (!inf.is_open()) {
        cout << "load_trace " << benchname <<  " failed\n" <<endl;
        return;
    }

    _u64 cycle;
    _u64 v_addr;

    int op;

    cout << "Load trace:   " << benchname  << endl;

    while (inf.is_open() && !inf.eof() && !inf.bad() && !inf.fail()) {

        //inf >> cycle >> std::hex >> v_addr >> std::dec;

        inf >> cycle >> op >> std::hex >> v_addr >> std::dec;

        if(L1_TLB.TLB_handle(v_addr)){ //l1 tlb hit
            continue;
        }
        else{
            enum unifiedTP_access_result_type tlb_result = L2_UTP.unifiedTP_check_hit(v_addr, u_TLB);
            if(tlb_result == u_TLB_MISS){
                enum unifiedTP_access_result_type ptc_result = L2_UTP.unifiedTP_check_hit(v_addr, u_PTC);
                switch (ptc_result)
                {
                    case u_L2PTC_HIT:
                        /* code */
                        L2_UTP.unifiedTP_update(v_addr, u_L2PTC);
                        break;
                    case u_L3PTC_HIT:
                        /* code */
                        L2_UTP.unifiedTP_update(v_addr, u_L3PTC);
                        L2_UTP.unifiedTP_update(v_addr, u_L2PTC);
                        break;
                    case u_L4PTC_HIT:
                        /* code */
                        L2_UTP.unifiedTP_update(v_addr, u_L4PTC);
                        L2_UTP.unifiedTP_update(v_addr, u_L3PTC);
                        L2_UTP.unifiedTP_update(v_addr, u_L2PTC);
                        break;
                    
                    case u_L4PTC_MISS:
                        L2_UTP.unifiedTP_update(v_addr, u_L4PTC);
                        L2_UTP.unifiedTP_update(v_addr, u_L3PTC);
                        L2_UTP.unifiedTP_update(v_addr, u_L2PTC);
                        break;
                }
            }

            L2_UTP.unifiedTP_update(v_addr, u_TLB);
        }
    }

    outfile << "Architectual 2 result!!!" <<  endl;
    outfile << "                            hit count     " << "miss count" << endl;
    outfile << "L1 TLB                " << L1_TLB.get_hit_count() << "     " << L1_TLB.get_miss_count() << endl;
    outfile << "L2 unifiedTP TLB      " << L2_UTP.get_TLB_hit_count() << "     " << L2_UTP.get_TLB_miss_count() << endl;
    outfile << "L2 unifiedTP L2 PTC   " << L2_UTP.get_L2_PTC_hit_count() << "     " << L2_UTP.get_L2_PTC_miss_count() << endl;
    outfile << "L2 unifiedTP L3 PTC   " << L2_UTP.get_L3_PTC_hit_count() << "     " << L2_UTP.get_L3_PTC_miss_count() << endl;
    outfile << "L2 unifiedTP L4 PTC   " << L2_UTP.get_L4_PTC_hit_count() << "     " << L2_UTP.get_L4_PTC_miss_count() << endl;

    outfile << endl;
    outfile.flush();

    inf.close();
}