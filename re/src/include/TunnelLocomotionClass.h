// TunnelLocomotionClass.h
// Reconstructed from GAME.EXE (C&C Tiberian Sun, Westwood Studios 1999).
// Evidence: RTTI COL 0x6b7538 -> ".?AVTunnelLocomotionClass@@".
// vtables: IPersistStream 0x6a65a4, ILocomotion 0x6a64dc.
// Constructor 0x6307d0; scalar deleting dtor 0x630790.
//
// Object layout (ctor 0x6307d0):
//   +0x00  IPersistStream vftable (0x6a65a4)
//   +0x04  ILocomotion     vftable (0x6a64dc)
//   +0x08  bool  m_bIsMoving (inherited)
//   +0x24  dword initialised from [0x7b2e54] /* ? */
//   +0x2C  dword = 0 /* ? */
//   +0x30  dword = 0 /* ? */
//
// Movement parameters read from the owning TechnoTypeClass INI:
// "TunnelSpeed" (0x6d6acc), "Tunnels" (0x6d...), "DirtTunnels", "DirtTrackTunnels",
// "TrackTunnels" (all read by 0x4eaa00), "ZFudgeTunnel" (0x6d...),
// "Locomotor" (0x6dba30), "Speed" (0x6c3334).

#ifndef TUNNELLOCOMOTIONCLASS_H
#define TUNNELLOCOMOTIONCLASS_H

#include "LocomotionClass.h"

class TunnelLocomotionClass : public LocomotionClass
{
public:
    // +0x00  IPersistStream vftable (0x6a65a4)
    // +0x04  ILocomotion     vftable (0x6a64dc)
    // +0x24  dword /* ? */
    // +0x2C  dword /* ? */
    // +0x30  dword /* ? */

    TunnelLocomotionClass();                 // 0x6307d0
    virtual ~TunnelLocomotionClass();        // 0x630790 (scalar deleting dtor)

    // ILocomotion overrides (vtable 0x6a64dc)
    virtual bool    __stdcall Is_Moving();                              // 0x62f1a0
    virtual void    __stdcall Head_To(CoordStruct* pDest);              // 0x62f1e0
    virtual int     __stdcall Do_Turn();                                // 0x630710
    virtual bool    __stdcall Is_Turning();                             // 0x6306e0
    virtual void    __stdcall Mark_All_Occupation_Bits(int mark);       // 0x6301d0
    virtual int     __stdcall Is_Not_Moving();                          // 0x62f8e0
    virtual int     __stdcall Get_Status();                             // 0x6304d0
    virtual int     __stdcall Get_Status_2();                           // 0x6306a0
    virtual bool    __stdcall Is_Ok_To_End();                           // 0x62f560
    virtual void    __stdcall Slot17();                                 // 0x62f250
    virtual void    __stdcall Slot18();                                 // 0x62f330
    virtual void    __stdcall Slot19();                                 // 0x630760
    virtual void    __stdcall Slot29();                                 // 0x630820
    virtual void    __stdcall Slot32();                                 // 0x62f1b0
    virtual int     __stdcall Slot35();                                 // 0x630840
    virtual bool    __stdcall Slot38();                                 // 0x630860

    // ---- data members (offsets from ctor 0x0062f150) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    // vtable-pointer stores are omitted: the compiler supplies the vptr.
    // The machine block begins at +0x8: the compiled end of the
    // base sub-object region. Evidence: LocomotionClass at +0x0 (ends +0x8).
    // 2 recovered offset(s) below +0x8 lie inside the base
    // sub-object region and are NOT declared here: they are declared and
    // compiler-checked in the base's own header -- +0x0, +0x4
    unsigned char pad_008[0xc];
    unsigned int m_014;  // +0x014 /* ? */; 4-byte store: mov dword ptr [esi + 0x14], eax
    unsigned char pad_018[0xc];
    unsigned int m_024;  // +0x024 /* ? */; 4-byte store: mov dword ptr [esi + 0x24], ecx
    unsigned char pad_028[0x4];
    unsigned int m_02c;  // +0x02c /* ? */; 4-byte store: mov dword ptr [esi + 0x2c], eax
    unsigned int m_030;  // +0x030 /* ? */; 4-byte store: mov dword ptr [esi + 0x30], eax
    unsigned char m_034;  // +0x034 /* ? */; 1-byte store: mov byte ptr [esi + 0x34], al

};

#endif // TUNNELLOCOMOTIONCLASS_H
