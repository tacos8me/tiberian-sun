// LocomotionClass.h
// Reconstructed from GAME.EXE (C&C Tiberian Sun, Westwood Studios 1999).
// Evidence: MSVC RTTI CompleteObjectLocator at 0x6b0310 -> ".?AVLocomotionClass@@".
//
// LocomotionClass is the abstract base of the ILocomotion interface family.
// Object layout recovered from the constructor at 0x4f99a0:
//   +0x00  vftable  IPersistStream   (vtable VA 0x69fe24)
//   +0x04  vftable  ILocomotion      (vtable VA 0x69fd5c)
//   +0x08  byte     m_bIsMoving /* ? */ (set/cleared by ILocomotion slots 22/23)
//
// The ILocomotion vtable at 0x69fd5c has 50 slots (0x00..0xC4).  Slots 0..48 are
// the pure/overridable ILocomotion interface; slot 49 is the RTTI
// CompleteObjectLocator pointer (0x6b0310).  The IPersistStream vtable at
// 0x69fe24 has 9 slots (0x00..0x20) followed by the COL.
//
// Slot semantics below are inferred from the base implementations and from the
// concrete overrides in the derived classes (see LOCOMOTION_INDEX.md).  Names
// marked /* ? */ are inferred from behaviour, not from a symbol.

#ifndef LOCOMOTIONCLASS_H
#define LOCOMOTIONCLASS_H

#include <windows.h>
#include "AbstractClass.h"
#include "ts_prelude.h"   // CoordStruct, CellStruct

class TechnoClass;
class ILocomotion;
class IPiggyback;
// ---------------------------------------------------------------------------
// IPersistStream
// COM persistence interface.  Recovered from the LocomotionClass IPersistStream
// vtable at 0x69fe24 (9 slots, 0x00..0x20).  Slot 8 is the RTTI COL pointer.
// ---------------------------------------------------------------------------
// IPersistStream is defined by AbstractClass.h (recovered from the real
// 9-slot vtable at 0x69fe24); reused here rather than redefined.

// ---------------------------------------------------------------------------
// ILocomotion
// Abstract interface implemented by every locomotion class.
// vtable VA 0x69fd5c (LocomotionClass), 50 slots.
// ---------------------------------------------------------------------------
class ILocomotion
{
public:
    // slot 0  (0x00) 0x4f9c50  QueryInterface-like; returns E_POINTER on null
    virtual HRESULT __stdcall QueryInterface(REFIID riid, void** ppv) = 0;
    // slot 1  (0x04) 0x4f9bf0  AddRef (increments refcount at 0x7d3104)
    virtual ULONG   __stdcall AddRef() = 0;
    // slot 2  (0x08) 0x4f9c10  Release (decrements refcount at 0x7d3104)
    virtual ULONG   __stdcall Release() = 0;
    // slot 3  (0x0C) 0x4f99c0  Init/SetOwner: stores arg at this+4, returns S_OK
    virtual HRESULT __stdcall Init(TechnoClass* pOwner) = 0;
    // slot 4  (0x10) 0x4f9f70  Is_Moving /* ? */ (base returns false)
    virtual bool    __stdcall Is_Moving() = 0;
    // slot 5  (0x14) 0x4f9f00  Destination/Head_To: copies 3 dwords from 0x7d3060
    virtual void    __stdcall Head_To(CoordStruct* pDest) = 0;
    // slot 6  (0x18) 0x4f9f30  Stop_Moving /* ? */
    virtual void    __stdcall Stop_Moving() = 0;
    // slot 7  (0x1C) 0x4f9e80  Do_Turn /* ? */ (base returns 0)
    virtual int     __stdcall Do_Turn() = 0;
    // slot 8  (0x20) 0x4f9e70  Is_Turning /* ? */ (base returns true)
    virtual bool    __stdcall Is_Turning() = 0;
    // slot 9  (0x24) 0x4f99d0  Mark_All_Occupation_Bits /* ? */
    virtual void    __stdcall Mark_All_Occupation_Bits(int mark) = 0;
    // slot 10 (0x28) 0x4f9a70  Is_Moving_Now /* ? */
    virtual bool    __stdcall Is_Moving_Now() = 0;
    // slot 11 (0x2C) 0x4f9e60  Force_Track /* ? */ (zeroes 2 dwords at arg)
    virtual void    __stdcall Force_Track(int track, CoordStruct* pCoord) = 0;
    // slot 12 (0x30) 0x4f9b60  Get_Speed /* ? */ (calls owner vtbl+0x198)
    virtual int     __stdcall Get_Speed() = 0;
    // slot 13 (0x34) 0x4f9e50  Is_Not_Moving /* ? */ (base returns 0)
    virtual int     __stdcall Is_Not_Moving() = 0;
    // slot 14 (0x38) 0x4f9e30  Get_Status /* ? */ (base returns 0)
    virtual int     __stdcall Get_Status() = 0;
    // slot 15 (0x3C) 0x4f9e40  Get_Status_2 /* ? */ (base returns 2)
    virtual int     __stdcall Get_Status_2() = 0;
    // slot 16 (0x40) 0x4f9ef0  Is_Ok_To_End /* ? */ (base returns true)
    virtual bool    __stdcall Is_Ok_To_End() = 0;
    // slot 17 (0x44) 0x4f9ee0  (base ret 0x10)
    virtual void    __stdcall Slot17() = 0;
    // slot 18 (0x48) 0x4f9ed0  (base ret 4)
    virtual void    __stdcall Slot18() = 0;
    // slot 19 (0x4C) 0x4f9ec0  (base ret 8)
    virtual void    __stdcall Slot19() = 0;
    // slot 20 (0x50) 0x4f9eb0  (base ret 4)
    virtual void    __stdcall Slot20() = 0;
    // slot 21 (0x54) 0x4f9e20  (base ret 4)
    virtual void    __stdcall Slot21() = 0;
    // slot 22 (0x58) 0x4f9b90  Set_Moving_Flag: byte[this+8]=1, calls vtbl+0x60
    virtual void    __stdcall Set_Moving_Flag() = 0;
    // slot 23 (0x5C) 0x4f9bb0  Clear_Moving_Flag: byte[this+8]=0, calls vtbl+0x60
    virtual void    __stdcall Clear_Moving_Flag() = 0;
    // slot 24 (0x60) 0x4f9bd0  Get_Moving_Flag: returns byte[this+8]
    virtual bool    __stdcall Get_Moving_Flag() = 0;
    // slot 25 (0x64) 0x4f9be0  (base returns false)
    virtual bool    __stdcall Slot25() = 0;
    // slot 26 (0x68) 0x4f9e00  (base returns false)
    virtual bool    __stdcall Slot26() = 0;
    // slot 27 (0x6C) 0x4f9e10  (base returns false)
    virtual bool    __stdcall Slot27() = 0;
    // slot 28 (0x70) 0x4f9ea0  (base ret 0x14)
    virtual void    __stdcall Slot28() = 0;
    // slot 29 (0x74) 0x6829a5  pure-call stub (push 0x19; call 0x6855eb)
    virtual void    __stdcall Slot29() = 0;
    // slot 30 (0x78) 0x4f9e90  (base ret 0x10)
    virtual void    __stdcall Slot30() = 0;
    // slot 31 (0x7C) 0x4f9f80  (base ret 8)
    virtual void    __stdcall Slot31() = 0;
    // slot 32 (0x80) 0x4803b0  forwards to owner vtbl+0x10
    virtual void    __stdcall Slot32() = 0;
    // slot 33 (0x84) 0x4f9fb0  forwards to owner vtbl+0x398
    virtual void    __stdcall Slot33() = 0;
    // slot 34 (0x88) 0x4f9f90  (base returns 0)
    virtual int     __stdcall Slot34() = 0;
    // slot 35 (0x8C) 0x4f9fa0  (base returns 0)
    virtual int     __stdcall Slot35() = 0;
    // slot 36 (0x90) 0x47ea20  (base returns 0)
    virtual int     __stdcall Slot36() = 0;
    // slot 37 (0x94) 0x47ea30  (base ret 4)
    virtual void    __stdcall Slot37() = 0;
    // slot 38 (0x98) 0x47ea40  (base returns false)
    virtual bool    __stdcall Slot38() = 0;
    // slot 39 (0x9C) 0x4803c0  (base ret 8)
    virtual void    __stdcall Slot39() = 0;
    // slot 40 (0xA0) 0x4803d0  (base returns false)
    virtual bool    __stdcall Slot40() = 0;
    // slot 41 (0xA4) 0x4803e0  (base returns false)
    virtual bool    __stdcall Slot41() = 0;
    // slot 42 (0xA8) 0x47ea10  forwards to owner vtbl+0x80
    virtual void    __stdcall Slot42() = 0;
    // slot 43 (0xAC) 0x47ea50  (base ret 4)
    virtual void    __stdcall Slot43() = 0;
    // slot 44 (0xB0) 0x4803f0  (base ret 4)
    virtual void    __stdcall Slot44() = 0;
    // slot 45 (0xB4) 0x480400  (base ret 4)
    virtual void    __stdcall Slot45() = 0;
    // slot 46 (0xB8) 0x480410  (base returns -1)
    virtual int     __stdcall Slot46() = 0;
    // slot 47 (0xBC) 0x480420  (base returns -1)
    virtual int     __stdcall Slot47() = 0;
    // slot 48 (0xC0) 0x480430  (base returns -1)
    virtual int     __stdcall Slot48() = 0;
    // slot 49 (0xC4) = CompleteObjectLocator pointer (0x6b0310), not a function
};

// ---------------------------------------------------------------------------
// IPiggyback
// Optional interface implemented by locomotion classes that can carry a
// passenger (Drive, DropPod, Walk).  vtable VA 0x69d6b8 (Drive), 9 slots.
// ---------------------------------------------------------------------------
class IPiggyback
{
public:
    // slot 0 (0x00) 0x47eb70  QueryInterface (adjusts this by -0x14)
    virtual HRESULT __stdcall QueryInterface(REFIID riid, void** ppv) = 0;
    // slot 1 (0x04) 0x47eb80  AddRef
    virtual ULONG   __stdcall AddRef() = 0;
    // slot 2 (0x08) 0x47eb90  Release
    virtual ULONG   __stdcall Release() = 0;
    // slot 3 (0x0C) 0x479a30  Begin_Piggyback /* ? */
    virtual HRESULT __stdcall Begin_Piggyback(TechnoClass* pPassenger) = 0;
    // slot 4 (0x10) 0x479a80  End_Piggyback /* ? */
    virtual HRESULT __stdcall End_Piggyback(TechnoClass* pPassenger) = 0;
    // slot 5 (0x14) 0x479ac0  Is_Ok_To_End /* ? */
    virtual bool    __stdcall Is_Ok_To_End() = 0;
    // slot 6 (0x18) 0x479760  Piggyback_Available /* ? */
    virtual HRESULT __stdcall Piggyback_Available() = 0;
    // slot 7 (0x1C) 0x47ea80  Is_Piggybacking /* ? */ (tests dword[this+0x54])
    virtual bool    __stdcall Is_Piggybacking() = 0;
    // slot 8 (0x20) = CompleteObjectLocator pointer
};

// ---------------------------------------------------------------------------
// LocomotionClass
// Abstract base.  RTTI: ".?AVLocomotionClass@@", COL VA 0x6b0310.
// vtables: IPersistStream 0x69fe24, ILocomotion 0x69fd5c.
// Size: 0x0C bytes (ctor 0x4f99a0).
// ---------------------------------------------------------------------------
class LocomotionClass : public IPersistStream, public ILocomotion
{
public:
    // +0x00  IPersistStream vftable (0x69fe24)
    // +0x04  ILocomotion vftable (0x69fd5c)
    // +0x08  bool m_bIsMoving /* ? */ (set by ILocomotion slot 22/23)

    LocomotionClass();                       // 0x4f99a0
    virtual ~LocomotionClass();              // 0x4cb060 (scalar deleting dtor)

    // IPersistStream (vtable 0x69fe24, 9 slots)
    virtual HRESULT __stdcall QueryInterface(REFIID riid, void** ppv);  // 0x4f9c50
    virtual ULONG   __stdcall AddRef();                                 // 0x4f9bf0
    virtual ULONG   __stdcall Release();                                // 0x4f9c10
    virtual HRESULT __stdcall GetClassID(CLSID* pClassID);              // 0x6829a5 stub
    virtual HRESULT __stdcall IsDirty();                                // 0x47e9f0
    virtual HRESULT __stdcall Load(IStream* pStm);                      // 0x4f9d00
    virtual HRESULT __stdcall Save(IStream* pStm, BOOL fClearDirty);    // 0x4f9dd0
    virtual HRESULT __stdcall GetSizeMax(ULARGE_INTEGER* pcbSize);      // 0x4cb060
    virtual HRESULT __stdcall Slot8();                                  // 0x6829a5 stub

    // ---- data members (offsets from ctor 0x004cae18) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69fe24
    unsigned long m_004;  // init 0x69fd5c

};

#endif // LOCOMOTIONCLASS_H
