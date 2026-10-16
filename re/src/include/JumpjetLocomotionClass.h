// JumpjetLocomotionClass.h
// Reconstructed from GAME.EXE (C&C Tiberian Sun, Westwood Studios 1999).
// Evidence: RTTI COL 0x6b1b00 -> ".?AVJumpjetLocomotionClass@@".
// vtables: IPersistStream 0x6a1484, ILocomotion 0x6a13bc.
// Constructor 0x4f0bb0; scalar deleting dtor 0x4f0b70.
//
// Object layout (ctor 0x4f0bb0): base LocomotionClass only (0x0C bytes).
//
// Movement parameters read from the owning TechnoTypeClass INI:
// "JumpJet" (0x6d...), "Locomotor" (0x6dba30), "Speed" (0x6c3334),
// "PitchSpeed" (0x6dba3c), "Weight" (0x6db9bc), "AccelerationFactor"
// (0x6db9c4), "DeaccelerationFactor" (0x6db9d8).

#ifndef JUMPJETLOCOMOTIONCLASS_H
#define JUMPJETLOCOMOTIONCLASS_H

#include "LocomotionClass.h"

class JumpjetLocomotionClass : public LocomotionClass
{
public:
    // +0x00  IPersistStream vftable (0x6a1484)
    // +0x04  ILocomotion     vftable (0x6a13bc)

    JumpjetLocomotionClass();                // 0x4f0bb0
    virtual ~JumpjetLocomotionClass();       // 0x4f0b70 (scalar deleting dtor)

    // ILocomotion overrides (vtable 0x6a13bc)
    virtual bool    __stdcall Is_Moving();                              // 0x4f04b0
    virtual void    __stdcall Head_To(CoordStruct* pDest);              // 0x4f04c0
    virtual void    __stdcall Stop_Moving();                            // 0x4f1a70
    virtual bool    __stdcall Is_Ok_To_End();                           // 0x4f0520
    virtual void    __stdcall Slot17();                                 // 0x4f0720
    virtual void    __stdcall Slot18();                                 // 0x4f0970
    virtual void    __stdcall Slot19();                                 // 0x4f0b40
    virtual void    __stdcall Slot29();                                 // 0x4f0be0
    virtual void    __stdcall Slot32();                                 // 0x4f12f0
    virtual void    __stdcall Slot39();                                 // 0x4f19f0

    // ---- data members (offsets from ctor 0x004f0420) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    unsigned char pad_008[0x18];
    unsigned char m_020;  // +0x020 /* ? */; 1-byte store: mov byte ptr [esi + 0x20], bl
    unsigned char pad_021[0x3];
    unsigned long m_024;  // +0x024 /* ? */; 4-byte store: mov dword ptr [esi + 0x24], ebx
    unsigned char pad_028[0x18];
    unsigned long m_040;  // +0x040 /* ? */; 4-byte store: mov dword ptr [esi + 0x40], ebx
    unsigned long m_044;  // +0x044 /* ? */; 4-byte store: mov dword ptr [esi + 0x44], ebx
    unsigned long m_048;  // +0x048 /* ? */; 4-byte store: mov dword ptr [esi + 0x48], ebx
    unsigned long m_04c;  // +0x04c /* ? */; 4-byte store: mov dword ptr [esi + 0x4c], ebx
    unsigned long m_050;  // +0x050 /* ? */; 4-byte store: mov dword ptr [esi + 0x50], ebx
    unsigned char pad_054[0x4];
    unsigned long m_058;  // +0x058 /* ? */; 4-byte store: mov dword ptr [esi + 0x58], ebx
    unsigned long m_05c;  // +0x05c /* ? */; 4-byte store: mov dword ptr [esi + 0x5c], ebx
    unsigned char m_060;  // +0x060 /* ? */; 1-byte store: mov byte ptr [esi + 0x60], bl
    // 2 member(s) already declared above; their offsets are recorded in out/members.json

};

#endif // JUMPJETLOCOMOTIONCLASS_H
