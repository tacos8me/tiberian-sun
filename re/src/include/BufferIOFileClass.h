#pragma once
// ==========================================================================
// BufferIOFileClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class BufferIOFileClass
// vtable       : 0x00699f40  (17 slots)
// constructor  : 0x0041ff40
// vtable store : 0x00420099  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "RawFileClass.h"

class BufferIOFileClass : public RawFileClass
{
public:
    // ---- vtable 0x00699f40 (17 slots) ----
    virtual void vfunc_1();  // [1] 0x00419090  // __cdecl
    virtual void vfunc_2(int a0);  // [2] 0x00420490  // __thiscall
    virtual void vfunc_3();  // [3] 0x005a6850  // __thiscall, returns int
    virtual void vfunc_4();  // [4] 0x005a6890  // __thiscall, returns int
    virtual void vfunc_5(int a0);  // [5] 0x00420520  // __stdcall
    virtual void vfunc_6();  // [6] 0x00420540  // __cdecl
    virtual void vfunc_7(int a0);  // [7] 0x00420580  // __thiscall
    virtual void vfunc_8(int a0, int a1);  // [8] 0x00420560  // __thiscall
    virtual void vfunc_9(int a0, int a1);  // [9] 0x004208c0  // __thiscall
    virtual void vfunc_10(int a0, int a1);  // [10] 0x00420ad0  // __thiscall
    virtual void vfunc_11();  // [11] 0x00420bc0  // __cdecl
    virtual void vfunc_12(int a0, int a1);  // [12] 0x00420670  // __thiscall
    virtual void vfunc_13();  // [13] 0x00420be0  // __thiscall
    virtual void vfunc_14(int a0, int a1);  // [14] 0x005a68f0  // __cdecl
    virtual void vfunc_15(int a0);  // [15] 0x005a6940  // __thiscall
    virtual void vfunc_16(int a0, int a1, int a2);  // [16] 0x005a6190  // __stdcall

    BufferIOFileClass();  // 0x0041ff40
    virtual ~BufferIOFileClass();

    // ---- data members (offsets from ctor 0x00420035) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x699f40
    unsigned long m_004;  // init 0x1
    unsigned long m_008;  // /* ? */
    unsigned long m_00c;  // /* ? */
    unsigned long m_010;  // /* ? */
    unsigned long m_014;  // /* ? */
    unsigned long m_018;  // /* ? */
    unsigned long m_01c;  // /* ? */
    unsigned long m_020;  // /* ? */
    unsigned long m_024;  // /* ? */
    unsigned long m_028;  // /* ? */
    unsigned long m_02c;  // /* ? */
    unsigned long m_030;  // /* ? */
    unsigned long m_034;  // /* ? */
    unsigned long m_038;  // /* ? */
    unsigned long m_03c;  // /* ? */
    unsigned long m_040;  // /* ? */
    unsigned long m_044;  // /* ? */
    unsigned long m_048;  // /* ? */
    unsigned long m_04c;  // /* ? */
};
