#pragma once
// ==========================================================================
// CheckListClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class CheckListClass
// vtable       : 0x0069aeb8  (50 slots)
// constructor  : 0x0045ab54
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
    virtual void vfunc_29(int a0, int a1);  // [29] 0x0045adf0  // __thiscall, returns int
    virtual void vfunc_30(int a0, int a1, int a2, int a3);  // [30] 0x004a5a60  // __stdcall, returns int
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

    CheckListClass();  // 0x0045ab54
    virtual ~CheckListClass();

    // ---- data members (offsets from ctor 0x0045ac20) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69aeb8
};
