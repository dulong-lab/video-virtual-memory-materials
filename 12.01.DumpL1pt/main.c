#include "common.h"
#include "win10.h"
#include "x64.h"

VOID
DriverUnload(PDRIVER_OBJECT DriverObject)
{
    DbgPrint("ÔÙ¼û! %wZ\n", &DriverObject->DriverName);
}

VOID
PrintL1pt(PL1PTE L1pt)
{
    DbgPrint("[");
    for (size_t i = 0; i < 512; i++)
    {
        DbgPrint("{"
                "'I':'%03zd',"
                "'V':'%016I64X',"
                "'P':%I64d,"
                "'R/W':%I64d,"
                "'U/S':%I64d,"
                "'XD':%I64d,"
                "'PPN':'%I64X'"
            "},"
            , i
            , L1pt[i].Value
            , L1pt[i].Fields.P
            , L1pt[i].Fields.R_W
            , L1pt[i].Fields.U_S
            , L1pt[i].Fields.XD
            , L1pt[i].Fields.PPN
        );
    }
    DbgPrint("]\n");
}

NTSTATUS
DriverEntry(PDRIVER_OBJECT  DriverObject, PUNICODE_STRING RegistryPath)
{
    UNREFERENCED_PARAMETER(RegistryPath);

    DbgPrint("ÄãºÃ! %wZ\n", &DriverObject->DriverName);
    DriverObject->DriverUnload = DriverUnload;

    NTSTATUS Status = STATUS_SUCCESS;
    PEPROCESS Process;
    CR3 DirectoryTableBase;
    CR3 UserDirectoryTableBase;
    PL4PTE L1ptL4pte = NULL;
    PL1PTE L1pt = NULL;

    Status = LookUpProcessByImageName(L"08.01.Repeater.exe", &Process);
    EXIT_IF_FAILED(Status, { DbgPrint("LookUpProcessByImageName failed. Status=%X\n", Status); });

    Status = GetProcessDirectoryTableBase(Process, &DirectoryTableBase);
    EXIT_IF_FAILED(Status, { DbgPrint("GetProcessDirectoryTableBase failed. Status=%X\n", Status); });

    Status = GetProcessUserDirectoryTableBase(Process, &UserDirectoryTableBase);
    EXIT_IF_FAILED(Status, { DbgPrint("GetProcessUserDirectoryTableBase failed. Status=%X\n", Status); });

    DbgPrint("DirectoryTableBase     = %I64X\n", DirectoryTableBase.Value);
    DbgPrint("UserDirectoryTableBase = %I64X\n", UserDirectoryTableBase.Value);

    Status = AllocateL4pte(&L1ptL4pte, &L1pt);
    EXIT_IF_FAILED(Status, { DbgPrint("AllocateL4pte failed. Status=%X\n", Status); });

    MapPage4KB(DirectoryTableBase.Fields.PPN, L1ptL4pte, L1pt);
    DbgPrint("Kernel L1pt\n");
    PrintL1pt(L1pt);

    MapPage4KB(UserDirectoryTableBase.Fields.PPN, L1ptL4pte, L1pt);
    DbgPrint("Shadow L1pt\n");
    PrintL1pt(L1pt);

Exit:
    if (L1ptL4pte) FreeL4pte(L1ptL4pte, L1pt);
    if (Process) ObDereferenceObject(Process);
    return Status;
}
