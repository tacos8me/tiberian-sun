// TeamTypeClass.h
// Reconstructed from GAME.EXE (C&C Tiberian Sun, Westwood Studios 1999).
// Evidence: RTTI COL 0x6b6c10 -> ".?AVTeamTypeClass@@".
// vtables: 0x6a5670 (primary, 28 slots), 0x6a5654 (secondary, 7 slots).
// Constructor 0x60ddd0; scalar deleting dtor 0x60db10.
//
// Object layout (ctor 0x60ddd0):
//   +0x00  vftable (0x6a5670)
//   +0x04  vftable (0x6a5654)  /* ? secondary base */
//   +0xa4  int m_nID /* ? */ (compared against TeamClass+0x14)
//
// TeamTypeClass is registered in the global type list at 0x7b2d70 / 0x7b2d80.
//
// INI keys read by the TeamTypeClass readers (0x60d180, 0x60d510, 0x60d37e):
//   "House", "VeteranLevel", "Loadable", "Full", "Annoyance", "GuardSlower",
//   "Autocreate", "Prebuild", "Reinforce", "Droppod", "Recruiter", "Whiner",
//   "Suicide", "LooseRecruit", "Aggressive", "OnTransOnly", "Priority",
//   "TechLevel", "Group", "AvoidThreats", "IonImmune",
//   "TransportsReturnOnUnload", "AreTeamMembersRecruitable", "IsBaseDefense",
//   "OnlyTargetHouseEnemy", "Waypoint", "Script", "TaskForce".
// Global AI team keys read by 0x5b2343: "TeamDelays", "FillEarliestTeamProbability",
// "DissolveUnfilledTeamDelay", "TotalAITeamCap", "MaximumAIDefensiveTeams",
// "MinimumAIDefensiveTeams", "BuildSpeed", "GameSpeedBias".

#ifndef TEAMTYPECLASS_H
#define TEAMTYPECLASS_H

#include <windows.h>

class TeamTypeClass
{
public:
    // +0x00  vftable (0x6a5670)
    // +0x04  vftable (0x6a5654) /* ? */
    // +0xa4  int m_nID /* ? */

    TeamTypeClass();                         // 0x60ddd0
    virtual ~TeamTypeClass();                // 0x60db10 (scalar deleting dtor)

    // vtable 0x6a5670 (28 slots).  Recovered slots:
    virtual void Slot0();                    // 0x405790
    virtual void Slot1();                    // 0x405830
    virtual void Slot2();                    // 0x405840
    virtual void Slot3();                    // 0x60db10 (dtor)
    virtual void Slot4();                    // 0x405990
    virtual void Slot5();                    // 0x60da90
    virtual void Slot6();                    // 0x60da70
    virtual void Slot7();                    // 0x405910
    virtual void Slot8();                    // 0x60ddd0
    virtual void Slot9();                    // 0x4059b0
    virtual void Slot10();                   // 0x60d130
    virtual void Slot11();                   // 0x60dda0
    virtual void Slot12();                   // 0x60ddb0
    virtual void Slot13();                   // 0x60db50
    virtual void Slot14();                   // 0x4059d0
    virtual void Slot15();                   // 0x4059e0
    virtual void Slot16();                   // 0x60ddc0
    virtual void Slot17();                   // 0x405970
    virtual void Slot18();                   // 0x405a00
    virtual void Slot19();                   // 0x405a30
    virtual void Slot20();                   // 0x405a60
    virtual void Slot21();                   // 0x405a70
    virtual void Slot22();                   // 0x405a80
    virtual void Slot23();                   // 0x405ab0
    virtual void Slot24();                   // 0x406010
    virtual void Slot25();                   // 0x60d180 (ReadINI part 1)
    virtual void Slot26();                   // 0x60d510 (ReadINI part 2)

    // ---- data members (offsets from ctor 0x0060c950) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned char pad_008[0x58];
    unsigned long m_060;  // init 0xffffffff
    unsigned long m_064;  // init 0xffffffff
    unsigned long m_068;  // /* ? */
    unsigned long Loadable;  // +0x06c (INI key)
    unsigned long m_070;  // /* ? */
    unsigned long Whiner;  // +0x074 (INI key)
    unsigned long Droppod;  // +0x078 (INI key)
    unsigned long m_07c;  // init 0x7
    unsigned long Max;  // +0x080 (INI key)
    unsigned long m_084;  // /* ? */
    unsigned long m_088;  // /* ? */
    unsigned long m_08c;  // /* ? */
    unsigned long m_090;  // /* ? */
    unsigned long m_094;  // init 0xffffffff
    unsigned long m_098;  // /* ? */
    unsigned long m_09c;  // /* ? */
    unsigned long m_0a0;  // /* ? */
    unsigned long m_0a4;  // /* ? */
    unsigned long m_0a8;  // /* ? */
    unsigned long m_0ac;  // /* ? */
    // 2 member(s) already declared above; their offsets are recorded in out/members.json

};

#endif // TEAMTYPECLASS_H
