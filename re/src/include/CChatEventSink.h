#pragma once
// ==========================================================================
// CChatEventSink.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class CChatEventSink
// vtable       : 0x006a78bc  (33 slots)
// constructor  : 0x006693e6
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
    virtual void vfunc_0();  // [0] 0x006829a5
    virtual void vfunc_1();  // [1] 0x006829a5
    virtual void vfunc_2();  // [2] 0x006829a5
    virtual void vfunc_3();  // [3] 0x006693f0
    virtual void vfunc_4();  // [4] 0x006699f0
    virtual void vfunc_5();  // [5] 0x00669d60
    virtual void vfunc_6();  // [6] 0x00669db0
    virtual void vfunc_7();  // [7] 0x0066a060
    virtual void vfunc_8();  // [8] 0x0066be10
    virtual void vfunc_9();  // [9] 0x0066a0d0
    virtual void vfunc_10();  // [10] 0x0066a210  // refs "Lob_18_"
    virtual void vfunc_11();  // [11] 0x0066ac70
    virtual void vfunc_12();  // [12] 0x0066c230
    virtual void vfunc_13();  // [13] 0x0066c1b0
    virtual void vfunc_14();  // [14] 0x0066c1f0
    virtual void vfunc_15();  // [15] 0x0066ac90
    virtual void vfunc_16();  // [16] 0x0066b920
    virtual void vfunc_17();  // [17] 0x0066b960
    virtual void vfunc_18();  // [18] 0x0066bcb0
    virtual void vfunc_19();  // [19] 0x0066bce0
    virtual void vfunc_20();  // [20] 0x00669d40
    virtual void vfunc_21();  // [21] 0x0066b5e0
    virtual void vfunc_22();  // [22] 0x0066b3f0
    virtual void vfunc_23();  // [23] 0x0066b140
    virtual void vfunc_24();  // [24] 0x0066c3f0
    virtual void vfunc_25();  // [25] 0x0066c240
    virtual void vfunc_26();  // [26] 0x0066bb30  // refs "Lob_18_"
    virtual void vfunc_27();  // [27] 0x0066bab0
    virtual void vfunc_28();  // [28] 0x0066b9a0
    virtual void vfunc_29();  // [29] 0x0066c250
    virtual void vfunc_30();  // [30] 0x0066c310
    virtual void vfunc_31();  // [31] 0x0066c2b0
    virtual void vfunc_32();  // [32] 0x00669c10

    CChatEventSink();  // 0x006693e6
    virtual ~CChatEventSink();
};
