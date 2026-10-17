#pragma once
// ==========================================================================
// SideClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class SideClass
// vtable       : 0x006a4300  (27 slots)
// constructor  : 0x005d7e60
// vtable store : 0x005d7e94  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "ObjectTypeClass.h"

class SideClass : public AbstractTypeClass
{
public:
    // ---- vtable 0x006a4300 (27 slots) ----
    virtual void QueryInterface(int a0, int a1, int a2);  // [0] 0x00405790  // __stdcall, returns int
    virtual void AddRef(int a0);  // [1] 0x00405830  // __stdcall, returns int
    virtual void Release(int a0);  // [2] 0x00405840  // __stdcall, returns int
    virtual void vfunc_3(int a0, int a1);  // [3] 0x005d8000  // __stdcall, returns int
    virtual void IsDirty(int a0);  // [4] 0x00405990  // __stdcall
    virtual void vfunc_5(int a0, int a1);  // [5] 0x005d8040  // __stdcall
    virtual void vfunc_6(int a0, int a1, int a2);  // [6] 0x005d8130  // __stdcall
    virtual void GetSizeMax(int a0, int a1);  // [7] 0x00405910  // __stdcall, returns int
    virtual void Init();  // [9] 0x004059b0  // __cdecl
    virtual void Detach(int a0, int a1);  // [10] 0x004059c0  // __stdcall
    virtual void What_Am_I();  // [11] 0x005d81a0  // __cdecl, returns int
    virtual void Fetch_Object_Size();  // [12] 0x005d8190  // __cdecl, returns int
    virtual void Compute_CRC(int a0);  // [13] 0x005d7fe0  // __thiscall
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
    virtual void Init_Theater(int a0);  // [24] 0x00406010  // __stdcall
    virtual void Read_INI(int a0, int a1, int a2, int a3, int a4);  // [25] 0x00405f20  // __thiscall  // refs "Name"
    virtual void Write_INI(int a0);  // [26] 0x00405fb0  // __thiscall  // refs "Name"

    SideClass();  // 0x005d7e60
    // The destructor is declared non-virtual here: slot 0 holds a stub
    // shared with 66 other classes (0x00405790), not the destructor.
    ~SideClass();

    // ---- data members (offsets from ctor 0x005d7e60) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    // vtable-pointer stores are omitted: the compiler supplies the vptr.
    // The machine block begins at +0x2e: the compiled end of the
    // base sub-object region. Evidence: AbstractTypeClass at +0x0 (ends +0x2e).
    // 1 recovered offset(s) below +0x2e lie inside the base
    // sub-object region and are NOT declared here: they are declared and
    // compiler-checked in the base's own header -- +0x0
    unsigned char pad_02e[0x36];
    unsigned int m_064;  // +0x064 /* ? */; 4-byte store: mov dword ptr [esi + 0x64], ebx
    unsigned int m_068;  // +0x068 /* ? */; 4-byte store: mov dword ptr [esi + 0x68], ebx
    unsigned char m_06c;  // +0x06c init 0x1; 1-byte store: mov byte ptr [esi + 0x6c], 1
    unsigned char m_06d;  // +0x06d /* ? */; 1-byte store: mov byte ptr [esi + 0x6d], bl
    unsigned char pad_06e[0x2];
    unsigned int m_070;  // +0x070 /* ? */; 4-byte store: mov dword ptr [esi + 0x70], ebx
    unsigned int m_074;  // +0x074 init 0xa; 4-byte store: mov dword ptr [esi + 0x74], 0xa
};
