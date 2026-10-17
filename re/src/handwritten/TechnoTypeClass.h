#pragma once
// ============================================================================
// TechnoTypeClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// Evidence:
//   RTTI class descriptor  : 0x6b6e30
//   TypeDescriptor name    : 0x6bf9e8 + 8 -> ".?AVTechnoTypeClass@@"
//   vtable (primary)       : 0x6a5bf0   (COL ptr slot 0x6b6e40)
//   vtable (IRTTITypeInfo) : 0x6a5bd4   (COL ptr slot 0x6b6df0, mdisp=4)
//   constructor            : 0x61ed9c   (writes vtbl 0x6a5bf0 / 0x6a5bd4)
//   INI reader             : 0x61f590   (vtable slot 25)
//   INI reader (cameo)     : 0x6213b0   (vtable slot 5, refs "Cameo"/"XXICON")
//
// TechnoTypeClass is the base for all mobile/armed type records.  Its INI
// reader at 0x61f590 references the field names below; the offsets come from
// the constructor at 0x61ed9c.
// ============================================================================

#include "ObjectTypeClass.h"

class TechnoTypeClass : public ObjectTypeClass
{
public:
    // ---- vtable 0x6a5bf0 (primary, 52 entries) ----------------------------
    // Slots 0..23 inherited; slot 6 is the COL ptr.
    // Slot 0 is a stub shared with 65 other classes (0x405790), not
    // the destructor, so the destructor is declared without claiming a
    // vtable position and slot 0 is named for what it is. Declaring
    // `virtual ~X()` here made the destructor look like slot 0.
    ~TechnoTypeClass();
    virtual void  vfunc_0();                        // [0] 0x405790 (shared stub, 66 classes)
    virtual void  Init();                                           // [1] 0x405830
    virtual void  Shutdown();                                       // [2] 0x405840
    virtual long  QueryInterface(const void*, void**);              // [3] 0x6829a5 (stub)
    virtual unsigned long AddRef();                                 // [4] 0x405990
    virtual long  Read_INI(void* ini, const char* section);         // [5] 0x6213b0  (refs "Cameo","XXICON")
    virtual void  vfunc_6();                                        // [6] 0x621980
    virtual void  vfunc_7();                                        // [7] 0x621bf0
    virtual void  vfunc_8();                                        // [8] 0x6221a0
    virtual void  Set_ID(int id);                                   // [9] 0x4059b0
    virtual int   Get_RTTI();                                       // [10] 0x4059c0
    virtual long  GetClassID(void* pClassID);                       // [11] 0x6829a5 (stub)
    virtual long  IsDirty();                                        // [12] 0x6829a5 (stub)
    virtual long  Load(IStream* pStm);                              // [13] 0x621c90
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
    virtual long  Read_INI_2(void* ini, const char* section);       // [25] 0x61f590  (INI reader)
    virtual int   Fetch_ID_2();                                     // [26] 0x405fb0  (refs "Name")
    virtual int   Get_RTTI_2();                                     // [28] 0x40f600
    virtual void  vfunc_29();                                       // [29] 0x61f470
    virtual void  vfunc_30();                                       // [30] 0x620f30
    virtual void  vfunc_31();                                       // [31] 0x5715f0
    virtual void  vfunc_32();                                       // [32] 0x571610
    virtual void  vfunc_33();                                       // [33] 0x6829a5 (stub)
    virtual void  vfunc_34();                                       // [34] 0x61f4b0
    virtual void  vfunc_35();                                       // [35] 0x61f490
    virtual void  vfunc_36();                                       // [36] 0x6829a5 (stub)
    virtual void  vfunc_37();                                       // [37] 0x571670
    virtual void  vfunc_38();                                       // [38] 0x571710
    virtual void  vfunc_39();                                       // [39] 0x61f4f0
    virtual void  vfunc_40();                                       // [40] 0x40f620
    virtual void  vfunc_41();                                       // [41] 0x620df0
    virtual void  vfunc_42();                                       // [42] 0x61f460
    virtual void  vfunc_43();                                       // [43] 0x61f500
    virtual void  vfunc_44();                                       // [44] 0x61f550
    virtual void  vfunc_45();                                       // [45] 0x622180
    virtual void  vfunc_46();                                       // [46] 0x622240
    virtual void  vfunc_47();                                       // [47] 0x61e2b0
    virtual void  vfunc_48();                                       // [48] 0x61e430
    virtual void  vfunc_49();                                       // [49] 0x61e260
    virtual void  vfunc_50();                                       // [50] 0x61e4e0
    virtual void  vfunc_51();                                       // [51] 0x61e290

    // ---- data members -----------------------------------------------------
    // Evidence: constructor 0x61ed9c writes these offsets; INI reader 0x61f590
    // references the named keys (ini_keys.json, reader VA 0x6419856 = 0x61f590).
    unsigned char pad_doc_gap_124[0x254];  // holds the gap before m_GuardRange, which documents 0x378 (the compiler would put it at 0x124)
    int   m_GuardRange;         // +0x378  INI "GuardRange"
    int   m_WalkRate;           // +0x37c  INI "WalkRate"
    int   m_TypeImmune;         // +0x380  /* ? */ INI "TypeImmune"
    int   m_MoveToShroud;       // +0x384  /* ? */ INI "MoveToShroud"
    int   m_IsTrain;            // +0x388  /* ? */ INI "IsTrain"
    int   m_DoubleOwned;        // +0x38c  /* ? */ INI "DoubleOwned"
    int   m_Explodes;           // +0x390  /* ? */ INI "Explodes"
    int   m_FlightLevel;        // +0x394  /* ? */ INI "FlightLevel"
    int   m_IsDropship;         // +0x398  /* ? */ INI "IsDropship"
    int   m_PitchAngle;         // +0x39c  /* ? */ INI "PitchAngle"
    // +0x3a0..+0x468: further fields initialised by the ctor; names not yet
    // pinned to individual INI keys.  /* ? */

    // ---- data members (offsets from ctor 0x0061ed9c) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    // vtable-pointer stores are omitted: the compiler supplies the vptr.
    // The machine block begins at +0x3a0: the compiled end of the
    // base sub-object region (+0x124) and of the curated block
    // above. Base evidence: ObjectTypeClass at +0x0 (ends +0x124).
    // 3 recovered offset(s) below +0x124 lie inside the base
    // sub-object region and are NOT declared here: they are declared and
    // compiler-checked in the base's own header -- +0x0, +0x4, +0xcd
    // 7 recovered offset(s) are inside the curated hand-written block
    // above (its span is +0x124..+0x3a0) and are NOT repeated here:
    // the curated block wins -- +0x378, +0x37c, +0x380, +0x384, +0x388, +0x38c, +0x39c
    unsigned int m_3a0;  // +0x3a0 init 0xa; 4-byte store: mov dword ptr [esi + 0x3a0], 0xa
    unsigned char pad_3a4[0x4];
    unsigned int m_3a8;  // +0x3a8 /* ? */; 4-byte store: mov dword ptr [esi + 0x3a8], ebx
    unsigned int m_3ac;  // +0x3ac /* ? */; 4-byte store: mov dword ptr [esi + 0x3ac], ebx
    unsigned int m_3b0;  // +0x3b0 /* ? */; 4-byte store: mov dword ptr [esi + 0x3b0], ebx
    unsigned int m_3b4;  // +0x3b4 /* ? */; 4-byte store: mov dword ptr [esi + 0x3b4], ebx
    unsigned char pad_3b8[0x10];
    unsigned int m_3c8;  // +0x3c8 /* ? */; 4-byte store: mov dword ptr [esi + 0x3c8], ebx
    unsigned int m_3cc;  // +0x3cc /* ? */; 4-byte store: mov dword ptr [esi + 0x3cc], eax
    unsigned char pad_3d0[0x4];
    unsigned int m_3d4;  // +0x3d4 /* ? */; 4-byte store: mov dword ptr [esi + 0x3d4], ebx
    unsigned int m_3d8;  // +0x3d8 /* ? */; 4-byte store: mov dword ptr [esi + 0x3d8], ebx
    unsigned int m_3dc;  // +0x3dc /* ? */; 4-byte store: mov dword ptr [esi + 0x3dc], ebx
    unsigned int m_3e0;  // +0x3e0 /* ? */; 4-byte store: mov dword ptr [esi + 0x3e0], ebx
    unsigned int m_3e4;  // +0x3e4 /* ? */; 4-byte store: mov dword ptr [esi + 0x3e4], ebx
    unsigned char m_3e8;  // +0x3e8 /* ? */; 1-byte store: mov byte ptr [esi + 0x3e8], bl
    unsigned char pad_3e9[0x4b];
    unsigned char TypeImmune;  // +0x434 (INI key); 1-byte store: mov byte ptr [esi + 0x434], bl
    unsigned char MoveToShroud;  // +0x435 (INI key); 1-byte store: mov byte ptr [esi + 0x435], 1
    unsigned char m_436;  // +0x436 init 0x1; 1-byte store: mov byte ptr [esi + 0x436], 1
    unsigned char m_437;  // +0x437 init 0x1; 1-byte store: mov byte ptr [esi + 0x437], 1
    unsigned char m_438;  // +0x438 /* ? */; 1-byte store: mov byte ptr [esi + 0x438], bl
    unsigned char m_439;  // +0x439 /* ? */; 1-byte store: mov byte ptr [esi + 0x439], bl
    unsigned char m_43a;  // +0x43a /* ? */; 1-byte store: mov byte ptr [esi + 0x43a], bl
    unsigned char m_43b;  // +0x43b /* ? */; 1-byte store: mov byte ptr [esi + 0x43b], bl
    unsigned char IsTrain;  // +0x43c (INI key); 1-byte store: mov byte ptr [esi + 0x43c], bl
    unsigned char m_43d;  // +0x43d /* ? */; 1-byte store: mov byte ptr [esi + 0x43d], bl
    unsigned char m_43e;  // +0x43e /* ? */; 1-byte store: mov byte ptr [esi + 0x43e], bl
    unsigned char m_43f;  // +0x43f init 0x1; 1-byte store: mov byte ptr [esi + 0x43f], 1
    unsigned char m_440;  // +0x440 /* ? */; 1-byte store: mov byte ptr [esi + 0x440], bl
    unsigned char DoubleOwned;  // +0x441 (INI key); 1-byte store: mov byte ptr [esi + 0x441], bl
    unsigned char m_442;  // +0x442 /* ? */; 1-byte store: mov byte ptr [esi + 0x442], bl
    unsigned char m_443;  // +0x443 /* ? */; 1-byte store: mov byte ptr [esi + 0x443], bl
    unsigned char m_444;  // +0x444 /* ? */; 1-byte store: mov byte ptr [esi + 0x444], bl
    unsigned char m_445;  // +0x445 /* ? */; 1-byte store: mov byte ptr [esi + 0x445], bl
    unsigned char m_446;  // +0x446 /* ? */; 1-byte store: mov byte ptr [esi + 0x446], bl
    unsigned char m_447;  // +0x447 /* ? */; 1-byte store: mov byte ptr [esi + 0x447], bl
    unsigned char m_448;  // +0x448 init 0x1; 1-byte store: mov byte ptr [esi + 0x448], 1
    unsigned char m_449;  // +0x449 /* ? */; 1-byte store: mov byte ptr [esi + 0x449], bl
    unsigned char m_44a;  // +0x44a /* ? */; 1-byte store: mov byte ptr [esi + 0x44a], bl
    unsigned char m_44b;  // +0x44b /* ? */; 1-byte store: mov byte ptr [esi + 0x44b], bl
    unsigned char m_44c;  // +0x44c /* ? */; 1-byte store: mov byte ptr [esi + 0x44c], bl
    unsigned char Explodes;  // +0x44d (INI key); 1-byte store: mov byte ptr [esi + 0x44d], bl
    unsigned char m_44e;  // +0x44e /* ? */; 1-byte store: mov byte ptr [esi + 0x44e], bl
    unsigned char m_44f;  // +0x44f /* ? */; 1-byte store: mov byte ptr [esi + 0x44f], bl
    unsigned char m_450;  // +0x450 /* ? */; 1-byte store: mov byte ptr [esi + 0x450], bl
    unsigned char m_451;  // +0x451 /* ? */; 1-byte store: mov byte ptr [esi + 0x451], bl
    unsigned char m_452;  // +0x452 /* ? */; 1-byte store: mov byte ptr [esi + 0x452], bl
    unsigned char m_453;  // +0x453 /* ? */; 1-byte store: mov byte ptr [esi + 0x453], bl
    unsigned char m_454;  // +0x454 /* ? */; 1-byte store: mov byte ptr [esi + 0x454], bl
    unsigned char m_455;  // +0x455 /* ? */; 1-byte store: mov byte ptr [esi + 0x455], bl
    unsigned char m_456;  // +0x456 init 0x1; 1-byte store: mov byte ptr [esi + 0x456], 1
    unsigned char m_457;  // +0x457 /* ? */; 1-byte store: mov byte ptr [esi + 0x457], bl
    unsigned char m_458;  // +0x458 /* ? */; 1-byte store: mov byte ptr [esi + 0x458], bl
    unsigned char m_459;  // +0x459 init 0x1; 1-byte store: mov byte ptr [esi + 0x459], 1
    unsigned char pad_45a[0x2];
    unsigned int m_45c;  // +0x45c /* ? */; 4-byte store: mov dword ptr [esi + 0x45c], eax
    unsigned int m_460;  // +0x460 init 0x5; 4-byte store: mov dword ptr [esi + 0x460], 5
    unsigned int m_464;  // +0x464 /* ? */; 4-byte store: mov dword ptr [esi + 0x464], eax
    unsigned int m_468;  // +0x468 /* ? */; 4-byte store: mov dword ptr [esi + 0x468], ebx

};
