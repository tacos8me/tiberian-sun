#pragma once
// ==========================================================================
// TiberiumClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class TiberiumClass
// vtable       : 0x006a6248  (27 slots)
// constructor  : 0x006281e0
// vtable store : 0x006282b3  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "ObjectTypeClass.h"

class TiberiumClass : public AbstractTypeClass
{
public:
    // ---- vtable 0x006a6248 (27 slots) ----
    virtual void QueryInterface(int a0, int a1, int a2);  // [0] 0x00405790  // __stdcall, returns int
    virtual void Init(int a0);  // [1] 0x00405830  // __stdcall, returns int
    virtual void Shutdown(int a0);  // [2] 0x00405840  // __stdcall, returns int
    virtual void vfunc_3(int a0, int a1);  // [3] 0x00628900  // __stdcall, returns int
    virtual void Release(int a0);  // [4] 0x00405990  // __stdcall
    virtual void vfunc_6(int a0, int a1, int a2);  // [6] 0x00628b80  // __stdcall
    virtual void vfunc_7(int a0, int a1);  // [7] 0x00628b50  // __stdcall, returns int
    virtual void vfunc_8(int a0);  // [8] 0x0062a100  // __thiscall
    virtual void Set_ID();  // [9] 0x004059b0  // __cdecl
    virtual void vfunc_10(int a0, int a1);  // [10] 0x00628bf0  // __stdcall
    virtual void vfunc_11();  // [11] 0x0062a0e0  // __cdecl, returns int
    virtual void vfunc_12();  // [12] 0x0062a0d0  // __cdecl, returns int
    virtual void vfunc_13(int a0);  // [13] 0x00628890  // __thiscall
    virtual void Save();  // [14] 0x004059d0  // __cdecl
    virtual void GetSizeMax();  // [15] 0x004059e0  // __cdecl, returns int
    virtual void vfunc_16();  // [16] 0x0062a0f0  // __cdecl
    virtual void vfunc_17();  // [17] 0x00405970  // __cdecl
    virtual void vfunc_18(int a0);  // [18] 0x00405a00  // __stdcall
    virtual void vfunc_19(int a0);  // [19] 0x00405a30  // __stdcall
    virtual void vfunc_20();  // [20] 0x00405a60  // __cdecl
    virtual void vfunc_21();  // [21] 0x00405a70  // __cdecl
    virtual void vfunc_22(int a0);  // [22] 0x00405a80  // __stdcall
    virtual void vfunc_23();  // [23] 0x00405ab0  // __cdecl
    virtual void vfunc_24(int a0);  // [24] 0x00406010  // __stdcall
    virtual void vfunc_25(int a0, int a1, int a2, int a3, int a4, int a5, int a6);  // [25] 0x00628540  // __thiscall  // refs "Spread"
    virtual void Set_ID(int a0);  // [26] 0x00405fb0  // __thiscall  // refs "Name"

    TiberiumClass();  // 0x006281e0
    virtual ~TiberiumClass();

    // ---- data members (offsets from ctor 0x006281e0) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x6a6248
    unsigned long m_004;  // init 0x6a622c
    unsigned char pad_008[0x58];
    unsigned long m_060;  // init 0xffffffff
    unsigned long m_064;  // /* ? */
    unsigned long m_068;  // /* ? */
    unsigned long m_06c;  // /* ? */
    unsigned long m_070;  // /* ? */
    unsigned char pad_074[0x4];
    unsigned long m_078;  // /* ? */
    unsigned long m_07c;  // /* ? */
    unsigned long Value;  // +0x080 (INI key)
    unsigned long m_084;  // /* ? */
    unsigned long m_088;  // /* ? */
    unsigned char pad_08c[0x1c];
    unsigned long m_0a8;  // /* ? */
    unsigned long m_0ac;  // /* ? */
    unsigned long m_0b0;  // /* ? */
    unsigned long m_0b4;  // /* ? */
    unsigned long m_0b8;  // /* ? */
    unsigned long m_0bc;  // /* ? */
    unsigned long m_0c0;  // /* ? */
    unsigned long m_0c4;  // /* ? */
    unsigned long m_0c8;  // /* ? */
    unsigned char pad_0cc[0x4];
    unsigned long m_0d0;  // /* ? */
    unsigned long m_0d4;  // /* ? */
    unsigned long m_0d8;  // /* ? */
    unsigned long m_0dc;  // /* ? */
    unsigned long m_0e0;  // /* ? */
    unsigned long m_0e4;  // /* ? */
    unsigned char pad_0e8[0x4];
    unsigned long m_0ec;  // /* ? */
};
