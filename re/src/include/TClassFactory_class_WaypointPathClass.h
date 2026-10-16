#pragma once
// ==========================================================================
// TClassFactory_class_WaypointPathClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class TClassFactory<class WaypointPathClass>
// vtable       : 0x006a4a00  (5 slots)
// constructor  : 0x005e9eb0
// vtable store : 0x005e9ebb  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
class WaypointPathClass;

class TClassFactory_class_WaypointPathClass : public IClassFactory
{
public:
    // ---- vtable 0x006a4a00 (5 slots) ----
    virtual void vfunc_0(int a0, int a1, int a2);  // [0] 0x005e9ec0  // __stdcall, returns int
    virtual void vfunc_1(int a0);  // [1] 0x005e9f30  // __stdcall
    virtual void vfunc_2(int a0);  // [2] 0x005e9f50  // __stdcall
    virtual void vfunc_3(int a0, int a1, int a2, int a3);  // [3] 0x005e9f80  // __stdcall, returns int
    virtual void vfunc_4(int a0, int a1);  // [4] 0x005ea000  // __stdcall, returns int

    TClassFactory_class_WaypointPathClass();  // 0x005e9eb0
    virtual ~TClassFactory_class_WaypointPathClass();

    // ---- data members (offsets from ctor 0x005e9eb0) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    unsigned long m_000;  // +0x000 init 0x6a4a00; 4-byte store: mov dword ptr [eax], 0x6a4a00
    unsigned long m_004;  // +0x004 zero-initialised; 4-byte store: mov dword ptr [eax + 4], 0
};
