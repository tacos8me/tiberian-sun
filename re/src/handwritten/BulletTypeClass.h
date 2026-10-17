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
    unsigned char pad_doc_Inaccuate[0x1];  // holds the gap before Inaccuate, which documents 0x12f (the compiler would put it at 0x12e)
    bool  Inaccuate;             // 0x12F INI "Inaccuate"     (0x6BFE9C)
    unsigned char pad_doc_Degenerates[0x2];  // holds the gap before Degenerates, which documents 0x132 (the compiler would put it at 0x130)
    bool  Degenerates;           // 0x132 INI "Degenerates"   (0x6BFE94)
    bool  Bouncy;                // 0x133 INI "Bouncy"        (0x6BFE8C)
    unsigned char pad_doc_Acceleration[0x14];  // holds the gap before Acceleration, which documents 0x148 (the compiler would put it at 0x134)
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
    // vtable-pointer stores are omitted: the compiler supplies the vptr.
    // The machine block begins at +0x1dd: the compiled end of the
    // base sub-object region (+0x124) and of the curated block
    // above. Base evidence: ObjectTypeClass at +0x0 (ends +0x124).
    // 8 recovered offset(s) below +0x124 lie inside the base
    // sub-object region and are NOT declared here: they are declared and
    // compiler-checked in the base's own header -- +0x0, +0xc8, +0xc9, +0xca, +0xcb, +0xcc, +0xcd, +0xce
    // 29 recovered offset(s) are inside the curated hand-written block
    // above (its span is +0x124..+0x1dd) and are NOT repeated here:
    // the curated block wins -- +0x124, +0x125, +0x126, +0x127, +0x128, +0x129, +0x12a, +0x12b ...

};

#endif // BULLETTYPECLASS_H
