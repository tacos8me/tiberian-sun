#pragma once
// ==========================================================================
// DynamicVectorClass_CCINIClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class DynamicVectorClass<CCINIClass*>
// vtable       : 0x006a0484  (6 slots)
// constructor  : 0x004d9b22
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "VectorClass_CCINIClass.h"

class DynamicVectorClass_CCINIClass : public VectorClass_CCINIClass
{
public:
    // ---- vtable 0x006a0484 (6 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x004e3790  // __thiscall
    virtual void vfunc_1(int a0);  // [1] 0x004e3240  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x004e33c0  // __thiscall, returns int
    virtual void vfunc_3();  // [3] 0x004e3100  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x004e3470  // __stdcall
    virtual void vfunc_5(int a0);  // [5] 0x004e3130  // __stdcall, returns int

    DynamicVectorClass_CCINIClass();  // 0x004d9b22
    virtual ~DynamicVectorClass_CCINIClass();

    // ---- data members (offsets from ctor 0x004d9aff) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned char pad_000[0x13];
    unsigned long m_013;  // init 0x1
    unsigned char pad_017[0x4];
    unsigned long m_01b;  // /* ? */
    unsigned char pad_01f[0x5];
    unsigned long m_024;  // /* ? */
    unsigned char pad_028[0x4];
    unsigned long m_02c;  // /* ? */
    unsigned long m_030;  // init 0xa
    unsigned char pad_034[0x58];
    unsigned long m_08c;  // /* ? */
};
