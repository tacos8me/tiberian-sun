#pragma once
// ==========================================================================
// TClassFactory_class_TriggerClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class TClassFactory<class TriggerClass>
// vtable       : 0x006a4a78  (5 slots)
// constructor  : 0x005ea630
// vtable store : 0x005ea63b  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "TriggerClass.h"

class TClassFactory_class_TriggerClass : public IClassFactory
{
public:
    // ---- vtable 0x006a4a78 (5 slots) ----
    virtual void vfunc_0(int a0, int a1, int a2);  // [0] 0x005ea640  // __stdcall, returns int
    virtual void vfunc_1(int a0);  // [1] 0x005ea6b0  // __stdcall
    virtual void vfunc_2(int a0);  // [2] 0x005ea6d0  // __stdcall
    virtual void vfunc_3(int a0, int a1, int a2, int a3);  // [3] 0x005ea700  // __stdcall, returns int
    virtual void vfunc_4(int a0, int a1);  // [4] 0x005ea780  // __stdcall, returns int

    TClassFactory_class_TriggerClass();  // 0x005ea630
    virtual ~TClassFactory_class_TriggerClass();

    // ---- data members (offsets from ctor 0x005ea630) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    unsigned long m_000;  // +0x000 init 0x6a4a78; 4-byte store: mov dword ptr [eax], 0x6a4a78
    unsigned long m_004;  // +0x004 zero-initialised; 4-byte store: mov dword ptr [eax + 4], 0
};
