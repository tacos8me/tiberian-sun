#pragma once
// ==========================================================================
// DynamicVectorClass_struct_SubzoneTrackingStruct.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class DynamicVectorClass<struct SubzoneTrackingStruct>
// vtable       : 0x006a1aa0  (6 slots)
// constructor  : 0x00501975
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "VectorClass_struct_SubzoneTrackingStruct.h"

class DynamicVectorClass_struct_SubzoneTrackingStruct : public VectorClass_struct_SubzoneTrackingStruct
{
public:
    // ---- vtable 0x006a1aa0 (6 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x00522140  // __thiscall
    virtual void vfunc_1(int a0);  // [1] 0x00520eb0  // __stdcall
    virtual void vfunc_2(int a0, int a1, int a2, int a3);  // [2] 0x00521220  // __thiscall
    virtual void vfunc_3(int a0, int a1);  // [3] 0x00520670  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x005213d0  // __stdcall
    virtual void vfunc_5(int a0);  // [5] 0x005206f0  // __stdcall, returns int

    DynamicVectorClass_struct_SubzoneTrackingStruct();  // 0x00501975
    virtual ~DynamicVectorClass_struct_SubzoneTrackingStruct();

    // ---- data members (offsets from ctor 0x0050192a) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x6a1abc
    unsigned char pad_004[0xc];
    unsigned long m_010;  // /* ? */
    unsigned long m_014;  // /* ? */
};
