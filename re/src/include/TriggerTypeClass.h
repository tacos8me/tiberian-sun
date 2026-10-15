// TriggerTypeClass.h
// Reconstructed from GAME.EXE (C&C Tiberian Sun, Westwood Studios 1999).
// Evidence: RTTI COL 0x6b7460 -> ".?AVTriggerTypeClass@@".
// vtables: 0x6a63dc (primary, 30 slots), 0x6a63c0 (secondary, 7 slots).
// Constructor 0x62d640; scalar deleting dtor 0x62e2b0.
//
// Inheritance (RTTI): TriggerTypeClass : AbstractTypeClass : AbstractClass :
//   IPersistStream : IPersist : IUnknown, plus IRTTITypeInfo.
//
// Object layout (ctor 0x62d640):
//   +0x00  vftable (0x6a63dc)
//   +0x04  vftable (0x6a63c0)  /* ? secondary base */
//   +0x78  member /* ? */
//
// TriggerTypeClass is registered in the global type list at 0x77bc40 / 0x77bc50.
//
// INI keys read by the TriggerTypeClass readers (0x62da00, 0x62ddd0):
//   "<none>" (0x62da00, default name placeholder).
// Global trigger keys read by 0x5b3451: "RevealTriggerRadius", "CrateTrigger",
//   "AITriggerSuccessWeightDelta", "AITriggerFailureWeightDelta",
//   "AITriggerTrackRecordCoefficient".
//
// ---------------------------------------------------------------------------
// AITriggerTypeClass
// RTTI COL 0x6a9978 -> ".?AVAITriggerTypeClass@@".
// vtables: 0x699250 (primary, 31 slots), 0x699234 (secondary, 7 slots).
// Inheritance (RTTI): AITriggerTypeClass : AbstractTypeClass : AbstractClass :
//   IPersistStream : IPersist : IUnknown, plus IRTTITypeInfo.
// The AI trigger weight parameters (AITriggerSuccessWeightDelta,
// AITriggerFailureWeightDelta, AITriggerTrackRecordCoefficient) are read by
// 0x5b3451 and stored in this class.

#ifndef TRIGGERTYPECLASS_H
#define TRIGGERTYPECLASS_H

#include <windows.h>
#include "ts_prelude.h"

class TriggerTypeClass
{
public:
    // +0x00  vftable (0x6a63dc)
    // +0x04  vftable (0x6a63c0) /* ? */
    // +0x78  member /* ? */

    TriggerTypeClass();                      // 0x62d640
    virtual ~TriggerTypeClass();             // 0x62e2b0 (scalar deleting dtor)

    // vtable 0x6a63dc (30 slots).  Recovered slots:
    virtual void Slot0();                    // 0x405790
    virtual void Slot1();                    // 0x405830
    virtual void Slot2();                    // 0x405840
    virtual void Slot3();                    // 0x62e2b0 (dtor)
    virtual void Slot4();                    // 0x405990
    virtual void Slot5();                    // 0x62e2f0
    virtual void Slot6();                    // 0x62e360
    virtual void Slot7();                    // 0x405910
    virtual void Slot8();                    // 0x62e3b0
    virtual void Slot9();                    // 0x4059b0
    virtual void Slot10();                   // 0x62d880
    virtual void Slot11();                   // 0x62e380
    virtual void Slot12();                   // 0x62e390
    virtual void Slot13();                   // 0x62e230
    virtual void Slot14();                   // 0x4059d0
    virtual void Slot15();                   // 0x4059e0
    virtual void Slot16();                   // 0x62e3a0
    virtual void Slot17();                   // 0x405970
    virtual void Slot18();                   // 0x405a00
    virtual void Slot19();                   // 0x405a30
    virtual void Slot20();                   // 0x405a60
    virtual void Slot21();                   // 0x405a70
    virtual void Slot22();                   // 0x405a80
    virtual void Slot23();                   // 0x405ab0
    virtual void Slot24();                   // 0x406010
    virtual void Slot25();                   // 0x62da00 (ReadINI)
    virtual void Slot26();                   // 0x62ddd0
};

// ---------------------------------------------------------------------------
// AITriggerTypeClass
// RTTI COL 0x6a9978 -> ".?AVAITriggerTypeClass@@".
// vtables: 0x699250 (primary, 31 slots), 0x699234 (secondary, 7 slots).
// ---------------------------------------------------------------------------
class AITriggerTypeClass
{
public:
    // +0x00  vftable (0x699250)
    // +0x04  vftable (0x699234) /* ? */

    virtual ~AITriggerTypeClass();           // 0x40fab0 (scalar deleting dtor)

    // vtable 0x699250 (31 slots).  Recovered slots:
    virtual void Slot0();                    // 0x405790
    virtual void Slot1();                    // 0x405830
    virtual void Slot2();                    // 0x405840
    virtual void Slot3();                    // 0x40fab0 (dtor)
    virtual void Slot4();                    // 0x405990
    virtual void Slot5();                    // 0x40faf0
    virtual void Slot6();                    // 0x40fb60
    virtual void Slot7();                    // 0x405910
    virtual void Slot8();                    // 0x411060
    virtual void Slot9();                    // 0x4059b0
    virtual void Slot10();                   // 0x4059c0
    virtual void Slot11();                   // 0x411040
    virtual void Slot12();                   // 0x411050
    virtual void Slot13();                   // 0x40fb80
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
    virtual void Slot24();                   // 0x406010
    virtual void Slot25();                   // 0x410730 (ReadINI)
    virtual void Slot26();                   // 0x410c80
};

#endif // TRIGGERTYPECLASS_H
