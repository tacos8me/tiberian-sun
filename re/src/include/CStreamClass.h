#pragma once
// ==========================================================================
// CStreamClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class CStreamClass
// vtable       : 0x0069b91c  (15 slots)
// constructor  : 0x0046e040
// vtable store : 0x0046e080  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class CStreamClass : public IStream, public ILinkStream
{
public:
    // ---- vtable 0x0069b91c (15 slots) ----
    virtual void QueryInterface(int a0, int a1, int a2);  // [0] 0x0046e1b0  // __stdcall, returns int
    virtual void AddRef(int a0);  // [1] 0x0046e150  // __stdcall
    virtual void Release(int a0);  // [2] 0x0046e170  // __stdcall
    virtual void vfunc_3(int a0, int a1, int a2, int a3);  // [3] 0x0046e380  // __stdcall, returns int
    virtual void vfunc_4(int a0, int a1, int a2, int a3);  // [4] 0x0046e4f0  // __stdcall, returns int
    virtual void vfunc_5(int a0, int a1, int a2, int a3, int a4);  // [5] 0x0046e620  // __stdcall
    virtual void vfunc_6(int a0, int a1, int a2);  // [6] 0x0046e670  // __stdcall
    virtual void vfunc_7(int a0, int a1, int a2, int a3, int a4, int a5);  // [7] 0x0046e6c0  // __stdcall
    virtual void vfunc_8(int a0, int a1);  // [8] 0x0046e700  // __stdcall
    virtual void vfunc_9(int a0);  // [9] 0x0046e730  // __stdcall
    virtual void vfunc_10(int a0, int a1, int a2, int a3, int a4, int a5);  // [10] 0x0046e760  // __stdcall
    virtual void vfunc_11(int a0, int a1, int a2, int a3, int a4, int a5);  // [11] 0x0046e7a0  // __stdcall
    virtual void vfunc_12(int a0, int a1, int a2);  // [12] 0x0046e7e0  // __stdcall
    virtual void vfunc_13(int a0, int a1);  // [13] 0x0046e810  // __stdcall

    CStreamClass();  // 0x0046e040
    virtual ~CStreamClass();

    // ---- data members (offsets from ctor 0x0046e040) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    // vtable-pointer stores are omitted: the compiler supplies the vptr.
    unsigned int m_008;  // +0x008 /* ? */; 4-byte store: mov dword ptr [esi + 8], eax
    unsigned int m_00c;  // +0x00c /* ? */; 4-byte store: mov dword ptr [esi + 0xc], eax
    unsigned char m_010;  // +0x010 /* ? */; 1-byte store: mov byte ptr [esi + 0x10], al
    unsigned char m_011;  // +0x011 /* ? */; 1-byte store: mov byte ptr [esi + 0x11], al
    unsigned char pad_012[0x2];
    unsigned int m_014;  // +0x014 /* ? */; 4-byte store: mov dword ptr [esi + 0x14], eax
    unsigned int m_018;  // +0x018 /* ? */; 4-byte store: mov dword ptr [esi + 0x18], eax
    unsigned int m_01c;  // +0x01c /* ? */; 4-byte store: mov dword ptr [esi + 0x1c], eax
    unsigned int m_020;  // +0x020 /* ? */; 4-byte store: mov dword ptr [esi + 0x20], eax
    unsigned int m_024;  // +0x024 init 0xffff; 4-byte store: mov dword ptr [esi + 0x24], 0xffff
};
