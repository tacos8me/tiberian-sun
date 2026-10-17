#pragma once
// ==========================================================================
// OverlayTypeClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class OverlayTypeClass
// vtable       : 0x006a2d58  (41 slots)
// constructor  : 0x00576bf0
// vtable store : 0x00576c81  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "ObjectTypeClass.h"

class OverlayTypeClass : public ObjectTypeClass
{
public:
    // ---- vtable 0x006a2d58 (41 slots) ----
    virtual void QueryInterface(int a0, int a1, int a2);  // [0] 0x00405790  // __stdcall, returns int
    virtual void AddRef(int a0);  // [1] 0x00405830  // __stdcall, returns int
    virtual void Release(int a0);  // [2] 0x00405840  // __stdcall, returns int
    virtual void vfunc_3(int a0, int a1);  // [3] 0x00577500  // __stdcall, returns int
    virtual void IsDirty(int a0);  // [4] 0x00405990  // __stdcall, returns ?
    virtual void vfunc_5(int a0, int a1);  // [5] 0x005773f0  // returns void*
    virtual void vfunc_6(int a0, int a1, int a2);  // [6] 0x005774e0  // __stdcall, returns int
    virtual void GetSizeMax(int a0, int a1);  // [7] 0x00572ba0  // __stdcall, returns int
    virtual void Init();  // [9] 0x004059b0  // __cdecl, returns void
    virtual void Detach(int a0, int a1);  // [10] 0x004059c0  // __stdcall, returns void
    virtual void What_Am_I();  // [11] 0x00577820  // __cdecl, returns int
    virtual void Fetch_Object_Size();  // [12] 0x00577830  // __cdecl, returns int
    virtual void Compute_CRC(int a0);  // [13] 0x00577350  // __thiscall, returns char
    virtual void Owner();  // [14] 0x004059d0  // __cdecl, returns int
    virtual void Owner_HouseClass();  // [15] 0x004059e0  // __cdecl, returns int
    virtual void Fetch_Heap_ID();  // [16] 0x00577840  // __thiscall, returns ?
    virtual void Is_Inactive();  // [17] 0x00405970  // __cdecl, returns char
    virtual void Center_Coord(int a0);  // [18] 0x00405a00  // __stdcall, returns ?
    virtual void Destination_Coord(int a0);  // [19] 0x00405a30  // __thiscall, returns ?
    virtual void On_Ground();  // [20] 0x00405a60  // __cdecl, returns int
    virtual void In_Air();  // [21] 0x00405a70  // __cdecl, returns int
    virtual void As_Coord(int a0);  // [22] 0x00405a80  // __thiscall, returns ?
    virtual void AI();  // [23] 0x00405ab0  // __cdecl, returns void
    virtual void Init_Theater(int a0);  // [24] 0x00406010  // __stdcall, returns void
    virtual void Read_INI(int a0);  // [25] 0x00577090  // __thiscall, returns void*  // refs "Land"
    virtual void Write_INI(int a0);  // [26] 0x00405fb0  // __thiscall, returns char  // refs "Name"
    virtual void Coord_Fixup(int a0, int a1);  // [27] 0x00577330  // __stdcall, returns ?
    virtual void Get_Ownable();  // [28] 0x00418f40  // __cdecl, returns int
    virtual void Max_Pips();  // [29] 0x005715e0  // __cdecl, returns int
    virtual void Pixel_Dimensions(int a0);  // [30] 0x005715f0  // __stdcall, returns ?
    virtual void Lepton_Dimensions(int a0);  // [31] 0x00571610  // __stdcall, returns ?
    virtual void Create_And_Place(int a0, int a1);  // [32] 0x00576eb0  // __thiscall, returns char
    virtual void Cost_Of(int a0);  // [33] 0x00571640  // __stdcall, returns int
    virtual void Time_To_Build();  // [34] 0x00571650  // __cdecl, returns int
    virtual void Create_One_Of(int a0);  // [35] 0x00576ee0  // __thiscall, returns void
    virtual void Occupy_List(int a0);  // [36] 0x00576e40  // __stdcall, returns void*
    virtual void Who_Can_Build_Me(int a0, int a1, int a2, int a3);  // [37] 0x00571710  // __thiscall, returns ?
    virtual void Get_Cameo_Data();  // [38] 0x00571660  // __cdecl, returns int
    virtual void Get_Image_Data();  // [39] 0x00577720  // __thiscall, returns void*
    virtual void vfunc_40(int a0, int a1);  // [40] 0x00576f10  // returns ?

    OverlayTypeClass();  // 0x00576bf0
    // The destructor is declared non-virtual here: slot 0 holds a stub
    // shared with 66 other classes (0x00405790), not the destructor.
    ~OverlayTypeClass();

    // ---- data members (offsets from ctor 0x00576bf0) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    // vtable-pointer stores are omitted: the compiler supplies the vptr.
    // The machine block begins at +0x124: the compiled end of the
    // base sub-object region. Evidence: ObjectTypeClass at +0x0 (ends +0x124).
    // 5 recovered offset(s) below +0x124 lie inside the base
    // sub-object region and are NOT declared here: they are declared and
    // compiler-checked in the base's own header -- +0x0, +0xc8, +0xc9, +0xcb, +0xce
    unsigned int m_124;  // +0x124 init 0xffffffff; 4-byte store: mov dword ptr [esi + 0x124], 0xffffffff
    unsigned int m_128;  // +0x128 /* ? */; 4-byte store: mov dword ptr [esi + 0x128], ebx
    unsigned int m_12c;  // +0x12c /* ? */; 4-byte store: mov dword ptr [esi + 0x12c], ebx
    unsigned int m_130;  // +0x130 /* ? */; 4-byte store: mov dword ptr [esi + 0x130], eax
    unsigned int m_134;  // +0x134 /* ? */; 4-byte store: mov dword ptr [esi + 0x134], eax
    unsigned char Wall;  // +0x138 (INI key); 1-byte store: mov byte ptr [esi + 0x138], bl
    unsigned char High;  // +0x139 (INI key); 1-byte store: mov byte ptr [esi + 0x139], bl
    unsigned char m_13a;  // +0x13a /* ? */; 1-byte store: mov byte ptr [esi + 0x13a], bl
    unsigned char Crate;  // +0x13b (INI key); 1-byte store: mov byte ptr [esi + 0x13b], bl
    unsigned char CrateTrigger;  // +0x13c (INI key); 1-byte store: mov byte ptr [esi + 0x13c], bl
    unsigned char m_13d;  // +0x13d /* ? */; 1-byte store: mov byte ptr [esi + 0x13d], al
    unsigned char m_13e;  // +0x13e /* ? */; 1-byte store: mov byte ptr [esi + 0x13e], bl
    unsigned char m_13f;  // +0x13f /* ? */; 1-byte store: mov byte ptr [esi + 0x13f], bl
    unsigned char m_140;  // +0x140 /* ? */; 1-byte store: mov byte ptr [esi + 0x140], bl
    unsigned char m_141;  // +0x141 /* ? */; 1-byte store: mov byte ptr [esi + 0x141], bl
    unsigned char m_142;  // +0x142 /* ? */; 1-byte store: mov byte ptr [esi + 0x142], bl
    unsigned char Overrides;  // +0x143 (INI key); 1-byte store: mov byte ptr [esi + 0x143], bl
    unsigned char m_144;  // +0x144 /* ? */; 1-byte store: mov byte ptr [esi + 0x144], al
    unsigned char m_145;  // +0x145 /* ? */; 1-byte store: mov byte ptr [esi + 0x145], bl
};
