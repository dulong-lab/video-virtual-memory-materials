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
    PEPROCESS Process = NULL;
    CR3 UserDirectoryTableBase;

    Status = LookUpProcessByImageName(L"08.01.Repeater.exe", &Process);
    EXIT_IF_FAILED(Status, { DbgPrint("LookUpProcessByImageName failed. Status=%X\n", Status); });

    Status = GetProcessUserDirectoryTableBase(Process, &UserDirectoryTableBase);
    EXIT_IF_FAILED(Status, { DbgPrint("GetProcessUserDirectoryTableBase failed. Status=%X\n", Status); });

    Status = DumpPageTable(UserDirectoryTableBase);
    EXIT_IF_FAILED(Status, { DbgPrint("DumpPageTable failed. Status=%X\n", Status); });

Exit:
    if (Process) ObDereferenceObject(Process);
    return Status;
}
