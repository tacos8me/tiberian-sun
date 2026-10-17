#pragma once
// ==========================================================================
// FactoryClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class FactoryClass
// vtable       : 0x0069dff8  (24 slots)
// constructor  : 0x00492e80
// vtable store : 0x00492ee6  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "AbstractClass.h"
#include "StageClass.h"

class FactoryClass : public AbstractClass, public StageClass
{
public:
    // ---- vtable 0x0069dff8 (24 slots) ----
    virtual void QueryInterface(int a0, int a1, int a2);  // [0] 0x00405790  // __stdcall, returns int
    virtual void AddRef(int a0);  // [1] 0x00405830  // __stdcall, returns int
    virtual void Release(int a0);  // [2] 0x00405840  // __stdcall, returns int
    virtual void GetClassID(int a0, int a1);  // [3] 0x00493790  // __stdcall, returns int
    virtual void IsDirty(int a0);  // [4] 0x00405990  // __stdcall
    virtual void Load(int a0, int a1);  // [5] 0x004937d0  // __stdcall
    virtual void Save(int a0, int a1, int a2);  // [6] 0x00493910  // __stdcall
    virtual void GetSizeMax(int a0, int a1);  // [7] 0x00405910  // __stdcall, returns int
    virtual void Init();  // [9] 0x004059b0  // __cdecl
    virtual void Detach(int a0, int a1);  // [10] 0x00493ad0  // __stdcall
    virtual void What_Am_I();  // [11] 0x00493c90  // __cdecl, returns int
    virtual void Fetch_Object_Size();  // [12] 0x00493ca0  // __cdecl, returns int
    virtual void Compute_CRC(int a0);  // [13] 0x00493980  // __thiscall
    virtual void Owner();  // [14] 0x004059d0  // __cdecl
    virtual void Owner_HouseClass();  // [15] 0x004059e0  // __cdecl, returns int
    virtual void Fetch_Heap_ID();  // [16] 0x004059f0  // __cdecl, returns int
    virtual void Is_Inactive();  // [17] 0x00405970  // __cdecl
    virtual void Center_Coord(int a0);  // [18] 0x00405a00  // __stdcall
    virtual void Destination_Coord(int a0);  // [19] 0x00405a30  // __stdcall
    virtual void On_Ground();  // [20] 0x00405a60  // __cdecl
    virtual void In_Air();  // [21] 0x00405a70  // __cdecl
    virtual void As_Coord(int a0);  // [22] 0x00405a80  // __stdcall
    virtual void AI(int a0);  // [23] 0x004930c0  // __thiscall

    FactoryClass();  // 0x00492e80
    // The destructor is declared non-virtual here: slot 0 holds a stub
    // shared with 66 other classes (0x00405790), not the destructor.
    ~FactoryClass();

    // ---- data members (offsets from ctor 0x00492e80) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    // vtable-pointer stores are omitted: the compiler supplies the vptr.
    // The machine block begins at +0x18: the compiled end of the
    // base sub-object region. Evidence: AbstractClass at +0x0 (ends +0x11), StageClass at +0x14 (ends +0x18).
    // 2 recovered offset(s) below +0x18 lie inside the base
    // sub-object region and are NOT declared here: they are declared and
    // compiler-checked in the base's own header -- +0x0, +0x14
    unsigned int m_018;  // +0x018 /* ? */; 4-byte store: mov dword ptr [esi + 0x18], eax
    unsigned char pad_01c[0x4];
    unsigned int m_020;  // +0x020 /* ? */; 4-byte store: mov dword ptr [esi + 0x20], ebx
    unsigned int m_024;  // +0x024 /* ? */; 4-byte store: mov dword ptr [esi + 0x24], ebx
    unsigned int m_028;  // +0x028 /* ? */; 4-byte store: mov dword ptr [esi + 0x28], eax
    unsigned char pad_02c[0x4];
    unsigned int m_030;  // +0x030 /* ? */; 4-byte store: mov dword ptr [esi + 0x30], ebx
    unsigned int m_034;  // +0x034 /* ? */; 4-byte store: mov dword ptr [esi + 0x34], ebx
    unsigned char m_038;  // +0x038 /* ? */; 1-byte store: mov byte ptr [esi + 0x38], al
    unsigned char m_039;  // +0x039 /* ? */; 1-byte store: mov byte ptr [esi + 0x39], bl
    unsigned char pad_03a[0x2];
    unsigned int m_03c;  // +0x03c /* ? */; 4-byte store: mov dword ptr [esi + 0x3c], ebx
    unsigned int m_040;  // +0x040 init 0xa; 4-byte store: mov dword ptr [esi + 0x40], 0xa
    unsigned int m_044;  // +0x044 /* ? */; 4-byte store: mov dword ptr [esi + 0x44], ebx
    unsigned char m_048;  // +0x048 /* ? */; 1-byte store: mov byte ptr [esi + 0x48], bl
    unsigned char pad_049[0x3];
    unsigned int m_04c;  // +0x04c /* ? */; 4-byte store: mov dword ptr [esi + 0x4c], ebx
    unsigned int m_050;  // +0x050 /* ? */; 4-byte store: mov dword ptr [esi + 0x50], ebx
    unsigned int m_054;  // +0x054 init 0xffffffff; 4-byte store: mov dword ptr [esi + 0x54], 0xffffffff
    unsigned int m_058;  // +0x058 /* ? */; 4-byte store: mov dword ptr [esi + 0x58], ebx
    unsigned char m_05c;  // +0x05c /* ? */; 1-byte store: mov byte ptr [esi + 0x5c], bl
    unsigned char m_05d;  // +0x05d /* ? */; 1-byte store: mov byte ptr [esi + 0x5d], al
};
