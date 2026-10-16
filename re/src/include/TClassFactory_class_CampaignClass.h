#pragma once
// ==========================================================================
// TClassFactory_class_CampaignClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class TClassFactory<class CampaignClass>
// vtable       : 0x006a49d0  (5 slots)
// constructor  : 0x005e9bb0
// vtable store : 0x005e9bbb  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "CampaignClass.h"

class TClassFactory_class_CampaignClass : public IClassFactory
{
public:
    // ---- vtable 0x006a49d0 (5 slots) ----
    virtual void vfunc_0(int a0, int a1, int a2);  // [0] 0x005e9bc0  // __stdcall, returns int
    virtual void vfunc_1(int a0);  // [1] 0x005e9c30  // __stdcall
    virtual void vfunc_2(int a0);  // [2] 0x005e9c50  // __stdcall
    virtual void vfunc_3(int a0, int a1, int a2, int a3);  // [3] 0x005e9c80  // __stdcall, returns int
    virtual void vfunc_4(int a0, int a1);  // [4] 0x005e9d00  // __stdcall, returns int

    TClassFactory_class_CampaignClass();  // 0x005e9bb0
    virtual ~TClassFactory_class_CampaignClass();

    // ---- data members (offsets from ctor 0x005e9bb0) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    unsigned long m_000;  // +0x000 init 0x6a49d0; 4-byte store: mov dword ptr [eax], 0x6a49d0
    unsigned long m_004;  // +0x004 zero-initialised; 4-byte store: mov dword ptr [eax + 4], 0
};
