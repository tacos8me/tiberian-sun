// WeaponTypeClass.h — reconstructed from GAME.EXE (C&C Tiberian Sun, Westwood 1999)
//
// Evidence:
//   RTTI:  .?AVWeaponTypeClass@@ (out/classes.json chd_va 0x6B7A00)
//          bases: WeaponTypeClass -> AbstractTypeClass -> AbstractClass
//                 -> IPersistStream -> IPersist -> IUnknown
//                 -> IRTTITypeInfo -> IUnknown
//   vtable: 0x006A7594 (out/vtables.txt "class WeaponTypeClass")
//           [0] 0x00405AF0  [1] 0x00405B00  [2] 0x00405B10
//           [3] 0x00405740  [4] 0x00405750  [5] 0x00405760
//   TClassFactory<WeaponTypeClass> vtable 0x006A4DF0
//   DynamicVectorClass<WeaponTypeClass*> vtable 0x00698A04
//   Read_INI: 0x00656D40 — reads the keys below from the [WeaponTypes]
//             section.  Member offsets are the store targets in that
//             function (e.g. `mov [esi+0x60], eax` after the
//             AmbientDamage GetString call).
//
// The class is a rules.ini "weapon type" record.  Field names are the
// literal INI keys; the C++ member names are the same identifiers.

#ifndef WEAPONTYPECLASS_H
#define WEAPONTYPECLASS_H

#include "ObjectTypeClass.h"  // declares AbstractTypeClass (no separate header)

class WeaponTypeClass : public AbstractTypeClass
{
public:
    WeaponTypeClass();
    virtual ~WeaponTypeClass();          // vtable 0x006A7594[0] = 0x00405AF0

    // ---- virtuals in vtable order (0x006A7594) -------------------------
    // [0] 0x00405AF0  ~WeaponTypeClass (scalar deleting dtor)
    // [1] 0x00405B00  /* ? */ (size 10)
    // [2] 0x00405B10  /* ? */ (size 10)
    // [3] 0x00405740  /* ? */ (size 16)
    // [4] 0x00405750  /* ? */ (size 10)
    // [5] 0x00405760  /* ? */ (size 22)

    // ---- rules.ini reader (0x00656D40) ---------------------------------
    // Reads the [WeaponTypes] section.  Offsets are the store targets in
    // the reader; the type is the accessor used (S=GetString, I=GetInt,
    // D=GetDouble).
    int   AmbientDamage;      // 0x60  INI "AmbientDamage"  (0x6DEE18)
    int   Burst;              // 0x64  INI "Burst"          (0x6DEE00)
    int   Damage;             // 0x6C  INI "Damage"         (0x6BFEE4)
    int   Speed;              // 0x70  INI "Speed"          (0x6C3334) GetInt
    double Range;             // 0x7C  INI "Range"          (0x6DEDF8) GetDouble
    double MinimumRange;      // 0x80  INI "MinimumRange"   (0x6DEDE8) GetDouble
    bool  IsSonic;            // 0xD0  INI "IsSonic"        (0x6DEE10)
    bool  Supress;            // 0xD2  INI "Supress"        (0x6DEE08)
    char  Report[64];         // /* ? */ INI "Report"       (0x6BFF6C)
    char  Anim[64];           // /* ? */ INI "Anim"         (0x6BEDC4)
    // /* ? */ "Projectile" (0x6E0F4E) is read by 0x0064E25E, a separate
    //         small helper (size 65) that stores the projectile type name.
    char  Projectile[64];     // /* ? */ INI "Projectile"

    // ---- helpers -------------------------------------------------------
    // 0x0064E25E — reads "Projectile" and resolves it to a BulletTypeClass*
    // /* ? */ exact signature not proven.

    // ---- data members (offsets from ctor 0x00656a90) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x6a75b0
    unsigned char pad_004[0x5c];
    // 17 member(s) already declared above; their offsets are recorded in out/members.json

};

#endif // WEAPONTYPECLASS_H
