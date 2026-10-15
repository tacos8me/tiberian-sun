#pragma once
// ============================================================================
// HouseClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// Evidence:
//   RTTI class descriptor  : 0x6afe60
//   TypeDescriptor name    : 0x6c9140 + 8 -> ".?AVHouseClass@@"
//   vtable (primary)       : 0x6afe70 is the COL ptr; primary vtable 0x69fa18
//   vtable (IHouse)        : 0x69f9fc   (COL ptr slot 0x6afd80, mdisp=20)
//   vtable (IPublicHouse)  : 0x69f9bc   (COL ptr slot 0x6afd68, mdisp=24)
//   vtable (IConnPtCont)   : 0x69f97c   (COL ptr slot 0x6afd38, mdisp=28)
//   vtable (IRTTITypeInfo) : 0x69f994   (COL ptr slot 0x6afd50, mdisp=4)
//   constructor            : 0x4b5100   (writes all five vtbl pointers)
//   ctor (light)           : 0x4b3940
//
// HouseClass represents a player/AI faction.  It implements the WOL/COM
// automation interfaces IHouse, IPublicHouse and IConnectionPointContainer
// at displacements 20/24/28.
// ============================================================================

#include "AbstractClass.h"
#include "ts_prelude.h"

// ---------------------------------------------------------------------------
// Automation interfaces implemented by HouseClass.
// Evidence: RTTI base list of HouseClass (classes.json, chd_va 0x6afe60):
//   IHouse (mdisp 20), IPublicHouse (mdisp 24),
//   IConnectionPointContainer (mdisp 28).
// Declared here because their own headers are outside this task's file set.
// ---------------------------------------------------------------------------
struct IHouse : public IUnknown
{
    virtual long  vfunc_0() = 0;                                    // vtbl[0] 0x4c7010
    virtual long  vfunc_1() = 0;                                    // vtbl[1] 0x4c7020
    virtual long  vfunc_2() = 0;                                    // vtbl[2] 0x4c7030
    virtual long  vfunc_3() = 0;                                    // vtbl[3] 0x4bda00
    virtual long  vfunc_4() = 0;                                    // vtbl[4] 0x4bda60
};

struct IPublicHouse : public IUnknown
{
    virtual long  vfunc_0() = 0;                                    // vtbl[0] 0x4c6fc0
    virtual long  vfunc_1() = 0;                                    // vtbl[1] 0x4c6fd0
    virtual long  vfunc_2() = 0;                                    // vtbl[2] 0x4c6fe0
    virtual long  vfunc_3() = 0;                                    // vtbl[3] 0x4c6ff0
    virtual long  vfunc_4() = 0;                                    // vtbl[4] 0x4c7000
};

struct IConnectionPointContainer : public IUnknown
{
    virtual long  vfunc_0() = 0;                                    // vtbl[0] 0x4c6f60
    virtual long  vfunc_1() = 0;                                    // vtbl[1] 0x4c6f70
    virtual long  vfunc_2() = 0;                                    // vtbl[2] 0x4c6f80
};

class HouseClass : public AbstractClass,
                   public IHouse,
                   public IPublicHouse,
                   public IConnectionPointContainer
{
public:
    // ---- vtable 0x69fa18 (primary, 80 entries) ----------------------------
    // Slot 6 is the RTTI CompleteObjectLocator pointer (0x6afe70), not code.
    virtual ~HouseClass();                                          // [0] 0x4b4a70
    virtual void  Init();                                           // [1] 0x4c5190
    virtual void  Shutdown();                                       // [2] 0x4c51a0
    virtual long  QueryInterface(const void*, void**);              // [3] 0x4be9d0
    virtual unsigned long AddRef();                                 // [4] 0x405990
    virtual void  vfunc_5();                                        // [5] 0x4be0b0
    virtual void  vfunc_6();                                        // [6] 0x4be700
    virtual int   GetType();                                        // [7] 0x405910
    virtual void  vfunc_8();                                        // [8] 0x4c51d0
    virtual void  Set_ID(int id);                                   // [9] 0x4059b0
    virtual void  vfunc_10();                                       // [10] 0x4b8850
    virtual long  GetClassID(void* pClassID);                       // [11] 0x4c51b0
    virtual long  IsDirty();                                        // [12] 0x4bea10
    virtual long  Load(IStream* pStm);                              // [13] 0x4bde10
    virtual long  Save(IStream* pStm, int fClearDirty);             // [14] 0x4059d0
    virtual long  GetSizeMax(unsigned long* pcbSize);               // [15] 0x4059e0
    virtual void  Compute_CRC();                                    // [16] 0x4c51c0
    virtual void  vfunc_17();                                       // [17] 0x405970
    virtual void  vfunc_18();                                       // [18] 0x405a00
    virtual void  vfunc_19();                                       // [19] 0x405a30
    virtual void  vfunc_20();                                       // [20] 0x405a60
    virtual void  vfunc_21();                                       // [21] 0x405a70
    virtual void  vfunc_22();                                       // [22] 0x405a80
    virtual void  vfunc_23();                                       // [23] 0x4b5fa0
    virtual void  vfunc_24();                                       // [24] 0x4c6810
    virtual void  vfunc_25();                                       // [25] 0x4c5aa0
    virtual void  vfunc_26();                                       // [26] 0x4c5c90
    virtual void  vfunc_27();                                       // [27] 0x4c5290
    virtual void  vfunc_28();                                       // [28] 0x4c5d90
    virtual void  vfunc_29();                                       // [29] 0x4c52c0
    virtual void  vfunc_30();                                       // [30] 0x4c6770
    virtual void  vfunc_31();                                       // [31] 0x4c5770
    virtual void  vfunc_32();                                       // [32] 0x4c5960
    virtual void  vfunc_33();                                       // [33] 0x4c5240
    virtual void  vfunc_34();                                       // [34] 0x4c5a60
    virtual void  vfunc_35();                                       // [35] 0x4c5270
    virtual void  vfunc_36();                                       // [36] 0x4c6720
    virtual void  vfunc_37();                                       // [37] 0x4c5770
    virtual void  vfunc_38();                                       // [38] 0x4c57d0
    virtual void  vfunc_39();                                       // [39] 0x4c58c0
    virtual void  vfunc_40();                                       // [40] 0x4c58f0
    virtual void  vfunc_41();                                       // [41] 0x4c5940
    virtual void  vfunc_42();                                       // [42] 0x4c67c0
    virtual void  vfunc_43();                                       // [43] 0x4c5aa0
    virtual void  vfunc_44();                                       // [44] 0x4c5b00
    virtual void  vfunc_45();                                       // [45] 0x4c5bf0
    virtual void  vfunc_46();                                       // [46] 0x4c5c20
    virtual void  vfunc_47();                                       // [47] 0x4c5c70
    virtual void  vfunc_48();                                       // [48] 0x4c53e0
    virtual void  vfunc_49();                                       // [49] 0x4c6030
    virtual void  vfunc_50();                                       // [50] 0x4c61b0
    virtual void  vfunc_51();                                       // [51] 0x4c5330
    virtual void  vfunc_52();                                       // [52] 0x4c6260
    virtual void  vfunc_53();                                       // [53] 0x4c5360
    virtual void  vfunc_54();                                       // [54] 0x4c6900
    virtual void  vfunc_55();                                       // [55] 0x4c6030
    virtual void  vfunc_56();                                       // [56] 0x4c6070
    virtual void  vfunc_57();                                       // [57] 0x4c6120
    virtual void  vfunc_58();                                       // [58] 0x4c6150
    virtual void  vfunc_59();                                       // [59] 0x4c6190
    virtual void  vfunc_60();                                       // [60] 0x4c6680
    virtual void  vfunc_61();                                       // [61] 0x4c5510
    virtual void  vfunc_62();                                       // [62] 0x4c5550
    virtual void  vfunc_63();                                       // [63] 0x4c5600
    virtual void  vfunc_64();                                       // [64] 0x4c5630
    virtual void  vfunc_65();                                       // [65] 0x4c5670
    virtual void  vfunc_66();                                       // [66] 0x4c66d0
    virtual void  vfunc_67();                                       // [67] 0x4c5510
    virtual void  vfunc_68();                                       // [68] 0x4c5690
    virtual void  vfunc_69();                                       // [69] 0x4c51f0
    virtual void  vfunc_70();                                       // [70] 0x4c5740
    virtual void  vfunc_71();                                       // [71] 0x4c5220

    // ---- data members -----------------------------------------------------
    // Evidence: constructor 0x4b5100 writes these offsets.
    int   m_Field24;            // +0x24  ctor: mov dword ptr [esi+0x24], ebx (0)
    int   m_Field2c;            // +0x2c  ctor: mov edx, dword ptr [esi+0x2c]
    int   m_Field128;           // +0x128 ctor: lea ecx, [esi+0x128]
    int   m_Field12c;           // +0x12c ctor: mov ecx, dword ptr [esi+0x12c]
    int   m_Field138;           // +0x138 ctor: mov eax, dword ptr [esi+0x138]
    int   m_Field3fc;           // +0x3fc ctor: mov dword ptr [esi+0x3fc], 0x69fab4
    int   m_Field400;           // +0x400 ctor: mov dword ptr [esi+0x400], ebx (0)
    int   m_Field404;           // +0x404 ctor: mov dword ptr [esi+0x404], ebx (0)
    bool  m_Field409;           // +0x409 ctor: mov byte ptr [esi+0x409], bl (0)
    int   m_Field414;           // +0x414 ctor: mov dword ptr [esi+0x414], 0x69fad0
    int   m_Field418;           // +0x418 ctor: mov dword ptr [esi+0x418], ebx (0)
    int   m_Field41c;           // +0x41c ctor: mov dword ptr [esi+0x41c], ebx (0)
    bool  m_Field421;           // +0x421 ctor: mov byte ptr [esi+0x421], bl (0)
    int   m_Field4f4;           // +0x4f4 ctor: mov dword ptr [esi+0x4f4], 0x699db8
    int   m_Field4f8;           // +0x4f8 ctor: lea edi, [esi+0x4f8]
    int   m_Field5d0;           // +0x5d0 ctor: mov dword ptr [esi+0x5d0], ebx (0)
    int   m_Field10e04;         // +0x10e04 ctor: lea edi, [esi+0x10e04]
    int   m_Field10e08;         // +0x10e08 ctor: mov ecx, dword ptr [esi+0x10e08]
    int   m_Field10e14;         // +0x10e14 ctor: mov eax, dword ptr [esi+0x10e14]
    int   m_Field10e1c;         // +0x10e1c ctor: mov dword ptr [esi+0x10e1c], ebx (0)
    // +0x358..+0x3c8: embedded VectorClass/DynamicVectorClass sub-objects
    //   constructed in place (lea ecx,[esi+0x358] .. [esi+0x3c8]).  /* ? */
    // +0x4ec, +0x514, +0x52c: further embedded containers.  /* ? */
};
