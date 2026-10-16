#pragma once
// ==========================================================================
// CDFileClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class CDFileClass
// vtable       : 0x00699b80  (17 slots)
// constructor  : 0x00418e88
// vtable store : 0x00418edc  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "BufferIOFileClass.h"

class CDFileClass : public BufferIOFileClass
{
public:
    // ---- vtable 0x00699b80 (17 slots) ----
    virtual void vfunc_1();  // [1] 0x00419090  // __cdecl
    virtual void vfunc_2(int a0);  // [2] 0x0044e320  // __thiscall
    virtual void vfunc_3();  // [3] 0x005a6850  // __thiscall, returns int
    virtual void vfunc_4();  // [4] 0x005a6890  // __thiscall, returns int
    virtual void vfunc_5(int a0);  // [5] 0x00420520  // __stdcall
    virtual void vfunc_6();  // [6] 0x00420540  // __cdecl
    virtual void vfunc_7(int a0);  // [7] 0x0044dfe0  // __stdcall
    virtual void vfunc_8(int a0, int a1);  // [8] 0x0044e420  // __thiscall
    virtual void vfunc_9(int a0, int a1);  // [9] 0x004208c0  // __thiscall
    virtual void vfunc_10(int a0, int a1);  // [10] 0x00420ad0  // __thiscall
    virtual void vfunc_11();  // [11] 0x00420bc0  // __cdecl
    virtual void vfunc_12(int a0, int a1);  // [12] 0x00420670  // __thiscall
    virtual void vfunc_13();  // [13] 0x00420be0  // __thiscall
    virtual void vfunc_14(int a0, int a1);  // [14] 0x005a68f0  // __cdecl
    virtual void vfunc_15(int a0);  // [15] 0x005a6940  // __thiscall
    virtual void vfunc_16(int a0, int a1, int a2);  // [16] 0x005a6190  // __stdcall

    CDFileClass();  // 0x00418e88
    virtual ~CDFileClass();

    // ---- data members (offsets from ctor 0x004190a0) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x699b80
};
