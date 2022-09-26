#include <stdio.h>
#include <excpt.h>
#include <intrin.h>
#include <stdint.h>

uint8_t probe_array[256][4096];
uint64_t access_time[256];

int
main()
{
    uint8_t secret = 42;
    uint8_t* p = &secret;

    for (size_t i = 0; i < 256; i++)
        _mm_clflush(&probe_array[i]);

    __try
    {
        *(uint32_t*)NULL = 0;
        probe_array[*p][0]++;
    }
    __except (EXCEPTION_EXECUTE_HANDLER) {}

    for (size_t i = 0; i < 256; i++)
    {
        uint32_t aux;
        uint64_t a = __rdtscp(&aux);
        probe_array[i][0]++;
        uint64_t b = __rdtscp(&aux);
        access_time[i] = b - a;
    }

    for (size_t i = 0; i < 256; i++)
        printf("%llu,", access_time[i]);
}
