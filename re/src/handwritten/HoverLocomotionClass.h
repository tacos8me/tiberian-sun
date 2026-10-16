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
    unsigned long m_048;  // +0x048 /* ? */; 4-byte store: mov dword ptr [esi + 0x48], ebx
    unsigned long m_04c;  // +0x04c /* ? */; 4-byte store: mov dword ptr [esi + 0x4c], ebx
    unsigned long m_050;  // +0x050 /* ? */; 4-byte store: mov dword ptr [esi + 0x50], ebx
    unsigned long m_054;  // +0x054 /* ? */; 4-byte store: mov dword ptr [esi + 0x54], ebx
    unsigned long m_058;  // +0x058 /* ? */; 4-byte store: mov dword ptr [esi + 0x58], ebx
    unsigned long m_05c;  // +0x05c init 0x3ff00000; 4-byte store: mov dword ptr [esi + 0x5c], 0x3ff00000
    unsigned long m_060;  // +0x060 /* ? */; 4-byte store: mov dword ptr [esi + 0x60], ebx
    unsigned long m_064;  // +0x064 /* ? */; 4-byte store: mov dword ptr [esi + 0x64], ebx
    unsigned char m_068;  // +0x068 /* ? */; 1-byte store: mov byte ptr [esi + 0x68], bl
    unsigned char pad_069[0x3];
    unsigned long m_06c;  // +0x06c /* ? */; 4-byte store: mov dword ptr [esi + 0x6c], ebx
    unsigned char m_070;  // +0x070 /* ? */; 1-byte store: mov byte ptr [esi + 0x70], bl

};

#endif // HOVERLOCOMOTIONCLASS_H
