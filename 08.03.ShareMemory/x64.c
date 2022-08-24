#include "x64.h"

NTSTATUS
CurrentProcessVaToPa(VA Va, PPA Pa)
{
    CR3 Cr3 = { .Value = __readcr3() };

    PA L1ptPa = { .Fields4KB.PPN = Cr3.Fields.PPN };
    PL1PTE L1pt = MmGetVirtualForPhysical(L1ptPa.AsLargeInteger);
    L1PTE L1pte = L1pt[Va.Fields.VPN1];
    if (!L1pte.Fields.P) return STATUS_INVALID_ADDRESS;

    PA L2ptPa = { .Fields4KB.PPN = L1pte.Fields.PPN };
    PL2PTE L2pt = MmGetVirtualForPhysical(L2ptPa.AsLargeInteger);
    L2PTE L2pte = L2pt[Va.Fields.VPN2];
    if (!L2pte.Fields.P) return STATUS_INVALID_ADDRESS;

    if (L2pte.Fields1GB.PS)
    {
        *Pa = (PA){ .Fields1GB = {
            .PPN = L2pte.Fields1GB.PPN,
            .PPO = Va.Fields1GB.VPO
        } };
        return STATUS_SUCCESS;
    }

    PA L3ptPa = { .Fields4KB.PPN = L2pte.Fields.PPN };
    PL3PTE L3pt = MmGetVirtualForPhysical(L3ptPa.AsLargeInteger);
    L3PTE L3pte = L3pt[Va.Fields.VPN3];
    if (!L3pte.Fields.P) return STATUS_INVALID_ADDRESS;

    if (L3pte.Fields2MB.PS)
    {
        *Pa = (PA){ .Fields2MB = {
            .PPN = L3pte.Fields2MB.PPN,
            .PPO = Va.Fields2MB.VPO
        } };
        return STATUS_SUCCESS;
    }

    PA L4ptPa = { .Fields4KB.PPN = L3pte.Fields.PPN };
    PL4PTE L4pt = MmGetVirtualForPhysical(L4ptPa.AsLargeInteger);
    L4PTE L4pte = L4pt[Va.Fields.VPN4];
    if (!L4pte.Fields.P) return STATUS_INVALID_ADDRESS;

    *Pa = (PA){ .Fields4KB = {
        .PPN = L4pte.Fields.PPN,
        .PPO = Va.Fields4KB.VPO
    } };
    return STATUS_SUCCESS;
}

NTSTATUS
VaToPa(PEPROCESS Process, VA Va, PPA Pa)
{
    if (PsGetCurrentProcess() == Process) return CurrentProcessVaToPa(Va, Pa);

    NTSTATUS Status;
    KAPC_STATE ApcState;
    KeStackAttachProcess(Process, &ApcState);
    Status = CurrentProcessVaToPa(Va, Pa);
    KeUnstackDetachProcess(&ApcState);
    return Status;
}
