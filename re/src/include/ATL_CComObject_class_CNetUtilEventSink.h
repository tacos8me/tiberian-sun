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
    virtual void vfunc_1(int a0);  // [1] 0x0066d1d0  // __stdcall, returns int
    virtual void vfunc_3(int a0, int a1, int a2, int a3, int a4);  // [3] 0x0066d180  // __stdcall, returns int
    virtual void vfunc_4(int a0, int a1, int a2, int a3, int a4, int a5);  // [4] 0x0066ca60  // returns char
    virtual void vfunc_5(int a0, int a1);  // [5] 0x0066ca50  // __stdcall, returns int
    virtual void vfunc_6(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7);  // [6] 0x0066d040  // returns ?
    virtual void vfunc_7(int a0, int a1, int a2, int a3);  // [7] 0x0066d120  // __stdcall, returns int  // refs "OnAgeCheck"

    ATL_CComObject_class_CNetUtilEventSink();  // 0x0066ca20
    virtual ~ATL_CComObject_class_CNetUtilEventSink();

    // ---- data members (offsets from ctor 0x0066ca20) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    // vtable-pointer stores are omitted: the compiler supplies the vptr.
    // The machine block begins at +0x4: the compiled end of the
    // base sub-object region. Evidence: CNetUtilEventSink at +0x0 (ends +0x4).
    // 1 recovered offset(s) below +0x4 lie inside the base
    // sub-object region and are NOT declared here: they are declared and
    // compiler-checked in the base's own header -- +0x0
    unsigned int m_004;  // +0x004 zero-initialised; 4-byte store: mov dword ptr [esi + 4], 0
};
