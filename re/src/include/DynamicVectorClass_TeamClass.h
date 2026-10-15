#pragma once
// ==========================================================================
// DynamicVectorClass_TeamClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class DynamicVectorClass<TeamClass*>
// vtable       : 0x0069f2c4  (6 slots)
// constructor  : 0x004a8293
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "VectorClass_TeamClass.h"

class DynamicVectorClass_TeamClass : public VectorClass_TeamClass
{
public:
    // ---- vtable 0x0069f2c4 (6 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x004b0a80  // __thiscall
    virtual void vfunc_1(int a0);  // [1] 0x004aadb0  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x004aaf30  // __thiscall, returns int
    virtual void vfunc_3();  // [3] 0x004a9640  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x004aafe0  // __stdcall
    virtual void vfunc_5(int a0);  // [5] 0x004a9670  // __stdcall, returns int

    DynamicVectorClass_TeamClass();  // 0x004a8293
    virtual ~DynamicVectorClass_TeamClass();

    // ---- data members (offsets from ctor 0x004fa212) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // /* ? */
    unsigned char pad_004[0xc];
    unsigned long m_010;  // init 0x69f2e0
    unsigned char pad_014[0xc];
    unsigned long m_020;  // init 0x69f2c4
    unsigned long m_024;  // /* ? */
    unsigned long m_028;  // /* ? */
    unsigned long m_02c;  // init 0x1
    unsigned long m_030;  // /* ? */
    unsigned long m_034;  // /* ? */
};
