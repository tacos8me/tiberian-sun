// BulletTypeClass.h — reconstructed from GAME.EXE (C&C Tiberian Sun, Westwood 1999)
//
// Evidence:
//   RTTI:  .?AVBulletTypeClass@@ (out/classes.json chd_va 0x6A9A38)
//          bases: BulletTypeClass -> ObjectTypeClass -> AbstractTypeClass
//                 -> AbstractClass -> IPersistStream -> IPersist
//                 -> IUnknown -> IRTTITypeInfo -> IUnknown
//   vtable: 0x0069AA7C (out/vtables.txt "class BulletTypeClass", offset 4)
//           0x0069AA98 (offset 0 — the ObjectTypeClass subobject vtable)
//   TClassFactory<BulletTypeClass> vtable 0x006A4BB0
//   DynamicVectorClass<BulletTypeClass*> vtable 0x0069F644
//   Read_INI: 0x00445BA0 — reads the keys below from the [Projectiles]
//             section.  Member offsets are the store targets in that
//             function.
//
// The class is a rules.ini "projectile type" record.  Field names are the
// literal INI keys.

#ifndef BULLETTYPECLASS_H
#define BULLETTYPECLASS_H

#include "ObjectTypeClass.h"

class BulletTypeClass : public ObjectTypeClass
{
public:
    BulletTypeClass();
    virtual ~BulletTypeClass();          // vtable 0x0069AA7C[0] = 0x00405AF0

    // ---- virtuals in vtable order (0x0069AA7C) -------------------------
    // [0] 0x00405AF0  ~BulletTypeClass (scalar deleting dtor)
    // [1] 0x00405B00  /* ? */ (size 10)
    // [2] 0x00405B10  /* ? */ (size 10)
    // [3] 0x00405740  /* ? */ (size 16)
    // [4] 0x00405750  /* ? */ (size 10)
    // [5] 0x00405760  /* ? */ (size 22)

    // ---- rules.ini reader (0x00445BA0) ---------------------------------
    // Reads the [Projectiles] section.  Offsets are the store targets in
    // the reader; the type is the accessor used (S=GetString).
    bool  Airburst;              // 0x124 INI "Airburst"      (0x6BFEEC)
    bool  Floater;               // 0x125 INI "Floater"       (0x6BFEE4)
    bool  High;                  // 0x126 INI "High"          (0x6BFEDC)
    bool  VeryHigh;              // 0x127 INI "VeryHigh"      (0x6BFED4)
    bool  Shadow;                // 0x128 INI "Shadow"        (0x6BFECC)
    bool  Arcing;                // 0x129 INI "Arcing"        (0x6BFEC4)
    bool  Dropping;              // 0x12A INI "Dropping"      (0x6BFEBC)
    bool  Inviso;                // 0x12B INI "Inviso"        (0x6BFEB4)
    bool  Proximity;             // 0x12C INI "Proximity"     (0x6BFEAC)
    bool  Ranged;                // 0x12D INI "Ranged"        (0x6BFEA4)
    bool  Inaccuate;             // 0x12F INI "Inaccuate"     (0x6BFE9C)
    bool  Degenerates;           // 0x132 INI "Degenerates"   (0x6BFE94)
    bool  Bouncy;                // 0x133 INI "Bouncy"        (0x6BFE8C)
    double Acceleration;         // 0x148 INI "Acceleration"  (0x6BFE84)
    // /* ? */ "Elasticity", "Color", "Cluster", "Image", "Trailer" are
    //         read by this function; store targets not isolated.
    double Elasticity;           // /* ? */ INI "Elasticity"
    int    Color;                // /* ? */ INI "Color" (RGB)
    bool   Cluster;              // /* ? */ INI "Cluster"
    char   Image[64];            // /* ? */ INI "Image"
    char   Trailer[64];          // /* ? */ INI "Trailer"

    // ---- helpers -------------------------------------------------------
    // 0x00445BA0 calls the ObjectTypeClass base reader 0x00572610.

    // ---- data members (offsets from ctor 0x00445900) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    unsigned long m_000;  // +0x000 init 0x69aa98; 4-byte store: mov dword ptr [esi], 0x69aa98
    unsigned char pad_004[0xc4];
    unsigned char m_0c8;  // +0x0c8 init 0x1; 1-byte store: mov byte ptr [esi + 0xc8], 1
    unsigned char m_0cc;  // +0x0cc init 0x1; 1-byte store: mov byte ptr [esi + 0xcc], 1
    unsigned char m_128;  // +0x128 init 0x1; 1-byte store: mov byte ptr [esi + 0x128], 1
    unsigned char m_12c;  // +0x12c /* ? */; 1-byte store: mov byte ptr [esi + 0x12c], bl
    unsigned char m_130;  // +0x130 /* ? */; 1-byte store: mov byte ptr [esi + 0x130], bl
    unsigned char m_134;  // +0x134 /* ? */; 1-byte store: mov byte ptr [esi + 0x134], bl
    unsigned long m_138;  // +0x138 init 0x1; 4-byte store: mov dword ptr [esi + 0x138], 1
    unsigned long m_13c;  // +0x13c /* ? */; 4-byte store: mov dword ptr [esi + 0x13c], ebx
    unsigned long m_140;  // +0x140 /* ? */; 4-byte store: mov dword ptr [esi + 0x140], ebx
    unsigned long m_144;  // +0x144 init 0x3fe80000; 4-byte store: mov dword ptr [esi + 0x144], 0x3fe80000
    unsigned long m_148;  // +0x148 init 0x3; 4-byte store: mov dword ptr [esi + 0x148], 3
    unsigned long m_14c;  // +0x14c /* ? */; 4-byte store: mov dword ptr [esi + 0x14c], ebx
    unsigned long m_150;  // +0x150 /* ? */; 4-byte store: mov dword ptr [esi + 0x150], ebx
    unsigned long ROT;  // +0x154 (INI key); 4-byte store: mov dword ptr [esi + 0x154], ebx
    unsigned long Arm;  // +0x158 (INI key); 4-byte store: mov dword ptr [esi + 0x158], ebx
    unsigned char m_15c;  // +0x15c /* ? */; 1-byte store: mov byte ptr [esi + 0x15c], bl
    // 22 member(s) already declared above; their offsets are recorded in out/members.json

};

#endif // BULLETTYPECLASS_H
