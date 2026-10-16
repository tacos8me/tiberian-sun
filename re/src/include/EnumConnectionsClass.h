#pragma once
// ==========================================================================
// EnumConnectionsClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class EnumConnectionsClass
// vtable       : 0x0069b830  (7 slots)
// constructor  : 0x0046b8e0
// vtable store : 0x0046b909  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class EnumConnectionsClass : public IEnumConnections
{
public:
    // ---- vtable 0x0069b830 (7 slots) ----
    virtual void QueryInterface(int a0, int a1, int a2);  // [0] 0x0046b850  // __stdcall, returns int
    virtual void AddRef(int a0);  // [1] 0x0046b8c0  // __stdcall
    virtual void Release(int a0);  // [2] 0x0046b8e0  // __stdcall
    virtual void vfunc_3(int a0, int a1, int a2, int a3);  // [3] 0x0046b980  // __stdcall, returns int
    virtual void vfunc_4(int a0, int a1);  // [4] 0x0046ba30  // __stdcall, returns int
    virtual void vfunc_5(int a0);  // [5] 0x0046ba50  // __stdcall, returns int
    virtual void vfunc_6(int a0, int a1);  // [6] 0x0046ba60  // __stdcall, returns int

    EnumConnectionsClass();  // 0x0046b8e0
    virtual ~EnumConnectionsClass();

    // ---- data members (offsets from ctor 0x0046b8e0) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    unsigned long m_000;  // +0x000 init 0x69b830; 4-byte store: mov dword ptr [edi], 0x69b830
};
