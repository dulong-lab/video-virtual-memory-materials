#pragma once
#include <stdbool.h>

#define EXIT_IF_FAILED(Status, ...) do\
{\
    if (!NT_SUCCESS(Status))\
    {\
        __VA_ARGS__\
        goto Exit; \
    }\
} while(false)
