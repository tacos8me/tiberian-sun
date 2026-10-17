#pragma once
// ==========================================================================
// Tactical.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class Tactical
// vtable       : 0x006a5298  (25 slots)
// constructor  : 0x005f4b30
// vtable store : 0x005f4c45  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "AbstractClass.h"

class Tactical : public AbstractClass
{
public:
    // ---- vtable 0x006a5298 (25 slots) ----
    virtual void QueryInterface(int a0, int a1, int a2);  // [0] 0x00405790  // __stdcall, returns int
    virtual void AddRef(int a0);  // [1] 0x00405830  // __stdcall, returns int
    virtual void Release(int a0);  // [2] 0x00405840  // __stdcall, returns int
    virtual void GetClassID(int a0, int a1);  // [3] 0x005fd160  // __stdcall, returns int
    virtual void IsDirty(int a0);  // [4] 0x00405990  // __stdcall, returns ?
    virtual void Load(int a0, int a1);  // [5] 0x005fd1a0  // __stdcall, returns int
    virtual void Save(int a0, int a1, int a2);  // [6] 0x005fd1f0  // __stdcall, returns int
    virtual void GetSizeMax(int a0, int a1);  // [7] 0x00405910  // __stdcall, returns int
    virtual void Init();  // [9] 0x004059b0  // __cdecl, returns void
    virtual void Detach(int a0, int a1);  // [10] 0x005fbbb0  // __stdcall, returns int
    virtual void What_Am_I();  // [11] 0x005fd260  // __cdecl, returns int
    virtual void Fetch_Object_Size();  // [12] 0x005fd270  // __cdecl, returns int
    virtual void Compute_CRC(int a0);  // [13] 0x00405940  // __thiscall, returns ?
    virtual void Owner();  // [14] 0x004059d0  // __cdecl, returns int
    virtual void Owner_HouseClass();  // [15] 0x004059e0  // __cdecl, returns int
    virtual void Fetch_Heap_ID();  // [16] 0x004059f0  // __cdecl, returns int
    virtual void Is_Inactive();  // [17] 0x00405970  // __cdecl, returns char
    virtual void Center_Coord(int a0);  // [18] 0x00405a00  // __stdcall, returns ?
    virtual void Destination_Coord(int a0);  // [19] 0x00405a30  // __thiscall, returns ?
    virtual void On_Ground();  // [20] 0x00405a60  // __cdecl, returns int
    virtual void In_Air();  // [21] 0x00405a70  // __cdecl, returns int
    virtual void As_Coord(int a0);  // [22] 0x00405a80  // __thiscall, returns ?
    virtual void AI();  // [23] 0x005f55d0  // __thiscall, returns ?
    virtual void vfunc_24(int a0, int a1, int a2, int a3);  // [24] 0x005fcf50  // __thiscall, returns ?

    Tactical();  // 0x005f4b30
    // The destructor is declared non-virtual here: slot 0 holds a stub
    // shared with 66 other classes (0x00405790), not the destructor.
    ~Tactical();

    // ---- data members (offsets from ctor 0x005f4b30) ----
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
    unsigned char m_014;  // +0x014 /* ? */; 1-byte store: mov byte ptr [esi + 0x14], bl
    unsigned char pad_015[0x3f];
    unsigned int m_054;  // +0x054 init 0xffffffff; 4-byte store: mov dword ptr [esi + 0x54], 0xffffffff
    unsigned char m_058;  // +0x058 /* ? */; 1-byte store: mov byte ptr [esi + 0x58], bl
    unsigned char m_059;  // +0x059 /* ? */; 1-byte store: mov byte ptr [esi + 0x59], bl
    unsigned char pad_05a[0x2];
    unsigned int m_05c;  // +0x05c /* ? */; 4-byte store: mov dword ptr [esi + 0x5c], ebx
    unsigned int m_060;  // +0x060 /* ? */; 4-byte store: mov dword ptr [esi + 0x60], ebx
    unsigned int m_064;  // +0x064 /* ? */; 4-byte store: mov dword ptr [esi + 0x64], ebx
    unsigned int m_068;  // +0x068 /* ? */; 4-byte store: mov dword ptr [esi + 0x68], ebx
    unsigned char pad_06c[0x4];
    unsigned int m_070;  // +0x070 /* ? */; 4-byte store: mov dword ptr [esi + 0x70], ebx
    unsigned int m_074;  // +0x074 init 0x3ff00000; 4-byte store: mov dword ptr [esi + 0x74], 0x3ff00000
    unsigned int m_078;  // +0x078 /* ? */; 4-byte store: mov dword ptr [esi + 0x78], ebx
    unsigned int m_07c;  // +0x07c /* ? */; 4-byte store: mov dword ptr [esi + 0x7c], eax
    unsigned int m_080;  // +0x080 /* ? */; 4-byte store: mov dword ptr [esi + 0x80], ecx
    unsigned int m_084;  // +0x084 /* ? */; 4-byte store: mov dword ptr [esi + 0x84], edx
    unsigned int m_088;  // +0x088 /* ? */; 4-byte store: mov dword ptr [esi + 0x88], eax
    unsigned int m_08c;  // +0x08c /* ? */; 4-byte store: mov dword ptr [esi + 0x8c], ebx
    unsigned int m_090;  // +0x090 /* ? */; 4-byte store: mov dword ptr [esi + 0x90], ebx
    unsigned int m_094;  // +0x094 /* ? */; 4-byte store: mov dword ptr [esi + 0x94], ebx
    unsigned char pad_098[0xc80];
    unsigned int m_d18;  // +0xd18 /* ? */; 4-byte store: mov dword ptr [esi + 0xd18], ebx
    unsigned int m_d1c;  // +0xd1c /* ? */; 4-byte store: mov dword ptr [esi + 0xd1c], ebx
    unsigned int m_d20;  // +0xd20 /* ? */; 4-byte store: mov dword ptr [esi + 0xd20], ebx
    unsigned int m_d24;  // +0xd24 /* ? */; 4-byte store: mov dword ptr [esi + 0xd24], ebx
    unsigned int m_d28;  // +0xd28 /* ? */; 4-byte store: mov dword ptr [esi + 0xd28], ebx
    unsigned int m_d2c;  // +0xd2c /* ? */; 4-byte store: mov dword ptr [esi + 0xd2c], ebx
    unsigned char m_d30;  // +0xd30 init 0x1; 1-byte store: mov byte ptr [esi + 0xd30], 1
    unsigned char m_d31;  // +0xd31 /* ? */; 1-byte store: mov byte ptr [esi + 0xd31], bl
    unsigned char m_d32;  // +0xd32 /* ? */; 1-byte store: mov byte ptr [esi + 0xd32], bl
    unsigned char pad_d33[0x1];
    unsigned int m_d34;  // +0xd34 /* ? */; 4-byte store: mov dword ptr [esi + 0xd34], edx
    unsigned char pad_d38[0xc];
    unsigned int m_d44;  // +0xd44 /* ? */; 4-byte store: mov dword ptr [esi + 0xd44], ebx
    unsigned int m_d48;  // +0xd48 /* ? */; 4-byte store: mov dword ptr [esi + 0xd48], ebx
    unsigned int m_d4c;  // +0xd4c /* ? */; 4-byte store: mov dword ptr [esi + 0xd4c], ebx
    unsigned int m_d50;  // +0xd50 /* ? */; 4-byte store: mov dword ptr [esi + 0xd50], ebx
    unsigned int m_d54;  // +0xd54 /* ? */; 4-byte store: mov dword ptr [esi + 0xd54], ebx
    unsigned int m_d58;  // +0xd58 /* ? */; 4-byte store: mov dword ptr [esi + 0xd58], eax
    unsigned char pad_d5c[0x4];
    unsigned int m_d60;  // +0xd60 /* ? */; 4-byte store: mov dword ptr [esi + 0xd60], ebx
    unsigned char pad_d64[0x30];
    unsigned int m_d94;  // +0xd94 init 0x40aaaaa4; 4-byte store: mov dword ptr [esi + 0xd94], 0x40aaaaa4
    unsigned int m_d98;  // +0xd98 /* ? */; 4-byte store: mov dword ptr [esi + 0xd98], eax
    unsigned int m_d9c;  // +0xd9c /* ? */; 4-byte store: mov dword ptr [esi + 0xd9c], ebx
    unsigned int m_da0;  // +0xda0 /* ? */; 4-byte store: mov dword ptr [esi + 0xda0], ebx
    unsigned int m_da4;  // +0xda4 init 0xc0aaaaa4; 4-byte store: mov dword ptr [esi + 0xda4], 0xc0aaaaa4
    unsigned int m_da8;  // +0xda8 /* ? */; 4-byte store: mov dword ptr [esi + 0xda8], eax
    unsigned int m_dac;  // +0xdac /* ? */; 4-byte store: mov dword ptr [esi + 0xdac], ebx
    unsigned int m_db0;  // +0xdb0 /* ? */; 4-byte store: mov dword ptr [esi + 0xdb0], ebx
    unsigned int m_db4;  // +0xdb4 /* ? */; 4-byte store: mov dword ptr [esi + 0xdb4], ebx
    unsigned int m_db8;  // +0xdb8 /* ? */; 4-byte store: mov dword ptr [esi + 0xdb8], ebx
    unsigned int m_dbc;  // +0xdbc init 0x3f800000; 4-byte store: mov dword ptr [esi + 0xdbc], 0x3f800000
    unsigned int m_dc0;  // +0xdc0 /* ? */; 4-byte store: mov dword ptr [esi + 0xdc0], ebx
};
