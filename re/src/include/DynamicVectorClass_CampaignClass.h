#pragma once
// ==========================================================================
// DynamicVectorClass_CampaignClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class DynamicVectorClass<CampaignClass*>
// vtable       : 0x0069f334  (6 slots)
// constructor  : 0x004a8393
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "VectorClass_CampaignClass.h"

class DynamicVectorClass_CampaignClass : public VectorClass_CampaignClass
{
public:
    // ---- vtable 0x0069f334 (6 slots) ----
    virtual void vfunc_1(int a0);  // [1] 0x004ad530  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x004ad6b0  // __thiscall, returns int
    virtual void vfunc_3();  // [3] 0x004a9b40  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x004ad760  // __stdcall
    virtual void vfunc_5(int a0);  // [5] 0x004a9b70  // __stdcall, returns int

    DynamicVectorClass_CampaignClass();  // 0x004a8393
    virtual ~DynamicVectorClass_CampaignClass();
};
