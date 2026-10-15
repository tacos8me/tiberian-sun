#pragma once
// ==========================================================================
// VectorClass_DDSURFACEDESC.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class VectorClass<_DDSURFACEDESC*>
// vtable       : 0x0069b95c  (6 slots)
// constructor  : 0x0046fdac
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class VectorClass_DDSURFACEDESC
{
public:
    // ---- vtable 0x0069b95c (6 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x00470100  // __thiscall
    virtual void vfunc_1(int a0);  // [1] 0x0046fea0  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x0046fee0  // __thiscall
    virtual void vfunc_3();  // [3] 0x0046ff90  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x0046ffc0  // __stdcall, returns int
    virtual void vfunc_5(int a0);  // [5] 0x00470000  // __stdcall, returns int

    VectorClass_DDSURFACEDESC();  // 0x0046fdac
    virtual ~VectorClass_DDSURFACEDESC();

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
