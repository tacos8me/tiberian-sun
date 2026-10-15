#pragma once
// ==========================================================================
// VectorClass_WinsockBufferType_WinsockInterfaceClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class VectorClass<WinsockBufferType*, WinsockInterfaceClass>
// vtable       : 0x006a7a70  (6 slots)
// constructor  : 0x0066ea9e
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class VectorClass_WinsockBufferType_WinsockInterfaceClass
{
public:
    // ---- vtable 0x006a7a70 (6 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x0066f400  // __thiscall
    virtual void vfunc_1(int a0);  // [1] 0x0066f1a0  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x0066f1e0  // __thiscall
    virtual void vfunc_3();  // [3] 0x0066f290  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x0066f2c0  // __stdcall, returns int
    virtual void vfunc_5(int a0);  // [5] 0x0066f300  // __stdcall, returns int

    VectorClass_WinsockBufferType_WinsockInterfaceClass();  // 0x0066ea9e
    virtual ~VectorClass_WinsockBufferType_WinsockInterfaceClass();

    // ---- data members (offsets from ctor 0x0066ea6e) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x6a79fc
    unsigned long m_004;  // init 0x6a7a70
    unsigned long m_008;  // /* ? */
    unsigned long m_00c;  // /* ? */
    unsigned char pad_010[0x1];
    unsigned long m_011;  // /* ? */
    unsigned char pad_015[0x7];
    unsigned long m_01c;  // init 0x6a7a70
    unsigned long m_020;  // /* ? */
    unsigned long m_024;  // /* ? */
    unsigned char pad_028[0x1];
    unsigned long m_029;  // /* ? */
    unsigned char pad_02d[0x7];
    unsigned long m_034;  // /* ? */
};
