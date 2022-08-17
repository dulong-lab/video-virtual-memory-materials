#include "win10.h"

NTSTATUS
QuerySystemInformation(SYSTEM_INFORMATION_CLASS SystemInformationClass, PVOID* SystemInformation)
{
    NTSTATUS Status;
    PVOID Buffer;
    ULONG BufferSize = 4096;

    do
    {
        Buffer = ExAllocatePool2(POOL_FLAG_NON_PAGED, BufferSize, 'bisQ');
        if (!Buffer) return STATUS_NO_MEMORY;

        Status = NtQuerySystemInformation(SystemInformationClass, Buffer, BufferSize, &BufferSize);
        if (NT_SUCCESS(Status)) {
            *SystemInformation = Buffer;
            return Status;
        }

        ExFreePool(Buffer);
        if (STATUS_INFO_LENGTH_MISMATCH != Status) return Status;
    } while (TRUE);
}

NTSTATUS
LookUpProcessByImageName(PCWSTR ImageName, PEPROCESS* Process)
{
    NTSTATUS Status;
    PSYSTEM_PROCESS_INFORMATION ProcessInformationArray = NULL;

    Status = QuerySystemInformation(SystemProcessInformation, &ProcessInformationArray);
    if (!NT_SUCCESS(Status)) return Status;

    PSYSTEM_PROCESS_INFORMATION CurrentInformation = ProcessInformationArray;
    UNICODE_STRING ImageNameUnicodeString;
    RtlInitUnicodeString(&ImageNameUnicodeString, ImageName);

    while (TRUE)
    {
        if (RtlCompareUnicodeString(&CurrentInformation->ImageName, &ImageNameUnicodeString, FALSE) == 0)
        {
            Status = PsLookupProcessByProcessId(CurrentInformation->UniqueProcessId, Process);
            ExFreePool(ProcessInformationArray);
            return Status;
        }

        if (CurrentInformation->NextEntryOffset == 0)
        {
            ExFreePool(ProcessInformationArray);
            return STATUS_NOT_FOUND;
        }

        CurrentInformation = (PSYSTEM_PROCESS_INFORMATION)((PUCHAR)CurrentInformation + CurrentInformation->NextEntryOffset);
    }
}
