#pragma once
// ==========================================================================
// DynamicVectorClass_uildingTypeClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class DynamicVectorClass<uildingTypeClass*>
// vtable       : 0x0069fb40  (6 slots)
// constructor  : 0x004baa88
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "VectorClass_uildingTypeClass.h"

class DynamicVectorClass_uildingTypeClass : public VectorClass_uildingTypeClass
{
public:
    // ---- vtable 0x0069fb40 (6 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x004c66d0  // __thiscall
    virtual void vfunc_1(int a0);  // [1] 0x004c5510  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x004c5690  // __thiscall, returns int
    virtual void vfunc_3();  // [3] 0x004c51f0  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x004c5740  // __stdcall
    virtual void vfunc_5(int a0);  // [5] 0x004c5220  // __stdcall, returns int

    DynamicVectorClass_uildingTypeClass();  // 0x004baa88
    virtual ~DynamicVectorClass_uildingTypeClass();

    // ---- data members (offsets from ctor 0x004bf2ca) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned char pad_000[0x18];
    unsigned long m_018;  // /* ? */
    unsigned long m_01c;  // /* ? */
    unsigned long m_020;  // /* ? */
    unsigned long m_024;  // /* ? */
    unsigned long m_028;  // init 0x69fb40
    unsigned char pad_02c[0xc];
    unsigned long m_038;  // /* ? */
    unsigned long m_03c;  // init 0xa
    unsigned long m_040;  // /* ? */
    unsigned char pad_044[0xc];
    unsigned long m_050;  // zero-initialised
    unsigned long m_054;  // /* ? */
    unsigned long m_058;  // /* ? */
    unsigned char pad_05c[0x4];
    unsigned long m_060;  // /* ? */
    unsigned char pad_064[0x4];
    unsigned long m_068;  // /* ? */
    unsigned char pad_06c[0x4];
    unsigned long m_070;  // /* ? */
    unsigned long m_074;  // /* ? */
    unsigned char pad_078[0x8];
    unsigned long m_080;  // /* ? */
    unsigned long m_084;  // /* ? */
};
