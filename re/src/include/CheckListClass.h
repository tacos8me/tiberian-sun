#pragma once
// ==========================================================================
// CheckListClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class CheckListClass
// vtable       : 0x0069aeb8  (50 slots)
// constructor  : 0x0045abd0
// vtable store : 0x0045ac0b  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "ListClass.h"

class CheckListClass : public ListClass
{
public:
    // ---- vtable 0x0069aeb8 (50 slots) ----
    virtual void vfunc_1();  // [1] 0x004a5b00
    virtual void vfunc_2();  // [2] 0x004a5b10
    virtual void vfunc_3(int a0);  // [3] 0x004f76e0  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x004f7780  // __thiscall
    virtual void vfunc_5(int a0);  // [5] 0x004f7730  // __thiscall
    virtual void vfunc_6();  // [6] 0x004f6980  // __cdecl
    virtual void vfunc_7();  // [7] 0x004f69b0  // __cdecl
    virtual void vfunc_8();  // [8] 0x004f6930  // __cdecl, returns int
    virtual void vfunc_9();  // [9] 0x004f77d0  // __thiscall
    virtual void Input(int a0, int a1);  // [10] 0x004a5be0  // __thiscall
    virtual void Draw_All(int a0);  // [11] 0x004a5bb0  // __thiscall
    virtual void Delete_List();  // [12] 0x004a5b20  // __thiscall
    virtual void Extract_Gadget(int a0);  // [13] 0x004a5de0  // __thiscall, returns int
    virtual void Flag_List_To_Redraw();  // [14] 0x0045afe0  // __cdecl
    virtual void Disable();  // [15] 0x004a5ad0  // __thiscall
    virtual void Enable();  // [16] 0x004a5ac0
    virtual void Get_ID();  // [17] 0x00460610  // __cdecl
    virtual void Flag_To_Redraw();  // [18] 0x004f78a0  // __thiscall
    virtual void Peer_To_Peer(int a0, int a1, int a2);  // [19] 0x004f73e0  // __thiscall
    virtual void Set_Focus();  // [20] 0x004a5e60  // __thiscall
    virtual void Clear_Focus();  // [21] 0x004a5e90  // __cdecl
    virtual void Has_Focus();  // [22] 0x004a5eb0  // __cdecl, returns int
    virtual void Is_List_To_Redraw();  // [23] 0x004a5ec0  // __cdecl, returns int
    virtual void Is_To_Redraw();  // [24] 0x0045aff0  // __cdecl
    virtual void Set_Position(int a0, int a1);  // [25] 0x004f6e10  // __stdcall
    virtual void Set_Size(int a0, int a1);  // [26] 0x004a5f00  // __stdcall
    virtual void Draw_Me(int a0);  // [27] 0x004f71f0  // __thiscall
    virtual void Sticky_Process(int a0);  // [28] 0x004a5e30  // __stdcall
    virtual void Action(int a0, int a1);  // [29] 0x0045adf0  // __thiscall, returns int
    virtual void Clicked_On(int a0, int a1, int a2, int a3);  // [30] 0x004a5a60  // __stdcall, returns int
    virtual void vfunc_31(int a0);  // [31] 0x00460600  // __stdcall
    virtual void vfunc_32(int a0);  // [32] 0x0045b010  // __stdcall
    virtual void vfunc_33(int a0);  // [33] 0x0045ac60  // __thiscall
    virtual void vfunc_34();  // [34] 0x004f74b0  // __thiscall, returns int
    virtual void vfunc_35(int a0);  // [35] 0x004f72f0  // __thiscall
    virtual void vfunc_36();  // [36] 0x0045b000  // __cdecl
    virtual void vfunc_37();  // [37] 0x004f73d0  // __cdecl
    virtual void vfunc_38();  // [38] 0x0045aca0  // __cdecl
    virtual void vfunc_39(int a0);  // [39] 0x0045acb0  // __stdcall
    virtual void vfunc_40(int a0);  // [40] 0x004f7880  // __stdcall
    virtual void vfunc_41(int a0);  // [41] 0x0045b020  // __stdcall
    virtual void vfunc_42(int a0);  // [42] 0x0045acd0  // __thiscall
    virtual void vfunc_43();  // [43] 0x004f7580  // __thiscall, returns int
    virtual void vfunc_44(int a0);  // [44] 0x0045ad40  // __thiscall
    virtual void vfunc_45(int a0);  // [45] 0x0045b030  // __stdcall
    virtual void vfunc_46(int a0);  // [46] 0x004f75e0  // __stdcall
    virtual void vfunc_47(int a0);  // [47] 0x004f7440  // __thiscall
    virtual void vfunc_48(int a0);  // [48] 0x004f7340  // __thiscall
    virtual void vfunc_49(int a0, int a1, int a2, int a3, int a4, int a5, int a6);  // [49] 0x0045ae70  // __thiscall

    CheckListClass();  // 0x0045abd0
    virtual ~CheckListClass();

    // ---- data members (offsets from ctor 0x0045abd0) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    // vtable-pointer stores are omitted: the compiler supplies the vptr.
    // The machine block begins at +0x14c: the compiled end of the
    // base sub-object region. Evidence: ListClass at +0x0 (ends +0x14c).
    // 1 recovered offset(s) below +0x14c lie inside the base
    // sub-object region and are NOT declared here: they are declared and
    // compiler-checked in the base's own header -- +0x0
    unsigned int m_14c;  // +0x14c zero-initialised; 4-byte store: mov dword ptr [esi + 0x14c], 0x69b05c
};
