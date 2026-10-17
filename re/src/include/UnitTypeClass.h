#pragma once
// ============================================================================
// UnitTypeClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// Evidence:
//   RTTI class descriptor  : 0x6b7698
//   TypeDescriptor name    : 0x6dc028 + 8 -> ".?AVUnitTypeClass@@"
//   vtable (primary)       : 0x6b76a8 is the COL ptr; primary vtable 0x6a6a90
//   vtable (IRTTITypeInfo) : 0x6a6a74   (COL ptr slot 0x6b7638, mdisp=4)
//   constructor            : 0x63e470   (writes vtbl 0x6a6a90 / 0x6a6a74)
//   INI reader             : 0x63e8e0   (vtable slot 25)
// ============================================================================

#include "TechnoTypeClass.h"

class UnitTypeClass : public TechnoTypeClass
{
public:
    // ---- vtable 0x6a6a90 (primary, 45 entries) ----------------------------
    // Slot 6 is the RTTI CompleteObjectLocator pointer (0x6b76a8), not code.
    // Slot 0 is a stub shared with 65 other classes (0x405790), not
    // the destructor, so the destructor is declared without claiming a
    // vtable position and slot 0 is named for what it is. Declaring
    // `virtual ~X()` here made the destructor look like slot 0.
    ~UnitTypeClass();
    virtual void  vfunc_0();                        // [0] 0x405790 (shared stub, 66 classes)
    virtual void  Init();                                           // [1] 0x405830
    virtual void  Shutdown();                                       // [2] 0x405840
    virtual long  QueryInterface(const void*, void**);              // [3] 0x63ec50
    virtual unsigned long AddRef();                                 // [4] 0x405990
    virtual long  Read_INI(void* ini, const char* section);         // [5] 0x63ed30  (refs ".SHP")
    virtual void  vfunc_6();                                        // [6] 0x63edc0
    virtual void  vfunc_7();                                        // [7] 0x621bf0
    virtual void  vfunc_8();                                        // [8] 0x63eea0
    virtual void  Set_ID(int id);                                   // [9] 0x4059b0
    virtual int   Get_RTTI();                                       // [10] 0x4059c0
    virtual long  GetClassID(void* pClassID);                       // [11] 0x63ee80
    virtual long  IsDirty();                                        // [12] 0x63ee70
    virtual long  Load(IStream* pStm);                              // [13] 0x63ec90
    virtual long  Save(IStream* pStm, int fClearDirty);             // [14] 0x4059d0
    virtual long  GetSizeMax(unsigned long* pcbSize);               // [15] 0x4059e0
    virtual void  Compute_CRC();                                    // [16] 0x63ee90
    virtual void  vfunc_17();                                       // [17] 0x405970
    virtual void  vfunc_18();                                       // [18] 0x405a00
    virtual void  vfunc_19();                                       // [19] 0x405a30
    virtual void  vfunc_20();                                       // [20] 0x405a60
    virtual void  vfunc_21();                                       // [21] 0x405a70
    virtual void  vfunc_22();                                       // [22] 0x405a80
    virtual void  vfunc_23();                                       // [23] 0x405ab0
    virtual int   GetType_2();                                      // [24] 0x406010
    virtual long  Read_INI_2(void* ini, const char* section);       // [25] 0x63e8e0  (INI reader)
    virtual int   Fetch_ID_2();                                     // [26] 0x405fb0  (refs "Name")
    virtual int   Get_RTTI_2();                                     // [28] 0x63ebd0
    virtual void  vfunc_29();                                       // [29] 0x61f470
    virtual void  vfunc_30();                                       // [30] 0x620f30
    virtual void  vfunc_31();                                       // [31] 0x63e850
    virtual void  vfunc_32();                                       // [32] 0x63e890
    virtual void  vfunc_33();                                       // [33] 0x63e770
    virtual void  vfunc_34();                                       // [34] 0x61f4b0
    virtual void  vfunc_35();                                       // [35] 0x61f490
    virtual void  vfunc_36();                                       // [36] 0x63e820
    virtual void  vfunc_37();                                       // [37] 0x571670
    virtual void  vfunc_38();                                       // [38] 0x571710
    virtual void  vfunc_39();                                       // [39] 0x61f4f0
    virtual void  vfunc_40();                                       // [40] 0x40f620
    virtual void  vfunc_41();                                       // [41] 0x620df0
    virtual void  vfunc_42();                                       // [42] 0x61f460
    virtual void  vfunc_43();                                       // [43] 0x61f500
    virtual void  vfunc_44();                                       // [44] 0x63ec40

    // ---- data members -----------------------------------------------------
    // Evidence: constructor 0x63e470 writes these offsets; INI reader 0x63e8e0
    // references the named keys (ini_keys.json, reader VA 0x6547680 = 0x63e8e0).
    unsigned char pad_doc_gap_46c[0x4];  // holds the gap before m_Field470, which documents 0x470 (the compiler would put it at 0x46c)
    int   m_Field470;           // +0x470 ctor: mov dword ptr [esi+0x470], 0xffffffff
    int   m_Field474;           // +0x474 ctor: mov dword ptr [esi+0x474], 0xffffffff
    int   m_Field478;           // +0x478 ctor: mov dword ptr [esi+0x478], ebx (0)
    int   m_Field47c;           // +0x47c ctor: mov dword ptr [esi+0x47c], ebx (0)
    int   m_Field480;           // +0x480 ctor: mov dword ptr [esi+0x480], ebx (0)
    bool  m_Field484;           // +0x484 ctor: mov byte ptr [esi+0x484], bl (0)
    bool  m_Field485;           // +0x485 ctor: mov byte ptr [esi+0x485], bl (0)
    bool  m_Field486;           // +0x486 ctor: mov byte ptr [esi+0x486], bl (0)
    bool  m_Field487;           // +0x487 ctor: mov byte ptr [esi+0x487], bl (0)
    bool  m_Field488;           // +0x488 ctor: mov byte ptr [esi+0x488], bl (0)
    bool  m_Field489;           // +0x489 ctor: mov byte ptr [esi+0x489], bl (0)
    bool  m_Field48a;           // +0x48a ctor: mov byte ptr [esi+0x48a], bl (0)
    bool  m_Field48b;           // +0x48b ctor: mov byte ptr [esi+0x48b], bl (0)
    bool  m_Field48c;           // +0x48c ctor: mov byte ptr [esi+0x48c], 1
    bool  m_Field48d;           // +0x48d ctor: mov byte ptr [esi+0x48d], bl (0)
    bool  m_Field48e;           // +0x48e ctor: mov byte ptr [esi+0x48e], bl (0)
    bool  m_Field48f;           // +0x48f ctor: mov byte ptr [esi+0x48f], bl (0)
    bool  m_Field490;           // +0x490 ctor: mov byte ptr [esi+0x490], bl (0)
    bool  m_Field491;           // +0x491 ctor: mov byte ptr [esi+0x491], bl (0)
    unsigned char pad_doc_gap_492[0x2];  // holds the gap before m_Field494, which documents 0x494 (the compiler would put it at 0x492)
    int   m_Field494;           // +0x494 ctor: mov dword ptr [esi+0x494], ebx (0)
    bool  m_Field498;           // +0x498 ctor: mov byte ptr [esi+0x498], 0xc
    bool  m_Field499;           // +0x499 ctor: mov byte ptr [esi+0x499], bl (0)
    bool  m_Field49a;           // +0x49a ctor: mov byte ptr [esi+0x49a], bl (0)

    // ---- INI keys read by 0x63e8e0 ----------------------------------------
    // Original rules.ini field names; exact offsets within +0x470..+0x49a not
    // yet individually pinned.  /* ? */
    //   "AltImage", "CarriesCrate", "CrateGoodie", "DeployToFire",
    //   "FiringFrames", "HalfDamageSmokeLocation", "Harvester", "IsTilter",
    //   "LargeVisceroid", "MovementRestrictedTo", "NoMovingFire", "Passive",
    //   "SmallVisceroid", "SpeedType", "TooBigToFitUnderBridge",
    //   "UseTurretShadow", "WalkFrames", "Weeder"

    // ---- data members (offsets from ctor 0x0063e470) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    // vtable-pointer stores are omitted: the compiler supplies the vptr.
    // The machine block begins at +0x49b: the compiled end of the
    // base sub-object region (+0x46c) and of the curated block
    // above. Base evidence: TechnoTypeClass at +0x0 (ends +0x46c).
    // 2 recovered offset(s) below +0x46c lie inside the base
    // sub-object region and are NOT declared here: they are declared and
    // compiler-checked in the base's own header -- +0x0, +0x37c
    // 23 recovered offset(s) are inside the curated hand-written block
    // above (its span is +0x46c..+0x49b) and are NOT repeated here:
    // the curated block wins -- +0x470, +0x474, +0x478, +0x47c, +0x480, +0x484, +0x485, +0x486 ...

};
