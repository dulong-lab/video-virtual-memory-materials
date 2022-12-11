#include "common.h"
#include "x64.h"
#include "win10.h"

VOID
CanonicalizeVa1(VA* Va)
{
    if (Va->Fields.VPN1 >= 256)
        Va->Fields.SEXT = 0xFFFF;
    else
        Va->Fields.SEXT = 0;
}

VOID
CanonicalizeVa2(VA* Va)
{
    Va->Value = (INT64)(Va->Value << 16) >> 16;
}

NTSTATUS
AllocateL4pte(PL4PTE* L4pte, PVOID* ptr)
{
    CR3 Cr3 = { .Value = __readcr3() };
    PA L1ptPa = (PA){ .Fields4KB.PPN = Cr3.Fields.PPN };
    PL1PTE L1pt = MmGetVirtualForPhysical(L1ptPa.AsLargeInteger);
    for (size_t n1 = 0; n1 < 512; n1++)
    {
        if (!L1pt[n1].Fields.P) continue;

        PA L2ptPa = { .Fields4KB.PPN = L1pt[n1].Fields.PPN };
        PL2PTE L2pt = MmGetVirtualForPhysical(L2ptPa.AsLargeInteger);
        for (size_t n2 = 0; n2 < 512; n2++)
        {
            if (!L2pt[n2].Fields.P || L2pt[n2].Fields1GB.PS) continue;

            PA L3ptPa = { .Fields4KB.PPN = L2pt[n2].Fields.PPN };
            PL3PTE L3pt = MmGetVirtualForPhysical(L3ptPa.AsLargeInteger);
            for (size_t n3 = 0; n3 < 512; n3++)
            {
                if (!L3pt[n3].Fields.P || L3pt[n3].Fields2MB.PS) continue;

                PA L4ptPa = { .Fields4KB.PPN = L3pt[n3].Fields.PPN };
                PL4PTE L4pt = MmGetVirtualForPhysical(L4ptPa.AsLargeInteger);
                for (size_t n4 = 0; n4 < 512; n4++)
                {
                    if (L4pt[n4].Fields.P) continue;

                    L4pt[n4] = (L4PTE){ .Fields = { .P = 1, .R_W = 1 } };
                    *L4pte = &L4pt[n4];
                    VA Va = { .Fields4KB = { .VPN1 = n1, .VPN2 = n2, .VPN3 = n3, .VPN4 = n4 } };
                    CanonicalizeVa(&Va);
                    *ptr = Va.AsPtr;
                    return STATUS_SUCCESS;
                }
            }
        }
    }

    return STATUS_NOT_FOUND;
}

VOID
MapPage4KB(UINT64 PPN, PL4PTE L4pte, PVOID Ptr)
{
    L4pte->Fields.PPN = PPN;
    __invlpg(Ptr);
}

VOID
FreeL4pte(PL4PTE L4pte, PVOID Ptr)
{
    L4pte->Value = 0;
    __invlpg(Ptr);
}
