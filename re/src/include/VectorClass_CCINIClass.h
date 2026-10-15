#pragma once
// ==========================================================================
// VectorClass_CCINIClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class VectorClass<CCINIClass*>
// vtable       : 0x006a0468  (6 slots)
// constructor  : 0x004d9da7
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class VectorClass_CCINIClass
{
public:
    // ---- vtable 0x006a0468 (6 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x004e3740  // __thiscall
    virtual void vfunc_1(int a0);  // [1] 0x004e3240  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x004e3280  // __thiscall
    virtual void vfunc_3();  // [3] 0x004e3330  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x004e3360  // __stdcall, returns int
    virtual void vfunc_5(int a0);  // [5] 0x004e33a0  // __stdcall, returns int

    VectorClass_CCINIClass();  // 0x004d9da7
    virtual ~VectorClass_CCINIClass();

    // ---- data members (offsets from ctor 0x004d9cc4) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned char pad_000[0x14];
    unsigned long m_014;  // init 0x6a0468
    unsigned char pad_018[0xc];
    unsigned long m_024;  // /* ? */
    unsigned char pad_028[0x4];
    unsigned long m_02c;  // init 0x699bc8
    unsigned char pad_030[0x5c];
    unsigned long m_08c;  // /* ? */
};
