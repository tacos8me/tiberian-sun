// TeamClass.h
// Reconstructed from GAME.EXE (C&C Tiberian Sun, Westwood Studios 1999).
// Evidence: RTTI COL 0x6b6b78 -> ".?AVTeamClass@@".
// vtables: 0x6a55e8 (primary, 27 slots), 0x6a55cc (secondary, 7 slots).
// Constructor 0x6072d0; scalar deleting dtor 0x6075e0.
//
// Object layout (ctor 0x6072d0):
//   +0x00  vftable (0x6a55e8)
//   +0x04  vftable (0x6a55cc)  /* ? secondary base */
//   +0x14  int m_nTeamTypeIndex /* ? */ (compared against TeamTypeClass+0xa4)
//   +0x73  bool m_bRecruitable /* ? */
//
// TeamClass is a runtime instance of a TeamTypeClass.  It is registered in the
// global team list at 0x7b0cec / 0x7b0cf8.  TeamTypeClass (see TeamTypeClass.h)
// holds the INI-driven configuration; TeamClass holds the live members.
//
// INI keys consumed by the TeamTypeClass readers that drive TeamClass:
// "Recruiter", "Autocreate", "LooseRecruit", "GuardSlower", "Droppod",
// "TransportsReturnOnUnload", "AreTeamMembersRecruitable", "Script",
// "Waypoint", "TaskForce", "Group", "House", "VeteranLevel", "Loadable",
// "Full", "Annoyance", "Prebuild", "Reinforce", "Whiner", "Suicide",
// "Aggressive", "OnTransOnly", "Priority", "TechLevel", "AvoidThreats",
// "IonImmune", "IsBaseDefense", "OnlyTargetHouseEnemy".

#ifndef TEAMCLASS_H
#define TEAMCLASS_H

#include <windows.h>

class TeamTypeClass;

class TeamClass
{
public:
    // +0x00  vftable (0x6a55e8)
    // +0x04  vftable (0x6a55cc) /* ? */
    // +0x14  int m_nTeamTypeIndex /* ? */
    // +0x73  bool m_bRecruitable /* ? */

    TeamClass();                             // 0x6072d0
    virtual ~TeamClass();                    // 0x6075e0 (scalar deleting dtor)

    // vtable 0x6a55e8 (27 slots).  Recovered slots:
    virtual void Slot0();                    // 0x405790
    virtual void Slot1();                    // 0x405830
    virtual void Slot2();                    // 0x405840
    virtual void Slot3();                    // 0x60a140
    virtual void Slot4();                    // 0x405990
    virtual void Slot5();                    // 0x60a030
    virtual void Slot6();                    // 0x60a120
    virtual void Slot7();                    // 0x405910
    virtual void Slot8();                    // 0x60c640
    virtual void Slot9();                    // 0x4059b0
    virtual void Slot10();                   // 0x608dc0
    virtual void Slot11();                   // 0x60c630
    virtual void Slot12();                   // 0x60c620
    virtual void Slot13();                   // 0x60a180
    virtual void Slot14();                   // 0x4059d0
    virtual void Slot15();                   // 0x4059e0
    virtual void Slot16();                   // 0x4059f0
    virtual void Slot17();                   // 0x405970
    virtual void Slot18();                   // 0x405a00
    virtual void Slot19();                   // 0x405a30
    virtual void Slot20();                   // 0x405a60
    virtual void Slot21();                   // 0x405a70
    virtual void Slot22();                   // 0x405a80
    virtual void Slot23();                   // 0x6075e0 (dtor)

    // ---- data members (offsets from ctor 0x00606fb0) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    unsigned char m_065;  // +0x065 /* ? */; 1-byte store: mov byte ptr [esi + 0x65], bl
    unsigned char m_066;  // +0x066 /* ? */; 1-byte store: mov byte ptr [esi + 0x66], bl
    unsigned char m_067;  // +0x067 /* ? */; 1-byte store: mov byte ptr [esi + 0x67], bl
    unsigned char m_069;  // +0x069 /* ? */; 1-byte store: mov byte ptr [esi + 0x69], bl
    unsigned char m_06a;  // +0x06a /* ? */; 1-byte store: mov byte ptr [esi + 0x6a], al
    unsigned char m_06b;  // +0x06b /* ? */; 1-byte store: mov byte ptr [esi + 0x6b], bl
    unsigned char m_06d;  // +0x06d /* ? */; 1-byte store: mov byte ptr [esi + 0x6d], al
    unsigned char m_06e;  // +0x06e /* ? */; 1-byte store: mov byte ptr [esi + 0x6e], bl
    unsigned char m_06f;  // +0x06f /* ? */; 1-byte store: mov byte ptr [esi + 0x6f], bl
    unsigned char m_071;  // +0x071 /* ? */; 1-byte store: mov byte ptr [esi + 0x71], bl
    unsigned char m_072;  // +0x072 /* ? */; 1-byte store: mov byte ptr [esi + 0x72], bl
    unsigned char m_073;  // +0x073 /* ? */; 1-byte store: mov byte ptr [esi + 0x73], bl
    // 27 member(s) already declared above; their offsets are recorded in out/members.json

};

#endif // TEAMCLASS_H
