#pragma once
// ============================================================================
// StageClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// A mix-in base embedded at a fixed displacement inside every class that has a
// staged animation state machine. It has no vtable of its own in the RTTI
// sense (no CompleteObjectLocator, so it is not emitted as a class by the
// generator), but its virtuals are real and its sub-object sits at a fixed
// offset in each derived class.
//
// Evidence for the base relationship — RTTI base entry "StageClass" with a
// non-zero mdisp in each of these classes (out/classes.json):
//
//   AnimClass      mdisp 76   (+0x4c)
//   TerrainClass   mdisp 76   (+0x4c)
//   FactoryClass   mdisp 20   (+0x14)
//   TechnoClass    mdisp 132  (+0x84)
//   FootClass      mdisp 132
//   AircraftClass  mdisp 132
//   BuildingClass  mdisp 132
//   InfantryClass  mdisp 132
//
// Evidence for the virtuals — the target addresses are real code in the
// binary (each begins with a `__thiscall` prologue, e.g. 0x61e560 is
// `56 8b f1` = push esi; mov esi, ecx). Slot order is the vtable order.
//
// This header previously did not exist: the definition lived inside
// TechnoClass.h, which no other class included. AnimClass therefore could not
// name StageClass as a base even though RTTI says it has one, so its emitted
// layout was missing a real base sub-object. Giving StageClass its own header
// is what ts_prelude.h always claimed ("defined by its own reconstructed
// header").
// ============================================================================

class StageClass
{
public:
    virtual ~StageClass();                                          // [0] 0x61e560
    virtual void vfunc_1();                                         // [1] 0x61e2b0
    virtual void vfunc_2();                                         // [2] 0x61e430
    virtual void vfunc_3();                                         // [3] 0x61e260
    virtual void vfunc_4();                                         // [4] 0x61e4e0
    virtual void vfunc_5();                                         // [5] 0x61e290
};
