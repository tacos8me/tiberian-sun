#pragma once
// ==========================================================================
// ParticleSystemTypeClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class ParticleSystemTypeClass
// vtable       : 0x006a33a8  (40 slots)
// constructor  : 0x00596e90
// vtable store : 0x00596f8e  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "ObjectTypeClass.h"

class ParticleSystemTypeClass : public ObjectTypeClass
{
public:
    // ---- vtable 0x006a33a8 (40 slots) ----
    virtual void QueryInterface(int a0, int a1, int a2);  // [0] 0x00405790  // __stdcall, returns int
    virtual void Init(int a0);  // [1] 0x00405830  // __stdcall, returns int
    virtual void Shutdown(int a0);  // [2] 0x00405840  // __stdcall, returns int
    virtual void vfunc_3(int a0, int a1);  // [3] 0x00597560  // __stdcall, returns int
    virtual void Release(int a0);  // [4] 0x00405990  // __stdcall
    virtual void vfunc_5(int a0, int a1);  // [5] 0x005975a0  // __stdcall, returns int
    virtual void vfunc_6(int a0, int a1, int a2);  // [6] 0x005975e0  // __stdcall, returns int
    virtual void GetType(int a0, int a1);  // [7] 0x00572ba0  // __stdcall, returns int
    virtual void Set_ID();  // [9] 0x004059b0  // __cdecl
    virtual void Get_RTTI(int a0, int a1);  // [10] 0x004059c0  // __stdcall
    virtual void What_Am_I();  // [11] 0x005976e0  // __cdecl, returns int
    virtual void Get_Heap();  // [12] 0x005976d0  // __cdecl, returns int
    virtual void Load(int a0);  // [13] 0x005974c0  // __thiscall
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
    virtual void Read_INI(int a0);  // [25] 0x00597090  // __thiscall  // refs "HoldsWhat"
    virtual void Set_ID(int a0);  // [26] 0x00405fb0  // __thiscall  // refs "Name"
    virtual void vfunc_27(int a0, int a1);  // [27] 0x0040f600  // __stdcall
    virtual void vfunc_28();  // [28] 0x00418f40  // __cdecl
    virtual void vfunc_29();  // [29] 0x005715e0  // __cdecl, returns int
    virtual void vfunc_30(int a0);  // [30] 0x005715f0  // __thiscall
    virtual void vfunc_31(int a0);  // [31] 0x00571610  // __thiscall
    virtual void vfunc_32(int a0, int a1);  // [32] 0x005976f0  // __stdcall
    virtual void vfunc_33(int a0);  // [33] 0x00571640  // __stdcall, returns int
    virtual void vfunc_34();  // [34] 0x00571650  // __cdecl, returns int
    virtual void vfunc_35(int a0);  // [35] 0x00597700  // __stdcall, returns int
    virtual void vfunc_36(int a0);  // [36] 0x00571670  // __stdcall, returns int
    virtual void vfunc_37(int a0, int a1, int a2, int a3, int a4, int a5);  // [37] 0x00571710  // __thiscall
    virtual void vfunc_38();  // [38] 0x00571660  // __cdecl, returns int
    virtual void vfunc_39();  // [39] 0x0040f620  // __cdecl

    ParticleSystemTypeClass();  // 0x00596e90
    // The destructor is declared non-virtual here: slot 0 holds a stub
    // shared with 66 other classes (0x00405790), not the destructor.
    ~ParticleSystemTypeClass();

    // ---- data members (offsets from ctor 0x00596e90) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    // vtable-pointer stores are omitted: the compiler supplies the vptr.
    unsigned char pad_004[0xc9];
    unsigned char m_0cd;  // +0x0cd init 0x1; 1-byte store: mov byte ptr [esi + 0xcd], 1
    unsigned char pad_0ce[0x56];
    unsigned int HoldsWhat;  // +0x124 (INI key); 4-byte store: mov dword ptr [esi + 0x124], eax
    unsigned char m_128;  // +0x128 /* ? */; 1-byte store: mov byte ptr [esi + 0x128], bl
    unsigned char pad_129[0x3];
    unsigned int SpawnFrames;  // +0x12c (INI key); 4-byte store: mov dword ptr [esi + 0x12c], 1
    unsigned int m_130;  // +0x130 /* ? */; 4-byte store: mov dword ptr [esi + 0x130], ebx
    unsigned int ParticleCap;  // +0x134 (INI key); 4-byte store: mov dword ptr [esi + 0x134], 0x32
    unsigned int SpawnRadius;  // +0x138 (INI key); 4-byte store: mov dword ptr [esi + 0x138], ebx
    unsigned int m_13c;  // +0x13c /* ? */; 4-byte store: mov dword ptr [esi + 0x13c], ebx
    unsigned int m_140;  // +0x140 /* ? */; 4-byte store: mov dword ptr [esi + 0x140], ebx
    unsigned int m_144;  // +0x144 /* ? */; 4-byte store: mov dword ptr [esi + 0x144], eax
    unsigned int Lifetime;  // +0x148 (INI key); 4-byte store: mov dword ptr [esi + 0x148], eax
    unsigned int m_14c;  // +0x14c /* ? */; 4-byte store: mov dword ptr [esi + 0x14c], ebx
    unsigned int m_150;  // +0x150 /* ? */; 4-byte store: mov dword ptr [esi + 0x150], ebx
    unsigned int m_154;  // +0x154 /* ? */; 4-byte store: mov dword ptr [esi + 0x154], ebx
    unsigned int m_158;  // +0x158 /* ? */; 4-byte store: mov dword ptr [esi + 0x158], eax
    unsigned int m_15c;  // +0x15c init 0x3fb99999; 4-byte store: mov dword ptr [esi + 0x15c], 0x3fb99999
    unsigned int m_160;  // +0x160 /* ? */; 4-byte store: mov dword ptr [esi + 0x160], eax
    unsigned int m_164;  // +0x164 init 0x3f999999; 4-byte store: mov dword ptr [esi + 0x164], 0x3f999999
    unsigned int m_168;  // +0x168 /* ? */; 4-byte store: mov dword ptr [esi + 0x168], ebx
    unsigned int m_16c;  // +0x16c init 0x40390000; 4-byte store: mov dword ptr [esi + 0x16c], 0x40390000
    unsigned int m_170;  // +0x170 /* ? */; 4-byte store: mov dword ptr [esi + 0x170], ebx
    unsigned int m_174;  // +0x174 /* ? */; 4-byte store: mov dword ptr [esi + 0x174], ebx
    unsigned int m_178;  // +0x178 /* ? */; 4-byte store: mov dword ptr [esi + 0x178], ebx
    unsigned int m_17c;  // +0x17c /* ? */; 4-byte store: mov dword ptr [esi + 0x17c], ebx
    unsigned int m_180;  // +0x180 /* ? */; 4-byte store: mov dword ptr [esi + 0x180], ebx
    unsigned int m_184;  // +0x184 /* ? */; 4-byte store: mov dword ptr [esi + 0x184], ebx
    unsigned int m_188;  // +0x188 /* ? */; 4-byte store: mov dword ptr [esi + 0x188], ebx
    unsigned int m_18c;  // +0x18c /* ? */; 4-byte store: mov dword ptr [esi + 0x18c], ebx
    unsigned int m_190;  // +0x190 /* ? */; 4-byte store: mov dword ptr [esi + 0x190], ebx
    unsigned int LightSize;  // +0x194 (INI key); 4-byte store: mov dword ptr [esi + 0x194], ebx
    unsigned short m_198;  // +0x198 /* ? */; 2-byte store: mov word ptr [esi + 0x198], dx
    unsigned char m_19a;  // +0x19a /* ? */; 1-byte store: mov byte ptr [esi + 0x19a], bl
    unsigned char Laser;  // +0x19b (INI key); 1-byte store: mov byte ptr [esi + 0x19b], bl
    unsigned char m_19c;  // +0x19c /* ? */; 1-byte store: mov byte ptr [esi + 0x19c], bl
};
