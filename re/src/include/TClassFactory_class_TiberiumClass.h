#pragma once
// ==========================================================================
// TClassFactory_class_TiberiumClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class TClassFactory<class TiberiumClass>
// vtable       : 0x006a49a0  (5 slots)
// constructor  : 0x005e98b0
// vtable store : 0x005e98bb  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
class TiberiumClass;

class TClassFactory_class_TiberiumClass : public IClassFactory
{
public:
    // ---- vtable 0x006a49a0 (5 slots) ----
    virtual void QueryInterface(int a0, int a1, int a2);  // [0] 0x005e98c0  // __stdcall, returns int
    virtual void AddRef(int a0);  // [1] 0x005e9930  // __stdcall
    virtual void Release(int a0);  // [2] 0x005e9950  // __stdcall
    virtual void vfunc_3(int a0, int a1, int a2, int a3);  // [3] 0x005e9980  // __stdcall, returns int
    virtual void vfunc_4(int a0, int a1);  // [4] 0x005e9a00  // __stdcall, returns int

    TClassFactory_class_TiberiumClass();  // 0x005e98b0
    virtual ~TClassFactory_class_TiberiumClass();

    // ---- data members (offsets from ctor 0x005e98b0) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    // vtable-pointer stores are omitted: the compiler supplies the vptr.
    unsigned int m_004;  // +0x004 zero-initialised; 4-byte store: mov dword ptr [eax + 4], 0
};
