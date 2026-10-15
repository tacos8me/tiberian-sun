#pragma once
// ==========================================================================
// ATL_CComObject_class_CNetUtilEventSink.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class ATL::CComObject<class CNetUtilEventSink>
// vtable       : 0x006a7970  (8 slots)
// constructor  : 0x0066ca3b
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class ATL_CComObject_class_CNetUtilEventSink : public CNetUtilEventSink, public ATL::CComObjectRootEx<ATL::CComMultiThreadModel, void>, public ATL::CComObjectRootBase, public WOL::INetUtilEvent, public IUnknown
{
public:
    // ---- vtable 0x006a7970 (8 slots) ----
    virtual void vfunc_0();  // [0] 0x0066d240
    virtual void vfunc_1();  // [1] 0x0066d1d0
    virtual void vfunc_2();  // [2] 0x0066d1f0
    virtual void vfunc_3();  // [3] 0x0066d180
    virtual void vfunc_4();  // [4] 0x0066ca60
    virtual void vfunc_5();  // [5] 0x0066ca50
    virtual void vfunc_6();  // [6] 0x0066d040
    virtual void vfunc_7();  // [7] 0x0066d120  // refs "OnAgeCheck"

    ATL_CComObject_class_CNetUtilEventSink();  // 0x0066ca3b
    virtual ~ATL_CComObject_class_CNetUtilEventSink();
};
