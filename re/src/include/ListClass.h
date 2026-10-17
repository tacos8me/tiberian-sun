#pragma once
// ==========================================================================
// ListClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class ListClass
// vtable       : 0x006a1654  (50 slots)
// constructor  : 0x004f6ad0
// vtable store : 0x004f6b99  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "ControlClass.h"

class ListClass : public ControlClass
{
public:
    // ---- vtable 0x006a1654 (50 slots) ----
    virtual void vfunc_1();  // [1] 0x004a5b00
    virtual void vfunc_2();  // [2] 0x004a5b10
    virtual void vfunc_3(int a0);  // [3] 0x004f76e0  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x004f7780  // __thiscall
    virtual void vfunc_5(int a0);  // [5] 0x004f7730  // __thiscall
    virtual void vfunc_6();  // [6] 0x004f6980  // __cdecl
    virtual void vfunc_7();  // [7] 0x004f69b0  // __cdecl
    virtual void vfunc_8();  // [8] 0x004f6930  // __cdecl, returns int
    virtual void vfunc_9();  // [9] 0x004f77d0  // __thiscall
    virtual void vfunc_10(int a0, int a1);  // [10] 0x004a5be0  // __thiscall
    virtual void vfunc_11(int a0);  // [11] 0x004a5bb0  // __thiscall
    virtual void vfunc_12();  // [12] 0x004a5b20  // __thiscall
    virtual void vfunc_13(int a0);  // [13] 0x004a5de0  // __thiscall, returns int
    virtual void vfunc_14();  // [14] 0x0045afe0  // __cdecl
    virtual void vfunc_15();  // [15] 0x004a5ad0  // __thiscall
    virtual void vfunc_16();  // [16] 0x004a5ac0
    virtual void vfunc_17();  // [17] 0x00460610  // __cdecl
    virtual void vfunc_18();  // [18] 0x004f78a0  // __thiscall
    virtual void vfunc_19(int a0, int a1, int a2);  // [19] 0x004f73e0  // __thiscall
    virtual void vfunc_20();  // [20] 0x004a5e60  // __thiscall
    virtual void vfunc_21();  // [21] 0x004a5e90  // __cdecl
    virtual void vfunc_22();  // [22] 0x004a5eb0  // __cdecl, returns int
    virtual void vfunc_23();  // [23] 0x004a5ec0  // __cdecl, returns int
    virtual void vfunc_24();  // [24] 0x0045aff0  // __cdecl
    virtual void vfunc_25(int a0, int a1);  // [25] 0x004f6e10  // __stdcall
    virtual void vfunc_26(int a0, int a1);  // [26] 0x004a5f00  // __stdcall
    virtual void vfunc_27(int a0);  // [27] 0x004f71f0  // __thiscall
    virtual void vfunc_28(int a0);  // [28] 0x004a5e30  // __stdcall
    virtual void vfunc_29(int a0, int a1);  // [29] 0x004f7100  // __thiscall, returns int
    virtual void vfunc_30(int a0, int a1, int a2, int a3);  // [30] 0x004a5a60  // __stdcall, returns int
    virtual void vfunc_31(int a0);  // [31] 0x00460600  // __stdcall
    virtual void vfunc_32(int a0);  // [32] 0x004f6fd0  // __thiscall
    virtual void vfunc_33(int a0);  // [33] 0x004f6f40  // __thiscall
    virtual void vfunc_34();  // [34] 0x004f74b0  // __thiscall, returns int
    virtual void vfunc_35(int a0);  // [35] 0x004f72f0  // __thiscall
    virtual void vfunc_36();  // [36] 0x0045b000  // __cdecl
    virtual void vfunc_37();  // [37] 0x004f73d0  // __cdecl
    virtual void vfunc_38();  // [38] 0x004f73b0  // __cdecl, returns int
    virtual void vfunc_39(int a0);  // [39] 0x004f7390  // __stdcall
    virtual void vfunc_40(int a0);  // [40] 0x004f7880  // __stdcall
    virtual void vfunc_41(int a0);  // [41] 0x004f7000  // __thiscall
    virtual void vfunc_42(int a0);  // [42] 0x004f70d0  // __thiscall
    virtual void vfunc_43();  // [43] 0x004f7580  // __thiscall, returns int
    virtual void vfunc_44(int a0);  // [44] 0x004f78e0  // __thiscall
    virtual void vfunc_45(int a0);  // [45] 0x004f7810  // __thiscall
    virtual void vfunc_46(int a0);  // [46] 0x004f75e0  // __stdcall
    virtual void vfunc_47(int a0);  // [47] 0x004f7440  // __thiscall
    virtual void vfunc_48(int a0);  // [48] 0x004f7340  // __thiscall
    virtual void vfunc_49(int a0, int a1, int a2, int a3, int a4, int a5);  // [49] 0x004f75f0  // __thiscall

    ListClass();  // 0x004f6ad0
    virtual ~ListClass();

    // ---- data members (offsets from ctor 0x004f6ad0) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    // vtable-pointer stores are omitted: the compiler supplies the vptr.
    unsigned char pad_004[0x28];
    unsigned int m_02c;  // +0x02c /* ? */; 4-byte store: mov dword ptr [esi + 0x2c], ecx
    unsigned int m_030;  // +0x030 /* ? */; 4-byte store: mov dword ptr [esi + 0x30], eax
    unsigned int m_038;  // +0x038 /* ? */; 4-byte store: mov dword ptr [esi + 0x38], eax
    unsigned int m_03c;  // +0x03c /* ? */; 4-byte store: mov dword ptr [esi + 0x3c], eax
    unsigned char m_040;  // +0x040 init 0x1; 1-byte store: mov byte ptr [esi + 0x40], 1
    unsigned char m_041;  // +0x041 /* ? */; 1-byte store: mov byte ptr [esi + 0x41], al
    unsigned char pad_042[0x2];
    unsigned int m_044;  // +0x044 /* ? */; 4-byte store: mov dword ptr [esi + 0x44], eax
    unsigned int m_048;  // +0x048 init 0xa; 4-byte store: mov dword ptr [esi + 0x48], 0xa
    unsigned int m_04c;  // +0x04c /* ? */; 4-byte store: mov dword ptr [esi + 0x4c], ecx
    unsigned int m_050;  // +0x050 /* ? */; 4-byte store: mov dword ptr [esi + 0x50], eax
    unsigned char m_054;  // +0x054 zero-initialised; 1-byte store: mov byte ptr [esi + 0x54], 0
    unsigned char pad_055[0xf];
    unsigned int m_064;  // +0x064 /* ? */; 4-byte store: mov dword ptr [esi + 0x64], edi
    unsigned char pad_068[0x48];
    unsigned int m_0b0;  // +0x0b0 /* ? */; 4-byte store: mov dword ptr [esi + 0xb0], edx
    unsigned int m_0b4;  // +0x0b4 /* ? */; 4-byte store: mov dword ptr [esi + 0xb4], ebp
    unsigned char pad_0b8[0x44];
    unsigned int m_0fc;  // +0x0fc /* ? */; 4-byte store: mov dword ptr [esi + 0xfc], ebp
    unsigned int m_100;  // +0x100 /* ? */; 4-byte store: mov dword ptr [esi + 0x100], ebp
    unsigned int m_104;  // +0x104 /* ? */; 4-byte store: mov dword ptr [esi + 0x104], eax
    unsigned int m_108;  // +0x108 /* ? */; 4-byte store: mov dword ptr [esi + 0x108], ebp
    unsigned char pad_10c[0x38];
    unsigned int m_144;  // +0x144 /* ? */; 4-byte store: mov dword ptr [esi + 0x144], eax
    unsigned int m_148;  // +0x148 /* ? */; 4-byte store: mov dword ptr [esi + 0x148], eax
};
