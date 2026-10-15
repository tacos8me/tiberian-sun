#pragma once
// ==========================================================================
// TypeList_class_RGBClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class TypeList<class RGBClass>
// vtable       : 0x0069adc8  (6 slots)
// constructor  : 0x0044a209
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "DynamicVectorClass_class_RGBClass.h"

class TypeList_class_RGBClass : public DynamicVectorClass_class_RGBClass
{
public:
    // ---- vtable 0x0069adc8 (6 slots) ----
    virtual void vfunc_1(int a0);  // [1] 0x0044b530  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x0044b740  // __thiscall
    virtual void vfunc_3();  // [3] 0x0044ad10  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x0044b840  // __stdcall
    virtual void vfunc_5(int a0);  // [5] 0x0044ad40  // __stdcall, returns int

    TypeList_class_RGBClass();  // 0x0044a209
    virtual ~TypeList_class_RGBClass();

    // ---- data members (offsets from ctor 0x0044a1d7) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned char pad_000[0xf];
    unsigned long m_00f;  // init 0x1
    unsigned char pad_013[0x1];
    unsigned long m_014;  // /* ? */
    unsigned long m_018;  // /* ? */
    unsigned long m_01c;  // init 0x69adc8
    unsigned char pad_020[0x1];
    unsigned long m_021;  // /* ? */
    unsigned char pad_025[0x7];
    unsigned long m_02c;  // /* ? */
    unsigned long m_030;  // init 0xa
    unsigned char pad_034[0x20c];
    unsigned long m_240;  // /* ? */
};
