// AnimTypeClass.h — reconstructed from GAME.EXE (C&C Tiberian Sun, Westwood 1999)
//
// Evidence:
//   RTTI:  .?AVAnimTypeClass@@ (out/classes.json chd_va 0x6A9A78)
//          bases: AnimTypeClass -> ObjectTypeClass -> AbstractTypeClass
//                 -> AbstractClass -> IPersistStream -> IPersist
//                 -> IUnknown -> IRTTITypeInfo -> IUnknown
//   vtable: 0x00699ABC (out/vtables.txt "class AnimTypeClass", offset 4)
//           0x00699AD8 (offset 0 — the ObjectTypeClass subobject vtable)
//   TClassFactory<AnimTypeClass> vtable 0x006A4C88
//   DynamicVectorClass<AnimTypeClass*> vtable 0x0069F5D4
//   Read_INI: 0x00417FD0 — reads the keys below from the [Animations]
//             section.  Member offsets are the store targets in that
//             function.
//
// The class is a rules.ini "animation type" record.  Field names are the
// literal INI keys.

#ifndef ANIMTYPECLASS_H
#define ANIMTYPECLASS_H

#include "ObjectTypeClass.h"

class AnimTypeClass : public ObjectTypeClass
{
public:
    AnimTypeClass();
    virtual ~AnimTypeClass();            // vtable 0x00699ABC[0] = 0x00405AF0

    // ---- virtuals in vtable order (0x00699ABC) -------------------------
    // [0] 0x00405AF0  ~AnimTypeClass (scalar deleting dtor)
    // [1] 0x00405B00  /* ? */ (size 10)
    // [2] 0x00405B10  /* ? */ (size 10)
    // [3] 0x00405740  /* ? */ (size 16)
    // [4] 0x00405750  /* ? */ (size 10)
    // [5] 0x00405760  /* ? */ (size 22)

    // ---- rules.ini reader (0x00417FD0) ---------------------------------
    // Reads the [Animations] section.  Offsets are the store targets in
    // the reader; the type is the accessor used (S=GetString).
    //<UNCOMMENT>    bool  Theater;               // 0xC6  INI "Theater"              (0x6BFEEC)  // commented out: it documents 0xc6, which C++ supplies itself (the compiler's own vptr or base sub-object is already there)
    //<UNCOMMENT>    bool  NewTheater;            // 0xD0  INI "NewTheater"           (0x6BFEE4)  // commented out: it documents 0xd0, which C++ supplies itself (the compiler's own vptr or base sub-object is already there)
    unsigned char pad_doc_TiberiumChainReaction[0xb3];  // holds the gap before TiberiumChainReaction, which documents 0x1d7 (the compiler would put it at 0x124)
    bool  TiberiumChainReaction; // 0x1D7 INI "TiberiumChainReaction"(0x6BFE14)
    unsigned char pad_doc_AltPalette[0x8];  // holds the gap before AltPalette, which documents 0x1e0 (the compiler would put it at 0x1d8)
    bool  AltPalette;            // 0x1E0 INI "AltPalette"           (0x6BFE00)
    bool  Normalized;            // 0x1E1 INI "Normalized"           (0x6BFDF4)
    bool  Surface;               // 0x1E2 INI "Surface"              (0x6BFDEC)
    bool  Flat;                  // 0x1E3 INI "Flat"                 (0x6BFDE4)
    bool  Translucent;           // 0x1E4 INI "Translucent"          (0x6BFDD8)
    bool  Scorch;                // 0x1E5 INI "Scorch"               (0x6BFDD0)
    bool  Flamer;                // 0x1E6 INI "Flamer"               (0x6BFDC8)
    bool  Crater;                // 0x1E7 INI "Crater"               (0x6BFDC0)
    bool  Sticky;                // 0x1E8 INI "Sticky"               (0x6BFDB8)
    bool  PingPong;              // 0x1E9 INI "PingPong"             (0x6BFDAC)
    bool  Reverse;               // 0x1EA INI "Reverse"              (0x6BFDA4)
    // /* ? */ "Report" (0x6BFEEC) and "Rate" (0x6BFD9C) are read by this
    //         function; store targets not isolated.
    char  Report[64];            // /* ? */ INI "Report"
    int   Rate;                  // /* ? */ INI "Rate"

    // ---- helpers -------------------------------------------------------
    // 0x004182CF (SmudgeTypeClass reader) and 0x00417FD0 share the
    // ObjectTypeClass base reader 0x00572610.

    // ---- data members (offsets from ctor 0x00417870) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    // vtable-pointer stores are omitted: the compiler supplies the vptr.
    // The machine block begins at +0x230: the compiled end of the
    // base sub-object region (+0x124) and of the curated block
    // above. Base evidence: ObjectTypeClass at +0x0 (ends +0x124).
    // 8 recovered offset(s) below +0x124 lie inside the base
    // sub-object region and are NOT declared here: they are declared and
    // compiler-checked in the base's own header -- +0x0, +0xc8, +0xc9, +0xca, +0xcb, +0xcc, +0xcd, +0xce
    // 67 recovered offset(s) are inside the curated hand-written block
    // above (its span is +0x124..+0x230) and are NOT repeated here:
    // the curated block wins -- +0x124, +0x128, +0x130, +0x134, +0x138, +0x13c, +0x140, +0x144 ...

};

#endif // ANIMTYPECLASS_H
