#pragma once
// ============================================================================
// SidebarClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// Evidence:
//   RTTI class descriptor  : 0x6b4cf8  (CompleteObjectLocator for SidebarClass)
//   TypeDescriptor name    : 0x6be708 + 8 -> ".?AVSidebarClass@@"
//   vtable (primary)       : 0x6a4480   (COL ptr slot 0x6a447c)
//   constructor            : 0x5d8940   (writes vtbl 0x6a35fc then 0x6a4480)
//   destructor             : 0x4b2de0   (vtable slot 0, inherited from MapClass)
//
// SidebarClass derives from PowerClass (RTTI base list: PowerClass mdisp 0,
// RadarClass mdisp 0, DisplayClass mdisp 0, MapClass mdisp 0, GScreenClass
// mdisp 0, IGameMap mdisp 0, IUnknown mdisp 0).  It is the right-hand build
// sidebar.  It owns two nested gadget classes:
//   SidebarClass::SBGadgetClass        (RTTI COL 0x6b4c50, vtable 0x6a437c)
//   SidebarClass::StripClass::SelectClass
//                                      (RTTI COL 0x6b4ca8, vtable 0x6a43fc)
//
// INI key "SidebarImage" is referenced by 0x5f3140 (sidebar art loader).
// ============================================================================

#include "RadarClass.h"
#include "PowerClass.h"
#include "ts_prelude.h"
#include "ControlClass.h"   // gadget/UI bases (GScreenClass, GadgetClass, ...)

class SidebarClass : public PowerClass
{
public:
    // ---- nested gadget classes -------------------------------------------
    // SBGadgetClass: RTTI COL 0x6b4c50, TypeDescriptor
    // ".?AVSBGadgetClass@SidebarClass@@", vtable 0x6a437c (116 entries).
    // Derives from GadgetClass (mdisp 0) and LinkClass (mdisp 0).
    class SBGadgetClass : public GadgetClass
    {
    public:
        virtual ~SBGadgetClass();                                   // [0] 0x5dcd00
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
        virtual void  vfunc_29();                                   // [29] 0x5dc310
        virtual void  vfunc_30();                                   // [30] 0x4a5a60
        // [31] 0x6b4cb8 is the RTTI COL pointer, not code.
        // Slots 32..115 re-dispatch the base virtuals (0x5dcce0, 0x5dbde0,
        // 0x5dcd50, 0x5d8a40, 0x5d8a90, 0x5d8b50, 0x5da0a0, 0x5d9990, ...).
    };

    // SelectClass: RTTI COL 0x6b4ca8, TypeDescriptor
    // ".?AVSelectClass@StripClass@SidebarClass@@", vtable 0x6a43fc (84 entries).
    // Derives from ControlClass (mdisp 0), GadgetClass (mdisp 0),
    // LinkClass (mdisp 0).  It is the per-strip selection button.
    class StripClass
    {
    public:
        class SelectClass : public ControlClass
        {
        public:
            virtual ~SelectClass();                                 // [0] 0x5dcce0
            virtual void  vfunc_1();                                // [1] 0x4a5b00
            virtual void  vfunc_2();                                // [2] 0x4a5b10
            virtual void  vfunc_3();                                // [3] 0x4f69e0
            virtual void  vfunc_4();                                // [4] 0x4f6a20
            virtual void  vfunc_5();                                // [5] 0x4f6a00
            virtual void  vfunc_6();                                // [6] 0x4f6980
            virtual void  vfunc_7();                                // [7] 0x4f69b0
            virtual void  vfunc_8();                                // [8] 0x4f6930
            virtual void  vfunc_9();                                // [9] 0x4a5ae0
            virtual void  vfunc_10();                               // [10] 0x4a5be0
            virtual void  vfunc_11();                               // [11] 0x4a5bb0
            virtual void  vfunc_12();                               // [12] 0x4a5b20
            virtual void  vfunc_13();                               // [13] 0x4a5de0
            virtual void  vfunc_14();                               // [14] 0x45afe0
            virtual void  vfunc_15();                               // [15] 0x4a5ad0
            virtual void  vfunc_16();                               // [16] 0x4a5ac0
            virtual void  vfunc_17();                               // [17] 0x460610
            virtual void  vfunc_18();                               // [18] 0x4a5e20
            virtual void  vfunc_19();                               // [19] 0x460650
            virtual void  vfunc_20();                               // [20] 0x4a5e60
            virtual void  vfunc_21();                               // [21] 0x4a5e90
            virtual void  vfunc_22();                               // [22] 0x4a5eb0
            virtual void  vfunc_23();                               // [23] 0x4a5ec0
            virtual void  vfunc_24();                               // [24] 0x45aff0
            virtual void  vfunc_25();                               // [25] 0x4a5ee0
            virtual void  vfunc_26();                               // [26] 0x4a5f00
            virtual void  vfunc_27();                               // [27] 0x460620
            virtual void  vfunc_28();                               // [28] 0x4a5e30
            virtual void  vfunc_29();                               // [29] 0x5dbde0
            virtual void  vfunc_30();                               // [30] 0x4a5a60
            virtual void  vfunc_31();                               // [31] 0x460600
            // [32] 0x6b4d08 is the RTTI COL pointer, not code.
            // Slots 33..83 re-dispatch the base virtuals.
        };
    };

    // ---- vtable 0x6a4480 (primary, 51 entries) ----------------------------
    virtual ~SidebarClass();                                        // [0] 0x4b2de0
    virtual void  Init();                                           // [1] 0x402ad0
    virtual void  Shutdown();                                       // [2] 0x402ae0
    virtual long  QueryInterface(const void*, void**);              // [3] 0x502000
    virtual unsigned long AddRef();                                 // [4] 0x5dcd50
    virtual void  vfunc_5();                                        // [5] 0x5d8a40
    virtual void  vfunc_6();                                        // [6] 0x4b2e50
    virtual void  vfunc_7();                                        // [7] 0x5d8a90
    virtual void  vfunc_8();                                        // [8] 0x5d8b50
    virtual void  vfunc_9();                                        // [9] 0x4b2ec0
    virtual void  vfunc_10();                                       // [10] 0x5da0a0
    virtual void  vfunc_11();                                       // [11] 0x4b2f90
    virtual void  vfunc_12();                                       // [12] 0x4b2fd0
    virtual void  vfunc_13();                                       // [13] 0x4b2e90
    virtual void  vfunc_14();                                       // [14] 0x4b2ff0
    virtual void  vfunc_15();                                       // [15] 0x5d9990
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
    virtual void  vfunc_29();                                       // [29] 0x5dccc0
    virtual void  vfunc_30();                                       // [30] 0x5dccd0
    virtual void  vfunc_31();                                       // [31] 0x476390
    virtual void  vfunc_32();                                       // [32] 0x5dca40
    virtual void  vfunc_33();                                       // [33] 0x5dc4a0
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
    virtual void  vfunc_50();                                       // [50] 0x5d8d30

    // ---- data members -----------------------------------------------------
    // Offsets recovered from constructor 0x5d8940, which first writes the
    // RadarClass vtable 0x6a35fc (base sub-object) and then the SidebarClass
    // vtable 0x6a4480.
    //
    // +0x00   vftable (0x6a4480)
    // +0x04   PowerClass / RadarClass / DisplayClass / MapClass base members
    // +0x0c   int  m_StripCount /* ? */       (0x5d8940: [esi+0x0c]=0)
    // +0x10   int  m_StripCapacity /* ? */    (0x5d8940: [esi+0x10]=0)
    // +0x14   int  m_IsActive /* ? */         (0x5d8940: [esi+0x14]=1)
    // +0x1508 int  m_Strips /* ? */           (0x5d8940: [edi+0x1508]=eax)
    // +0x1510 int  m_SelectedStrip /* ? */    (0x5d8940: [edi+0x1510]=0)
    //
    // The object is at least 0x1514 bytes; the exact StripClass array layout
    // between +0x18 and +0x1508 is not resolved here /* ? */.
};
