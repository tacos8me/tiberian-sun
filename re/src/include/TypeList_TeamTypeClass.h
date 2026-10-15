#pragma once
// ==========================================================================
// TypeList_TeamTypeClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class TypeList<TeamTypeClass*>
// vtable       : 0x0069faec  (6 slots)
// constructor  : 0x004b633d
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "DynamicVectorClass_TeamTypeClass.h"

class TypeList_TeamTypeClass : public DynamicVectorClass_TeamTypeClass
{
public:
    // ---- vtable 0x0069faec (6 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x004c53e0  // __thiscall
    virtual void vfunc_1(int a0);  // [1] 0x004c6030  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x004c61b0  // __thiscall, returns int
    virtual void vfunc_3();  // [3] 0x004c5330  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x004c6260  // __stdcall
    virtual void vfunc_5(int a0);  // [5] 0x004c5360  // __stdcall, returns int

    TypeList_TeamTypeClass();  // 0x004b633d
    virtual ~TypeList_TeamTypeClass();

    // ---- data members (offsets from ctor 0x004b796e) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69fb08
    unsigned long m_004;  // /* ? */
    unsigned long m_008;  // /* ? */
    unsigned long m_00c;  // init 0x1
    unsigned long m_010;  // /* ? */
    unsigned long m_014;  // /* ? */
};
