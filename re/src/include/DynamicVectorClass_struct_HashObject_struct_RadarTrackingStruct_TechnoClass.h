#pragma once
// ==========================================================================
// DynamicVectorClass_struct_HashObject_struct_RadarTrackingStruct_TechnoClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class DynamicVectorClass<struct HashObject<struct RadarTrackingStruct>, TechnoClass*>
// vtable       : 0x006a36d8  (6 slots)
// constructor  : 0x005a2d3e
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "VectorClass_struct_HashObject_struct_RadarTrackingStruct_TechnoClass.h"

class DynamicVectorClass_struct_HashObject_struct_RadarTrackingStruct_TechnoClass : public VectorClass_struct_HashObject_struct_RadarTrackingStruct_TechnoClass
{
public:
    // ---- vtable 0x006a36d8 (6 slots) ----
    virtual void vfunc_1(int a0);  // [1] 0x005a4f50  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x005a5150  // __thiscall
    virtual void vfunc_3();  // [3] 0x005a4f00  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x005a5260  // __stdcall
    virtual void vfunc_5(int a0);  // [5] 0x005a4f30  // __stdcall, returns int

    DynamicVectorClass_struct_HashObject_struct_RadarTrackingStruct_TechnoClass();  // 0x005a2d3e
    virtual ~DynamicVectorClass_struct_HashObject_struct_RadarTrackingStruct_TechnoClass();

    // ---- data members (offsets from ctor 0x005a2cec) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x100
    unsigned long m_004;  // /* ? */
    unsigned long m_008;  // /* ? */
    unsigned long m_00c;  // /* ? */
    unsigned long m_010;  // /* ? */
    unsigned long m_014;  // /* ? */
};
