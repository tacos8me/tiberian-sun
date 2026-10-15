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
};

#endif // BULLETTYPECLASS_H
