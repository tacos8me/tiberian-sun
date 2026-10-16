// FlyLocomotionClass.h
// Reconstructed from GAME.EXE (C&C Tiberian Sun, Westwood Studios 1999).
// Evidence: RTTI COL 0x6ade98 -> ".?AVFlyLocomotionClass@@".
// vtables: IPersistStream 0x69e1c0, ILocomotion 0x69e0f8.
// Constructor 0x498ed0; scalar deleting dtor 0x498e90.
//
// Object layout (ctor 0x498ed0): base LocomotionClass only (0x0C bytes).
//
// Movement parameters read from the owning AircraftTypeClass INI:
// "Airspeed" (0x6d...), "PitchSpeed" (0x6dba3c), "PitchAngle" (0x6db...),
// "Locomotor" (0x6dba30), "Weight" (0x6db9bc), "AccelerationFactor"
// (0x6db9c4), "DeaccelerationFactor" (0x6db9d8).

#ifndef FLYLOCOMOTIONCLASS_H
#define FLYLOCOMOTIONCLASS_H

#include "LocomotionClass.h"

class FlyLocomotionClass : public LocomotionClass
{
public:
    // +0x00  IPersistStream vftable (0x69e1c0)
    // +0x04  ILocomotion     vftable (0x69e0f8)

    FlyLocomotionClass();                    // 0x498ed0
    virtual ~FlyLocomotionClass();           // 0x498e90 (scalar deleting dtor)

    // ILocomotion overrides (vtable 0x69e0f8)
    virtual bool    __stdcall Is_Moving();                              // 0x495770
    virtual void    __stdcall Head_To(CoordStruct* pDest);              // 0x4957c0
    virtual void    __stdcall Mark_All_Occupation_Bits(int mark);       // 0x4987b0
    virtual bool    __stdcall Is_Moving_Now();                          // 0x498d20
    virtual void    __stdcall Force_Track(int track, CoordStruct* pCoord); // 0x498b50
    virtual int     __stdcall Get_Speed();                              // 0x498c60
    virtual bool    __stdcall Is_Ok_To_End();                           // 0x495820
    virtual void    __stdcall Slot17();                                 // 0x4959a0
    virtual void    __stdcall Slot18();                                 // 0x495c90
    virtual void    __stdcall Slot19();                                 // 0x498e20
    virtual void    __stdcall Clear_Moving_Flag();                      // 0x498f30
    virtual bool    __stdcall Get_Moving_Flag();                        // 0x498fa0
    virtual bool    __stdcall Slot25();                                 // 0x498fb0
    virtual void    __stdcall Slot29();                                 // 0x498f00
    virtual void    __stdcall Slot32();                                 // 0x4957a0
    virtual void    __stdcall Slot33();                                 // 0x499030
    virtual int     __stdcall Slot36();                                 // 0x499060
    virtual void    __stdcall Slot37();                                 // 0x499090

    // ---- data members (offsets from ctor 0x004956d0) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    unsigned long m_000;  // +0x000 init 0x69e1c0; 4-byte store: mov dword ptr [esi], 0x69e1c0
    unsigned long m_004;  // +0x004 init 0x69e0f8; 4-byte store: mov dword ptr [esi + 4], 0x69e0f8
    unsigned char pad_008[0xc];
    unsigned long m_014;  // +0x014 /* ? */; 4-byte store: mov dword ptr [esi + 0x14], ecx
    unsigned char pad_018[0x14];
    unsigned long m_02c;  // +0x02c /* ? */; 4-byte store: mov dword ptr [esi + 0x2c], eax
    unsigned long m_030;  // +0x030 /* ? */; 4-byte store: mov dword ptr [esi + 0x30], eax
    unsigned char pad_034[0x4];
    unsigned long m_038;  // +0x038 /* ? */; 4-byte store: mov dword ptr [esi + 0x38], eax
    unsigned long m_03c;  // +0x03c /* ? */; 4-byte store: mov dword ptr [esi + 0x3c], eax
    unsigned long m_040;  // +0x040 /* ? */; 4-byte store: mov dword ptr [esi + 0x40], eax
    unsigned long m_044;  // +0x044 /* ? */; 4-byte store: mov dword ptr [esi + 0x44], eax
    unsigned char m_048;  // +0x048 /* ? */; 1-byte store: mov byte ptr [esi + 0x48], al
    unsigned long m_04c;  // +0x04c /* ? */; 4-byte store: mov dword ptr [esi + 0x4c], eax
    unsigned long m_050;  // +0x050 /* ? */; 4-byte store: mov dword ptr [esi + 0x50], eax
    unsigned char m_054;  // +0x054 /* ? */; 1-byte store: mov byte ptr [esi + 0x54], al
    // 3 member(s) already declared above; their offsets are recorded in out/members.json

};

#endif // FLYLOCOMOTIONCLASS_H
