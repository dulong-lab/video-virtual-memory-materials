#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int
main()
{
    int Value = 123;

    while (true)
    {
        printf("0x%p # %d\n", &Value, Value);
        system("pause>nul");
    }
}
