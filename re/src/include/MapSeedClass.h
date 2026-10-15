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
    virtual void vfunc_0();  // [0] 0x00540230
    virtual void vfunc_1();  // [1] 0x0052d760  // refs "Description"
    virtual void vfunc_2();  // [2] 0x0052d4f0  // refs "Description"
    virtual void vfunc_3();  // [3] 0x0052da20
    virtual void vfunc_4();  // [4] 0x0052da30

    MapSeedClass();  // 0x0052c54a
    virtual ~MapSeedClass();
};
