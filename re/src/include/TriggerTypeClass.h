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

    // ---- data members (offsets from ctor 0x0062d4f0) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    unsigned char m_065;  // +0x065 /* ? */; 1-byte store: mov byte ptr [esi + 0x65], al
    unsigned char m_066;  // +0x066 /* ? */; 1-byte store: mov byte ptr [esi + 0x66], al
    unsigned char m_067;  // +0x067 /* ? */; 1-byte store: mov byte ptr [esi + 0x67], al
    unsigned char pad_069[0x3];
    // 10 member(s) already declared above; their offsets are recorded in out/members.json

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

    // ---- data members (offsets from ctor 0x0040f930) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    unsigned char pad_008[0x58];
    unsigned long m_060;  // +0x060 /* ? */; 4-byte store: mov dword ptr [esi + 0x60], eax
    unsigned long m_064;  // +0x064 /* ? */; 4-byte store: mov dword ptr [esi + 0x64], edx
    unsigned long m_068;  // +0x068 /* ? */; 4-byte store: mov dword ptr [esi + 0x68], edx
    unsigned char m_06c;  // +0x06c /* ? */; 1-byte store: mov byte ptr [esi + 0x6c], dl
    unsigned char pad_06d[0x3];
    unsigned long m_070;  // +0x070 /* ? */; 4-byte store: mov dword ptr [esi + 0x70], eax
    unsigned long m_074;  // +0x074 /* ? */; 4-byte store: mov dword ptr [esi + 0x74], edx
    unsigned long m_078;  // +0x078 /* ? */; 4-byte store: mov dword ptr [esi + 0x78], edx
    unsigned char pad_07c[0x4];
    unsigned long m_080;  // +0x080 /* ? */; 4-byte store: mov dword ptr [esi + 0x80], edx
    unsigned long m_084;  // +0x084 /* ? */; 4-byte store: mov dword ptr [esi + 0x84], eax
    unsigned long m_088;  // +0x088 /* ? */; 4-byte store: mov dword ptr [esi + 0x88], edx
    unsigned long m_08c;  // +0x08c /* ? */; 4-byte store: mov dword ptr [esi + 0x8c], eax
    unsigned long m_090;  // +0x090 /* ? */; 4-byte store: mov dword ptr [esi + 0x90], edx
    unsigned long m_094;  // +0x094 /* ? */; 4-byte store: mov dword ptr [esi + 0x94], eax
    unsigned char m_098;  // +0x098 /* ? */; 1-byte store: mov byte ptr [esi + 0x98], dl
    unsigned char m_099;  // +0x099 /* ? */; 1-byte store: mov byte ptr [esi + 0x99], dl
    unsigned char m_09a;  // +0x09a /* ? */; 1-byte store: mov byte ptr [esi + 0x9a], al
    unsigned char m_09b;  // +0x09b /* ? */; 1-byte store: mov byte ptr [esi + 0x9b], al
    unsigned char m_09c;  // +0x09c /* ? */; 1-byte store: mov byte ptr [esi + 0x9c], al
    unsigned char pad_09d[0x3];
    unsigned long m_0a0;  // +0x0a0 /* ? */; 4-byte store: mov dword ptr [esi + 0xa0], edx
    unsigned long m_0a4;  // +0x0a4 /* ? */; 4-byte store: mov dword ptr [esi + 0xa4], edx
    unsigned long m_0a8;  // +0x0a8 /* ? */; 4-byte store: mov dword ptr [esi + 0xa8], edx
    unsigned char pad_0ac[0x20];
    unsigned long m_0cc;  // +0x0cc /* ? */; 4-byte store: mov dword ptr [esi + 0xcc], edx
    unsigned long m_0d0;  // +0x0d0 /* ? */; 4-byte store: mov dword ptr [esi + 0xd0], edx
    // 2 member(s) already declared above; their offsets are recorded in out/members.json

};

#endif // TRIGGERTYPECLASS_H
