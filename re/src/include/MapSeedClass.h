#pragma once
// ==========================================================================
// MapSeedClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class MapSeedClass
// vtable       : 0x006a1e24  (5 slots)
// constructor  : 0x0052c54a
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "LoadOptionsClass.h"

class MapSeedClass : public LoadOptionsClass
{
public:
    // ---- vtable 0x006a1e24 (5 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x00540230  // __thiscall
    virtual void vfunc_1(int a0);  // [1] 0x0052d760  // __thiscall  // refs "Description"
    virtual void vfunc_2(int a0, int a1);  // [2] 0x0052d4f0  // __thiscall  // refs "Description"
    virtual void vfunc_3(int a0);  // [3] 0x0052da20  // __stdcall
    virtual void vfunc_4(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11, int a12, int a13);  // [4] 0x0052da30

    MapSeedClass();  // 0x0052c54a
    virtual ~MapSeedClass();

    // ---- data members (offsets from ctor 0x0052c4f4) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x6a1e24
    unsigned char pad_004[0x4];
    unsigned long m_008;  // init 0x6cd9cc
    unsigned long m_00c;  // /* ? */
    unsigned char pad_010[0x24];
    unsigned long m_034;  // /* ? */
    unsigned long m_038;  // /* ? */
    unsigned long RandomMap;  // +0x03c (INI key)
    unsigned long m_040;  // /* ? */
    unsigned long m_044;  // init 0x2
    unsigned long m_048;  // /* ? */
    unsigned long m_04c;  // /* ? */
    unsigned long m_050;  // /* ? */
    unsigned long m_054;  // /* ? */
    unsigned long m_058;  // /* ? */
    unsigned long m_05c;  // /* ? */
    unsigned long m_060;  // /* ? */
    unsigned long m_064;  // /* ? */
    unsigned long m_068;  // init 0xffffffff
};
