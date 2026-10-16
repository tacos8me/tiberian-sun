// HoverLocomotionClass.h
// Reconstructed from GAME.EXE (C&C Tiberian Sun, Westwood Studios 1999).
// Evidence: RTTI COL 0x6b02b0 -> ".?AVHoverLocomotionClass@@".
// vtables: IPersistStream 0x69fd24, ILocomotion 0x69fc5c.
// Constructor 0x4cae20; scalar deleting dtor 0x4cade0.
//
// Object layout (ctor 0x4cae20):
//   +0x00  IPersistStream vftable (0x69fd24)
//   +0x04  ILocomotion     vftable (0x69fc5c)
//   +0x08  bool  m_bIsMoving (inherited)
//   +0x2C  member initialised by 0x492750 /* ? */
//
// Movement parameters read from the owning TechnoTypeClass INI:
// "HoverAcceleration" (0x6d6a88), "HoverBrake" (0x6d6a7c), "HoverBoost"
// (0x6d6a9c), "HoverBob" (0x6d6ab4), "HoverDampen" (0x6d6ac0), "HoverHeight"
// (0x6d6aa8), "HoverPad" (0x6c1318), "Locomotor" (0x6dba30), "Speed" (0x6c3334).

#ifndef HOVERLOCOMOTIONCLASS_H
#define HOVERLOCOMOTIONCLASS_H

#include "LocomotionClass.h"

class HoverLocomotionClass : public LocomotionClass
{
public:
    // +0x00  IPersistStream vftable (0x69fd24)
    // +0x04  ILocomotion     vftable (0x69fc5c)
    // +0x2C  member /* ? */

    HoverLocomotionClass();                  // 0x4cae20
    virtual ~HoverLocomotionClass();         // 0x4cade0 (scalar deleting dtor)

    // ILocomotion overrides (vtable 0x69fc5c)
    virtual HRESULT __stdcall Init(TechnoClass* pOwner);                // 0x4c7f00
    virtual bool    __stdcall Is_Moving();                              // 0x4c8a90
    virtual void    __stdcall Head_To(CoordStruct* pDest);              // 0x4c8b10
    virtual void    __stdcall Stop_Moving();                            // 0x4c8b70
    virtual void    __stdcall Mark_All_Occupation_Bits(int mark);       // 0x4c8180
    virtual bool    __stdcall Is_Ok_To_End();                           // 0x4c82a0
    virtual void    __stdcall Slot17();                                 // 0x4c8be0
    virtual void    __stdcall Slot18();                                 // 0x4ca0f0
    virtual void    __stdcall Slot19();                                 // 0x4ca140
    virtual void    __stdcall Clear_Moving_Flag();                      // 0x4ca990
    virtual bool    __stdcall Get_Moving_Flag();                        // 0x4caa00
    virtual bool    __stdcall Slot25();                                 // 0x4caa30
    virtual bool    __stdcall Slot26();                                 // 0x4cab80
    virtual bool    __stdcall Slot27();                                 // 0x4cad30
    virtual void    __stdcall Slot29();                                 // 0x4cae70
    virtual void    __stdcall Slot32();                                 // 0x4c8ae0
    virtual void    __stdcall Slot39();                                 // 0x4caf30
    virtual bool    __stdcall Slot40();                                 // 0x4caf80

    // ---- data members (offsets from ctor 0x004c7e70) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    unsigned long m_000;  // +0x000 init 0x69fd24; 4-byte store: mov dword ptr [esi], 0x69fd24
    unsigned long m_004;  // +0x004 init 0x69fc5c; 4-byte store: mov dword ptr [esi + 4], 0x69fc5c
    unsigned char pad_008[0x40];
    // 12 member(s) already declared above; their offsets are recorded in out/members.json

};

#endif // HOVERLOCOMOTIONCLASS_H
