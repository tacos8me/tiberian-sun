#pragma once
// ==========================================================================
// VectorClass_class_oxelAnimTypeClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class VectorClass<class oxelAnimTypeClass*>
// vtable       : 0x006a3c9c  (6 slots)
// constructor  : 0x005adb60
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class VectorClass_class_oxelAnimTypeClass
{
public:
    // ---- vtable 0x006a3c9c (6 slots) ----
    virtual void vfunc_1(int a0);  // [1] 0x005b9f10  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x005b9f50  // __thiscall
    virtual void vfunc_3();  // [3] 0x005ba000  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x005ba030  // __stdcall, returns int
    virtual void vfunc_5(int a0);  // [5] 0x005ba070  // __stdcall, returns int

    VectorClass_class_oxelAnimTypeClass();  // 0x005adb60
    virtual ~VectorClass_class_oxelAnimTypeClass();

    // ---- data members (offsets from ctor 0x005b18e0) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned char pad_000[0xc];
    unsigned long m_00c;  // /* ? */
    unsigned long m_010;  // init 0x6a3c9c
    unsigned char pad_014[0x4];
    unsigned long m_018;  // /* ? */
    unsigned char pad_01c[0x10];
    unsigned long m_02c;  // /* ? */
};
