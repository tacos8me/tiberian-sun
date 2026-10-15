#pragma once
// ==========================================================================
// VectorClass_class_WaypointClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class VectorClass<class WaypointClass>
// vtable       : 0x006a7570  (6 slots)
// constructor  : 0x006559c7
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class VectorClass_class_WaypointClass
{
public:
    // ---- vtable 0x006a7570 (6 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x00656080  // __thiscall
    virtual void vfunc_1(int a0);  // [1] 0x006560e0  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x00656140  // __thiscall
    virtual void vfunc_3();  // [3] 0x00656250  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x00656290  // __stdcall, returns int
    virtual void vfunc_5(int a0);  // [5] 0x006562e0  // __stdcall, returns int

    VectorClass_class_WaypointClass();  // 0x006559c7
    virtual ~VectorClass_class_WaypointClass();

    // ---- data members (offsets from ctor 0x00655951) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x6a74f0
    unsigned long m_004;  // init 0x6a74d4
    unsigned char pad_008[0x10];
    unsigned long m_018;  // init 0x6a7570
    unsigned long m_01c;  // zero-initialised
    unsigned long m_020;  // zero-initialised
    unsigned char pad_024[0x1];
    unsigned long m_025;  // zero-initialised
};
