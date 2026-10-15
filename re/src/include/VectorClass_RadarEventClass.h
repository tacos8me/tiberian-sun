#pragma once
// ==========================================================================
// VectorClass_RadarEventClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class VectorClass<RadarEventClass*>
// vtable       : 0x006a3b48  (6 slots)
// constructor  : 0x005a9984
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class VectorClass_RadarEventClass
{
public:
    // ---- vtable 0x006a3b48 (6 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x005aacc0  // __thiscall
    virtual void vfunc_1(int a0);  // [1] 0x005aaa20  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x005aaa60  // __thiscall
    virtual void vfunc_3();  // [3] 0x005aab10  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x005aab40  // __stdcall, returns int
    virtual void vfunc_5(int a0);  // [5] 0x005aab80  // __stdcall, returns int

    VectorClass_RadarEventClass();  // 0x005a9984
    virtual ~VectorClass_RadarEventClass();

    // ---- data members (offsets from ctor 0x005aacb6) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x6a3b48
    unsigned long m_004;  // zero-initialised
    unsigned long m_008;  // zero-initialised
    unsigned char pad_00c[0x1];
    unsigned long m_00d;  // zero-initialised
};
