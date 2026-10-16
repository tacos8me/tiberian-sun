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
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    unsigned long m_000;  // +0x000 init 0x6a5670; 4-byte store: mov dword ptr [esi], 0x6a5670
    unsigned long m_004;  // +0x004 init 0x6a5654; 4-byte store: mov dword ptr [esi + 4], 0x6a5654
    unsigned char pad_008[0x58];
    unsigned long m_060;  // +0x060 init 0xffffffff; 4-byte store: mov dword ptr [esi + 0x60], 0xffffffff
    unsigned long m_064;  // +0x064 init 0xffffffff; 4-byte store: mov dword ptr [esi + 0x64], 0xffffffff
    unsigned long m_068;  // +0x068 /* ? */; 4-byte store: mov dword ptr [esi + 0x68], eax
    unsigned char Loadable;  // +0x06c (INI key); 1-byte store: mov byte ptr [esi + 0x6c], bl
    unsigned char Full;  // +0x06d (INI key); 1-byte store: mov byte ptr [esi + 0x6d], bl
    unsigned char m_06e;  // +0x06e /* ? */; 1-byte store: mov byte ptr [esi + 0x6e], bl
    unsigned char GuardSlower;  // +0x06f (INI key); 1-byte store: mov byte ptr [esi + 0x6f], bl
    unsigned char m_070;  // +0x070 /* ? */; 1-byte store: mov byte ptr [esi + 0x70], bl
    unsigned char Autocreate;  // +0x071 (INI key); 1-byte store: mov byte ptr [esi + 0x71], bl
    unsigned char m_072;  // +0x072 /* ? */; 1-byte store: mov byte ptr [esi + 0x72], bl
    unsigned char Reinforce;  // +0x073 (INI key); 1-byte store: mov byte ptr [esi + 0x73], bl
    unsigned char Whiner;  // +0x074 (INI key); 1-byte store: mov byte ptr [esi + 0x74], bl
    unsigned char Aggressive;  // +0x075 (INI key); 1-byte store: mov byte ptr [esi + 0x75], bl
    unsigned char m_076;  // +0x076 /* ? */; 1-byte store: mov byte ptr [esi + 0x76], bl
    unsigned char Suicide;  // +0x077 (INI key); 1-byte store: mov byte ptr [esi + 0x77], bl
    unsigned char Droppod;  // +0x078 (INI key); 1-byte store: mov byte ptr [esi + 0x78], bl
    unsigned char m_079;  // +0x079 /* ? */; 1-byte store: mov byte ptr [esi + 0x79], bl
    unsigned char OnTransOnly;  // +0x07a (INI key); 1-byte store: mov byte ptr [esi + 0x7a], bl
    unsigned char pad_07b[0x1];
    unsigned long m_07c;  // +0x07c init 0x7; 4-byte store: mov dword ptr [esi + 0x7c], 7
    unsigned long Max;  // +0x080 (INI key); 4-byte store: mov dword ptr [esi + 0x80], ebx
    unsigned long m_084;  // +0x084 /* ? */; 4-byte store: mov dword ptr [esi + 0x84], ebx
    unsigned long m_088;  // +0x088 /* ? */; 4-byte store: mov dword ptr [esi + 0x88], ebx
    unsigned long m_08c;  // +0x08c /* ? */; 4-byte store: mov dword ptr [esi + 0x8c], ebx
    unsigned long m_090;  // +0x090 /* ? */; 4-byte store: mov dword ptr [esi + 0x90], ebx
    unsigned long m_094;  // +0x094 init 0xffffffff; 4-byte store: mov dword ptr [esi + 0x94], 0xffffffff
    unsigned long m_098;  // +0x098 /* ? */; 4-byte store: mov dword ptr [esi + 0x98], ebx
    unsigned long m_09c;  // +0x09c /* ? */; 4-byte store: mov dword ptr [esi + 0x9c], ebx
    unsigned long m_0a0;  // +0x0a0 /* ? */; 4-byte store: mov dword ptr [esi + 0xa0], ebx
    unsigned long m_0a4;  // +0x0a4 /* ? */; 4-byte store: mov dword ptr [esi + 0xa4], ebx
    unsigned char m_0a8;  // +0x0a8 /* ? */; 1-byte store: mov byte ptr [esi + 0xa8], bl
    unsigned char m_0a9;  // +0x0a9 /* ? */; 1-byte store: mov byte ptr [esi + 0xa9], bl
    unsigned char m_0aa;  // +0x0aa /* ? */; 1-byte store: mov byte ptr [esi + 0xaa], bl
    unsigned char m_0ab;  // +0x0ab /* ? */; 1-byte store: mov byte ptr [esi + 0xab], al
    unsigned char m_0ac;  // +0x0ac /* ? */; 1-byte store: mov byte ptr [esi + 0xac], bl
    unsigned char m_0ad;  // +0x0ad /* ? */; 1-byte store: mov byte ptr [esi + 0xad], bl

};

#endif // TEAMTYPECLASS_H
