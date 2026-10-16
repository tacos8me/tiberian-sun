#pragma once
// ==========================================================================
// CounterClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class CounterClass
// vtable       : 0x0069b7ec  (6 slots)
// constructor  : 0x0046b270
// vtable store : 0x0046b283  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "VectorClass.h"

class H;  // RTTI formal param, no type evidence
class CounterClass : public VectorClass<H>
{
public:
    // ---- vtable 0x0069b7ec (6 slots) ----
    virtual void vfunc_1(int a0);  // [1] 0x0044af30  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x0044af70  // __thiscall
    virtual void vfunc_3();  // [3] 0x0046b3e0  // __cdecl
    virtual void vfunc_4(int a0);  // [4] 0x0044b050  // __stdcall, returns int
    virtual void vfunc_5(int a0);  // [5] 0x0044b090  // __stdcall, returns int

    CounterClass();  // 0x0046b270
    virtual ~CounterClass();

    // ---- data members (offsets from ctor 0x0046b270) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    unsigned long m_000;  // +0x000 init 0x69b7ec; 4-byte store: mov dword ptr [eax], 0x69b7ec
    unsigned long m_004;  // +0x004 /* ? */; 4-byte store: mov dword ptr [eax + 4], ecx
    unsigned long m_008;  // +0x008 /* ? */; 4-byte store: mov dword ptr [eax + 8], ecx
    unsigned char m_00c;  // +0x00c init 0x1; 1-byte store: mov byte ptr [eax + 0xc], 1
    unsigned char m_00d;  // +0x00d /* ? */; 1-byte store: mov byte ptr [eax + 0xd], cl
};
