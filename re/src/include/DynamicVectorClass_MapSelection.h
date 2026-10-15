#pragma once
// ==========================================================================
// DynamicVectorClass_MapSelection.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class DynamicVectorClass<MapSelection*>
// vtable       : 0x006a25c4  (6 slots)
// constructor  : 0x0055a32a
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "VectorClass_MapSelection.h"

class DynamicVectorClass_MapSelection : public VectorClass_MapSelection
{
public:
    // ---- vtable 0x006a25c4 (6 slots) ----
    virtual void vfunc_1(int a0);  // [1] 0x0055b3d0  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x0055b550  // __thiscall, returns int
    virtual void vfunc_3();  // [3] 0x0055ad40  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x0055b600  // __stdcall
    virtual void vfunc_5(int a0);  // [5] 0x0055ad70  // __stdcall, returns int

    DynamicVectorClass_MapSelection();  // 0x0055a32a
    virtual ~DynamicVectorClass_MapSelection();

    // ---- data members (offsets from ctor 0x0055a2b5) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // /* ? */
    unsigned long m_004;  // /* ? */
    unsigned long m_008;  // /* ? */
    unsigned long m_00c;  // /* ? */
    unsigned long m_010;  // /* ? */
    unsigned long m_014;  // /* ? */
    unsigned char pad_018[0x8];
    unsigned long m_020;  // /* ? */
    unsigned char pad_024[0x30];
    unsigned long m_054;  // init 0x6a25c4
    unsigned long m_058;  // /* ? */
    unsigned long m_05c;  // /* ? */
    unsigned long m_060;  // init 0x1
    unsigned long m_064;  // /* ? */
    unsigned long m_068;  // /* ? */
};
