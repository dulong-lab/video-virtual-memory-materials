#pragma once
#include <Windows.h>

BOOL
EnableLargePage();

PVOID
AllocPage2MB();

PVOID
AllocPage1GB();

PVOID
AllocPage4KB();
