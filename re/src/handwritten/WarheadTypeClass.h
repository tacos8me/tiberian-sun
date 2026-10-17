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
    unsigned char pad_doc_gap_02e[0x6e];  // holds the gap before UseFireParticles, which documents 0xc9 (the compiler would put it at 0x2e)
    unsigned int m_09c;  // +0x09c (gap member); 4-byte store: mov dword ptr [esi + 0x9c], 0x6a3c48
    unsigned int m_0a0;  // +0x0a0 (gap member); 4-byte store: mov dword ptr [esi + 0xa0], eax
    unsigned int m_0a4;  // +0x0a4 (gap member); 4-byte store: mov dword ptr [esi + 0xa4], eax
    unsigned char m_0a8;  // +0x0a8 (gap member); 1-byte store: mov byte ptr [esi + 0xa8], 1
    unsigned char m_0a9;  // +0x0a9 (gap member); 1-byte store: mov byte ptr [esi + 0xa9], al
    unsigned char pad_doc_gap_0aa[0x2];  // holds the gap before m_0a9, which documents 0xc9 (the compiler would put it at 0xaa)
    unsigned int m_0ac;  // +0x0ac (gap member); 4-byte store: mov dword ptr [esi + 0xac], eax
    unsigned int m_0b0;  // +0x0b0 (gap member); 4-byte store: mov dword ptr [esi + 0xb0], 0xa
    unsigned char pad_doc_gap_0b4[0x15];  // holds the gap before m_0b0, which documents 0xc9 (the compiler would put it at 0xb4)
    bool  UseFireParticles;      // 0xC9  INI "UseFireParticles"   (0x6E0F2C)
    bool  UseSparkParticles;     // 0xCA  INI "UseSparkParticles"  (0x6E0F14)
    bool  IsRailgun;             // 0xCB  INI "IsRailgun"          (0x6E0F04)
    bool  Lobber;                // 0xCC  INI "Lobber"             (0x6E0EFC)
    bool  Bright;                // 0xCD  INI "Bright"             (0x6E0EE4)
    bool  LaserDuration;         // 0xCE  INI "LaserDuration"      (0x6E0EC8)
    bool  IsBigLaser;            // 0xCF  INI "IsBigLaser"         (0x6E0EB4)
    unsigned char pad_doc_gap_0d0[0x1];  // holds the gap before TurboBoost, which documents 0xd1 (the compiler would put it at 0xd0)
    bool  TurboBoost;            // 0xD1  INI "TurboBoost"         (0x6D5F28)
    unsigned char pad_doc_gap_0d2[0x1];  // holds the gap before Camera, which documents 0xd3 (the compiler would put it at 0xd2)
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
    // vtable-pointer stores are omitted: the compiler supplies the vptr.
    // The machine block begins at +0x164: the compiled end of the
    // base sub-object region (+0x2e) and of the curated block
    // above. Base evidence: AbstractTypeClass at +0x0 (ends +0x2e).
    // 2 recovered offset(s) below +0x2e lie inside the base
    // sub-object region and are NOT declared here: they are declared and
    // compiler-checked in the base's own header -- +0x0, +0x4
    // 7 recovered offset(s) are inside the curated hand-written block
    // above (its span is +0x2e..+0x164) and are NOT repeated here:
    // the curated block wins -- +0x9c, +0xa0, +0xa4, +0xa8, +0xa9, +0xac, +0xb0

};

#endif // WARHEADTYPECLASS_H
