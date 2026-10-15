#pragma once
// ==========================================================================
// DynamicVectorClass_class_WaypointClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class DynamicVectorClass<class WaypointClass>
// vtable       : 0x006a7554  (6 slots)
// constructor  : 0x00655801
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "VectorClass_class_WaypointClass.h"

class DynamicVectorClass_class_WaypointClass : public VectorClass_class_WaypointClass
{
public:
    // ---- vtable 0x006a7554 (6 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x00656020  // __thiscall
    virtual void vfunc_1(int a0);  // [1] 0x006560e0  // __stdcall
    virtual void vfunc_2(int a0, int a1, int a2);  // [2] 0x00656310  // __thiscall
    virtual void vfunc_3();  // [3] 0x00655f60  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x00656450  // __stdcall
    virtual void vfunc_5(int a0);  // [5] 0x00655fa0  // __stdcall, returns int

    DynamicVectorClass_class_WaypointClass();  // 0x00655801
    virtual ~DynamicVectorClass_class_WaypointClass();

    // ---- data members (offsets from ctor 0x006557d1) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x6a74f0
    unsigned long m_004;  // init 0x6a74d4
    unsigned char pad_008[0xc];
    unsigned long m_014;  // init 0xffffffff
    unsigned long m_018;  // init 0x6a7554
    unsigned long m_01c;  // /* ? */
    unsigned long m_020;  // /* ? */
    unsigned long m_024;  // init 0x1
    unsigned long m_028;  // /* ? */
    unsigned long m_02c;  // init 0xa
};
