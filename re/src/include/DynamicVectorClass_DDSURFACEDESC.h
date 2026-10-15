#pragma once
// ==========================================================================
// DynamicVectorClass_DDSURFACEDESC.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class DynamicVectorClass<_DDSURFACEDESC*>
// vtable       : 0x0069b978  (6 slots)
// constructor  : 0x0046fcbb
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "VectorClass_DDSURFACEDESC.h"

class DynamicVectorClass_DDSURFACEDESC : public VectorClass_DDSURFACEDESC
{
public:
    // ---- vtable 0x0069b978 (6 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x00470150  // __thiscall
    virtual void vfunc_1(int a0);  // [1] 0x0046fea0  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x00470020  // __thiscall, returns int
    virtual void vfunc_3();  // [3] 0x0046fe50  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x004700d0  // __stdcall
    virtual void vfunc_5(int a0);  // [5] 0x0046fe80  // __stdcall, returns int

    DynamicVectorClass_DDSURFACEDESC();  // 0x0046fcbb
    virtual ~DynamicVectorClass_DDSURFACEDESC();

    // ---- data members (offsets from ctor 0x0046fc77) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned char pad_000[0xc];
    unsigned long m_00c;  // zero-initialised
    unsigned long m_010;  // /* ? */
    unsigned long m_014;  // /* ? */
    unsigned long m_018;  // init 0x69b95c
    unsigned char pad_01c[0x4];
    unsigned long m_020;  // init 0x69b978
    unsigned char pad_024[0xc];
    unsigned long m_030;  // zero-initialised
    unsigned long m_034;  // init 0xa
};
