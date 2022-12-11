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
AllocateL4pte(PL4PTE* L4pte, PVOID* Ptr)
{
    CR3 Cr3 = { .Value = __readcr3() };
    PA L1ptPa = { .Fields4KB.PPN = Cr3.Fields.PPN };
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
                    *Ptr = Va.AsPtr;
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

NTSTATUS
DumpPageTable(CR3 Cr3)
{
    NTSTATUS Status = STATUS_SUCCESS;
    PL4PTE SrcL4pte = NULL;
    PVOID Src = NULL;
    L1PTE L1pt[512] = { 0 };
    L2PTE L2pt[512] = { 0 };
    L3PTE L3pt[512] = { 0 };
    L4PTE L4pt[512] = { 0 };

    Status = AllocateL4pte(&SrcL4pte, &Src);
    EXIT_IF_FAILED(Status, { DbgPrint("AllocateL1pte failed. Status=%X\n", Status); });

    PA L1ptPa = (PA){ .Fields4KB.PPN = Cr3.Fields.PPN };
    DbgPrint("data={title:'UserDirectoryTableBase',items:[['Value','0x%016I64X'],['PPN','0x%016I64X'],['L1pt.Pa','0x%016I64X',"
        , Cr3.Value
        , Cr3.Fields.PPN
        , L1ptPa.Value
    );

    MapPage4KB(L1ptPa.Fields4KB.PPN, SrcL4pte, Src);
    memcpy(L1pt, Src, 4096);
    DbgPrint("{title:'L1pt',items:[");
    for (int n1 = 0; n1 < 512; n1++)
    {
        if (!L1pt[n1].Fields.P) continue;
        PA L2ptPa = { .Fields4KB.PPN = L1pt[n1].Fields.PPN };
        DbgPrint("['%03d', '"
            "  Value=0x%016I64X\\n"
            "    U/S=%I64d R/W=%I64d XD=%I64d\\n"
            "    PPN=0x%016I64X\\n"
            "L2pt.Pa=0x%016I64X',"
            , n1
            , L1pt[n1].Value
            , L1pt[n1].Fields.U_S
            , L1pt[n1].Fields.R_W
            , L1pt[n1].Fields.XD
            , L1pt[n1].Fields.PPN
            , L2ptPa.Value
        );
        if (L2ptPa.Value == L1ptPa.Value)
        {
            DbgPrint("],");
            continue;
        }
        MapPage4KB(L2ptPa.Fields4KB.PPN, SrcL4pte, Src);
        memcpy(L2pt, Src, 4096);
        DbgPrint("{title:'L1pt[%03d].L2pt',items:[", n1);
        for (int n2 = 0; n2 < 512; n2++)
        {
            if (!L2pt[n2].Fields.P) continue;
            if (L2pt[n2].Fields1GB.PS) {
                PA Pa = { .Fields1GB.PPN = L2pt[n2].Fields1GB.PPN };
                VA Va = { .Fields1GB = { .VPN1 = n1, .VPN2 = n2 } };
                CanonicalizeVa(&Va);
                DbgPrint("['%03d','"
                    "  Value=0x%016I64X\\n"
                    "    U/S=%I64d R/W=%I64d PS=%I64d G=%I64d XD=%I64d\\n"
                    "    PPN=0x%016I64X\\n"
                    "     Pa=0x%016I64X\\n"
                    "     Va=0x%016I64X'],"
                    , n2
                    , L2pt[n2].Value
                    , L2pt[n2].Fields1GB.U_S
                    , L2pt[n2].Fields1GB.R_W
                    , L2pt[n2].Fields1GB.PS
                    , L2pt[n2].Fields1GB.G
                    , L2pt[n2].Fields1GB.XD
                    , L2pt[n2].Fields1GB.PPN
                    , Pa.Value
                    , Va.Value
                );
                continue;
            }
            PA L3ptPa = { .Fields4KB.PPN = L2pt[n2].Fields.PPN };
            DbgPrint("['%03d','"
                "  Value=0x%016I64X\\n"
                "    U/S=%I64d R/W=%I64d PS=%I64d XD=%I64d\\n"
                "    PPN=0x%016I64X\\n"
                "L3pt.Pa=0x%016I64X',"
                , n2
                , L2pt[n2].Value
                , L2pt[n2].Fields.U_S
                , L2pt[n2].Fields.R_W
                , L2pt[n2].Fields.PS
                , L2pt[n2].Fields.XD
                , L2pt[n2].Fields.PPN
                , L3ptPa.Value
            );
            MapPage4KB(L3ptPa.Fields4KB.PPN, SrcL4pte, Src);
            memcpy(L3pt, Src, 4096);
            DbgPrint("{title:'L1pt[%03d].L2pt[%03d].L3pt',items:[", n1, n2);
            for (int n3 = 0; n3 < 512; n3++)
            {
                if (!L3pt[n3].Fields.P) continue;
                if (L3pt[n3].Fields2MB.PS)
                {
                    PA Pa = { .Fields2MB.PPN = L3pt[n3].Fields2MB.PPN };
                    VA Va = { .Fields2MB = {.VPN1 = n1, .VPN2 = n2, .VPN3 = n3 } };
                    CanonicalizeVa(&Va);
                    DbgPrint("['%03d','"
                        "  Value=0x%016I64X\\n"
                        "    U/S=%I64d R/W=%I64d PS=%I64d G=%I64d XD=%I64d\\n"
                        "    PPN=0x%016I64X\\n"
                        "     Pa=0x%016I64X\\n"
                        "     Va=0x%016I64X'],"
                        , n3
                        , L3pt[n3].Value
                        , L3pt[n3].Fields2MB.U_S
                        , L3pt[n3].Fields2MB.R_W
                        , L3pt[n3].Fields2MB.PS
                        , L3pt[n3].Fields2MB.G
                        , L3pt[n3].Fields2MB.XD
                        , L3pt[n3].Fields2MB.PPN
                        , Pa.Value
                        , Va.Value
                    );
                    continue;
                }
                PA L4ptPa = { .Fields4KB.PPN = L3pt[n3].Fields.PPN };
                DbgPrint("['%03d','"
                    "  Value=0x%016I64X\\n"
                    "    U/S=%I64d R/W=%I64d PS=%I64d XD=%I64d\\n"
                    "    PPN=0x%016I64X\\n"
                    "L4pt.Pa=0x%016I64X',"
                    , n3
                    , L3pt[n3].Value
                    , L3pt[n3].Fields.U_S
                    , L3pt[n3].Fields.R_W
                    , L3pt[n3].Fields.PS
                    , L3pt[n3].Fields.XD
                    , L3pt[n3].Fields.PPN
                    , L4ptPa.Value
                );
                MapPage4KB(L4ptPa.Fields4KB.PPN, SrcL4pte, Src);
                memcpy(L4pt, Src, 4096);
                DbgPrint("{title:'L1pt[%03d].L2pt[%03d].L3pt[%03d].L4pt',items:[", n1, n2, n3);
                for (int n4 = 0; n4 < 512; n4++)
                {
                    if (!L4pt[n4].Fields.P) continue;
                    PA Pa = { .Fields4KB.PPN = L4pt[n4].Fields.PPN };
                    VA Va = { .Fields4KB = { .VPN1 = n1, .VPN2 = n2, .VPN3 = n3, .VPN4 = n4 } };
                    CanonicalizeVa(&Va);
                    DbgPrint("['%03d','"
                        "Value=0x%016I64X\\n"
                        "  U/S=%I64d R/W=%I64d G=%I64d XD=%I64d\\n"
                        "  PPN=0x%016I64X\\n"
                        "   Pa=0x%016I64X\\n"
                        "   Va=0x%016I64X'],"
                        , n4
                        , L4pt[n4].Value
                        , L4pt[n4].Fields.U_S
                        , L4pt[n4].Fields.R_W
                        , L4pt[n4].Fields.G
                        , L4pt[n4].Fields.XD
                        , L4pt[n4].Fields.PPN
                        , Pa.Value
                        , Va.Value
                    );
                }
                DbgPrint("]}],");
            }
            DbgPrint("]}],");
        }
        DbgPrint("]}],");
    }
    DbgPrint("]}]]}\n");

Exit:
    if (SrcL4pte) FreeL4pte(SrcL4pte, Src);
    return Status;
}
