#pragma once
// ==========================================================================
// non_rtti_object.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class __non_rtti_object
// vtable       : 0x006a7d40  (2 slots)
// constructor  : 0x00682f40
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "bad_typeid.h"

class non_rtti_object : public bad_typeid
{
public:
    // ---- vtable 0x006a7d40 (2 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x00682f4b  // __thiscall
    virtual void vfunc_1();  // [1] 0x006878ac  // __cdecl, returns int

    non_rtti_object();  // 0x00682f40
    virtual ~non_rtti_object();

    // ---- data members (offsets from ctor 0x00682f29) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x6a7d40
};
