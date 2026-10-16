#pragma once
// ============================================================================
// RadarClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// Evidence:
//   RTTI class descriptor  : 0x6b3a78  (CompleteObjectLocator for RadarClass)
//   TypeDescriptor name    : 0x6be6c8 + 8 -> ".?AVRadarClass@@"
//   vtable (primary)       : 0x6a35fc   (COL ptr slot 0x6a35f8)
//   constructor            : 0x5a4e20   (writes vtbl 0x6a35fc; full member init)
//   ctor (from stream)     : 0x5a0980   (writes vtbl 0x6a35fc)
//   ctor (SidebarClass)    : 0x5d8940   (writes vtbl 0x6a35fc)
//   destructor             : 0x4b2de0   (vtable slot 0, inherited from MapClass)
//
// RadarClass derives from DisplayClass (RTTI base list: DisplayClass mdisp 0,
// MapClass mdisp 0, GScreenClass mdisp 0, IGameMap mdisp 0, IUnknown mdisp 0).
// It renders the minimap and owns the RTacticalClass gadget (nested class
// RadarClass::RTacticalClass, RTTI COL 0x6b3a30, vtables 0x6a357c and
// 0x704a04).  SidebarClass and PowerClass derive from it (hierarchy.txt lines
// 1289-1293, 1572-1576).
//
// INI keys read by the radar/event code (out/ini_keys.json):
//   "RadarEventColorSpeed", "RadarEventDurations", "RadarEventMinRadius",
//   "RadarEventRotationSpeed", "RadarEventSpeed",
//   "RadarEventSuppressionDistances", "RadarEventVisibilityDurations",
//   "RadarCombatFlashTime"  -> all referenced by 0x5b3451 (rules reader).
//   "FreeRadar" -> 0x5c6e8e / 0x5c7410 (scenario reader).
//   "BuildRadar" -> 0x5b4f7b.
// ============================================================================

#include "DisplayClass.h"
#include "ts_prelude.h"   // gadget/UI bases (GScreenClass, GadgetClass, ...)

class RadarClass : public DisplayClass
{
public:
    // ---- nested gadget class ---------------------------------------------
    // RTTI COL 0x6b3a30, TypeDescriptor ".?AVRTacticalClass@RadarClass@@".
    // Two vtables exist: 0x6a357c (83 entries, the live one) and 0x704a04
    // (54 entries, in .data — a secondary/alternate dispatch table).
    // Derives from GadgetClass (mdisp 0) and LinkClass (mdisp 0).
    class RTacticalClass : public GadgetClass
    {
    public:
        virtual ~RTacticalClass();                                  // [0] 0x5a4e00
        virtual void  vfunc_1();                                    // [1] 0x4a5b00
        virtual void  vfunc_2();                                    // [2] 0x4a5b10
        virtual void  vfunc_3();                                    // [3] 0x4f69e0
        virtual void  vfunc_4();                                    // [4] 0x4f6a20
        virtual void  vfunc_5();                                    // [5] 0x4f6a00
        virtual void  vfunc_6();                                    // [6] 0x4f6980
        virtual void  vfunc_7();                                    // [7] 0x4f69b0
        virtual void  vfunc_8();                                    // [8] 0x4f6930
        virtual void  vfunc_9();                                    // [9] 0x4a5ae0
        virtual void  vfunc_10();                                   // [10] 0x4a5be0
        virtual void  vfunc_11();                                   // [11] 0x4a5bb0
        virtual void  vfunc_12();                                   // [12] 0x4a5b20
        virtual void  vfunc_13();                                   // [13] 0x4a5de0
        virtual void  vfunc_14();                                   // [14] 0x45afe0
        virtual void  vfunc_15();                                   // [15] 0x4a5ad0
        virtual void  vfunc_16();                                   // [16] 0x4a5ac0
        virtual void  vfunc_17();                                   // [17] 0x4788d0
        virtual void  vfunc_18();                                   // [18] 0x4a5e20
        virtual void  vfunc_19();                                   // [19] 0x460650
        virtual void  vfunc_20();                                   // [20] 0x4a5e60
        virtual void  vfunc_21();                                   // [21] 0x4a5e90
        virtual void  vfunc_22();                                   // [22] 0x4a5eb0
        virtual void  vfunc_23();                                   // [23] 0x4a5ec0
        virtual void  vfunc_24();                                   // [24] 0x45aff0
        virtual void  vfunc_25();                                   // [25] 0x4a5ee0
        virtual void  vfunc_26();                                   // [26] 0x4a5f00
        virtual void  vfunc_27();                                   // [27] 0x4a5b90
        virtual void  vfunc_28();                                   // [28] 0x4a5e30
        virtual void  vfunc_29();                                   // [29] 0x5a1050
        virtual void  vfunc_30();                                   // [30] 0x4a5a60
        // [31] 0x6b3a88 is the RTTI COL pointer, not code.
        // Slots 32..82 re-dispatch the MapClass/DisplayClass/RadarClass
        // virtuals (0x5a4e20, 0x5a0a50, 0x5a0b10, 0x5a0ff0, 0x5a0b90, ...).
    };

    // ---- vtable 0x6a35fc (primary, 51 entries) ----------------------------
    virtual ~RadarClass();                                          // [0] 0x4b2de0
    virtual void  Init();                                           // [1] 0x402ad0
    virtual void  Shutdown();                                       // [2] 0x402ae0
    virtual long  QueryInterface(const void*, void**);              // [3] 0x502000
    virtual unsigned long AddRef();                                 // [4] 0x5a4e20
    virtual void  vfunc_5();                                        // [5] 0x5a0a50
    virtual void  vfunc_6();                                        // [6] 0x4b2e50
    virtual void  vfunc_7();                                        // [7] 0x5a0b10
    virtual void  vfunc_8();                                        // [8] 0x472590
    virtual void  vfunc_9();                                        // [9] 0x4b2ec0
    virtual void  vfunc_10();                                       // [10] 0x5a0ff0
    virtual void  vfunc_11();                                       // [11] 0x4b2f90
    virtual void  vfunc_12();                                       // [12] 0x4b2fd0
    virtual void  vfunc_13();                                       // [13] 0x4b2e90
    virtual void  vfunc_14();                                       // [14] 0x4b2ff0
    virtual void  vfunc_15();                                       // [15] 0x5a0b90
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
    virtual void  vfunc_27();                                       // [27] 0x5a1440
    virtual void  vfunc_28();                                       // [28] 0x5a1890
    virtual void  vfunc_29();                                       // [29] 0x5a3aa0
    virtual void  vfunc_30();                                       // [30] 0x5a3c60
    virtual void  vfunc_31();                                       // [31] 0x476390
    virtual void  vfunc_32();                                       // [32] 0x4776a0
    virtual void  vfunc_33();                                       // [33] 0x5a1840
    virtual void  vfunc_34();                                       // [34] 0x4787f0
    virtual void  vfunc_35();                                       // [35] 0x5a0fd0
    virtual void  vfunc_36();                                       // [36] 0x4736b0
    virtual void  vfunc_37();                                       // [37] 0x473930
    virtual void  vfunc_38();                                       // [38] 0x6829a5 (stub)
    virtual void  vfunc_39();                                       // [39] 0x473260
    virtual void  vfunc_40();                                       // [40] 0x4725c0
    virtual void  vfunc_41();                                       // [41] 0x402af0
    virtual void  vfunc_42();                                       // [42] 0x474520
    virtual void  vfunc_43();                                       // [43] 0x475960
    virtual void  vfunc_44();                                       // [44] 0x474660
    virtual void  vfunc_45();                                       // [45] 0x4759d0
    virtual void  vfunc_46();                                       // [46] 0x475070
    virtual void  vfunc_47();                                       // [47] 0x474530
    virtual void  vfunc_48();                                       // [48] 0x5a0f20
    virtual void  vfunc_49();                                       // [49] 0x5a1460
    virtual void  vfunc_50();                                       // [50] 0x5a0b70

    // ---- data members -----------------------------------------------------
    // Offsets recovered from constructor 0x5a4e20.  The constructor installs
    // two embedded sub-object vtables:
    //   +0x122c vftable 0x699e00 -> a VectorClass<> member (radar event list)
    //   +0x1264 vftable 0x6a2618 -> a VectorClass<> member (radar event list)
    //
    // +0x00   vftable (0x6a35fc)
    // +0x04   DisplayClass / MapClass / GScreenClass base members
    // +0x122c VectorClass<> m_RadarEvents /* ? */  (vtable 0x699e00)
    // +0x1230 int  m_RadarEventsCount /* ? */      (0x5a4e20: [edi+0x1230]=0)
    // +0x1234 int  m_RadarEventsCapacity /* ? */   (0x5a4e20: [edi+0x1234]=0)
    // +0x1264 VectorClass<> m_RadarEvents2 /* ? */ (vtable 0x6a2618)
    // +0x1268 int  m_RadarEvents2Count /* ? */     (0x5a4e20: [edi+0x1268]=0)
    // +0x126c int  m_RadarEvents2Capacity /* ? */  (0x5a4e20: [edi+0x126c]=0)
    //
    // The SidebarClass constructor 0x5d8940 (which also writes vtbl 0x6a35fc)
    // additionally initialises +0x0c, +0x10, +0x14 (=1), +0x1508 and +0x1510,
    // showing the full SidebarClass object extends past 0x1500.
};
