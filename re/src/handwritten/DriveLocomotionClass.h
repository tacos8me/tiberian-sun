// DriveLocomotionClass.h
// Reconstructed from GAME.EXE (C&C Tiberian Sun, Westwood Studios 1999).
// Evidence: RTTI COL 0x6ad798 -> ".?AVDriveLocomotionClass@@".
// vtables: IPersistStream 0x69d7a4, ILocomotion 0x69d6dc, IPiggyback 0x69d6b8.
// Constructor 0x4798d0; scalar deleting dtor 0x479870.
//
// Object layout (ctor 0x4798d0):
//   +0x00  IPersistStream vftable (0x69d7a4)
//   +0x04  ILocomotion     vftable (0x69d6dc)
//   +0x08  bool  m_bIsMoving (inherited)
//   +0x14  IPiggyback      vftable (0x69d6b8)
//   +0x68  void* m_pPiggybackedObject /* ? */ (freed in dtor)
//
// Movement parameters are read from the owning TechnoTypeClass INI, not stored
// here: "Speed" (0x6c3334), "Locomotor" (0x6dba30), "AccelerationFactor"
// (0x6db9c4), "DeaccelerationFactor" (0x6db9d8), "Weight" (0x6db9bc),
// "TrackedUphill"/"TrackedDownhill" (0x6d6958/0x6d6948), "IceBreakingWeight"
// (0x6d6990), "IceCrackingWeight" (0x6d69a4), "MovementZone" (0x6db5xx).

#ifndef DRIVELOCOMOTIONCLASS_H
#define DRIVELOCOMOTIONCLASS_H

#include "LocomotionClass.h"

class DriveLocomotionClass : public LocomotionClass, public IPiggyback
{
public:
    // +0x00  IPersistStream vftable (0x69d7a4)
    // +0x04  ILocomotion     vftable (0x69d6dc)
    // +0x14  IPiggyback      vftable (0x69d6b8)
    // +0x68  void* m_pPiggybackedObject /* ? */

    DriveLocomotionClass();                  // 0x4798d0
    virtual ~DriveLocomotionClass();         // 0x479870 (scalar deleting dtor)

    // ILocomotion overrides (vtable 0x69d6dc)
    virtual HRESULT __stdcall QueryInterface(REFIID riid, void** ppv);  // 0x47eb40
    virtual ULONG   __stdcall AddRef();                                 // 0x47eb50
    virtual ULONG   __stdcall Release();                                // 0x47eb60
    virtual bool    __stdcall Is_Moving();                              // 0x479cc0
    virtual void    __stdcall Head_To(CoordStruct* pDest);              // 0x479dc0
    virtual void    __stdcall Stop_Moving();                            // 0x479df0
    virtual void    __stdcall Mark_All_Occupation_Bits(int mark);       // 0x47a040
    virtual int     __stdcall Get_Status();                             // 0x47e630
    virtual int     __stdcall Get_Status_2();                           // 0x47e640
    virtual bool    __stdcall Is_Ok_To_End();                           // 0x47a520
    virtual void    __stdcall Slot17();                                 // 0x479e60
    virtual void    __stdcall Slot18();                                 // 0x479ee0
    virtual void    __stdcall Slot19();                                 // 0x47ae90
    virtual void    __stdcall Slot20();                                 // 0x47a4f0
    virtual void    __stdcall Slot28();                                 // 0x47abf0
    virtual void    __stdcall Slot29();                                 // 0x47e5e0
    virtual void    __stdcall Slot31();                                 // 0x479c80
    virtual void    __stdcall Slot32();                                 // 0x479d50
    virtual void    __stdcall Slot39();                                 // 0x47e690
    virtual bool    __stdcall Slot40();                                 // 0x47e6e0
    virtual bool    __stdcall Slot41();                                 // 0x47e8c0
    virtual void    __stdcall Slot44();                                 // 0x47e9a0
    virtual void    __stdcall Slot45();                                 // 0x47e9b0
    virtual int     __stdcall Slot46();                                 // 0x47e9c0
    virtual int     __stdcall Slot47();                                 // 0x47e9d0
    virtual int     __stdcall Slot48();                                 // 0x47e9e0

    // IPiggyback overrides (vtable 0x69d6b8)
    virtual HRESULT __stdcall Begin_Piggyback(TechnoClass* pPassenger); // 0x479a30
    virtual HRESULT __stdcall End_Piggyback(TechnoClass* pPassenger);   // 0x479a80
    virtual bool    __stdcall Is_Ok_To_End_Piggyback();                 // 0x479ac0
    virtual HRESULT __stdcall Piggyback_Available();                    // 0x479760
    virtual bool    __stdcall Is_Piggybacking();                        // 0x47ea80
};

#endif // DRIVELOCOMOTIONCLASS_H
