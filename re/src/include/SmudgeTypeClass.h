#pragma once
// ==========================================================================
// SmudgeTypeClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class SmudgeTypeClass
// vtable       : 0x006a4790  (41 slots)
// constructor  : 0x005e1580
// vtable store : 0x005e15bd  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "ObjectTypeClass.h"

class SmudgeTypeClass : public ObjectTypeClass
{
public:
    // ---- vtable 0x006a4790 (41 slots) ----
    virtual void QueryInterface(int a0, int a1, int a2);  // [0] 0x00405790  // __stdcall, returns int
    virtual void AddRef(int a0);  // [1] 0x00405830  // __stdcall, returns int
    virtual void Release(int a0);  // [2] 0x00405840  // __stdcall, returns int
    virtual void vfunc_3(int a0, int a1);  // [3] 0x005e1b40  // __stdcall, returns int
    virtual void IsDirty(int a0);  // [4] 0x00405990  // __stdcall, returns ?
    virtual void vfunc_5(int a0, int a1);  // [5] 0x005e1ad0  // __stdcall, returns int
    virtual void vfunc_6(int a0, int a1, int a2);  // [6] 0x005e1b20  // __stdcall, returns int
    virtual void GetSizeMax(int a0, int a1);  // [7] 0x00572ba0  // __stdcall, returns int
    virtual void Init();  // [9] 0x004059b0  // __cdecl, returns void
    virtual void Detach(int a0, int a1);  // [10] 0x004059c0  // __stdcall, returns void
    virtual void What_Am_I();  // [11] 0x005e2380  // __cdecl, returns int
    virtual void Fetch_Object_Size();  // [12] 0x005e2390  // __cdecl, returns int
    virtual void Compute_CRC(int a0);  // [13] 0x005e1a70  // __thiscall, returns ?
    virtual void Owner();  // [14] 0x004059d0  // __cdecl, returns int
    virtual void Owner_HouseClass();  // [15] 0x004059e0  // __cdecl, returns int
    virtual void Fetch_Heap_ID();  // [16] 0x005e23a0  // __thiscall, returns ?
    virtual void Is_Inactive();  // [17] 0x00405970  // __cdecl, returns char
    virtual void Center_Coord(int a0);  // [18] 0x00405a00  // __stdcall, returns ?
    virtual void Destination_Coord(int a0);  // [19] 0x00405a30  // __thiscall, returns ?
    virtual void On_Ground();  // [20] 0x00405a60  // __cdecl, returns int
    virtual void In_Air();  // [21] 0x00405a70  // __cdecl, returns int
    virtual void As_Coord(int a0);  // [22] 0x00405a80  // __thiscall, returns ?
    virtual void AI();  // [23] 0x00405ab0  // __cdecl, returns void
    virtual void Init_Theater(int a0);  // [24] 0x00406010  // __stdcall, returns void
    virtual void Read_INI(int a0);  // [25] 0x005e1960  // __thiscall, returns char  // refs "Crater"
    virtual void Write_INI(int a0);  // [26] 0x00405fb0  // __thiscall, returns char  // refs "Name"
    virtual void Coord_Fixup(int a0, int a1);  // [27] 0x0040f600  // __stdcall, returns ?
    virtual void Get_Ownable();  // [28] 0x00418f40  // __cdecl, returns int
    virtual void Max_Pips();  // [29] 0x005715e0  // __cdecl, returns int
    virtual void Pixel_Dimensions(int a0);  // [30] 0x005715f0  // __stdcall, returns ?
    virtual void Lepton_Dimensions(int a0);  // [31] 0x00571610  // __stdcall, returns ?
    virtual void Create_And_Place(int a0, int a1);  // [32] 0x005e17e0  // __thiscall, returns char
    virtual void Cost_Of(int a0);  // [33] 0x00571640  // __stdcall, returns int
    virtual void Time_To_Build();  // [34] 0x00571650  // __cdecl, returns int
    virtual void Create_One_Of(int a0);  // [35] 0x005e1850  // __thiscall, returns void
    virtual void Occupy_List(int a0);  // [36] 0x00571670  // __stdcall, returns void*
    virtual void Who_Can_Build_Me(int a0, int a1, int a2, int a3);  // [37] 0x00571710  // __thiscall, returns ?
    virtual void Get_Cameo_Data();  // [38] 0x00571660  // __cdecl, returns int
    virtual void Get_Image_Data();  // [39] 0x0040f620  // __thiscall, returns ?
    virtual void vfunc_40(int a0, int a1, int a2, int a3, int a4);  // [40] 0x005e1880  // __thiscall, returns ?

    SmudgeTypeClass();  // 0x005e1580
    // The destructor is declared non-virtual here: slot 0 holds a stub
    // shared with 66 other classes (0x00405790), not the destructor.
    ~SmudgeTypeClass();

    // ---- data members (offsets from ctor 0x005e1580) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    // vtable-pointer stores are omitted: the compiler supplies the vptr.
    // The machine block begins at +0x124: the compiled end of the
    // base sub-object region. Evidence: ObjectTypeClass at +0x0 (ends +0x124).
    // 7 recovered offset(s) below +0x124 lie inside the base
    // sub-object region and are NOT declared here: they are declared and
    // compiler-checked in the base's own header -- +0x0, +0xc8, +0xc9, +0xca, +0xcb, +0xcc, +0xce
    unsigned int m_124;  // +0x124 init 0xffffffff; 4-byte store: mov dword ptr [esi + 0x124], 0xffffffff
    unsigned int Width;  // +0x128 (INI key); 4-byte store: mov dword ptr [esi + 0x128], ebx
    unsigned int Height;  // +0x12c (INI key); 4-byte store: mov dword ptr [esi + 0x12c], ebx
    unsigned char Crater;  // +0x130 (INI key); 1-byte store: mov byte ptr [esi + 0x130], 0
    unsigned char m_131;  // +0x131 zero-initialised; 1-byte store: mov byte ptr [esi + 0x131], 0
};
