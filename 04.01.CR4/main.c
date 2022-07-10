#include <ntifs.h>
#include <intrin.h>

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

    CR4 Cr4 = { .Value = __readcr4() };
    DbgPrint("CR4=0x%016I64X, CR4.LA57=%I64d\n", Cr4.Value, Cr4.Fields.LA57);

    return STATUS_SUCCESS;
}
