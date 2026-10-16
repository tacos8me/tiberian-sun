#pragma once
// ==========================================================================
// LayerClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class LayerClass
// vtable       : 0x0069ba60  (9 slots)
// constructor  : 0x00472000
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

    LayerClass();  // 0x00472000
    virtual ~LayerClass();

    // ---- data members (offsets from ctor 0x00472292) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // /* ? */
    unsigned long m_004;  // init 0x1
    unsigned long m_008;  // /* ? */
    unsigned long m_00c;  // init 0xa
};
