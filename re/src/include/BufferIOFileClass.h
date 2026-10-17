#pragma once
// ==========================================================================
// BufferIOFileClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class BufferIOFileClass
// vtable       : 0x00699f40  (17 slots)
// constructor  : 0x00420040
// vtable store : 0x00420099  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "RawFileClass.h"

class BufferIOFileClass : public RawFileClass
{
public:
    // ---- vtable 0x00699f40 (17 slots) ----
    virtual void vfunc_1();  // [1] 0x00419090  // __cdecl
    virtual void vfunc_2(int a0);  // [2] 0x00420490  // __thiscall
    virtual void vfunc_3();  // [3] 0x005a6850  // __thiscall, returns int
    virtual void vfunc_4();  // [4] 0x005a6890  // __thiscall, returns int
    virtual void vfunc_5(int a0);  // [5] 0x00420520  // __stdcall
    virtual void vfunc_6();  // [6] 0x00420540  // __cdecl
    virtual void vfunc_7(int a0);  // [7] 0x00420580  // __thiscall
    virtual void vfunc_8(int a0, int a1);  // [8] 0x00420560  // __thiscall
    virtual void vfunc_9(int a0, int a1);  // [9] 0x004208c0  // __thiscall
    virtual void vfunc_10(int a0, int a1);  // [10] 0x00420ad0  // __thiscall
    virtual void vfunc_11();  // [11] 0x00420bc0  // __cdecl
    virtual void vfunc_12(int a0, int a1);  // [12] 0x00420670  // __thiscall
    virtual void vfunc_13();  // [13] 0x00420be0  // __thiscall
    virtual void vfunc_14(int a0, int a1);  // [14] 0x005a68f0  // __cdecl
    virtual void vfunc_15(int a0);  // [15] 0x005a6940  // __thiscall
    virtual void vfunc_16(int a0, int a1, int a2);  // [16] 0x005a6190  // __stdcall

    BufferIOFileClass();  // 0x00420040
    virtual ~BufferIOFileClass();

    // ---- data members (offsets from ctor 0x00420040) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    // vtable-pointer stores are omitted: the compiler supplies the vptr.
    // The machine block begins at +0x1d: the compiled end of the
    // base sub-object region. Evidence: RawFileClass at +0x0 (ends +0x1d).
    // 9 recovered offset(s) below +0x1d lie inside the base
    // sub-object region and are NOT declared here: they are declared and
    // compiler-checked in the base's own header -- +0x0, +0x4, +0x8, +0xc, +0x10, +0x14, +0x18, +0x1a ...
    unsigned char pad_01d[0x3];
    unsigned char m_020;  // +0x020 /* ? */; 1-byte store: mov byte ptr [esi + 0x20], bl
    unsigned char m_021;  // +0x021 /* ? */; 1-byte store: mov byte ptr [esi + 0x21], bl
    unsigned char m_022;  // +0x022 /* ? */; 1-byte store: mov byte ptr [esi + 0x22], bl
    unsigned char m_023;  // +0x023 /* ? */; 1-byte store: mov byte ptr [esi + 0x23], bl
    unsigned char m_024;  // +0x024 /* ? */; 1-byte store: mov byte ptr [esi + 0x24], bl
    unsigned char m_025;  // +0x025 /* ? */; 1-byte store: mov byte ptr [esi + 0x25], bl
    unsigned char pad_026[0x2];
    unsigned int m_028;  // +0x028 /* ? */; 4-byte store: mov dword ptr [esi + 0x28], ebx
    unsigned int m_02c;  // +0x02c /* ? */; 4-byte store: mov dword ptr [esi + 0x2c], ebx
    unsigned int m_030;  // +0x030 /* ? */; 4-byte store: mov dword ptr [esi + 0x30], ebx
    unsigned int m_034;  // +0x034 /* ? */; 4-byte store: mov dword ptr [esi + 0x34], ebx
    unsigned int m_038;  // +0x038 /* ? */; 4-byte store: mov dword ptr [esi + 0x38], ebx
    unsigned int m_03c;  // +0x03c /* ? */; 4-byte store: mov dword ptr [esi + 0x3c], eax
    unsigned int m_040;  // +0x040 /* ? */; 4-byte store: mov dword ptr [esi + 0x40], eax
    unsigned int m_044;  // +0x044 /* ? */; 4-byte store: mov dword ptr [esi + 0x44], ebx
    unsigned int m_048;  // +0x048 /* ? */; 4-byte store: mov dword ptr [esi + 0x48], ebx
    unsigned int m_04c;  // +0x04c /* ? */; 4-byte store: mov dword ptr [esi + 0x4c], ebx
};
