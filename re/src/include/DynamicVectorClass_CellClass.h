#pragma once
// ==========================================================================
// DynamicVectorClass_CellClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class DynamicVectorClass<CellClass*>
// vtable       : 0x006a2d14  (6 slots)
// constructor  : 0x005765bc
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "VectorClass_CellClass.h"

class DynamicVectorClass_CellClass : public VectorClass_CellClass
{
public:
    // ---- vtable 0x006a2d14 (6 slots) ----
    virtual void vfunc_1(int a0);  // [1] 0x00521870  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x00576870  // __thiscall, returns int
    virtual void vfunc_3();  // [3] 0x005767d0  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x00576920  // __stdcall
    virtual void vfunc_5(int a0);  // [5] 0x00576800  // __stdcall, returns int

    DynamicVectorClass_CellClass();  // 0x005765bc
    virtual ~DynamicVectorClass_CellClass();

    // ---- data members (offsets from ctor 0x00576574) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned char pad_000[0xc];
    unsigned long m_00c;  // init 0x6a2d14
    unsigned long m_010;  // /* ? */
    unsigned long m_014;  // /* ? */
    unsigned long m_018;  // init 0x1
    unsigned long m_01c;  // /* ? */
    unsigned long m_020;  // /* ? */
};
