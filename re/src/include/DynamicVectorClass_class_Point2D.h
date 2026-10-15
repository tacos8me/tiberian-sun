#pragma once
// ==========================================================================
// DynamicVectorClass_class_Point2D.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class DynamicVectorClass<class Point2D>
// vtable       : 0x006a25fc  (6 slots)
// constructor  : 0x0055a2ec
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "VectorClass_class_Point2D.h"

class DynamicVectorClass_class_Point2D : public VectorClass_class_Point2D
{
public:
    // ---- vtable 0x006a25fc (6 slots) ----
    virtual void vfunc_1(int a0);  // [1] 0x0055ae80  // __stdcall
    virtual void vfunc_2(int a0, int a1, int a2);  // [2] 0x0055b050  // __thiscall
    virtual void vfunc_3();  // [3] 0x0055aca0  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x0055b130  // __stdcall
    virtual void vfunc_5(int a0);  // [5] 0x0055acd0  // __stdcall, returns int

    DynamicVectorClass_class_Point2D();  // 0x0055a2ec
    virtual ~DynamicVectorClass_class_Point2D();

    // ---- data members (offsets from ctor 0x0055a2b5) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x6a25fc
    unsigned long m_004;  // /* ? */
    unsigned long m_008;  // /* ? */
    unsigned long m_00c;  // init 0x1
    unsigned long m_010;  // /* ? */
    unsigned long m_014;  // /* ? */
};
