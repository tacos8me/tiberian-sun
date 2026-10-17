#pragma once
// ==========================================================================
// SliderClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class SliderClass
// vtable       : 0x006a1758  (43 slots)
// constructor  : 0x004f6c30
// vtable store : 0x004f6dc9  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "GaugeClass.h"

class SliderClass : public GaugeClass
{
public:
    // ---- vtable 0x006a1758 (43 slots) ----
    virtual void vfunc_1();  // [1] 0x004a5b00
    virtual void vfunc_2();  // [2] 0x004a5b10
    virtual void vfunc_3(int a0);  // [3] 0x004f69e0  // __stdcall
    virtual void vfunc_4(int a0);  // [4] 0x004f6a20  // __thiscall
    virtual void vfunc_5(int a0);  // [5] 0x004f6a00  // __thiscall
    virtual void vfunc_6();  // [6] 0x004f6980  // __cdecl
    virtual void vfunc_7();  // [7] 0x004f69b0  // __cdecl
    virtual void vfunc_8();  // [8] 0x004f6930  // __cdecl, returns int
    virtual void vfunc_9();  // [9] 0x004a5ae0  // __thiscall
    virtual void vfunc_10(int a0, int a1);  // [10] 0x004a5be0  // __thiscall
    virtual void vfunc_11(int a0);  // [11] 0x004a5bb0  // __thiscall
    virtual void vfunc_12();  // [12] 0x004a5b20  // __thiscall
    virtual void vfunc_13(int a0);  // [13] 0x004a5de0  // __thiscall, returns int
    virtual void vfunc_14();  // [14] 0x0045afe0  // __cdecl
    virtual void vfunc_15();  // [15] 0x004a5ad0  // __thiscall
    virtual void vfunc_16();  // [16] 0x004a5ac0
    virtual void vfunc_17();  // [17] 0x00460610  // __cdecl
    virtual void vfunc_18();  // [18] 0x004a5e20  // __cdecl
    virtual void vfunc_19(int a0, int a1, int a2);  // [19] 0x005de480  // __thiscall
    virtual void vfunc_20();  // [20] 0x004a5e60  // __thiscall
    virtual void vfunc_21();  // [21] 0x004a5e90  // __cdecl
    virtual void vfunc_22();  // [22] 0x004a5eb0  // __cdecl, returns int
    virtual void vfunc_23();  // [23] 0x004a5ec0  // __cdecl, returns int
    virtual void vfunc_24();  // [24] 0x0045aff0  // __cdecl
    virtual void vfunc_25(int a0, int a1);  // [25] 0x004a5ee0  // __stdcall
    virtual void vfunc_26(int a0, int a1);  // [26] 0x004a5f00  // __stdcall
    virtual void vfunc_27(int a0);  // [27] 0x005de410  // __thiscall, returns int
    virtual void vfunc_28(int a0);  // [28] 0x004a5e30  // __stdcall
    virtual void vfunc_29(int a0);  // [29] 0x005de280  // __thiscall
    virtual void vfunc_30(int a0, int a1, int a2, int a3);  // [30] 0x004a5a60  // __stdcall, returns int
    virtual void vfunc_31(int a0);  // [31] 0x00460600  // __stdcall
    virtual void vfunc_32(int a0);  // [32] 0x005de070  // __thiscall
    virtual void vfunc_33(int a0, int a1);  // [33] 0x005de180  // __thiscall
    virtual void vfunc_34();  // [34] 0x004a7590  // __cdecl
    virtual void vfunc_35(int a0);  // [35] 0x004a75a0  // __stdcall
    virtual void vfunc_36();  // [36] 0x004f79f0  // __cdecl
    virtual void vfunc_37(int a0);  // [37] 0x005de390  // __cdecl
    virtual void vfunc_38(int a0);  // [38] 0x004a6ad0
    virtual void vfunc_39(int a0);  // [39] 0x004a6b50
    virtual void vfunc_40(int a0, int a1);  // [40] 0x005de0f0  // __thiscall
    virtual void vfunc_41(int a0);  // [41] 0x005de320  // __stdcall
    virtual void vfunc_42(int a0);  // [42] 0x005de360  // __stdcall

    SliderClass();  // 0x004f6c30
    virtual ~SliderClass();

    // ---- data members (offsets from ctor 0x004f6c30) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    // vtable-pointer stores are omitted: the compiler supplies the vptr.
    // The machine block begins at +0x3c: the compiled end of the
    // base sub-object region. Evidence: GaugeClass at +0x0 (ends +0x3c).
    // 7 recovered offset(s) below +0x3c lie inside the base
    // sub-object region and are NOT declared here: they are declared and
    // compiler-checked in the base's own header -- +0x0, +0x2c, +0x2d, +0x2e, +0x30, +0x34, +0x38
    unsigned int m_03c;  // +0x03c /* ? */; 4-byte store: mov dword ptr [edi + 0x3c], ecx
    unsigned int m_040;  // +0x040 /* ? */; 4-byte store: mov dword ptr [edi + 0x40], edx
    unsigned char m_044;  // +0x044 /* ? */; 1-byte store: mov byte ptr [edi + 0x44], cl
    unsigned char pad_045[0x3];
    unsigned int m_048;  // +0x048 /* ? */; 4-byte store: mov dword ptr [edi + 0x48], edx
    unsigned int m_04c;  // +0x04c /* ? */; 4-byte store: mov dword ptr [edi + 0x4c], ecx
    unsigned int m_050;  // +0x050 /* ? */; 4-byte store: mov dword ptr [edi + 0x50], edx
};
