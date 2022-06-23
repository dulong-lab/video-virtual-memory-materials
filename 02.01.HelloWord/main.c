#include <ntifs.h>

VOID
DriverUnload(PDRIVER_OBJECT DriverObject)
{
    UNREFERENCED_PARAMETER(DriverObject);
    DbgPrint("Bye\n");
}

NTSTATUS
DriverEntry(PDRIVER_OBJECT  DriverObject, PUNICODE_STRING RegistryPath)
{
    UNREFERENCED_PARAMETER(RegistryPath);

    DbgPrint("Hello World!\n");
    DriverObject->DriverUnload = DriverUnload;
    return STATUS_SUCCESS;
}
