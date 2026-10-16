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

    // ---- data members (offsets from ctor 0x00606fa7) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x6a55e8
    unsigned long m_004;  // init 0x6a55cc
    unsigned char pad_008[0xc];
    unsigned long m_014;  // /* ? */
    unsigned long m_018;  // /* ? */
    unsigned long m_01c;  // /* ? */
    unsigned long m_020;  // /* ? */
    unsigned long m_024;  // /* ? */
    unsigned long m_028;  // /* ? */
    unsigned long m_02c;  // /* ? */
    unsigned long m_030;  // /* ? */
    unsigned long m_034;  // /* ? */
    unsigned long m_038;  // /* ? */
    unsigned long m_03c;  // /* ? */
    unsigned long m_040;  // /* ? */
    unsigned long m_044;  // /* ? */
    unsigned long m_048;  // /* ? */
    unsigned char pad_04c[0x4];
    unsigned long m_050;  // /* ? */
    unsigned long m_054;  // /* ? */
    unsigned char pad_058[0x4];
    unsigned long m_05c;  // /* ? */
    unsigned long m_060;  // /* ? */
    unsigned long m_064;  // /* ? */
    unsigned long m_068;  // /* ? */
    unsigned long m_06c;  // /* ? */
    unsigned long m_070;  // /* ? */

};

#endif // TEAMCLASS_H
