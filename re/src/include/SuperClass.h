#pragma once
// ==========================================================================
// SuperClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class SuperClass
// vtable       : 0x006a4f88  (24 slots)
// constructor  : 0x005f12e0
// vtable store : 0x005f1315  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "AbstractClass.h"

class SuperClass : public AbstractClass
{
public:
    // ---- vtable 0x006a4f88 (24 slots) ----
    virtual void QueryInterface(int a0, int a1, int a2);  // [0] 0x00405790  // __stdcall, returns int
    virtual void Init(int a0);  // [1] 0x00405830  // __stdcall, returns int
    virtual void Shutdown(int a0);  // [2] 0x00405840  // __stdcall, returns int
    virtual void vfunc_3(int a0, int a1);  // [3] 0x005f26e0  // __stdcall, returns int
    virtual void Release(int a0);  // [4] 0x00405990  // __stdcall
    virtual void vfunc_5(int a0, int a1);  // [5] 0x005f2720  // __stdcall, returns int
    virtual void vfunc_6(int a0, int a1, int a2);  // [6] 0x005f2770  // __stdcall, returns int
    virtual void GetType(int a0, int a1);  // [7] 0x00405910  // __stdcall, returns int
    virtual void Set_ID();  // [9] 0x004059b0  // __cdecl
    virtual void Get_RTTI(int a0, int a1);  // [10] 0x005f2790  // __stdcall
    virtual void What_Am_I();  // [11] 0x005f2820  // __cdecl, returns int
    virtual void Get_Heap();  // [12] 0x005f2830  // __cdecl, returns int
    virtual void Load(int a0);  // [13] 0x005f27b0  // __thiscall
    virtual void Save();  // [14] 0x004059d0  // __cdecl
    virtual void GetSizeMax();  // [15] 0x004059e0  // __cdecl, returns int
    virtual void Compute_CRC();  // [16] 0x004059f0  // __cdecl, returns int
    virtual void vfunc_17();  // [17] 0x00405970  // __cdecl
    virtual void vfunc_18(int a0);  // [18] 0x00405a00  // __stdcall
    virtual void vfunc_19(int a0);  // [19] 0x00405a30  // __stdcall
    virtual void vfunc_20();  // [20] 0x00405a60  // __cdecl
    virtual void vfunc_21();  // [21] 0x00405a70  // __cdecl
    virtual void vfunc_22(int a0);  // [22] 0x00405a80  // __stdcall
    virtual void vfunc_23();  // [23] 0x00405ab0  // __cdecl

    SuperClass();  // 0x005f12e0
    // The destructor is declared non-virtual here: slot 0 holds a stub
    // shared with 66 other classes (0x00405790), not the destructor.
    ~SuperClass();

    // ---- data members (offsets from ctor 0x005f12e0) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    // vtable-pointer stores are omitted: the compiler supplies the vptr.
    // The machine block begins at +0x11: the compiled end of the
    // base sub-object region. Evidence: AbstractClass at +0x0 (ends +0x11).
    // 2 recovered offset(s) below +0x11 lie inside the base
    // sub-object region and are NOT declared here: they are declared and
    // compiler-checked in the base's own header -- +0x0, +0x4
    unsigned char pad_011[0x3];
    unsigned int m_014;  // +0x014 /* ? */; 4-byte store: mov dword ptr [esi + 0x14], eax
    unsigned int m_018;  // +0x018 /* ? */; 4-byte store: mov dword ptr [esi + 0x18], eax
    unsigned int m_01c;  // +0x01c /* ? */; 4-byte store: mov dword ptr [esi + 0x1c], ecx
    unsigned char pad_020[0x4];
    unsigned int m_024;  // +0x024 /* ? */; 4-byte store: mov dword ptr [esi + 0x24], eax
    unsigned char m_028;  // +0x028 init 0x1; 1-byte store: mov byte ptr [esi + 0x28], 1
    unsigned char m_029;  // +0x029 /* ? */; 1-byte store: mov byte ptr [esi + 0x29], al
    unsigned char m_02a;  // +0x02a /* ? */; 1-byte store: mov byte ptr [esi + 0x2a], al
    unsigned char m_02b;  // +0x02b /* ? */; 1-byte store: mov byte ptr [esi + 0x2b], al
    unsigned char m_02c;  // +0x02c /* ? */; 1-byte store: mov byte ptr [esi + 0x2c], al
    unsigned char pad_02d[0x7];
    unsigned int m_034;  // +0x034 init 0xffffffff; 4-byte store: mov dword ptr [esi + 0x34], 0xffffffff
};
