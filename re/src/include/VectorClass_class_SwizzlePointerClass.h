#pragma once
// ==========================================================================
// VectorClass_class_SwizzlePointerClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class VectorClass<class SwizzlePointerClass>
// vtable       : 0x006a50d8  (6 slots)
// constructor  : 0x005f35b4
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class VectorClass_class_SwizzlePointerClass
{
public:
    // ---- vtable 0x006a50d8 (6 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x005f3cd0  // __thiscall
    virtual void vfunc_1(int a0);  // [1] 0x005f39e0  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x005f3a20  // __thiscall
    virtual void vfunc_3();  // [3] 0x005f3b10  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x005f3b40  // __stdcall, returns int
    virtual void vfunc_5(int a0);  // [5] 0x005f3b80  // __stdcall, returns int

    VectorClass_class_SwizzlePointerClass();  // 0x005f35b4
    virtual ~VectorClass_class_SwizzlePointerClass();

    // ---- data members (offsets from ctor 0x005f3676) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned char pad_000[0x4];
    unsigned long m_004;  // init 0x6a50d8
    unsigned long m_008;  // /* ? */
    unsigned long m_00c;  // /* ? */
    unsigned char pad_010[0x1];
    unsigned long m_011;  // /* ? */
    unsigned char pad_015[0x7];
    unsigned long m_01c;  // init 0x6a50d8
    unsigned long m_020;  // /* ? */
    unsigned long m_024;  // /* ? */
    unsigned char pad_028[0x1];
    unsigned long m_029;  // /* ? */
};
