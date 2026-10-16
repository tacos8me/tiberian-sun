#pragma once
// ==========================================================================
// CChatEventSink.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class CChatEventSink
// vtable       : 0x006a78bc  (33 slots)
// constructor  : 0x006693d0
// vtable store : 0x006693e6  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class CChatEventSink : public ATL::CComObjectRootEx<ATL::CComMultiThreadModel, void>, public ATL::CComObjectRootBase, public WOL::IChatEvent, public IUnknown
{
public:
    // ---- vtable 0x006a78bc (33 slots) ----
    virtual void vfunc_0();  // [0] 0x006829a5  // __cdecl
    virtual void vfunc_1();  // [1] 0x006829a5  // __cdecl
    virtual void vfunc_2();  // [2] 0x006829a5  // __cdecl
    virtual void vfunc_3(int a0, int a1, int a2);  // [3] 0x006693f0  // __stdcall
    virtual void vfunc_4(int a0, int a1, int a2, int a3);  // [4] 0x006699f0
    virtual void vfunc_5(int a0, int a1, int a2);  // [5] 0x00669d60  // __stdcall, returns int
    virtual void vfunc_6(int a0, int a1, int a2);  // [6] 0x00669db0  // __stdcall, returns int
    virtual void vfunc_7(int a0, int a1, int a2);  // [7] 0x0066a060  // __stdcall, returns int
    virtual void vfunc_8(int a0, int a1, int a2);  // [8] 0x0066be10  // __stdcall, returns int
    virtual void vfunc_9(int a0, int a1, int a2);  // [9] 0x0066a0d0  // __stdcall, returns int
    virtual void vfunc_10(int a0, int a1, int a2);  // [10] 0x0066a210  // refs "Lob_18_"
    virtual void vfunc_11(int a0, int a1, int a2, int a3);  // [11] 0x0066ac70  // __stdcall, returns int
    virtual void vfunc_12(int a0, int a1, int a2, int a3);  // [12] 0x0066c230  // __stdcall, returns int
    virtual void vfunc_13(int a0, int a1, int a2, int a3);  // [13] 0x0066c1b0  // __stdcall, returns int
    virtual void vfunc_14(int a0, int a1, int a2, int a3, int a4);  // [14] 0x0066c1f0  // __stdcall, returns int
    virtual void vfunc_15(int a0, int a1, int a2, int a3);  // [15] 0x0066ac90  // __stdcall, returns int
    virtual void vfunc_16(int a0, int a1, int a2, int a3, int a4);  // [16] 0x0066b920  // __stdcall, returns int
    virtual void vfunc_17(int a0, int a1, int a2, int a3);  // [17] 0x0066b960  // __stdcall, returns int
    virtual void vfunc_18(int a0, int a1, int a2);  // [18] 0x0066bcb0  // __stdcall, returns int
    virtual void vfunc_19(int a0, int a1);  // [19] 0x0066bce0  // __stdcall
    virtual void vfunc_20(int a0, int a1, int a2);  // [20] 0x00669d40  // __stdcall, returns int
    virtual void vfunc_21(int a0, int a1, int a2, int a3);  // [21] 0x0066b5e0  // __stdcall, returns int
    virtual void vfunc_22(int a0, int a1, int a2, int a3, int a4);  // [22] 0x0066b3f0
    virtual void vfunc_23(int a0, int a1, int a2, int a3);  // [23] 0x0066b140
    virtual void vfunc_24(int a0, int a1, int a2, int a3, int a4);  // [24] 0x0066c3f0  // __stdcall, returns int
    virtual void vfunc_25(int a0, int a1, int a2);  // [25] 0x0066c240  // __stdcall, returns int
    virtual void vfunc_26(int a0, int a1, int a2);  // [26] 0x0066bb30  // refs "Lob_18_"
    virtual void vfunc_27(int a0);  // [27] 0x0066bab0
    virtual void vfunc_28(int a0, int a1, int a2, int a3);  // [28] 0x0066b9a0  // __stdcall, returns int
    virtual void vfunc_29(int a0, int a1, int a2);  // [29] 0x0066c250  // __stdcall, returns int
    virtual void vfunc_30(int a0, int a1, int a2, int a3, int a4);  // [30] 0x0066c310  // __stdcall, returns int
    virtual void vfunc_31(int a0, int a1, int a2, int a3);  // [31] 0x0066c2b0  // __stdcall, returns int
    virtual void vfunc_32(int a0, int a1, int a2);  // [32] 0x00669c10

    CChatEventSink();  // 0x006693d0
    virtual ~CChatEventSink();

    // ---- data members (offsets from ctor 0x006693d0) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x6a78bc
    unsigned long m_004;  // zero-initialised
};
