#pragma once
// ============================================================================
// AircraftTypeClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// Evidence:
//   RTTI class descriptor  : 0x6a9880
//   TypeDescriptor name    : 0x6bfa08 + 8 -> ".?AVAircraftTypeClass@@"
//   vtable (primary)       : 0x6a9890 is the COL ptr; primary vtable 0x699168
//   vtable (IRTTITypeInfo) : 0x69914c   (COL ptr slot 0x6a97f0, mdisp=4)
//   constructor            : 0x40f4a0   (writes vtbl 0x699168 / 0x69914c)
//   INI reader             : 0x40f300   (vtable slot 25)
// ============================================================================

#include "TechnoTypeClass.h"

class AircraftTypeClass : public TechnoTypeClass
{
public:
    // ---- vtable 0x699168 (primary, 45 entries) ----------------------------
    // Slot 6 is the RTTI CompleteObjectLocator pointer (0x6a9890), not code.
    virtual ~AircraftTypeClass();                                   // [0] 0x405790
    virtual void  Init();                                           // [1] 0x405830
    virtual void  Shutdown();                                       // [2] 0x405840
    virtual long  QueryInterface(const void*, void**);              // [3] 0x40f460
    virtual unsigned long AddRef();                                 // [4] 0x405990
    virtual long  Read_INI(void* ini, const char* section);         // [5] 0x40f3f0
    virtual void  vfunc_6();                                        // [6] 0x40f440
    virtual void  vfunc_7();                                        // [7] 0x621bf0
    virtual void  vfunc_8();                                        // [8] 0x40f660
    virtual void  Set_ID(int id);                                   // [9] 0x4059b0
    virtual int   Get_RTTI();                                       // [10] 0x4059c0
    virtual long  GetClassID(void* pClassID);                       // [11] 0x40f630
    virtual long  IsDirty();                                        // [12] 0x40f640
    virtual long  Load(IStream* pStm);                              // [13] 0x40f3a0
    virtual long  Save(IStream* pStm, int fClearDirty);             // [14] 0x4059d0
    virtual long  GetSizeMax(unsigned long* pcbSize);               // [15] 0x4059e0
    virtual void  Compute_CRC();                                    // [16] 0x40f650
    virtual void  vfunc_17();                                       // [17] 0x405970
    virtual void  vfunc_18();                                       // [18] 0x405a00
    virtual void  vfunc_19();                                       // [19] 0x405a30
    virtual void  vfunc_20();                                       // [20] 0x405a60
    virtual void  vfunc_21();                                       // [21] 0x405a70
    virtual void  vfunc_22();                                       // [22] 0x405a80
    virtual void  vfunc_23();                                       // [23] 0x405ab0
    virtual int   GetType_2();                                      // [24] 0x406010
    virtual long  Read_INI_2(void* ini, const char* section);       // [25] 0x40f300  (INI reader)
    virtual int   Fetch_ID_2();                                     // [26] 0x405fb0  (refs "Name")
    virtual int   Get_RTTI_2();                                     // [28] 0x40f600
    virtual void  vfunc_29();                                       // [29] 0x61f470
    virtual void  vfunc_30();                                       // [30] 0x620f30
    virtual void  vfunc_31();                                       // [31] 0x5715f0
    virtual void  vfunc_32();                                       // [32] 0x40f2d0
    virtual void  vfunc_33();                                       // [33] 0x40f2c0
    virtual void  vfunc_34();                                       // [34] 0x61f4b0
    virtual void  vfunc_35();                                       // [35] 0x61f490
    virtual void  vfunc_36();                                       // [36] 0x40f220
    virtual void  vfunc_37();                                       // [37] 0x40f250
    virtual void  vfunc_38();                                       // [38] 0x571710
    virtual void  vfunc_39();                                       // [39] 0x61f4f0
    virtual void  vfunc_40();                                       // [40] 0x40f620
    virtual void  vfunc_41();                                       // [41] 0x620df0
    virtual void  vfunc_42();                                       // [42] 0x61f460
    virtual void  vfunc_43();                                       // [43] 0x61f500
    virtual void  vfunc_44();                                       // [44] 0x622180

    // ---- data members -----------------------------------------------------
    // Evidence: constructor 0x40f4a0 writes these offsets; INI reader 0x40f300
    // references the named keys (ini_keys.json, reader VA 0x4256512 = 0x40f300).
    int   m_Field470;           // +0x470 ctor: mov dword ptr [esi+0x470], 0xffffffff
    bool  m_Landable;           // +0x474 ctor: mov byte ptr [esi+0x474], bl (0)  INI "Landable"
    bool  m_Carryall;           // +0x475 ctor: mov byte ptr [esi+0x475], bl (0)  INI "Carryall"
    bool  m_Rotors;             // +0x476 ctor: mov byte ptr [esi+0x476], bl (0)  INI "Rotors"
    bool  m_CustomRotor;        // +0x477 ctor: mov byte ptr [esi+0x477], bl (0)  INI "CustomRotor"

    // ---- data members (offsets from ctor 0x0040f040) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    // 11 member(s) already declared above; their offsets are recorded in out/members.json

};
