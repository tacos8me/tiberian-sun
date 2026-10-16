// DropPodLocomotionClass.h
// Reconstructed from GAME.EXE (C&C Tiberian Sun, Westwood Studios 1999).
// Evidence: RTTI COL 0x6ad990 -> ".?AVDropPodLocomotionClass@@".
// vtables: IPersistStream 0x69db40, ILocomotion 0x69da78, IPiggyback 0x69da54.
// Constructor 0x47ffe0; scalar deleting dtor 0x480210.
//
// Object layout (ctor 0x47ffe0):
//   +0x00  IPersistStream vftable (0x69db40)
//   +0x04  ILocomotion     vftable (0x69da78)
//   +0x08  bool  m_bIsMoving (inherited)
//   +0x14  IPiggyback      vftable (0x69da54)
//   +0x28  void* m_pPiggybackedObject /* ? */ (freed in dtor)
//
// Movement parameters read from the owning TechnoTypeClass INI:
// "DropPodSpeed" (0x6d6af4), "DropPodAngle" (0x6d6ae4), "DropPodHeight"
// (0x6d6b04), "DropPodWeapon" (0x6d...), "Droppod" (0x6db388).

#ifndef DROPPODLOCOMOTIONCLASS_H
#define DROPPODLOCOMOTIONCLASS_H

#include "LocomotionClass.h"

class DropPodLocomotionClass : public LocomotionClass, public IPiggyback
{
public:
    // +0x00  IPersistStream vftable (0x69db40)
    // +0x04  ILocomotion     vftable (0x69da78)
    // +0x14  IPiggyback      vftable (0x69da54)
    // +0x28  void* m_pPiggybackedObject /* ? */

    DropPodLocomotionClass();                // 0x47ffe0
    virtual ~DropPodLocomotionClass();       // 0x480210 (scalar deleting dtor)

    // ILocomotion overrides (vtable 0x69da78)
    virtual HRESULT __stdcall QueryInterface(REFIID riid, void** ppv);  // 0x4804e0
    virtual ULONG   __stdcall AddRef();                                 // 0x4804f0
    virtual ULONG   __stdcall Release();                                // 0x480500
    virtual bool    __stdcall Is_Moving();                              // 0x47f8e0
    virtual void    __stdcall Head_To(CoordStruct* pDest);              // 0x47f8f0
    virtual bool    __stdcall Is_Ok_To_End();                           // 0x47f920
    virtual void    __stdcall Slot17();                                 // 0x47fde0
    virtual void    __stdcall Slot18();                                 // 0x480140
    virtual void    __stdcall Slot29();                                 // 0x480270
    virtual int     __stdcall Slot34();                                 // 0x480390

    // IPiggyback overrides (vtable 0x69da54)
    virtual HRESULT __stdcall Begin_Piggyback(TechnoClass* pPassenger); // 0x480150
    virtual HRESULT __stdcall End_Piggyback(TechnoClass* pPassenger);   // 0x4801a0
    virtual bool    __stdcall Is_Ok_To_End_Piggyback();                 // 0x4801e0
    virtual HRESULT __stdcall Piggyback_Available();                    // 0x480280
    virtual bool    __stdcall Is_Piggybacking();                        // 0x480460

    // ---- data members (offsets from ctor 0x0047f857) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69db40
    unsigned long m_004;  // init 0x69da78
    unsigned char pad_008[0xc];
    unsigned long m_014;  // init 0x69da54
    unsigned long m_018;  // /* ? */
    unsigned char pad_01c[0xc];
    unsigned long m_028;  // /* ? */

};

#endif // DROPPODLOCOMOTIONCLASS_H
