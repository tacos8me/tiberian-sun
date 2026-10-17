#pragma once
// ==========================================================================
// VoxelAnimTypeClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class VoxelAnimTypeClass
// vtable       : 0x006a6d48  (40 slots)
// constructor  : 0x00641ad0
// vtable store : 0x00641bc3  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "ObjectTypeClass.h"

class VoxelAnimTypeClass : public ObjectTypeClass
{
public:
    // ---- vtable 0x006a6d48 (40 slots) ----
    virtual void QueryInterface(int a0, int a1, int a2);  // [0] 0x00405790  // __stdcall, returns int
    virtual void Init(int a0);  // [1] 0x00405830  // __stdcall, returns int
    virtual void Shutdown(int a0);  // [2] 0x00405840  // __stdcall, returns int
    virtual void vfunc_3(int a0, int a1);  // [3] 0x00642550  // __stdcall, returns int
    virtual void Release(int a0);  // [4] 0x00405990  // __stdcall
    virtual void vfunc_5(int a0, int a1);  // [5] 0x00642590  // __stdcall, returns int
    virtual void vfunc_6(int a0, int a1, int a2);  // [6] 0x00642640  // __stdcall, returns int
    virtual void GetType(int a0, int a1);  // [7] 0x00572ba0  // __stdcall, returns int
    virtual void Set_ID();  // [9] 0x004059b0  // __cdecl
    virtual void Get_RTTI(int a0, int a1);  // [10] 0x00642660  // __stdcall
    virtual void What_Am_I();  // [11] 0x00642760  // __cdecl, returns int
    virtual void Get_Heap();  // [12] 0x00642770  // __cdecl, returns int
    virtual void Load(int a0);  // [13] 0x00642400  // __thiscall
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
    virtual void vfunc_24(int a0);  // [24] 0x00406010  // __stdcall
    virtual void Read_INI(int a0);  // [25] 0x00641d80  // __thiscall  // refs "Normalized"
    virtual void Set_ID(int a0);  // [26] 0x00405fb0  // __thiscall  // refs "Name"
    virtual void vfunc_27(int a0, int a1);  // [27] 0x0040f600  // __stdcall
    virtual void vfunc_28();  // [28] 0x00418f40  // __cdecl
    virtual void vfunc_29();  // [29] 0x005715e0  // __cdecl, returns int
    virtual void vfunc_30(int a0);  // [30] 0x005715f0  // __thiscall
    virtual void vfunc_31(int a0);  // [31] 0x00571610  // __thiscall
    virtual void vfunc_32(int a0, int a1);  // [32] 0x00642780  // __stdcall
    virtual void vfunc_33(int a0);  // [33] 0x00571640  // __stdcall, returns int
    virtual void vfunc_34();  // [34] 0x00571650  // __cdecl, returns int
    virtual void vfunc_35(int a0);  // [35] 0x00642790  // __stdcall, returns int
    virtual void vfunc_36(int a0);  // [36] 0x00571670  // __stdcall, returns int
    virtual void vfunc_37(int a0, int a1, int a2, int a3, int a4, int a5);  // [37] 0x00571710  // __thiscall
    virtual void vfunc_38();  // [38] 0x00571660  // __cdecl, returns int
    virtual void vfunc_39();  // [39] 0x0040f620  // __cdecl

    VoxelAnimTypeClass();  // 0x00641ad0
    // The destructor is declared non-virtual here: slot 0 holds a stub
    // shared with 66 other classes (0x00405790), not the destructor.
    ~VoxelAnimTypeClass();

    // ---- data members (offsets from ctor 0x00641ad0) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    // vtable-pointer stores are omitted: the compiler supplies the vptr.
    // The machine block begins at +0x124: the compiled end of the
    // base sub-object region. Evidence: ObjectTypeClass at +0x0 (ends +0x124).
    // 8 recovered offset(s) below +0x124 lie inside the base
    // sub-object region and are NOT declared here: they are declared and
    // compiler-checked in the base's own header -- +0x0, +0xc8, +0xc9, +0xca, +0xcb, +0xcc, +0xcd, +0xce
    unsigned char Normalized;  // +0x124 (INI key); 1-byte store: mov byte ptr [esi + 0x124], bl
    unsigned char m_125;  // +0x125 /* ? */; 1-byte store: mov byte ptr [esi + 0x125], bl
    unsigned char m_126;  // +0x126 /* ? */; 1-byte store: mov byte ptr [esi + 0x126], bl
    unsigned char pad_127[0x1];
    unsigned int m_128;  // +0x128 /* ? */; 4-byte store: mov dword ptr [esi + 0x128], ebx
    unsigned int m_12c;  // +0x12c init 0x1e; 4-byte store: mov dword ptr [esi + 0x12c], 0x1e
    unsigned int m_130;  // +0x130 init 0xa0000000; 4-byte store: mov dword ptr [esi + 0x130], 0xa0000000
    unsigned int m_134;  // +0x134 init 0x3fe99999; 4-byte store: mov dword ptr [esi + 0x134], 0x3fe99999
    unsigned int m_138;  // +0x138 /* ? */; 4-byte store: mov dword ptr [esi + 0x138], ebx
    unsigned int m_13c;  // +0x13c /* ? */; 4-byte store: mov dword ptr [esi + 0x13c], ebx
    unsigned int m_140;  // +0x140 init 0x1ce64946; 4-byte store: mov dword ptr [esi + 0x140], 0x1ce64946
    unsigned int m_144;  // +0x144 init 0x3fc656ed; 4-byte store: mov dword ptr [esi + 0x144], 0x3fc656ed
    unsigned int m_148;  // +0x148 /* ? */; 4-byte store: mov dword ptr [esi + 0x148], ebx
    unsigned int m_14c;  // +0x14c init 0x400c0000; 4-byte store: mov dword ptr [esi + 0x14c], 0x400c0000
    unsigned int m_150;  // +0x150 /* ? */; 4-byte store: mov dword ptr [esi + 0x150], ebx
    unsigned int m_154;  // +0x154 init 0x40140000; 4-byte store: mov dword ptr [esi + 0x154], 0x40140000
    unsigned int m_158;  // +0x158 /* ? */; 4-byte store: mov dword ptr [esi + 0x158], ebx
    unsigned int m_15c;  // +0x15c init 0x402e0000; 4-byte store: mov dword ptr [esi + 0x15c], 0x402e0000
    unsigned char m_160;  // +0x160 /* ? */; 1-byte store: mov byte ptr [esi + 0x160], bl
    unsigned char pad_161[0x3];
    unsigned int m_164;  // +0x164 /* ? */; 4-byte store: mov dword ptr [esi + 0x164], ebx
    unsigned int m_168;  // +0x168 /* ? */; 4-byte store: mov dword ptr [esi + 0x168], ebx
    unsigned int m_16c;  // +0x16c /* ? */; 4-byte store: mov dword ptr [esi + 0x16c], eax
    unsigned int m_170;  // +0x170 /* ? */; 4-byte store: mov dword ptr [esi + 0x170], eax
    unsigned int m_174;  // +0x174 /* ? */; 4-byte store: mov dword ptr [esi + 0x174], eax
    unsigned int m_178;  // +0x178 /* ? */; 4-byte store: mov dword ptr [esi + 0x178], ebx
    unsigned int m_17c;  // +0x17c /* ? */; 4-byte store: mov dword ptr [esi + 0x17c], ebx
    unsigned int m_180;  // +0x180 /* ? */; 4-byte store: mov dword ptr [esi + 0x180], ebx
    unsigned int m_184;  // +0x184 /* ? */; 4-byte store: mov dword ptr [esi + 0x184], ebx
    unsigned int m_188;  // +0x188 /* ? */; 4-byte store: mov dword ptr [esi + 0x188], ebx
    unsigned int m_18c;  // +0x18c /* ? */; 4-byte store: mov dword ptr [esi + 0x18c], ebx
    unsigned int m_190;  // +0x190 /* ? */; 4-byte store: mov dword ptr [esi + 0x190], ebx
    unsigned char IsTiberium;  // +0x194 (INI key); 1-byte store: mov byte ptr [esi + 0x194], bl
};
