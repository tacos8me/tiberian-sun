#pragma once
// ==========================================================================
// DynamicVectorClass_D.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class DynamicVectorClass<D*>
// vtable       : 0x006a1720  (6 slots)
// constructor  : 0x004f6b27
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "VectorClass_D.h"

class DynamicVectorClass_D : public VectorClass_D
{
public:
    // ---- vtable 0x006a1720 (6 slots) ----
    virtual void vfunc_1(int a0);  // [1] 0x004f7ac0  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x004f7c40  // __thiscall, returns int
    virtual void vfunc_3();  // [3] 0x004f7a70  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x004f7cf0  // __stdcall
    virtual void vfunc_5(int a0);  // [5] 0x004f7aa0  // __stdcall, returns int

    DynamicVectorClass_D();  // 0x004f6b27
    virtual ~DynamicVectorClass_D();

    // ---- data members (offsets from ctor 0x004f6ac7) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x6a1654
    unsigned char pad_004[0x28];
    unsigned long m_02c;  // /* ? */
    unsigned long m_030;  // /* ? */
    unsigned long m_034;  // init 0x6a1720
    unsigned long m_038;  // /* ? */
    unsigned long m_03c;  // /* ? */
    unsigned long m_040;  // init 0x1
    unsigned long m_044;  // /* ? */
    unsigned long m_048;  // init 0xa
    unsigned long m_04c;  // /* ? */
    unsigned long m_050;  // /* ? */
    unsigned long m_054;  // zero-initialised
    unsigned char pad_058[0xc];
    unsigned long m_064;  // /* ? */
    unsigned char pad_068[0x48];
    unsigned long m_0b0;  // /* ? */
    unsigned long m_0b4;  // /* ? */
    unsigned char pad_0b8[0x44];
    unsigned long m_0fc;  // /* ? */
    unsigned long m_100;  // /* ? */
    unsigned long m_104;  // /* ? */
    unsigned long m_108;  // /* ? */
    unsigned char pad_10c[0x38];
    unsigned long m_144;  // /* ? */
    unsigned long m_148;  // /* ? */
};
