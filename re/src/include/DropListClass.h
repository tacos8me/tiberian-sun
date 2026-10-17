#pragma once
// ==========================================================================
// DropListClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class DropListClass
// vtable       : 0x0069d7f4  (44 slots)
// constructor  : 0x0047ebc0
// vtable store : 0x0047ec5e  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "EditClass.h"

class DropListClass : public EditClass
{
public:
    // ---- vtable 0x0069d7f4 (44 slots) ----
    virtual void vfunc_1();  // [1] 0x004a5b00
    virtual void vfunc_2();  // [2] 0x004a5b10
    virtual void vfunc_3(int a0);  // [3] 0x0047ecd0  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x0047ecf0  // __thiscall
    virtual void vfunc_5(int a0);  // [5] 0x0047ed10  // __thiscall
    virtual void vfunc_6();  // [6] 0x004f6980  // __cdecl
    virtual void vfunc_7();  // [7] 0x004f69b0  // __cdecl
    virtual void vfunc_8();  // [8] 0x0047ec90  // __thiscall
    virtual void vfunc_9();  // [9] 0x0047ed30  // __thiscall
    virtual void Input(int a0, int a1);  // [10] 0x004a5be0  // __thiscall
    virtual void Draw_All(int a0);  // [11] 0x004a5bb0  // __thiscall
    virtual void Delete_List();  // [12] 0x004a5b20  // __thiscall
    virtual void Extract_Gadget(int a0);  // [13] 0x004a5de0  // __thiscall, returns int
    virtual void Flag_List_To_Redraw();  // [14] 0x0045afe0  // __cdecl
    virtual void Disable();  // [15] 0x004a5ad0  // __thiscall
    virtual void Enable();  // [16] 0x004a5ac0
    virtual void Get_ID();  // [17] 0x00460610  // __cdecl
    virtual void Flag_To_Redraw();  // [18] 0x004a5e20  // __cdecl
    virtual void Peer_To_Peer(int a0, int a1);  // [19] 0x0047ee80  // __thiscall
    virtual void Set_Focus();  // [20] 0x0048cf90  // __thiscall
    virtual void Clear_Focus();  // [21] 0x0047ee60  // __thiscall
    virtual void Has_Focus();  // [22] 0x004a5eb0  // __cdecl, returns int
    virtual void Is_List_To_Redraw();  // [23] 0x004a5ec0  // __cdecl, returns int
    virtual void Is_To_Redraw();  // [24] 0x0045aff0  // __cdecl
    virtual void Set_Position(int a0, int a1);  // [25] 0x0047f260  // __thiscall
    virtual void Set_Size(int a0, int a1);  // [26] 0x004a5f00  // __stdcall
    virtual void Draw_Me(int a0);  // [27] 0x0048cb40  // __thiscall
    virtual void Sticky_Process(int a0);  // [28] 0x004a5e30  // __stdcall
    virtual void Action(int a0, int a1);  // [29] 0x0048cbc0  // __thiscall, returns int
    virtual void Clicked_On(int a0, int a1, int a2, int a3);  // [30] 0x004a5a60  // __stdcall, returns int
    virtual void vfunc_31(int a0);  // [31] 0x00460600  // __stdcall
    virtual void vfunc_32(int a0, int a1);  // [32] 0x0048cb10  // __thiscall
    virtual void vfunc_33();  // [33] 0x0047f320  // __cdecl
    virtual void vfunc_34(int a0);  // [34] 0x0048ccd0  // __cdecl
    virtual void vfunc_35(int a0);  // [35] 0x0048cd10  // __thiscall
    virtual void vfunc_36(int a0);  // [36] 0x0048ce50  // __thiscall
    virtual void vfunc_37(int a0);  // [37] 0x0047ed60  // __thiscall
    virtual void vfunc_38();  // [38] 0x0047eda0
    virtual void vfunc_39();  // [39] 0x0047edc0
    virtual void vfunc_40(int a0);  // [40] 0x0047f2b0  // __thiscall
    virtual void vfunc_41(int a0);  // [41] 0x0047ede0  // __thiscall
    virtual void vfunc_42();  // [42] 0x0047f330
    virtual void vfunc_43(int a0);  // [43] 0x0047f350  // __stdcall

    DropListClass();  // 0x0047ebc0
    virtual ~DropListClass();

    // ---- data members (offsets from ctor 0x0047ebc0) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    // vtable-pointer stores are omitted: the compiler supplies the vptr.
    // The machine block begins at +0x50: the compiled end of the
    // base sub-object region. Evidence: EditClass at +0x0 (ends +0x50).
    // 4 recovered offset(s) below +0x50 lie inside the base
    // sub-object region and are NOT declared here: they are declared and
    // compiler-checked in the base's own header -- +0x0, +0x18, +0x48, +0x4c
};
