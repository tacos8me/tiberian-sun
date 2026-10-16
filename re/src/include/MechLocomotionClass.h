// MechLocomotionClass.h
// Reconstructed from GAME.EXE (C&C Tiberian Sun, Westwood Studios 1999).
// Evidence: RTTI COL 0x6b27a8 -> ".?AVMechLocomotionClass@@".
// vtables: IPersistStream 0x6a208c, ILocomotion 0x6a1fc4.
// Constructor 0x5453b0; scalar deleting dtor 0x545370.
//
// Object layout (ctor 0x5453b0): base LocomotionClass only (0x0C bytes).
//
// Movement parameters read from the owning TechnoTypeClass INI:
// "Locomotor" (0x6dba30), "Speed" (0x6c3334), "Weight" (0x6db9bc),
// "AccelerationFactor" (0x6db9c4), "DeaccelerationFactor" (0x6db9d8),
// "TrackedUphill"/"TrackedDownhill" (0x6d6958/0x6d6948).

#ifndef MECHLOCOMOTIONCLASS_H
#define MECHLOCOMOTIONCLASS_H

#include "LocomotionClass.h"
#include "ts_prelude.h"

class MechLocomotionClass : public LocomotionClass
{
public:
    // +0x00  IPersistStream vftable (0x6a208c)
    // +0x04  ILocomotion     vftable (0x6a1fc4)

    MechLocomotionClass();                   // 0x5453b0
    virtual ~MechLocomotionClass();          // 0x545370 (scalar deleting dtor)

    // ILocomotion overrides (vtable 0x6a1fc4)
    virtual bool    __stdcall Is_Moving();                              // 0x543a50
    virtual void    __stdcall Head_To(CoordStruct* pDest);              // 0x543a60
    virtual void    __stdcall Stop_Moving();                            // 0x543ac0
    virtual bool    __stdcall Is_Ok_To_End();                           // 0x543b30
    virtual void    __stdcall Slot17();                                 // 0x543b50
    virtual void    __stdcall Slot18();                                 // 0x543bd0
    virtual void    __stdcall Slot19();                                 // 0x543c20
    virtual void    __stdcall Slot29();                                 // 0x5453e0
    virtual void    __stdcall Slot30();                                 // 0x543c50
    virtual void    __stdcall Slot32();                                 // 0x5453f0
    virtual void    __stdcall Slot39();                                 // 0x545460
    virtual bool    __stdcall Slot40();                                 // 0x5454b0
};

#endif // MECHLOCOMOTIONCLASS_H
