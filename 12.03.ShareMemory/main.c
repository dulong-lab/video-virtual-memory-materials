#include "common.h"
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

    NTSTATUS Status = STATUS_SUCCESS;

    PEPROCESS RepeaterProcess = NULL;
    PEPROCESS PlaygroundProcess = NULL;

    CR3 RepeaterDirectoryTableBase;
    CR3 PlaygroundDirectoryTableBase;
    CR3 PlaygroundUserDirectoryTableBase;

    PL4PTE RepeaterL1ptL4pte = NULL;
    PL4PTE PlaygroundL1ptL4pte = NULL;
    PL4PTE PlaygroundUserL1ptL4pte = NULL;

    PL1PTE RepeaterL1pt = NULL;
    PL1PTE PlaygroundL1pt = NULL;
    PL1PTE PlaygroundUserL1pt = NULL;

    Status = LookUpProcessByImageName(L"08.01.Repeater.exe", &RepeaterProcess);
    EXIT_IF_FAILED(Status, { DbgPrint("LookUpProcessByImageName failed. Status=%X\n", Status); });

    Status = LookUpProcessByImageName(L"08.02.Playground.exe", &PlaygroundProcess);
    EXIT_IF_FAILED(Status, { DbgPrint("LookUpProcessByImageName failed. Status=%X\n", Status); });

    Status = GetProcessDirectoryTableBase(RepeaterProcess, &RepeaterDirectoryTableBase);
    EXIT_IF_FAILED(Status, { DbgPrint("Get RepeaterDirectoryTableBase failed. Status=%X\n", Status); });

    Status = GetProcessDirectoryTableBase(PlaygroundProcess, &PlaygroundDirectoryTableBase);
    EXIT_IF_FAILED(Status, { DbgPrint("GetProcessDirectoryTableBase failed. Status=%X\n", Status); });

    Status = GetProcessUserDirectoryTableBase(PlaygroundProcess, &PlaygroundUserDirectoryTableBase);
    EXIT_IF_FAILED(Status, { DbgPrint("GetProcessUserDirectoryTableBase failed. Status=%X\n", Status); });

    Status = AllocateL4pte(&RepeaterL1ptL4pte, &RepeaterL1pt);
    EXIT_IF_FAILED(Status, { DbgPrint("Allocate RepeaterL1ptL4pte failed. Status=%X\n", Status); });
    MapPage4KB(RepeaterDirectoryTableBase.Fields.PPN, RepeaterL1ptL4pte, RepeaterL1pt);

    Status = AllocateL4pte(&PlaygroundL1ptL4pte, &PlaygroundL1pt);
    EXIT_IF_FAILED(Status, { DbgPrint("Allocate PlaygroundL1ptL4pte failed. Status=%X\n", Status); });
    MapPage4KB(PlaygroundDirectoryTableBase.Fields.PPN, PlaygroundL1ptL4pte, PlaygroundL1pt);

    if (PlaygroundUserDirectoryTableBase.Fields.PPN)
    {
        Status = AllocateL4pte(&PlaygroundUserL1ptL4pte, &PlaygroundUserL1pt);
        EXIT_IF_FAILED(Status, { DbgPrint("Allocate PlaygroundUserL1ptL4pte failed. Status=%X\n", Status); });
        MapPage4KB(PlaygroundUserDirectoryTableBase.Fields.PPN, PlaygroundUserL1ptL4pte, PlaygroundUserL1pt);
    }

    size_t p = 0;
    for (size_t r = 0; r < 256; r++)
    {
        if (!RepeaterL1pt[r].Fields.P || !RepeaterL1pt[r].Fields.U_S) continue;

        for (; p < 256; p++)
        {
            if (PlaygroundL1pt[p].Fields.P) continue;

            L1PTE L1pte = RepeaterL1pt[r];
            L1pte.Fields.R_W = 1;
            PlaygroundL1pt[p] = L1pte;

            if (PlaygroundUserDirectoryTableBase.Fields.PPN)
            {
                L1pte.Fields.XD = 0;
                PlaygroundUserL1pt[p] = L1pte;
            }

            DbgPrint("%03zd -> %03zd\n", r, p);
            break;
        }
    }

Exit:
    if (PlaygroundL1ptL4pte) FreeL4pte(PlaygroundL1ptL4pte, PlaygroundL1pt);
    if (PlaygroundUserL1ptL4pte) FreeL4pte(PlaygroundUserL1ptL4pte, PlaygroundUserL1pt);
    if (RepeaterL1ptL4pte) FreeL4pte(RepeaterL1ptL4pte, RepeaterL1pt);
    if (PlaygroundProcess) ObDereferenceObject(PlaygroundProcess);
    if (RepeaterProcess) ObDereferenceObject(RepeaterProcess);
    return Status;
}
