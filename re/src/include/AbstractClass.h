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
// IUnknown is the shared COM base.  The shape here matches ts_prelude.h and
// the binary's own stubs; when this header is copied into src/include,
// tools/reconcile_prelude.py defers this definition to the prelude so the two
// copies never collide.
// IUnknown is defined by ts_prelude.h (shared COM base).
struct IPersist : public IUnknown
{
    virtual HRESULT __stdcall GetClassID(CLSID* pClassID) = 0;              // vtbl[3]
};

// IPersistStream is real COM, not the flat 4-argument form an earlier revision
// of this header carried.
// Evidence: LocomotionClass's IPersistStream vtable at 0x69fe24 has slot 7 =
// 0x4f9dd0, which disassembles to
//   push esi; mov esi,[esp+0xc]; test esi,esi; jne .ok;
//   mov eax,0x80004003; pop esi; ret 8
// -- a __stdcall returning E_POINTER (0x80004003) that pops TWO stack arguments
// (this plus one pointer argument): the COM GetSizeMax(ULARGE_INTEGER*)
// signature, matching ts_prelude.h.
// NOTE: 0x4f9dd0 returns E_POINTER for a null pointer without ever
// dereferencing it, so the binary alone cannot distinguish ULARGE_INTEGER* from
// unsigned long* here; the parameter type comes from the COM convention
// (objidl.h), not from the disassembly.
// The types used below (HRESULT, ULONG, REFIID, CLSID, BOOL, ULARGE_INTEGER)
// come from ts_prelude.h, which tools/make_selfcontained.py includes when this
// header is copied into src/include.
struct IPersistStream : public IPersist
{
    virtual HRESULT __stdcall IsDirty() = 0;                                // vtbl[4]
    virtual HRESULT __stdcall Load(IStream* pStm) = 0;                      // vtbl[5]
    virtual HRESULT __stdcall Save(IStream* pStm, BOOL fClearDirty) = 0;    // vtbl[6]
    virtual HRESULT __stdcall GetSizeMax(ULARGE_INTEGER* pcbSize) = 0;      // vtbl[7]
};

// IRTTITypeInfo lives at displacement 4 of AbstractClass.
// Evidence: RTTI base entry "IRTTITypeInfo (mdisp=4)".
//
// KEEP THIS DEFINITION.  ts_prelude.h carries a conflicting IRTTITypeInfo
// (3 methods, different order); this one has 4 methods with per-slot evidence
// taken from the AbstractClass sub-object vtable, so it is the richer record.
// Which spelling wins is an owner decision, not a defect in this file -- the
// prelude's copy is reduced to a forward declaration by
// tools/reconcile_prelude.py, so the two do not collide at compile time.
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
    // Every slot 0..23 holds code.  The RTTI CompleteObjectLocator pointer for
    // this vtable is at 0x698a5c, immediately BEFORE the vtable (banner above),
    // not in a slot -- so no slot may be skipped.
    //
    // Slot 0 is NOT this class's destructor.  0x405790 is the shared
    // QueryInterface-shaped E_POINTER stub
    //   mov eax,[esp+0xc]; test eax,eax; mov eax,0x80004003; ret 0xc
    // and it is slot 0 of 66 unrelated classes; one address cannot be 66
    // destructors.  AbstractClass's own destructors are 0x405710 (complete,
    // `ret 4`) and 0x405730 (base, `ret`), which set vtables
    // 0x698a60/0x698a44 and appear in NO vtable slot, so the destructor is
    // declared non-virtual here and slot 0 is left unnamed.
    ~AbstractClass();                                                // 0x405710 / 0x405730 (not in a slot)
    virtual void  vfunc_0();                                        // [0] 0x405790 (shared E_POINTER stub; takes this+2, returns E_POINTER)
    virtual void  Init();                                           // [1] 0x405830
    virtual void  Shutdown();                                       // [2] 0x405840
    virtual HRESULT __stdcall QueryInterface(REFIID riid, void** ppv); // [3] 0x6829a5 (stub)
    virtual ULONG   __stdcall AddRef();                             // [4] 0x405990
    virtual ULONG   __stdcall Release();                            // [5] 0x6829a5 (stub)
    virtual void  vfunc_6();                                        // [6] 0x6829a5 (stub; slot was previously omitted)
    virtual int   GetType();                                        // [7] 0x405910
    virtual int   Fetch_ID();                                       // [8] 0x405ac0
    virtual void  Set_ID(int id);                                   // [9] 0x4059b0
    virtual int   Get_RTTI();                                       // [10] 0x4059c0
    virtual HRESULT __stdcall GetClassID(CLSID* pClassID);          // [11] 0x6829a5 (stub)
    virtual HRESULT __stdcall IsDirty();                            // [12] 0x6829a5 (stub)
    virtual HRESULT __stdcall Load(IStream* pStm);                  // [13] 0x405940
    virtual HRESULT __stdcall Save(IStream* pStm, BOOL fClearDirty); // [14] 0x4059d0
    virtual HRESULT __stdcall GetSizeMax(ULARGE_INTEGER* pcbSize);  // [15] 0x4059e0
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
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    unsigned long m_000;  // +0x000 init 0x698a60; 4-byte store: mov dword ptr [eax], 0x698a60
    unsigned long m_004;  // +0x004 init 0x698a44; 4-byte store: mov dword ptr [eax + 4], 0x698a44
    unsigned long m_008;  // +0x008 init 0xffffffff; 4-byte store: mov dword ptr [eax + 8], 0xffffffff
    unsigned long m_00c;  // +0x00c /* ? */; 4-byte store: mov dword ptr [eax + 0xc], ecx
    unsigned char m_010;  // +0x010 /* ? */; 1-byte store: mov byte ptr [eax + 0x10], cl

};
