#pragma once
// ==========================================================================
// AircraftClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class AircraftClass
// vtable       : 0x00698d50  (236 slots)
// constructor  : 0x00407d60
// vtable store : 0x00407dca  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "FootClass.h"

class AircraftClass : public FootClass, public IFlyControl
{
public:
    // ---- vtable 0x00698d50 (236 slots) ----
    virtual void QueryInterface(int a0, int a1, int a2);  // [0] 0x00408050  // __stdcall, returns ?
    virtual void AddRef(int a0);  // [1] 0x004080b0  // __stdcall, returns ?
    virtual void Release(int a0);  // [2] 0x004080c0  // __stdcall, returns ?
    virtual void vfunc_3(int a0, int a1);  // [3] 0x0040e8c0  // __stdcall, returns int
    virtual void IsDirty(int a0);  // [4] 0x00405990  // __stdcall, returns ?
    virtual void Load(int a0, int a1, int a2, int a3, int a4);  // [5] 0x0040df60  // returns void*
    virtual void vfunc_6(int a0, int a1, int a2);  // [6] 0x0040e160  // __stdcall, returns ?
    virtual void GetSizeMax(int a0, int a1);  // [7] 0x00405910  // __stdcall, returns int
    virtual void Init();  // [9] 0x00407fb0  // __thiscall, returns ?
    virtual void Detach(int a0, int a1);  // [10] 0x0040e230  // __thiscall, returns ?
    virtual void What_Am_I();  // [11] 0x0040e8b0  // __cdecl, returns int
    virtual void Fetch_Object_Size();  // [12] 0x0040e8a0  // __cdecl, returns int
    virtual void Compute_CRC(int a0);  // [13] 0x0040e1b0  // __thiscall, returns ?
    virtual void Owner();  // [14] 0x00612db0  // __thiscall, returns ?
    virtual void Owner_HouseClass();  // [15] 0x00612dc0  // __thiscall, returns ?
    virtual void Fetch_Heap_ID();  // [16] 0x004059f0  // __cdecl, returns int
    virtual void Is_Inactive();  // [17] 0x00570a20  // __thiscall, returns ?
    virtual void Center_Coord(int a0);  // [18] 0x00570960  // __thiscall, returns ?
    virtual void Destination_Coord(int a0);  // [19] 0x004a26e0  // __thiscall, returns ?
    virtual void On_Ground();  // [20] 0x00570e80  // __thiscall, returns char
    virtual void In_Air();  // [21] 0x004a4a30  // __thiscall, returns ?
    virtual void As_Coord(int a0);  // [22] 0x00405a80  // __thiscall, returns ?
    virtual void AI();  // [23] 0x00408850  // __thiscall, returns ?
    virtual void Is_Players_Army();  // [24] 0x0060eb70  // __thiscall, returns ?
    virtual void Visual_Character(int a0, int a1);  // [25] 0x004a1690  // __thiscall, returns ?
    virtual void Get_Image_Data();  // [26] 0x0056e820  // __thiscall, returns int
    virtual void What_Action(int a0, int a1, int a2);  // [27] 0x0040afe0  // __thiscall, returns int
    virtual void What_Action(int a0, int a1);  // [28] 0x0040ade0  // __thiscall, returns int
    virtual void In_Which_Layer();  // [29] 0x0040d9b0  // __thiscall, returns ?
    virtual void Not_Underground();  // [30] 0x00570ef0  // __thiscall, returns ?
    virtual void Considered_Vehicle(int a0, int a1, int a2);  // [31] 0x0040e430  // __thiscall, returns ?
    virtual void Techno_Type_Class();  // [32] 0x0060eb10  // __thiscall, returns ?
    virtual void Class_Of();  // [33] 0x0040e930  // __thiscall, returns ?
    virtual void Get_Ownable();  // [34] 0x0061b1b0  // __thiscall, returns ?
    virtual void Full_Name();  // [35] 0x0040e900  // __thiscall, returns int
    virtual void Can_Repair();  // [36] 0x00616470  // __thiscall, returns int
    virtual void Can_Demolish();  // [37] 0x004a1080  // __thiscall, returns char
    virtual void Can_Player_Fire();  // [38] 0x00616410  // __thiscall, returns char
    virtual void Can_Player_Move();  // [39] 0x00616130  // __thiscall, returns char
    virtual void Target_Coord(int a0);  // [40] 0x0040e5d0  // __thiscall, returns ?
    virtual void Docking_Coord(int a0);  // [41] 0x0040e600  // __thiscall, returns ?
    virtual void Render_Coord(int a0);  // [42] 0x0040e630  // __thiscall, returns ?
    virtual void Fire_Coord(int a0, int a1);  // [43] 0x0060edd0  // __thiscall, returns ?
    virtual void Exit_Coord(int a0);  // [44] 0x0040e660  // __thiscall, returns ?
    virtual void Sort_Y();  // [45] 0x00570ed0  // __thiscall, returns ?
    virtual void Is_Moving_Onto_Bridge();  // [46] 0x004a4120  // __thiscall, returns int
    virtual void Occupies_Cells();  // [47] 0x0040e7c0  // __thiscall, returns char
    virtual void Limbo();  // [48] 0x004a1cc0  // __thiscall, returns ?
    virtual void Unlimbo(int a0, int a1);  // [49] 0x004080d0  // __thiscall, returns char
    virtual void Detach_All(int a0);  // [50] 0x004a0a00  // __thiscall, returns ?
    virtual void Record_The_Kill(int a0, int a1, int a2, int a3, int a4);  // [51] 0x00617760  // __thiscall, returns ?
    virtual void Paradrop(int a0);  // [52] 0x005700c0  // __thiscall, returns ?
    virtual void Fall_From_Height();  // [53] 0x0056ea70  // __thiscall, returns ?
    virtual void Set_Occupy_Bit(int a0);  // [54] 0x00570480  // __stdcall, returns int
    virtual void Clear_Occupy_Bit(int a0);  // [55] 0x00570500  // __stdcall, returns int
    virtual void Delete_Me();  // [56] 0x004a4a10  // __thiscall, returns void
    virtual void Do_Shimmer();  // [57] 0x00617ce0  // __thiscall, returns ?
    virtual void Exit_Object(int a0);  // [58] 0x00408fd0  // __thiscall, returns int
    virtual void Render(int a0, int a1, int a2);  // [59] 0x0056f480  // __thiscall, returns int
    virtual void Occupy_List();  // [60] 0x005701b0  // __thiscall, returns short
    virtual void Draw_Pre_Render();  // [61] 0x00610880  // __thiscall, returns ?
    virtual void Draw_Post_Render(int a0, int a1);  // [62] 0x0060fb90  // __thiscall, returns ?
    virtual void Draw_It(int a0, int a1, int a2);  // [63] 0x00408260  // __thiscall, returns ?
    virtual void Editor_Draw_It(int a0, int a1);  // [64] 0x00570990  // __thiscall, returns ?
    virtual void Hidden();  // [65] 0x0060f590  // __thiscall, returns char
    virtual void Look(int a0, int a1);  // [66] 0x0040d9e0  // __thiscall, returns ?
    virtual void Mark(int a0);  // [67] 0x0049c590  // __thiscall, returns char
    virtual void Get_Visual_Rect(int a0);  // [68] 0x0056f000  // __thiscall, returns ?
    virtual void Get_Render_Rect(int a0);  // [69] 0x0056f160  // __thiscall, returns ?
    virtual void Draw_Radial_Indicator();  // [70] 0x0040e690  // __cdecl, returns void
    virtual void Mark_For_Redraw();  // [71] 0x0056f680  // __thiscall, returns char
    virtual void Active_Click_With(int a0, int a1, int a2);  // [72] 0x0040ad40  // __stdcall, returns ?
    virtual void vfunc_73(int a0, int a1, int a2);  // [73] 0x0040ac90  // __thiscall, returns char
    virtual void Clicked_As_Target(int a0);  // [74] 0x00612dd0  // __thiscall, returns ?
    virtual void Select();  // [75] 0x00613f10  // __thiscall, returns char
    virtual void Unselect();  // [76] 0x0056eda0  // __thiscall, returns int
    virtual void In_Range(int a0, int a1);  // [77] 0x00611540  // __thiscall, returns ?
    virtual void Weapon_Range(int a0);  // [78] 0x006164c0  // __thiscall, returns ?
    virtual void Take_Damage(int a0, int a1, int a2, int a3, int a4, int a5);  // [79] 0x00409890  // __thiscall, returns int
    virtual void Scatter(int a0, int a1, int a2);  // [80] 0x0040d1b0  // __thiscall, returns ?
    virtual void Catch_Fire();  // [81] 0x0056eca0  // __cdecl, returns int
    virtual void Fire_Out();  // [82] 0x0056ecb0  // __cdecl, returns void
    virtual void Value();  // [83] 0x0061a4d0  // __thiscall, returns int
    virtual void Get_Mission();  // [84] 0x005468d0  // __thiscall, returns ?
    virtual void Assign_Mission(int a0);  // [85] 0x00546d60  // __thiscall, returns int
    virtual void Per_Cell_Process(int a0);  // [86] 0x0049fae0  // __thiscall, returns ?
    virtual void Who_Can_Build_Me(int a0, int a1);  // [87] 0x00570240  // __thiscall, returns ?
    virtual void Receive_Message(int a0, int a1, int a2);  // [88] 0x0040be00  // __thiscall, returns int
    virtual void Revealed(int a0);  // [89] 0x0060f490  // __thiscall, returns int
    virtual void Repair(int a0);  // [90] 0x0056ece0  // __stdcall, returns void
    virtual void Sell_Back(int a0);  // [91] 0x004a1160  // __thiscall, returns ?
    virtual void Set_Waypoint_Path(int a0, int a1);  // [92] 0x004a2cd0  // __thiscall, returns short
    virtual void Move(int a0);  // [93] 0x0056ed00  // __thiscall, returns ?
    virtual void Can_Enter_Cell(int a0, int a1, int a2, int a3, int a4);  // [94] 0x0040c350  // __thiscall, returns int
    virtual void Can_Reach(int a0, int a1, int a2, int a3, int a4);  // [95] 0x004a0e70  // __stdcall, returns int
    virtual void Get_Coord(int a0);  // [96] 0x0040e6a0  // __thiscall, returns ?
    virtual void Set_Coord(int a0);  // [97] 0x004a2170  // __thiscall, returns ?
    virtual void Get_Cell(int a0);  // [98] 0x0040e6c0  // __thiscall, returns ?
    virtual void Get_Cell_Ptr();  // [99] 0x00570ce0  // __thiscall, returns ?
    virtual void Get_Target_Cell(int a0);  // [100] 0x00570d00  // __thiscall, returns ?
    virtual void Get_Target_Cell_Ptr();  // [101] 0x00570d50  // __thiscall, returns ?
    virtual void Get_Height_AGL();  // [102] 0x00570370  // __thiscall, returns ?
    virtual void Set_Height_AGL(int a0);  // [103] 0x005703b0  // __thiscall, returns int
    virtual void Get_Height();  // [104] 0x00570360  // __thiscall, returns ?
    virtual void Commence();  // [105] 0x0040e3d0  // __thiscall, returns char
    virtual void Ready_To_Commence();  // [106] 0x0040e180  // __thiscall, returns ?
    virtual void Do_MISSION_SLEEP();  // [107] 0x00546750  // __cdecl, returns int
    virtual void Do_MISSION_HARMLESS();  // [108] 0x00546760  // __cdecl, returns int
    virtual void Do_MISSION_AMBUSH();  // [109] 0x00546770  // __cdecl, returns int
    virtual void Do_MISSION_ATTACK();  // [110] 0x0040b040  // __thiscall, returns int
    virtual void Do_MISSION_CAPTURE(int a0);  // [111] 0x0049d8e0  // __thiscall, returns int
    virtual void Do_MISSION_GUARD();  // [112] 0x0040d1e0  // __thiscall, returns ?
    virtual void Do_MISSION_GUARD_AREA();  // [113] 0x0040d530  // __thiscall, returns int
    virtual void Do_MISSION_HARVEST();  // [114] 0x005467c0  // __cdecl, returns int
    virtual void Do_MISSION_HUNT();  // [115] 0x00408740  // __thiscall, returns int
    virtual void Do_MISSION_MOVE();  // [116] 0x00409a00  // __thiscall, returns ?
    virtual void Do_MISSION_RETREAT();  // [117] 0x00408fc0  // __cdecl, returns int
    virtual void Do_MISSION_RETURN();  // [118] 0x00546800  // __cdecl, returns int
    virtual void Do_MISSION_STOP();  // [119] 0x00546810  // __cdecl, returns int
    virtual void Do_MISSION_UNLOAD();  // [120] 0x00408aa0  // __thiscall, returns ?
    virtual void Do_MISSION_ENTER();  // [121] 0x0040c8c0  // __thiscall, returns int
    virtual void Do_MISSION_CONSTRUCTION();  // [122] 0x00546840  // __cdecl, returns int
    virtual void Do_MISSION_DECONSTRUCTION();  // [123] 0x00546850  // __cdecl, returns int
    virtual void Do_MISSION_REPAIR();  // [124] 0x00546860  // __cdecl, returns int
    virtual void Do_MISSION_MISSILE();  // [125] 0x00546870  // __cdecl, returns int
    virtual void Do_MISSION_OPEN();  // [126] 0x00546880  // __cdecl, returns int
    virtual void Do_MISSION_RESCUE();  // [127] 0x004a4420  // __thiscall, returns int
    virtual void Do_MISSION_PATROL();  // [128] 0x0040a510  // __thiscall, returns ?
    virtual void Set_Mission(int a0);  // [129] 0x005468b0  // __thiscall, returns ?
    virtual void Override_Mission(int a0, int a1, int a2);  // [130] 0x004a0330  // __thiscall, returns ?
    virtual void Restore_Mission();  // [131] 0x004a0370  // __thiscall, returns char
    virtual void Has_Suspended_Mission();  // [132] 0x00547110  // __thiscall, returns ?
    virtual void Transmit_Message(int a0, int a1);  // [133] 0x005a58a0  // __thiscall, returns ?
    virtual void Transmit_Message(int a0, int a1, int a2);  // [134] 0x005a57d0  // __thiscall, returns int
    virtual void Is_Move_Override();  // [135] 0x0040e700  // __cdecl, returns int
    virtual void Is_Allowed_To_Recloak();  // [136] 0x004a2690  // __thiscall, returns char
    virtual void Can_Scatter();  // [137] 0x0060eb20  // __thiscall, returns char
    virtual void Is_In_Team();  // [138] 0x0040e7a0  // __thiscall, returns ?
    virtual void Should_Self_Heal_Now();  // [139] 0x0061cb60  // __thiscall, returns int
    virtual void Is_Voxel_Loaded();  // [140] 0x00612de0  // __thiscall, returns int
    virtual void Is_Ready_To_Move();  // [141] 0x0040e710  // __cdecl, returns char
    virtual void Is_Ready_To_Cloak();  // [142] 0x00613d60  // __thiscall, returns int
    virtual void Should_Uncloak();  // [143] 0x00613c60  // __thiscall, returns int
    virtual void Turret_Facing(int a0);  // [144] 0x0040e910  // __thiscall, returns ?
    virtual void Is_Weapon_Equipped();  // [145] 0x00616450  // __thiscall, returns ?
    virtual void Is_On_Elevation();  // [146] 0x0061cfa0  // __thiscall, returns int
    virtual void Tiberium_Load();  // [147] 0x0061b1e0  // __cdecl, returns double
    virtual void Weed_Load();  // [148] 0x0061d000  // __cdecl, returns double
    virtual void Pip_Count();  // [149] 0x0061b210  // __thiscall, returns int
    virtual void Refund_Amount();  // [150] 0x0061bf60  // __thiscall, returns ?
    virtual void Risk();  // [151] 0x0061b1c0  // __thiscall, returns ?
    virtual void Is_In_Same_Zone_As(int a0);  // [152] 0x004a2340  // __thiscall, returns ?
    virtual void Barrel_Pitch(int a0, int a1);  // [153] 0x00614770  // __thiscall, returns ?
    virtual void Is_In_Same_Zone(int a0);  // [154] 0x0049c620  // __thiscall, returns char
    virtual void How_Many_Survivors();  // [155] 0x0060ed00  // __thiscall, returns ?
    virtual void Scatter_Incoming_Infantry();  // [156] 0x0061df10  // __cdecl, returns void
    virtual void Get_Collateral_Damage();  // [157] 0x0061dd70  // __thiscall, returns ?
    virtual void Get_Z_Adjust();  // [158] 0x004a1a50  // __thiscall, returns int
    virtual void Get_Z_Gradient();  // [159] 0x004a1b30  // __thiscall, returns ?
    virtual void Find_Docking_Bay(int a0, int a1, int a2, int a3, int a4, int a5, int a6);  // [160] 0x0061bd30  // __thiscall, returns char
    virtual void Find_Exit_Cell(int a0, int a1);  // [161] 0x0061bf10  // __thiscall, returns ?
    virtual void Turret_Coord(int a0, int a1);  // [162] 0x0060efa0  // __thiscall, returns ?
    virtual void Desired_Load_Dir(int a0, int a1);  // [163] 0x0040c240  // __thiscall, returns ?
    virtual void Fire_Direction(int a0);  // [164] 0x0040cfd0  // __thiscall, returns ?
    virtual void Crew_Type();  // [165] 0x0061a450  // __thiscall, returns int
    virtual void Can_Attack_Now();  // [166] 0x00616160  // __cdecl, returns int
    virtual void Can_Deploy_Now(int a0, int a1, int a2);  // [167] 0x006161b0  // __thiscall, returns int
    virtual void Rearm_Delay(int a0);  // [168] 0x006143a0  // __thiscall, returns int
    virtual void Threat_Range(int a0);  // [169] 0x0061a560  // __thiscall, returns int
    virtual void Is_Allowed_To_Leave_Map();  // [170] 0x004a13b0  // __thiscall, returns int
    virtual void Is_Radar_Visible(int a0);  // [171] 0x0061d9d0  // __thiscall, returns char
    virtual void Is_Sensed_By_Player();  // [172] 0x0061db90  // __thiscall, returns ?
    virtual void Is_Renovator();  // [173] 0x0040e720  // __cdecl, returns int
    virtual void Advance_Waypoint_Path();  // [174] 0x004a49c0  // __thiscall, returns ?
    virtual void Response_Select();  // [175] 0x0061b2e0  // __thiscall, returns int
    virtual void Response_Move();  // [176] 0x0061b340  // __thiscall, returns ?
    virtual void Response_Attack();  // [177] 0x0061b3a0  // __thiscall, returns ?
    virtual void Player_Assign_Mission(int a0, int a1, int a2);  // [178] 0x0040ad60  // __thiscall, returns ?
    virtual void Reduce_Ammunition();  // [179] 0x0040e420  // __cdecl, returns void
    virtual void Target_Something_Nearby(int a0, int a1);  // [180] 0x0061b400  // __thiscall, returns ?
    virtual void Stun();  // [181] 0x0049de00  // __thiscall, returns ?
    virtual void vfunc_182(int a0, int a1);  // [182] 0x006114b0  // __thiscall, returns char
    virtual void Death_Announcement(int a0);  // [183] 0x004a0bb0  // __thiscall, returns short
    virtual void Can_Fire(int a0, int a1);  // [184] 0x0040d5a0  // __thiscall, returns int
    virtual void Greatest_Threat(int a0, int a1, int a2);  // [185] 0x004a0c10  // __thiscall, returns int
    virtual void Assign_Target(int a0);  // [186] 0x006142e0  // __thiscall, returns ?
    virtual void Fire_At(int a0, int a1);  // [187] 0x00409220  // __thiscall, returns int
    virtual void Captured(int a0);  // [188] 0x004a27c0  // __thiscall, returns char
    virtual void Laser_Zap(int a0, int a1, int a2, int a3);  // [189] 0x00614530  // __thiscall, returns void*
    virtual void Rock(int a0, int a1);  // [190] 0x0061c360  // __thiscall, returns ?
    virtual void Get_Class_Weapon_Data(int a0);  // [191] 0x0040e730  // __thiscall, returns ?
    virtual void Is_Turret_Equipped();  // [192] 0x0040e780  // __thiscall, returns char
    virtual void Renovate();  // [193] 0x00616550  // __thiscall, returns int
    virtual void Cloaking_AI(int a0);  // [194] 0x00613760  // __thiscall, returns ?
    virtual void Rocking_AI();  // [195] 0x0061c650  // __thiscall, returns char
    virtual void Try_To_Cloak();  // [196] 0x0060f8f0  // __thiscall, returns ?
    virtual void Draw_Action_Line();  // [197] 0x004a2880  // __thiscall, returns ?
    virtual void Draw_Voxel(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8);  // [198] 0x004a19a0  // __thiscall, returns ?
    virtual void Draw_Health_Bar_Old(int a0, int a1);  // [199] 0x00610870  // __stdcall, returns void
    virtual void Draw_Health_Bar(int a0, int a1);  // [200] 0x00610c50  // __thiscall, returns int
    virtual void Draw_Pips(int a0, int a1, int a2);  // [201] 0x0061b4f0  // __thiscall, returns ?
    virtual void Draw_Text_Overlay(int a0, int a1, int a2);  // [202] 0x0061bc30  // __thiscall, returns ?
    virtual void Do_Uncloak(int a0);  // [203] 0x00617b90  // __thiscall, returns void*
    virtual void Do_Cloak(int a0);  // [204] 0x00617c30  // __thiscall, returns char
    virtual void Apparent_Brightness(int a0);  // [205] 0x0061d9a0  // __thiscall, returns int
    virtual void Is_Ready_To_Random_Animate();  // [206] 0x0061b490  // __thiscall, returns char
    virtual void Random_Animate();  // [207] 0x0040e790  // __cdecl, returns int
    virtual void Assign_Destination(int a0, int a1);  // [208] 0x0040d640  // __thiscall, returns ?
    virtual void Enter_Idle_Mode(int a0, int a1);  // [209] 0x0040a930  // __thiscall, returns int
    virtual void Radar_Track();  // [210] 0x0061d4c0  // __thiscall, returns ?
    virtual void Radar_Untrack();  // [211] 0x0061d4f0  // __thiscall, returns ?
    virtual void Plot_On_Radar();  // [212] 0x0061d520  // __thiscall, returns ?
    virtual void Update_Radar_Position(int a0, int a1, int a2);  // [213] 0x0061df20  // __thiscall, returns ?
    virtual void Should_Delete_Off_Map();  // [214] 0x0040e3e0  // __thiscall, returns char
    virtual void Is_Considered_Slow();  // [215] 0x004a2800  // __thiscall, returns char
    virtual void Deploy_To_Fire();  // [216] 0x0040e7b0  // __cdecl, returns int
    virtual void Likely_Coord(int a0);  // [217] 0x004a11e0  // __thiscall, returns ?
    virtual void On_Movement_Blocked();  // [218] 0x004a2850  // __thiscall, returns ?
    virtual void JumpJet_To_Walk();  // [219] 0x0040e7d0  // __cdecl, returns int
    virtual void Start_Driver(int a0);  // [220] 0x0049dd90  // __thiscall, returns int
    virtual void Stop_Driver();  // [221] 0x0049dd60  // __thiscall, returns int
    virtual void Is_Docked_For_Repair();  // [222] 0x004a22c0  // __thiscall, returns ?
    virtual void Draw_Object(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11, int a12, int a13, int a14);  // [223] 0x0040e7e0  // __stdcall, returns ?
    virtual void Berzerk();  // [224] 0x0040e840  // __cdecl, returns void
    virtual void Start_Fear();  // [225] 0x0040e850  // __cdecl, returns void
    virtual void Stop_Fear();  // [226] 0x0040e860  // __cdecl, returns void
    virtual void Do_Idle(int a0);  // [227] 0x0040e870  // __stdcall, returns void
    virtual void Offload_Tiberium_Bail();  // [228] 0x004a0e10  // __cdecl, returns int
    virtual void Overrun_Square(int a0, int a1);  // [229] 0x0040e880  // __stdcall, returns void
    virtual void Current_Speed();  // [230] 0x004a1c20  // __thiscall, returns int
    virtual void Approach_Target(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11, int a12, int a13, int a14, int a15, int a16, int a17, int a18, int a19, int a20, int a21, int a22, int a23, int a24, int a25, int a26, int a27, int a28, int a29, int a30, int a31, int a32, int a33, int a34, int a35, int a36, int a37, int a38, int a39, int a40, int a41, int a42, int a43, int a44, int a45, int a46, int a47, int a48, int a49, int a50, int a51, int a52, int a53, int a54, int a55, int a56, int a57, int a58, int a59, int a60, int a61, int a62, int a63, int a64, int a65, int a66, int a67, int a68, int a69);  // [231] 0x0049de30  // __thiscall, returns ?
    virtual void Fixup_Path(int a0);  // [232] 0x0040e890  // __stdcall, returns void
    virtual void Set_Speed(int a0, int a1);  // [233] 0x0049c520  // __thiscall, returns double
    virtual void Stop_Movement_Animation();  // [234] 0x004a2330  // __cdecl, returns void
    virtual void Is_LZ_Clear(int a0);  // [235] 0x004a4140  // __thiscall, returns int

    AircraftClass();  // 0x00407d60
    virtual ~AircraftClass();

    // ---- data members (offsets from ctor 0x00407d60) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    // vtable-pointer stores are omitted: the compiler supplies the vptr.
    // The machine block begins at +0x34c: the compiled end of the
    // base sub-object region. Evidence: FootClass at +0x0 (ends +0x348), IFlyControl at +0x348 (ends +0x34c).
    // 2 recovered offset(s) below +0x34c lie inside the base
    // sub-object region and are NOT declared here: they are declared and
    // compiler-checked in the base's own header -- +0x0, +0x348
    unsigned int m_34c;  // +0x34c /* ? */; 4-byte store: mov dword ptr [esi + 0x34c], ecx
    unsigned char m_350;  // +0x350 zero-initialised; 1-byte store: mov byte ptr [esi + 0x350], 0
    unsigned char m_351;  // +0x351 zero-initialised; 1-byte store: mov byte ptr [esi + 0x351], 0
    unsigned char m_352;  // +0x352 zero-initialised; 1-byte store: mov byte ptr [esi + 0x352], 0
    unsigned char m_353;  // +0x353 zero-initialised; 1-byte store: mov byte ptr [esi + 0x353], 0
    unsigned char m_354;  // +0x354 zero-initialised; 1-byte store: mov byte ptr [esi + 0x354], 0
    unsigned char pad_355[0x3];
    unsigned int m_358;  // +0x358 /* ? */; 4-byte store: mov dword ptr [esi + 0x358], edx
    unsigned char pad_35c[0x4];
    unsigned int m_360;  // +0x360 zero-initialised; 4-byte store: mov dword ptr [esi + 0x360], 0
    unsigned int m_364;  // +0x364 /* ? */; 4-byte store: mov dword ptr [esi + 0x364], eax
    unsigned char m_368;  // +0x368 /* ? */; 1-byte store: mov byte ptr [esi + 0x368], al
};
