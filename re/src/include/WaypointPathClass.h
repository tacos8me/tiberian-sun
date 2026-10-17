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
    virtual void AddRef(int a0);  // [1] 0x00405830  // __stdcall, returns int
    virtual void Release(int a0);  // [2] 0x00405840  // __stdcall, returns int
    virtual void GetClassID(int a0, int a1);  // [3] 0x00655cc0  // __stdcall, returns int
    virtual void IsDirty(int a0);  // [4] 0x00405990  // __stdcall, returns ?
    virtual void Load(int a0, int a1);  // [5] 0x00655d00  // __stdcall, returns int
    virtual void Save(int a0, int a1, int a2);  // [6] 0x00655e10  // __stdcall, returns int
    virtual void GetSizeMax(int a0, int a1);  // [7] 0x00405910  // __stdcall, returns int
    virtual void Init();  // [9] 0x004059b0  // __cdecl, returns void
    virtual void Detach(int a0, int a1);  // [10] 0x004059c0  // __stdcall, returns void
    virtual void What_Am_I();  // [11] 0x00655e90  // __cdecl, returns int
    virtual void Fetch_Object_Size();  // [12] 0x00655e80  // __cdecl, returns int
    virtual void Compute_CRC(int a0);  // [13] 0x00655c90  // __thiscall, returns ?
    virtual void Owner();  // [14] 0x004059d0  // __cdecl, returns int
    virtual void Owner_HouseClass();  // [15] 0x004059e0  // __cdecl, returns int
    virtual void Fetch_Heap_ID();  // [16] 0x004059f0  // __cdecl, returns int
    virtual void Is_Inactive();  // [17] 0x00405970  // __cdecl, returns char
    virtual void Center_Coord(int a0);  // [18] 0x00405a00  // __stdcall, returns ?
    virtual void Destination_Coord(int a0);  // [19] 0x00405a30  // __thiscall, returns ?
    virtual void On_Ground();  // [20] 0x00405a60  // __cdecl, returns int
    virtual void In_Air();  // [21] 0x00405a70  // __cdecl, returns int
    virtual void As_Coord(int a0);  // [22] 0x00405a80  // __thiscall, returns ?
    virtual void AI();  // [23] 0x00405ab0  // __cdecl, returns void

    WaypointPathClass();  // 0x006557e0
    // The destructor is declared non-virtual here: slot 0 holds a stub
    // shared with 66 other classes (0x00405790), not the destructor.
    ~WaypointPathClass();

    // ---- data members (offsets from ctor 0x006557e0) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    // vtable-pointer stores are omitted: the compiler supplies the vptr.
    // The machine block begins at +0x11: the compiled end of the
    // base sub-object region. Evidence: AbstractClass at +0x0 (ends +0x11).
    // 2 recovered offset(s) below +0x11 lie inside the base
    // sub-object region and are NOT declared here: they are declared and
    // compiler-checked in the base's own header -- +0x0, +0x4
    unsigned char pad_011[0x3];
    unsigned int m_014;  // +0x014 init 0xffffffff; 4-byte store: mov dword ptr [esi + 0x14], 0xffffffff
    unsigned char pad_018[0x4];
    unsigned int m_01c;  // +0x01c /* ? */; 4-byte store: mov dword ptr [esi + 0x1c], eax
    unsigned int m_020;  // +0x020 /* ? */; 4-byte store: mov dword ptr [esi + 0x20], eax
    unsigned char m_024;  // +0x024 init 0x1; 1-byte store: mov byte ptr [esi + 0x24], 1
    unsigned char m_025;  // +0x025 /* ? */; 1-byte store: mov byte ptr [esi + 0x25], al
    unsigned char pad_026[0x2];
    unsigned int m_028;  // +0x028 /* ? */; 4-byte store: mov dword ptr [esi + 0x28], eax
    unsigned int m_02c;  // +0x02c init 0xa; 4-byte store: mov dword ptr [esi + 0x2c], 0xa
};
