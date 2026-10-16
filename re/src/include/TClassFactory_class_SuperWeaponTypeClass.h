#pragma once
// ==========================================================================
// TClassFactory_class_SuperWeaponTypeClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class TClassFactory<class SuperWeaponTypeClass>
// vtable       : 0x006a48f8  (5 slots)
// constructor  : 0x005e8e30
// vtable store : 0x005e8e3b  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "SuperWeaponTypeClass.h"

class TClassFactory_class_SuperWeaponTypeClass : public IClassFactory
{
public:
    // ---- vtable 0x006a48f8 (5 slots) ----
    virtual void QueryInterface(int a0, int a1, int a2);  // [0] 0x005e8e40  // __stdcall, returns int
    virtual void AddRef(int a0);  // [1] 0x005e8eb0  // __stdcall
    virtual void Release(int a0);  // [2] 0x005e8ed0  // __stdcall
    virtual void vfunc_3(int a0, int a1, int a2, int a3);  // [3] 0x005e8f00  // __stdcall, returns int
    virtual void vfunc_4(int a0, int a1);  // [4] 0x005e8f80  // __stdcall, returns int

    TClassFactory_class_SuperWeaponTypeClass();  // 0x005e8e30
    virtual ~TClassFactory_class_SuperWeaponTypeClass();

    // ---- data members (offsets from ctor 0x005e8e30) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    unsigned long m_000;  // +0x000 init 0x6a48f8; 4-byte store: mov dword ptr [eax], 0x6a48f8
    unsigned long m_004;  // +0x004 zero-initialised; 4-byte store: mov dword ptr [eax + 4], 0
};
