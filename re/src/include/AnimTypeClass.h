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
    unsigned int m_124;  // +0x124 (gap member); 4-byte store: mov dword ptr [esi + 0x124], edi
    unsigned int m_128;  // +0x128 (gap member); 4-byte store: mov dword ptr [esi + 0x128], ebx
    unsigned char pad_doc_gap_12c[0x4];  // holds the gap before m_128, which documents 0x1d7 (the compiler would put it at 0x12c)
    unsigned int m_130;  // +0x130 (gap member); 4-byte store: mov dword ptr [esi + 0x130], ebx
    unsigned int m_134;  // +0x134 (gap member); 4-byte store: mov dword ptr [esi + 0x134], ebx
    unsigned int m_138;  // +0x138 (gap member); 4-byte store: mov dword ptr [esi + 0x138], 1
    unsigned int m_13c;  // +0x13c (gap member); 4-byte store: mov dword ptr [esi + 0x13c], ebx
    unsigned int m_140;  // +0x140 (gap member); 4-byte store: mov dword ptr [esi + 0x140], ebx
    unsigned int m_144;  // +0x144 (gap member); 4-byte store: mov dword ptr [esi + 0x144], ebx
    unsigned int m_148;  // +0x148 (gap member); 4-byte store: mov dword ptr [esi + 0x148], ebx
    unsigned int m_14c;  // +0x14c (gap member); 4-byte store: mov dword ptr [esi + 0x14c], ebx
    unsigned int m_150;  // +0x150 (gap member); 4-byte store: mov dword ptr [esi + 0x150], edi
    unsigned int m_154;  // +0x154 (gap member); 4-byte store: mov dword ptr [esi + 0x154], ebx
    unsigned int m_158;  // +0x158 (gap member); 4-byte store: mov dword ptr [esi + 0x158], ebx
    unsigned int m_15c;  // +0x15c (gap member); 4-byte store: mov dword ptr [esi + 0x15c], ebx
    unsigned int m_160;  // +0x160 (gap member); 4-byte store: mov dword ptr [esi + 0x160], ebx
    unsigned int m_164;  // +0x164 (gap member); 4-byte store: mov dword ptr [esi + 0x164], ebx
    unsigned int m_168;  // +0x168 (gap member); 4-byte store: mov dword ptr [esi + 0x168], ebx
    unsigned int m_16c;  // +0x16c (gap member); 4-byte store: mov dword ptr [esi + 0x16c], ebx
    unsigned int m_170;  // +0x170 (gap member); 4-byte store: mov dword ptr [esi + 0x170], ebx
    unsigned int m_174;  // +0x174 (gap member); 4-byte store: mov dword ptr [esi + 0x174], ebx
    unsigned int m_178;  // +0x178 (gap member); 4-byte store: mov dword ptr [esi + 0x178], ebx
    unsigned int m_17c;  // +0x17c (gap member); 4-byte store: mov dword ptr [esi + 0x17c], edi
    unsigned int m_180;  // +0x180 (gap member); 4-byte store: mov dword ptr [esi + 0x180], edi
    unsigned int m_184;  // +0x184 (gap member); 4-byte store: mov dword ptr [esi + 0x184], edi
    unsigned int m_188;  // +0x188 (gap member); 4-byte store: mov dword ptr [esi + 0x188], ebx
    unsigned int m_18c;  // +0x18c (gap member); 4-byte store: mov dword ptr [esi + 0x18c], ebx
    unsigned int m_190;  // +0x190 (gap member); 4-byte store: mov dword ptr [esi + 0x190], ebx
    unsigned int m_194;  // +0x194 (gap member); 4-byte store: mov dword ptr [esi + 0x194], ebx
    unsigned int m_198;  // +0x198 (gap member); 4-byte store: mov dword ptr [esi + 0x198], 0xa0000000
    unsigned int m_19c;  // +0x19c (gap member); 4-byte store: mov dword ptr [esi + 0x19c], 0x3fe99999
    unsigned int m_1a0;  // +0x1a0 (gap member); 4-byte store: mov dword ptr [esi + 0x1a0], ebx
    unsigned int m_1a4;  // +0x1a4 (gap member); 4-byte store: mov dword ptr [esi + 0x1a4], eax
    unsigned int m_1a8;  // +0x1a8 (gap member); 4-byte store: mov dword ptr [esi + 0x1a8], ebx
    unsigned int m_1ac;  // +0x1ac (gap member); 4-byte store: mov dword ptr [esi + 0x1ac], eax
    unsigned int m_1b0;  // +0x1b0 (gap member); 4-byte store: mov dword ptr [esi + 0x1b0], ebx
    unsigned int m_1b4;  // +0x1b4 (gap member); 4-byte store: mov dword ptr [esi + 0x1b4], 0x402e0000
    unsigned int m_1b8;  // +0x1b8 (gap member); 4-byte store: mov dword ptr [esi + 0x1b8], ebx
    unsigned int m_1bc;  // +0x1bc (gap member); 4-byte store: mov dword ptr [esi + 0x1bc], ebx
    unsigned int m_1c0;  // +0x1c0 (gap member); 4-byte store: mov dword ptr [esi + 0x1c0], ebx
    unsigned int m_1c4;  // +0x1c4 (gap member); 4-byte store: mov dword ptr [esi + 0x1c4], ebx
    unsigned int m_1c8;  // +0x1c8 (gap member); 4-byte store: mov dword ptr [esi + 0x1c8], ebx
    unsigned int m_1cc;  // +0x1cc (gap member); 4-byte store: mov dword ptr [esi + 0x1cc], ebx
    unsigned int m_1d0;  // +0x1d0 (gap member); 4-byte store: mov dword ptr [esi + 0x1d0], ebx
    unsigned char m_1d4;  // +0x1d4 (gap member); 1-byte store: mov byte ptr [esi + 0x1d4], bl
    unsigned char m_1d5;  // +0x1d5 (gap member); 1-byte store: mov byte ptr [esi + 0x1d5], bl
    unsigned char m_1d6;  // +0x1d6 (gap member); 1-byte store: mov byte ptr [esi + 0x1d6], bl
    bool  TiberiumChainReaction; // 0x1D7 INI "TiberiumChainReaction"(0x6BFE14)
    unsigned char m_1d8;  // +0x1d8 (gap member); 1-byte store: mov byte ptr [esi + 0x1d8], bl
    unsigned char m_1d9;  // +0x1d9 (gap member); 1-byte store: mov byte ptr [esi + 0x1d9], bl
    unsigned char m_1da;  // +0x1da (gap member); 1-byte store: mov byte ptr [esi + 0x1da], bl
    unsigned char m_1db;  // +0x1db (gap member); 1-byte store: mov byte ptr [esi + 0x1db], 1
    unsigned char m_1dc;  // +0x1dc (gap member); 1-byte store: mov byte ptr [esi + 0x1dc], bl
    unsigned char m_1dd;  // +0x1dd (gap member); 1-byte store: mov byte ptr [esi + 0x1dd], bl
    unsigned char m_1de;  // +0x1de (gap member); 1-byte store: mov byte ptr [esi + 0x1de], bl
    unsigned char m_1df;  // +0x1df (gap member); 1-byte store: mov byte ptr [esi + 0x1df], bl
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
