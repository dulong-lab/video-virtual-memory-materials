#include <stdio.h>
#include <Windows.h>
#include <ntsecapi.h>

BOOL
InitLsaString(PLSA_UNICODE_STRING LsaString, LPCWSTR String)
{
    size_t Length = 0;

    if (NULL == LsaString) return FALSE;

    if (NULL != LsaString)
    {
        Length = wcslen(String);
        if (Length > 0x7ffe) return FALSE;
    }

    LsaString->Buffer = (WCHAR*)String;
    LsaString->Length = (USHORT)Length * sizeof(WCHAR);
    LsaString->MaximumLength = (USHORT)(Length + 1) * sizeof(WCHAR);

    return TRUE;
}

BOOL
AllocTokenInformation(HANDLE TokenHandle, TOKEN_INFORMATION_CLASS TokenInformationClass, PVOID* TokenInformation)
{
    PVOID Buffer = NULL;
    DWORD BufferSize = 4096;
    do
    {
        Buffer = malloc(BufferSize);
        if (GetTokenInformation(TokenHandle, TokenInformationClass, Buffer, BufferSize, &BufferSize))
        {
            *TokenInformation = Buffer;
            return TRUE;
        }

        DWORD LastError = GetLastError();
        free(Buffer);
        if (LastError != ERROR_INSUFFICIENT_BUFFER)
        {
            SetLastError(LastError);
            return FALSE;
        }
    } while (TRUE);
}

NTSTATUS
OpenLocalSystemPolicy(DWORD DesiredAccess, PLSA_HANDLE PolicyHandle)
{
    LSA_OBJECT_ATTRIBUTES ObjectAttributes = { 0 };
    return LsaOpenPolicy(NULL, &ObjectAttributes, DesiredAccess, PolicyHandle);
}

NTSTATUS
AddAccountRight(LSA_HANDLE PolicyHandle, PSID AccountSid, PCWSTR UserRightName)
{
    LSA_UNICODE_STRING UserRight;
    if (!InitLsaString(&UserRight, UserRightName)) return STATUS_INVALID_PARAMETER;

    return LsaAddAccountRights(PolicyHandle, AccountSid, &UserRight, 1);
}

int
main()
{
    HANDLE TokenHandle;
    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &TokenHandle))
    {
        printf("OpenProcessToken failed. LastError=%d\n", GetLastError());
        system("pause");
        return -1;
    }

    PTOKEN_USER TokenInformation = NULL;
    if (!AllocTokenInformation(TokenHandle, TokenUser, &TokenInformation))
    {
        printf("AllocTokenInformation failed. LastError=%d\n", GetLastError());
        CloseHandle(TokenHandle);
        system("pause");
        return -1;
    }

    CloseHandle(TokenHandle);

    NTSTATUS Status;
    LSA_HANDLE PolicyHandle = NULL;
    Status = OpenLocalSystemPolicy(POLICY_CREATE_ACCOUNT | POLICY_LOOKUP_NAMES, &PolicyHandle);
    if (Status < 0)
    {
        printf("OpenLocalSystemPolicy failed. Status=0x%08X\n", Status);
        free(TokenInformation);
        system("pause");
        return -1;
    }

    Status = AddAccountRight(PolicyHandle, TokenInformation->User.Sid, SE_LOCK_MEMORY_NAME);
    if (Status < 0)
    {
        printf("AddAccountRight failed. Status=0x%08X\n", Status);
        CloseHandle(PolicyHandle);
        free(TokenInformation);
        system("pause");
        return -1;
    }

    CloseHandle(PolicyHandle);
    free(TokenInformation);
    printf("Enable Lock Pages in Memory Option OK!\n");
    system("pause");
}
