#pragma once
// ==========================================================================
// IPXGlobalConnClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class IPXGlobalConnClass
// vtable       : 0x006a0af4  (13 slots)
// constructor  : 0x004e7460
// vtable store : 0x004e749e  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "IPXConnClass.h"

class IPXGlobalConnClass : public IPXConnClass
{
public:
    // ---- vtable 0x006a0af4 (13 slots) ----
    virtual void vfunc_1(int a0);  // [1] 0x004e6f10
    virtual void vfunc_2(int a0, int a1, int a2);  // [2] 0x004e79e0  // __stdcall
    virtual void vfunc_3(int a0, int a1);  // [3] 0x004e7a00  // __stdcall
    virtual void vfunc_4(int a0, int a1);  // [4] 0x004e7a20  // __stdcall
    virtual void vfunc_5();  // [5] 0x0045ee80  // __thiscall, returns int
    virtual void vfunc_6(int a0, int a1, int a2, int a3, int a4);  // [6] 0x004e7970  // __thiscall
    virtual void vfunc_7(int a0, int a1);  // [7] 0x0045eeb0  // __thiscall, returns int
    virtual void vfunc_8();  // [8] 0x004e7910  // __thiscall, returns int
    virtual void vfunc_9(int a0, int a1, int a2, int a3);  // [9] 0x004e78a0  // __thiscall
    virtual void vfunc_10(int a0, int a1, int a2);  // [10] 0x004e7590  // __thiscall
    virtual void vfunc_11(int a0, int a1, int a2);  // [11] 0x004e7640  // __thiscall, returns int
    virtual void vfunc_12(int a0, int a1, int a2, int a3);  // [12] 0x004e7810  // __stdcall, returns int

    IPXGlobalConnClass();  // 0x004e7460
    virtual ~IPXGlobalConnClass();

    // ---- data members (offsets from ctor 0x004e7460) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    // vtable-pointer stores are omitted: the compiler supplies the vptr.
    unsigned char pad_004[0x74];
    unsigned short m_078;  // +0x078 /* ? */; 2-byte store: mov word ptr [esi + 0x78], dx
    unsigned char pad_07a[0xa];
    unsigned int m_084;  // +0x084 /* ? */; 4-byte store: mov dword ptr [esi + 0x84], ebp
    unsigned int m_088;  // +0x088 /* ? */; 4-byte store: mov dword ptr [esi + 0x88], ebx
    unsigned int m_08c;  // +0x08c /* ? */; 4-byte store: mov dword ptr [esi + 0x8c], eax
    unsigned int m_090;  // +0x090 /* ? */; 4-byte store: mov dword ptr [esi + 0x90], edi
    unsigned int m_094;  // +0x094 /* ? */; 4-byte store: mov dword ptr [esi + 0x94], ebp
};
