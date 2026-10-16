#pragma once
// ==========================================================================
// SmudgeTypeClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class SmudgeTypeClass
// vtable       : 0x006a4790  (41 slots)
// constructor  : 0x005e1580
// vtable store : 0x005e15bd  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "ObjectTypeClass.h"

class SmudgeTypeClass : public ObjectTypeClass
{
public:
    // ---- vtable 0x006a4790 (41 slots) ----
    virtual void QueryInterface(int a0, int a1, int a2);  // [0] 0x00405790  // __stdcall, returns int
    virtual void Init(int a0);  // [1] 0x00405830  // __stdcall, returns int
    virtual void Shutdown(int a0);  // [2] 0x00405840  // __stdcall, returns int
    virtual void vfunc_3(int a0, int a1);  // [3] 0x005e1b40  // __stdcall, returns int
    virtual void Release(int a0);  // [4] 0x00405990  // __stdcall
    virtual void vfunc_5(int a0, int a1);  // [5] 0x005e1ad0  // __stdcall, returns int
    virtual void vfunc_6(int a0, int a1, int a2);  // [6] 0x005e1b20  // __stdcall, returns int
    virtual void vfunc_7(int a0, int a1);  // [7] 0x00572ba0  // __stdcall, returns int
    virtual void Set_ID();  // [9] 0x004059b0  // __cdecl
    virtual void Get_RTTI(int a0, int a1);  // [10] 0x004059c0  // __stdcall
    virtual void vfunc_11();  // [11] 0x005e2380  // __cdecl, returns int
    virtual void vfunc_12();  // [12] 0x005e2390  // __cdecl, returns int
    virtual void vfunc_13(int a0);  // [13] 0x005e1a70  // __thiscall
    virtual void Save();  // [14] 0x004059d0  // __cdecl
    virtual void GetSizeMax();  // [15] 0x004059e0  // __cdecl, returns int
    virtual void vfunc_16();  // [16] 0x005e23a0  // __cdecl
    virtual void vfunc_17();  // [17] 0x00405970  // __cdecl
    virtual void vfunc_18(int a0);  // [18] 0x00405a00  // __stdcall
    virtual void vfunc_19(int a0);  // [19] 0x00405a30  // __stdcall
    virtual void vfunc_20();  // [20] 0x00405a60  // __cdecl
    virtual void vfunc_21();  // [21] 0x00405a70  // __cdecl
    virtual void vfunc_22(int a0);  // [22] 0x00405a80  // __stdcall
    virtual void vfunc_23();  // [23] 0x00405ab0  // __cdecl
    virtual void vfunc_24(int a0);  // [24] 0x00406010  // __stdcall
    virtual void Read_INI(int a0, int a1, int a2);  // [25] 0x005e1960  // __thiscall  // refs "Crater"
    virtual void Set_ID(int a0);  // [26] 0x00405fb0  // __thiscall  // refs "Name"
    virtual void vfunc_27(int a0, int a1);  // [27] 0x0040f600  // __stdcall
    virtual void vfunc_28();  // [28] 0x00418f40  // __cdecl
    virtual void vfunc_29();  // [29] 0x005715e0  // __cdecl, returns int
    virtual void vfunc_30(int a0);  // [30] 0x005715f0  // __thiscall
    virtual void vfunc_31(int a0);  // [31] 0x00571610  // __thiscall
    virtual void vfunc_32(int a0, int a1);  // [32] 0x005e17e0  // __thiscall
    virtual void vfunc_33(int a0);  // [33] 0x00571640  // __stdcall, returns int
    virtual void vfunc_34();  // [34] 0x00571650  // __cdecl, returns int
    virtual void vfunc_35(int a0);  // [35] 0x005e1850  // __thiscall
    virtual void vfunc_36(int a0);  // [36] 0x00571670  // __stdcall, returns int
    virtual void vfunc_37(int a0, int a1, int a2, int a3, int a4, int a5);  // [37] 0x00571710  // __thiscall
    virtual void vfunc_38();  // [38] 0x00571660  // __cdecl, returns int
    virtual void vfunc_39();  // [39] 0x0040f620  // __cdecl
    virtual void vfunc_40(int a0, int a1, int a2, int a3, int a4);  // [40] 0x005e1880  // __thiscall

    SmudgeTypeClass();  // 0x005e1580
    virtual ~SmudgeTypeClass();

    // ---- data members (offsets from ctor 0x005e1580) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    unsigned long m_000;  // +0x000 init 0x6a4790; 4-byte store: mov dword ptr [esi], 0x6a4790
    unsigned char pad_004[0xc4];
    unsigned char m_0c8;  // +0x0c8 /* ? */; 1-byte store: mov byte ptr [esi + 0xc8], bl
    unsigned char m_0c9;  // +0x0c9 zero-initialised; 1-byte store: mov byte ptr [esi + 0xc9], 0
    unsigned char m_0ca;  // +0x0ca zero-initialised; 1-byte store: mov byte ptr [esi + 0xca], 0
    unsigned char m_0cb;  // +0x0cb /* ? */; 1-byte store: mov byte ptr [esi + 0xcb], bl
    unsigned char m_0cc;  // +0x0cc /* ? */; 1-byte store: mov byte ptr [esi + 0xcc], bl
    unsigned char pad_0cd[0x1];
    unsigned char m_0ce;  // +0x0ce zero-initialised; 1-byte store: mov byte ptr [esi + 0xce], 0
    unsigned char pad_0cf[0x55];
    unsigned long m_124;  // +0x124 init 0xffffffff; 4-byte store: mov dword ptr [esi + 0x124], 0xffffffff
    unsigned long Width;  // +0x128 (INI key); 4-byte store: mov dword ptr [esi + 0x128], ebx
    unsigned long Height;  // +0x12c (INI key); 4-byte store: mov dword ptr [esi + 0x12c], ebx
    unsigned char Crater;  // +0x130 (INI key); 1-byte store: mov byte ptr [esi + 0x130], 0
    unsigned char m_131;  // +0x131 zero-initialised; 1-byte store: mov byte ptr [esi + 0x131], 0
};
