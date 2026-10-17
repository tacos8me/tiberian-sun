#pragma once
// ==========================================================================
// ShapeButtonClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class ShapeButtonClass
// vtable       : 0x0069d8a8  (33 slots)
// constructor  : 0x0047f180
// vtable store : 0x0047f233  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "ToggleClass.h"

class ShapeButtonClass : public ToggleClass
{
public:
    // ---- vtable 0x0069d8a8 (33 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x0047f560  // __thiscall
    virtual void vfunc_1();  // [1] 0x004a5b00
    virtual void vfunc_2();  // [2] 0x004a5b10
    virtual void vfunc_3(int a0);  // [3] 0x004f69e0  // __stdcall
    virtual void vfunc_4(int a0);  // [4] 0x004f6a20  // __thiscall
    virtual void vfunc_5(int a0);  // [5] 0x004f6a00  // __thiscall
    virtual void vfunc_6();  // [6] 0x004f6980  // __cdecl
    virtual void vfunc_7();  // [7] 0x004f69b0  // __cdecl
    virtual void vfunc_8();  // [8] 0x004f6930  // __cdecl, returns int
    virtual void vfunc_9();  // [9] 0x004a5ae0  // __thiscall
    virtual void vfunc_10(int a0, int a1);  // [10] 0x004a5be0  // __thiscall
    virtual void vfunc_11(int a0);  // [11] 0x004a5bb0  // __thiscall
    virtual void vfunc_12();  // [12] 0x004a5b20  // __thiscall
    virtual void vfunc_13(int a0);  // [13] 0x004a5de0  // __thiscall, returns int
    virtual void vfunc_14();  // [14] 0x0045afe0  // __cdecl
    virtual void vfunc_15();  // [15] 0x004a5ad0  // __thiscall
    virtual void vfunc_16();  // [16] 0x004a5ac0
    virtual void vfunc_17();  // [17] 0x00460610  // __cdecl
    virtual void vfunc_18();  // [18] 0x004a5e20  // __cdecl
    virtual void vfunc_19(int a0, int a1, int a2);  // [19] 0x00460650  // __stdcall
    virtual void vfunc_20();  // [20] 0x004a5e60  // __thiscall
    virtual void vfunc_21();  // [21] 0x004a5e90  // __cdecl
    virtual void vfunc_22();  // [22] 0x004a5eb0  // __cdecl, returns int
    virtual void vfunc_23();  // [23] 0x004a5ec0  // __cdecl, returns int
    virtual void vfunc_24();  // [24] 0x0045aff0  // __cdecl
    virtual void vfunc_25(int a0, int a1);  // [25] 0x004a5ee0  // __stdcall
    virtual void vfunc_26(int a0, int a1);  // [26] 0x004a5f00  // __stdcall
    virtual void vfunc_27();  // [27] 0x005d7ad0  // __thiscall
    virtual void vfunc_28(int a0);  // [28] 0x004a5e30  // __stdcall
    virtual void vfunc_29(int a0);  // [29] 0x0062a8c0  // __thiscall
    virtual void vfunc_30(int a0, int a1, int a2, int a3);  // [30] 0x004a5a60  // __stdcall, returns int
    virtual void vfunc_31(int a0);  // [31] 0x00460600  // __stdcall
    virtual void vfunc_32(int a0, int a1, int a2);  // [32] 0x005d7a90  // __stdcall

    ShapeButtonClass();  // 0x0047f180
    virtual ~ShapeButtonClass();

    // ---- data members (offsets from ctor 0x0047f180) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    // vtable-pointer stores are omitted: the compiler supplies the vptr.
    unsigned char pad_004[0x28];
    unsigned char m_02c;  // +0x02c /* ? */; 1-byte store: mov byte ptr [edi + 0x2c], al
    unsigned char m_02d;  // +0x02d /* ? */; 1-byte store: mov byte ptr [edi + 0x2d], cl
    unsigned char m_02e;  // +0x02e /* ? */; 1-byte store: mov byte ptr [edi + 0x2e], dl
    unsigned char pad_02f[0x1];
    unsigned char m_030;  // +0x030 /* ? */; 1-byte store: mov byte ptr [edi + 0x30], al
    unsigned char pad_031[0x3];
    unsigned int m_034;  // +0x034 /* ? */; 4-byte store: mov dword ptr [edi + 0x34], ecx
    unsigned int m_038;  // +0x038 /* ? */; 4-byte store: mov dword ptr [edi + 0x38], edx
    unsigned char m_03c;  // +0x03c /* ? */; 1-byte store: mov byte ptr [edi + 0x3c], al
    unsigned char pad_03d[0x3];
    unsigned int m_040;  // +0x040 /* ? */; 4-byte store: mov dword ptr [edi + 0x40], ecx
    unsigned char m_044;  // +0x044 /* ? */; 1-byte store: mov byte ptr [edi + 0x44], dl
    unsigned char m_045;  // +0x045 /* ? */; 1-byte store: mov byte ptr [edi + 0x45], al
    unsigned char pad_046[0x2];
    unsigned int m_048;  // +0x048 /* ? */; 4-byte store: mov dword ptr [edi + 0x48], ecx
};
