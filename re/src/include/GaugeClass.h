#pragma once
// ==========================================================================
// GaugeClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class GaugeClass
// vtable       : 0x0069e804  (40 slots)
// constructor  : 0x004a6a00
// vtable store : 0x004a6a35  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "ControlClass.h"

class GaugeClass : public ControlClass
{
public:
    // ---- vtable 0x0069e804 (40 slots) ----
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
    virtual void Get_ID();  // [17] 0x00460610  // __thiscall, returns ?
    virtual void Flag_To_Redraw();  // [18] 0x004a5e20  // __thiscall, returns void
    virtual void Peer_To_Peer(int a0, int a1, int a2);  // [19] 0x00460650  // __stdcall, returns void
    virtual void Set_Focus();  // [20] 0x004a5e60  // __thiscall, returns ?
    virtual void Clear_Focus();  // [21] 0x004a5e90  // __thiscall, returns ?
    virtual void Has_Focus();  // [22] 0x004a5eb0  // __thiscall, returns ?
    virtual void Is_List_To_Redraw();  // [23] 0x004a5ec0  // __thiscall, returns int
    virtual void Is_To_Redraw();  // [24] 0x0045aff0  // __thiscall, returns char
    virtual void Set_Position(int a0, int a1);  // [25] 0x004a5ee0  // __thiscall, returns ?
    virtual void Set_Size(int a0, int a1);  // [26] 0x004a5f00  // __thiscall, returns int
    virtual void Draw_Me(int a0);  // [27] 0x004a6b90  // __thiscall, returns short
    virtual void Sticky_Process(int a0);  // [28] 0x004a5e30  // __thiscall, returns char
    virtual void Action(int a0, int a1);  // [29] 0x004a6d30  // __thiscall, returns int
    virtual void Clicked_On(int a0, int a1, int a2, int a3);  // [30] 0x004a5a60  // __thiscall, returns int
    virtual void vfunc_31(int a0);  // [31] 0x00460600  // __thiscall, returns ?
    virtual void vfunc_32(int a0);  // [32] 0x004a6a80  // __thiscall, returns int
    virtual void vfunc_33(int a0);  // [33] 0x004a6aa0  // __thiscall, returns int
    virtual void vfunc_34();  // [34] 0x004a7590  // __thiscall, returns ?
    virtual void vfunc_35(int a0);  // [35] 0x004a75a0  // __thiscall, returns ?
    virtual void vfunc_36();  // [36] 0x004a75b0  // __cdecl, returns int
    virtual void vfunc_37();  // [37] 0x004a6e90  // __thiscall, returns ?
    virtual void vfunc_38(int a0);  // [38] 0x004a6ad0  // __thiscall, returns int
    virtual void vfunc_39(int a0);  // [39] 0x004a6b50  // __thiscall, returns int

    GaugeClass();  // 0x004a6a00
    virtual ~GaugeClass();

    // ---- data members (offsets from ctor 0x004a6a00) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    // vtable-pointer stores are omitted: the compiler supplies the vptr.
    // The machine block begins at +0x2c: the compiled end of the
    // base sub-object region. Evidence: ControlClass at +0x0 (ends +0x2c).
    // 1 recovered offset(s) below +0x2c lie inside the base
    // sub-object region and are NOT declared here: they are declared and
    // compiler-checked in the base's own header -- +0x0
    unsigned char m_02c;  // +0x02c init 0x1; 1-byte store: mov byte ptr [esi + 0x2c], 1
    unsigned char m_02d;  // +0x02d init 0x1; 1-byte store: mov byte ptr [esi + 0x2d], 1
    unsigned char m_02e;  // +0x02e /* ? */; 1-byte store: mov byte ptr [esi + 0x2e], cl
    unsigned char pad_02f[0x1];
    unsigned int m_030;  // +0x030 /* ? */; 4-byte store: mov dword ptr [esi + 0x30], eax
    unsigned int m_034;  // +0x034 /* ? */; 4-byte store: mov dword ptr [esi + 0x34], eax
    unsigned int m_038;  // +0x038 zero-initialised; 4-byte store: mov dword ptr [esi + 0x38], 0
};
