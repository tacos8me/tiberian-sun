#pragma once
// ============================================================================
// BulletClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// Evidence:
//   RTTI class descriptor  : 0x6aa580  (CompleteObjectLocator for BulletClass)
//   TypeDescriptor name    : 0x6c13e8 + 8 -> ".?AVBulletClass@@"
//   vtable (primary)       : 0x69a84c   (COL ptr slot 0x69a848)
//   vtable (IRTTITypeInfo) : 0x69a830   (COL ptr slot 0x69a82c, mdisp=4)
//   constructor            : 0x444e40   (writes vtbl 0x69a84c)
//   ctor (from stream)     : 0x445200   (writes vtbl 0x69a84c)
//   destructor             : 0x405790   (vtable slot 0)
//
// BulletClass derives from ObjectClass (RTTI base list: ObjectClass mdisp 0,
// AbstractClass mdisp 0, IPersistStream/IPersist/IUnknown mdisp 0,
// IRTTITypeInfo/IUnknown mdisp 4).  It is a projectile in flight.
//
// INI keys read by the projectile type reader (out/ini_keys.json):
//   "Projectile" -> 0x6571be (weapon type reader).
//   "AmbientDamage", "IsSonic", "Supress", "Burst", "Damage", "Speed",
//   "Range", "MinimumRange" -> 0x656d40 (weapon type reader).
// ============================================================================

#include "ObjectClass.h"
#include "ts_prelude.h"

class BulletClass : public ObjectClass
{
public:
    // ---- vtable 0x69a84c (primary, 108 entries) ---------------------------
    // Slot 6 is the RTTI CompleteObjectLocator pointer (0x69a848), not code.
    virtual ~BulletClass();                                         // [0] 0x405790
    virtual void  Init();                                           // [1] 0x444f00
    virtual void  Shutdown();                                       // [2] 0x444f20
    virtual long  QueryInterface(const void*, void**);              // [3] 0x4451a0
    virtual unsigned long AddRef();                                 // [4] 0x405990
    virtual long  Load(IStream* pStm);                              // [5] 0x444e40
    virtual long  Save(IStream* pStm, int fClearDirty);             // [6] 0x444ee0
    virtual int   GetType();                                        // [7] 0x405910
    virtual long  GetClassID(void* pClassID);                       // [8] 0x445200
    virtual void  Set_ID(int id);                                   // [9] 0x4059b0
    virtual int   Get_RTTI();                                       // [10] 0x444010
    virtual long  IsDirty();                                        // [11] 0x445190
    virtual long  GetSizeMax(unsigned long* pcbSize);               // [12] 0x445180
    virtual void  Compute_CRC();                                    // [13] 0x570630
    virtual long  vfunc_14();                                       // [14] 0x4059d0
    virtual long  vfunc_15();                                       // [15] 0x4059e0
    virtual void  vfunc_16();                                       // [16] 0x4059f0
    virtual void  vfunc_17();                                       // [17] 0x570a20
    virtual void  vfunc_18();                                       // [18] 0x570960
    virtual void  vfunc_19();                                       // [19] 0x405a30
    virtual void  vfunc_20();                                       // [20] 0x570e80
    virtual void  vfunc_21();                                       // [21] 0x570ea0
    virtual void  vfunc_22();                                       // [22] 0x405a80
    virtual void  vfunc_23();                                       // [23] 0x4428f0
    virtual void  vfunc_24();                                       // [24] 0x4167b0
    virtual void  vfunc_25();                                       // [25] 0x4167c0
    virtual void  vfunc_26();                                       // [26] 0x56e820
    virtual void  vfunc_27();                                       // [27] 0x56eb50
    virtual void  vfunc_28();                                       // [28] 0x56eb40
    virtual void  vfunc_29();                                       // [29] 0x4443c0
    virtual void  vfunc_30();                                       // [30] 0x570ef0
    virtual void  vfunc_31();                                       // [31] 0x4167d0
    virtual void  vfunc_32();                                       // [32] 0x570ec0
    virtual void  vfunc_33();                                       // [33] 0x4451f0
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
    virtual void  vfunc_60();                                       // [60] 0x442870
    virtual void  vfunc_61();                                       // [61] 0x416840
    virtual void  vfunc_62();                                       // [62] 0x416850
    virtual void  vfunc_63();                                       // [63] 0x443d10
    virtual void  vfunc_64();                                       // [64] 0x570990
    virtual void  vfunc_65();                                       // [65] 0x56ec20
    virtual void  vfunc_66();                                       // [66] 0x56ec30
    virtual void  vfunc_67();                                       // [67] 0x4428d0
    virtual void  vfunc_68();                                       // [68] 0x56f000
    virtual void  vfunc_69();                                       // [69] 0x56f160
    virtual void  vfunc_70();                                       // [70] 0x40e690
    virtual void  vfunc_71();                                       // [71] 0x56f680
    virtual void  vfunc_72();                                       // [72] 0x56ec50
    virtual void  vfunc_73();                                       // [73] 0x56ec40
    virtual void  vfunc_74();                                       // [74] 0x56ec60
    virtual void  vfunc_75();                                       // [75] 0x56ee20
    virtual void  vfunc_76();                                       // [76] 0x56eda0
    virtual void  vfunc_77();                                       // [77] 0x56ec70
    virtual void  vfunc_78();                                       // [78] 0x56ec80
    virtual void  vfunc_79();                                       // [79] 0x56fbe0
    virtual void  vfunc_80();                                       // [80] 0x56ec90
    virtual void  vfunc_81();                                       // [81] 0x56eca0
    virtual void  vfunc_82();                                       // [82] 0x56ecb0
    virtual void  vfunc_83();                                       // [83] 0x56ecc0
    virtual void  vfunc_84();                                       // [84] 0x56ecd0
    virtual void  vfunc_85();                                       // [85] 0x416860
    virtual void  vfunc_86();                                       // [86] 0x416870
    virtual void  vfunc_87();                                       // [87] 0x570240
    virtual void  vfunc_88();                                       // [88] 0x56fbc0
    virtual void  vfunc_89();                                       // [89] 0x5700b0
    virtual void  vfunc_90();                                       // [90] 0x56ece0
    virtual void  vfunc_91();                                       // [91] 0x56ecf0
    virtual void  vfunc_92();                                       // [92] 0x570e70
    virtual void  vfunc_93();                                       // [93] 0x56ed00
    virtual void  vfunc_94();                                       // [94] 0x416880
    virtual void  vfunc_95();                                       // [95] 0x416890
    virtual void  vfunc_96();                                       // [96] 0x40e6a0
    virtual void  vfunc_97();                                       // [97] 0x570cc0
    virtual void  vfunc_98();                                       // [98] 0x40e6c0
    virtual void  vfunc_99();                                       // [99] 0x570ce0
    virtual void  vfunc_100();                                      // [100] 0x570d00
    virtual void  vfunc_101();                                      // [101] 0x570d50
    virtual void  vfunc_102();                                      // [102] 0x570370
    virtual void  vfunc_103();                                      // [103] 0x5703b0
    virtual void  vfunc_104();                                      // [104] 0x570360
    virtual void  vfunc_105();                                      // [105] 0x443c80
    virtual void  vfunc_106();                                      // [106] 0x4451e0
    virtual void  vfunc_107();                                      // [107] 0x444050

    // ---- data members -----------------------------------------------------
    // Offsets recovered from constructor 0x444e40 (which only writes the
    // vtable) and the stream ctor 0x445200 (which additionally clears +0x4c
    // and +0x50).  The ObjectClass base occupies +0x00..+0x4b.
    //
    // +0x00  vftable (0x69a84c)
    // +0x04  IRTTITypeInfo sub-object vftable (0x69a830)
    // +0x08  ObjectClass / AbstractClass base members (see ObjectClass.h)
    // +0x4c  int   m_Target /* ? */           (0x445200: [esi+0x4c]=0)
    // +0x50  int   m_TargetData /* ? */       (0x445200: [esi+0x50]=0)
    //
    // The remaining BulletClass members (velocity, damage, warhead, ...) are
    // not initialised by either constructor and are set by the firing code;
    // their offsets are not resolved here /* ? */.

    // ---- data members (offsets from ctor 0x004426a0) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    // 21 member(s) already declared above; their offsets are recorded in out/members.json

};
