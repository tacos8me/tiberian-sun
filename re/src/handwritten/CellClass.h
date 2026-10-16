#pragma once
// ============================================================================
// CellClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// Evidence:
//   RTTI class descriptor  : 0x6aab90  (CompleteObjectLocator for CellClass)
//   TypeDescriptor name    : 0x6c1428 + 8 -> ".?AVCellClass@@"
//   vtable (primary)       : 0x69ae20   (COL ptr slot 0x69ae1c)
//   vtable (IRTTITypeInfo) : 0x69ae04   (COL ptr slot 0x69ae00, mdisp=4)
//   constructor            : 0x44f110   (writes vtbl 0x69ae20; full member init)
//   ctor (from stream)     : 0x456900   (writes vtbl 0x69ae20)
//   ctor (copy)            : 0x45a7e0   (writes vtbl 0x69ae20)
//   destructor             : 0x405790   (vtable slot 0)
//
// CellClass is one isometric map cell.  It derives directly from AbstractClass
// (RTTI base list: AbstractClass mdisp 0, IPersistStream/IPersist/IUnknown
// mdisp 0, IRTTITypeInfo/IUnknown mdisp 4).  It is the element type of
// MapClass's cell grid (VectorClass<CellClass*> @ 0x6a1a68,
// DynamicVectorClass<CellClass*> @ 0x6a2d14) and of the
// DiscreteDistributionClass<CellClass> used for tiberium/ore distribution.
// ============================================================================

#include "AbstractClass.h"

class CellClass : public AbstractClass
{
public:
    // ---- vtable 0x69ae20 (primary, 24 entries) ----------------------------
    // Slot 6 is the RTTI CompleteObjectLocator pointer (0x69ae1c), not code.
    virtual ~CellClass();                                           // [0] 0x405790
    virtual void  Init();                                           // [1] 0x405830
    virtual void  Shutdown();                                       // [2] 0x405840
    virtual long  QueryInterface(const void*, void**);              // [3] 0x457ed0
    virtual unsigned long AddRef();                                 // [4] 0x405990
    virtual long  Load(IStream* pStm);                              // [5] 0x456900
    virtual long  Save(IStream* pStm, int fClearDirty);             // [6] 0x456ab0
    virtual int   GetType();                                        // [7] 0x405910
    virtual long  GetClassID(void* pClassID);                       // [8] 0x45a7e0
    virtual void  Set_ID(int id);                                   // [9] 0x4059b0
    virtual int   Get_RTTI();                                       // [10] 0x4059c0
    virtual long  IsDirty();                                        // [11] 0x45a7c0
    virtual long  GetSizeMax(unsigned long* pcbSize);               // [12] 0x45a7d0
    virtual void  Compute_CRC();                                    // [13] 0x405940
    virtual long  vfunc_14();                                       // [14] 0x4059d0
    virtual long  vfunc_15();                                       // [15] 0x4059e0
    virtual void  vfunc_16();                                       // [16] 0x4059f0
    virtual void  vfunc_17();                                       // [17] 0x405970
    virtual void  vfunc_18();                                       // [18] 0x459530
    virtual void  vfunc_19();                                       // [19] 0x405a30
    virtual void  vfunc_20();                                       // [20] 0x405a60
    virtual void  vfunc_21();                                       // [21] 0x405a70
    virtual void  vfunc_22();                                       // [22] 0x459580
    virtual void  vfunc_23();                                       // [23] 0x405ab0

    // ---- data members -----------------------------------------------------
    // Offsets recovered from the full constructor 0x44f110, which writes the
    // vtable then initialises each member in order.  Names are inferred from
    // the surrounding map/INI semantics; uncertain ones are marked /* ? */.
    //
    // +0x00  vftable (0x69ae20)
    // +0x04  IRTTITypeInfo sub-object vftable (0x69ae04)  [AbstractClass base]
    // +0x08  AbstractClass members (see AbstractClass.h)
    // +0x14  int   m_OccupationBits /* ? */   (0x44f110: [esi+0x14]=eax)
    // +0x18  int   m_Overlay /* ? */          (0x44f110: [esi+0x18]=0)
    // +0x1c  int   m_OverlayData /* ? */      (0x44f110: [esi+0x1c]=0)
    // +0x20  int   m_Smudge /* ? */           (0x44f110: [esi+0x20]=0)
    // +0x24  int   m_LandType /* ? */         (0x44f110: [esi+0x24]=0)
    // +0x28  short m_Height /* ? */           (0x44f110: [esi+0x28]=0xffff)
    // +0x2c  int   m_Flags /* ? */            (0x44f110: [esi+0x2c]=0)
    // +0x30  int   m_IsShrouded /* ? */       (0x44f110: [esi+0x30]=eax)
    // +0x34  int   m_IsVisible /* ? */        (0x44f110: [esi+0x34]=eax)
    // +0x38  int   m_RadarVisible /* ? */     (0x44f110: [esi+0x38]=0)
    // +0x3c  int   m_Occupier /* ? */         (0x44f110: [esi+0x3c]=eax)
    // +0x40  int   m_OccupierData /* ? */     (0x44f110: [esi+0x40]=eax)
    // +0x44  int   m_OccupierList /* ? */     (0x44f110: [esi+0x44]=eax)
    // +0x48  int   m_Infantry /* ? */         (0x44f110: [esi+0x48]=eax)
    // +0x4c  int   m_Unit /* ? */             (0x44f110: [esi+0x4c]=eax)
    // +0x50  int   m_Building /* ? */         (0x44f110: [esi+0x50]=eax)
    // +0x64  int   m_AltOccupationBits /* ? */(0x44f110: [esi+0x64]=0)
    // +0x68  int   m_AltOverlay /* ? */       (0x44f110: [esi+0x68]=0)
    // +0x6c  int   m_AltOverlayData /* ? */   (0x44f110: [esi+0x6c]=0)
    // +0x70  int   m_AltSmudge /* ? */        (0x44f110: [esi+0x70]=0)
    // +0x74  int   m_AltLandType /* ? */      (0x44f110: [esi+0x74]=0)
    // +0x78  int   m_AltHeight /* ? */        (0x44f110: [esi+0x78]=0)
    // +0x7c  int   m_AltFlags /* ? */         (0x44f110: [esi+0x7c]=0x10000)
    // +0xa4  int   m_CellTags /* ? */         (0x44f110: [esi+0xa4]=edx)
    //        INI key "CellTags" is read by 0x47661d (map trigger loader).
    //
    // The stream ctor 0x456900 additionally clears +0x10, +0x24, +0x38;
    // the copy ctor 0x45a7e0 clears +0x18, +0x24, +0x70.

    // ---- data members (offsets from ctor 0x0044e8b0) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    // 6 member(s) already declared above; their offsets are recorded in out/members.json

};
