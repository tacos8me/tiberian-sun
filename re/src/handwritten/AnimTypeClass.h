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
    bool  Theater;               // 0xC6  INI "Theater"              (0x6BFEEC)
    bool  NewTheater;            // 0xD0  INI "NewTheater"           (0x6BFEE4)
    bool  TiberiumChainReaction; // 0x1D7 INI "TiberiumChainReaction"(0x6BFE14)
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
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_128;  // /* ? */
    unsigned char pad_12c[0x4];
    unsigned long m_130;  // /* ? */
    unsigned long m_134;  // /* ? */
    unsigned long m_138;  // init 0x1
    unsigned long m_13c;  // /* ? */
    unsigned long m_140;  // /* ? */
    unsigned long m_144;  // /* ? */
    unsigned long m_148;  // /* ? */
    unsigned long m_14c;  // /* ? */
    unsigned long m_150;  // /* ? */
    unsigned long m_154;  // /* ? */
    unsigned long m_158;  // /* ? */
    unsigned long m_15c;  // /* ? */
    unsigned long m_160;  // /* ? */
    unsigned long m_164;  // /* ? */
    unsigned long m_168;  // /* ? */
    unsigned long m_16c;  // /* ? */
    unsigned long m_170;  // /* ? */
    unsigned long m_174;  // /* ? */
    unsigned long m_178;  // /* ? */
    unsigned long m_17c;  // /* ? */
    unsigned long m_180;  // /* ? */
    unsigned long m_184;  // /* ? */
    unsigned long m_188;  // /* ? */
    unsigned long m_18c;  // /* ? */
    unsigned long m_190;  // /* ? */
    unsigned long m_194;  // /* ? */
    unsigned long m_198;  // init 0xa0000000
    unsigned long m_19c;  // init 0x3fe99999
    unsigned long m_1a0;  // /* ? */
    unsigned long m_1a4;  // /* ? */
    unsigned long m_1a8;  // /* ? */
    unsigned long m_1ac;  // /* ? */
    unsigned long m_1b0;  // /* ? */
    unsigned long m_1b4;  // init 0x402e0000
    unsigned long m_1b8;  // /* ? */
    unsigned long m_1bc;  // /* ? */
    unsigned long m_1c0;  // /* ? */
    unsigned long m_1c4;  // /* ? */
    unsigned long m_1c8;  // /* ? */
    unsigned long m_1cc;  // /* ? */
    unsigned long m_1d0;  // /* ? */
    // 12 member(s) already declared above; their offsets are recorded in out/members.json

};

#endif // ANIMTYPECLASS_H
