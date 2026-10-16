#pragma once
// ==========================================================================
// MouseClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class MouseClass
// vtable       : 0x00698754  (51 slots)
// constructor  : 0x004027e9
// vtable store : 0x00402a68  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "ScrollClass.h"

class MouseClass : public ScrollClass
{
public:
    // ---- vtable 0x00698754 (51 slots) ----
    virtual void vfunc_0(int a0, int a1, int a2);  // [0] 0x004b2de0  // __stdcall, returns int
    virtual void vfunc_1(int a0);  // [1] 0x00402ad0  // __stdcall, returns int
    virtual void vfunc_2(int a0);  // [2] 0x00402ae0  // __stdcall, returns int
    virtual void vfunc_3(int a0, int a1);  // [3] 0x00502000  // __stdcall
    virtual void vfunc_5();  // [5] 0x0054f650  // __cdecl
    virtual void vfunc_6(int a0);  // [6] 0x004b2e50  // __thiscall
    virtual void vfunc_7();  // [7] 0x0054f670  // __thiscall
    virtual void vfunc_8();  // [8] 0x00402b00  // __cdecl
    virtual void vfunc_9(int a0, int a1, int a2);  // [9] 0x004b2ec0  // __thiscall
    virtual void vfunc_10(int a0, int a1);  // [10] 0x0054f4e0  // __thiscall
    virtual void vfunc_11(int a0);  // [11] 0x004b2f90  // __stdcall
    virtual void vfunc_12(int a0);  // [12] 0x004b2fd0  // __stdcall
    virtual void vfunc_13(int a0);  // [13] 0x004b2e90  // __stdcall
    virtual void vfunc_14(int a0, int a1);  // [14] 0x004b2ff0  // __thiscall
    virtual void vfunc_15(int a0, int a1, int a2, int a3, int a4, int a5, int a6);  // [15] 0x005f40b0  // __thiscall
    virtual void vfunc_16();  // [16] 0x004b3100  // __cdecl
    virtual void vfunc_17(int a0, int a1);  // [17] 0x0054f1e0  // __stdcall
    virtual void vfunc_18(int a0, int a1);  // [18] 0x0054f3a0  // __thiscall
    virtual void vfunc_19();  // [19] 0x0054f200  // __cdecl
    virtual void vfunc_20();  // [20] 0x0054f210
    virtual void vfunc_21();  // [21] 0x005023e0  // __thiscall
    virtual void vfunc_22();  // [22] 0x00502440  // __thiscall
    virtual void vfunc_23(int a0, int a1, int a2);  // [23] 0x00502500  // __thiscall
    virtual void vfunc_24(int a0);  // [24] 0x00510cc0  // __thiscall
    virtual void vfunc_25();  // [25] 0x005cfd40  // __cdecl
    virtual void vfunc_26(int a0, int a1, int a2, int a3, int a4, int a5, int a6);  // [26] 0x00507150  // __thiscall
    virtual void vfunc_27(int a0, int a1, int a2, int a3);  // [27] 0x005a1440  // __stdcall
    virtual void vfunc_28(int a0, int a1, int a2, int a3);  // [28] 0x005a1890  // __thiscall
    virtual void vfunc_30(int a0);  // [30] 0x0054ff20  // __thiscall
    virtual void vfunc_31(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11);  // [31] 0x00476390  // __thiscall  // refs "Size"
    virtual void vfunc_32(int a0);  // [32] 0x005dca40  // __thiscall
    virtual void vfunc_33(int a0);  // [33] 0x005dc4a0
    virtual void vfunc_34();  // [34] 0x005d04a0  // __thiscall
    virtual void vfunc_35(int a0, int a1);  // [35] 0x005a0fd0  // __stdcall
    virtual void vfunc_36(int a0, int a1, int a2, int a3, int a4, int a5, int a6);  // [36] 0x004736b0  // __thiscall
    virtual void vfunc_37(int a0, int a1, int a2, int a3, int a4, int a5);  // [37] 0x00473930  // __thiscall
    virtual void vfunc_38();  // [38] 0x00402b10  // __cdecl
    virtual void vfunc_39(int a0, int a1, int a2);  // [39] 0x00473260  // __stdcall
    virtual void vfunc_40(int a0);  // [40] 0x004725c0
    virtual void vfunc_41(int a0);  // [41] 0x00402af0  // __stdcall
    virtual void vfunc_42(int a0);  // [42] 0x005d0460  // __thiscall
    virtual void vfunc_43(int a0);  // [43] 0x00475960  // __stdcall
    virtual void vfunc_44(int a0, int a1, int a2, int a3);  // [44] 0x00474660  // __thiscall
    virtual void vfunc_45(int a0);  // [45] 0x004759d0  // __thiscall
    virtual void vfunc_46(int a0, int a1, int a2, int a3, int a4);  // [46] 0x00475070  // __thiscall
    virtual void vfunc_47(int a0);  // [47] 0x005d0420  // __thiscall
    virtual void vfunc_48(int a0, int a1);  // [48] 0x005a0f20  // __stdcall
    virtual void vfunc_49(int a0);  // [49] 0x005a1460  // __stdcall
    virtual void vfunc_50();  // [50] 0x005f4700  // __thiscall

    MouseClass();  // 0x004027e9
    virtual ~MouseClass();

    // ---- data members (offsets from ctor 0x00402b20) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x698754
};
