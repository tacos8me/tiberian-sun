#pragma once
// ==========================================================================
// UDPInterfaceClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class UDPInterfaceClass
// vtable       : 0x006a7a94  (21 slots)
// constructor  : 0x0066f6d0
// vtable store : 0x0066f73c  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "WinsockInterfaceClass.h"

class UDPInterfaceClass : public WinsockInterfaceClass
{
public:
    // ---- vtable 0x006a7a94 (21 slots) ----
    virtual void vfunc_1();  // [1] 0x0066eb20  // __thiscall
    virtual void vfunc_2(int a0, int a1, int a2, int a3);  // [2] 0x0066ed50  // __thiscall, returns int
    virtual void vfunc_3(int a0, int a1, int a2, int a3);  // [3] 0x0066ede0  // __thiscall
    virtual void vfunc_4(int a0, int a1);  // [4] 0x0066fbe0  // __stdcall
    virtual void vfunc_5();  // [5] 0x0066ebb0  // __thiscall
    virtual void vfunc_6();  // [6] 0x0066ec00  // __thiscall
    virtual void vfunc_7();  // [7] 0x0066eb40  // __thiscall
    virtual void vfunc_8();  // [8] 0x0066eb90  // __thiscall
    virtual void vfunc_9(int a0);  // [9] 0x0066ef50  // __stdcall
    virtual void vfunc_10();  // [10] 0x0066efa0  // __thiscall
    virtual void vfunc_11(int a0);  // [11] 0x0066f8f0  // __thiscall
    virtual void vfunc_12();  // [12] 0x0066f890  // __thiscall
    virtual void vfunc_13();  // [13] 0x0066fef0  // __cdecl, returns int
    virtual void vfunc_14();  // [14] 0x0066ff00  // __cdecl, returns int
    virtual void vfunc_15(int a0);  // [15] 0x0066f990  // __thiscall
    virtual void vfunc_16(int a0, int a1, int a2, int a3);  // [16] 0x0066fcd0  // __thiscall, returns int
    virtual void vfunc_17(int a0, int a1);  // [17] 0x0066f030  // __stdcall
    virtual void vfunc_18();  // [18] 0x0066ff10  // __cdecl
    virtual void vfunc_19(int a0);  // [19] 0x0066ff20  // __stdcall
    virtual void vfunc_20(int a0);  // [20] 0x0066e9a0  // __stdcall

    UDPInterfaceClass();  // 0x0066f6d0
    virtual ~UDPInterfaceClass();

    // ---- data members (offsets from ctor 0x0066f6c2) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x6a7a94
    unsigned char pad_004[0x444];
    unsigned long m_448;  // /* ? */
    unsigned long m_44c;  // /* ? */
    unsigned long m_450;  // /* ? */
    unsigned long m_454;  // init 0x1
    unsigned long m_458;  // /* ? */
    unsigned long m_45c;  // /* ? */
    unsigned long m_460;  // /* ? */
    unsigned long m_464;  // /* ? */
    unsigned long m_468;  // /* ? */
    unsigned long m_46c;  // init 0x1
    unsigned long m_470;  // /* ? */
    unsigned long m_474;  // /* ? */
};
