#pragma once
// ==========================================================================
// DynamicVectorClass_MSSfxEntry.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class DynamicVectorClass<MSSfxEntry*>
// vtable       : 0x006a251c  (6 slots)
// constructor  : 0x00559a10
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "VectorClass_MSSfxEntry.h"

class DynamicVectorClass_MSSfxEntry : public VectorClass_MSSfxEntry
{
public:
    // ---- vtable 0x006a251c (6 slots) ----
    virtual void vfunc_1(int a0);  // [1] 0x0055baf0  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x0055bc70  // __thiscall, returns int
    virtual void vfunc_3();  // [3] 0x0055ae30  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x0055bd20  // __stdcall
    virtual void vfunc_5(int a0);  // [5] 0x0055ae60  // __stdcall, returns int

    DynamicVectorClass_MSSfxEntry();  // 0x00559a10
    virtual ~DynamicVectorClass_MSSfxEntry();

    // ---- data members (offsets from ctor 0x005599b7) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x6a2554
    unsigned long m_004;  // /* ? */
    unsigned long m_008;  // /* ? */
    unsigned long m_00c;  // /* ? */
    unsigned long m_010;  // /* ? */
    unsigned long m_014;  // /* ? */
    unsigned char pad_018[0x10];
    unsigned long m_028;  // /* ? */
    unsigned char pad_02c[0x18];
    unsigned long m_044;  // init 0x6a251c
    unsigned long m_048;  // /* ? */
    unsigned long m_04c;  // /* ? */
    unsigned long m_050;  // /* ? */
    unsigned long m_054;  // /* ? */
    unsigned long m_058;  // /* ? */
};
