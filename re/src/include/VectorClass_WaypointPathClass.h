#pragma once
// ==========================================================================
// VectorClass_WaypointPathClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class VectorClass<WaypointPathClass*>
// vtable       : 0x006a74b8  (6 slots)
// constructor  : 0x0065568e
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class VectorClass_WaypointPathClass
{
public:
    // ---- vtable 0x006a74b8 (6 slots) ----
    virtual void vfunc_1(int a0);  // [1] 0x00656490  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x006564d0  // __thiscall
    virtual void vfunc_3();  // [3] 0x00656580  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x006565b0  // __stdcall, returns int
    virtual void vfunc_5(int a0);  // [5] 0x006565f0  // __stdcall, returns int

    VectorClass_WaypointPathClass();  // 0x0065568e
    virtual ~VectorClass_WaypointPathClass();

    // ---- data members (offsets from ctor 0x006566e6) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x6a74b8
    unsigned long m_004;  // zero-initialised
    unsigned long m_008;  // zero-initialised
    unsigned char pad_00c[0x1];
    unsigned long m_00d;  // zero-initialised
};
