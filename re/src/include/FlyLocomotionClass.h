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
};

#endif // FLYLOCOMOTIONCLASS_H
