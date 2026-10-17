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
    unsigned char pad_doc_gap_02e[0x32];  // holds the gap before AmbientDamage, which documents 0x60 (the compiler would put it at 0x2e)
    int   AmbientDamage;      // 0x60  INI "AmbientDamage"  (0x6DEE18)
    int   Burst;              // 0x64  INI "Burst"          (0x6DEE00)
    unsigned int m_068;  // +0x068 (gap member); 4-byte store: mov dword ptr [esi + 0x68], ebx
    int   Damage;             // 0x6C  INI "Damage"         (0x6BFEE4)
    int   Speed;              // 0x70  INI "Speed"          (0x6C3334) GetInt
    unsigned int m_074;  // +0x074 (gap member); 4-byte store: mov dword ptr [esi + 0x74], ebx
    unsigned int m_078;  // +0x078 (gap member); 4-byte store: mov dword ptr [esi + 0x78], ebx
    double Range;             // 0x7C  INI "Range"          (0x6DEDF8) GetDouble
    double MinimumRange;      // 0x80  INI "MinimumRange"   (0x6DEDE8) GetDouble
    unsigned char pad_doc_gap_08c[0x30];  // holds the gap before IsSonic, which documents 0xd0 (the compiler would put it at 0x8c)
    unsigned int m_0bc;  // +0x0bc (gap member); 4-byte store: mov dword ptr [esi + 0xbc], ebx
    unsigned char m_0c0;  // +0x0c0 (gap member); 1-byte store: mov byte ptr [esi + 0xc0], bl
    unsigned char m_0c1;  // +0x0c1 (gap member); 1-byte store: mov byte ptr [esi + 0xc1], bl
    unsigned char m_0c2;  // +0x0c2 (gap member); 1-byte store: mov byte ptr [esi + 0xc2], bl
    unsigned char m_0c3;  // +0x0c3 (gap member); 1-byte store: mov byte ptr [esi + 0xc3], bl
    unsigned char m_0c4;  // +0x0c4 (gap member); 1-byte store: mov byte ptr [esi + 0xc4], bl
    unsigned char m_0c5;  // +0x0c5 (gap member); 1-byte store: mov byte ptr [esi + 0xc5], bl
    unsigned char m_0c6;  // +0x0c6 (gap member); 1-byte store: mov byte ptr [esi + 0xc6], bl
    unsigned char m_0c7;  // +0x0c7 (gap member); 1-byte store: mov byte ptr [esi + 0xc7], bl
    unsigned char m_0c8;  // +0x0c8 (gap member); 1-byte store: mov byte ptr [esi + 0xc8], bl
    unsigned char m_0c9;  // +0x0c9 (gap member); 1-byte store: mov byte ptr [esi + 0xc9], bl
    unsigned char m_0ca;  // +0x0ca (gap member); 1-byte store: mov byte ptr [esi + 0xca], bl
    unsigned char m_0cb;  // +0x0cb (gap member); 1-byte store: mov byte ptr [esi + 0xcb], bl
    unsigned char m_0cc;  // +0x0cc (gap member); 1-byte store: mov byte ptr [esi + 0xcc], bl
    unsigned char m_0cd;  // +0x0cd (gap member); 1-byte store: mov byte ptr [esi + 0xcd], bl
    unsigned char m_0ce;  // +0x0ce (gap member); 1-byte store: mov byte ptr [esi + 0xce], 0xa
    unsigned char m_0cf;  // +0x0cf (gap member); 1-byte store: mov byte ptr [esi + 0xcf], bl
    bool  IsSonic;            // 0xD0  INI "IsSonic"        (0x6DEE10)
    unsigned char m_0d1;  // +0x0d1 (gap member); 1-byte store: mov byte ptr [esi + 0xd1], bl
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
    // vtable-pointer stores are omitted: the compiler supplies the vptr.
    // The machine block begins at +0x193: the compiled end of the
    // base sub-object region (+0x2e) and of the curated block
    // above. Base evidence: AbstractTypeClass at +0x0 (ends +0x2e).
    // 1 curated declaration(s) do not sit at the offset their comment
    // documents -- a comment does not move a member -- so the machine
    // block below is padded from where the compiler actually puts them:
    // MinimumRange documented +0x80, compiler +0x84
    // 1 recovered offset(s) below +0x2e lie inside the base
    // sub-object region and are NOT declared here: they are declared and
    // compiler-checked in the base's own header -- +0x0
    // 33 recovered offset(s) are inside the curated hand-written block
    // above (its span is +0x2e..+0x193) and are NOT repeated here:
    // the curated block wins -- +0x60, +0x64, +0x68, +0x6c, +0x70, +0x74, +0x78, +0x7c ...

};

#endif // WEAPONTYPECLASS_H
