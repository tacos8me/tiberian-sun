#pragma once
// ==========================================================================
// DynamicVectorClass_W4DiskID.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class DynamicVectorClass<W4DiskID>
// vtable       : 0x006a42bc  (6 slots)
// constructor  : 0x005d66c5
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "VectorClass_W4DiskID.h"

class DynamicVectorClass_W4DiskID : public VectorClass_W4DiskID
{
public:
    // ---- vtable 0x006a42bc (6 slots) ----
    virtual void vfunc_1(int a0);  // [1] 0x005d6500  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x005d66e0  // __thiscall, returns int
    virtual void vfunc_3();  // [3] 0x005d6320  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x005d67e0  // __stdcall
    virtual void vfunc_5(int a0);  // [5] 0x005d6350  // __stdcall, returns int

    DynamicVectorClass_W4DiskID();  // 0x005d66c5
    virtual ~DynamicVectorClass_W4DiskID();

    // ---- data members (offsets from ctor 0x005d667b) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x6a4298
    unsigned long m_004;  // zero-initialised
    unsigned long m_008;  // /* ? */
    unsigned long m_00c;  // init 0x1
    unsigned long m_010;  // zero-initialised
    unsigned long m_014;  // init 0xa
};
