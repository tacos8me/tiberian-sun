#pragma once
// ==========================================================================
// DynamicVectorClass_class_SwizzlePointerClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class DynamicVectorClass<class SwizzlePointerClass>
// vtable       : 0x006a50bc  (6 slots)
// constructor  : 0x005f352d
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "VectorClass_class_SwizzlePointerClass.h"

class DynamicVectorClass_class_SwizzlePointerClass : public VectorClass_class_SwizzlePointerClass
{
public:
    // ---- vtable 0x006a50bc (6 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x005f3d20  // __thiscall
    virtual void vfunc_1(int a0);  // [1] 0x005f39e0  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x005f3ba0  // __thiscall
    virtual void vfunc_3();  // [3] 0x005f3990  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x005f3ca0  // __stdcall
    virtual void vfunc_5(int a0);  // [5] 0x005f39c0  // __stdcall, returns int

    DynamicVectorClass_class_SwizzlePointerClass();  // 0x005f352d
    virtual ~DynamicVectorClass_class_SwizzlePointerClass();

    // ---- data members (offsets from ctor 0x005f3615) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x6a5090
    unsigned long m_004;  // init 0x6a50bc
    unsigned long m_008;  // /* ? */
    unsigned long m_00c;  // /* ? */
    unsigned long m_010;  // init 0x1
    unsigned long m_014;  // /* ? */
    unsigned long m_018;  // /* ? */
    unsigned long m_01c;  // init 0x6a50bc
    unsigned long m_020;  // /* ? */
    unsigned long m_024;  // /* ? */
    unsigned long m_028;  // init 0x1
    unsigned long m_02c;  // /* ? */
    unsigned long m_030;  // /* ? */
};
