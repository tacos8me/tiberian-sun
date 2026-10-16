#pragma once
// ==========================================================================
// TClassFactory_class_TagClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class TClassFactory<class TagClass>
// vtable       : 0x006a4ad8  (5 slots)
// constructor  : 0x005eac30
// vtable store : 0x005eac3b  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
class TagClass;

class TClassFactory_class_TagClass : public IClassFactory
{
public:
    // ---- vtable 0x006a4ad8 (5 slots) ----
    virtual void QueryInterface(int a0, int a1, int a2);  // [0] 0x005eac40  // __stdcall, returns int
    virtual void AddRef(int a0);  // [1] 0x005eacb0  // __stdcall
    virtual void Release(int a0);  // [2] 0x005eacd0  // __stdcall
    virtual void vfunc_3(int a0, int a1, int a2, int a3);  // [3] 0x005ead00  // __stdcall, returns int
    virtual void vfunc_4(int a0, int a1);  // [4] 0x005ead80  // __stdcall, returns int

    TClassFactory_class_TagClass();  // 0x005eac30
    virtual ~TClassFactory_class_TagClass();

    // ---- data members (offsets from ctor 0x005eac30) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    unsigned long m_000;  // +0x000 init 0x6a4ad8; 4-byte store: mov dword ptr [eax], 0x6a4ad8
    unsigned long m_004;  // +0x004 zero-initialised; 4-byte store: mov dword ptr [eax + 4], 0
};
