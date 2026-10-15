#pragma once
// ==========================================================================
// DynamicVectorClass_MyButton.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class DynamicVectorClass<MyButton*>
// vtable       : 0x006a3a60  (6 slots)
// constructor  : 0x005a8135
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "VectorClass_MyButton.h"

class DynamicVectorClass_MyButton : public VectorClass_MyButton
{
public:
    // ---- vtable 0x006a3a60 (6 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x005a9690  // __thiscall
    virtual void vfunc_1(int a0);  // [1] 0x005a93e0  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x005a9560  // __thiscall, returns int
    virtual void vfunc_3();  // [3] 0x005a9390  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x005a9610  // __stdcall
    virtual void vfunc_5(int a0);  // [5] 0x005a93c0  // __stdcall, returns int

    DynamicVectorClass_MyButton();  // 0x005a8135
    virtual ~DynamicVectorClass_MyButton();

    // ---- data members (offsets from ctor 0x005a8122) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned char pad_000[0x10];
    unsigned long m_010;  // /* ? */
    unsigned char pad_014[0x470];
    unsigned long m_484;  // init 0x6a3a60
    unsigned long m_488;  // /* ? */
    unsigned long m_48c;  // /* ? */
    unsigned char pad_490[0x1];
    unsigned long m_491;  // /* ? */
    unsigned char pad_495[0x3];
    unsigned long m_498;  // init 0xa
};
