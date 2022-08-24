#include "win10.h"
#include "x64.h"

VOID
DriverUnload(PDRIVER_OBJECT DriverObject)
{
    DbgPrint("ÔÙ¼û! %wZ\n", &DriverObject->DriverName);
}

NTSTATUS
DriverEntry(PDRIVER_OBJECT  DriverObject, PUNICODE_STRING RegistryPath)
{
    UNREFERENCED_PARAMETER(RegistryPath);

    DbgPrint("ÄãºÃ! %wZ\n", &DriverObject->DriverName);
    DriverObject->DriverUnload = DriverUnload;

    NTSTATUS Status;

    PEPROCESS RepeaterProcess;
    Status = LookUpProcessByImageName(L"08.01.Repeater.exe", &RepeaterProcess);
    if (!NT_SUCCESS(Status)) return Status;

    VA RepeaterVa = { .Value = 0x00000006194FF764 };
    PA RepeaterPa;
    Status = VaToPa(RepeaterProcess, RepeaterVa, &RepeaterPa);
    ObDereferenceObject(RepeaterProcess);
    if (!NT_SUCCESS(Status)) return Status;

    DbgPrint("RepeaterPa=%016I64X\n", RepeaterPa.Value);

    PEPROCESS PlaygroundProcess;
    Status = LookUpProcessByImageName(L"08.02.Playground.exe", &PlaygroundProcess);
    if (!NT_SUCCESS(Status)) return Status;

    KAPC_STATE ApcState;
    KeStackAttachProcess(PlaygroundProcess, &ApcState);
    CR3 Cr3 = { .Value = __readcr3() };
    PA L1ptPa = { .Fields4KB.PPN = Cr3.Fields.PPN };
    PL1PTE L1pt = MmGetVirtualForPhysical(L1ptPa.AsLargeInteger);
    for (size_t n1 = 0; n1 < 512; n1++)
    {
        if (!L1pt[n1].Fields.P || !L1pt[n1].Fields.U_S) continue;
        PA L2ptPa = { .Fields4KB.PPN = L1pt[n1].Fields.PPN };
        PL2PTE L2pt = MmGetVirtualForPhysical(L2ptPa.AsLargeInteger);
        for (size_t n2 = 0; n2 < 512; n2++)
        {
            if (n1 == 0 && n2 == 0) continue;
            if (L2pt[n2].Fields.P) continue;

            L2pt[n2] = (L2PTE){ .Fields1GB = {
                .P = 1,
                .PS = 1,
                .U_S = 1,
                .R_W = 1,
                .PPN = RepeaterPa.Fields1GB.PPN
            } };
            VA PlaygroundVa = { .Fields1GB = {
                .VPN1 = n1,
                .VPN2 = n2,
                .VPO = RepeaterPa.Fields1GB.PPO
            } };
            DbgPrint("PlaygroundVa=%016I64X VPN1=%I64d VPN2=%I64d VPO=%016I64X\n"
                , PlaygroundVa.Value
                , PlaygroundVa.Fields1GB.VPN1
                , PlaygroundVa.Fields1GB.VPN2
                , PlaygroundVa.Fields1GB.VPO
            );

            goto exit;
        }
    }
exit:
    KeUnstackDetachProcess(&ApcState);

    ObDereferenceObject(PlaygroundProcess);

    return STATUS_SUCCESS;
}
