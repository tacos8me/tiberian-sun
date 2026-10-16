#pragma once
// ==========================================================================
// IsometricTileTypeClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class IsometricTileTypeClass
// vtable       : 0x006a12d8  (40 slots)
// constructor  : 0x004ea1cb
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "ObjectTypeClass.h"

class IsometricTileTypeClass : public ObjectTypeClass
{
public:
    // ---- vtable 0x006a12d8 (40 slots) ----
    virtual void QueryInterface(int a0, int a1, int a2);  // [0] 0x00405790  // __stdcall, returns int
    virtual void Init(int a0);  // [1] 0x00405830  // __stdcall, returns int
    virtual void Shutdown(int a0);  // [2] 0x00405840  // __stdcall, returns int
    virtual void vfunc_3(int a0, int a1);  // [3] 0x004ef540  // __stdcall, returns int
    virtual void Release(int a0);  // [4] 0x00405990  // __stdcall
    virtual void vfunc_5(int a0, int a1);  // [5] 0x004ef440  // __stdcall, returns int
    virtual void vfunc_6(int a0, int a1, int a2);  // [6] 0x004ef520  // __stdcall, returns int
    virtual void vfunc_7(int a0, int a1);  // [7] 0x00572ba0  // __stdcall, returns int
    virtual void vfunc_8(int a0);  // [8] 0x004ef950  // __thiscall
    virtual void Set_ID();  // [9] 0x004059b0  // __cdecl
    virtual void vfunc_10(int a0, int a1);  // [10] 0x004ef580  // __stdcall
    virtual void vfunc_11();  // [11] 0x004ef920  // __cdecl, returns int
    virtual void vfunc_12();  // [12] 0x004ef930  // __cdecl, returns int
    virtual void vfunc_13(int a0);  // [13] 0x004ef330  // __thiscall
    virtual void Save();  // [14] 0x004059d0  // __cdecl
    virtual void GetSizeMax();  // [15] 0x004059e0  // __cdecl, returns int
    virtual void vfunc_16();  // [16] 0x004ef940  // __cdecl
    virtual void vfunc_17();  // [17] 0x00405970  // __cdecl
    virtual void vfunc_18(int a0);  // [18] 0x00405a00  // __stdcall
    virtual void vfunc_19(int a0);  // [19] 0x00405a30  // __stdcall
    virtual void vfunc_20();  // [20] 0x00405a60  // __cdecl
    virtual void vfunc_21();  // [21] 0x00405a70  // __cdecl
    virtual void vfunc_22(int a0);  // [22] 0x00405a80  // __stdcall
    virtual void vfunc_23();  // [23] 0x00405ab0  // __cdecl
    virtual void vfunc_24(int a0);  // [24] 0x00406010  // __stdcall
    virtual void vfunc_25(int a0);  // [25] 0x00572610  // __thiscall  // refs "Image"
    virtual void Set_ID(int a0);  // [26] 0x00405fb0  // __thiscall  // refs "Name"
    virtual void vfunc_27(int a0, int a1);  // [27] 0x004ef310  // __stdcall
    virtual void vfunc_28();  // [28] 0x00418f40  // __cdecl
    virtual void vfunc_29();  // [29] 0x005715e0  // __cdecl, returns int
    virtual void vfunc_30(int a0);  // [30] 0x005715f0  // __thiscall
    virtual void vfunc_31(int a0);  // [31] 0x00571610  // __thiscall
    virtual void vfunc_32(int a0, int a1);  // [32] 0x004ef260  // __thiscall
    virtual void vfunc_33(int a0);  // [33] 0x00571640  // __stdcall, returns int
    virtual void vfunc_34();  // [34] 0x00571650  // __cdecl, returns int
    virtual void vfunc_35(int a0);  // [35] 0x004ef2a0  // __thiscall
    virtual void vfunc_36(int a0);  // [36] 0x004ea600  // __thiscall, returns int
    virtual void vfunc_37(int a0, int a1, int a2, int a3, int a4, int a5);  // [37] 0x00571710  // __thiscall
    virtual void vfunc_38();  // [38] 0x00571660  // __cdecl, returns int
    virtual void vfunc_39();  // [39] 0x004ea590  // __thiscall

    IsometricTileTypeClass();  // 0x004ea1cb
    virtual ~IsometricTileTypeClass();

    // ---- data members (offsets from ctor 0x004ea0b9) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x6a12d8
    unsigned long m_004;  // init 0x6a12bc
    unsigned char pad_008[0xc0];
    unsigned long m_0c8;  // init 0x1
    unsigned long Immune;  // +0x0cc (INI key)
    unsigned char pad_0d0[0x54];
    unsigned long m_124;  // /* ? */
    unsigned long m_128;  // /* ? */
    unsigned long m_12c;  // /* ? */
    unsigned long m_130;  // /* ? */
    unsigned long m_134;  // init 0x6a137c
    unsigned long m_138;  // /* ? */
    unsigned long m_13c;  // /* ? */
    unsigned long m_140;  // init 0x1
    unsigned long m_144;  // /* ? */
    unsigned long m_148;  // init 0xa
    unsigned long m_14c;  // /* ? */
    unsigned long m_150;  // /* ? */
    unsigned long m_154;  // /* ? */
    unsigned long m_158;  // /* ? */
    unsigned long m_15c;  // /* ? */
    unsigned long m_160;  // /* ? */
    unsigned long m_164;  // /* ? */
    unsigned long m_168;  // /* ? */
    unsigned long m_16c;  // /* ? */
    unsigned long m_170;  // /* ? */
    unsigned long m_174;  // /* ? */
    unsigned long m_178;  // /* ? */
    unsigned long m_17c;  // /* ? */
    unsigned long m_180;  // init 0x1
    unsigned long m_184;  // /* ? */
    unsigned char pad_188[0xd];
    unsigned long m_195;  // init 0x1
};
