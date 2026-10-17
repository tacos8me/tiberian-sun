#pragma once
// ==========================================================================
// NullModemClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class NullModemClass
// vtable       : 0x006a2814  (16 slots)
// constructor  : 0x0056c330
// vtable store : 0x0056c363  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "ConnManClass.h"

class NullModemClass : public ConnManClass
{
public:
    // ---- vtable 0x006a2814 (16 slots) ----
    virtual void vfunc_1();  // [1] 0x0056c9d0  // __thiscall, returns int
    virtual void vfunc_2(int a0, int a1, int a2, int a3);  // [2] 0x0056e040  // __thiscall, returns int
    virtual void vfunc_3(int a0, int a1, int a2);  // [3] 0x0056e080  // __stdcall, returns int
    virtual void vfunc_4();  // [4] 0x0056c6c0  // __cdecl
    virtual void vfunc_5(int a0);  // [5] 0x0056e000  // __stdcall, returns int
    virtual void vfunc_6(int a0);  // [6] 0x0056e010  // __stdcall, returns int
    virtual void vfunc_7();  // [7] 0x0056e0b0  // __cdecl
    virtual void vfunc_8();  // [8] 0x0056e0d0  // __cdecl
    virtual void vfunc_9(int a0);  // [9] 0x0056e0f0  // __stdcall
    virtual void vfunc_10(int a0);  // [10] 0x0056e110  // __stdcall
    virtual void vfunc_11();  // [11] 0x0056cc70  // __cdecl
    virtual void vfunc_12();  // [12] 0x0056cc50  // __cdecl, returns int
    virtual void vfunc_13(int a0, int a1, int a2, int a3);  // [13] 0x0056c930  // __stdcall
    virtual void vfunc_14(int a0, int a1, int a2);  // [14] 0x0056e020  // __stdcall
    virtual void vfunc_15(int a0, int a1, int a2, int a3, int a4, int a5);  // [15] 0x0056ccd0  // __stdcall

    NullModemClass();  // 0x0056c330
    virtual ~NullModemClass();

    // ---- data members (offsets from ctor 0x0056c330) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    // vtable-pointer stores are omitted: the compiler supplies the vptr.
    // The machine block begins at +0x44: the compiled end of the
    // base sub-object region. Evidence: ConnManClass at +0x0 (ends +0x44).
    // 16 recovered offset(s) below +0x44 lie inside the base
    // sub-object region and are NOT declared here: they are declared and
    // compiler-checked in the base's own header -- +0x0, +0x4, +0x8, +0xc, +0x10, +0x18, +0x1c, +0x20 ...
    unsigned int m_044;  // +0x044 /* ? */; 4-byte store: mov dword ptr [eax + 0x44], esi
    unsigned int m_048;  // +0x048 /* ? */; 4-byte store: mov dword ptr [eax + 0x48], esi
    unsigned short m_04c;  // +0x04c /* ? */; 2-byte store: mov word ptr [eax + 0x4c], si
    unsigned char pad_04e[0x2];
    unsigned int m_050;  // +0x050 /* ? */; 4-byte store: mov dword ptr [eax + 0x50], ecx
    unsigned char pad_054[0x8];
    unsigned int m_05c;  // +0x05c /* ? */; 4-byte store: mov dword ptr [eax + 0x5c], esi
    unsigned int m_060;  // +0x060 /* ? */; 4-byte store: mov dword ptr [eax + 0x60], edx
    unsigned int m_064;  // +0x064 init 0x4b0; 4-byte store: mov dword ptr [eax + 0x64], 0x4b0
    unsigned int m_068;  // +0x068 /* ? */; 4-byte store: mov dword ptr [eax + 0x68], ecx
    unsigned int m_06c;  // +0x06c /* ? */; 4-byte store: mov dword ptr [eax + 0x6c], ecx
    unsigned int m_070;  // +0x070 /* ? */; 4-byte store: mov dword ptr [eax + 0x70], ecx
};
