#pragma once
// ==========================================================================
// IsometricTileTypeClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class IsometricTileTypeClass
// vtable       : 0x006a12d8  (40 slots)
// constructor  : 0x004ea14e
// vtable store : 0x004ea1cb  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "ObjectTypeClass.h"

class IsometricTileTypeClass : public ObjectTypeClass
{
public:
    // ---- vtable 0x006a12d8 (40 slots) ----
    virtual void QueryInterface(int a0, int a1, int a2);  // [0] 0x00405790  // __stdcall, returns int
    virtual void AddRef(int a0);  // [1] 0x00405830  // __stdcall, returns int
    virtual void Release(int a0);  // [2] 0x00405840  // __stdcall, returns int
    virtual void vfunc_3(int a0, int a1);  // [3] 0x004ef540  // __stdcall, returns int
    virtual void IsDirty(int a0);  // [4] 0x00405990  // __stdcall, returns ?
    virtual void vfunc_5(int a0, int a1);  // [5] 0x004ef440  // __stdcall, returns int
    virtual void vfunc_6(int a0, int a1, int a2);  // [6] 0x004ef520  // __stdcall, returns int
    virtual void GetSizeMax(int a0, int a1);  // [7] 0x00572ba0  // __stdcall, returns int
    virtual void Init();  // [9] 0x004059b0  // __cdecl, returns void
    virtual void Detach(int a0, int a1);  // [10] 0x004ef580  // __thiscall, returns ?
    virtual void What_Am_I();  // [11] 0x004ef920  // __cdecl, returns int
    virtual void Fetch_Object_Size();  // [12] 0x004ef930  // __cdecl, returns int
    virtual void Compute_CRC(int a0);  // [13] 0x004ef330  // __thiscall, returns ?
    virtual void Owner();  // [14] 0x004059d0  // __cdecl, returns int
    virtual void Owner_HouseClass();  // [15] 0x004059e0  // __cdecl, returns int
    virtual void Fetch_Heap_ID();  // [16] 0x004ef940  // __thiscall, returns ?
    virtual void Is_Inactive();  // [17] 0x00405970  // __cdecl, returns char
    virtual void Center_Coord(int a0);  // [18] 0x00405a00  // __stdcall, returns ?
    virtual void Destination_Coord(int a0);  // [19] 0x00405a30  // __thiscall, returns ?
    virtual void On_Ground();  // [20] 0x00405a60  // __cdecl, returns int
    virtual void In_Air();  // [21] 0x00405a70  // __cdecl, returns int
    virtual void As_Coord(int a0);  // [22] 0x00405a80  // __thiscall, returns ?
    virtual void AI();  // [23] 0x00405ab0  // __cdecl, returns void
    virtual void Init_Theater(int a0);  // [24] 0x00406010  // __stdcall, returns void
    virtual void Read_INI(int a0);  // [25] 0x00572610  // __thiscall, returns char  // refs "Image"
    virtual void Write_INI(int a0);  // [26] 0x00405fb0  // __thiscall, returns char  // refs "Name"
    virtual void Coord_Fixup(int a0, int a1);  // [27] 0x004ef310  // __stdcall, returns ?
    virtual void Get_Ownable();  // [28] 0x00418f40  // __cdecl, returns int
    virtual void Max_Pips();  // [29] 0x005715e0  // __cdecl, returns int
    virtual void Pixel_Dimensions(int a0);  // [30] 0x005715f0  // __stdcall, returns ?
    virtual void Lepton_Dimensions(int a0);  // [31] 0x00571610  // __stdcall, returns ?
    virtual void Create_And_Place(int a0, int a1);  // [32] 0x004ef260  // __thiscall, returns char
    virtual void Cost_Of(int a0);  // [33] 0x00571640  // __stdcall, returns int
    virtual void Time_To_Build();  // [34] 0x00571650  // __cdecl, returns int
    virtual void Create_One_Of(int a0);  // [35] 0x004ef2a0  // __thiscall, returns char
    virtual void Occupy_List();  // [36] 0x004ea600  // __thiscall, returns ?
    virtual void Who_Can_Build_Me(int a0, int a1, int a2, int a3);  // [37] 0x00571710  // __thiscall, returns ?
    virtual void Get_Cameo_Data();  // [38] 0x00571660  // __cdecl, returns int
    virtual void Get_Image_Data();  // [39] 0x004ea590  // __thiscall, returns ?

    IsometricTileTypeClass();  // 0x004ea14e
    // The destructor is declared non-virtual here: slot 0 holds a stub
    // shared with 66 other classes (0x00405790), not the destructor.
    ~IsometricTileTypeClass();

    // ---- data members (offsets from ctor 0x004ea2f0) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    // vtable-pointer stores are omitted: the compiler supplies the vptr.
    // The machine block begins at +0x124: the compiled end of the
    // base sub-object region. Evidence: ObjectTypeClass at +0x0 (ends +0x124).
    // 2 recovered offset(s) below +0x124 lie inside the base
    // sub-object region and are NOT declared here: they are declared and
    // compiler-checked in the base's own header -- +0x0, +0x4
    unsigned char pad_124[0x14];
    unsigned int m_138;  // +0x138 /* ? */; 4-byte store: mov dword ptr [esi + 0x138], eax
    unsigned int m_13c;  // +0x13c /* ? */; 4-byte store: mov dword ptr [esi + 0x13c], eax
    unsigned char m_140;  // +0x140 init 0x1; 1-byte store: mov byte ptr [esi + 0x140], 1
    unsigned char m_141;  // +0x141 /* ? */; 1-byte store: mov byte ptr [esi + 0x141], al
    unsigned char pad_142[0x2];
    unsigned int m_144;  // +0x144 /* ? */; 4-byte store: mov dword ptr [esi + 0x144], eax
    unsigned int m_148;  // +0x148 init 0xa; 4-byte store: mov dword ptr [esi + 0x148], 0xa
};
