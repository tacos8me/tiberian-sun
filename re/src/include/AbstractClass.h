#pragma once
#include "ts_prelude.h"
// ============================================================================
// AbstractClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// Evidence:
//   RTTI class descriptor  : 0x6a94d8  (CompleteObjectLocator for AbstractClass)
//   TypeDescriptor name    : 0x6bf0f0 + 8 -> ".?AVAbstractClass@@"
//   vtable (primary)       : 0x698a60   (COL ptr slot 0x6a94e8)
//   vtable (IRTTITypeInfo) : 0x698a44   (COL ptr slot 0x6a9410, mdisp=4)
//   constructor            : 0x4056f0   (writes vtbl 0x698a60 / 0x698a44)
//   destructor / deleting  : 0x405710, 0x405730
//
// AbstractClass is the root of the game-object hierarchy.  It implements
// IPersistStream (so every game object is stream-serialisable) and, at
// displacement 4, IRTTITypeInfo (the RTTI/type-info interface).
// ============================================================================

// IStream is the OLE structured-storage stream interface used by
// IPersistStream.  Forward-declared here; the real definition lives in the
// platform headers (objidl.h) which are outside this task's file set.
struct IStream;
// --- COM-style interfaces implemented by AbstractClass ----------------------
// Evidence: RTTI base list of AbstractClass (classes.json, chd_va 0x6a94d8):
//   IPersistStream (mdisp 0), IPersist (mdisp 0), IUnknown (mdisp 0),
//   IRTTITypeInfo (mdisp 4), IUnknown (mdisp 4).
// IUnknown is defined by ts_prelude.h (shared COM base).
struct IPersist : public IUnknown
{
    virtual long GetClassID(void* pClassID) = 0;                    // vtbl[3]
};

struct IPersistStream : public IPersist
{
    virtual long IsDirty() = 0;                                     // vtbl[4]
    virtual long Load(IStream* pStm) = 0;                           // vtbl[5]
    virtual long Save(IStream* pStm, int fClearDirty) = 0;          // vtbl[6]
    virtual long GetSizeMax(unsigned long* pcbSize) = 0;            // vtbl[7]
};

// IRTTITypeInfo lives at displacement 4 of AbstractClass.
// Evidence: RTTI base entry "IRTTITypeInfo (mdisp=4)".
struct IRTTITypeInfo : public IUnknown
{
    virtual int  GetType() = 0;                                     // vtbl[0]
    virtual int  Fetch_ID() = 0;                                    // vtbl[1]
    virtual void Set_ID(int id) = 0;                                // vtbl[2]
    virtual int  Get_RTTI() = 0;                                    // vtbl[3]
};

// ============================================================================
// AbstractClass
// ============================================================================
class AbstractClass : public IPersistStream, public IRTTITypeInfo
{
public:
    // ---- vtable 0x698a60 (primary, 24 entries) ----------------------------
    // Slot 6 is the RTTI CompleteObjectLocator pointer (0x6a94e8), not code.
    virtual ~AbstractClass();                                       // [0] 0x405790
    virtual void  Init();                                           // [1] 0x405830
    virtual void  Shutdown();                                       // [2] 0x405840
    virtual long  QueryInterface(const void*, void**);              // [3] 0x6829a5 (stub)
    virtual unsigned long AddRef();                                 // [4] 0x405990
    virtual unsigned long Release();                                // [5] 0x6829a5 (stub)
    virtual int   GetType();                                        // [7] 0x405910
    virtual int   Fetch_ID();                                       // [8] 0x405ac0
    virtual void  Set_ID(int id);                                   // [9] 0x4059b0
    virtual int   Get_RTTI();                                       // [10] 0x4059c0
    virtual long  GetClassID(void* pClassID);                       // [11] 0x6829a5 (stub)
    virtual long  IsDirty();                                        // [12] 0x6829a5 (stub)
    virtual long  Load(IStream* pStm);                              // [13] 0x405940
    virtual long  Save(IStream* pStm, int fClearDirty);             // [14] 0x4059d0
    virtual long  GetSizeMax(unsigned long* pcbSize);               // [15] 0x4059e0
    virtual void  Compute_CRC();                                    // [16] 0x4059f0
    virtual void  vfunc_17();                                       // [17] 0x405970
    virtual void  vfunc_18();                                       // [18] 0x405a00
    virtual void  vfunc_19();                                       // [19] 0x405a30
    virtual void  vfunc_20();                                       // [20] 0x405a60
    virtual void  vfunc_21();                                       // [21] 0x405a70
    virtual void  vfunc_22();                                       // [22] 0x405a80
    virtual void  vfunc_23();                                       // [23] 0x405ab0

    // ---- vtable 0x698a44 (IRTTITypeInfo sub-object, 31 entries) -----------
    // Slots 0..23 mirror the primary vtable; slots 24..30 are the
    // IRTTITypeInfo-specific overrides.
    virtual int   GetType_2();                                      // [24] 0x406010
    virtual int   Fetch_ID_2();                                     // [25] 0x405f20  (refs "Name")
    virtual void  Set_ID_2(int id);                                 // [26] 0x405fb0  (refs "Name")
    virtual int   Get_RTTI_2();                                     // [28] 0x406a30
    virtual void  vfunc_29();                                       // [29] 0x675870
    virtual void  vfunc_30();                                       // [30] 0x675910

    // ---- data members -----------------------------------------------------
    // Evidence: constructor 0x4056f0 writes these offsets.
    int   m_ID;             // +0x08  ctor: mov dword ptr [eax+8], 0xffffffff
    int   m_UniqueID;       // +0x0c  ctor: mov dword ptr [eax+0xc], ecx (0)
    bool  m_IsActive;       // +0x10  ctor: mov byte ptr [eax+0x10], cl (0)

    // ---- data members (offsets from ctor 0x004056f0) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    // 5 member(s) already declared above; their offsets are recorded in out/members.json

};
