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

    VA Va1 = { .Value = 0x00000206800003BC }; PA Pa1;
    VA Va2 = { .Value = 0x000002065560002A }; PA Pa2;
    VA Va3 = { .Value = 0x00000206551E0010 }; PA Pa3;

    NTSTATUS Status;
    PEPROCESS Process;
    Status = LookUpProcessByImageName(L"06.02.Sample.exe", &Process);
    if (!NT_SUCCESS(Status)) return Status;

    Status = VaToPa(Process, Va1, &Pa1);
    if (NT_SUCCESS(Status)) DbgPrint("Va1 0x%016I64X => Pa1 0x%016I64X\n", Va1.Value, Pa1.Value);

    Status = VaToPa(Process, Va2, &Pa2);
    if (NT_SUCCESS(Status)) DbgPrint("Va2 0x%016I64X => Pa2 0x%016I64X\n", Va2.Value, Pa2.Value);

    Status = VaToPa(Process, Va3, &Pa3);
    if (NT_SUCCESS(Status)) DbgPrint("Va3 0x%016I64X => Pa3 0x%016I64X\n", Va3.Value, Pa3.Value);

    ObDereferenceObject(Process);
    return STATUS_SUCCESS;
}
