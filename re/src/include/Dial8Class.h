#pragma once
// ==========================================================================
// Dial8Class.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class Dial8Class
// vtable       : 0x0069b99c  (32 slots)
// constructor  : 0x00470760
// vtable store : 0x0047078c  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "ControlClass.h"

class Dial8Class : public ControlClass
{
public:
    // ---- vtable 0x0069b99c (32 slots) ----
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
    virtual void Get_ID();  // [17] 0x00460610  // __cdecl
    virtual void Flag_To_Redraw();  // [18] 0x004a5e20  // __cdecl
    virtual void Peer_To_Peer(int a0, int a1, int a2);  // [19] 0x00460650  // __stdcall
    virtual void Set_Focus();  // [20] 0x004a5e60  // __thiscall
    virtual void Clear_Focus();  // [21] 0x004a5e90  // __cdecl
    virtual void Has_Focus();  // [22] 0x004a5eb0  // __cdecl, returns int
    virtual void Is_List_To_Redraw();  // [23] 0x004a5ec0  // __cdecl, returns int
    virtual void Is_To_Redraw();  // [24] 0x0045aff0  // __cdecl
    virtual void Set_Position(int a0, int a1);  // [25] 0x004a5ee0  // __stdcall
    virtual void Set_Size(int a0, int a1);  // [26] 0x004a5f00  // __stdcall
    virtual void Draw_Me(int a0, int a1, int a2, int a3, int a4, int a5, int a6);  // [27] 0x00470b60  // __thiscall
    virtual void Sticky_Process(int a0);  // [28] 0x004a5e30  // __stdcall
    virtual void Action(int a0, int a1);  // [29] 0x00470a10  // __thiscall
    virtual void Clicked_On(int a0, int a1, int a2, int a3);  // [30] 0x004a5a60  // __stdcall, returns int
    virtual void vfunc_31(int a0);  // [31] 0x00460600  // __stdcall

    Dial8Class();  // 0x00470760
    virtual ~Dial8Class();

    // ---- data members (offsets from ctor 0x00470760) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    // vtable-pointer stores are omitted: the compiler supplies the vptr.
    // The machine block begins at +0x2c: the compiled end of the
    // base sub-object region. Evidence: ControlClass at +0x0 (ends +0x2c).
    // 1 recovered offset(s) below +0x2c lie inside the base
    // sub-object region and are NOT declared here: they are declared and
    // compiler-checked in the base's own header -- +0x0
    unsigned int m_02c;  // +0x02c /* ? */; 4-byte store: mov dword ptr [esi + 0x2c], eax
    unsigned int m_030;  // +0x030 /* ? */; 4-byte store: mov dword ptr [esi + 0x30], eax
    unsigned int m_034;  // +0x034 /* ? */; 4-byte store: mov dword ptr [esi + 0x34], edx
    unsigned int m_038;  // +0x038 /* ? */; 4-byte store: mov dword ptr [esi + 0x38], ecx
    unsigned int m_03c;  // +0x03c /* ? */; 4-byte store: mov dword ptr [esi + 0x3c], eax
    unsigned int m_040;  // +0x040 /* ? */; 4-byte store: mov dword ptr [esi + 0x40], eax
    unsigned int m_044;  // +0x044 /* ? */; 4-byte store: mov dword ptr [esi + 0x44], edx
    unsigned int m_048;  // +0x048 /* ? */; 4-byte store: mov dword ptr [esi + 0x48], edi
    unsigned int m_04c;  // +0x04c /* ? */; 4-byte store: mov dword ptr [esi + 0x4c], eax
    unsigned int m_050;  // +0x050 /* ? */; 4-byte store: mov dword ptr [esi + 0x50], eax
    unsigned int m_054;  // +0x054 /* ? */; 4-byte store: mov dword ptr [esi + 0x54], ecx
    unsigned int m_058;  // +0x058 /* ? */; 4-byte store: mov dword ptr [esi + 0x58], eax
    unsigned int m_05c;  // +0x05c /* ? */; 4-byte store: mov dword ptr [esi + 0x5c], eax
    unsigned int m_060;  // +0x060 /* ? */; 4-byte store: mov dword ptr [esi + 0x60], edx
    unsigned int m_064;  // +0x064 /* ? */; 4-byte store: mov dword ptr [esi + 0x64], eax
    unsigned int m_068;  // +0x068 /* ? */; 4-byte store: mov dword ptr [esi + 0x68], edi
    unsigned int m_06c;  // +0x06c /* ? */; 4-byte store: mov dword ptr [esi + 0x6c], edx
    unsigned int m_070;  // +0x070 /* ? */; 4-byte store: mov dword ptr [esi + 0x70], eax
    unsigned int m_074;  // +0x074 /* ? */; 4-byte store: mov dword ptr [esi + 0x74], ecx
    unsigned int m_078;  // +0x078 /* ? */; 4-byte store: mov dword ptr [esi + 0x78], eax
    unsigned int m_07c;  // +0x07c /* ? */; 4-byte store: mov dword ptr [esi + 0x7c], eax
    unsigned int m_080;  // +0x080 /* ? */; 4-byte store: mov dword ptr [esi + 0x80], ebx
    unsigned int m_084;  // +0x084 /* ? */; 4-byte store: mov dword ptr [esi + 0x84], eax
    unsigned int m_088;  // +0x088 /* ? */; 4-byte store: mov dword ptr [esi + 0x88], edi
    unsigned int m_08c;  // +0x08c /* ? */; 4-byte store: mov dword ptr [esi + 0x8c], eax
    unsigned int m_090;  // +0x090 /* ? */; 4-byte store: mov dword ptr [esi + 0x90], ebp
    unsigned int m_094;  // +0x094 /* ? */; 4-byte store: mov dword ptr [esi + 0x94], ecx
    unsigned int m_098;  // +0x098 /* ? */; 4-byte store: mov dword ptr [esi + 0x98], eax
    unsigned int m_09c;  // +0x09c /* ? */; 4-byte store: mov dword ptr [esi + 0x9c], eax
    unsigned int m_0a0;  // +0x0a0 /* ? */; 4-byte store: mov dword ptr [esi + 0xa0], ebp
    unsigned int m_0a4;  // +0x0a4 /* ? */; 4-byte store: mov dword ptr [esi + 0xa4], ecx
    unsigned int m_0a8;  // +0x0a8 /* ? */; 4-byte store: mov dword ptr [esi + 0xa8], edi
    unsigned int m_0ac;  // +0x0ac /* ? */; 4-byte store: mov dword ptr [esi + 0xac], eax
    unsigned int m_0b0;  // +0x0b0 /* ? */; 4-byte store: mov dword ptr [esi + 0xb0], ebx
    unsigned int m_0b4;  // +0x0b4 /* ? */; 4-byte store: mov dword ptr [esi + 0xb4], eax
    unsigned int m_0b8;  // +0x0b8 /* ? */; 4-byte store: mov dword ptr [esi + 0xb8], eax
    unsigned int m_0bc;  // +0x0bc /* ? */; 4-byte store: mov dword ptr [esi + 0xbc], eax
};
