#include <stdio.h>
#include <intrin.h>

int main()
{
    unsigned cpuInfo[4] = { 0 };
    __cpuid(cpuInfo, 0x80000008);

    unsigned eax = cpuInfo[0];
    printf("MAXPHYADDR=%u\n", eax & 0xff);
    return 0;
}