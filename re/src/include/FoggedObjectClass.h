#pragma once
// ==========================================================================
// FoggedObjectClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class FoggedObjectClass
// vtable       : 0x0069e218  (25 slots)
// constructor  : 0x0049986f
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "AbstractClass.h"

class FoggedObjectClass : public AbstractClass
{
public:
    // ---- vtable 0x0069e218 (25 slots) ----
    virtual void vfunc_0(int a0, int a1, int a2);  // [0] 0x00405790  // __stdcall, returns int
    virtual void vfunc_1(int a0);  // [1] 0x00405830  // __stdcall, returns int
    virtual void vfunc_2(int a0);  // [2] 0x00405840  // __stdcall, returns int
    virtual void vfunc_3(int a0, int a1);  // [3] 0x0049b6d0  // __stdcall, returns int
    virtual void vfunc_4(int a0);  // [4] 0x00405990  // __stdcall
    virtual void vfunc_5(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9);  // [5] 0x0049b420
    virtual void vfunc_6(int a0, int a1, int a2);  // [6] 0x0049b3b0  // __stdcall
    virtual void vfunc_7(int a0, int a1);  // [7] 0x00405910  // __stdcall, returns int
    virtual void vfunc_8(int a0);  // [8] 0x0049b810  // __thiscall
    virtual void vfunc_9();  // [9] 0x004059b0  // __cdecl
    virtual void vfunc_10(int a0, int a1);  // [10] 0x004059c0  // __stdcall
    virtual void vfunc_11();  // [11] 0x0049b6b0  // __cdecl, returns int
    virtual void vfunc_12();  // [12] 0x0049b6c0  // __cdecl, returns int
    virtual void vfunc_13(int a0);  // [13] 0x0049b710  // __thiscall
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
    virtual void vfunc_24(int a0);  // [24] 0x0049b7d0  // __stdcall

    FoggedObjectClass();  // 0x0049986f
    virtual ~FoggedObjectClass();

    // ---- data members (offsets from ctor 0x0049993b) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69e218
    unsigned long m_004;  // init 0x69e1fc
    unsigned char pad_008[0xc];
    unsigned long m_014;  // /* ? */
    unsigned long m_018;  // /* ? */
    unsigned long m_01c;  // /* ? */
    unsigned long m_020;  // init 0x14
    unsigned char pad_024[0x10];
    unsigned long m_034;  // /* ? */
    unsigned char pad_038[0x8];
    unsigned long m_040;  // /* ? */
    unsigned long m_044;  // init 0xffffffff
    unsigned long m_048;  // /* ? */
    unsigned char pad_04c[0x18];
    unsigned long m_064;  // init 0x1
};
