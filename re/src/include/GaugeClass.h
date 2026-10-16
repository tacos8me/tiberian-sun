#pragma once
// ==========================================================================
// GaugeClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class GaugeClass
// vtable       : 0x0069e804  (40 slots)
// constructor  : 0x004a69a0
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
    virtual void vfunc_0(int a0);  // [0] 0x004a75c0  // __thiscall
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
    virtual void vfunc_19(int a0, int a1, int a2);  // [19] 0x00460650  // __stdcall
    virtual void vfunc_20();  // [20] 0x004a5e60  // __thiscall
    virtual void vfunc_21();  // [21] 0x004a5e90  // __cdecl
    virtual void vfunc_22();  // [22] 0x004a5eb0  // __cdecl, returns int
    virtual void vfunc_23();  // [23] 0x004a5ec0  // __cdecl, returns int
    virtual void vfunc_24();  // [24] 0x0045aff0  // __cdecl
    virtual void vfunc_25(int a0, int a1);  // [25] 0x004a5ee0  // __stdcall
    virtual void vfunc_26(int a0, int a1);  // [26] 0x004a5f00  // __stdcall
    virtual void vfunc_27(int a0, int a1, int a2, int a3);  // [27] 0x004a6b90  // __thiscall
    virtual void vfunc_28(int a0);  // [28] 0x004a5e30  // __stdcall
    virtual void vfunc_29(int a0, int a1);  // [29] 0x004a6d30  // __thiscall, returns int
    virtual void vfunc_30(int a0, int a1, int a2, int a3);  // [30] 0x004a5a60  // __stdcall, returns int
    virtual void vfunc_31(int a0);  // [31] 0x00460600  // __stdcall
    virtual void vfunc_32(int a0);  // [32] 0x004a6a80  // __stdcall, returns int
    virtual void vfunc_33(int a0);  // [33] 0x004a6aa0  // __stdcall, returns int
    virtual void vfunc_34();  // [34] 0x004a7590  // __cdecl
    virtual void vfunc_35(int a0);  // [35] 0x004a75a0  // __stdcall
    virtual void vfunc_36();  // [36] 0x004a75b0  // __cdecl, returns int
    virtual void vfunc_37(int a0, int a1);  // [37] 0x004a6e90  // __thiscall
    virtual void vfunc_38(int a0);  // [38] 0x004a6ad0
    virtual void vfunc_39(int a0);  // [39] 0x004a6b50

    GaugeClass();  // 0x004a69a0
    virtual ~GaugeClass();

    // ---- data members (offsets from ctor 0x004a69f7) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69e804
    unsigned char pad_004[0x28];
    unsigned long m_02c;  // init 0x1
    unsigned long m_030;  // /* ? */
    unsigned long m_034;  // /* ? */
    unsigned long m_038;  // zero-initialised
};
