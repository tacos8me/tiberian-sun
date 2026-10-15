#pragma once
// ==========================================================================
// VectorClass_struct_SubzoneTrackingStruct.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class VectorClass<struct SubzoneTrackingStruct>
// vtable       : 0x006a1af4  (6 slots)
// constructor  : 0x00501d47
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class VectorClass_struct_SubzoneTrackingStruct
{
public:
    // ---- vtable 0x006a1af4 (6 slots) ----
    virtual void vfunc_1(int a0);  // [1] 0x00520eb0  // __stdcall
    virtual void vfunc_2(int a0, int a1, int a2, int a3, int a4, int a5);  // [2] 0x00520f00  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x005211a0  // __stdcall, returns int
    virtual void vfunc_5(int a0);  // [5] 0x005211f0  // __stdcall, returns int

    VectorClass_struct_SubzoneTrackingStruct();  // 0x00501d47
    virtual ~VectorClass_struct_SubzoneTrackingStruct();

    // ---- data members (offsets from ctor 0x00501d2e) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x6a1af4
    unsigned long m_004;  // /* ? */
    unsigned long m_008;  // /* ? */
    unsigned char pad_00c[0x1];
    unsigned long m_00d;  // /* ? */
};
