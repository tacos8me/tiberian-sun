#pragma once
// ==========================================================================
// TaskForceClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class TaskForceClass
// vtable       : 0x006a5550  (27 slots)
// constructor  : 0x006062d0
// vtable store : 0x006062f6  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "ObjectTypeClass.h"

class TaskForceClass : public AbstractTypeClass
{
public:
    // ---- vtable 0x006a5550 (27 slots) ----
    virtual void QueryInterface(int a0, int a1, int a2);  // [0] 0x00405790  // __stdcall, returns int
    virtual void Init(int a0);  // [1] 0x00405830  // __stdcall, returns int
    virtual void Shutdown(int a0);  // [2] 0x00405840  // __stdcall, returns int
    virtual void vfunc_3(int a0, int a1);  // [3] 0x00606c20  // __stdcall, returns int
    virtual void Release(int a0);  // [4] 0x00405990  // __stdcall
    virtual void vfunc_5(int a0, int a1);  // [5] 0x00606bc0  // __stdcall, returns int
    virtual void vfunc_6(int a0, int a1, int a2);  // [6] 0x00606ba0  // __stdcall, returns int
    virtual void GetType(int a0, int a1);  // [7] 0x00405910  // __stdcall, returns int
    virtual void Set_ID();  // [9] 0x004059b0  // __cdecl
    virtual void Get_RTTI(int a0, int a1);  // [10] 0x004059c0  // __stdcall
    virtual void vfunc_11();  // [11] 0x00606cc0  // __cdecl, returns int
    virtual void vfunc_12();  // [12] 0x00606cd0  // __cdecl, returns int
    virtual void vfunc_13(int a0);  // [13] 0x00606c60  // __thiscall
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
    virtual void vfunc_24(int a0);  // [24] 0x00406010  // __stdcall
    virtual void vfunc_25(int a0);  // [25] 0x006068d0  // __thiscall  // refs "Group"
    virtual void vfunc_26(int a0, int a1, int a2);  // [26] 0x00606990  // __thiscall

    TaskForceClass();  // 0x006062d0
    virtual ~TaskForceClass();

    // ---- data members (offsets from ctor 0x006062c7) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x6a5550
    unsigned long m_004;  // init 0x6a5534
    unsigned char pad_008[0x58];
    unsigned long m_060;  // init 0xffffffff
    unsigned long m_064;  // zero-initialised
    unsigned long m_068;  // zero-initialised
};
