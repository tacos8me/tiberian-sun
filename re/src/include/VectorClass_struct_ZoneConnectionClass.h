#pragma once
// ==========================================================================
// VectorClass_struct_ZoneConnectionClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class VectorClass<struct ZoneConnectionClass>
// vtable       : 0x006a1ad8  (6 slots)
// constructor  : 0x00501d72
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class VectorClass_struct_ZoneConnectionClass
{
public:
    // ---- vtable 0x006a1ad8 (6 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x00521f60  // __thiscall
    virtual void vfunc_1(int a0);  // [1] 0x00520ac0  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x00520b30  // __thiscall
    virtual void vfunc_3();  // [3] 0x00520c60  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x00520c90  // __stdcall, returns int
    virtual void vfunc_5(int a0);  // [5] 0x00520cf0  // __stdcall, returns int

    VectorClass_struct_ZoneConnectionClass();  // 0x00501d72
    virtual ~VectorClass_struct_ZoneConnectionClass();

    // ---- data members (offsets from ctor 0x00501d2e) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69f89c
    unsigned char pad_004[0x3c];
    unsigned long m_040;  // init 0x6a1ad8
    unsigned long m_044;  // /* ? */
    unsigned long m_048;  // /* ? */
    unsigned char pad_04c[0x1];
    unsigned long m_04d;  // /* ? */
};
