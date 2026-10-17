#pragma once
// ==========================================================================
// ParticleSystemClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class ParticleSystemClass
// vtable       : 0x006a3084  (105 slots)
// constructor  : 0x0058df60
// vtable store : 0x0058e00d  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "ObjectClass.h"

class ParticleSystemClass : public ObjectClass
{
public:
    // ---- vtable 0x006a3084 (105 slots) ----
    virtual void QueryInterface(int a0, int a1, int a2);  // [0] 0x00405790  // __stdcall, returns int
    virtual void AddRef(int a0);  // [1] 0x00405830  // __stdcall, returns int
    virtual void Release(int a0);  // [2] 0x00405840  // __stdcall, returns int
    virtual void vfunc_3(int a0, int a1);  // [3] 0x00590310  // __stdcall, returns int
    virtual void IsDirty(int a0);  // [4] 0x00405990  // __stdcall, returns ?
    virtual void Load(int a0, int a1);  // [5] 0x005900b0  // __stdcall, returns int
    virtual void vfunc_6(int a0, int a1, int a2);  // [6] 0x00590200  // __stdcall, returns void*
    virtual void GetSizeMax(int a0, int a1);  // [7] 0x00405910  // __stdcall, returns int
    virtual void Init();  // [9] 0x004059b0  // __cdecl, returns void
    virtual void Detach(int a0, int a1);  // [10] 0x00590020  // __thiscall, returns int
    virtual void What_Am_I();  // [11] 0x00590370  // __cdecl, returns int
    virtual void Fetch_Object_Size();  // [12] 0x00590360  // __cdecl, returns int
    virtual void Compute_CRC(int a0);  // [13] 0x00590270  // __thiscall, returns ?
    virtual void Owner();  // [14] 0x004059d0  // __cdecl, returns int
    virtual void Owner_HouseClass();  // [15] 0x004059e0  // __cdecl, returns int
    virtual void Fetch_Heap_ID();  // [16] 0x004059f0  // __cdecl, returns int
    virtual void Is_Inactive();  // [17] 0x0058fff0  // __thiscall, returns int
    virtual void Center_Coord(int a0);  // [18] 0x00570960  // __thiscall, returns ?
    virtual void Destination_Coord(int a0);  // [19] 0x00405a30  // __thiscall, returns ?
    virtual void On_Ground();  // [20] 0x00570e80  // __thiscall, returns char
    virtual void In_Air();  // [21] 0x00570ea0  // __thiscall, returns char
    virtual void As_Coord(int a0);  // [22] 0x00405a80  // __thiscall, returns ?
    virtual void AI();  // [23] 0x0058ff00  // __thiscall, returns ?
    virtual void Is_Players_Army();  // [24] 0x004167b0  // __cdecl, returns int
    virtual void Visual_Character(int a0, int a1);  // [25] 0x004167c0  // __stdcall, returns int
    virtual void Get_Image_Data();  // [26] 0x0056e820  // __thiscall, returns int
    virtual void What_Action(int a0, int a1, int a2);  // [27] 0x0056eb50  // __stdcall, returns int
    virtual void What_Action(int a0, int a1);  // [28] 0x0056eb40  // __stdcall, returns int
    virtual void In_Which_Layer();  // [29] 0x00590010  // __cdecl, returns int
    virtual void Not_Underground();  // [30] 0x00570ef0  // __thiscall, returns ?
    virtual void Considered_Vehicle();  // [31] 0x004167d0  // __cdecl, returns int
    virtual void Techno_Type_Class();  // [32] 0x00570ec0  // __cdecl, returns int
    virtual void Class_Of();  // [33] 0x00590380  // __thiscall, returns ?
    virtual void Get_Ownable();  // [34] 0x0056eba0  // __cdecl, returns int
    virtual void Full_Name();  // [35] 0x004167e0  // __cdecl, returns void*
    virtual void Can_Repair();  // [36] 0x0056ebb0  // __cdecl, returns int
    virtual void Can_Demolish();  // [37] 0x0056ebc0  // __cdecl, returns int
    virtual void Can_Player_Fire();  // [38] 0x0056ebd0  // __cdecl, returns int
    virtual void Can_Player_Move();  // [39] 0x0056ebe0  // __cdecl, returns int
    virtual void Target_Coord(int a0);  // [40] 0x0040e5d0  // __thiscall, returns ?
    virtual void Docking_Coord(int a0);  // [41] 0x0040e600  // __thiscall, returns ?
    virtual void Render_Coord(int a0);  // [42] 0x0040e630  // __thiscall, returns ?
    virtual void Fire_Coord(int a0, int a1);  // [43] 0x004167f0  // __thiscall, returns ?
    virtual void Exit_Coord(int a0);  // [44] 0x0040e660  // __thiscall, returns ?
    virtual void Sort_Y();  // [45] 0x00570ed0  // __thiscall, returns ?
    virtual void Is_Moving_Onto_Bridge();  // [46] 0x00570db0  // __thiscall, returns char
    virtual void Occupies_Cells();  // [47] 0x00416830  // __cdecl, returns char
    virtual void Limbo();  // [48] 0x0056f6a0  // __thiscall, returns char
    virtual void Unlimbo(int a0);  // [49] 0x0056f810  // __thiscall, returns void*
    virtual void Detach_All(int a0);  // [50] 0x0056fb40  // __thiscall, returns ?
    virtual void Record_The_Kill(int a0);  // [51] 0x0056ebf0  // __stdcall, returns void
    virtual void Paradrop(int a0);  // [52] 0x005700c0  // __thiscall, returns ?
    virtual void Fall_From_Height();  // [53] 0x0056ea70  // __thiscall, returns ?
    virtual void Set_Occupy_Bit(int a0);  // [54] 0x00570480  // __stdcall, returns int
    virtual void Clear_Occupy_Bit(int a0);  // [55] 0x00570500  // __stdcall, returns int
    virtual void Delete_Me();  // [56] 0x00590350  // __thiscall, returns void
    virtual void Do_Shimmer();  // [57] 0x0056ec00  // __cdecl, returns void
    virtual void Exit_Object(int a0);  // [58] 0x0056ec10  // __stdcall, returns int
    virtual void Render(int a0, int a1, int a2);  // [59] 0x0056f480  // __thiscall, returns int
    virtual void Occupy_List();  // [60] 0x005701b0  // __thiscall, returns short
    virtual void Draw_Pre_Render(int a0, int a1);  // [61] 0x00416840  // __stdcall, returns void
    virtual void Draw_Post_Render(int a0, int a1);  // [62] 0x00416850  // __stdcall, returns void
    virtual void Draw_It();  // [63] 0x0058e500  // __thiscall, returns ?
    virtual void Editor_Draw_It(int a0, int a1);  // [64] 0x00570990  // __thiscall, returns ?
    virtual void Hidden();  // [65] 0x0056ec20  // __cdecl, returns void
    virtual void Look(int a0, int a1);  // [66] 0x0056ec30  // __stdcall, returns void
    virtual void Mark(int a0);  // [67] 0x00570000  // __thiscall, returns char
    virtual void Get_Visual_Rect(int a0);  // [68] 0x0056f000  // __thiscall, returns ?
    virtual void Get_Render_Rect(int a0);  // [69] 0x0056f160  // __thiscall, returns ?
    virtual void Draw_Radial_Indicator();  // [70] 0x0040e690  // __cdecl, returns void
    virtual void Mark_For_Redraw();  // [71] 0x0056f680  // __thiscall, returns char
    virtual void Active_Click_With(int a0, int a1, int a2);  // [72] 0x0056ec50  // __stdcall, returns int
    virtual void vfunc_73(int a0, int a1, int a2);  // [73] 0x0056ec40  // __stdcall, returns int
    virtual void Clicked_As_Target(int a0);  // [74] 0x0056ec60  // __stdcall, returns void
    virtual void Select();  // [75] 0x0056ee20  // __thiscall, returns int
    virtual void Unselect();  // [76] 0x0056eda0  // __thiscall, returns int
    virtual void In_Range(int a0, int a1);  // [77] 0x0056ec70  // __stdcall, returns int
    virtual void Weapon_Range(int a0);  // [78] 0x0056ec80  // __stdcall, returns int
    virtual void Take_Damage(int a0, int a1, int a2, int a3, int a4, int a5);  // [79] 0x0056fbe0  // __thiscall, returns int
    virtual void Scatter(int a0, int a1, int a2);  // [80] 0x0056ec90  // __stdcall, returns void
    virtual void Catch_Fire();  // [81] 0x0056eca0  // __cdecl, returns int
    virtual void Fire_Out();  // [82] 0x0056ecb0  // __cdecl, returns void
    virtual void Value();  // [83] 0x0056ecc0  // __cdecl, returns int
    virtual void Get_Mission();  // [84] 0x0056ecd0  // __cdecl, returns int
    virtual void Assign_Mission(int a0);  // [85] 0x00416860  // __stdcall, returns void
    virtual void Per_Cell_Process(int a0);  // [86] 0x00416870  // __stdcall, returns void
    virtual void Who_Can_Build_Me(int a0, int a1);  // [87] 0x00570240  // __thiscall, returns ?
    virtual void Receive_Message(int a0, int a1, int a2);  // [88] 0x0056fbc0  // __stdcall, returns int
    virtual void Revealed(int a0);  // [89] 0x005700b0  // __stdcall, returns char
    virtual void Repair(int a0);  // [90] 0x0056ece0  // __stdcall, returns void
    virtual void Sell_Back(int a0);  // [91] 0x0056ecf0  // __stdcall, returns void
    virtual void Set_Waypoint_Path(int a0, int a1);  // [92] 0x00570e70  // __stdcall, returns void
    virtual void Move(int a0);  // [93] 0x0056ed00  // __thiscall, returns ?
    virtual void Can_Enter_Cell(int a0, int a1, int a2, int a3, int a4);  // [94] 0x00416880  // __stdcall, returns int
    virtual void Can_Reach(int a0, int a1, int a2, int a3, int a4);  // [95] 0x00416890  // __stdcall, returns int
    virtual void Get_Coord(int a0);  // [96] 0x0040e6a0  // __thiscall, returns ?
    virtual void Set_Coord(int a0);  // [97] 0x00570cc0  // __thiscall, returns ?
    virtual void Get_Cell(int a0);  // [98] 0x0040e6c0  // __thiscall, returns ?
    virtual void Get_Cell_Ptr();  // [99] 0x00570ce0  // __thiscall, returns ?
    virtual void Get_Target_Cell(int a0);  // [100] 0x00570d00  // __thiscall, returns ?
    virtual void Get_Target_Cell_Ptr();  // [101] 0x00570d50  // __thiscall, returns ?
    virtual void Get_Height_AGL();  // [102] 0x00570370  // __thiscall, returns ?
    virtual void Set_Height_AGL(int a0);  // [103] 0x005703b0  // __thiscall, returns int
    virtual void Get_Height();  // [104] 0x00570360  // __thiscall, returns ?

    ParticleSystemClass();  // 0x0058df60
    // The destructor is declared non-virtual here: slot 0 holds a stub
    // shared with 66 other classes (0x00405790), not the destructor.
    ~ParticleSystemClass();

    // ---- data members (offsets from ctor 0x0058df60) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    // vtable-pointer stores are omitted: the compiler supplies the vptr.
    // The machine block begins at +0x4c: the compiled end of the
    // base sub-object region. Evidence: ObjectClass at +0x0 (ends +0x4c).
    // 1 recovered offset(s) below +0x4c lie inside the base
    // sub-object region and are NOT declared here: they are declared and
    // compiler-checked in the base's own header -- +0x0
    unsigned int m_04c;  // +0x04c /* ? */; 4-byte store: mov dword ptr [esi + 0x4c], eax
    unsigned int m_050;  // +0x050 /* ? */; 4-byte store: mov dword ptr [esi + 0x50], ebx
    unsigned int m_054;  // +0x054 /* ? */; 4-byte store: mov dword ptr [esi + 0x54], ebx
    unsigned int m_058;  // +0x058 /* ? */; 4-byte store: mov dword ptr [esi + 0x58], ebx
    unsigned char pad_05c[0x20];
    unsigned int m_07c;  // +0x07c /* ? */; 4-byte store: mov dword ptr [esi + 0x7c], ecx
    unsigned int m_080;  // +0x080 /* ? */; 4-byte store: mov dword ptr [esi + 0x80], ebx
    unsigned int m_084;  // +0x084 /* ? */; 4-byte store: mov dword ptr [esi + 0x84], ebx
    unsigned char pad_088[0x4];
    unsigned int m_08c;  // +0x08c /* ? */; 4-byte store: mov dword ptr [esi + 0x8c], ecx
    unsigned int m_090;  // +0x090 /* ? */; 4-byte store: mov dword ptr [esi + 0x90], edx
    unsigned int m_094;  // +0x094 init 0x1d; 4-byte store: mov dword ptr [esi + 0x94], 0x1d
    unsigned char m_098;  // +0x098 /* ? */; 1-byte store: mov byte ptr [esi + 0x98], bl
    unsigned char m_099;  // +0x099 /* ? */; 1-byte store: mov byte ptr [esi + 0x99], bl
};
