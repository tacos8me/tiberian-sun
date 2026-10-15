// CommandClass.h
// Reconstructed from GAME.EXE (C&C Tiberian Sun, Westwood Studios 1999).
// Evidence: RTTI COL 0x6b1458 -> ".?AVCommandClass@@".
// vtable VA 0x6a0938, 7 slots (0x00..0x18); slot 6 is the COL pointer.
//
// Object layout (scalar deleting dtor 0x4e0a00):
//   +0x00  vftable (0x6a0938)
//   +0x04  int m_nID /* ? */  (command identifier; used as %d in name strings)
//
// The 6 virtual slots are:
//   0 (0x00) 0x4e0a30  scalar deleting destructor
//   1 (0x04) 0x4e0a20  Execute()  -- base forwards to vtbl+4 (tail-call)
//   2 (0x08) 0x4e0a70  Get_Name() -- formats "TeamSelect_%d" (0x6cad88) with m_nID
//   3 (0x0C) 0x4e0a90  Get_UI_Name() -- string-table id 0x21 formatted with m_nID
//   4 (0x10) 0x4e0ac0  Get_Description() -- string-table id 0x1c
//   5 (0x14) 0x4e0ad0  Get_UI_Description() -- string-table id 0x22 formatted with m_nID
//
// Every concrete command class overrides slots 1..5 with its own Execute() and
// name/description strings.  The per-class name string constants recovered from
// the vtables are listed with each class below.
//
// Command classes are registered in a global list (0x7b2d54 / 0x7b2d60) and
// dispatched by ID; see 0x4e0990 (base Execute helper).

#ifndef COMMANDCLASS_H
#define COMMANDCLASS_H

#include <windows.h>

class CommandClass
{
public:
    // +0x00  vftable (0x6a0938)
    // +0x04  int m_nID /* ? */

    virtual ~CommandClass();                 // 0x4e0a30 (scalar deleting dtor)
    virtual void Execute();                  // 0x4e0a20 (base: tail-call vtbl+4)
    virtual const char* Get_Name();          // 0x4e0a70 ("TeamSelect_%d")
    virtual const char* Get_UI_Name();       // 0x4e0a90 (string id 0x21)
    virtual const char* Get_Description();   // 0x4e0ac0 (string id 0x1c)
    virtual const char* Get_UI_Description();// 0x4e0ad0 (string id 0x22)
};

// ---------------------------------------------------------------------------
// Concrete command classes.
// Each entry: class name (RTTI), vtable VA, and the recovered name string.
// ---------------------------------------------------------------------------

// vtable 0x6a04a0, COL 0x6b1000
class NextObjectCommandClass : public CommandClass
{
public:
    virtual ~NextObjectCommandClass();       // 0x4e3000
    virtual void Execute();                  // 0x4e0f30
    virtual const char* Get_Name();          // 0x4e0f40
    virtual const char* Get_UI_Name();       // 0x4e0f50
    virtual const char* Get_Description();   // 0x4e0f60
    virtual const char* Get_UI_Description();// 0x4e0f70
};

// vtable 0x6a04bc, COL 0x6b0e70
class PrevObjectCommandClass : public CommandClass
{
public:
    virtual ~PrevObjectCommandClass();       // 0x4e2fe0
    virtual void Execute();                  // 0x4e0ff0
    virtual const char* Get_Name();          // 0x4e1000
    virtual const char* Get_UI_Name();       // 0x4e1010
    virtual const char* Get_Description();   // 0x4e1020
    virtual const char* Get_UI_Description();// 0x4e1030
};

// vtable 0x6a04d8, COL 0x6b0f60
class DeployCommandClass : public CommandClass
{
public:
    virtual ~DeployCommandClass();           // 0x4e2fc0
    virtual void Execute();                  // 0x4e11d0
    virtual const char* Get_Name();          // 0x4e11e0
    virtual const char* Get_UI_Name();       // 0x4e11f0
    virtual const char* Get_Description();   // 0x4e1200
    virtual const char* Get_UI_Description();// 0x4e1210
};

// vtable 0x6a04f4, COL 0x6b0c90
class StopCommandClass : public CommandClass
{
public:
    virtual ~StopCommandClass();             // 0x4e2fa0
    virtual void Execute();                  // 0x4e10b0
    virtual const char* Get_Name();          // 0x4e10c0
    virtual const char* Get_UI_Name();       // 0x4e10d0
    virtual const char* Get_Description();   // 0x4e10e0
    virtual const char* Get_UI_Description();// 0x4e10f0
};

// vtable 0x6a0510, COL 0x6b0ba0
class GuardCommandClass : public CommandClass
{
public:
    virtual ~GuardCommandClass();            // 0x4e2f80
    virtual void Execute();                  // 0x4e1350
    virtual const char* Get_Name();          // 0x4e1360
    virtual const char* Get_UI_Name();       // 0x4e1370
    virtual const char* Get_Description();   // 0x4e1380
    virtual const char* Get_UI_Description();// 0x4e1390
};

// vtable 0x6a052c, COL 0x6b0a60
class ScatterCommandClass : public CommandClass
{
public:
    virtual ~ScatterCommandClass();          // 0x4e2f60
    virtual void Execute();                  // 0x4e1440
    virtual const char* Get_Name();          // 0x4e1450
    virtual const char* Get_UI_Name();       // 0x4e1460
    virtual const char* Get_Description();   // 0x4e1470
    virtual const char* Get_UI_Description();// 0x4e1480
};

// vtable 0x6a0548, COL 0x6b0920
class CenterViewCommandClass : public CommandClass
{
public:
    virtual ~CenterViewCommandClass();       // 0x4e2f40
    virtual void Execute();                  // 0x4e1550
    virtual const char* Get_Name();          // 0x4e1560
    virtual const char* Get_UI_Name();       // 0x4e1570
    virtual const char* Get_Description();   // 0x4e1580
    virtual const char* Get_UI_Description();// 0x4e1590
};

// vtable 0x6a0564, COL 0x6b0790
class CenterBaseCommandClass : public CommandClass
{
public:
    virtual ~CenterBaseCommandClass();       // 0x4e2f20
    virtual void Execute();                  // 0x4e15b0
    virtual const char* Get_Name();          // 0x4e15c0
    virtual const char* Get_UI_Name();       // 0x4e15d0
    virtual const char* Get_Description();   // 0x4e15e0
    virtual const char* Get_UI_Description();// 0x4e15f0
};

// vtable 0x6a0580, COL 0x6b12d0
class AllianceCommandClass : public CommandClass
{
public:
    virtual ~AllianceCommandClass();         // 0x4e2f00
    virtual void Execute();                  // 0x4e1850
    virtual const char* Get_Name();          // 0x4e1860
    virtual const char* Get_UI_Name();       // 0x4e1870
    virtual const char* Get_Description();   // 0x4e1880
    virtual const char* Get_UI_Description();// 0x4e1890
};

// vtable 0x6a059c, COL 0x6b11e0
class SelectViewCommandClass : public CommandClass
{
public:
    virtual ~SelectViewCommandClass();       // 0x4e2ee0
    virtual void Execute();                  // 0x4e1980
    virtual const char* Get_Name();          // 0x4e1990
    virtual const char* Get_UI_Name();       // 0x4e19a0
    virtual const char* Get_Description();   // 0x4e19b0
    virtual const char* Get_UI_Description();// 0x4e19c0
};

// vtable 0x6a05b8, COL 0x6b0e20
class ToggleRepairCommandClass : public CommandClass
{
public:
    virtual ~ToggleRepairCommandClass();     // 0x4e2ec0
    virtual void Execute();                  // 0x4e19e0
    virtual const char* Get_Name();          // 0x4e19f0
    virtual const char* Get_UI_Name();       // 0x4e1a00
    virtual const char* Get_Description();   // 0x4e1a10
    virtual const char* Get_UI_Description();// 0x4e1a20
};

// vtable 0x6a05d4, COL 0x6b0ce0
class ToggleSellCommandClass : public CommandClass
{
public:
    virtual ~ToggleSellCommandClass();       // 0x4e2ea0
    virtual void Execute();                  // 0x4e1a30
    virtual const char* Get_Name();          // 0x4e1a40
    virtual const char* Get_UI_Name();       // 0x4e1a50
    virtual const char* Get_Description();   // 0x4e1a60
    virtual const char* Get_UI_Description();// 0x4e1a70
};

// vtable 0x6a05f0, COL 0x6b0ab0
class TogglePowerCommandClass : public CommandClass
{
public:
    virtual ~TogglePowerCommandClass();      // 0x4e2e80
    virtual void Execute();                  // 0x4e1a80
    virtual const char* Get_Name();          // 0x4e1a90
    virtual const char* Get_UI_Name();       // 0x4e1aa0
    virtual const char* Get_Description();   // 0x4e1ab0
    virtual const char* Get_UI_Description();// 0x4e1ac0
};

// vtable 0x6a060c, COL 0x6b0bf0
class ToggleRadarCommandClass : public CommandClass
{
public:
    virtual ~ToggleRadarCommandClass();      // 0x4e2e60
    virtual void Execute();                  // 0x4e1ba0
    virtual const char* Get_Name();          // 0x4e1bb0
    virtual const char* Get_UI_Name();       // 0x4e1bc0
    virtual const char* Get_Description();   // 0x4e1bd0
    virtual const char* Get_UI_Description();// 0x4e1be0
};

// vtable 0x6a0628, COL 0x6b0f10
class CenterREventCommandClass : public CommandClass
{
public:
    virtual ~CenterREventCommandClass();     // 0x4e2e40
    virtual void Execute();                  // 0x4e1ad0
    virtual const char* Get_Name();          // 0x4e1ae0
    virtual const char* Get_UI_Name();       // 0x4e1af0
    virtual const char* Get_Description();   // 0x4e1b00
    virtual const char* Get_UI_Description();// 0x4e1b10
};

// vtable 0x6a0644, COL 0x6b0fb0
class RSidebarPageDownCommandClass : public CommandClass
{
public:
    virtual ~RSidebarPageDownCommandClass(); // 0x4e2e20
    virtual void Execute();                  // 0x4e1f60
    virtual const char* Get_Name();          // 0x4e1f70
    virtual const char* Get_UI_Name();       // 0x4e1f80
    virtual const char* Get_Description();   // 0x4e1f90
    virtual const char* Get_UI_Description();// 0x4e1fa0
};

// vtable 0x6a0660, COL 0x6b1050
class LSidebarPageDownCommandClass : public CommandClass
{
public:
    virtual ~LSidebarPageDownCommandClass(); // 0x4e2e00
    virtual void Execute();                  // 0x4e1f10
    virtual const char* Get_Name();          // 0x4e1f20
    virtual const char* Get_UI_Name();       // 0x4e1f30
    virtual const char* Get_Description();   // 0x4e1f40
    virtual const char* Get_UI_Description();// 0x4e1f50
};

// vtable 0x6a067c, COL 0x6b13d8
class SidebarPageDownCommandClass : public CommandClass
{
public:
    virtual ~SidebarPageDownCommandClass();  // 0x4e2de0
    virtual void Execute();                  // 0x4e1ec0
    virtual const char* Get_Name();          // 0x4e1ed0
    virtual const char* Get_UI_Name();       // 0x4e1ee0
    virtual const char* Get_Description();   // 0x4e1ef0
    virtual const char* Get_UI_Description();// 0x4e1f00
};

// vtable 0x6a0698, COL 0x6b0d30
class RSidebarPageUpCommandClass : public CommandClass
{
public:
    virtual ~RSidebarPageUpCommandClass();   // 0x4e2dc0
    virtual void Execute();                  // 0x4e1d80
    virtual const char* Get_Name();          // 0x4e1d90
    virtual const char* Get_UI_Name();       // 0x4e1da0
    virtual const char* Get_Description();   // 0x4e1db0
    virtual const char* Get_UI_Description();// 0x4e1dc0
};

// vtable 0x6a06b4, COL 0x6b0ec0
class LSidebarPageUpCommandClass : public CommandClass
{
public:
    virtual ~LSidebarPageUpCommandClass();   // 0x4e2da0
    virtual void Execute();                  // 0x4e1d30
    virtual const char* Get_Name();          // 0x4e1d40
    virtual const char* Get_UI_Name();       // 0x4e1d50
    virtual const char* Get_Description();   // 0x4e1d60
    virtual const char* Get_UI_Description();// 0x4e1d70
};

// vtable 0x6a06d0, COL 0x6b10f0
class SidebarPageUpCommandClass : public CommandClass
{
public:
    virtual ~SidebarPageUpCommandClass();    // 0x4e2d80
    virtual void Execute();                  // 0x4e1ce0
    virtual const char* Get_Name();          // 0x4e1cf0
    virtual const char* Get_UI_Name();       // 0x4e1d00
    virtual const char* Get_Description();   // 0x4e1d10
    virtual const char* Get_UI_Description();// 0x4e1d20
};

// vtable 0x6a06ec, COL 0x6b1190
class RSidebarDownCommandClass : public CommandClass
{
public:
    virtual ~RSidebarDownCommandClass();     // 0x4e2d60
    virtual void Execute();                  // 0x4e1e70
    virtual const char* Get_Name();          // 0x4e1e80
    virtual const char* Get_UI_Name();       // 0x4e1e90
    virtual const char* Get_Description();   // 0x4e1ea0
    virtual const char* Get_UI_Description();// 0x4e1eb0
};

// vtable 0x6a0708, COL 0x6b1280
class LSidebarDownCommandClass : public CommandClass
{
public:
    virtual ~LSidebarDownCommandClass();     // 0x4e2d40
    virtual void Execute();                  // 0x4e1e20
    virtual const char* Get_Name();          // 0x4e1e30
    virtual const char* Get_UI_Name();       // 0x4e1e40
    virtual const char* Get_Description();   // 0x4e1e50
    virtual const char* Get_UI_Description();// 0x4e1e60
};

// vtable 0x6a0724, COL 0x6b1370
class SidebarDownCommandClass : public CommandClass
{
public:
    virtual ~SidebarDownCommandClass();      // 0x4e2d20
    virtual void Execute();                  // 0x4e1dd0
    virtual const char* Get_Name();          // 0x4e1de0
    virtual const char* Get_UI_Name();       // 0x4e1df0
    virtual const char* Get_Description();   // 0x4e1e00
    virtual const char* Get_UI_Description();// 0x4e1e10
};

// vtable 0x6a0740, COL 0x6b0830
class RSidebarUpCommandClass : public CommandClass
{
public:
    virtual ~RSidebarUpCommandClass();       // 0x4e2d00
    virtual void Execute();                  // 0x4e1c90
    virtual const char* Get_Name();          // 0x4e1ca0
    virtual const char* Get_UI_Name();       // 0x4e1cb0
    virtual const char* Get_Description();   // 0x4e1cc0
    virtual const char* Get_UI_Description();// 0x4e1cd0
};

// vtable 0x6a075c, COL 0x6b0970
class LSidebarUpCommandClass : public CommandClass
{
public:
    virtual ~LSidebarUpCommandClass();       // 0x4e2ce0
    virtual void Execute();                  // 0x4e1c40
    virtual const char* Get_Name();          // 0x4e1c50
    virtual const char* Get_UI_Name();       // 0x4e1c60
    virtual const char* Get_Description();   // 0x4e1c70
    virtual const char* Get_UI_Description();// 0x4e1c80
};

// vtable 0x6a0778, COL 0x6b0740
class SidebarUpCommandClass : public CommandClass
{
public:
    virtual ~SidebarUpCommandClass();        // 0x4e2cc0
    virtual void Execute();                  // 0x4e1bf0
    virtual const char* Get_Name();          // 0x4e1c00
    virtual const char* Get_UI_Name();       // 0x4e1c10
    virtual const char* Get_Description();   // 0x4e1c20
    virtual const char* Get_UI_Description();// 0x4e1c30
};

// vtable 0x6a0794, COL 0x6b0880
class ScrollWCommandClass : public CommandClass
{
public:
    virtual ~ScrollWCommandClass();          // 0x4e2ca0
    virtual void Execute();                  // 0x4e2120
    virtual const char* Get_Name();          // 0x4e2130
    virtual const char* Get_UI_Name();       // 0x4e2140
    virtual const char* Get_Description();   // 0x4e2150
    virtual const char* Get_UI_Description();// 0x4e2160
};

// vtable 0x6a07b0, COL 0x6b09c0
class ScrollECommandClass : public CommandClass
{
public:
    virtual ~ScrollECommandClass();          // 0x4e2c80
    virtual void Execute();                  // 0x4e20c0
    virtual const char* Get_Name();          // 0x4e20d0
    virtual const char* Get_UI_Name();       // 0x4e20e0
    virtual const char* Get_Description();   // 0x4e20f0
    virtual const char* Get_UI_Description();// 0x4e2100
};

// vtable 0x6a07cc, COL 0x6b0b00
class ScrollSCommandClass : public CommandClass
{
public:
    virtual ~ScrollSCommandClass();          // 0x4e2c60
    virtual void Execute();                  // 0x4e2060
    virtual const char* Get_Name();          // 0x4e2070
    virtual const char* Get_UI_Name();       // 0x4e2080
    virtual const char* Get_Description();   // 0x4e2090
    virtual const char* Get_UI_Description();// 0x4e20a0
};

// vtable 0x6a07e8, COL 0x6b0d80
class ScrollNCommandClass : public CommandClass
{
public:
    virtual ~ScrollNCommandClass();          // 0x4e2c40
    virtual void Execute();                  // 0x4e2000
    virtual const char* Get_Name();          // 0x4e2010
    virtual const char* Get_UI_Name();       // 0x4e2020
    virtual const char* Get_Description();   // 0x4e2030
    virtual const char* Get_UI_Description();// 0x4e2040
};

// vtable 0x6a0804, COL 0x6b1230
class OptionsCommandClass : public CommandClass
{
public:
    virtual ~OptionsCommandClass();          // 0x4e2c20
    virtual void Execute();                  // 0x4e1fb0
    virtual const char* Get_Name();          // 0x4e1fc0
    virtual const char* Get_UI_Name();       // 0x4e1fd0
    virtual const char* Get_Description();   // 0x4e1fe0
    virtual const char* Get_UI_Description();// 0x4e1ff0
};

// vtable 0x6a0820, COL 0x6b1320
class SetView4CommandClass : public CommandClass
{
public:
    virtual ~SetView4CommandClass();         // 0x4e2c00
    virtual void Execute();                  // 0x4e25b0
    virtual const char* Get_Name();          // 0x4e25c0
    virtual const char* Get_UI_Name();       // 0x4e25d0
    virtual const char* Get_Description();   // 0x4e25e0
    virtual const char* Get_UI_Description();// 0x4e25f0
};

// vtable 0x6a083c, COL 0x6b07e0
class SetView3CommandClass : public CommandClass
{
public:
    virtual ~SetView3CommandClass();         // 0x4e2be0
    virtual void Execute();                  // 0x4e2520
    virtual const char* Get_Name();          // 0x4e2530
    virtual const char* Get_UI_Name();       // 0x4e2540
    virtual const char* Get_Description();   // 0x4e2550
    virtual const char* Get_UI_Description();// 0x4e2560
};

// vtable 0x6a0858, COL 0x6b08d0
class SetView2CommandClass : public CommandClass
{
public:
    virtual ~SetView2CommandClass();         // 0x4e2bc0
    virtual void Execute();                  // 0x4e2490
    virtual const char* Get_Name();          // 0x4e24a0
    virtual const char* Get_UI_Name();       // 0x4e24b0
    virtual const char* Get_Description();   // 0x4e24c0
    virtual const char* Get_UI_Description();// 0x4e24d0
};

// vtable 0x6a0874, COL 0x6b0a10
class SetView1CommandClass : public CommandClass
{
public:
    virtual ~SetView1CommandClass();         // 0x4e2ba0
    virtual void Execute();                  // 0x4e2400
    virtual const char* Get_Name();          // 0x4e2410
    virtual const char* Get_UI_Name();       // 0x4e2420
    virtual const char* Get_Description();   // 0x4e2430
    virtual const char* Get_UI_Description();// 0x4e2440
};

// vtable 0x6a0890, COL 0x6b0b50
class View4CommandClass : public CommandClass
{
public:
    virtual ~View4CommandClass();            // 0x4e2b80
    virtual void Execute();                  // 0x4e2360
    virtual const char* Get_Name();          // 0x4e2370
    virtual const char* Get_UI_Name();       // 0x4e2380
    virtual const char* Get_Description();   // 0x4e2390
    virtual const char* Get_UI_Description();// 0x4e23a0
};

// vtable 0x6a08ac, COL 0x6b0c40
class View3CommandClass : public CommandClass
{
public:
    virtual ~View3CommandClass();            // 0x4e2b60
    virtual void Execute();                  // 0x4e22c0
    virtual const char* Get_Name();          // 0x4e22d0
    virtual const char* Get_UI_Name();       // 0x4e22e0
    virtual const char* Get_Description();   // 0x4e22f0
    virtual const char* Get_UI_Description();// 0x4e2300
};

// vtable 0x6a08c8, COL 0x6b0dd0
class View2CommandClass : public CommandClass
{
public:
    virtual ~View2CommandClass();            // 0x4e2b40
    virtual void Execute();                  // 0x4e2220
    virtual const char* Get_Name();          // 0x4e2230
    virtual const char* Get_UI_Name();       // 0x4e2240
    virtual const char* Get_Description();   // 0x4e2250
    virtual const char* Get_UI_Description();// 0x4e2260
};

// vtable 0x6a08e4, COL 0x6b1140
class View1CommandClass : public CommandClass
{
public:
    virtual ~View1CommandClass();            // 0x4e2b20
    virtual void Execute();                  // 0x4e2180
    virtual const char* Get_Name();          // 0x4e2190
    virtual const char* Get_UI_Name();       // 0x4e21a0
    virtual const char* Get_Description();   // 0x4e21b0
    virtual const char* Get_UI_Description();// 0x4e21c0
};

// vtable 0x6a0900, COL 0x6b1428
class FollowCommandClass : public CommandClass
{
public:
    virtual ~FollowCommandClass();           // 0x4e2b00
    virtual void Execute();                  // 0x4e2640
    virtual const char* Get_Name();          // 0x4e2650
    virtual const char* Get_UI_Name();       // 0x4e2660
    virtual const char* Get_Description();   // 0x4e2670
    virtual const char* Get_UI_Description();// 0x4e2680
};

// vtable 0x6a091c, COL 0x6b1458
class CreateTeamCommandClass : public CommandClass
{
public:
    virtual ~CreateTeamCommandClass();       // 0x4e0a00
    virtual void Execute();                  // 0x4e0900 ("TeamCreate_%d")
    virtual const char* Get_Name();          // 0x4e0920 (string id 0x1f)
    virtual const char* Get_UI_Name();       // 0x4e0950 (string id 0x1c)
    virtual const char* Get_Description();   // 0x4e0960 (string id 0x20)
    virtual const char* Get_UI_Description();// 0x4e0990
};

// vtable 0x6a0954, COL 0x6b14f8
class SelectTeamCommandClass : public CommandClass
{
public:
    virtual ~SelectTeamCommandClass();       // 0x4e0be0
    virtual void Execute();                  // 0x4e0a70 ("TeamSelect_%d")
    virtual const char* Get_Name();          // 0x4e0a90 (string id 0x21)
    virtual const char* Get_UI_Name();       // 0x4e0ac0 (string id 0x1c)
    virtual const char* Get_Description();   // 0x4e0ad0 (string id 0x22)
    virtual const char* Get_UI_Description();// 0x4e0b00
};

// vtable 0x6a0970, COL 0x6b1548
class AddTeamCommandClass : public CommandClass
{
public:
    virtual ~AddTeamCommandClass();          // 0x4e0d50
    virtual void Execute();                  // 0x4e0c20 ("TeamAddSelect_%d")
    virtual const char* Get_Name();          // 0x4e0c40 (string id 0x23)
    virtual const char* Get_UI_Name();       // 0x4e0c70 (string id 0x1c)
    virtual const char* Get_Description();   // 0x4e0c80 (string id 0x24)
    virtual const char* Get_UI_Description();// 0x4e0cb0
};

// vtable 0x6a098c, COL 0x6b1598
class CenterTeamCommandClass : public CommandClass
{
public:
    virtual ~CenterTeamCommandClass();       // 0x4e0f10
    virtual void Execute();                  // 0x4e0d90 ("TeamCenter_%d")
    virtual const char* Get_Name();          // 0x4e0db0 (string id 0x25)
    virtual const char* Get_UI_Name();       // 0x4e0de0 (string id 0x1c)
    virtual const char* Get_Description();   // 0x4e0df0 (string id 0x26)
    virtual const char* Get_UI_Description();// 0x4e0e20
};

// vtable 0x6a09a8, COL 0x6b15e8
class WaypointCommandClass : public CommandClass
{
public:
    virtual ~WaypointCommandClass();         // 0x4e3020
    virtual void Execute();                  // 0x4e26d0 ("WaypointMode")
    virtual const char* Get_Name();          // 0x4e26f0 (string id 0x87)
    virtual const char* Get_UI_Name();       // 0x4e26e0 (string id 0x1b)
    virtual const char* Get_Description();   // 0x4e2700 (string id 0x88)
    virtual const char* Get_UI_Description();// 0x4e2710
};

// vtable 0x6a09c4, COL 0x6b1638
class ScreenCaptureCommandClass : public CommandClass
{
public:
    virtual ~ScreenCaptureCommandClass();    // 0x4e3040
    virtual void Execute();                  // 0x4e2730 ("ScreenCapture")
    virtual const char* Get_Name();          // 0x4e2750 (string id 0x8c)
    virtual const char* Get_UI_Name();       // 0x4e2740 (string id 0x1b)
    virtual const char* Get_Description();   // 0x4e2760 (string id 0x8d)
    virtual const char* Get_UI_Description();// 0x4e2770 ("SCRN%04d.pcx")
};

// vtable 0x6a09e0, COL 0x6b1688
class PageUserCommandClass : public CommandClass
{
public:
    virtual ~PageUserCommandClass();         // 0x4e3060
    virtual void Execute();                  // 0x4e2990 ("PageUser")
    virtual const char* Get_Name();          // 0x4e29a0 (string id 0x17c)
    virtual const char* Get_UI_Name();       // 0x4e29b0 (string id 0x1b)
    virtual const char* Get_Description();   // 0x4e29c0 (string id 0x17d)
    virtual const char* Get_UI_Description();// 0x4e29d0
};

// vtable 0x6a09fc, COL 0x0 (last vtable in .rdata block)
class DeleteWaypointCommandClass : public CommandClass
{
public:
    virtual ~DeleteWaypointCommandClass();   // 0x4e3080
    virtual void Execute();                  // 0x4e29f0 ("DeleteWaypoint")
    virtual const char* Get_Name();          // 0x4e2a00 (string id 0x17a)
    virtual const char* Get_UI_Name();       // 0x4e2a10 (string id 0x1b)
    virtual const char* Get_Description();   // 0x4e2a20 (string id 0x17b)
    virtual const char* Get_UI_Description();// 0x4e2a30
};

#endif // COMMANDCLASS_H
