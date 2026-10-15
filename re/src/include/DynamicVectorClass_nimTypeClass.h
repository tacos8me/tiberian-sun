#pragma once
// ==========================================================================
// DynamicVectorClass_nimTypeClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class DynamicVectorClass<nimTypeClass&*>
// vtable       : 0x006a3d50  (6 slots)
// constructor  : 0x005ba455
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "VectorClass_nimTypeClass.h"

class DynamicVectorClass_nimTypeClass : public VectorClass_nimTypeClass
{
public:
    // ---- vtable 0x006a3d50 (6 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x005bb480  // __thiscall
    virtual void vfunc_1(int a0);  // [1] 0x005ba290  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x005ba470  // __thiscall, returns int
    virtual void vfunc_3();  // [3] 0x005b9740  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x005ba570  // __stdcall
    virtual void vfunc_5(int a0);  // [5] 0x005b9770  // __stdcall, returns int

    DynamicVectorClass_nimTypeClass();  // 0x005ba455
    virtual ~DynamicVectorClass_nimTypeClass();

    // ---- data members (offsets from ctor 0x005ba40b) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x6a3d0c
    unsigned long m_004;  // zero-initialised
    unsigned long m_008;  // /* ? */
    unsigned long m_00c;  // init 0x1
    unsigned long m_010;  // zero-initialised
    unsigned long m_014;  // init 0xa
};
