#include "largepage.h"
#include <stdio.h>

BOOL
EnableLargePage()
{
    DWORD LastError;

    HANDLE TokenHandle;
    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY | TOKEN_ADJUST_PRIVILEGES, &TokenHandle))
    {
        return FALSE;
    }

    TOKEN_PRIVILEGES TokenPrivileges = {
        .PrivilegeCount = 1,
        .Privileges[0].Attributes = SE_PRIVILEGE_ENABLED
    };

    if (!LookupPrivilegeValue(NULL, SE_LOCK_MEMORY_NAME, &TokenPrivileges.Privileges[0].Luid))
    {
        LastError = GetLastError();
        CloseHandle(TokenHandle);
        SetLastError(LastError);
        return FALSE;
    }

    if (!AdjustTokenPrivileges(TokenHandle, FALSE, &TokenPrivileges, 0, NULL, 0))
    {
        LastError = GetLastError();
        CloseHandle(TokenHandle);
        SetLastError(LastError);
        return FALSE;
    }


    CloseHandle(TokenHandle);
    return TRUE;
}

PVOID
AllocPage2MB()
{
    return VirtualAlloc(NULL, 2ULL * 1024 * 1024, MEM_RESERVE | MEM_COMMIT | MEM_LARGE_PAGES, PAGE_READWRITE);
}

PVOID
AllocPage1GB()
{
    return VirtualAlloc(NULL, 1ULL * 1024 * 1024 * 1024, MEM_RESERVE | MEM_COMMIT | MEM_LARGE_PAGES, PAGE_READWRITE);
}

PVOID
AllocPage4KB()
{
    return VirtualAlloc(NULL, 4ULL * 1024, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
}
