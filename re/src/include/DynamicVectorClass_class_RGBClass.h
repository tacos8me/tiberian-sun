#pragma once
// ==========================================================================
// DynamicVectorClass_class_RGBClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class DynamicVectorClass<class RGBClass>
// vtable       : 0x006a3514  (6 slots)
// constructor  : 0x0059886e
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "VectorClass_class_RGBClass.h"

class DynamicVectorClass_class_RGBClass : public VectorClass_class_RGBClass
{
public:
    // ---- vtable 0x006a3514 (6 slots) ----
    virtual void vfunc_1(int a0);  // [1] 0x0044b530  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x0044b740  // __thiscall
    virtual void vfunc_3();  // [3] 0x0044ad10  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x0044b840  // __stdcall
    virtual void vfunc_5(int a0);  // [5] 0x0044ad40  // __stdcall, returns int

    DynamicVectorClass_class_RGBClass();  // 0x0059886e
    virtual ~DynamicVectorClass_class_RGBClass();

    // ---- data members (offsets from ctor 0x005987ea) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69adac
    unsigned long m_004;  // /* ? */
    unsigned long m_008;  // /* ? */
    unsigned long m_00c;  // init 0x1
    unsigned long m_010;  // /* ? */
    unsigned long m_014;  // init 0xa
};
