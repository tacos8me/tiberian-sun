// DriveLocomotionClass.h
// Reconstructed from GAME.EXE (C&C Tiberian Sun, Westwood Studios 1999).
// Evidence: RTTI COL 0x6ad798 -> ".?AVDriveLocomotionClass@@".
// vtables: IPersistStream 0x69d7a4, ILocomotion 0x69d6dc, IPiggyback 0x69d6b8.
// Constructor 0x4798d0; scalar deleting dtor 0x479870.
//
// Object layout (ctor 0x4798d0):
//   +0x00  IPersistStream vftable (0x69d7a4)
//   +0x04  ILocomotion     vftable (0x69d6dc)
//   +0x08  bool  m_bIsMoving (inherited)
//   +0x14  IPiggyback      vftable (0x69d6b8)
//   +0x68  void* m_pPiggybackedObject /* ? */ (freed in dtor)
//
// Movement parameters are read from the owning TechnoTypeClass INI, not stored
// here: "Speed" (0x6c3334), "Locomotor" (0x6dba30), "AccelerationFactor"
// (0x6db9c4), "DeaccelerationFactor" (0x6db9d8), "Weight" (0x6db9bc),
// "TrackedUphill"/"TrackedDownhill" (0x6d6958/0x6d6948), "IceBreakingWeight"
// (0x6d6990), "IceCrackingWeight" (0x6d69a4), "MovementZone" (0x6db5xx).

#ifndef DRIVELOCOMOTIONCLASS_H
#define DRIVELOCOMOTIONCLASS_H

#include "LocomotionClass.h"

class DriveLocomotionClass : public LocomotionClass, public IPiggyback
{
public:
    // +0x00  IPersistStream vftable (0x69d7a4)
    // +0x04  ILocomotion     vftable (0x69d6dc)
    // +0x14  IPiggyback      vftable (0x69d6b8)
    // +0x68  void* m_pPiggybackedObject /* ? */

    DriveLocomotionClass();                  // 0x4798d0
    virtual ~DriveLocomotionClass();         // 0x479870 (scalar deleting dtor)

    // ILocomotion overrides (vtable 0x69d6dc)
    virtual HRESULT __stdcall QueryInterface(REFIID riid, void** ppv);  // 0x47eb40
    virtual ULONG   __stdcall AddRef();                                 // 0x47eb50
    virtual ULONG   __stdcall Release();                                // 0x47eb60
    virtual bool    __stdcall Is_Moving();                              // 0x479cc0
    virtual void    __stdcall Head_To(CoordStruct* pDest);              // 0x479dc0
    virtual void    __stdcall Stop_Moving();                            // 0x479df0
    virtual void    __stdcall Mark_All_Occupation_Bits(int mark);       // 0x47a040
    virtual int     __stdcall Get_Status();                             // 0x47e630
    virtual int     __stdcall Get_Status_2();                           // 0x47e640
    virtual bool    __stdcall Is_Ok_To_End();                           // 0x47a520
    virtual void    __stdcall Slot17();                                 // 0x479e60
    virtual void    __stdcall Slot18();                                 // 0x479ee0
    virtual void    __stdcall Slot19();                                 // 0x47ae90
    virtual void    __stdcall Slot20();                                 // 0x47a4f0
    virtual void    __stdcall Slot28();                                 // 0x47abf0
    virtual void    __stdcall Slot29();                                 // 0x47e5e0
    virtual void    __stdcall Slot31();                                 // 0x479c80
    virtual void    __stdcall Slot32();                                 // 0x479d50
    virtual void    __stdcall Slot39();                                 // 0x47e690
    virtual bool    __stdcall Slot40();                                 // 0x47e6e0
    virtual bool    __stdcall Slot41();                                 // 0x47e8c0
    virtual void    __stdcall Slot44();                                 // 0x47e9a0
    virtual void    __stdcall Slot45();                                 // 0x47e9b0
    virtual int     __stdcall Slot46();                                 // 0x47e9c0
    virtual int     __stdcall Slot47();                                 // 0x47e9d0
    virtual int     __stdcall Slot48();                                 // 0x47e9e0

    // IPiggyback overrides (vtable 0x69d6b8)
    virtual HRESULT __stdcall Begin_Piggyback(TechnoClass* pPassenger); // 0x479a30
    virtual HRESULT __stdcall End_Piggyback(TechnoClass* pPassenger);   // 0x479a80
    virtual bool    __stdcall Is_Ok_To_End_Piggyback();                 // 0x479ac0
    virtual HRESULT __stdcall Piggyback_Available();                    // 0x479760
    virtual bool    __stdcall Is_Piggybacking();                        // 0x47ea80

    // ---- data members (offsets from ctor 0x00479690) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    unsigned long m_000;  // +0x000 init 0x69d7a4; 4-byte store: mov dword ptr [esi], 0x69d7a4
    unsigned long m_004;  // +0x004 init 0x69d6dc; 4-byte store: mov dword ptr [esi + 4], 0x69d6dc
    unsigned char pad_008[0xc];
    unsigned long m_014;  // +0x014 init 0x69d6b8; 4-byte store: mov dword ptr [esi + 0x14], 0x69d6b8
    unsigned long m_018;  // +0x018 /* ? */; 4-byte store: mov dword ptr [esi + 0x18], eax
    unsigned long m_01c;  // +0x01c /* ? */; 4-byte store: mov dword ptr [esi + 0x1c], eax
    unsigned long m_020;  // +0x020 /* ? */; 4-byte store: mov dword ptr [esi + 0x20], ecx
    unsigned char pad_024[0x4];
    unsigned long m_028;  // +0x028 /* ? */; 4-byte store: mov dword ptr [esi + 0x28], eax
    unsigned long m_02c;  // +0x02c /* ? */; 4-byte store: mov dword ptr [esi + 0x2c], eax
    unsigned char pad_030[0xc];
    unsigned long m_03c;  // +0x03c /* ? */; 4-byte store: mov dword ptr [esi + 0x3c], ecx
    unsigned char pad_040[0x8];
    unsigned long m_048;  // +0x048 /* ? */; 4-byte store: mov dword ptr [esi + 0x48], eax
    unsigned char pad_04c[0x4];
    unsigned long m_050;  // +0x050 /* ? */; 4-byte store: mov dword ptr [esi + 0x50], eax
    unsigned long m_054;  // +0x054 /* ? */; 4-byte store: mov dword ptr [esi + 0x54], eax
    unsigned long m_058;  // +0x058 /* ? */; 4-byte store: mov dword ptr [esi + 0x58], ecx
    unsigned long m_05c;  // +0x05c /* ? */; 4-byte store: mov dword ptr [esi + 0x5c], ecx
    unsigned char m_060;  // +0x060 /* ? */; 1-byte store: mov byte ptr [esi + 0x60], al
    unsigned char m_061;  // +0x061 /* ? */; 1-byte store: mov byte ptr [esi + 0x61], al
    unsigned char m_062;  // +0x062 /* ? */; 1-byte store: mov byte ptr [esi + 0x62], al
    unsigned char m_063;  // +0x063 /* ? */; 1-byte store: mov byte ptr [esi + 0x63], al
    unsigned char m_064;  // +0x064 /* ? */; 1-byte store: mov byte ptr [esi + 0x64], al
    unsigned char m_065;  // +0x065 init 0x1; 1-byte store: mov byte ptr [esi + 0x65], 1
    unsigned char pad_066[0x2];
    unsigned long m_068;  // +0x068 /* ? */; 4-byte store: mov dword ptr [esi + 0x68], eax

};

#endif // DRIVELOCOMOTIONCLASS_H
