#pragma once
#include <ntifs.h>

typedef union _CR4 {
    UINT64 Value;
    struct
    {
        UINT64 VME : 1;
        UINT64 PVI : 1;
        UINT64 TSD : 1;
        UINT64 DE : 1;
        UINT64 PSE : 1;
        UINT64 PAE : 1;
        UINT64 MCE : 1;
        UINT64 PGE : 1;
        UINT64 PCE : 1;
        UINT64 OSFXSR : 1;
        UINT64 OSXMMEXCPT : 1;
        UINT64 UMIP : 1;
        UINT64 LA57 : 1;
        UINT64 VMXE : 1;
        UINT64 SMXE : 1;
        UINT64 Reserved1 : 1;
        UINT64 FSGSBASE : 1;
        UINT64 PCIDE : 1;
        UINT64 OSXSAVE : 1;
        UINT64 KL : 1;
        UINT64 SMEP : 1;
        UINT64 SMAP : 1;
        UINT64 PKE : 1;
        UINT64 CET : 1;
        UINT64 PKS : 1;
        UINT64 Reserved2 : 39;
    } Fields;
} CR4, * PCR4;
