#pragma once
// ==========================================================================
// DynamicVectorClass_MSAnim.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class DynamicVectorClass<MSAnim*>
// vtable       : 0x006a2698  (6 slots)
// constructor  : 0x0055c162
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "VectorClass_MSAnim.h"

class DynamicVectorClass_MSAnim : public VectorClass_MSAnim
{
public:
    // ---- vtable 0x006a2698 (6 slots) ----
    virtual void vfunc_1(int a0);  // [1] 0x0055cf20  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x0055d0a0  // __thiscall, returns int
    virtual void vfunc_3();  // [3] 0x0055cc70  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x0055d150  // __stdcall
    virtual void vfunc_5(int a0);  // [5] 0x0055cca0  // __stdcall, returns int

    DynamicVectorClass_MSAnim();  // 0x0055c162
    virtual ~DynamicVectorClass_MSAnim();

    // ---- data members (offsets from ctor 0x0055c127) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x6a2698
    unsigned long m_004;  // /* ? */
    unsigned long m_008;  // /* ? */
    unsigned long m_00c;  // /* ? */
    unsigned long m_010;  // /* ? */
    unsigned long m_014;  // init 0xa
};
