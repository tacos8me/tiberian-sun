#pragma once
// ==========================================================================
// ParticleTypeClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class ParticleTypeClass
// vtable       : 0x006a3470  (40 slots)
// constructor  : 0x005979c0
// vtable store : 0x00597ad6  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "ObjectTypeClass.h"

class ParticleTypeClass : public ObjectTypeClass
{
public:
    // ---- vtable 0x006a3470 (40 slots) ----
    virtual void QueryInterface(int a0, int a1, int a2);  // [0] 0x00405790  // __stdcall, returns int
    virtual void AddRef(int a0);  // [1] 0x00405830  // __stdcall, returns int
    virtual void Release(int a0);  // [2] 0x00405840  // __stdcall, returns int
    virtual void vfunc_3(int a0, int a1);  // [3] 0x005983c0  // __stdcall, returns int
    virtual void IsDirty(int a0);  // [4] 0x00405990  // __stdcall
    virtual void vfunc_5(int a0, int a1);  // [5] 0x00598400  // __stdcall
    virtual void vfunc_6(int a0, int a1, int a2);  // [6] 0x00598530  // __stdcall
    virtual void GetSizeMax(int a0, int a1);  // [7] 0x00572ba0  // __stdcall, returns int
    virtual void Init();  // [9] 0x004059b0  // __cdecl
    virtual void Detach(int a0, int a1);  // [10] 0x00598640  // __stdcall
    virtual void What_Am_I();  // [11] 0x005986b0  // __cdecl, returns int
    virtual void Fetch_Object_Size();  // [12] 0x005986a0  // __cdecl, returns int
    virtual void Compute_CRC(int a0);  // [13] 0x00598280  // __thiscall
    virtual void Owner();  // [14] 0x004059d0  // __cdecl
    virtual void Owner_HouseClass();  // [15] 0x004059e0  // __cdecl, returns int
    virtual void Fetch_Heap_ID();  // [16] 0x004059f0  // __cdecl, returns int
    virtual void Is_Inactive();  // [17] 0x00405970  // __cdecl
    virtual void Center_Coord(int a0);  // [18] 0x00405a00  // __stdcall
    virtual void Destination_Coord(int a0);  // [19] 0x00405a30  // __stdcall
    virtual void On_Ground();  // [20] 0x00405a60  // __cdecl
    virtual void In_Air();  // [21] 0x00405a70  // __cdecl
    virtual void As_Coord(int a0);  // [22] 0x00405a80  // __stdcall
    virtual void AI();  // [23] 0x00405ab0  // __cdecl
    virtual void Init_Theater(int a0);  // [24] 0x00406010  // __stdcall
    virtual void Read_INI(int a0, int a1, int a2, int a3, int a4, int a5, int a6);  // [25] 0x00597d00  // __thiscall  // refs "ColorList"
    virtual void Write_INI(int a0);  // [26] 0x00405fb0  // __thiscall  // refs "Name"
    virtual void Coord_Fixup(int a0, int a1);  // [27] 0x0040f600  // __stdcall
    virtual void Get_Ownable();  // [28] 0x00418f40  // __cdecl
    virtual void Max_Pips();  // [29] 0x005715e0  // __cdecl, returns int
    virtual void Pixel_Dimensions(int a0);  // [30] 0x005715f0  // __thiscall
    virtual void Lepton_Dimensions(int a0);  // [31] 0x00571610  // __thiscall
    virtual void Create_And_Place(int a0, int a1);  // [32] 0x005986c0  // __stdcall
    virtual void Cost_Of(int a0);  // [33] 0x00571640  // __stdcall, returns int
    virtual void Time_To_Build();  // [34] 0x00571650  // __cdecl, returns int
    virtual void Create_One_Of(int a0);  // [35] 0x005986d0  // __stdcall, returns int
    virtual void Occupy_List(int a0);  // [36] 0x00571670  // __stdcall, returns int
    virtual void Who_Can_Build_Me(int a0, int a1, int a2, int a3, int a4, int a5);  // [37] 0x00571710  // __thiscall
    virtual void Get_Cameo_Data();  // [38] 0x00571660  // __cdecl, returns int
    virtual void Get_Image_Data();  // [39] 0x0040f620  // __cdecl

    ParticleTypeClass();  // 0x005979c0
    // The destructor is declared non-virtual here: slot 0 holds a stub
    // shared with 66 other classes (0x00405790), not the destructor.
    ~ParticleTypeClass();

    // ---- data members (offsets from ctor 0x005979c0) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    // vtable-pointer stores are omitted: the compiler supplies the vptr.
    // The machine block begins at +0x124: the compiled end of the
    // base sub-object region. Evidence: ObjectTypeClass at +0x0 (ends +0x124).
    // 3 recovered offset(s) below +0x124 lie inside the base
    // sub-object region and are NOT declared here: they are declared and
    // compiler-checked in the base's own header -- +0x0, +0x4, +0xcd
    unsigned int m_124;  // +0x124 /* ? */; 4-byte store: mov dword ptr [esi + 0x124], ebx
    unsigned int m_128;  // +0x128 /* ? */; 4-byte store: mov dword ptr [esi + 0x128], ebx
    unsigned int m_12c;  // +0x12c /* ? */; 4-byte store: mov dword ptr [esi + 0x12c], ebx
    unsigned int m_130;  // +0x130 /* ? */; 4-byte store: mov dword ptr [esi + 0x130], ebp
    unsigned int YVelocity;  // +0x134 (INI key); 4-byte store: mov dword ptr [esi + 0x134], ebp
    unsigned int MinZVelocity;  // +0x138 (INI key); 4-byte store: mov dword ptr [esi + 0x138], ebx
    unsigned int m_13c;  // +0x13c /* ? */; 4-byte store: mov dword ptr [esi + 0x13c], ebp
    unsigned int m_140;  // +0x140 /* ? */; 4-byte store: mov dword ptr [esi + 0x140], ebx
    unsigned int m_144;  // +0x144 /* ? */; 4-byte store: mov dword ptr [esi + 0x144], ebx
    unsigned char pad_148[0x1c];
    unsigned char m_164;  // +0x164 /* ? */; 1-byte store: mov byte ptr [esi + 0x164], bl
    unsigned char m_165;  // +0x165 /* ? */; 1-byte store: mov byte ptr [esi + 0x165], bl
    unsigned char m_166;  // +0x166 /* ? */; 1-byte store: mov byte ptr [esi + 0x166], bl
    unsigned char m_167;  // +0x167 /* ? */; 1-byte store: mov byte ptr [esi + 0x167], bl
    unsigned char m_168;  // +0x168 /* ? */; 1-byte store: mov byte ptr [esi + 0x168], bl
    unsigned char m_169;  // +0x169 /* ? */; 1-byte store: mov byte ptr [esi + 0x169], bl
    unsigned char pad_16a[0x2];
    unsigned int m_16c;  // +0x16c /* ? */; 4-byte store: mov dword ptr [esi + 0x16c], ebx
    unsigned int MaxEC;  // +0x170 (INI key); 4-byte store: mov dword ptr [esi + 0x170], ebp
    unsigned int m_174;  // +0x174 /* ? */; 4-byte store: mov dword ptr [esi + 0x174], ebx
    unsigned int m_178;  // +0x178 /* ? */; 4-byte store: mov dword ptr [esi + 0x178], ebx
    unsigned int StartFrame;  // +0x17c (INI key); 4-byte store: mov dword ptr [esi + 0x17c], ebx
    unsigned int NumLoopFrames;  // +0x180 (INI key); 4-byte store: mov dword ptr [esi + 0x180], ebp
    unsigned int m_184;  // +0x184 /* ? */; 4-byte store: mov dword ptr [esi + 0x184], ebx
    unsigned int WindEffect;  // +0x188 (INI key); 4-byte store: mov dword ptr [esi + 0x188], ebx
    unsigned int m_18c;  // +0x18c /* ? */; 4-byte store: mov dword ptr [esi + 0x18c], ebx
    unsigned int m_190;  // +0x190 /* ? */; 4-byte store: mov dword ptr [esi + 0x190], ebx
    unsigned int Radius;  // +0x194 (INI key); 4-byte store: mov dword ptr [esi + 0x194], ebx
    unsigned char m_198;  // +0x198 /* ? */; 1-byte store: mov byte ptr [esi + 0x198], bl
    unsigned char EndStateAI;  // +0x199 (INI key); 1-byte store: mov byte ptr [esi + 0x199], bl
    unsigned char m_19a;  // +0x19a /* ? */; 1-byte store: mov byte ptr [esi + 0x19a], bl
    unsigned char StateAIAdvance;  // +0x19b (INI key); 1-byte store: mov byte ptr [esi + 0x19b], 4
    unsigned char m_19c;  // +0x19c /* ? */; 1-byte store: mov byte ptr [esi + 0x19c], bl
    unsigned char Translucent25State;  // +0x19d (INI key); 1-byte store: mov byte ptr [esi + 0x19d], al
    unsigned char Translucent50State;  // +0x19e (INI key); 1-byte store: mov byte ptr [esi + 0x19e], al
    unsigned char Normalized;  // +0x19f (INI key); 1-byte store: mov byte ptr [esi + 0x19f], bl
    unsigned int m_1a0;  // +0x1a0 /* ? */; 4-byte store: mov dword ptr [esi + 0x1a0], eax
    unsigned int m_1a4;  // +0x1a4 /* ? */; 4-byte store: mov dword ptr [esi + 0x1a4], eax
};
