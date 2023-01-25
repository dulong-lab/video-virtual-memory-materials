#include <stdio.h>
#include "x64.h"

int main() {
    VA Va = { .Fields4KB = { .VPN1=493, .VPN2=493, .VPN3=493, .VPN4=493, .VPO=493*8 } };
    CanonicalizeVa(&Va);

    if (!Va.Value) return 0; // make vs happy

    __try
    {
        L1PTE SelfMappingEntry = *(PL1PTE)Va.Value;
        printf("SelfMappingEntry Value=0x%016I64X U/S=%I64d R/W=%I64d\n"
            , SelfMappingEntry.Value
            , SelfMappingEntry.Fields.U_S
            , SelfMappingEntry.Fields.R_W
        );
    }
    __except (EXCEPTION_EXECUTE_HANDLER)
    {
        printf("NO\n");
    }

    return 0;
}
