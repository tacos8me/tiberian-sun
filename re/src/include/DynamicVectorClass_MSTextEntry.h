#pragma once
// ==========================================================================
// DynamicVectorClass_MSTextEntry.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class DynamicVectorClass<MSTextEntry*>
// vtable       : 0x006a25e0  (6 slots)
// constructor  : 0x0055a305
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "VectorClass_MSTextEntry.h"

class DynamicVectorClass_MSTextEntry : public VectorClass_MSTextEntry
{
public:
    // ---- vtable 0x006a25e0 (6 slots) ----
    virtual void vfunc_1(int a0);  // [1] 0x0055b170  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x0055b2f0  // __thiscall, returns int
    virtual void vfunc_3();  // [3] 0x0055acf0  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x0055b3a0  // __stdcall
    virtual void vfunc_5(int a0);  // [5] 0x0055ad20  // __stdcall, returns int

    DynamicVectorClass_MSTextEntry();  // 0x0055a305
    virtual ~DynamicVectorClass_MSTextEntry();

    // ---- data members (offsets from ctor 0x0055a2b5) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x6a25e0
    unsigned long m_004;  // /* ? */
    unsigned long m_008;  // /* ? */
    unsigned long m_00c;  // init 0x1
    unsigned long m_010;  // /* ? */
    unsigned long m_014;  // /* ? */
};
