#pragma once
// ==========================================================================
// IPXInterfaceClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class IPXInterfaceClass
// vtable       : 0x006a79a4  (21 slots)
// constructor  : 0x0066e130
// vtable store : 0x0066e146  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "WinsockInterfaceClass.h"

class IPXInterfaceClass : public WinsockInterfaceClass
{
public:
    // ---- vtable 0x006a79a4 (21 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x0066e9d0  // __thiscall
    virtual void vfunc_1();  // [1] 0x0066eb20  // __thiscall
    virtual void vfunc_2(int a0, int a1, int a2, int a3);  // [2] 0x0066ed50  // __thiscall, returns int
    virtual void vfunc_3(int a0, int a1, int a2, int a3);  // [3] 0x0066ede0  // __thiscall
    virtual void vfunc_4(int a0, int a1);  // [4] 0x0066eea0  // __thiscall
    virtual void vfunc_5();  // [5] 0x0066ebb0  // __thiscall
    virtual void vfunc_6();  // [6] 0x0066ec00  // __thiscall
    virtual void vfunc_7();  // [7] 0x0066eb40  // __thiscall
    virtual void vfunc_8();  // [8] 0x0066eb90  // __thiscall
    virtual void vfunc_9(int a0);  // [9] 0x0066ef50  // __stdcall
    virtual void vfunc_10();  // [10] 0x0066efa0  // __thiscall
    virtual void vfunc_11(int a0);  // [11] 0x0066e960  // __stdcall
    virtual void vfunc_12();  // [12] 0x0066e970  // __cdecl
    virtual void vfunc_13();  // [13] 0x0066e9b0  // __cdecl, returns int
    virtual void vfunc_14();  // [14] 0x0066e9c0  // __cdecl, returns int
    virtual void vfunc_15(int a0);  // [15] 0x0066e3f0  // __thiscall
    virtual void vfunc_16(int a0, int a1, int a2, int a3);  // [16] 0x0066e6d0  // __thiscall, returns int
    virtual void vfunc_17(int a0, int a1);  // [17] 0x0066f030  // __stdcall
    virtual void vfunc_18();  // [18] 0x0066e980  // __cdecl, returns int
    virtual void vfunc_19(int a0);  // [19] 0x0066e990  // __stdcall, returns int
    virtual void vfunc_20(int a0);  // [20] 0x0066e9a0  // __stdcall

    IPXInterfaceClass();  // 0x0066e130
    virtual ~IPXInterfaceClass();

    // ---- data members (offsets from ctor 0x0066e130) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    // vtable-pointer stores are omitted: the compiler supplies the vptr.
    unsigned char pad_004[0x44e];
    unsigned int m_452;  // +0x452 /* ? */; 4-byte store: mov dword ptr [esi + 0x452], eax
};
