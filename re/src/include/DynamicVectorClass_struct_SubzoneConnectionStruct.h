#pragma once
// ==========================================================================
// DynamicVectorClass_struct_SubzoneConnectionStruct.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class DynamicVectorClass<struct SubzoneConnectionStruct>
// vtable       : 0x006a1b9c  (6 slots)
// constructor  : 0x0051aabd
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "VectorClass_struct_SubzoneConnectionStruct.h"

class DynamicVectorClass_struct_SubzoneConnectionStruct : public VectorClass_struct_SubzoneConnectionStruct
{
public:
    // ---- vtable 0x006a1b9c (6 slots) ----
    virtual void vfunc_1(int a0);  // [1] 0x004013d0  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x00520950  // __thiscall
    virtual void vfunc_3();  // [3] 0x005205d0  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x00520a90  // __stdcall
    virtual void vfunc_5(int a0);  // [5] 0x00520600  // __stdcall, returns int

    DynamicVectorClass_struct_SubzoneConnectionStruct();  // 0x0051aabd
    virtual ~DynamicVectorClass_struct_SubzoneConnectionStruct();

    // ---- data members (offsets from ctor 0x0051a8a5) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned char pad_000[0x10];
    unsigned long m_010;  // /* ? */
    unsigned long m_014;  // /* ? */
    unsigned long m_018;  // /* ? */
    unsigned long m_01c;  // /* ? */
    unsigned long m_020;  // /* ? */
    unsigned char pad_024[0x2];
    unsigned long m_026;  // /* ? */
    unsigned char pad_02a[0x2];
    unsigned long m_02c;  // /* ? */
    unsigned long m_030;  // /* ? */
    unsigned long m_034;  // /* ? */
    unsigned long m_038;  // /* ? */
    unsigned long m_03c;  // /* ? */
    unsigned long m_040;  // /* ? */
    unsigned long m_044;  // /* ? */
    unsigned long m_048;  // /* ? */
    unsigned long m_04c;  // /* ? */
    unsigned long m_050;  // /* ? */
    unsigned long m_054;  // /* ? */
    unsigned long m_058;  // init 0x698634
    unsigned long m_05c;  // zero-initialised
    unsigned long m_060;  // zero-initialised
    unsigned char pad_064[0x1];
    unsigned long m_065;  // zero-initialised
    unsigned char pad_069[0x3];
    unsigned long m_06c;  // init 0xa
    unsigned long m_070;  // /* ? */
    unsigned long m_074;  // /* ? */
    unsigned long m_078;  // /* ? */
    unsigned char pad_07c[0x4];
    unsigned long m_080;  // /* ? */
};
