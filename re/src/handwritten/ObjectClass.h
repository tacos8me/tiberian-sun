#pragma once
// ============================================================================
// ObjectClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// Evidence:
//   RTTI class descriptor  : 0x6b3388
//   TypeDescriptor name    : 0x6bf538 -> ".?AVObjectClass@@"
//   vtable (primary)       : 0x6b3398 is the COL ptr; primary vtable 0x6a2880
//   vtable (IRTTITypeInfo) : 0x6a2864   (COL ptr slot 0x6b3350, mdisp=4)
//   constructor            : 0x56e420   (writes vtbl 0x6a2880 / 0x6a2864)
//   copy-ctor              : 0x56e5f0
//   ctor (from stream)     : 0x56e610
//
// ObjectClass is the base for every instantiated game object (units,
// buildings, terrain, anims, bullets, ...).  It owns a coordinate, a
// health/strength value and a heap-allocated type pointer.
// ============================================================================

#include "AbstractClass.h"

class ObjectClass : public AbstractClass
{
public:
    // ---- vtable 0x6a2880 (primary, 105 entries) ---------------------------
    // Slot 6 is the RTTI CompleteObjectLocator pointer (0x6b3398), not code.
    virtual ~ObjectClass();                                         // [0] 0x405790
    virtual void  Init();                                           // [1] 0x405830
    virtual void  Shutdown();                                       // [2] 0x405840
    virtual long  QueryInterface(const void*, void**);              // [3] 0x6829a5 (stub)
    virtual unsigned long AddRef();                                 // [4] 0x405990
    virtual void  vfunc_5();                                        // [5] 0x5702f0
    virtual void  vfunc_6();                                        // [6] 0x6829a5 (stub)
    virtual int   GetType();                                        // [7] 0x405910
    virtual void  vfunc_8();                                        // [8] 0x570f10
    virtual void  Set_ID(int id);                                   // [9] 0x4059b0
    virtual void  vfunc_10();                                       // [10] 0x56fb00
    virtual long  GetClassID(void* pClassID);                       // [11] 0x6829a5 (stub)
    virtual long  IsDirty();                                        // [12] 0x6829a5 (stub)
    virtual long  Load(IStream* pStm);                              // [13] 0x570630
    virtual long  Save(IStream* pStm, int fClearDirty);             // [14] 0x4059d0
    virtual long  GetSizeMax(unsigned long* pcbSize);               // [15] 0x4059e0
    virtual void  Compute_CRC();                                    // [16] 0x4059f0
    virtual void  vfunc_17();                                       // [17] 0x570a20
    virtual void  vfunc_18();                                       // [18] 0x570960
    virtual void  vfunc_19();                                       // [19] 0x405a30
    virtual void  vfunc_20();                                       // [20] 0x570e80
    virtual void  vfunc_21();                                       // [21] 0x570ea0
    virtual void  vfunc_22();                                       // [22] 0x405a80
    virtual void  vfunc_23();                                       // [23] 0x56e860
    virtual void  vfunc_24();                                       // [24] 0x4167b0
    virtual void  vfunc_25();                                       // [25] 0x4167c0
    virtual void  vfunc_26();                                       // [26] 0x56e820
    virtual void  vfunc_27();                                       // [27] 0x56eb50
    virtual void  vfunc_28();                                       // [28] 0x56eb40
    virtual void  vfunc_29();                                       // [29] 0x56eb60
    virtual void  vfunc_30();                                       // [30] 0x570ef0
    virtual void  vfunc_31();                                       // [31] 0x4167d0
    virtual void  vfunc_32();                                       // [32] 0x570ec0
    virtual void  vfunc_33();                                       // [33] 0x4a4b00
    virtual void  vfunc_34();                                       // [34] 0x56eba0
    virtual void  vfunc_35();                                       // [35] 0x4167e0
    virtual void  vfunc_36();                                       // [36] 0x56ebb0
    virtual void  vfunc_37();                                       // [37] 0x56ebc0
    virtual void  vfunc_38();                                       // [38] 0x56ebd0
    virtual void  vfunc_39();                                       // [39] 0x56ebe0
    virtual void  vfunc_40();                                       // [40] 0x40e5d0
    virtual void  vfunc_41();                                       // [41] 0x40e600
    virtual void  vfunc_42();                                       // [42] 0x40e630
    virtual void  vfunc_43();                                       // [43] 0x4167f0
    virtual void  vfunc_44();                                       // [44] 0x40e660
    virtual void  vfunc_45();                                       // [45] 0x570ed0
    virtual void  vfunc_46();                                       // [46] 0x570db0
    virtual void  vfunc_47();                                       // [47] 0x416830
    virtual void  vfunc_48();                                       // [48] 0x56f6a0
    virtual void  vfunc_49();                                       // [49] 0x56f810
    virtual void  vfunc_50();                                       // [50] 0x56fb40
    virtual void  vfunc_51();                                       // [51] 0x56ebf0
    virtual void  vfunc_52();                                       // [52] 0x5700c0
    virtual void  vfunc_53();                                       // [53] 0x56ea70
    virtual void  vfunc_54();                                       // [54] 0x570480
    virtual void  vfunc_55();                                       // [55] 0x570500
    virtual void  vfunc_56();                                       // [56] 0x5709b0
    virtual void  vfunc_57();                                       // [57] 0x56ec00
    virtual void  vfunc_58();                                       // [58] 0x56ec10
    virtual void  vfunc_59();                                       // [59] 0x56f480
    virtual void  vfunc_60();                                       // [60] 0x5701b0
    virtual void  vfunc_61();                                       // [61] 0x416840
    virtual void  vfunc_62();                                       // [62] 0x416850
    virtual void  vfunc_63();                                       // [63] 0x547120
    // Slots 64..104 are further virtuals whose function records were not
    // recovered by the disassembler (targets 0x56ec20..0x570d50).  /* ? */

    // ---- data members -----------------------------------------------------
    // Evidence: constructor 0x56e420 writes these offsets.
    int   m_NextObject;         // +0x14  ctor: mov dword ptr [esi+0x14], ebx (0)
    int   m_PrevObject;         // +0x18  ctor: mov dword ptr [esi+0x18], ebx (0)
    int   m_Type;               // +0x1c  ctor: mov dword ptr [esi+0x1c], ebx (0)
    int   m_Health;             // +0x20  ctor: mov dword ptr [esi+0x20], ebx (0)
    int   m_Strength;           // +0x24  ctor: mov dword ptr [esi+0x24], ebx (0)
    int   m_Owner;              // +0x28  ctor: mov dword ptr [esi+0x28], 0xff
    bool  m_IsSelected;         // +0x2c  ctor: mov byte ptr [esi+0x2c], bl (0)
    bool  m_IsOnMap;            // +0x2d  ctor: mov byte ptr [esi+0x2d], bl (0)
    bool  m_IsActive;           // +0x2e  ctor: mov byte ptr [esi+0x2e], bl (0)
    bool  m_IsVisible;          // +0x2f  ctor: mov byte ptr [esi+0x2f], al (1)
    bool  m_IsSelected2;        // +0x30  ctor: mov byte ptr [esi+0x30], bl (0)
    bool  m_IsAnimating;        // +0x31  ctor: mov byte ptr [esi+0x31], bl (0)
    bool  m_IsDirty;            // +0x32  ctor: mov byte ptr [esi+0x32], bl (0)
    bool  m_IsCloaked;          // +0x33  ctor: mov byte ptr [esi+0x33], bl (0)
    bool  m_IsPowered;          // +0x34  ctor: mov byte ptr [esi+0x34], bl (0)
    bool  m_IsSelected3;        // +0x35  ctor: mov byte ptr [esi+0x35], al (1)
    int   m_ObjectID;           // +0x38  ctor: mov dword ptr [esi+0x38], 0xffffffff
    bool  m_IsDead;             // +0x3c  ctor: mov byte ptr [esi+0x3c], bl (0)
    // +0x40: 12-byte coordinate (3 x int) copied from global 0x7d69f0.
    int   m_CoordX;             // +0x40  ctor: mov dword ptr [esi+0x40], ecx
    int   m_CoordY;             // +0x44  ctor: mov dword ptr [eax+4], edx
    int   m_CoordZ;             // +0x48  ctor: mov dword ptr [eax+8], ecx

    // ---- data members (offsets from ctor 0x0056e420) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x6a2880
    unsigned long m_004;  // init 0x6a2864
    unsigned char pad_008[0xc];
    unsigned long m_014;  // /* ? */
    unsigned long m_018;  // /* ? */
    unsigned long m_01c;  // /* ? */
    unsigned long m_020;  // /* ? */
    unsigned long m_024;  // /* ? */
    unsigned long m_028;  // init 0xff
    unsigned long m_02c;  // /* ? */
    unsigned long m_030;  // /* ? */
    unsigned long m_034;  // /* ? */
    unsigned long m_038;  // init 0xffffffff
    unsigned long m_03c;  // /* ? */
    unsigned long m_040;  // /* ? */

};
