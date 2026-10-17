#pragma once
// ==========================================================================
// TerrainTypeClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class TerrainTypeClass
// vtable       : 0x006a5fc8  (40 slots)
// constructor  : 0x00624f20
// vtable store : 0x00624f99  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "ObjectTypeClass.h"

class TerrainTypeClass : public ObjectTypeClass
{
public:
    // ---- vtable 0x006a5fc8 (40 slots) ----
    virtual void QueryInterface(int a0, int a1, int a2);  // [0] 0x00405790  // __stdcall, returns int
    virtual void AddRef(int a0);  // [1] 0x00405830  // __stdcall, returns int
    virtual void Release(int a0);  // [2] 0x00405840  // __stdcall, returns int
    virtual void vfunc_3(int a0, int a1);  // [3] 0x00625610  // __stdcall, returns int
    virtual void IsDirty(int a0);  // [4] 0x00405990  // __stdcall
    virtual void vfunc_5(int a0, int a1);  // [5] 0x00625590  // __stdcall, returns int
    virtual void vfunc_6(int a0, int a1, int a2);  // [6] 0x006255f0  // __stdcall, returns int
    virtual void GetSizeMax(int a0, int a1);  // [7] 0x00572ba0  // __stdcall, returns int
    virtual void Init();  // [9] 0x004059b0  // __cdecl
    virtual void Detach(int a0, int a1);  // [10] 0x004059c0  // __stdcall
    virtual void What_Am_I();  // [11] 0x006256e0  // __cdecl, returns int
    virtual void Fetch_Object_Size();  // [12] 0x006256f0  // __cdecl, returns int
    virtual void Compute_CRC(int a0);  // [13] 0x00625500  // __thiscall
    virtual void Owner();  // [14] 0x004059d0  // __cdecl
    virtual void Owner_HouseClass();  // [15] 0x004059e0  // __cdecl, returns int
    virtual void Fetch_Heap_ID();  // [16] 0x00625700  // __cdecl
    virtual void Is_Inactive();  // [17] 0x00405970  // __cdecl
    virtual void Center_Coord(int a0);  // [18] 0x00405a00  // __stdcall
    virtual void Destination_Coord(int a0);  // [19] 0x00405a30  // __stdcall
    virtual void On_Ground();  // [20] 0x00405a60  // __cdecl
    virtual void In_Air();  // [21] 0x00405a70  // __cdecl
    virtual void As_Coord(int a0);  // [22] 0x00405a80  // __stdcall
    virtual void AI();  // [23] 0x00405ab0  // __cdecl
    virtual void Init_Theater(int a0);  // [24] 0x00406010  // __stdcall
    virtual void Read_INI(int a0, int a1, int a2, int a3);  // [25] 0x00625280  // __thiscall  // refs "IsVeinhole"
    virtual void Write_INI(int a0);  // [26] 0x00405fb0  // __thiscall  // refs "Name"
    virtual void Coord_Fixup(int a0, int a1);  // [27] 0x00625490  // __stdcall
    virtual void Get_Ownable();  // [28] 0x00418f40  // __cdecl
    virtual void Max_Pips();  // [29] 0x005715e0  // __cdecl, returns int
    virtual void Pixel_Dimensions(int a0);  // [30] 0x005715f0  // __thiscall
    virtual void Lepton_Dimensions(int a0);  // [31] 0x00571610  // __thiscall
    virtual void Create_And_Place(int a0, int a1);  // [32] 0x006251c0  // __thiscall
    virtual void Cost_Of(int a0);  // [33] 0x00571640  // __stdcall, returns int
    virtual void Time_To_Build();  // [34] 0x00571650  // __cdecl, returns int
    virtual void Create_One_Of(int a0);  // [35] 0x006251f0  // __thiscall
    virtual void Occupy_List(int a0);  // [36] 0x00625220  // __stdcall, returns int
    virtual void Who_Can_Build_Me(int a0, int a1, int a2, int a3, int a4, int a5);  // [37] 0x00571710  // __thiscall
    virtual void Get_Cameo_Data();  // [38] 0x00571660  // __cdecl, returns int
    virtual void Get_Image_Data();  // [39] 0x0040f620  // __cdecl

    TerrainTypeClass();  // 0x00624f20
    // The destructor is declared non-virtual here: slot 0 holds a stub
    // shared with 66 other classes (0x00405790), not the destructor.
    ~TerrainTypeClass();

    // ---- data members (offsets from ctor 0x00624f20) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    // vtable-pointer stores are omitted: the compiler supplies the vptr.
    // The machine block begins at +0x124: the compiled end of the
    // base sub-object region. Evidence: ObjectTypeClass at +0x0 (ends +0x124).
    // 8 recovered offset(s) below +0x124 lie inside the base
    // sub-object region and are NOT declared here: they are declared and
    // compiler-checked in the base's own header -- +0x0, +0x64, +0x68, +0xc8, +0xc9, +0xca, +0xcb, +0xcd
    // 1 recovered offset(s) fall inside a wider declaration above
    // (the store width and the recovered table disagree): +0x12d
    unsigned int m_124;  // +0x124 init 0xffffffff; 4-byte store: mov dword ptr [esi + 0x124], 0xffffffff
    unsigned int m_128;  // +0x128 /* ? */; 4-byte store: mov dword ptr [esi + 0x128], ebx
    unsigned short m_12c;  // +0x12c /* ? */; 2-byte store: mov word ptr [esi + 0x12c], cx
    unsigned char m_12e;  // +0x12e /* ? */; 1-byte store: mov byte ptr [esi + 0x12e], bl
    unsigned char pad_12f[0x1];
    unsigned int m_130;  // +0x130 /* ? */; 4-byte store: mov dword ptr [esi + 0x130], ebx
    unsigned int m_134;  // +0x134 /* ? */; 4-byte store: mov dword ptr [esi + 0x134], ebx
    unsigned int m_138;  // +0x138 /* ? */; 4-byte store: mov dword ptr [esi + 0x138], eax
    unsigned int m_13c;  // +0x13c /* ? */; 4-byte store: mov dword ptr [esi + 0x13c], eax
    unsigned char WaterBound;  // +0x140 (INI key); 1-byte store: mov byte ptr [esi + 0x140], bl
    unsigned char m_141;  // +0x141 /* ? */; 1-byte store: mov byte ptr [esi + 0x141], bl
    unsigned char IsFlammable;  // +0x142 (INI key); 1-byte store: mov byte ptr [esi + 0x142], bl
    unsigned char m_143;  // +0x143 /* ? */; 1-byte store: mov byte ptr [esi + 0x143], bl
    unsigned char IsVeinhole;  // +0x144 (INI key); 1-byte store: mov byte ptr [esi + 0x144], bl
    unsigned char pad_145[0x3];
    unsigned int m_148;  // +0x148 /* ? */; 4-byte store: mov dword ptr [esi + 0x148], ebx
};
