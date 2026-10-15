#pragma once
// ==========================================================================
// DynamicVectorClass_struct_tagCONNECTDATA.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class DynamicVectorClass<struct tagCONNECTDATA>
// vtable       : 0x0069b850  (6 slots)
// constructor  : 0x0046bb24
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "VectorClass_struct_tagCONNECTDATA.h"

class DynamicVectorClass_struct_tagCONNECTDATA : public VectorClass_struct_tagCONNECTDATA
{
public:
    // ---- vtable 0x0069b850 (6 slots) ----
    virtual void vfunc_1(int a0);  // [1] 0x0046c820  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x0046c9e0  // __thiscall, returns int
    virtual void vfunc_3();  // [3] 0x0046c780  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x0046caa0  // __stdcall
    virtual void vfunc_5(int a0);  // [5] 0x0046c7b0  // __stdcall, returns int

    DynamicVectorClass_struct_tagCONNECTDATA();  // 0x0046bb24
    virtual ~DynamicVectorClass_struct_tagCONNECTDATA();

    // ---- data members (offsets from ctor 0x0046bb76) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69b814
    unsigned long m_004;  // /* ? */
    unsigned long m_008;  // /* ? */
    unsigned long m_00c;  // init 0x1
    unsigned long m_010;  // /* ? */
    unsigned long m_014;  // /* ? */
};
