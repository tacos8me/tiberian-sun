#pragma once
// ==========================================================================
// FactoryClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class FactoryClass
// vtable       : 0x0069dff8  (24 slots)
// constructor  : 0x00492ee6
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "TechnoClass.h"
#include "AbstractClass.h"

class FactoryClass : public AbstractClass, public StageClass
{
public:
    // ---- vtable 0x0069dff8 (24 slots) ----
    virtual void QueryInterface(int a0, int a1, int a2);  // [0] 0x00405790  // __stdcall, returns int
    virtual void Init(int a0);  // [1] 0x00405830  // __stdcall, returns int
    virtual void Shutdown(int a0);  // [2] 0x00405840  // __stdcall, returns int
    virtual void vfunc_3(int a0, int a1);  // [3] 0x00493790  // __stdcall, returns int
    virtual void Release(int a0);  // [4] 0x00405990  // __stdcall
    virtual void vfunc_5(int a0, int a1);  // [5] 0x004937d0  // __stdcall
    virtual void vfunc_6(int a0, int a1, int a2);  // [6] 0x00493910  // __stdcall
    virtual void GetType(int a0, int a1);  // [7] 0x00405910  // __stdcall, returns int
    virtual void Set_ID();  // [9] 0x004059b0  // __cdecl
    virtual void vfunc_10(int a0, int a1);  // [10] 0x00493ad0  // __stdcall
    virtual void vfunc_11();  // [11] 0x00493c90  // __cdecl, returns int
    virtual void vfunc_12();  // [12] 0x00493ca0  // __cdecl, returns int
    virtual void vfunc_13(int a0);  // [13] 0x00493980  // __thiscall
    virtual void Save();  // [14] 0x004059d0  // __cdecl
    virtual void GetSizeMax();  // [15] 0x004059e0  // __cdecl, returns int
    virtual void Compute_CRC();  // [16] 0x004059f0  // __cdecl, returns int
    virtual void vfunc_17();  // [17] 0x00405970  // __cdecl
    virtual void vfunc_18(int a0);  // [18] 0x00405a00  // __stdcall
    virtual void vfunc_19(int a0);  // [19] 0x00405a30  // __stdcall
    virtual void vfunc_20();  // [20] 0x00405a60  // __cdecl
    virtual void vfunc_21();  // [21] 0x00405a70  // __cdecl
    virtual void vfunc_22(int a0);  // [22] 0x00405a80  // __stdcall
    virtual void vfunc_23(int a0);  // [23] 0x004930c0  // __thiscall

    FactoryClass();  // 0x00492ee6
    virtual ~FactoryClass();

    // ---- data members (offsets from ctor 0x00492e77) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69dff8
    unsigned char pad_004[0x10];
    unsigned long m_014;  // /* ? */
    unsigned long m_018;  // /* ? */
    unsigned char pad_01c[0x4];
    unsigned long m_020;  // /* ? */
    unsigned long m_024;  // /* ? */
    unsigned long m_028;  // /* ? */
    unsigned long m_02c;  // init 0x69e05c
    unsigned long m_030;  // /* ? */
    unsigned long m_034;  // /* ? */
    unsigned long m_038;  // /* ? */
    unsigned long m_03c;  // /* ? */
    unsigned long m_040;  // init 0xa
    unsigned long m_044;  // /* ? */
    unsigned long m_048;  // /* ? */
    unsigned long m_04c;  // /* ? */
    unsigned long m_050;  // /* ? */
    unsigned long m_054;  // init 0xffffffff
    unsigned long m_058;  // /* ? */
    unsigned long m_05c;  // /* ? */
};
