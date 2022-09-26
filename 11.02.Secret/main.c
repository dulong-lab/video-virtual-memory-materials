#include "win10.h"
#include "x64.h"
#include "secret.h"

PWSTR Secret;
HANDLE ThreadHandle;
BOOLEAN ThreadStopFlag = FALSE;

VOID
DriverUnload(PDRIVER_OBJECT DriverObject)
{
    DbgPrint("ÔÙ¼û! %wZ\n", &DriverObject->DriverName);
    ThreadStopFlag = TRUE;
    ZwWaitForSingleObject(ThreadHandle, FALSE, NULL);
    ZwClose(ThreadHandle);
    ExFreePool(Secret);
}

VOID
StartRoutine(PVOID StartContext)
{
    UNREFERENCED_PARAMETER(StartContext);

    KeSetSystemAffinityThread(1);

    UINT32 Junk = 0;
    size_t SecretLength = wcslen(Secret);
    LARGE_INTEGER Inteval = { .QuadPart = -10000 };

    while (!ThreadStopFlag)
    {
        for (size_t i = 0; i < SecretLength; i++)
        {
            Junk ^= Secret[i];
            Junk++;
            KeDelayExecutionThread(KernelMode, FALSE, &Inteval);
        }
    }

    PsTerminateSystemThread(STATUS_SUCCESS);
}

NTSTATUS
DriverEntry(PDRIVER_OBJECT  DriverObject, PUNICODE_STRING RegistryPath)
{
    UNREFERENCED_PARAMETER(RegistryPath);

    DbgPrint("ÄãºÃ! %wZ\n", &DriverObject->DriverName);
    DriverObject->DriverUnload = DriverUnload;

    Secret = ExAllocatePool2(POOL_FLAG_NON_PAGED, 4096, 'xxxx');
    if (!Secret) return STATUS_MEMORY_NOT_ALLOCATED;

    wcscpy(Secret, SecretData);

    NTSTATUS Status = PsCreateSystemThread(&ThreadHandle,
        0, NULL, NULL, NULL,
        StartRoutine, NULL
    );

    if (!NT_SUCCESS(Status))
    {
        ExFreePool(Secret);
        return Status;
    }

    DbgPrint("Secret @ %p\n", Secret);

    return STATUS_SUCCESS;
}
