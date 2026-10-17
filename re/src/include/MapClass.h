#pragma once
// ============================================================================
// MapClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// Evidence:
//   RTTI class descriptor  : 0x6b2228  (CompleteObjectLocator for MapClass)
//   TypeDescriptor name    : 0x6be690 + 8 -> ".?AVMapClass@@"
//   vtable (primary)       : 0x6a19f0   (COL ptr slot 0x6a19ec)
//   constructor            : 0x501b90   (writes vtbl 0x6a19f0; full member init)
//   ctor (from stream)     : 0x501a40   (writes vtbl 0x69f89c)
//   destructor             : 0x4b2de0   (vtable slot 0)
//
// MapClass derives from GScreenClass (RTTI base list: GScreenClass mdisp 0,
// IGameMap mdisp 0, IUnknown mdisp 0).  It owns the isometric cell grid and
// the map-wide object lists.  DisplayClass, RadarClass, SidebarClass and
// PowerClass all derive from it (see hierarchy.txt lines 373-376, 1189-1193,
// 1289-1293, 1572-1576).
// ============================================================================

#include "AbstractClass.h"
#include "ts_prelude.h"   // gadget/UI bases (GScreenClass, GadgetClass, ...)

class GScreenClass;   // base, defined in GScreenClass.h (outside this task)

class MapClass : public GScreenClass
{
public:
    // ---- vtable 0x6a19f0 (primary, 127 entries) ---------------------------
    // Slot 29/36/43/50/57/64/71/78/85/92/99/106/113/120 are RTTI
    // CompleteObjectLocator pointers (0x6b2190, 0x6b21f8, 0x6b22a0, ...),
    // not code.  The remaining slots are listed below in vtable order.
    virtual ~MapClass();                                            // [0] 0x4b2de0
    virtual void  Init();                                           // [1] 0x402ad0
    virtual void  Shutdown();                                       // [2] 0x402ae0
    virtual long  QueryInterface(const void*, void**);              // [3] 0x502000
    virtual unsigned long AddRef();                                 // [4] 0x520540
    virtual void  vfunc_5();                                        // [5] 0x502130
    virtual void  vfunc_6();                                        // [6] 0x4b2e50
    virtual void  vfunc_7();                                        // [7] 0x502320
    virtual void  vfunc_8();                                        // [8] 0x4b2e80
    virtual void  vfunc_9();                                        // [9] 0x4b2ec0
    virtual void  vfunc_10();                                       // [10] 0x4b34c0
    virtual void  vfunc_11();                                       // [11] 0x4b2f90
    virtual void  vfunc_12();                                       // [12] 0x4b2fd0
    virtual void  vfunc_13();                                       // [13] 0x4b2e90
    virtual void  vfunc_14();                                       // [14] 0x4b2ff0
    virtual void  vfunc_15();                                       // [15] 0x478900
    virtual void  vfunc_16();                                       // [16] 0x4b3100
    virtual void  vfunc_17();                                       // [17] 0x6829a5 (stub)
    virtual void  vfunc_18();                                       // [18] 0x6829a5 (stub)
    virtual void  vfunc_19();                                       // [19] 0x6829a5 (stub)
    virtual void  vfunc_20();                                       // [20] 0x6829a5 (stub)
    virtual void  vfunc_21();                                       // [21] 0x5023e0
    virtual void  vfunc_22();                                       // [22] 0x502440
    virtual void  vfunc_23();                                       // [23] 0x502500
    virtual void  vfunc_24();                                       // [24] 0x510cc0
    virtual void  vfunc_25();                                       // [25] 0x478910
    virtual void  vfunc_26();                                       // [26] 0x507150
    virtual void  vfunc_27();                                       // [27] 0x502550
    virtual void  vfunc_28();                                       // [28] 0x503870
    virtual void  vfunc_30();                                       // [30] 0x520870
    virtual void  vfunc_31();                                       // [31] 0x521870
    virtual void  vfunc_32();                                       // [32] 0x5218b0
    virtual void  vfunc_33();                                       // [33] 0x521960
    virtual void  vfunc_34();                                       // [34] 0x521990
    virtual void  vfunc_35();                                       // [35] 0x5219d0
    virtual void  vfunc_37();                                       // [37] 0x522210
    virtual void  vfunc_38();                                       // [38] 0x5214f0
    virtual void  vfunc_39();                                       // [39] 0x521700
    virtual void  vfunc_40();                                       // [40] 0x520720
    virtual void  vfunc_41();                                       // [41] 0x521820
    virtual void  vfunc_42();                                       // [42] 0x520750
    virtual void  vfunc_44();                                       // [44] 0x522140
    virtual void  vfunc_45();                                       // [45] 0x520eb0
    virtual void  vfunc_46();                                       // [46] 0x521220
    virtual void  vfunc_47();                                       // [47] 0x520670
    virtual void  vfunc_48();                                       // [48] 0x5213d0
    virtual void  vfunc_49();                                       // [49] 0x5206f0
    virtual void  vfunc_51();                                       // [51] 0x521fb0
    virtual void  vfunc_52();                                       // [52] 0x520ac0
    virtual void  vfunc_53();                                       // [53] 0x520d10
    virtual void  vfunc_54();                                       // [54] 0x520620
    virtual void  vfunc_55();                                       // [55] 0x520e60
    virtual void  vfunc_56();                                       // [56] 0x520650
    virtual void  vfunc_58();                                       // [58] 0x521f60
    virtual void  vfunc_59();                                       // [59] 0x520ac0
    virtual void  vfunc_60();                                       // [60] 0x520b30
    virtual void  vfunc_61();                                       // [61] 0x520c60
    virtual void  vfunc_62();                                       // [62] 0x520c90
    virtual void  vfunc_63();                                       // [63] 0x520cf0
    virtual void  vfunc_65();                                       // [65] 0x522000
    virtual void  vfunc_66();                                       // [66] 0x520eb0
    virtual void  vfunc_67();                                       // [67] 0x520f00
    virtual void  vfunc_68();                                       // [68] 0x521120
    virtual void  vfunc_69();                                       // [69] 0x5211a0
    virtual void  vfunc_70();                                       // [70] 0x5211f0
    virtual void  vfunc_72();                                       // [72] 0x5221c0
    virtual void  vfunc_73();                                       // [73] 0x5214f0
    virtual void  vfunc_74();                                       // [74] 0x521550
    virtual void  vfunc_75();                                       // [75] 0x521650
    virtual void  vfunc_76();                                       // [76] 0x521680
    virtual void  vfunc_77();                                       // [77] 0x5216e0
    virtual void  vfunc_79();                                       // [79] 0x522470
    virtual void  vfunc_80();                                       // [80] 0x521da0
    virtual void  vfunc_81();                                       // [81] 0x521de0
    virtual void  vfunc_82();                                       // [82] 0x520810
    virtual void  vfunc_83();                                       // [83] 0x521ee0
    virtual void  vfunc_84();                                       // [84] 0x520840
    virtual void  vfunc_86();                                       // [86] 0x522260
    virtual void  vfunc_87();                                       // [87] 0x521c50
    virtual void  vfunc_88();                                       // [88] 0x521c90
    virtual void  vfunc_89();                                       // [89] 0x5207c0
    virtual void  vfunc_90();                                       // [90] 0x521d70
    virtual void  vfunc_91();                                       // [91] 0x5207f0
    virtual void  vfunc_93();                                       // [93] 0x5226c0
    virtual void  vfunc_94();                                       // [94] 0x5219f0
    virtual void  vfunc_95();                                       // [95] 0x521a30
    virtual void  vfunc_96();                                       // [96] 0x521ae0
    virtual void  vfunc_97();                                       // [97] 0x521b10
    virtual void  vfunc_98();                                       // [98] 0x521b50
    virtual void  vfunc_100();                                      // [100] 0x522710
    virtual void  vfunc_101();                                      // [101] 0x5219f0
    virtual void  vfunc_102();                                      // [102] 0x521b70
    virtual void  vfunc_103();                                      // [103] 0x520770
    virtual void  vfunc_104();                                      // [104] 0x521c20
    virtual void  vfunc_105();                                      // [105] 0x5207a0
    virtual void  vfunc_107();                                      // [107] 0x521f10
    virtual void  vfunc_108();                                      // [108] 0x4013d0
    virtual void  vfunc_109();                                      // [109] 0x520950
    virtual void  vfunc_110();                                      // [110] 0x5205d0
    virtual void  vfunc_111();                                      // [111] 0x520a90
    virtual void  vfunc_112();                                      // [112] 0x520600
    virtual void  vfunc_114();                                      // [114] 0x522760
    virtual void  vfunc_115();                                      // [115] 0x521c50
    virtual void  vfunc_116();                                      // [116] 0x522310
    virtual void  vfunc_117();                                      // [117] 0x5223e0
    virtual void  vfunc_118();                                      // [118] 0x522410
    virtual void  vfunc_119();                                      // [119] 0x522450
    virtual void  vfunc_121();                                      // [121] 0x5227b0
    virtual void  vfunc_122();                                      // [122] 0x521da0
    virtual void  vfunc_123();                                      // [123] 0x522520
    virtual void  vfunc_124();                                      // [124] 0x522620
    virtual void  vfunc_125();                                      // [125] 0x522650
    virtual void  vfunc_126();                                      // [126] 0x522690

    // ---- data members -----------------------------------------------------
    // Offsets recovered from constructor 0x501b90.  The constructor also
    // installs three embedded sub-object vtables, which identify the member
    // types:
    //   +0x40  vftable 0x6a1ad8  -> DynamicVectorClass<struct CrackedIceStruct,
    //                              MapClass> (vtable 0x6a1a84)
    //   +0x14c vftable 0x6a1b10  -> VectorClass<struct CrackedIceStruct,
    //                              MapClass> (vtable 0x6a1b10)
    //   +0x164 vftable 0x6a1a68  -> VectorClass<CellClass*> (vtable 0x6a1a68)
    //
    // +0x00  vftable (0x6a19f0)
    // +0x04  GScreenClass / IGameMap base members (see GScreenClass.h)
    // +0x10  int   m_Width /* ? */            (0x501b90: [esi+0x10]=0)
    // +0x40  DynamicVectorClass<CrackedIceStruct,MapClass> m_CrackedIce /* ? */
    // +0x44  int   m_CrackedIceCount /* ? */  (0x501b90: [esi+0x44]=0)
    // +0x48  int   m_CrackedIceCapacity /* ? */(0x501b90: [esi+0x48]=0)
    // +0x58  int   m_Height /* ? */           (0x501b90: [esi+0x58]=0)
    // +0x60  int   m_CellCount /* ? */        (0x501b90: [esi+0x60]=0)
    // +0xc4  int   m_CellGrid /* ? */         (0x501b90: [esi+0xc4]=edi)
    // +0xc8  int   m_CellGridCount /* ? */    (0x501b90: [esi+0xc8]=0)
    // +0xcc  int   m_CellGridCapacity /* ? */ (0x501b90: [esi+0xcc]=0)
    // +0xdc  int   m_ShroudGrid /* ? */       (0x501b90: [esi+0xdc]=edi)
    // +0xe0  int   m_ShroudGridCount /* ? */  (0x501b90: [esi+0xe0]=0)
    // +0xe4  int   m_ShroudGridCapacity /* ?*/(0x501b90: [esi+0xe4]=0)
    // +0x14c VectorClass<CrackedIceStruct,MapClass> m_CrackedIceVec /* ? */
    // +0x150 int   m_CrackedIceVecCount /* ? */(0x501b90: [esi+0x150]=0)
    // +0x154 int   m_CrackedIceVecCap /* ? */ (0x501b90: [esi+0x154]=0)
    // +0x164 VectorClass<CellClass*> m_Cells /* ? */  (vtable 0x6a1a68)
    // +0x168 int   m_CellsCount /* ? */       (0x501b90: [esi+0x168]=0)
    // +0x16c int   m_CellsCapacity /* ? */    (0x501b90: [esi+0x16c]=0)
    // +0x1184 int  m_OverlayGrid /* ? */      (0x501b90: [esi+0x1184]=edi)
    // +0x1188 int  m_OverlayGridCount /* ? */ (0x501b90: [esi+0x1188]=0)
    // +0x118c int  m_OverlayGridCap /* ? */   (0x501b90: [esi+0x118c]=0)
    //
    // The stream ctor 0x501a40 additionally initialises +0x1194 and +0x1198
    // and installs vtable 0x699dc8 at +0x1184 (a different vector type).

    // ---- data members (offsets from ctor 0x00501930) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    // vtable-pointer stores are omitted: the compiler supplies the vptr.
    unsigned char pad_004[0x160];
    unsigned int m_168;  // +0x168 /* ? */; 4-byte store: mov dword ptr [esi + 0x168], ebx
    unsigned int m_16c;  // +0x16c /* ? */; 4-byte store: mov dword ptr [esi + 0x16c], ebx
    unsigned char m_170;  // +0x170 init 0x1; 1-byte store: mov byte ptr [esi + 0x170], 1
    unsigned char m_171;  // +0x171 /* ? */; 1-byte store: mov byte ptr [esi + 0x171], bl

};
