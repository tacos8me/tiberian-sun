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
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    unsigned long m_068;  // +0x068 /* ? */; 4-byte store: mov dword ptr [esi + 0x68], ebx
    unsigned long m_06c;  // +0x06c /* ? */; 4-byte store: mov dword ptr [esi + 0x6c], ebx
    unsigned long m_070;  // +0x070 /* ? */; 4-byte store: mov dword ptr [esi + 0x70], ebx
    unsigned long m_074;  // +0x074 /* ? */; 4-byte store: mov dword ptr [esi + 0x74], ebx
    unsigned long ROF;  // +0x078 (INI key); 4-byte store: mov dword ptr [esi + 0x78], ebx
    unsigned long m_07c;  // +0x07c /* ? */; 4-byte store: mov dword ptr [esi + 0x7c], ebx
    unsigned char pad_084[0x38];
    unsigned long m_0bc;  // +0x0bc /* ? */; 4-byte store: mov dword ptr [esi + 0xbc], ebx
    unsigned char m_0c0;  // +0x0c0 /* ? */; 1-byte store: mov byte ptr [esi + 0xc0], bl
    unsigned char m_0c1;  // +0x0c1 /* ? */; 1-byte store: mov byte ptr [esi + 0xc1], bl
    unsigned char m_0c2;  // +0x0c2 /* ? */; 1-byte store: mov byte ptr [esi + 0xc2], bl
    unsigned char m_0c3;  // +0x0c3 /* ? */; 1-byte store: mov byte ptr [esi + 0xc3], bl
    unsigned char m_0c4;  // +0x0c4 /* ? */; 1-byte store: mov byte ptr [esi + 0xc4], bl
    unsigned char m_0c5;  // +0x0c5 /* ? */; 1-byte store: mov byte ptr [esi + 0xc5], bl
    unsigned char m_0c6;  // +0x0c6 /* ? */; 1-byte store: mov byte ptr [esi + 0xc6], bl
    unsigned char m_0c7;  // +0x0c7 /* ? */; 1-byte store: mov byte ptr [esi + 0xc7], bl
    unsigned char m_0c8;  // +0x0c8 /* ? */; 1-byte store: mov byte ptr [esi + 0xc8], bl
    unsigned char m_0c9;  // +0x0c9 /* ? */; 1-byte store: mov byte ptr [esi + 0xc9], bl
    unsigned char m_0ca;  // +0x0ca /* ? */; 1-byte store: mov byte ptr [esi + 0xca], bl
    unsigned char m_0cb;  // +0x0cb /* ? */; 1-byte store: mov byte ptr [esi + 0xcb], bl
    unsigned char m_0cc;  // +0x0cc /* ? */; 1-byte store: mov byte ptr [esi + 0xcc], bl
    unsigned char m_0cd;  // +0x0cd /* ? */; 1-byte store: mov byte ptr [esi + 0xcd], bl
    unsigned char m_0ce;  // +0x0ce init 0xa; 1-byte store: mov byte ptr [esi + 0xce], 0xa
    unsigned char m_0cf;  // +0x0cf /* ? */; 1-byte store: mov byte ptr [esi + 0xcf], bl
    unsigned char m_0d0;  // +0x0d0 /* ? */; 1-byte store: mov byte ptr [esi + 0xd0], bl
    unsigned char m_0d1;  // +0x0d1 /* ? */; 1-byte store: mov byte ptr [esi + 0xd1], bl
    unsigned char m_0d3;  // +0x0d3 /* ? */; 1-byte store: mov byte ptr [esi + 0xd3], bl
    unsigned char m_0d4;  // +0x0d4 /* ? */; 1-byte store: mov byte ptr [esi + 0xd4], bl
    unsigned char m_0d5;  // +0x0d5 /* ? */; 1-byte store: mov byte ptr [esi + 0xd5], bl
    unsigned char m_0d6;  // +0x0d6 /* ? */; 1-byte store: mov byte ptr [esi + 0xd6], bl
    // 6 member(s) already declared above; their offsets are recorded in out/members.json

};

#endif // WEAPONTYPECLASS_H
