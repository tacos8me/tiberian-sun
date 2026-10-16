#pragma once
// ==========================================================================
// ATL_CComObject_class_CNetUtilEventSink.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class ATL::CComObject<class CNetUtilEventSink>
// vtable       : 0x006a7970  (8 slots)
// constructor  : 0x0066ca20
// vtable store : 0x0066ca3b  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "ts_prelude.h"

class ATL_CComObject_class_CNetUtilEventSink : public CNetUtilEventSink
{
public:
    // ---- vtable 0x006a7970 (8 slots) ----
    virtual void vfunc_0(int a0, int a1, int a2);  // [0] 0x0066d240  // __stdcall, returns int
    virtual void vfunc_1(int a0);  // [1] 0x0066d1d0  // __stdcall
    virtual void vfunc_3(int a0, int a1, int a2, int a3, int a4);  // [3] 0x0066d180  // __stdcall, returns int
    virtual void vfunc_4(int a0, int a1);  // [4] 0x0066ca60
    virtual void vfunc_5(int a0, int a1);  // [5] 0x0066ca50  // __stdcall, returns int
    virtual void vfunc_6(int a0, int a1, int a2, int a3, int a4, int a5);  // [6] 0x0066d040
    virtual void vfunc_7(int a0, int a1, int a2, int a3);  // [7] 0x0066d120  // __stdcall, returns int  // refs "OnAgeCheck"

    ATL_CComObject_class_CNetUtilEventSink();  // 0x0066ca20
    virtual ~ATL_CComObject_class_CNetUtilEventSink();

    // ---- data members (offsets from ctor 0x0066ca20) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x6a7970
    unsigned long m_004;  // zero-initialised
};
