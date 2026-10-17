#pragma once
// ==========================================================================
// TagTypeClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class TagTypeClass
// vtable       : 0x006a54b4  (27 slots)
// constructor  : 0x00603ee0
// vtable store : 0x00603f08  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "ObjectTypeClass.h"

class TagTypeClass : public AbstractTypeClass
{
public:
    // ---- vtable 0x006a54b4 (27 slots) ----
    virtual void QueryInterface(int a0, int a1, int a2);  // [0] 0x00405790  // __stdcall, returns int
    virtual void AddRef(int a0);  // [1] 0x00405830  // __stdcall, returns int
    virtual void Release(int a0);  // [2] 0x00405840  // __stdcall, returns int
    virtual void vfunc_3(int a0, int a1);  // [3] 0x00604670  // __stdcall, returns int
    virtual void IsDirty(int a0);  // [4] 0x00405990  // __stdcall, returns ?
    virtual void vfunc_5(int a0, int a1);  // [5] 0x006046d0  // __stdcall, returns int
    virtual void vfunc_6(int a0, int a1, int a2);  // [6] 0x00604720  // __stdcall, returns int
    virtual void GetSizeMax(int a0, int a1);  // [7] 0x00405910  // __stdcall, returns int
    virtual void Init();  // [9] 0x004059b0  // __cdecl, returns void
    virtual void Detach(int a0, int a1);  // [10] 0x00604180  // __thiscall, returns ?
    virtual void What_Am_I();  // [11] 0x00604740  // __cdecl, returns int
    virtual void Fetch_Object_Size();  // [12] 0x00604750  // __cdecl, returns int
    virtual void Compute_CRC(int a0);  // [13] 0x006046b0  // __thiscall, returns ?
    virtual void Owner();  // [14] 0x004059d0  // __cdecl, returns int
    virtual void Owner_HouseClass();  // [15] 0x004059e0  // __cdecl, returns int
    virtual void Fetch_Heap_ID();  // [16] 0x00604760  // __thiscall, returns ?
    virtual void Is_Inactive();  // [17] 0x00405970  // __cdecl, returns char
    virtual void Center_Coord(int a0);  // [18] 0x00405a00  // __stdcall, returns ?
    virtual void Destination_Coord(int a0);  // [19] 0x00405a30  // __thiscall, returns ?
    virtual void On_Ground();  // [20] 0x00405a60  // __cdecl, returns int
    virtual void In_Air();  // [21] 0x00405a70  // __cdecl, returns int
    virtual void As_Coord(int a0);  // [22] 0x00405a80  // __thiscall, returns ?
    virtual void AI();  // [23] 0x00405ab0  // __cdecl, returns void
    virtual void Init_Theater(int a0);  // [24] 0x00406010  // __stdcall, returns void
    virtual void Read_INI(int a0);  // [25] 0x006043a0  // __thiscall, returns char
    virtual void Write_INI(int a0);  // [26] 0x00604470  // __thiscall, returns char

    TagTypeClass();  // 0x00603ee0
    // The destructor is declared non-virtual here: slot 0 holds a stub
    // shared with 66 other classes (0x00405790), not the destructor.
    ~TagTypeClass();

    // ---- data members (offsets from ctor 0x00604000) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    // vtable-pointer stores are omitted: the compiler supplies the vptr.
    // The machine block begins at +0x2e: the compiled end of the
    // base sub-object region. Evidence: AbstractTypeClass at +0x0 (ends +0x2e).
    // 2 recovered offset(s) below +0x2e lie inside the base
    // sub-object region and are NOT declared here: they are declared and
    // compiler-checked in the base's own header -- +0x0, +0x4
    unsigned char pad_02e[0x3a];
    unsigned int m_068;  // +0x068 zero-initialised; 4-byte store: mov dword ptr [edi + 0x68], 0
};
