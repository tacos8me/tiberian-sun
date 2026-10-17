#pragma once
// ==========================================================================
// RadarClass_RTacticalClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class RadarClass::RTacticalClass
// vtable       : 0x006a357c  (31 slots)
// constructor  : 0x005a06a0
// vtable store : 0x005a06c4  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "GadgetClass.h"

class RadarClass_RTacticalClass : public GadgetClass
{
public:
    // ---- vtable 0x006a357c (31 slots) ----
    virtual void vfunc_1();  // [1] 0x004a5b00
    virtual void vfunc_2();  // [2] 0x004a5b10
    virtual void vfunc_3(int a0);  // [3] 0x004f69e0  // __stdcall
    virtual void vfunc_4(int a0);  // [4] 0x004f6a20  // __thiscall
    virtual void vfunc_5(int a0);  // [5] 0x004f6a00  // __thiscall
    virtual void vfunc_6();  // [6] 0x004f6980  // __cdecl
    virtual void vfunc_7();  // [7] 0x004f69b0  // __cdecl
    virtual void vfunc_8();  // [8] 0x004f6930  // __cdecl, returns int
    virtual void vfunc_9();  // [9] 0x004a5ae0  // __thiscall
    virtual void Input(int a0, int a1);  // [10] 0x004a5be0  // __thiscall
    virtual void Draw_All(int a0);  // [11] 0x004a5bb0  // __thiscall
    virtual void Delete_List();  // [12] 0x004a5b20  // __thiscall
    virtual void Extract_Gadget(int a0);  // [13] 0x004a5de0  // __thiscall, returns int
    virtual void Flag_List_To_Redraw();  // [14] 0x0045afe0  // __cdecl
    virtual void Disable();  // [15] 0x004a5ad0  // __thiscall
    virtual void Enable();  // [16] 0x004a5ac0
    virtual void Get_ID();  // [17] 0x004788d0  // __cdecl, returns int
    virtual void Flag_To_Redraw();  // [18] 0x004a5e20  // __cdecl
    virtual void Peer_To_Peer(int a0, int a1, int a2);  // [19] 0x00460650  // __stdcall
    virtual void Set_Focus();  // [20] 0x004a5e60  // __thiscall
    virtual void Clear_Focus();  // [21] 0x004a5e90  // __cdecl
    virtual void Has_Focus();  // [22] 0x004a5eb0  // __cdecl, returns int
    virtual void Is_List_To_Redraw();  // [23] 0x004a5ec0  // __cdecl, returns int
    virtual void Is_To_Redraw();  // [24] 0x0045aff0  // __cdecl
    virtual void Set_Position(int a0, int a1);  // [25] 0x004a5ee0  // __stdcall
    virtual void Set_Size(int a0, int a1);  // [26] 0x004a5f00  // __stdcall
    virtual void Draw_Me(int a0);  // [27] 0x004a5b90  // __stdcall, returns int
    virtual void Sticky_Process(int a0);  // [28] 0x004a5e30  // __stdcall
    virtual void Action(int a0, int a1);  // [29] 0x005a1050
    virtual void Clicked_On(int a0, int a1, int a2, int a3);  // [30] 0x004a5a60  // __stdcall, returns int

    RadarClass_RTacticalClass();  // 0x005a06a0
    virtual ~RadarClass_RTacticalClass();
};
