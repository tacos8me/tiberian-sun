#pragma once
// ==========================================================================
// AlphaShapeClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class AlphaShapeClass
// vtable       : 0x006997ec  (24 slots)
// constructor  : 0x00411a0e
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "AbstractClass.h"

class AlphaShapeClass : public AbstractClass
{
public:
    // ---- vtable 0x006997ec (24 slots) ----
    virtual void vfunc_0(int a0, int a1, int a2);  // [0] 0x00405790  // __stdcall, returns int
    virtual void vfunc_1(int a0);  // [1] 0x00405830  // __stdcall, returns int
    virtual void vfunc_2(int a0);  // [2] 0x00405840  // __stdcall, returns int
    virtual void vfunc_3(int a0, int a1);  // [3] 0x00411da0  // __stdcall, returns int
    virtual void vfunc_4(int a0);  // [4] 0x00405990  // __stdcall
    virtual void vfunc_5(int a0, int a1);  // [5] 0x00411e40  // __stdcall
    virtual void vfunc_6(int a0, int a1, int a2);  // [6] 0x00411e90  // __stdcall
    virtual void vfunc_7(int a0, int a1);  // [7] 0x00405910  // __stdcall, returns int
    virtual void vfunc_8(int a0);  // [8] 0x00412790  // __thiscall
    virtual void vfunc_9();  // [9] 0x004059b0  // __cdecl
    virtual void vfunc_10(int a0, int a1);  // [10] 0x00411ec0  // __stdcall
    virtual void vfunc_11();  // [11] 0x00411de0  // __cdecl, returns int
    virtual void vfunc_12();  // [12] 0x00411df0  // __cdecl, returns int
    virtual void vfunc_13(int a0);  // [13] 0x00411e00  // __thiscall
    virtual void vfunc_14();  // [14] 0x004059d0  // __cdecl
    virtual void vfunc_15();  // [15] 0x004059e0  // __cdecl, returns int
    virtual void vfunc_16();  // [16] 0x004059f0  // __cdecl, returns int
    virtual void vfunc_17();  // [17] 0x00405970  // __cdecl
    virtual void vfunc_18(int a0);  // [18] 0x00405a00  // __stdcall
    virtual void vfunc_19(int a0);  // [19] 0x00405a30  // __stdcall
    virtual void vfunc_20();  // [20] 0x00405a60  // __cdecl
    virtual void vfunc_21();  // [21] 0x00405a70  // __cdecl
    virtual void vfunc_22(int a0);  // [22] 0x00405a80  // __stdcall
    virtual void vfunc_23();  // [23] 0x00405ab0  // __cdecl

    AlphaShapeClass();  // 0x00411a0e
    virtual ~AlphaShapeClass();

    // ---- data members (offsets from ctor 0x004119eb) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x6997ec
    unsigned long m_004;  // init 0x6997d0
    unsigned char pad_008[0xc];
    unsigned long m_014;  // /* ? */
    unsigned long m_018;  // /* ? */
    unsigned char pad_01c[0xc];
    unsigned long m_028;  // zero-initialised
    unsigned long m_02c;  // zero-initialised
};
