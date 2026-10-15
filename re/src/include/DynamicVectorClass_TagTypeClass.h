#pragma once
// ==========================================================================
// DynamicVectorClass_TagTypeClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class DynamicVectorClass<TagTypeClass*>
// vtable       : 0x006a5460  (6 slots)
// constructor  : 0x00603e83
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "VectorClass_TagTypeClass.h"

class DynamicVectorClass_TagTypeClass : public VectorClass_TagTypeClass
{
public:
    // ---- vtable 0x006a5460 (6 slots) ----
    virtual void vfunc_1(int a0);  // [1] 0x006047e0  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x00604960  // __thiscall, returns int
    virtual void vfunc_3();  // [3] 0x00604790  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x00604a10  // __stdcall
    virtual void vfunc_5(int a0);  // [5] 0x006047c0  // __stdcall, returns int

    DynamicVectorClass_TagTypeClass();  // 0x00603e83
    virtual ~DynamicVectorClass_TagTypeClass();
};
