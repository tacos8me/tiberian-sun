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
    virtual void Init(int a0);  // [1] 0x00405830  // __stdcall, returns int
    virtual void Shutdown(int a0);  // [2] 0x00405840  // __stdcall, returns int
    virtual void vfunc_3(int a0, int a1);  // [3] 0x005fd160  // __stdcall, returns int
    virtual void Release(int a0);  // [4] 0x00405990  // __stdcall
    virtual void vfunc_5(int a0, int a1);  // [5] 0x005fd1a0  // __stdcall, returns int
    virtual void vfunc_6(int a0, int a1, int a2);  // [6] 0x005fd1f0  // __stdcall, returns int
    virtual void GetType(int a0, int a1);  // [7] 0x00405910  // __stdcall, returns int
    virtual void Set_ID();  // [9] 0x004059b0  // __cdecl
    virtual void vfunc_10(int a0, int a1);  // [10] 0x005fbbb0  // __stdcall
    virtual void vfunc_11();  // [11] 0x005fd260  // __cdecl, returns int
    virtual void vfunc_12();  // [12] 0x005fd270  // __cdecl, returns int
    virtual void Load(int a0);  // [13] 0x00405940  // __thiscall
    virtual void Save();  // [14] 0x004059d0  // __cdecl
    virtual void GetSizeMax();  // [15] 0x004059e0  // __cdecl, returns int
    virtual void Compute_CRC();  // [16] 0x004059f0  // __cdecl, returns int
    virtual void vfunc_17();  // [17] 0x00405970  // __cdecl
    virtual void vfunc_18(int a0);  // [18] 0x00405a00  // __stdcall
    virtual void vfunc_19(int a0);  // [19] 0x00405a30  // __stdcall
    virtual void vfunc_20();  // [20] 0x00405a60  // __cdecl
    virtual void vfunc_21();  // [21] 0x00405a70  // __cdecl
    virtual void vfunc_22(int a0);  // [22] 0x00405a80  // __stdcall
    virtual void vfunc_23(int a0, int a1, int a2, int a3, int a4);  // [23] 0x005f55d0  // __thiscall
    virtual void vfunc_24(int a0, int a1, int a2, int a3);  // [24] 0x005fcf50  // __thiscall

    Tactical();  // 0x005f4b30
    virtual ~Tactical();

    // ---- data members (offsets from ctor 0x005f4b30) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x6a5298
    unsigned long m_004;  // init 0x6a527c
    unsigned char pad_008[0xc];
    unsigned long m_014;  // /* ? */
    unsigned char pad_018[0x3c];
    unsigned long m_054;  // init 0xffffffff
    unsigned long m_058;  // /* ? */
    unsigned long m_05c;  // /* ? */
    unsigned long m_060;  // /* ? */
    unsigned long m_064;  // /* ? */
    unsigned long m_068;  // /* ? */
    unsigned char pad_06c[0x4];
    unsigned long m_070;  // /* ? */
    unsigned long m_074;  // init 0x3ff00000
    unsigned long m_078;  // /* ? */
    unsigned long m_07c;  // /* ? */
    unsigned long m_080;  // /* ? */
    unsigned long m_084;  // /* ? */
    unsigned long m_088;  // /* ? */
    unsigned long m_08c;  // /* ? */
    unsigned long m_090;  // /* ? */
    unsigned long m_094;  // /* ? */
    unsigned char pad_098[0xc80];
    unsigned long m_d18;  // /* ? */
    unsigned long m_d1c;  // /* ? */
    unsigned long m_d20;  // /* ? */
    unsigned long m_d24;  // /* ? */
    unsigned long m_d28;  // /* ? */
    unsigned long m_d2c;  // /* ? */
    unsigned long m_d30;  // init 0x1
    unsigned long m_d34;  // /* ? */
    unsigned char pad_d38[0xc];
    unsigned long m_d44;  // /* ? */
    unsigned long m_d48;  // /* ? */
    unsigned long m_d4c;  // /* ? */
    unsigned long m_d50;  // /* ? */
    unsigned long m_d54;  // /* ? */
    unsigned long m_d58;  // /* ? */
    unsigned char pad_d5c[0x4];
    unsigned long m_d60;  // /* ? */
    unsigned char pad_d64[0x30];
    unsigned long m_d94;  // init 0x40aaaaa4
    unsigned long m_d98;  // /* ? */
    unsigned long m_d9c;  // /* ? */
    unsigned long m_da0;  // /* ? */
    unsigned long m_da4;  // init 0xc0aaaaa4
    unsigned long m_da8;  // /* ? */
    unsigned long m_dac;  // /* ? */
    unsigned long m_db0;  // /* ? */
    unsigned long m_db4;  // /* ? */
    unsigned long m_db8;  // /* ? */
    unsigned long m_dbc;  // init 0x3f800000
    unsigned long m_dc0;  // /* ? */
};
