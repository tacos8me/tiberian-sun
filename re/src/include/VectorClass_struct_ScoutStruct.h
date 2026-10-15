#pragma once
// ==========================================================================
// VectorClass_struct_ScoutStruct.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class VectorClass<struct ScoutStruct>
// vtable       : 0x0069fad0  (6 slots)
// constructor  : 0x004b54d7
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class VectorClass_struct_ScoutStruct
{
public:
    // ---- vtable 0x0069fad0 (6 slots) ----
    virtual void vfunc_1(int a0);  // [1] 0x004c5aa0  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x004c5b00  // __thiscall
    virtual void vfunc_3();  // [3] 0x004c5bf0  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x004c5c20  // __stdcall, returns int
    virtual void vfunc_5(int a0);  // [5] 0x004c5c70  // __stdcall, returns int

    VectorClass_struct_ScoutStruct();  // 0x004b54d7
    virtual ~VectorClass_struct_ScoutStruct();

    // ---- data members (offsets from ctor 0x004b5433) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned char pad_000[0x28];
    unsigned long m_028;  // init 0x69f85c
    unsigned long m_02c;  // /* ? */
    unsigned long m_030;  // /* ? */
    unsigned char pad_034[0x1];
    unsigned long m_035;  // /* ? */
    unsigned char pad_039[0x7];
    unsigned long m_040;  // init 0x69a298
    unsigned long m_044;  // /* ? */
    unsigned long m_048;  // /* ? */
    unsigned char pad_04c[0x1];
    unsigned long m_04d;  // /* ? */
    unsigned char pad_051[0xd7];
    unsigned long m_128;  // init 0x69f7b0
    unsigned long m_12c;  // /* ? */
    unsigned long m_130;  // /* ? */
    unsigned char pad_134[0x1];
    unsigned long m_135;  // /* ? */
    unsigned char pad_139[0x2c3];
    unsigned long m_3fc;  // init 0x69fab4
    unsigned long m_400;  // /* ? */
    unsigned long m_404;  // /* ? */
    unsigned char pad_408[0x1];
    unsigned long m_409;  // /* ? */
    unsigned char pad_40d[0x7];
    unsigned long m_414;  // init 0x69fad0
    unsigned long m_418;  // /* ? */
    unsigned long m_41c;  // /* ? */
    unsigned char pad_420[0x1];
    unsigned long m_421;  // /* ? */
    unsigned char pad_425[0xcf];
    unsigned long m_4f4;  // init 0x699db8
};
