// WarheadTypeClass.h — reconstructed from GAME.EXE (C&C Tiberian Sun, Westwood 1999)
//
// Evidence:
//   RTTI:  .?AVWarheadTypeClass@@ (out/classes.json chd_va 0x6B7A40)
//          bases: WarheadTypeClass -> AbstractTypeClass -> AbstractClass
//                 -> IPersistStream -> IPersist -> IUnknown
//                 -> IRTTITypeInfo -> IUnknown
//   vtable: 0x006A7214 (out/vtables.txt "class WarheadTypeClass")
//           [0] 0x00405AF0  [1] 0x00405B00  [2] 0x00405B10
//           [3] 0x00405740  [4] 0x00405750  [5] 0x00405760
//   TClassFactory<WarheadTypeClass> vtable 0x006A4DD8
//   DynamicVectorClass<WarheadTypeClass*> vtable 0x006989C4
//   Read_INI: 0x00656F60 — reads the keys below from the [Warheads]
//             section.  Member offsets are the store targets in that
//             function.
//
// The class is a rules.ini "warhead type" record.  Field names are the
// literal INI keys.

#ifndef WARHEADTYPECLASS_H
#define WARHEADTYPECLASS_H

#include "ObjectTypeClass.h"  // declares AbstractTypeClass (no separate header)
#include "ts_prelude.h"

class WarheadTypeClass : public AbstractTypeClass
{
public:
    WarheadTypeClass();
    virtual ~WarheadTypeClass();         // vtable 0x006A7214[0] = 0x00405AF0

    // ---- virtuals in vtable order (0x006A7214) -------------------------
    // [0] 0x00405AF0  ~WarheadTypeClass (scalar deleting dtor)
    // [1] 0x00405B00  /* ? */ (size 10)
    // [2] 0x00405B10  /* ? */ (size 10)
    // [3] 0x00405740  /* ? */ (size 16)
    // [4] 0x00405750  /* ? */ (size 10)
    // [5] 0x00405760  /* ? */ (size 22)

    // ---- rules.ini reader (0x00656F60) ---------------------------------
    // Reads the [Warheads] section.  Offsets are the store targets in the
    // reader; the type is the accessor used (S=GetString).
    bool  UseFireParticles;      // 0xC9  INI "UseFireParticles"   (0x6E0F2C)
    bool  UseSparkParticles;     // 0xCA  INI "UseSparkParticles"  (0x6E0F14)
    bool  IsRailgun;             // 0xCB  INI "IsRailgun"          (0x6E0F04)
    bool  Lobber;                // 0xCC  INI "Lobber"             (0x6E0EFC)
    bool  Bright;                // 0xCD  INI "Bright"             (0x6E0EE4)
    bool  LaserDuration;         // 0xCE  INI "LaserDuration"      (0x6E0EC8)
    bool  IsBigLaser;            // 0xCF  INI "IsBigLaser"         (0x6E0EB4)
    bool  TurboBoost;            // 0xD1  INI "TurboBoost"         (0x6D5F28)
    bool  Camera;                // 0xD3  INI "Camera"             (0x6E0F00)
    bool  Charges;               // 0xD4  INI "Charges"            (0x6E0EF0)
    bool  IsLaser;               // 0xD5  INI "IsLaser"            (0x6E0EF8)
    bool  IonSensitive;          // /* ? */ INI "IonSensitive"     (0x6E0E9C)
    // /* ? */ "AttachedParticleSystem" (0x6E0E84) and "Warhead" (0x6BFEEC)
    //         are read by this function but their store targets were not
    //         isolated; both are string-valued.
    char  AttachedParticleSystem[64];  // /* ? */ INI "AttachedParticleSystem"
    char  Warhead[64];                 // /* ? */ INI "Warhead"

    // ---- helpers -------------------------------------------------------
    // 0x00656F60 also reads "LaserInnerColor"/"LaserOuterColor"/
    // "LaserOuterSpread" (0x6E0E60/0x6E0E4C/0x6E0E38) as RGB ints.
    int   LaserInnerColor;       // /* ? */ INI "LaserInnerColor"
    int   LaserOuterColor;       // /* ? */ INI "LaserOuterColor"
    int   LaserOuterSpread;      // /* ? */ INI "LaserOuterSpread"

    // ---- data members (offsets from ctor 0x00651510) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    unsigned long m_000;  // +0x000 init 0x6a7230; 4-byte store: mov dword ptr [esi], 0x6a7230
    unsigned long m_004;  // +0x004 init 0x6a7214; 4-byte store: mov dword ptr [esi + 4], 0x6a7214
    unsigned char pad_008[0x94];
    unsigned long m_09c;  // +0x09c init 0x6a3c48; 4-byte store: mov dword ptr [esi + 0x9c], 0x6a3c48
    unsigned long m_0a0;  // +0x0a0 /* ? */; 4-byte store: mov dword ptr [esi + 0xa0], eax
    unsigned long m_0a4;  // +0x0a4 /* ? */; 4-byte store: mov dword ptr [esi + 0xa4], eax
    unsigned char m_0a8;  // +0x0a8 init 0x1; 1-byte store: mov byte ptr [esi + 0xa8], 1
    unsigned long m_0ac;  // +0x0ac /* ? */; 4-byte store: mov dword ptr [esi + 0xac], eax
    unsigned long m_0b0;  // +0x0b0 init 0xa; 4-byte store: mov dword ptr [esi + 0xb0], 0xa
    // 2 member(s) already declared above; their offsets are recorded in out/members.json

};

#endif // WARHEADTYPECLASS_H
