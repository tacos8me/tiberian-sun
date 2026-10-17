#pragma once
// ==========================================================================
// StaticButtonClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class StaticButtonClass
// vtable       : 0x006a4ec8  (34 slots)
// constructor  : 0x005eeee0
// vtable store : 0x005eef2a  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "GadgetClass.h"

class StaticButtonClass : public GadgetClass
{
public:
    // ---- vtable 0x006a4ec8 (34 slots) ----
    virtual void vfunc_1();  // [1] 0x004a5b00  // __cdecl, returns ?
    virtual void vfunc_2();  // [2] 0x004a5b10  // __cdecl, returns ?
    virtual void vfunc_3(int a0);  // [3] 0x004f69e0  // __thiscall, returns ?
    virtual void vfunc_4(int a0);  // [4] 0x004f6a20  // __thiscall, returns ?
    virtual void vfunc_5(int a0);  // [5] 0x004f6a00  // __thiscall, returns ?
    virtual void vfunc_6();  // [6] 0x004f6980  // __thiscall, returns ?
    virtual void vfunc_7();  // [7] 0x004f69b0  // __thiscall, returns ?
    virtual void vfunc_8();  // [8] 0x004f6930  // __thiscall, returns int
    virtual void vfunc_9();  // [9] 0x004a5ae0  // __thiscall, returns ?
    virtual void Input();  // [10] 0x004a5be0  // __thiscall, returns ?
    virtual void Draw_All(int a0);  // [11] 0x004a5bb0  // __thiscall, returns ?
    virtual void Delete_List();  // [12] 0x004a5b20  // __thiscall, returns ?
    virtual void Extract_Gadget(int a0);  // [13] 0x004a5de0  // __thiscall, returns int
    virtual void Flag_List_To_Redraw();  // [14] 0x0045afe0  // __cdecl, returns void
    virtual void Disable();  // [15] 0x004a5ad0  // __thiscall, returns ?
    virtual void Enable();  // [16] 0x004a5ac0  // __thiscall, returns ?
    virtual void Get_ID();  // [17] 0x004788d0  // __cdecl, returns int
    virtual void Flag_To_Redraw();  // [18] 0x004a5e20  // __thiscall, returns void
    virtual void Peer_To_Peer(int a0, int a1, int a2);  // [19] 0x00460650  // __stdcall, returns void
    virtual void Set_Focus();  // [20] 0x004a5e60  // __thiscall, returns ?
    virtual void Clear_Focus();  // [21] 0x004a5e90  // __thiscall, returns ?
    virtual void Has_Focus();  // [22] 0x004a5eb0  // __thiscall, returns ?
    virtual void Is_List_To_Redraw();  // [23] 0x004a5ec0  // __thiscall, returns int
    virtual void Is_To_Redraw();  // [24] 0x0045aff0  // __thiscall, returns char
    virtual void Set_Position(int a0, int a1);  // [25] 0x004a5ee0  // __thiscall, returns ?
    virtual void Set_Size(int a0, int a1);  // [26] 0x004a5f00  // __thiscall, returns int
    virtual void Draw_Me(int a0);  // [27] 0x005eeff0  // __thiscall, returns int
    virtual void Sticky_Process(int a0);  // [28] 0x004a5e30  // __thiscall, returns char
    virtual void Action(int a0, int a1);  // [29] 0x004a5b70  // __thiscall, returns int
    virtual void Clicked_On(int a0, int a1, int a2, int a3);  // [30] 0x004a5a60  // __thiscall, returns int
    virtual void vfunc_31(int a0, int a1);  // [31] 0x005ef030  // __thiscall, returns ?
    virtual void vfunc_32();  // [32] 0x005ef100  // __thiscall, returns ?
    virtual void vfunc_33(int a0);  // [33] 0x005ef1b0  // __thiscall, returns void*

    StaticButtonClass();  // 0x005eeee0
    virtual ~StaticButtonClass();

    // ---- data members (offsets from ctor 0x005eeee0) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    // vtable-pointer stores are omitted: the compiler supplies the vptr.
    // The machine block begins at +0x24: the compiled end of the
    // base sub-object region. Evidence: GadgetClass at +0x0 (ends +0x24).
    // 3 recovered offset(s) below +0x24 lie inside the base
    // sub-object region and are NOT declared here: they are declared and
    // compiler-checked in the base's own header -- +0x0, +0x14, +0x18
    unsigned char pad_024[0xc];
    unsigned int m_030;  // +0x030 zero-initialised; 4-byte store: mov dword ptr [esi + 0x30], 0
    unsigned int m_034;  // +0x034 /* ? */; 4-byte store: mov dword ptr [esi + 0x34], edx
};
