#pragma once
// ============================================================================
// InfantryTypeClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// Evidence:
//   RTTI class descriptor  : 0x6b0420
//   TypeDescriptor name    : 0x6c99e8 + 8 -> ".?AVInfantryTypeClass@@"
//   vtable (primary)       : 0x6b0430 is the COL ptr; primary vtable 0x6a0348
//   vtable (IRTTITypeInfo) : 0x6a032c   (COL ptr slot 0x6b03c0, mdisp=4)
//   constructor            : 0x4d3200   (writes vtbl 0x6a0348 / 0x6a032c)
//   INI reader             : 0x4d3980   (vtable slot 25)
// ============================================================================

#include "TechnoTypeClass.h"

class InfantryTypeClass : public TechnoTypeClass
{
public:
    // ---- vtable 0x6a0348 (primary, 67 entries) ----------------------------
    // Slot 6 is the RTTI CompleteObjectLocator pointer (0x6b0430), not code.
    virtual ~InfantryTypeClass();                                   // [0] 0x405790
    virtual void  Init();                                           // [1] 0x405830
    virtual void  Shutdown();                                       // [2] 0x405840
    virtual long  QueryInterface(const void*, void**);              // [3] 0x4d3fe0
    virtual unsigned long AddRef();                                 // [4] 0x405990
    virtual long  Read_INI(void* ini, const char* section);         // [5] 0x4d3e20
    virtual void  vfunc_6();                                        // [6] 0x4d3f50
    virtual void  vfunc_7();                                        // [7] 0x621bf0
    virtual void  vfunc_8();                                        // [8] 0x4d40e0
    virtual void  Set_ID(int id);                                   // [9] 0x4059b0
    virtual int   Get_RTTI();                                       // [10] 0x4059c0
    virtual long  GetClassID(void* pClassID);                       // [11] 0x4d40b0
    virtual long  IsDirty();                                        // [12] 0x4d40c0
    virtual long  Load(IStream* pStm);                              // [13] 0x4d3d10
    virtual long  Save(IStream* pStm, int fClearDirty);             // [14] 0x4059d0
    virtual long  GetSizeMax(unsigned long* pcbSize);               // [15] 0x4059e0
    virtual void  Compute_CRC();                                    // [16] 0x4d40d0
    virtual void  vfunc_17();                                       // [17] 0x405970
    virtual void  vfunc_18();                                       // [18] 0x405a00
    virtual void  vfunc_19();                                       // [19] 0x405a30
    virtual void  vfunc_20();                                       // [20] 0x405a60
    virtual void  vfunc_21();                                       // [21] 0x405a70
    virtual void  vfunc_22();                                       // [22] 0x405a80
    virtual void  vfunc_23();                                       // [23] 0x405ab0
    virtual int   GetType_2();                                      // [24] 0x406010
    virtual long  Read_INI_2(void* ini, const char* section);       // [25] 0x4d3980  (INI reader)
    virtual int   Fetch_ID_2();                                     // [26] 0x405fb0  (refs "Name")
    virtual int   Get_RTTI_2();                                     // [28] 0x4d3ca0
    virtual void  vfunc_29();                                       // [29] 0x61f470
    virtual void  vfunc_30();                                       // [30] 0x620f30
    virtual void  vfunc_31();                                       // [31] 0x5715f0
    virtual void  vfunc_32();                                       // [32] 0x4d3c50
    virtual void  vfunc_33();                                       // [33] 0x4d34b0
    virtual void  vfunc_34();                                       // [34] 0x61f4b0
    virtual void  vfunc_35();                                       // [35] 0x61f490
    virtual void  vfunc_36();                                       // [36] 0x4d3480
    virtual void  vfunc_37();                                       // [37] 0x4d3590
    virtual void  vfunc_38();                                       // [38] 0x571710
    virtual void  vfunc_39();                                       // [39] 0x61f4f0
    virtual void  vfunc_40();                                       // [40] 0x40f620
    virtual void  vfunc_41();                                       // [41] 0x620df0
    virtual void  vfunc_42();                                       // [42] 0x61f460
    virtual void  vfunc_43();                                       // [43] 0x4d3c90
    virtual void  vfunc_44();                                       // [44] 0x4d3c80
    virtual void  vfunc_45();                                       // [45] 0x622180
    virtual void  vfunc_46();                                       // [46] 0x4d7dc0
    virtual void  vfunc_47();                                       // [47] 0x4d7ed0
    virtual void  vfunc_48();                                       // [48] 0x4d7f80
    virtual void  vfunc_49();                                       // [49] 0x4d7fa0
    virtual void  vfunc_50();                                       // [50] 0x4d7ff0
    virtual void  vfunc_51();                                       // [51] 0x5f1020
    virtual void  vfunc_52();                                       // [52] 0x46dfa0
    virtual void  vfunc_53();                                       // [53] 0x4d8020
    virtual void  vfunc_54();                                       // [54] 0x5f1020
    virtual void  vfunc_55();                                       // [55] 0x41e140
    virtual void  vfunc_56();                                       // [56] 0x4d8060
    virtual void  vfunc_57();                                       // [57] 0x41e0f0
    virtual void  vfunc_58();                                       // [58] 0x44ab70
    virtual void  vfunc_59();                                       // [59] 0x591f40
    virtual void  vfunc_60();                                       // [60] 0x41df60

    // ---- data members -----------------------------------------------------
    // Evidence: constructor 0x4d3200 writes these offsets; INI reader 0x4d3980
    // references the named keys (ini_keys.json, reader VA 0x5060992 = 0x4d3980).
    int   m_Field470;           // +0x470 ctor: mov dword ptr [esi+0x470], 0xffffffff
    int   m_Field474;           // +0x474 ctor: mov dword ptr [esi+0x474], 1
    int   m_Field478;           // +0x478 ctor: mov dword ptr [esi+0x478], ebx (0)
    int   m_Field47c;           // +0x47c ctor: mov dword ptr [esi+0x47c], ebx (0)
    int   m_Field480;           // +0x480 ctor: mov dword ptr [esi+0x480], ebx (0)
    int   m_Field484;           // +0x484 ctor: mov dword ptr [esi+0x484], 0x69ad58
    int   m_Field488;           // +0x488 ctor: mov dword ptr [esi+0x488], ebx (0)
    int   m_Field48c;           // +0x48c ctor: mov dword ptr [esi+0x48c], ebx (0)
    bool  m_Field490;           // +0x490 ctor: mov byte ptr [esi+0x490], 1
    bool  m_Field491;           // +0x491 ctor: mov byte ptr [esi+0x491], bl (0)
    int   m_Field494;           // +0x494 ctor: mov dword ptr [esi+0x494], ebx (0)
    int   m_Field498;           // +0x498 ctor: mov dword ptr [esi+0x498], 0xa
    bool  m_Field4a0;           // +0x4a0 ctor: mov byte ptr [esi+0x4a0], bl (0)
    bool  m_Field4a1;           // +0x4a1 ctor: mov byte ptr [esi+0x4a1], bl (0)
    bool  m_Field4a2;           // +0x4a2 ctor: mov byte ptr [esi+0x4a2], 1
    bool  m_Field4a3;           // +0x4a3 ctor: mov byte ptr [esi+0x4a3], bl (0)
    bool  m_Field4a4;           // +0x4a4 ctor: mov byte ptr [esi+0x4a4], bl (0)
    bool  m_Field4a5;           // +0x4a5 ctor: mov byte ptr [esi+0x4a5], bl (0)
    bool  m_Field4a6;           // +0x4a6 ctor: mov byte ptr [esi+0x4a6], bl (0)
    bool  m_Field4a7;           // +0x4a7 ctor: mov byte ptr [esi+0x4a7], bl (0)
    bool  m_Field4a8;           // +0x4a8 ctor: mov byte ptr [esi+0x4a8], bl (0)
    bool  m_Field4a9;           // +0x4a9 ctor: mov byte ptr [esi+0x4a9], bl (0)
    bool  m_Field4aa;           // +0x4aa ctor: mov byte ptr [esi+0x4aa], bl (0)
    bool  m_Field4ab;           // +0x4ab ctor: mov byte ptr [esi+0x4ab], bl (0)
    bool  m_Field4ac;           // +0x4ac ctor: mov byte ptr [esi+0x4ac], bl (0)
    bool  m_Field4ad;           // +0x4ad ctor: mov byte ptr [esi+0x4ad], bl (0)
    bool  m_Field4ae;           // +0x4ae ctor: mov byte ptr [esi+0x4ae], bl (0)

    // ---- INI keys read by 0x4d3980 ----------------------------------------
    // Original rules.ini field names; exact offsets within +0x470..+0x4ae not
    // yet individually pinned.  /* ? */
    //   "Agent", "Civilian", "Crawls", "Cyborg", "Disguised", "Doggie",
    //   "Engineer", "Fearless", "FireProne", "FireUp", "Fraidycat",
    //   "Infiltrate", "JumpJet", "Thief", "TiberiumProof", "VehicleThief",
    //   "VoiceComment"

    // ---- data members (offsets from ctor 0x004d3200) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    // 42 member(s) already declared above; their offsets are recorded in out/members.json

};
