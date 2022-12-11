#pragma once
#include <ntifs.h>
#include <intrin.h>

typedef union _CR3
{
    UINT64 Value;
    struct
    {
        UINT64 Ignored1 : 3;
        UINT64 PWT : 1;
        UINT64 PCD : 1;
        UINT64 Ignored2 : 7;
        UINT64 PPN : 40;
        UINT64 Reserved1 : 12;
    } Fields;
} CR3, * PCR3;
static_assert(sizeof(CR3) == 8, "sizeof CR3");

typedef union _PA
{
    UINT64 Value;
    LARGE_INTEGER AsLargeInteger;
    struct
    {
        UINT64 PPO : 12;
        UINT64 PPN : 40;
        UINT64 Reserved1 : 12;
    } Fields4KB;
    struct
    {
        UINT64 PPO : 21;
        UINT64 PPN : 31;
        UINT64 Reserved1 : 12;
    } Fields2MB;
    struct
    {
        UINT64 PPO : 30;
        UINT64 PPN : 22;
        UINT64 Reserved1 : 12;
    } Fields1GB;
} PA, * PPA;

typedef union _L1PTE
{
    UINT64 Value;
    struct
    {
        UINT64 P : 1;
        UINT64 R_W : 1;
        UINT64 U_S : 1;
        UINT64 PWT : 1;
        UINT64 PCD : 1;
        UINT64 A : 1;
        UINT64 Ingored1 : 1;
        UINT64 Reserved1 : 1;
        UINT64 Ignored2 : 3;
        UINT64 R : 1;
        UINT64 PPN : 40;
        UINT64 Ignored3 : 11;
        UINT64 XD : 1;
    } Fields;
} L1PTE, * PL1PTE;
static_assert(sizeof(L1PTE) == 8, "sizeof L1PTE");

typedef union _L2PTE
{
    UINT64 Value;
    struct
    {
        UINT64 P : 1;
        UINT64 R_W : 1;
        UINT64 U_S : 1;
        UINT64 PWT : 1;
        UINT64 PCD : 1;
        UINT64 A : 1;
        UINT64 D : 1;
        UINT64 PS : 1;
        UINT64 G : 1;
        UINT64 Ignored1 : 2;
        UINT64 R : 1;
        UINT64 PAT : 1;
        UINT64 Reserved1 : 17;
        UINT64 PPN : 22;
        UINT64 Ignored2 : 7;
        UINT64 ProtKey : 4;
        UINT64 XD : 1;
    } Fields1GB;
    struct
    {
        UINT64 P : 1;
        UINT64 R_W : 1;
        UINT64 U_S : 1;
        UINT64 PWT : 1;
        UINT64 PCD : 1;
        UINT64 A : 1;
        UINT64 Ignored1 : 1;
        UINT64 PS : 1;
        UINT64 Ignored2 : 3;
        UINT64 R : 1;
        UINT64 PPN : 40;
        UINT64 Ignored3 : 11;
        UINT64 XD : 1;
    } Fields;
} L2PTE, * PL2PTE;
static_assert(sizeof(L2PTE) == 8, "sizeof L2PTE");

typedef union _L3PTE
{
    UINT64 Value;
    struct
    {
        UINT64 P : 1;
        UINT64 R_W : 1;
        UINT64 U_S : 1;
        UINT64 PWT : 1;
        UINT64 PCD : 1;
        UINT64 A : 1;
        UINT64 D : 1;
        UINT64 PS : 1;
        UINT64 G : 1;
        UINT64 Ignored1 : 2;
        UINT64 R : 1;
        UINT64 PAT : 1;
        UINT64 Reserved1 : 8;
        UINT64 PPN : 31;
        UINT64 Ignored2 : 7;
        UINT64 ProtKey : 4;
        UINT64 XD : 1;
    } Fields2MB;
    struct
    {
        UINT64 P : 1;
        UINT64 R_W : 1;
        UINT64 U_S : 1;
        UINT64 PWT : 1;
        UINT64 PCD : 1;
        UINT64 A : 1;
        UINT64 Ingored1 : 1;
        UINT64 PS : 1;
        UINT64 Ignored2 : 3;
        UINT64 R : 1;
        UINT64 PPN : 40;
        UINT64 Ignored3 : 11;
        UINT64 XD : 1;
    } Fields;
} L3PTE, * PL3PTE;
static_assert(sizeof(L3PTE) == 8, "sizeof L3PTE");

typedef union _L4PTE
{
    UINT64 Value;
    struct
    {
        UINT64 P : 1;
        UINT64 R_W : 1;
        UINT64 U_S : 1;
        UINT64 PWT : 1;
        UINT64 PCD : 1;
        UINT64 A : 1;
        UINT64 D : 1;
        UINT64 PAT : 1;
        UINT64 G : 1;
        UINT64 Ignored1 : 2;
        UINT64 R : 1;
        UINT64 PPN : 40;
        UINT64 Ignored2 : 7;
        UINT64 ProtKey : 4;
        UINT64 XD : 1;
    } Fields;
} L4PTE, * PL4PTE;
static_assert(sizeof(L4PTE) == 8, "sizeof L4PTE");

typedef union _VA
{
    UINT64 Value;
    PVOID AsPtr;
    struct
    {
        UINT64 VPO : 12;
        UINT64 VPN4 : 9;
        UINT64 VPN3 : 9;
        UINT64 VPN2 : 9;
        UINT64 VPN1 : 9;
        UINT64 SEXT : 16;
    } Fields4KB;
    struct
    {
        UINT64 VPO : 21;
        UINT64 VPN3 : 9;
        UINT64 VPN2 : 9;
        UINT64 VPN1 : 9;
        UINT64 SEXT : 16;
    } Fields2MB;
    struct
    {
        UINT64 VPO : 30;
        UINT64 VPN2 : 9;
        UINT64 VPN1 : 9;
        UINT64 SEXT : 16;
    } Fields1GB;
    struct
    {
        UINT64 Ignored1 : 12;
        UINT64 VPN4 : 9;
        UINT64 VPN3 : 9;
        UINT64 VPN2 : 9;
        UINT64 VPN1 : 9;
        UINT64 SEXT : 16;
    } Fields;
} VA, * PVA;
static_assert(sizeof(VA) == 8, "sizeof VA");

VOID
CanonicalizeVa1(VA* Va);

VOID
CanonicalizeVa2(VA* Va);

#define CanonicalizeVa CanonicalizeVa1

NTSTATUS
AllocateL4pte(PL4PTE* L4pte, PVOID* Ptr);

VOID
MapPage4KB(UINT64 PPN, PL4PTE L4pte, PVOID Ptr);

VOID
FreeL4pte(PL4PTE L4pte, PVOID Ptr);

NTSTATUS
DumpPageTable(CR3 Cr3);
