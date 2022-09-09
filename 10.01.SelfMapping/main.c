#include "win10.h"
#include "x64.h"

VOID
DriverUnload(PDRIVER_OBJECT DriverObject)
{
    DbgPrint("ÔÙ¼û! %wZ\n", &DriverObject->DriverName);
}

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

#define CanonicalizeVa CanonicalizeVa1

NTSTATUS
FindSelfMappingEntryIndex1(UINT64* index)
{
    CR3 Cr3 = { .Value = __readcr3() };
    PA L1ptPa = { .Fields4KB.PPN = Cr3.Fields.PPN };
    PL1PTE L1pt = MmGetVirtualForPhysical(L1ptPa.AsLargeInteger);

    for (size_t n1 = 256; n1 < 512; n1++)
    {
        if (!L1pt[n1].Fields.P) continue;
        if (L1pt[n1].Fields.PPN != L1ptPa.Fields4KB.PPN) continue;

        *index = n1;
        return STATUS_SUCCESS;
    }

    return STATUS_NOT_FOUND;
}

NTSTATUS
FindSelfMappingEntryIndex2(UINT64* index)
{
    CR3 Cr3 = { .Value = __readcr3() };

    for (size_t i = 256; i < 512; i++)
    {
        VA L1ptVa = { .Fields4KB = { 
            .VPN1 = i,
            .VPN2 = i,
            .VPN3 = i,
            .VPN4 = i,
        } };
        CanonicalizeVa(&L1ptVa);
        PA L1ptPa = { .Value = MmGetPhysicalAddress((PVOID)L1ptVa.Value).QuadPart };
        if (L1ptPa.Fields4KB.PPN != Cr3.Fields.PPN) continue;

        *index = i;
        return STATUS_SUCCESS;
    }

    return STATUS_NOT_FOUND;
}

NTSTATUS
FindSelfMappingEntryIndex3(UINT64* index)
{
    VA L4ptVaMin = { .Value = *(PUINT64)((UINT64)MmGetVirtualForPhysical + 0x22) };
    *index = L4ptVaMin.Fields4KB.VPN1;
    return STATUS_SUCCESS;
}

#define FindSelfMappingEntryIndex FindSelfMappingEntryIndex1

PL1PTE
GetL1pt(UINT64 i)
{
    VA Va = { .Fields4KB = {
        .VPN1 = i,
        .VPN2 = i,
        .VPN3 = i,
        .VPN4 = i
    } };
    CanonicalizeVa(&Va);
    return (PL1PTE)Va.Value;
}

PL2PTE
GetL2pt(UINT64 i, UINT64 n1)
{
    VA Va = { .Fields4KB = {
        .VPN1 = i,
        .VPN2 = i,
        .VPN3 = i,
        .VPN4 = n1
    } };
    CanonicalizeVa(&Va);
    return (PL2PTE)Va.Value;
}

PL3PTE
GetL3pt(UINT64 i, UINT64 n1, UINT64 n2)
{
    VA Va = { .Fields4KB = {
        .VPN1 = i,
        .VPN2 = i,
        .VPN3 = n1,
        .VPN4 = n2
    } };
    CanonicalizeVa(&Va);
    return (PL3PTE)Va.Value;
}

PL4PTE
GetL4pt(UINT64 i, UINT64 n1, UINT64 n2, UINT64 n3)
{
    VA Va = { .Fields4KB = {
        .VPN1 = i,
        .VPN2 = n1,
        .VPN3 = n2,
        .VPN4 = n3
    } };
    CanonicalizeVa(&Va);
    return (PL4PTE)Va.Value;
}

NTSTATUS
DriverEntry(PDRIVER_OBJECT  DriverObject, PUNICODE_STRING RegistryPath)
{
    UNREFERENCED_PARAMETER(RegistryPath);

    DbgPrint("ÄãºÃ! %wZ\n", &DriverObject->DriverName);
    DriverObject->DriverUnload = DriverUnload;

    NTSTATUS Status;

    UINT64 i1;
    Status = FindSelfMappingEntryIndex1(&i1);
    if (!NT_SUCCESS(Status)) return Status;
    DbgPrint("FindSelfMappingEntryIndex1 %I64d\n", i1);

    UINT64 i2;
    Status = FindSelfMappingEntryIndex2(&i2);
    if (!NT_SUCCESS(Status)) return Status;
    DbgPrint("FindSelfMappingEntryIndex2 %I64d\n", i2);

    UINT64 i3;
    Status = FindSelfMappingEntryIndex3(&i3);
    if (!NT_SUCCESS(Status)) return Status;
    DbgPrint("FindSelfMappingEntryIndex3 %I64d\n", i3);
    UINT64 i;
    Status = FindSelfMappingEntryIndex(&i);
    if (!NT_SUCCESS(Status)) return Status;

    PL1PTE L1pt = GetL1pt(i);
    for (size_t n1 = 0; n1 < 512; n1++)
    {
        if (!L1pt[n1].Fields.P || !L1pt[n1].Fields.U_S) continue;
        DbgPrint("%03zd 0x%016I64X\n", n1, L1pt[n1].Value);

        PL2PTE L2pt = GetL2pt(i, n1);
        for (size_t n2 = 0; n2 < 512; n2++)
        {
            if (!L2pt[n2].Fields.P || !L2pt[n2].Fields.U_S) continue;
            DbgPrint(".. %03zd 0x%016I64X\n", n2, L2pt[n2].Value);
            if (L2pt[n2].Fields1GB.PS) continue;

            PL3PTE L3pt = GetL3pt(i, n1, n2);
            for (size_t n3 = 0; n3 < 512; n3++)
            {
                if (!L3pt[n3].Fields.P || !L3pt[n3].Fields.U_S) continue;
                DbgPrint(".. .. %03zd 0x%016I64X\n", n3, L3pt[n3].Value);
                if (L3pt[n3].Fields2MB.PS) continue;

                PL4PTE L4pt = GetL4pt(i, n1, n2, n3);
                for (size_t n4 = 0; n4 < 512; n4++)
                {
                    if (!L4pt[n4].Fields.P || !L4pt[n4].Fields.U_S) continue;
                    DbgPrint(".. .. .. %03zd 0x%016I64X\n", n4, L4pt[n4].Value);
                }
            }
        }
    }

    DumpUserPageTable(PsGetCurrentProcess());

    return STATUS_SUCCESS;
}
