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
    virtual void vfunc_7(int a0, int a1);  // [7] 0x00572ba0  // __stdcall, returns int
    virtual void Set_ID();  // [9] 0x004059b0  // __cdecl
    virtual void vfunc_10(int a0, int a1);  // [10] 0x00642660  // __stdcall
    virtual void vfunc_11();  // [11] 0x00642760  // __cdecl, returns int
    virtual void vfunc_12();  // [12] 0x00642770  // __cdecl, returns int
    virtual void vfunc_13(int a0);  // [13] 0x00642400  // __thiscall
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
    virtual void vfunc_25(int a0);  // [25] 0x00641d80  // __thiscall  // refs "Normalized"
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
    virtual ~VoxelAnimTypeClass();

    // ---- data members (offsets from ctor 0x00641cab) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x6a6d48
    unsigned long m_004;  // init 0x6a6d2c
    unsigned char pad_008[0x6c];
    unsigned long m_074;  // /* ? */
    unsigned long m_078;  // /* ? */
};
