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
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    unsigned char m_0c9;  // +0x0c9 /* ? */; 1-byte store: mov byte ptr [esi + 0xc9], bl
    unsigned char m_0ca;  // +0x0ca /* ? */; 1-byte store: mov byte ptr [esi + 0xca], bl
    unsigned char m_0cb;  // +0x0cb init 0x1; 1-byte store: mov byte ptr [esi + 0xcb], 1
    unsigned char m_0cd;  // +0x0cd init 0x1; 1-byte store: mov byte ptr [esi + 0xcd], 1
    unsigned char m_0ce;  // +0x0ce /* ? */; 1-byte store: mov byte ptr [esi + 0xce], bl
    unsigned char pad_0cf[0x55];
    unsigned long m_128;  // +0x128 /* ? */; 4-byte store: mov dword ptr [esi + 0x128], ebx
    unsigned char pad_12c[0x4];
    unsigned long m_130;  // +0x130 /* ? */; 4-byte store: mov dword ptr [esi + 0x130], ebx
    unsigned long m_134;  // +0x134 /* ? */; 4-byte store: mov dword ptr [esi + 0x134], ebx
    unsigned long m_138;  // +0x138 init 0x1; 4-byte store: mov dword ptr [esi + 0x138], 1
    unsigned long m_13c;  // +0x13c /* ? */; 4-byte store: mov dword ptr [esi + 0x13c], ebx
    unsigned long m_140;  // +0x140 /* ? */; 4-byte store: mov dword ptr [esi + 0x140], ebx
    unsigned long m_144;  // +0x144 /* ? */; 4-byte store: mov dword ptr [esi + 0x144], ebx
    unsigned long m_148;  // +0x148 /* ? */; 4-byte store: mov dword ptr [esi + 0x148], ebx
    unsigned long m_14c;  // +0x14c /* ? */; 4-byte store: mov dword ptr [esi + 0x14c], ebx
    unsigned long m_150;  // +0x150 /* ? */; 4-byte store: mov dword ptr [esi + 0x150], edi
    unsigned long m_154;  // +0x154 /* ? */; 4-byte store: mov dword ptr [esi + 0x154], ebx
    unsigned long m_158;  // +0x158 /* ? */; 4-byte store: mov dword ptr [esi + 0x158], ebx
    unsigned long m_15c;  // +0x15c /* ? */; 4-byte store: mov dword ptr [esi + 0x15c], ebx
    unsigned long m_160;  // +0x160 /* ? */; 4-byte store: mov dword ptr [esi + 0x160], ebx
    unsigned long m_164;  // +0x164 /* ? */; 4-byte store: mov dword ptr [esi + 0x164], ebx
    unsigned long m_168;  // +0x168 /* ? */; 4-byte store: mov dword ptr [esi + 0x168], ebx
    unsigned long m_16c;  // +0x16c /* ? */; 4-byte store: mov dword ptr [esi + 0x16c], ebx
    unsigned long m_170;  // +0x170 /* ? */; 4-byte store: mov dword ptr [esi + 0x170], ebx
    unsigned long m_174;  // +0x174 /* ? */; 4-byte store: mov dword ptr [esi + 0x174], ebx
    unsigned long m_178;  // +0x178 /* ? */; 4-byte store: mov dword ptr [esi + 0x178], ebx
    unsigned long m_17c;  // +0x17c /* ? */; 4-byte store: mov dword ptr [esi + 0x17c], edi
    unsigned long m_180;  // +0x180 /* ? */; 4-byte store: mov dword ptr [esi + 0x180], edi
    unsigned long m_184;  // +0x184 /* ? */; 4-byte store: mov dword ptr [esi + 0x184], edi
    unsigned long m_188;  // +0x188 /* ? */; 4-byte store: mov dword ptr [esi + 0x188], ebx
    unsigned long m_18c;  // +0x18c /* ? */; 4-byte store: mov dword ptr [esi + 0x18c], ebx
    unsigned long m_190;  // +0x190 /* ? */; 4-byte store: mov dword ptr [esi + 0x190], ebx
    unsigned long m_194;  // +0x194 /* ? */; 4-byte store: mov dword ptr [esi + 0x194], ebx
    unsigned long m_198;  // +0x198 init 0xa0000000; 4-byte store: mov dword ptr [esi + 0x198], 0xa0000000
    unsigned long m_19c;  // +0x19c init 0x3fe99999; 4-byte store: mov dword ptr [esi + 0x19c], 0x3fe99999
    unsigned long m_1a0;  // +0x1a0 /* ? */; 4-byte store: mov dword ptr [esi + 0x1a0], ebx
    unsigned long m_1a4;  // +0x1a4 /* ? */; 4-byte store: mov dword ptr [esi + 0x1a4], eax
    unsigned long m_1a8;  // +0x1a8 /* ? */; 4-byte store: mov dword ptr [esi + 0x1a8], ebx
    unsigned long m_1ac;  // +0x1ac /* ? */; 4-byte store: mov dword ptr [esi + 0x1ac], eax
    unsigned long m_1b0;  // +0x1b0 /* ? */; 4-byte store: mov dword ptr [esi + 0x1b0], ebx
    unsigned long m_1b4;  // +0x1b4 init 0x402e0000; 4-byte store: mov dword ptr [esi + 0x1b4], 0x402e0000
    unsigned long m_1b8;  // +0x1b8 /* ? */; 4-byte store: mov dword ptr [esi + 0x1b8], ebx
    unsigned long m_1bc;  // +0x1bc /* ? */; 4-byte store: mov dword ptr [esi + 0x1bc], ebx
    unsigned long m_1c0;  // +0x1c0 /* ? */; 4-byte store: mov dword ptr [esi + 0x1c0], ebx
    unsigned long m_1c4;  // +0x1c4 /* ? */; 4-byte store: mov dword ptr [esi + 0x1c4], ebx
    unsigned long m_1c8;  // +0x1c8 /* ? */; 4-byte store: mov dword ptr [esi + 0x1c8], ebx
    unsigned long m_1cc;  // +0x1cc /* ? */; 4-byte store: mov dword ptr [esi + 0x1cc], ebx
    unsigned long m_1d0;  // +0x1d0 /* ? */; 4-byte store: mov dword ptr [esi + 0x1d0], ebx
    unsigned char m_1d5;  // +0x1d5 /* ? */; 1-byte store: mov byte ptr [esi + 0x1d5], bl
    unsigned char m_1d6;  // +0x1d6 /* ? */; 1-byte store: mov byte ptr [esi + 0x1d6], bl
    unsigned char m_1d9;  // +0x1d9 /* ? */; 1-byte store: mov byte ptr [esi + 0x1d9], bl
    unsigned char m_1da;  // +0x1da /* ? */; 1-byte store: mov byte ptr [esi + 0x1da], bl
    unsigned char m_1db;  // +0x1db init 0x1; 1-byte store: mov byte ptr [esi + 0x1db], 1
    unsigned char m_1dd;  // +0x1dd /* ? */; 1-byte store: mov byte ptr [esi + 0x1dd], bl
    unsigned char m_1de;  // +0x1de /* ? */; 1-byte store: mov byte ptr [esi + 0x1de], bl
    unsigned char m_1df;  // +0x1df /* ? */; 1-byte store: mov byte ptr [esi + 0x1df], bl
    unsigned char m_1eb;  // +0x1eb init 0x1; 1-byte store: mov byte ptr [esi + 0x1eb], 1
    // 20 member(s) already declared above; their offsets are recorded in out/members.json

};

#endif // ANIMTYPECLASS_H
