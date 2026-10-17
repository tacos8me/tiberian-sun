#pragma once
// ==========================================================================
// PowerClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class PowerClass
// vtable       : 0x006a328c  (51 slots)
// constructor  : 0x00593aa0
// vtable store : 0x00593b0e  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "RadarClass.h"

class PowerClass : public RadarClass
{
public:
    // ---- vtable 0x006a328c (51 slots) ----
    virtual void QueryInterface(int a0, int a1, int a2);  // [0] 0x004b2de0  // __stdcall, returns int
    virtual void AddRef(int a0);  // [1] 0x00402ad0  // __stdcall, returns int
    virtual void Release(int a0);  // [2] 0x00402ae0  // __stdcall, returns int
    virtual void vfunc_3(int a0, int a1);  // [3] 0x00502000  // __stdcall
    virtual void vfunc_5();  // [5] 0x00593ba0  // __cdecl
    virtual void vfunc_6(int a0);  // [6] 0x004b2e50  // __thiscall
    virtual void vfunc_7(int a0);  // [7] 0x00593b20  // __thiscall
    virtual void vfunc_8();  // [8] 0x00472590  // __thiscall
    virtual void vfunc_9(int a0, int a1, int a2);  // [9] 0x004b2ec0  // __thiscall
    virtual void vfunc_10(int a0, int a1, int a2, int a3, int a4, int a5);  // [10] 0x00594290  // __thiscall
    virtual void vfunc_11(int a0);  // [11] 0x004b2f90  // __stdcall
    virtual void vfunc_12(int a0);  // [12] 0x004b2fd0  // __stdcall
    virtual void vfunc_13(int a0);  // [13] 0x004b2e90  // __stdcall
    virtual void vfunc_14(int a0, int a1);  // [14] 0x004b2ff0  // __thiscall
    virtual void vfunc_15(int a0);  // [15] 0x00593f30  // __thiscall
    virtual void vfunc_16();  // [16] 0x004b3100  // __cdecl
    virtual void vfunc_17();  // [17] 0x006829a5  // __cdecl
    virtual void vfunc_18();  // [18] 0x006829a5  // __cdecl
    virtual void vfunc_19();  // [19] 0x006829a5  // __cdecl
    virtual void vfunc_20();  // [20] 0x006829a5  // __cdecl
    virtual void vfunc_21();  // [21] 0x005023e0  // __thiscall
    virtual void vfunc_22();  // [22] 0x00502440  // __thiscall
    virtual void vfunc_23(int a0, int a1, int a2);  // [23] 0x00502500  // __thiscall
    virtual void vfunc_24(int a0);  // [24] 0x00510cc0  // __thiscall
    virtual void vfunc_25();  // [25] 0x00478910  // __cdecl
    virtual void vfunc_26(int a0, int a1, int a2, int a3, int a4, int a5, int a6);  // [26] 0x00507150  // __thiscall
    virtual void vfunc_27(int a0, int a1, int a2, int a3);  // [27] 0x005a1440  // __stdcall
    virtual void vfunc_28(int a0, int a1, int a2, int a3);  // [28] 0x005a1890  // __thiscall
    virtual void vfunc_29(int a0);  // [29] 0x005a3aa0  // __thiscall
    virtual void vfunc_30(int a0);  // [30] 0x005a3c60  // __thiscall
    virtual void vfunc_31(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11);  // [31] 0x00476390  // __thiscall  // refs "Size"
    virtual void vfunc_32(int a0);  // [32] 0x00594820  // __stdcall, returns int
    virtual void vfunc_33(int a0, int a1, int a2, int a3);  // [33] 0x00594790  // __cdecl
    virtual void vfunc_34();  // [34] 0x004787f0  // __thiscall
    virtual void vfunc_35(int a0, int a1);  // [35] 0x005a0fd0  // __stdcall
    virtual void vfunc_36(int a0, int a1, int a2, int a3, int a4, int a5, int a6);  // [36] 0x004736b0  // __thiscall
    virtual void vfunc_37(int a0, int a1, int a2, int a3, int a4, int a5);  // [37] 0x00473930  // __thiscall
    virtual void vfunc_38();  // [38] 0x006829a5  // __cdecl
    virtual void vfunc_39(int a0, int a1, int a2);  // [39] 0x00473260  // __stdcall
    virtual void vfunc_40(int a0);  // [40] 0x004725c0
    virtual void vfunc_41(int a0);  // [41] 0x00402af0  // __stdcall
    virtual void vfunc_42(int a0);  // [42] 0x00474520  // __stdcall
    virtual void vfunc_43(int a0);  // [43] 0x00475960  // __stdcall
    virtual void vfunc_44(int a0, int a1, int a2, int a3);  // [44] 0x00474660  // __thiscall
    virtual void vfunc_45(int a0);  // [45] 0x004759d0  // __thiscall
    virtual void vfunc_46(int a0, int a1, int a2, int a3, int a4);  // [46] 0x00475070  // __thiscall
    virtual void vfunc_47(int a0);  // [47] 0x00474530  // __thiscall
    virtual void vfunc_48(int a0, int a1);  // [48] 0x005a0f20  // __stdcall
    virtual void vfunc_49(int a0);  // [49] 0x005a1460  // __stdcall
    virtual void vfunc_50();  // [50] 0x00593bb0  // __cdecl

    PowerClass();  // 0x00593aa0
    // The destructor is declared non-virtual here: slot 0 holds a stub
    // shared with 9 other classes (0x004b2de0), not the destructor.
    ~PowerClass();

    // ---- data members (offsets from ctor 0x00593aa0) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    // vtable-pointer stores are omitted: the compiler supplies the vptr.
    // The machine block begins at +0x14f4: the compiled end of the
    // base sub-object region. Evidence: RadarClass at +0x0 (ends +0x14f4).
    // 1 recovered offset(s) below +0x14f4 lie inside the base
    // sub-object region and are NOT declared here: they are declared and
    // compiler-checked in the base's own header -- +0x0
    unsigned char m_14f4;  // +0x14f4 /* ? */; 1-byte store: mov byte ptr [esi + 0x14f4], bl
    unsigned char pad_14f5[0x3];
    unsigned int m_14f8;  // +0x14f8 /* ? */; 4-byte store: mov dword ptr [esi + 0x14f8], eax
    unsigned char pad_14fc[0x4];
    unsigned int m_1500;  // +0x1500 /* ? */; 4-byte store: mov dword ptr [esi + 0x1500], ebx
    unsigned int m_1504;  // +0x1504 /* ? */; 4-byte store: mov dword ptr [esi + 0x1504], ebx
    unsigned int m_1508;  // +0x1508 /* ? */; 4-byte store: mov dword ptr [esi + 0x1508], eax
    unsigned char pad_150c[0x4];
    unsigned int m_1510;  // +0x1510 /* ? */; 4-byte store: mov dword ptr [esi + 0x1510], ebx
    unsigned int m_1514;  // +0x1514 /* ? */; 4-byte store: mov dword ptr [esi + 0x1514], ebx
    unsigned int m_1518;  // +0x1518 /* ? */; 4-byte store: mov dword ptr [esi + 0x1518], ebx
    unsigned int m_151c;  // +0x151c /* ? */; 4-byte store: mov dword ptr [esi + 0x151c], ebx
    unsigned char m_1520;  // +0x1520 /* ? */; 1-byte store: mov byte ptr [esi + 0x1520], bl
    unsigned char pad_1521[0x3];
    unsigned int m_1524;  // +0x1524 /* ? */; 4-byte store: mov dword ptr [esi + 0x1524], eax
    unsigned int m_1528;  // +0x1528 /* ? */; 4-byte store: mov dword ptr [esi + 0x1528], eax
};
