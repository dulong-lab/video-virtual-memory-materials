#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "largepage.h"

int
main()
{
    if (!EnableLargePage())
    {
        printf("EnableLargePage failed. LastError=%d\n", GetLastError());
        system("pause");
        return -1;
    }

    char* Mem1 = AllocPage1GB();
    char* Mem2 = AllocPage2MB();
    char* Mem3 = AllocPage4KB();

    if (!Mem1) {
        printf("Allocate Mem1 failed\n");
        system("pause");
        return -1;
    }
    if (!Mem2) {
        printf("Allocate Mem2 failed\n");
        system("pause");
        return -1;
    }
    if (!Mem3) {
        printf("Allocate Mem3 failed\n");
        system("pause");
        return -1;
    }

    char* Str1 = Mem1 + 0x3bc;
    char* Str2 = Mem2 + 0x2a;
    char* Str3 = Mem3 + 0x10;
    strcpy(Str1, "Str1");
    strcpy(Str2, "Str2");
    strcpy(Str3, "Str3");

    for (;;)
    {
        printf("0x%p `%s`\n0x%p `%s`\n0x%p `%s`\n\n"
            , Str1, Str1
            , Str2, Str2
            , Str3, Str3
        );
        system("pause>nul");
    }
}
