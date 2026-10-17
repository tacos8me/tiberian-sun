#pragma once
// ==========================================================================
// FoggedObjectClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class FoggedObjectClass
// vtable       : 0x0069e218  (25 slots)
// constructor  : 0x00499840
// vtable store : 0x0049986f  (inside the ctor)
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
    virtual void QueryInterface(int a0, int a1, int a2);  // [0] 0x00405790  // __stdcall, returns int
    virtual void AddRef(int a0);  // [1] 0x00405830  // __stdcall, returns int
    virtual void Release(int a0);  // [2] 0x00405840  // __stdcall, returns int
    virtual void GetClassID(int a0, int a1);  // [3] 0x0049b6d0  // __stdcall, returns int
    virtual void IsDirty(int a0);  // [4] 0x00405990  // __stdcall
    virtual void Load(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9);  // [5] 0x0049b420
    virtual void Save(int a0, int a1, int a2);  // [6] 0x0049b3b0  // __stdcall
    virtual void GetSizeMax(int a0, int a1);  // [7] 0x00405910  // __stdcall, returns int
    virtual void Init();  // [9] 0x004059b0  // __cdecl
    virtual void Detach(int a0, int a1);  // [10] 0x004059c0  // __stdcall
    virtual void What_Am_I();  // [11] 0x0049b6b0  // __cdecl, returns int
    virtual void Fetch_Object_Size();  // [12] 0x0049b6c0  // __cdecl, returns int
    virtual void Compute_CRC(int a0);  // [13] 0x0049b710  // __thiscall
    virtual void Owner();  // [14] 0x004059d0  // __cdecl
    virtual void Owner_HouseClass();  // [15] 0x004059e0  // __cdecl, returns int
    virtual void Fetch_Heap_ID();  // [16] 0x004059f0  // __cdecl, returns int
    virtual void Is_Inactive();  // [17] 0x00405970  // __cdecl
    virtual void Center_Coord(int a0);  // [18] 0x00405a00  // __stdcall
    virtual void Destination_Coord(int a0);  // [19] 0x00405a30  // __stdcall
    virtual void On_Ground();  // [20] 0x00405a60  // __cdecl
    virtual void In_Air();  // [21] 0x00405a70  // __cdecl
    virtual void As_Coord(int a0);  // [22] 0x00405a80  // __stdcall
    virtual void AI();  // [23] 0x00405ab0  // __cdecl
    virtual void vfunc_24(int a0);  // [24] 0x0049b7d0  // __stdcall

    FoggedObjectClass();  // 0x00499840
    // The destructor is declared non-virtual here: slot 0 holds a stub
    // shared with 66 other classes (0x00405790), not the destructor.
    ~FoggedObjectClass();

    // ---- data members (offsets from ctor 0x00499840) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    // vtable-pointer stores are omitted: the compiler supplies the vptr.
    // The machine block begins at +0x11: the compiled end of the
    // base sub-object region. Evidence: AbstractClass at +0x0 (ends +0x11).
    // 2 recovered offset(s) below +0x11 lie inside the base
    // sub-object region and are NOT declared here: they are declared and
    // compiler-checked in the base's own header -- +0x0, +0x4
    unsigned char pad_011[0x3f];
    unsigned int m_050;  // +0x050 /* ? */; 4-byte store: mov dword ptr [esi + 0x50], eax
    unsigned int m_054;  // +0x054 /* ? */; 4-byte store: mov dword ptr [esi + 0x54], eax
    unsigned char m_058;  // +0x058 init 0x1; 1-byte store: mov byte ptr [esi + 0x58], 1
    unsigned char m_059;  // +0x059 /* ? */; 1-byte store: mov byte ptr [esi + 0x59], al
    unsigned char pad_05a[0x2];
    unsigned int m_05c;  // +0x05c /* ? */; 4-byte store: mov dword ptr [esi + 0x5c], eax
    unsigned int m_060;  // +0x060 init 0xa; 4-byte store: mov dword ptr [esi + 0x60], 0xa
};
