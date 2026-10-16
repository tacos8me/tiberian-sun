// TriggerClass.h
// Reconstructed from GAME.EXE (C&C Tiberian Sun, Westwood Studios 1999).
// Evidence: RTTI COL 0x6b73e0 -> ".?AVTriggerClass@@".
// vtables: 0x6a6348 (primary, 30 slots), 0x6a632c (secondary, 7 slots).
// Constructor 0x62c890; scalar deleting dtor 0x62d080.
//
// Object layout (ctor 0x62c890):
//   +0x00  vftable (0x6a6348)
//   +0x04  vftable (0x6a632c)  /* ? secondary base */
//   +0x14  int m_nOwner /* ? */ (from ctor arg)
//   +0x18  int m_nState /* ? */ = 0
//   +0x1c  bool m_bEnabled /* ? */ = 0
//   +0x20  dword from [0x7b2e54] /* ? */
//   +0x28  int /* ? */ = 0
//   +0x2c  int /* ? */ = 0
//   +0x30  bool /* ? */ = 1
//
// TriggerClass is a runtime instance of a TriggerTypeClass.  It is registered in
// the global trigger list at 0x7dd4c8 / 0x7dd4d0.
//
// INI keys consumed by the TriggerTypeClass readers that drive TriggerClass:
// "RevealTriggerRadius", "CrateTrigger", "AITriggerSuccessWeightDelta",
// "AITriggerFailureWeightDelta", "AITriggerTrackRecordCoefficient".

#ifndef TRIGGERCLASS_H
#define TRIGGERCLASS_H

#include <windows.h>

class TriggerTypeClass;

class TriggerClass
{
public:
    // +0x00  vftable (0x6a6348)
    // +0x04  vftable (0x6a632c) /* ? */
    // +0x14  int m_nOwner /* ? */
    // +0x18  int m_nState /* ? */
    // +0x1c  bool m_bEnabled /* ? */
    // +0x20  dword /* ? */
    // +0x28  int /* ? */
    // +0x2c  int /* ? */
    // +0x30  bool /* ? */

    TriggerClass();                          // 0x62c890
    virtual ~TriggerClass();                 // 0x62d080 (scalar deleting dtor)

    // vtable 0x6a6348 (30 slots).  Recovered slots:
    virtual void Slot0();                    // 0x405790
    virtual void Slot1();                    // 0x405830
    virtual void Slot2();                    // 0x405840
    virtual void Slot3();                    // 0x62d080 (dtor)
    virtual void Slot4();                    // 0x405990
    virtual void Slot5();                    // 0x62d0c0
    virtual void Slot6();                    // 0x62d120
    virtual void Slot7();                    // 0x405910
    virtual void Slot8();                    // 0x62d180
    virtual void Slot9();                    // 0x4059b0
    virtual void Slot10();                   // 0x62cef0
    virtual void Slot11();                   // 0x62d170
    virtual void Slot12();                   // 0x62d160
    virtual void Slot13();                   // 0x62cff0
    virtual void Slot14();                   // 0x4059d0
    virtual void Slot15();                   // 0x4059e0
    virtual void Slot16();                   // 0x4059f0
    virtual void Slot17();                   // 0x405970
    virtual void Slot18();                   // 0x405a00
    virtual void Slot19();                   // 0x405a30
    virtual void Slot20();                   // 0x405a60
    virtual void Slot21();                   // 0x405a70
    virtual void Slot22();                   // 0x405a80
    virtual void Slot23();                   // 0x405ab0

    // ---- data members (offsets from ctor 0x0062c890) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x6a6348
    unsigned long m_004;  // init 0x6a632c
    unsigned char pad_008[0xc];
    unsigned long m_014;  // /* ? */
    unsigned long m_018;  // /* ? */
    unsigned long m_01c;  // /* ? */
    unsigned long m_020;  // /* ? */
    unsigned char pad_024[0x4];
    unsigned long m_028;  // /* ? */
    unsigned long m_02c;  // /* ? */
    unsigned long m_030;  // init 0x1

};

#endif // TRIGGERCLASS_H
