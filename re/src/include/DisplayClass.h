#pragma once
// ============================================================================
// DisplayClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// Evidence:
//   RTTI class descriptor  : 0x6ad4f8  (CompleteObjectLocator for DisplayClass)
//   TypeDescriptor name    : 0x6be6a8 + 8 -> ".?AVDisplayClass@@"
//   vtable (primary)       : 0x69bb08   (COL ptr slot 0x69bb04)
//   constructor            : 0x5dcd70   (writes vtbl 0x69bb08)
//   destructor             : 0x4b2de0   (vtable slot 0, inherited from MapClass)
//
// DisplayClass derives from MapClass (RTTI base list: MapClass mdisp 0,
// GScreenClass mdisp 0, IGameMap mdisp 0, IUnknown mdisp 0).  It is the
// tactical-view renderer: it owns the TacticalClass gadget (nested class
// DisplayClass::TacticalClass, RTTI COL 0x6ad4b8, vtable 0x69ba88) and the
// cell-drawing / shroud / selection logic.  RadarClass and SidebarClass derive
// from it (hierarchy.txt lines 1189-1193, 1289-1293).
// ============================================================================

#include "MapClass.h"
#include "GadgetClass.h"
#include "ts_prelude.h"   // gadget/UI bases (GScreenClass, GadgetClass, ...)

class DisplayClass : public MapClass
{
public:
    // ---- nested gadget class ---------------------------------------------
    // RTTI COL 0x6ad4b8, TypeDescriptor ".?AVTacticalClass@DisplayClass@@",
    // vtable 0x69ba88 (114 entries).  Derives from GadgetClass (mdisp 0) and
    // LinkClass (mdisp 0).  Handles mouse input over the tactical map.
    class TacticalClass : public GadgetClass
    {
    public:
        virtual ~TacticalClass();                                   // [0] 0x4788e0
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
        virtual void  vfunc_29();                                   // [29] 0x474410
        virtual void  vfunc_30();                                   // [30] 0x4a5a60
        // [31] 0x6ad508 is the RTTI COL pointer, not code.
        // Slots 32..113 re-dispatch the MapClass/DisplayClass virtuals.
    };

    // ---- vtable 0x69bb08 (primary, 82 entries) ----------------------------
    // Slots 48/52/56/62/68/75 are RTTI CompleteObjectLocator pointers
    // (0x6ad558, 0x6ad588, 0x6ad608, 0x6ad5b8, 0x6ad638, 0x6ad6a0), not code.
    virtual ~DisplayClass();                                        // [0] 0x4b2de0
    virtual void  Init();                                           // [1] 0x402ad0
    virtual void  Shutdown();                                       // [2] 0x402ae0
    virtual long  QueryInterface(const void*, void**);              // [3] 0x502000
    virtual unsigned long AddRef();                                 // [4] 0x478920
    virtual void  vfunc_5();                                        // [5] 0x472480
    virtual void  vfunc_6();                                        // [6] 0x4b2e50
    virtual void  vfunc_7();                                        // [7] 0x472520
    virtual void  vfunc_8();                                        // [8] 0x472590
    virtual void  vfunc_9();                                        // [9] 0x4b2ec0
    virtual void  vfunc_10();                                       // [10] 0x473130
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
    virtual void  vfunc_29();                                       // [29] 0x477930
    virtual void  vfunc_30();                                       // [30] 0x477960
    virtual void  vfunc_31();                                       // [31] 0x476390
    virtual void  vfunc_32();                                       // [32] 0x4776a0
    virtual void  vfunc_33();                                       // [33] 0x4778f0
    virtual void  vfunc_34();                                       // [34] 0x4787f0
    virtual void  vfunc_35();                                       // [35] 0x4732b0
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
    virtual void  vfunc_49();                                       // [49] 0x478980
    virtual void  vfunc_50();                                       // [50] 0x5f1020
    virtual void  vfunc_51();                                       // [51] 0x674260
    virtual void  vfunc_53();                                       // [53] 0x478960
    virtual void  vfunc_54();                                       // [54] 0x5f1020
    virtual void  vfunc_55();                                       // [55] 0x5f1070
    virtual void  vfunc_57();                                       // [57] 0x4789d0
    virtual void  vfunc_58();                                       // [58] 0x591fb0
    virtual void  vfunc_59();                                       // [59] 0x44ab70
    virtual void  vfunc_60();                                       // [60] 0x591f40
    virtual void  vfunc_61();                                       // [61] 0x674150
    virtual void  vfunc_63();                                       // [63] 0x4789b0
    virtual void  vfunc_64();                                       // [64] 0x591fb0
    virtual void  vfunc_65();                                       // [65] 0x44ab70
    virtual void  vfunc_66();                                       // [66] 0x591f40
    virtual void  vfunc_67();                                       // [67] 0x591f90
    virtual void  vfunc_69();                                       // [69] 0x478cb0
    virtual void  vfunc_70();                                       // [70] 0x478a50
    virtual void  vfunc_71();                                       // [71] 0x478a90
    virtual void  vfunc_72();                                       // [72] 0x478b40
    virtual void  vfunc_73();                                       // [73] 0x478b70
    virtual void  vfunc_74();                                       // [74] 0x478bb0
    virtual void  vfunc_76();                                       // [76] 0x478d00
    virtual void  vfunc_77();                                       // [77] 0x478a50
    virtual void  vfunc_78();                                       // [78] 0x478bd0
    virtual void  vfunc_79();                                       // [79] 0x478a00
    virtual void  vfunc_80();                                       // [80] 0x478c80
    virtual void  vfunc_81();                                       // [81] 0x478a30

    // ---- data members -----------------------------------------------------
    // The constructor 0x5dcd70 only writes the vtable; the remaining members
    // are initialised by the derived RadarClass/SidebarClass constructors
    // (0x5a4e20, 0x5d8940) which share this layout.  See RadarClass.h for the
    // offsets that belong to the DisplayClass portion of the object.
    //
    // +0x00  vftable (0x69bb08)
    // +0x04  MapClass / GScreenClass base members (see MapClass.h)

    // ---- data members (offsets from ctor 0x004723a0) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69bb08
    unsigned char pad_004[0x1198];
    unsigned long m_119c;  // /* ? */
    unsigned long m_11a0;  // /* ? */
    unsigned long m_11a4;  // /* ? */
    unsigned long m_11a8;  // /* ? */
    unsigned long m_11ac;  // /* ? */
    unsigned long m_11b0;  // /* ? */
    unsigned long m_11b4;  // /* ? */
    unsigned long m_11b8;  // /* ? */
    unsigned long m_11bc;  // /* ? */
    unsigned long m_11c0;  // /* ? */
    unsigned long m_11c4;  // /* ? */
    unsigned long m_11c8;  // /* ? */
    unsigned long m_11cc;  // /* ? */
    unsigned long m_11d0;  // /* ? */
    unsigned long m_11d4;  // /* ? */
    unsigned long m_11d8;  // /* ? */
    unsigned long m_11dc;  // /* ? */
    unsigned long m_11e0;  // /* ? */
    unsigned long m_11e4;  // /* ? */
    unsigned long m_11e8;  // /* ? */

};
