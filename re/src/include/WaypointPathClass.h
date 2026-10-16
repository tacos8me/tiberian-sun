#pragma once
// ==========================================================================
// WaypointPathClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class WaypointPathClass
// vtable       : 0x006a74f0  (24 slots)
// constructor  : 0x006557e0
// vtable store : 0x00655811  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "AbstractClass.h"

class WaypointPathClass : public AbstractClass
{
public:
    // ---- vtable 0x006a74f0 (24 slots) ----
    virtual void QueryInterface(int a0, int a1, int a2);  // [0] 0x00405790  // __stdcall, returns int
    virtual void Init(int a0);  // [1] 0x00405830  // __stdcall, returns int
    virtual void Shutdown(int a0);  // [2] 0x00405840  // __stdcall, returns int
    virtual void vfunc_3(int a0, int a1);  // [3] 0x00655cc0  // __stdcall, returns int
    virtual void Release(int a0);  // [4] 0x00405990  // __stdcall
    virtual void vfunc_5(int a0, int a1);  // [5] 0x00655d00  // __stdcall
    virtual void vfunc_6(int a0, int a1, int a2);  // [6] 0x00655e10  // __stdcall
    virtual void GetType(int a0, int a1);  // [7] 0x00405910  // __stdcall, returns int
    virtual void Set_ID();  // [9] 0x004059b0  // __cdecl
    virtual void Get_RTTI(int a0, int a1);  // [10] 0x004059c0  // __stdcall
    virtual void vfunc_11();  // [11] 0x00655e90  // __cdecl, returns int
    virtual void vfunc_12();  // [12] 0x00655e80  // __cdecl, returns int
    virtual void vfunc_13(int a0);  // [13] 0x00655c90  // __thiscall
    virtual void Save();  // [14] 0x004059d0  // __cdecl
    virtual void GetSizeMax();  // [15] 0x004059e0  // __cdecl, returns int
    virtual void Compute_CRC();  // [16] 0x004059f0  // __cdecl, returns int
    virtual void vfunc_17();  // [17] 0x00405970  // __cdecl
    virtual void vfunc_18(int a0);  // [18] 0x00405a00  // __stdcall
    virtual void vfunc_19(int a0);  // [19] 0x00405a30  // __stdcall
    virtual void vfunc_20();  // [20] 0x00405a60  // __cdecl
    virtual void vfunc_21();  // [21] 0x00405a70  // __cdecl
    virtual void vfunc_22(int a0);  // [22] 0x00405a80  // __stdcall
    virtual void vfunc_23();  // [23] 0x00405ab0  // __cdecl

    WaypointPathClass();  // 0x006557e0
    virtual ~WaypointPathClass();

    // ---- data members (offsets from ctor 0x006557e0) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x6a74f0
    unsigned long m_004;  // init 0x6a74d4
    unsigned char pad_008[0xc];
    unsigned long m_014;  // init 0xffffffff
    unsigned long m_018;  // init 0x6a7554
    unsigned long m_01c;  // /* ? */
    unsigned long m_020;  // /* ? */
    unsigned long m_024;  // init 0x1
    unsigned long m_028;  // /* ? */
    unsigned long m_02c;  // init 0xa
};
