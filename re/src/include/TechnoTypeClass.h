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
    virtual ~TechnoTypeClass();                                     // [0] 0x405790
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
    // 74 member(s) already declared above; their offsets are recorded in out/members.json

};
