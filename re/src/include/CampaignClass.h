#pragma once
// ==========================================================================
// CampaignClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class CampaignClass
// vtable       : 0x0069ab60  (27 slots)
// constructor  : 0x00446670
// vtable store : 0x00446684  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "ObjectTypeClass.h"

class CampaignClass : public AbstractTypeClass
{
public:
    // ---- vtable 0x0069ab60 (27 slots) ----
    virtual void QueryInterface(int a0, int a1, int a2);  // [0] 0x00405790  // __stdcall, returns int
    virtual void Init(int a0);  // [1] 0x00405830  // __stdcall, returns int
    virtual void Shutdown(int a0);  // [2] 0x00405840  // __stdcall, returns int
    virtual void vfunc_3(int a0, int a1);  // [3] 0x004469e0  // __stdcall, returns int
    virtual void Release(int a0);  // [4] 0x00405990  // __stdcall
    virtual void vfunc_5(int a0, int a1);  // [5] 0x00446a70  // __stdcall, returns int
    virtual void vfunc_6(int a0, int a1, int a2);  // [6] 0x00446ab0  // __stdcall, returns int
    virtual void GetType(int a0, int a1);  // [7] 0x00405910  // __stdcall, returns int
    virtual void Set_ID();  // [9] 0x004059b0  // __cdecl
    virtual void Get_RTTI(int a0, int a1);  // [10] 0x004059c0  // __stdcall
    virtual void What_Am_I();  // [11] 0x00446ae0  // __cdecl, returns int
    virtual void Get_Heap();  // [12] 0x00446ad0  // __cdecl, returns int
    virtual void Load(int a0);  // [13] 0x00446a20  // __thiscall
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
    virtual void Fetch_ID(int a0);  // [25] 0x004467d0  // __thiscall  // refs "FinalMovie"
    virtual void Set_ID(int a0);  // [26] 0x00405fb0  // __thiscall  // refs "Name"

    CampaignClass();  // 0x00446670
    // The destructor is declared non-virtual here: slot 0 holds a stub
    // shared with 66 other classes (0x00405790), not the destructor.
    ~CampaignClass();

    // ---- data members (offsets from ctor 0x00446670) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    // vtable-pointer stores are omitted: the compiler supplies the vptr.
    // The machine block begins at +0x2e: the compiled end of the
    // base sub-object region. Evidence: AbstractTypeClass at +0x0 (ends +0x2e).
    // 2 recovered offset(s) below +0x2e lie inside the base
    // sub-object region and are NOT declared here: they are declared and
    // compiler-checked in the base's own header -- +0x0, +0x4
    unsigned char pad_02e[0x32];
    unsigned int m_060;  // +0x060 /* ? */; 4-byte store: mov dword ptr [ebx + 0x60], esi
    unsigned char m_064;  // +0x064 zero-initialised; 1-byte store: mov byte ptr [ebx + 0x64], 0
    unsigned char pad_065[0x1ff];
    unsigned int m_264;  // +0x264 /* ? */; 4-byte store: mov dword ptr [ebx + 0x264], esi
};
