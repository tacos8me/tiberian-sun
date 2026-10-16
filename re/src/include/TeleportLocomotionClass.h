// TeleportLocomotionClass.h
// Reconstructed from GAME.EXE (C&C Tiberian Sun, Westwood Studios 1999).
// Evidence: RTTI COL 0x6b6eb8 -> ".?AVTeleportLocomotionClass@@".
// vtables: IPersistStream 0x6a5d9c, ILocomotion 0x6a5cd4.
// Constructor 0x622930; scalar deleting dtor 0x6228f0.
//
// Object layout (ctor 0x622930): base LocomotionClass only (0x0C bytes).
//
// Movement parameters read from the owning TechnoTypeClass INI:
// "Locomotor" (0x6dba30), "Speed" (0x6c3334).

#ifndef TELEPORTLOCOMOTIONCLASS_H
#define TELEPORTLOCOMOTIONCLASS_H

#include "LocomotionClass.h"

class TeleportLocomotionClass : public LocomotionClass
{
public:
    // +0x00  IPersistStream vftable (0x6a5d9c)
    // +0x04  ILocomotion     vftable (0x6a5cd4)

    TeleportLocomotionClass();               // 0x622930
    virtual ~TeleportLocomotionClass();      // 0x6228f0 (scalar deleting dtor)

    // ILocomotion overrides (vtable 0x6a5cd4)
    virtual bool    __stdcall Is_Moving();                              // 0x622780
    virtual void    __stdcall Head_To(CoordStruct* pDest);              // 0x6227d0
    virtual bool    __stdcall Is_Ok_To_End();                           // 0x622890
    virtual void    __stdcall Slot17();                                 // 0x622840
    virtual void    __stdcall Slot18();                                 // 0x622860
    virtual void    __stdcall Slot29();                                 // 0x622960

    // ---- data members (offsets from ctor 0x00622740) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    // 4 member(s) already declared above; their offsets are recorded in out/members.json

};

#endif // TELEPORTLOCOMOTIONCLASS_H
