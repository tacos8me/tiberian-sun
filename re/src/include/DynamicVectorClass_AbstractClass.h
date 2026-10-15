#pragma once
// ==========================================================================
// DynamicVectorClass_AbstractClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class DynamicVectorClass<AbstractClass*>
// vtable       : 0x0069aa58  (6 slots)
// constructor  : 0x004449e8
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "VectorClass_AbstractClass.h"

class DynamicVectorClass_AbstractClass : public VectorClass_AbstractClass
{
public:
    // ---- vtable 0x0069aa58 (6 slots) ----
    virtual void vfunc_1(int a0);  // [1] 0x00445310  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x00445490  // __thiscall, returns int
    virtual void vfunc_3();  // [3] 0x004452a0  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x00445540  // __stdcall
    virtual void vfunc_5(int a0);  // [5] 0x004452d0  // __stdcall, returns int

    DynamicVectorClass_AbstractClass();  // 0x004449e8
    virtual ~DynamicVectorClass_AbstractClass();

    // ---- data members (offsets from ctor 0x0044492a) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned char pad_000[0xc];
    unsigned long m_00c;  // /* ? */
    unsigned long m_010;  // /* ? */
    unsigned long m_014;  // /* ? */
    unsigned long m_018;  // /* ? */
    unsigned char pad_01c[0x10];
    unsigned long m_02c;  // init 0x69aa58
    unsigned char pad_030[0xc];
    unsigned long m_03c;  // zero-initialised
    unsigned long m_040;  // init 0xa
    unsigned long m_044;  // /* ? */
    unsigned long m_048;  // /* ? */
    unsigned char pad_04c[0xc];
    unsigned long m_058;  // /* ? */
    unsigned char pad_05c[0xc];
    unsigned long m_068;  // /* ? */
    unsigned char pad_06c[0x28];
    unsigned long m_094;  // /* ? */
    unsigned long m_098;  // /* ? */
    unsigned char pad_09c[0x8];
    unsigned long m_0a4;  // /* ? */
    unsigned long m_0a8;  // /* ? */
    unsigned long m_0ac;  // /* ? */
    unsigned long m_0b0;  // /* ? */
};
