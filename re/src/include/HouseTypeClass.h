#pragma once
// ============================================================================
// HouseTypeClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// Evidence:
//   RTTI class descriptor  : 0x6b0228
//   TypeDescriptor name    : 0x6c9810 + 8 -> ".?AVHouseTypeClass@@"
//   vtable (primary)       : 0x6b0238 is the COL ptr; primary vtable 0x69fbe0
//   vtable (IRTTITypeInfo) : 0x69fbc4   (COL ptr slot 0x6b01d0, mdisp=4)
//   constructor            : 0x4c7290   (writes vtbl 0x69fbe0 / 0x69fbc4)
//   INI reader             : 0x4c74d0   (vtable slot 25)
// ============================================================================

#include "ObjectTypeClass.h"

class HouseTypeClass : public AbstractTypeClass
{
public:
    // ---- vtable 0x69fbe0 (primary, 27 entries) ----------------------------
    // Slot 6 is the RTTI CompleteObjectLocator pointer (0x6b0238), not code.
    virtual ~HouseTypeClass();                                      // [0] 0x4c79b0
    virtual void  Init();                                           // [1] 0x4c7b50
    virtual void  Shutdown();                                       // [2] 0x4c7b60
    virtual long  QueryInterface(const void*, void**);              // [3] 0x4c7a50
    virtual unsigned long AddRef();                                 // [4] 0x4c7890
    virtual long  Read_INI(void* ini, const char* section);         // [5] 0x4c78a0
    virtual void  vfunc_6();                                        // [6] 0x4c7920
    virtual void  vfunc_7();                                        // [7] 0x4c7980
    virtual void  vfunc_8();                                        // [8] 0x4c7b70
    virtual void  Set_ID(int id);                                   // [9] 0x4059b0
    virtual int   Get_RTTI();                                       // [10] 0x4059c0
    virtual long  GetClassID(void* pClassID);                       // [11] 0x4c7b20
    virtual long  IsDirty();                                        // [12] 0x4c7b30
    virtual long  Load(IStream* pStm);                              // [13] 0x4c7790
    virtual long  Save(IStream* pStm, int fClearDirty);             // [14] 0x4059d0
    virtual long  GetSizeMax(unsigned long* pcbSize);               // [15] 0x4059e0
    virtual void  Compute_CRC();                                    // [16] 0x4c7b40
    virtual void  vfunc_17();                                       // [17] 0x405970
    virtual void  vfunc_18();                                       // [18] 0x405a00
    virtual void  vfunc_19();                                       // [19] 0x405a30
    virtual void  vfunc_20();                                       // [20] 0x405a60
    virtual void  vfunc_21();                                       // [21] 0x405a70
    virtual void  vfunc_22();                                       // [22] 0x405a80
    virtual void  vfunc_23();                                       // [23] 0x405ab0
    virtual int   GetType_2();                                      // [24] 0x406010
    virtual long  Read_INI_2(void* ini, const char* section);       // [25] 0x4c74d0  (INI reader)
    virtual int   Fetch_ID_2();                                     // [26] 0x405fb0  (refs "Name")

    // ---- data members -----------------------------------------------------
    // Evidence: constructor 0x4c7290 writes these offsets; INI reader 0x4c74d0
    // references the named keys (ini_keys.json, reader VA 0x5010640 = 0x4c74d0).
    unsigned char pad_doc_m_Field60[0x32];  // holds the gap before m_Field60, which documents 0x60 (the compiler would put it at 0x2e)
    int   m_Field60;            // +0x60  ctor: mov dword ptr [esi+0x60], 0xffffffff
    int   m_Field64;            // +0x64  ctor: mov dword ptr [esi+0x64], 0xffffffff
    int   m_Field68;            // +0x68  ctor: mov dword ptr [esi+0x68], 0xffffffff
    int   m_Field6c;            // +0x6c  ctor: mov dword ptr [esi+0x6c], ebx (0)
    int   m_Field70;            // +0x70  ctor: mov dword ptr [esi+0x70], ebx (0)
    int   m_Field74;            // +0x74  ctor: mov dword ptr [esi+0x74], 0x3ff00000
    int   m_Field78;            // +0x78  ctor: mov dword ptr [esi+0x78], ebx (0)
    int   m_Field7c;            // +0x7c  ctor: mov dword ptr [esi+0x7c], 0x3ff00000
    int   m_Field80;            // +0x80  ctor: mov dword ptr [esi+0x80], ebx (0)
    int   m_Field84;            // +0x84  ctor: mov dword ptr [esi+0x84], 0x3ff00000
    int   m_Field88;            // +0x88  ctor: mov dword ptr [esi+0x88], ebx (0)
    int   m_Field8c;            // +0x8c  ctor: mov dword ptr [esi+0x8c], 0x3ff00000
    int   m_Field90;            // +0x90  ctor: mov dword ptr [esi+0x90], ebx (0)
    int   m_Field94;            // +0x94  ctor: mov dword ptr [esi+0x94], 0x3ff00000
    int   m_Field98;            // +0x98  ctor: mov dword ptr [esi+0x98], ebx (0)
    int   m_Field9c;            // +0x9c  ctor: mov dword ptr [esi+0x9c], 0x3ff00000
    int   m_Fielda0;            // +0xa0  ctor: mov dword ptr [esi+0xa0], ebx (0)
    int   m_Fielda4;            // +0xa4  ctor: mov dword ptr [esi+0xa4], 0x3ff00000
    bool  m_Fielda8;            // +0xa8  ctor: mov byte ptr [esi+0xa8], bl (0)
    unsigned char pad_doc_m_Fieldac[0x3];  // holds the gap before m_Fieldac, which documents 0xac (the compiler would put it at 0xa9)
    char  m_Fieldac;            // +0xac  ctor: mov byte ptr [esi+0xac], 0x41 ('A')
    bool  m_Fieldad;            // +0xad  ctor: mov byte ptr [esi+0xad], bl (0)
    bool  m_Fieldae;            // +0xae  ctor: mov byte ptr [esi+0xae], bl (0)
    bool  m_Fieldaf;            // +0xaf  ctor: mov byte ptr [esi+0xaf], 1
    bool  m_Fieldb0;            // +0xb0  ctor: mov byte ptr [esi+0xb0], bl (0)

    // ---- INI keys read by 0x4c74d0 ----------------------------------------
    // Original rules.ini field names; exact offsets within +0x60..+0xb0 not
    // yet individually pinned.  /* ? */
    //   "Airspeed", "Armor", "BuildTime", "Color", "Cost", "Firepower",
    //   "Groundspeed", "Multiplay", "MultiplayPassive", "Prefix", "Side",
    //   "SmartAI", "Suffix", "WallOwner"

    // ---- data members (offsets from ctor 0x004c7290) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    // vtable-pointer stores are omitted: the compiler supplies the vptr.
    // The machine block begins at +0xb1: the compiled end of the
    // base sub-object region (+0x2e) and of the curated block
    // above. Base evidence: AbstractTypeClass at +0x0 (ends +0x2e).
    // 1 recovered offset(s) below +0x2e lie inside the base
    // sub-object region and are NOT declared here: they are declared and
    // compiler-checked in the base's own header -- +0x0
    // 24 recovered offset(s) are inside the curated hand-written block
    // above (its span is +0x2e..+0xb1) and are NOT repeated here:
    // the curated block wins -- +0x60, +0x64, +0x68, +0x6c, +0x70, +0x74, +0x78, +0x7c ...

};
