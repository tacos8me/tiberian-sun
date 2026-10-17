#pragma once
// ==========================================================================
// IPXManagerClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class IPXManagerClass
// vtable       : 0x006a0b34  (17 slots)
// constructor  : 0x004e7d00
// vtable store : 0x004e7d09  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "ConnManClass.h"

class IPXManagerClass : public ConnManClass
{
public:
    // ---- vtable 0x006a0b34 (17 slots) ----
    virtual void vfunc_1(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10);  // [1] 0x004e8510  // __thiscall
    virtual void vfunc_2(int a0, int a1, int a2, int a3);  // [2] 0x004e83a0  // __thiscall, returns int
    virtual void vfunc_3(int a0, int a1, int a2);  // [3] 0x004e8480  // __thiscall, returns int
    virtual void vfunc_4();  // [4] 0x004e81b0  // __cdecl
    virtual void vfunc_5(int a0);  // [5] 0x004e81c0  // __stdcall
    virtual void vfunc_6(int a0);  // [6] 0x004e8260  // __stdcall
    virtual void vfunc_7();  // [7] 0x004e87c0  // __cdecl
    virtual void vfunc_8();  // [8] 0x004e87e0  // __cdecl
    virtual void vfunc_9(int a0);  // [9] 0x004e8800  // __thiscall
    virtual void vfunc_10(int a0);  // [10] 0x004e8870  // __thiscall
    virtual void vfunc_11();  // [11] 0x004e8960  // __thiscall
    virtual void vfunc_12();  // [12] 0x004e8900  // __thiscall
    virtual void vfunc_13(int a0, int a1, int a2, int a3);  // [13] 0x004e7f90  // __thiscall
    virtual void vfunc_14(int a0, int a1, int a2);  // [14] 0x004e8040  // __stdcall
    virtual void vfunc_15(int a0, int a1, int a2, int a3, int a4, int a5);  // [15] 0x004e8a50  // __stdcall
    virtual void vfunc_16(int a0, int a1);  // [16] 0x004e8ab0  // __stdcall

    IPXManagerClass();  // 0x004e7d00
    virtual ~IPXManagerClass();

    // ---- data members (offsets from ctor 0x004e7d00) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    // vtable-pointer stores are omitted: the compiler supplies the vptr.
    // The machine block begins at +0x44: the compiled end of the
    // base sub-object region. Evidence: ConnManClass at +0x0 (ends +0x44).
    // 12 recovered offset(s) below +0x44 lie inside the base
    // sub-object region and are NOT declared here: they are declared and
    // compiler-checked in the base's own header -- +0x0, +0x4, +0x5, +0x8, +0xc, +0x10, +0x14, +0x18 ...
    unsigned int m_044;  // +0x044 /* ? */; 4-byte store: mov dword ptr [esi + 0x44], ebx
    unsigned int m_048;  // +0x048 init 0x2; 4-byte store: mov dword ptr [esi + 0x48], 2
    unsigned int m_04c;  // +0x04c /* ? */; 4-byte store: mov dword ptr [esi + 0x4c], eax
    unsigned int m_050;  // +0x050 init 0x3c; 4-byte store: mov dword ptr [esi + 0x50], 0x3c
    unsigned char pad_054[0x30];
    unsigned int m_084;  // +0x084 /* ? */; 4-byte store: mov dword ptr [esi + 0x84], ebx
    unsigned int m_088;  // +0x088 /* ? */; 4-byte store: mov dword ptr [esi + 0x88], ebx
    unsigned int m_08c;  // +0x08c /* ? */; 4-byte store: mov dword ptr [esi + 0x8c], eax
};
