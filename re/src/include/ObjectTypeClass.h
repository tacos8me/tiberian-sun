#pragma once
// ============================================================================
// ObjectTypeClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// Evidence:
//   RTTI class descriptor  : 0x6b3488
//   TypeDescriptor name    : 0x6bf9d0 -> ".?AVObjectTypeClass@@"
//   vtable (primary)       : 0x6a2a90   (COL ptr slot 0x6b3498)
//   vtable (IRTTITypeInfo) : 0x6a2a74   (COL ptr slot 0x6b3448, mdisp=4)
//   constructor            : 0x571220   (writes vtbl 0x6a2a90 / 0x6a2a74)
//   INI reader             : 0x572610   (vtable slot 25)
//
// ObjectTypeClass is the base for every "type" record loaded from rules.ini.
// Its INI reader at 0x572610 references the field names below; the offsets
// come from the constructor at 0x571220.
// ============================================================================

#include "AbstractClass.h"

// ---------------------------------------------------------------------------
// AbstractTypeClass — intermediate base for all rules.ini "type" records.
// Evidence: RTTI chd_va 0x6a9550; TypeDescriptor 0x6bf118 ->
//   ".?AVAbstractTypeClass@@"; vtable 0x698ae8 (COL ptr 0x6a9560);
//   ctor 0x405d50 (writes vtbl 0x698ae8 / 0x698acc).
// Declared here because AbstractTypeClass.h is outside this task's file set.
// ---------------------------------------------------------------------------
class AbstractTypeClass : public AbstractClass
{
public:
    // Slot 0 is a stub shared with 65 other classes (0x405790), not
    // the destructor, so the destructor is declared without claiming a
    // vtable position and slot 0 is named for what it is. Declaring
    // `virtual ~X()` here made the destructor look like slot 0.
    ~AbstractTypeClass();
    virtual void  vfunc_0();                        // [0] 0x405790 (shared stub, 66 classes)
    virtual void  Init();                                           // [1] 0x405830
    virtual void  Shutdown();                                       // [2] 0x405840
    virtual long  QueryInterface(const void*, void**);              // [3] 0x6829a5 (stub)
    virtual unsigned long AddRef();                                 // [4] 0x405990
    virtual long  Read_INI(void* ini, const char* section);         // [5] 0x406020
    virtual void  Set_ID(int id);                                   // [9] 0x4059b0
    virtual int   Get_RTTI();                                       // [10] 0x4059c0
    virtual long  GetClassID(void* pClassID);                       // [11] 0x6829a5 (stub)
    virtual long  IsDirty();                                        // [12] 0x6829a5 (stub)
    virtual long  Load(IStream* pStm);                              // [13] 0x405fe0
    virtual long  Save(IStream* pStm, int fClearDirty);             // [14] 0x4059d0
    virtual long  GetSizeMax(unsigned long* pcbSize);               // [15] 0x4059e0
    virtual void  Compute_CRC();                                    // [16] 0x4059f0
    virtual void  vfunc_17();                                       // [17] 0x405970
    virtual void  vfunc_18();                                       // [18] 0x405a00
    virtual void  vfunc_19();                                       // [19] 0x405a30
    virtual void  vfunc_20();                                       // [20] 0x405a60
    virtual void  vfunc_21();                                       // [21] 0x405a70
    virtual void  vfunc_22();                                       // [22] 0x405a80
    virtual void  vfunc_23();                                       // [23] 0x405ab0
    virtual int   GetType_2();                                      // [24] 0x406010
    virtual long  Read_INI_2(void* ini, const char* section);       // [25] 0x405f20  (refs "Name")
    virtual int   Fetch_ID_2();                                     // [26] 0x405fb0  (refs "Name")
    virtual int   Get_RTTI_2();                                     // [28] 0x406a30

    // Evidence: ctor 0x405d50 writes [ebp+0x14] (name buffer) and [ebp+0x2d].
    unsigned char pad_doc_gap_011[0x3];  // holds the gap before m_Name, which documents 0x14 (the compiler would put it at 0x11)
    char  m_Name[25];       // +0x14  /* ? */ INI "Name" (25-byte inline buffer)
    bool  m_NameSet;        // +0x2d  /* ? */

    // ---- data members (offsets from ctor 0x00405e90) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    // vtable-pointer stores are omitted: the compiler supplies the vptr.
    // The machine block begins at +0x2e: the compiled end of the
    // base sub-object region (+0x11) and of the curated block
    // above. Base evidence: AbstractClass at +0x0 (ends +0x11).
    // 2 recovered offset(s) below +0x11 lie inside the base
    // sub-object region and are NOT declared here: they are declared and
    // compiler-checked in the base's own header -- +0x0, +0x4

};

class ObjectTypeClass : public AbstractTypeClass
{
public:
    // ---- vtable 0x6a2a90 (primary, 40 entries) ----------------------------
    // Slots 0..23 are inherited from AbstractTypeClass; slot 6 is the COL ptr.
    // Slot 0 is a stub shared with 65 other classes (0x405790), not
    // the destructor, so the destructor is declared without claiming a
    // vtable position and slot 0 is named for what it is. Declaring
    // `virtual ~X()` here made the destructor look like slot 0.
    ~ObjectTypeClass();
    virtual void  vfunc_0();                        // [0] 0x405790 (shared stub, 66 classes)
    virtual void  Init();                                           // [1] 0x405830
    virtual void  Shutdown();                                       // [2] 0x405840
    virtual long  QueryInterface(const void*, void**);              // [3] 0x6829a5 (stub)
    virtual unsigned long AddRef();                                 // [4] 0x405990
    virtual long  Read_INI(void* ini, const char* section);         // [5] 0x5729b0  (refs ".SHP")
    virtual void  vfunc_6();                                        // [6] 0x572b80
    virtual void  vfunc_7();                                        // [7] 0x572ba0
    virtual void  vfunc_8();                                        // [8] 0x572d10
    virtual void  Set_ID(int id);                                   // [9] 0x4059b0
    virtual int   Get_RTTI();                                       // [10] 0x4059c0
    virtual long  GetClassID(void* pClassID);                       // [11] 0x6829a5 (stub)
    virtual long  IsDirty();                                        // [12] 0x6829a5 (stub)
    virtual long  Load(IStream* pStm);                              // [13] 0x405fe0
    virtual long  Save(IStream* pStm, int fClearDirty);             // [14] 0x4059d0
    virtual long  GetSizeMax(unsigned long* pcbSize);               // [15] 0x4059e0
    virtual void  Compute_CRC();                                    // [16] 0x4059f0
    virtual void  vfunc_17();                                       // [17] 0x405970
    virtual void  vfunc_18();                                       // [18] 0x405a00
    virtual void  vfunc_19();                                       // [19] 0x405a30
    virtual void  vfunc_20();                                       // [20] 0x405a60
    virtual void  vfunc_21();                                       // [21] 0x405a70
    virtual void  vfunc_22();                                       // [22] 0x405a80
    virtual void  vfunc_23();                                       // [23] 0x405ab0
    virtual int   GetType_2();                                      // [24] 0x406010
    virtual long  Read_INI_2(void* ini, const char* section);       // [25] 0x572610  (INI reader)
    virtual int   Fetch_ID_2();                                     // [26] 0x405fb0  (refs "Name")
    virtual int   Get_RTTI_2();                                     // [28] 0x40f600
    virtual void  vfunc_29();                                       // [29] 0x418f40
    virtual void  vfunc_30();                                       // [30] 0x5715e0
    virtual void  vfunc_31();                                       // [31] 0x5715f0
    virtual void  vfunc_32();                                       // [32] 0x571610
    virtual void  vfunc_33();                                       // [33] 0x6829a5 (stub)
    virtual void  vfunc_34();                                       // [34] 0x571640
    virtual void  vfunc_35();                                       // [35] 0x571650
    virtual void  vfunc_36();                                       // [36] 0x6829a5 (stub)
    virtual void  vfunc_37();                                       // [37] 0x571670
    virtual void  vfunc_38();                                       // [38] 0x571710
    virtual void  vfunc_39();                                       // [39] 0x571660

    // ---- data members -----------------------------------------------------
    // Evidence: constructor 0x571220 writes these offsets; INI reader 0x572610
    // references the named keys (ini_keys.json, reader VA 0x572610).
    unsigned char pad_doc_gap_02e[0x32];  // holds the gap before m_IsCrushable, which documents 0x60 (the compiler would put it at 0x2e)
    bool  m_IsCrushable;        // +0x60  /* ? */ INI "Crushable"
    bool  m_IsSelectable;       // +0x61  /* ? */ INI "Selectable"
    bool  m_IsLegalTarget;      // +0x62  /* ? */ INI "LegalTarget"
    unsigned char pad_doc_gap_063[0x1];  // holds the gap before m_Strength, which documents 0x64 (the compiler would put it at 0x63)
    int   m_Strength;           // +0x64  INI "Strength"
    int   m_Armor;              // +0x68  INI "Armor"
    int   m_Image;              // +0x6c  /* ? */ INI "Image"
    int   m_AlphaImage;         // +0x70  /* ? */ INI "AlphaImage"
    int   m_CrushSound;         // +0x74  /* ? */ INI "CrushSound"
    int   m_RadarInvisible;     // +0x78  /* ? */ INI "RadarInvisible"
    int   m_RadialColor;        // +0x7c  /* ? */ INI "RadialColor"
    int   m_HasRadialIndicator; // +0x80  /* ? */ INI "HasRadialIndicator"
    int   m_Insignificant;      // +0x84  /* ? */ INI "Insignificant"
    int   m_Immune;             // +0x88  /* ? */ INI "Immune"
    int   m_IgnoresFirestorm;   // +0x8c  /* ? */ INI "IgnoresFirestorm"
    int   m_Theater;            // +0x90  /* ? */ INI "Theater"
    int   m_NewTheater;         // +0x94  /* ? */ INI "NewTheater"
    int   m_Voxel;              // +0x98  /* ? */ INI "Voxel"
    // +0x9c..+0x120: further fields initialised by the ctor; names not yet
    // pinned to individual INI keys.  /* ? */

    // ---- data members (offsets from ctor 0x00571410) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    // vtable-pointer stores are omitted: the compiler supplies the vptr.
    // The machine block begins at +0x9c: the compiled end of the
    // base sub-object region (+0x2e) and of the curated block
    // above. Base evidence: AbstractTypeClass at +0x0 (ends +0x2e).
    // 2 recovered offset(s) below +0x2e lie inside the base
    // sub-object region and are NOT declared here: they are declared and
    // compiler-checked in the base's own header -- +0x0, +0x4
    // 3 recovered offset(s) are inside the curated hand-written block
    // above (its span is +0x2e..+0x9c) and are NOT repeated here:
    // the curated block wins -- +0x64, +0x68, +0x6c
    // 5 recovered offset(s) fall inside a wider declaration above
    // (the store width and the recovered table disagree): +0xc9, +0xca, +0xcb, +0xcd, +0xce
    unsigned char pad_09c[0x2b];
    unsigned char Crushable;  // +0x0c7 (INI key); /* width ? */
    unsigned int m_0c8;  // +0x0c8 /* ? */; /* width ? */
    unsigned int Immune;  // +0x0cc (INI key); /* width ? */
    unsigned char pad_0d0[0x4];
    unsigned int m_0d4;  // +0x0d4 /* ? */; 4-byte store: mov dword ptr [esi + 0xd4], eax
    unsigned int m_0d8;  // +0x0d8 /* ? */; 4-byte store: mov dword ptr [esi + 0xd8], eax
    unsigned int m_0dc;  // +0x0dc /* ? */; 4-byte store: mov dword ptr [esi + 0xdc], eax
    unsigned char m_0e0;  // +0x0e0 /* ? */; 1-byte store: mov byte ptr [esi + 0xe0], al
    unsigned char pad_0e1[0x3];
    unsigned int m_0e4;  // +0x0e4 /* ? */; 4-byte store: mov dword ptr [esi + 0xe4], eax
    unsigned int m_0e8;  // +0x0e8 /* ? */; 4-byte store: mov dword ptr [esi + 0xe8], eax
    unsigned int m_0ec;  // +0x0ec /* ? */; 4-byte store: mov dword ptr [esi + 0xec], eax
    unsigned int m_0f0;  // +0x0f0 /* ? */; 4-byte store: mov dword ptr [esi + 0xf0], eax
    unsigned char m_0f4;  // +0x0f4 /* ? */; 1-byte store: mov byte ptr [esi + 0xf4], al
    unsigned char pad_0f5[0x3];
    unsigned int m_0f8;  // +0x0f8 /* ? */; 4-byte store: mov dword ptr [esi + 0xf8], eax
    unsigned int m_0fc;  // +0x0fc /* ? */; 4-byte store: mov dword ptr [esi + 0xfc], eax
    unsigned int m_100;  // +0x100 /* ? */; 4-byte store: mov dword ptr [esi + 0x100], eax
    unsigned int m_104;  // +0x104 /* ? */; 4-byte store: mov dword ptr [esi + 0x104], eax
    unsigned char m_108;  // +0x108 /* ? */; 1-byte store: mov byte ptr [esi + 0x108], al
    unsigned char pad_109[0x3];
    unsigned int m_10c;  // +0x10c /* ? */; 4-byte store: mov dword ptr [esi + 0x10c], eax
    unsigned int m_110;  // +0x110 /* ? */; 4-byte store: mov dword ptr [esi + 0x110], eax
    unsigned int m_114;  // +0x114 /* ? */; 4-byte store: mov dword ptr [esi + 0x114], eax
    unsigned int m_118;  // +0x118 /* ? */; 4-byte store: mov dword ptr [esi + 0x118], eax
    unsigned char m_11c;  // +0x11c /* ? */; 1-byte store: mov byte ptr [esi + 0x11c], al
    unsigned char pad_11d[0x3];
    unsigned int m_120;  // +0x120 /* ? */; 4-byte store: mov dword ptr [esi + 0x120], eax

};
