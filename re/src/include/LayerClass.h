#pragma once
// ==========================================================================
// LayerClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class LayerClass
// vtable       : 0x0069ba60  (9 slots)
// constructor  : 0x004722a0
// vtable store : 0x004722d3  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "VectorClass.h"
#include "ObjectClass.h"

class LayerClass : public DynamicVectorClass<ObjectClass*>
{
public:
    // ---- vtable 0x0069ba60 (9 slots) ----
    virtual void vfunc_1(int a0);  // [1] 0x00402540  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x004026c0  // __thiscall, returns int
    virtual void vfunc_3();  // [3] 0x004024f0  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x00402770  // __stdcall
    virtual void vfunc_5(int a0);  // [5] 0x00402520  // __stdcall, returns int
    virtual void vfunc_6(int a0, int a1);  // [6] 0x004f3ac0  // __thiscall
    virtual void vfunc_7();  // [7] 0x00402480  // __cdecl
    virtual void vfunc_8();  // [8] 0x00402490  // __cdecl

    LayerClass();  // 0x004722a0
    virtual ~LayerClass();

    // ---- data members (offsets from ctor 0x004722a0) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    unsigned long m_000;  // +0x000 /* ? */; 4-byte store: mov dword ptr [eax], ecx
    unsigned char m_004;  // +0x004 init 0x1; 1-byte store: mov byte ptr [eax + 4], 1
    unsigned char m_005;  // +0x005 /* ? */; 1-byte store: mov byte ptr [eax + 5], cl
    unsigned char pad_006[0x2];
    unsigned long m_008;  // +0x008 /* ? */; 4-byte store: mov dword ptr [eax + 8], ecx
    unsigned long m_00c;  // +0x00c init 0xa; 4-byte store: mov dword ptr [eax + 0xc], 0xa
};
