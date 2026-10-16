#pragma once
// ==========================================================================
// ATL_CComObject_class_CDownloadEventSink.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class ATL::CComObject<class CDownloadEventSink>
// vtable       : 0x006a794c  (8 slots)
// constructor  : 0x0066c6b0
// vtable store : 0x0066c6d6  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "ts_prelude.h"

class ATL_CComObject_class_CDownloadEventSink : public CDownloadEventSink
{
public:
    // ---- vtable 0x006a794c (8 slots) ----
    virtual void vfunc_0(int a0, int a1, int a2);  // [0] 0x0066c920  // __stdcall, returns int
    virtual void vfunc_1(int a0);  // [1] 0x0066c8b0  // __stdcall
    virtual void vfunc_3(int a0);  // [3] 0x0066c720  // __stdcall, returns int
    virtual void vfunc_4(int a0, int a1);  // [4] 0x0066c740  // __stdcall, returns int
    virtual void vfunc_5(int a0, int a1, int a2, int a3, int a4, int a5);  // [5] 0x0066c760
    virtual void vfunc_6(int a0);  // [6] 0x0066c8a0  // __stdcall, returns int
    virtual void vfunc_7(int a0);  // [7] 0x0066c830

    ATL_CComObject_class_CDownloadEventSink();  // 0x0066c6b0
    virtual ~ATL_CComObject_class_CDownloadEventSink();

    // ---- data members (offsets from ctor 0x0066c6b0) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x6a794c
};
