# TLBSim

The project simulation a tradictional MMU with unifiedTP MMU.
Tradictional MMU: with two-level TLB and a level PTC.
unified MMU : 
    config 1: one level TLB and one level unifiedTP.  
    config 2: one level TLB and two level unifiedTP.


**Build:**

&emsp;The project is constructed by CMAKE.

&emsp;Build it by run:
```bash
    cmake -B build && cmake --build build
```

**Run:**

&emsp;Two exe project. The Experiment1 exe compares the tradictional MMU with unifiedTP config 1. The Experiment2 exe compares the tradictional MMU with unifiedTP config 2.

```bash
    "path to exe"/Experiment2.exe benchname1 benchname2 ...
```

**bencn:**
&ensp;Config your .txt trace file in bench.h.

**Output:**
&emsp;output file in the result subdirectory.

| :-----:  | :-----:             | :-----:         |  
|          |hit count            |miss count       |  
|L1 TLB    |L1_TLB hit count     |L1_TLB miss count|  
|L2 TLB    |L2_TLB hit count     |L2_TLB miss count|  
|L2 PTC    |L2_PTC hit count     |L3_PTC miss count|  
|L3 PTC    |L3_PTC hit count     |L3_PTC miss count|  
|L4 PTC    |L4_PTC hit count     |L3_PTC miss count|  

