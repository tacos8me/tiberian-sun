// WalkLocomotionClass.h
// Reconstructed from GAME.EXE (C&C Tiberian Sun, Westwood Studios 1999).
// Evidence: RTTI COL 0x6b7ae8 -> ".?AVWalkLocomotionClass@@".
// vtables: IPersistStream 0x6a71e0, ILocomotion 0x6a7118, IPiggyback 0x6a70f4.
// Constructor 0x650b50; scalar deleting dtor 0x650cc0.
//
// Object layout (ctor 0x650b50):
//   +0x00  IPersistStream vftable (0x6a71e0)
//   +0x04  ILocomotion     vftable (0x6a7118)
//   +0x08  bool  m_bIsMoving (inherited)
//   +0x14  IPiggyback      vftable (0x6a70f4)
//   +0x34  void* m_pPiggybackedObject /* ? */ (freed in dtor)
//
// Movement parameters read from the owning InfantryTypeClass INI:
// "WalkFrames" (0x6dc0a0), "WalkRate" (0x6d...), "SpeedType" (0x6dc108),
// "MovementRestrictedTo" (0x6d...), "Locomotor" (0x6dba30), "Speed" (0x6c3334).

#ifndef WALKLOCOMOTIONCLASS_H
#define WALKLOCOMOTIONCLASS_H

#include "LocomotionClass.h"

class WalkLocomotionClass : public LocomotionClass, public IPiggyback
{
public:
    // +0x00  IPersistStream vftable (0x6a71e0)
    // +0x04  ILocomotion     vftable (0x6a7118)
    // +0x14  IPiggyback      vftable (0x6a70f4)
    // +0x34  void* m_pPiggybackedObject /* ? */

    WalkLocomotionClass();                   // 0x650b50
    virtual ~WalkLocomotionClass();          // 0x650cc0 (scalar deleting dtor)

    // ILocomotion overrides (vtable 0x6a7118)
    virtual HRESULT __stdcall QueryInterface(REFIID riid, void** ppv);  // 0x6510a0
    virtual ULONG   __stdcall AddRef();                                 // 0x6510b0
    virtual ULONG   __stdcall Release();                                // 0x6510c0
    virtual bool    __stdcall Is_Moving();                              // 0x64f190
    virtual void    __stdcall Head_To(CoordStruct* pDest);              // 0x64f200
    virtual void    __stdcall Stop_Moving();                            // 0x64f260
    virtual bool    __stdcall Is_Ok_To_End();                           // 0x64f2d0
    virtual void    __stdcall Slot17();                                 // 0x64f300
    virtual void    __stdcall Slot18();                                 // 0x64f3b0
    virtual void    __stdcall Slot19();                                 // 0x64f400
    virtual void    __stdcall Slot29();                                 // 0x650cb0
    virtual void    __stdcall Slot30();                                 // 0x64f430
    virtual void    __stdcall Slot32();                                 // 0x64f1a0
    virtual void    __stdcall Slot39();                                 // 0x650ef0
    virtual bool    __stdcall Slot40();                                 // 0x650f40
    virtual void    __stdcall Slot42();                                 // 0x650fe0
    virtual void    __stdcall Slot43();                                 // 0x651030

    // IPiggyback overrides (vtable 0x6a70f4)
    virtual HRESULT __stdcall Begin_Piggyback(TechnoClass* pPassenger); // 0x650d20
    virtual HRESULT __stdcall End_Piggyback(TechnoClass* pPassenger);   // 0x650d70
    virtual bool    __stdcall Is_Ok_To_End_Piggyback();                 // 0x650db0
    virtual HRESULT __stdcall Piggyback_Available();                    // 0x650de0
    virtual bool    __stdcall Is_Piggybacking();                        // 0x651010

    // ---- data members (offsets from ctor 0x0064f0f0) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    unsigned long m_000;  // +0x000 init 0x6a71e0; 4-byte store: mov dword ptr [esi], 0x6a71e0
    unsigned long m_004;  // +0x004 init 0x6a7118; 4-byte store: mov dword ptr [esi + 4], 0x6a7118
    unsigned char pad_008[0xc];
    unsigned long m_014;  // +0x014 init 0x6a70f4; 4-byte store: mov dword ptr [esi + 0x14], 0x6a70f4
    unsigned long m_018;  // +0x018 /* ? */; 4-byte store: mov dword ptr [esi + 0x18], ecx
    unsigned char pad_01c[0x14];
    unsigned char m_030;  // +0x030 /* ? */; 1-byte store: mov byte ptr [esi + 0x30], al
    unsigned long m_034;  // +0x034 /* ? */; 4-byte store: mov dword ptr [esi + 0x34], eax
    // 3 member(s) already declared above; their offsets are recorded in out/members.json

};

#endif // WALKLOCOMOTIONCLASS_H
