#pragma once
// ==========================================================================
// FootClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class FootClass
// vtable       : 0x0069e340  (236 slots)
// constructor  : 0x0049c0f0
// vtable store : 0x0049c2c3  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "TechnoClass.h"

class FootClass : public TechnoClass
{
public:
    // ---- vtable 0x0069e340 (236 slots) ----
    virtual void QueryInterface(int a0, int a1, int a2);  // [0] 0x00405790  // __stdcall, returns int
    virtual void AddRef(int a0);  // [1] 0x00405830  // __stdcall, returns int
    virtual void Release(int a0);  // [2] 0x00405840  // __stdcall, returns int
    virtual void vfunc_3();  // [3] 0x006829a5  // __cdecl
    virtual void IsDirty(int a0);  // [4] 0x00405990  // __stdcall
    virtual void Load(int a0, int a1);  // [5] 0x004a1d80  // __stdcall
    virtual void vfunc_6(int a0, int a1, int a2, int a3, int a4, int a5);  // [6] 0x004a1ff0
    virtual void GetSizeMax(int a0, int a1);  // [7] 0x00405910  // __stdcall, returns int
    virtual void Init();  // [9] 0x0060f180  // __cdecl
    virtual void Detach(int a0, int a1);  // [10] 0x004a0ca0  // __thiscall
    virtual void What_Am_I();  // [11] 0x006829a5  // __cdecl
    virtual void Fetch_Object_Size();  // [12] 0x006829a5  // __cdecl
    virtual void Compute_CRC(int a0, int a1, int a2);  // [13] 0x004a23c0  // __thiscall
    virtual void Owner();  // [14] 0x00612db0  // __cdecl
    virtual void Owner_HouseClass();  // [15] 0x00612dc0  // __cdecl
    virtual void Fetch_Heap_ID();  // [16] 0x004059f0  // __cdecl, returns int
    virtual void Is_Inactive();  // [17] 0x00570a20  // __cdecl, returns int
    virtual void Center_Coord(int a0);  // [18] 0x00570960  // __stdcall
    virtual void Destination_Coord(int a0);  // [19] 0x004a26e0  // __thiscall
    virtual void On_Ground();  // [20] 0x00570e80  // __cdecl
    virtual void In_Air(int a0, int a1);  // [21] 0x004a4a30  // __thiscall
    virtual void As_Coord(int a0);  // [22] 0x00405a80  // __stdcall
    virtual void AI(int a0);  // [23] 0x004a16e0  // __thiscall
    virtual void Is_Players_Army();  // [24] 0x0060eb70  // __thiscall
    virtual void Visual_Character(int a0, int a1);  // [25] 0x004a1690  // __thiscall
    virtual void Get_Image_Data();  // [26] 0x0056e820
    virtual void What_Action(int a0, int a1, int a2);  // [27] 0x004a4270  // __thiscall, returns int
    virtual void What_Action(int a0, int a1);  // [28] 0x004a4360  // __thiscall
    virtual void In_Which_Layer();  // [29] 0x004a2140  // __thiscall
    virtual void Not_Underground();  // [30] 0x00570ef0  // __cdecl
    virtual void Considered_Vehicle();  // [31] 0x004167d0  // __cdecl
    virtual void Techno_Type_Class();  // [32] 0x0060eb10  // __thiscall
    virtual void Class_Of();  // [33] 0x004a4b00  // __cdecl, returns int
    virtual void Get_Ownable();  // [34] 0x0061b1b0  // __cdecl
    virtual void Full_Name();  // [35] 0x004167e0  // __cdecl, returns int
    virtual void Can_Repair();  // [36] 0x00616470  // __thiscall
    virtual void Can_Demolish(int a0, int a1, int a2, int a3, int a4);  // [37] 0x004a1080  // __thiscall
    virtual void Can_Player_Fire();  // [38] 0x00616410  // __thiscall
    virtual void Can_Player_Move();  // [39] 0x00616130  // __thiscall
    virtual void Target_Coord(int a0);  // [40] 0x0040e5d0  // __stdcall
    virtual void Docking_Coord(int a0);  // [41] 0x0040e600  // __stdcall
    virtual void Render_Coord(int a0);  // [42] 0x0040e630  // __stdcall
    virtual void Fire_Coord(int a0, int a1);  // [43] 0x0060edd0  // __thiscall
    virtual void Exit_Coord(int a0);  // [44] 0x0040e660  // __stdcall
    virtual void Sort_Y();  // [45] 0x00570ed0  // __cdecl
    virtual void Is_Moving_Onto_Bridge();  // [46] 0x004a4120  // __cdecl
    virtual void Occupies_Cells();  // [47] 0x0040e7c0  // __cdecl
    virtual void Limbo(int a0, int a1, int a2, int a3, int a4);  // [48] 0x004a1cc0  // __thiscall
    virtual void Unlimbo(int a0, int a1);  // [49] 0x0049ec30  // __thiscall
    virtual void Detach_All(int a0);  // [50] 0x004a0a00  // __thiscall
    virtual void Record_The_Kill(int a0);  // [51] 0x00617760  // __thiscall
    virtual void Paradrop(int a0);  // [52] 0x005700c0  // __thiscall
    virtual void Fall_From_Height(int a0);  // [53] 0x0056ea70  // __thiscall
    virtual void Set_Occupy_Bit(int a0);  // [54] 0x00570480  // __stdcall
    virtual void Clear_Occupy_Bit(int a0);  // [55] 0x00570500  // __stdcall
    virtual void Delete_Me();  // [56] 0x004a4a10  // __thiscall
    virtual void Do_Shimmer();  // [57] 0x00617ce0  // __cdecl
    virtual void Exit_Object(int a0);  // [58] 0x0061b480  // __stdcall, returns int
    virtual void Render(int a0, int a1, int a2);  // [59] 0x0056f480  // __thiscall
    virtual void Occupy_List(int a0);  // [60] 0x005701b0  // __thiscall, returns int
    virtual void Draw_Pre_Render(int a0, int a1, int a2, int a3, int a4);  // [61] 0x00610880  // __thiscall
    virtual void Draw_Post_Render(int a0, int a1, int a2, int a3, int a4, int a5);  // [62] 0x0060fb90  // __thiscall
    virtual void Draw_It(int a0, int a1);  // [63] 0x004a1cb0  // __stdcall
    virtual void Editor_Draw_It(int a0, int a1);  // [64] 0x00570990  // __stdcall
    virtual void Hidden();  // [65] 0x0060f590  // __thiscall
    virtual void Look(int a0, int a1);  // [66] 0x0061c1e0  // __thiscall
    virtual void Mark(int a0);  // [67] 0x0049c590  // __thiscall
    virtual void Get_Visual_Rect(int a0);  // [68] 0x0056f000  // __thiscall
    virtual void Get_Render_Rect(int a0);  // [69] 0x0056f160  // __thiscall
    virtual void Draw_Radial_Indicator();  // [70] 0x0040e690  // __cdecl
    virtual void Mark_For_Redraw();  // [71] 0x0056f680  // __cdecl
    virtual void Active_Click_With(int a0, int a1, int a2);  // [72] 0x0049f500  // __thiscall
    virtual void Active_Click_With(int a0, int a1);  // [73] 0x0049ee70  // __thiscall
    virtual void Clicked_As_Target(int a0);  // [74] 0x00612dd0  // __stdcall
    virtual void Select();  // [75] 0x00613f10  // __thiscall
    virtual void Unselect(int a0);  // [76] 0x0056eda0  // __thiscall
    virtual void In_Range(int a0, int a1);  // [77] 0x00611540  // __thiscall
    virtual void Weapon_Range(int a0);  // [78] 0x006164c0  // __stdcall
    virtual void Take_Damage(int a0, int a1, int a2, int a3, int a4, int a5);  // [79] 0x0049eda0  // __thiscall
    virtual void Scatter(int a0, int a1, int a2);  // [80] 0x0056ec90  // __stdcall
    virtual void Catch_Fire();  // [81] 0x0056eca0  // __cdecl
    virtual void Fire_Out();  // [82] 0x0056ecb0  // __cdecl
    virtual void Value();  // [83] 0x0061a4d0  // __thiscall
    virtual void Get_Mission();  // [84] 0x005468d0  // __cdecl
    virtual void Assign_Mission(int a0);  // [85] 0x00546d60  // __stdcall
    virtual void Per_Cell_Process(int a0, int a1, int a2, int a3, int a4, int a5, int a6);  // [86] 0x0049fae0  // __thiscall
    virtual void Who_Can_Build_Me(int a0, int a1);  // [87] 0x00570240  // __thiscall
    virtual void Receive_Message(int a0, int a1);  // [88] 0x004a03a0  // __thiscall
    virtual void Revealed(int a0);  // [89] 0x0060f490  // __thiscall
    virtual void Repair(int a0);  // [90] 0x0056ece0  // __stdcall
    virtual void Sell_Back(int a0);  // [91] 0x004a1160  // __thiscall
    virtual void Set_Waypoint_Path(int a0);  // [92] 0x004a2cd0  // __thiscall
    virtual void Move(int a0);  // [93] 0x0056ed00  // __thiscall
    virtual void Can_Enter_Cell(int a0, int a1, int a2, int a3, int a4);  // [94] 0x004a0e20  // __thiscall
    virtual void Can_Reach(int a0, int a1, int a2, int a3, int a4);  // [95] 0x004a0e70  // __stdcall, returns int
    virtual void Get_Coord(int a0);  // [96] 0x0040e6a0  // __stdcall
    virtual void Set_Coord(int a0);  // [97] 0x004a2170  // __thiscall
    virtual void Get_Cell(int a0);  // [98] 0x0040e6c0  // __stdcall
    virtual void Get_Cell_Ptr();  // [99] 0x00570ce0  // __cdecl
    virtual void Get_Target_Cell(int a0);  // [100] 0x00570d00  // __stdcall
    virtual void Get_Target_Cell_Ptr(int a0);  // [101] 0x00570d50  // __cdecl
    virtual void Get_Height_AGL();  // [102] 0x00570370  // __thiscall
    virtual void Set_Height_AGL(int a0);  // [103] 0x005703b0  // __thiscall
    virtual void Get_Height();  // [104] 0x00570360  // __cdecl
    virtual void Commence(int a0);  // [105] 0x00546d20  // __cdecl
    virtual void Ready_To_Commence();  // [106] 0x004a4b10  // __cdecl
    virtual void Do_MISSION_SLEEP();  // [107] 0x00546750  // __cdecl, returns int
    virtual void Do_MISSION_HARMLESS();  // [108] 0x00546760  // __cdecl, returns int
    virtual void Do_MISSION_AMBUSH();  // [109] 0x00546770  // __cdecl, returns int
    virtual void Do_MISSION_ATTACK();  // [110] 0x0049da30  // __thiscall
    virtual void Do_MISSION_CAPTURE(int a0);  // [111] 0x0049d8e0  // __thiscall
    virtual void Do_MISSION_GUARD(int a0);  // [112] 0x0049da90  // __thiscall
    virtual void Do_MISSION_GUARD_AREA(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9);  // [113] 0x0049e7f0  // __thiscall
    virtual void Do_MISSION_HARVEST();  // [114] 0x005467c0  // __cdecl, returns int
    virtual void Do_MISSION_HUNT();  // [115] 0x0049dbe0  // __thiscall
    virtual void Do_MISSION_MOVE();  // [116] 0x0049cfb0  // __thiscall, returns int
    virtual void Do_MISSION_RETREAT();  // [117] 0x004a1480  // __thiscall
    virtual void Do_MISSION_RETURN();  // [118] 0x00546800  // __cdecl, returns int
    virtual void Do_MISSION_STOP();  // [119] 0x00546810  // __cdecl, returns int
    virtual void Do_MISSION_UNLOAD();  // [120] 0x00546820  // __cdecl, returns int
    virtual void Do_MISSION_ENTER();  // [121] 0x004a0660  // __thiscall
    virtual void Do_MISSION_CONSTRUCTION();  // [122] 0x00546840  // __cdecl, returns int
    virtual void Do_MISSION_DECONSTRUCTION();  // [123] 0x00546850  // __cdecl, returns int
    virtual void Do_MISSION_REPAIR();  // [124] 0x00546860  // __cdecl, returns int
    virtual void Do_MISSION_MISSILE();  // [125] 0x00546870  // __cdecl, returns int
    virtual void Do_MISSION_OPEN();  // [126] 0x00546880  // __cdecl, returns int
    virtual void Do_MISSION_RESCUE();  // [127] 0x004a4420  // __thiscall
    virtual void Do_MISSION_PATROL();  // [128] 0x0049d080  // __thiscall, returns int
    virtual void Set_Mission(int a0);  // [129] 0x005468b0  // __stdcall
    virtual void Override_Mission(int a0, int a1, int a2);  // [130] 0x004a0330  // __thiscall
    virtual void Restore_Mission();  // [131] 0x004a0370  // __thiscall
    virtual void Has_Suspended_Mission();  // [132] 0x00547110  // __cdecl, returns int
    virtual void Transmit_Message(int a0, int a1);  // [133] 0x005a58a0  // __stdcall
    virtual void Transmit_Message(int a0, int a1, int a2);  // [134] 0x005a57d0  // __thiscall, returns int
    virtual void Is_Move_Override();  // [135] 0x0040e700  // __cdecl
    virtual void Is_Allowed_To_Recloak();  // [136] 0x004a2690  // __thiscall
    virtual void Can_Scatter();  // [137] 0x0060eb20  // __thiscall
    virtual void Is_In_Team();  // [138] 0x0040e7a0  // __cdecl, returns int
    virtual void Should_Self_Heal_Now();  // [139] 0x0061cb60  // __thiscall
    virtual void Is_Voxel_Loaded();  // [140] 0x00612de0  // __thiscall
    virtual void Is_Ready_To_Move();  // [141] 0x0040e710  // __cdecl
    virtual void Is_Ready_To_Cloak(int a0, int a1, int a2);  // [142] 0x00613d60  // __thiscall
    virtual void Should_Uncloak(int a0, int a1, int a2);  // [143] 0x00613c60  // __thiscall
    virtual void Turret_Facing(int a0);  // [144] 0x004a4b20  // __stdcall
    virtual void Is_Weapon_Equipped();  // [145] 0x00616450  // __cdecl
    virtual void Is_On_Elevation(int a0, int a1);  // [146] 0x0061cfa0  // __thiscall, returns int
    virtual void Tiberium_Load();  // [147] 0x0061b1e0  // __cdecl
    virtual void Weed_Load();  // [148] 0x0061d000  // __cdecl
    virtual void Pip_Count();  // [149] 0x0061b210  // __thiscall, returns int
    virtual void Refund_Amount(int a0);  // [150] 0x0061bf60  // __thiscall
    virtual void Risk();  // [151] 0x0061b1c0  // __cdecl
    virtual void Is_In_Same_Zone_As(int a0);  // [152] 0x004a2340  // __thiscall
    virtual void Barrel_Pitch(int a0, int a1);  // [153] 0x00614770  // __thiscall
    virtual void Is_In_Same_Zone(int a0);  // [154] 0x0049c620  // __thiscall
    virtual void How_Many_Survivors();  // [155] 0x0060ed00  // __thiscall
    virtual void Scatter_Incoming_Infantry();  // [156] 0x0061df10  // __cdecl
    virtual void Get_Collateral_Damage();  // [157] 0x0061dd70  // __cdecl
    virtual void Get_Z_Adjust(int a0, int a1, int a2);  // [158] 0x004a1a50  // __thiscall
    virtual void Get_Z_Gradient();  // [159] 0x004a1b30  // __thiscall
    virtual void Find_Docking_Bay(int a0, int a1, int a2);  // [160] 0x0061bd30  // __thiscall
    virtual void Find_Exit_Cell(int a0, int a1);  // [161] 0x0061bf10  // __stdcall
    virtual void Turret_Coord(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7);  // [162] 0x0060efa0  // __thiscall
    virtual void Desired_Load_Dir(int a0, int a1);  // [163] 0x0061b1f0  // __stdcall, returns int
    virtual void Fire_Direction(int a0);  // [164] 0x0061b2c0  // __stdcall
    virtual void Crew_Type();  // [165] 0x0061a450  // __thiscall, returns int
    virtual void Can_Attack_Now();  // [166] 0x00616160  // __cdecl
    virtual void Can_Deploy_Now(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7);  // [167] 0x006161b0  // __thiscall
    virtual void Rearm_Delay(int a0);  // [168] 0x006143a0  // __thiscall, returns int
    virtual void Threat_Range(int a0);  // [169] 0x0061a560  // __thiscall
    virtual void Is_Allowed_To_Leave_Map();  // [170] 0x004a13b0  // __thiscall
    virtual void Is_Radar_Visible(int a0);  // [171] 0x0061d9d0  // __thiscall
    virtual void Is_Sensed_By_Player();  // [172] 0x0061db90  // __cdecl
    virtual void Is_Renovator();  // [173] 0x0040e720  // __cdecl
    virtual void Advance_Waypoint_Path();  // [174] 0x004a49c0  // __thiscall
    virtual void Response_Select();  // [175] 0x0061b2e0  // __thiscall
    virtual void Response_Move();  // [176] 0x0061b340  // __thiscall
    virtual void Response_Attack();  // [177] 0x0061b3a0  // __thiscall
    virtual void Player_Assign_Mission(int a0);  // [178] 0x00615820  // __thiscall
    virtual void Reduce_Ammunition();  // [179] 0x0061dd50  // __cdecl
    virtual void Target_Something_Nearby(int a0, int a1);  // [180] 0x0061b400  // __thiscall
    virtual void Stun();  // [181] 0x0049de00  // __thiscall
    virtual void vfunc_182(int a0, int a1);  // [182] 0x006114b0  // __thiscall
    virtual void Death_Announcement(int a0);  // [183] 0x004a0bb0  // __stdcall
    virtual void Can_Fire(int a0, int a1);  // [184] 0x00613f90  // __thiscall, returns int
    virtual void Greatest_Threat(int a0, int a1, int a2);  // [185] 0x004a0c10  // __thiscall, returns int
    virtual void Assign_Target(int a0, int a1);  // [186] 0x006142e0  // __thiscall
    virtual void Fire_At(int a0, int a1, int a2, int a3, int a4, int a5, int a6);  // [187] 0x006148b0  // __thiscall
    virtual void Captured(int a0);  // [188] 0x004a27c0  // __thiscall
    virtual void Laser_Zap(int a0, int a1, int a2, int a3, int a4, int a5);  // [189] 0x00614530  // __thiscall
    virtual void Rock(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11);  // [190] 0x0061c360  // __thiscall
    virtual void Get_Class_Weapon_Data(int a0);  // [191] 0x0040e730  // __thiscall
    virtual void Is_Turret_Equipped();  // [192] 0x0040e780  // __cdecl
    virtual void Renovate();  // [193] 0x00616550  // __thiscall
    virtual void Cloaking_AI(int a0);  // [194] 0x00613760  // __thiscall
    virtual void Rocking_AI();  // [195] 0x0061c650  // __thiscall
    virtual void Try_To_Cloak(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8);  // [196] 0x0060f8f0  // __thiscall
    virtual void Draw_Action_Line(int a0, int a1, int a2, int a3, int a4, int a5, int a6);  // [197] 0x004a2880  // __thiscall
    virtual void Draw_Voxel(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8);  // [198] 0x004a19a0  // __thiscall
    virtual void Draw_Health_Bar_Old(int a0, int a1);  // [199] 0x00610870  // __stdcall
    virtual void Draw_Health_Bar(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11, int a12, int a13, int a14, int a15, int a16);  // [200] 0x00610c50  // __thiscall
    virtual void Draw_Pips(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11);  // [201] 0x0061b4f0  // __thiscall
    virtual void Draw_Text_Overlay(int a0, int a1, int a2);  // [202] 0x0061bc30  // __thiscall
    virtual void Do_Uncloak(int a0);  // [203] 0x00617b90  // __thiscall
    virtual void Do_Cloak(int a0);  // [204] 0x00617c30  // __thiscall
    virtual void Apparent_Brightness(int a0);  // [205] 0x0061d9a0  // __stdcall
    virtual void Is_Ready_To_Random_Animate();  // [206] 0x0061b490  // __cdecl
    virtual void Random_Animate();  // [207] 0x0040e790  // __cdecl
    virtual void Assign_Destination(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11, int a12);  // [208] 0x004a0830  // __thiscall
    virtual void Enter_Idle_Mode(int a0, int a1);  // [209] 0x0049f8e0  // __thiscall
    virtual void Radar_Track();  // [210] 0x0061d4c0  // __thiscall
    virtual void Radar_Untrack();  // [211] 0x0061d4f0  // __thiscall
    virtual void Plot_On_Radar();  // [212] 0x0061d520  // __cdecl
    virtual void Update_Radar_Position(int a0, int a1, int a2, int a3, int a4);  // [213] 0x0061df20  // __thiscall
    virtual void Should_Delete_Off_Map();  // [214] 0x004a2c50  // __thiscall
    virtual void Is_Considered_Slow();  // [215] 0x004a2800  // __thiscall
    virtual void Deploy_To_Fire();  // [216] 0x0040e7b0  // __cdecl
    virtual void Likely_Coord(int a0);  // [217] 0x004a11e0  // __stdcall
    virtual void On_Movement_Blocked();  // [218] 0x004a2850  // __thiscall
    virtual void JumpJet_To_Walk();  // [219] 0x0040e7d0  // __cdecl
    virtual void Start_Driver(int a0);  // [220] 0x0049dd90  // __thiscall
    virtual void Stop_Driver();  // [221] 0x0049dd60  // __thiscall
    virtual void Is_Docked_For_Repair();  // [222] 0x004a22c0  // __thiscall
    virtual void Draw_Object(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11, int a12, int a13, int a14);  // [223] 0x0040e7e0  // __stdcall
    virtual void Berzerk();  // [224] 0x0040e840  // __cdecl
    virtual void Start_Fear();  // [225] 0x0040e850  // __cdecl
    virtual void Stop_Fear();  // [226] 0x0040e860  // __cdecl
    virtual void Do_Idle(int a0);  // [227] 0x0040e870  // __stdcall
    virtual void Offload_Tiberium_Bail();  // [228] 0x004a0e10  // __cdecl, returns int
    virtual void Overrun_Square(int a0, int a1);  // [229] 0x0040e880  // __stdcall
    virtual void Current_Speed(int a0, int a1);  // [230] 0x004a1c20  // __thiscall
    virtual void Approach_Target(int a0, int a1, int a2, int a3);  // [231] 0x0049de30  // __thiscall
    virtual void Fixup_Path(int a0);  // [232] 0x0040e890  // __stdcall
    virtual void Set_Speed(int a0, int a1);  // [233] 0x0049c520  // __stdcall
    virtual void Stop_Movement_Animation();  // [234] 0x004a2330  // __cdecl
    virtual void Is_LZ_Clear(int a0);  // [235] 0x004a4140  // __thiscall

    FootClass();  // 0x0049c0f0
    // The destructor is declared non-virtual here: slot 0 holds a stub
    // shared with 66 other classes (0x00405790), not the destructor.
    ~FootClass();

    // ---- data members (offsets from ctor 0x0049c0f0) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    // vtable-pointer stores are omitted: the compiler supplies the vptr.
    // The machine block begins at +0x214: the compiled end of the
    // base sub-object region. Evidence: TechnoClass at +0x0 (ends +0x214).
    // 2 recovered offset(s) below +0x214 lie inside the base
    // sub-object region and are NOT declared here: they are declared and
    // compiler-checked in the base's own header -- +0x0, +0x4
    unsigned char pad_214[0x4];
    unsigned int m_218;  // +0x218 init 0xffffffff; 4-byte store: mov dword ptr [esi + 0x218], 0xffffffff
    unsigned short m_21c;  // +0x21c /* ? */; 2-byte store: mov word ptr [esi + 0x21c], bx
    unsigned short m_21e;  // +0x21e /* ? */; 2-byte store: mov word ptr [esi + 0x21e], bx
    unsigned short m_220;  // +0x220 /* ? */; 2-byte store: mov word ptr [esi + 0x220], bx
    unsigned short m_222;  // +0x222 /* ? */; 2-byte store: mov word ptr [esi + 0x222], bx
    unsigned char pad_224[0x4];
    unsigned int m_228;  // +0x228 /* ? */; 4-byte store: mov dword ptr [esi + 0x228], ebx
    unsigned int m_22c;  // +0x22c /* ? */; 4-byte store: mov dword ptr [esi + 0x22c], ebx
    unsigned int m_230;  // +0x230 /* ? */; 4-byte store: mov dword ptr [esi + 0x230], ebx
    unsigned short m_234;  // +0x234 /* ? */; 2-byte store: mov word ptr [esi + 0x234], bx
    unsigned short m_236;  // +0x236 /* ? */; 2-byte store: mov word ptr [esi + 0x236], bx
    unsigned short m_238;  // +0x238 /* ? */; 2-byte store: mov word ptr [esi + 0x238], bx
    unsigned short m_23a;  // +0x23a /* ? */; 2-byte store: mov word ptr [esi + 0x23a], bx
    unsigned int m_23c;  // +0x23c /* ? */; 4-byte store: mov dword ptr [esi + 0x23c], ebx
    unsigned int m_240;  // +0x240 /* ? */; 4-byte store: mov dword ptr [esi + 0x240], ebx
    unsigned int m_244;  // +0x244 /* ? */; 4-byte store: mov dword ptr [esi + 0x244], ebx
    unsigned int m_248;  // +0x248 /* ? */; 4-byte store: mov dword ptr [esi + 0x248], ebx
    unsigned int m_24c;  // +0x24c /* ? */; 4-byte store: mov dword ptr [esi + 0x24c], ebx
    unsigned int m_250;  // +0x250 /* ? */; 4-byte store: mov dword ptr [esi + 0x250], ebx
    unsigned int m_254;  // +0x254 init 0x3ff00000; 4-byte store: mov dword ptr [esi + 0x254], 0x3ff00000
    unsigned char pad_258[0x18];
    unsigned int m_270;  // +0x270 /* ? */; 4-byte store: mov dword ptr [esi + 0x270], ebx
    unsigned int m_274;  // +0x274 /* ? */; 4-byte store: mov dword ptr [esi + 0x274], ebx
    unsigned char pad_278[0x18];
    unsigned int m_290;  // +0x290 /* ? */; 4-byte store: mov dword ptr [esi + 0x290], ebx
    unsigned int m_294;  // +0x294 /* ? */; 4-byte store: mov dword ptr [esi + 0x294], ebx
    unsigned int m_298;  // +0x298 /* ? */; 4-byte store: mov dword ptr [esi + 0x298], ebx
    unsigned int m_29c;  // +0x29c init 0xffffffff; 4-byte store: mov dword ptr [esi + 0x29c], 0xffffffff
    unsigned char pad_2a0[0x5c];
    unsigned int m_2fc;  // +0x2fc /* ? */; 4-byte store: mov dword ptr [esi + 0x2fc], ecx
    unsigned char pad_300[0x4];
    unsigned int m_304;  // +0x304 /* ? */; 4-byte store: mov dword ptr [esi + 0x304], ebx
    unsigned int m_308;  // +0x308 init 0xa; 4-byte store: mov dword ptr [esi + 0x308], 0xa
    unsigned int m_30c;  // +0x30c /* ? */; 4-byte store: mov dword ptr [esi + 0x30c], edx
    unsigned char pad_310[0x4];
    unsigned int m_314;  // +0x314 /* ? */; 4-byte store: mov dword ptr [esi + 0x314], ebx
    unsigned int m_318;  // +0x318 /* ? */; 4-byte store: mov dword ptr [esi + 0x318], eax
    unsigned char pad_31c[0x4];
    unsigned int m_320;  // +0x320 /* ? */; 4-byte store: mov dword ptr [esi + 0x320], ebx
    unsigned int m_324;  // +0x324 /* ? */; 4-byte store: mov dword ptr [esi + 0x324], ebx
    unsigned int m_328;  // +0x328 /* ? */; 4-byte store: mov dword ptr [esi + 0x328], edx
    unsigned char pad_32c[0x8];
    unsigned char m_334;  // +0x334 init 0xff; 1-byte store: mov byte ptr [esi + 0x334], 0xff
    unsigned char m_335;  // +0x335 /* ? */; 1-byte store: mov byte ptr [esi + 0x335], bl
    unsigned char m_336;  // +0x336 /* ? */; 1-byte store: mov byte ptr [esi + 0x336], bl
    unsigned char m_337;  // +0x337 /* ? */; 1-byte store: mov byte ptr [esi + 0x337], bl
    unsigned char m_338;  // +0x338 /* ? */; 1-byte store: mov byte ptr [esi + 0x338], bl
    unsigned char m_339;  // +0x339 /* ? */; 1-byte store: mov byte ptr [esi + 0x339], bl
    unsigned char m_33a;  // +0x33a /* ? */; 1-byte store: mov byte ptr [esi + 0x33a], bl
    unsigned char m_33b;  // +0x33b /* ? */; 1-byte store: mov byte ptr [esi + 0x33b], bl
    unsigned char m_33c;  // +0x33c /* ? */; 1-byte store: mov byte ptr [esi + 0x33c], bl
    unsigned char m_33d;  // +0x33d /* ? */; 1-byte store: mov byte ptr [esi + 0x33d], bl
    unsigned char m_33e;  // +0x33e /* ? */; 1-byte store: mov byte ptr [esi + 0x33e], bl
    unsigned char m_33f;  // +0x33f /* ? */; 1-byte store: mov byte ptr [esi + 0x33f], bl
    unsigned char m_340;  // +0x340 /* ? */; 1-byte store: mov byte ptr [esi + 0x340], bl
    unsigned char m_341;  // +0x341 /* ? */; 1-byte store: mov byte ptr [esi + 0x341], bl
    unsigned char m_342;  // +0x342 /* ? */; 1-byte store: mov byte ptr [esi + 0x342], bl
    unsigned char m_343;  // +0x343 /* ? */; 1-byte store: mov byte ptr [esi + 0x343], bl
    unsigned char m_344;  // +0x344 /* ? */; 1-byte store: mov byte ptr [esi + 0x344], bl
    unsigned char m_345;  // +0x345 init 0x1; 1-byte store: mov byte ptr [esi + 0x345], 1
    unsigned char m_346;  // +0x346 /* ? */; 1-byte store: mov byte ptr [esi + 0x346], bl
    unsigned char m_347;  // +0x347 /* ? */; 1-byte store: mov byte ptr [esi + 0x347], bl
};
