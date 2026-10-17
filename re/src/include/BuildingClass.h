#pragma once
// ==========================================================================
// BuildingClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class BuildingClass
// vtable       : 0x0069a2d0  (221 slots)
// constructor  : 0x00425530
// vtable store : 0x00425569  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "TechnoClass.h"

class BuildingClass : public TechnoClass
{
public:
    // ---- vtable 0x0069a2d0 (221 slots) ----
    virtual void QueryInterface(int a0, int a1, int a2);  // [0] 0x00405790  // __stdcall, returns int
    virtual void AddRef(int a0);  // [1] 0x00405830  // __stdcall, returns int
    virtual void Release(int a0);  // [2] 0x00405840  // __stdcall, returns int
    virtual void vfunc_3(int a0, int a1);  // [3] 0x0043a180  // __stdcall, returns int
    virtual void IsDirty(int a0);  // [4] 0x00405990  // __stdcall
    virtual void Load(int a0, int a1);  // [5] 0x00436cd0  // __stdcall
    virtual void vfunc_6(int a0, int a1, int a2);  // [6] 0x00436e80  // __stdcall, returns int
    virtual void GetSizeMax(int a0, int a1);  // [7] 0x00405910  // __stdcall, returns int
    virtual void Init();  // [9] 0x0042adc0  // __thiscall
    virtual void Detach(int a0, int a1);  // [10] 0x00432dd0  // __thiscall
    virtual void What_Am_I();  // [11] 0x0043a1c0  // __cdecl, returns int
    virtual void Fetch_Object_Size();  // [12] 0x0043a170  // __cdecl, returns int
    virtual void Compute_CRC(int a0);  // [13] 0x00436eb0  // __thiscall
    virtual void Owner();  // [14] 0x00612db0  // __cdecl
    virtual void Owner_HouseClass();  // [15] 0x00612dc0  // __cdecl
    virtual void Fetch_Heap_ID();  // [16] 0x004059f0  // __cdecl, returns int
    virtual void Is_Inactive();  // [17] 0x00570a20  // __cdecl, returns int
    virtual void Center_Coord(int a0);  // [18] 0x0042e3e0  // __thiscall
    virtual void Destination_Coord(int a0);  // [19] 0x0042e540
    virtual void On_Ground();  // [20] 0x00570e80  // __cdecl
    virtual void In_Air();  // [21] 0x00570ea0  // __cdecl
    virtual void As_Coord(int a0);  // [22] 0x00405a80  // __stdcall
    virtual void AI(int a0);  // [23] 0x00428c30  // __thiscall
    virtual void Is_Players_Army();  // [24] 0x0060eb70  // __thiscall
    virtual void Visual_Character(int a0, int a1);  // [25] 0x004370c0  // __stdcall, returns int
    virtual void Get_Image_Data(int a0, int a1);  // [26] 0x00434c20  // __thiscall
    virtual void What_Action(int a0, int a1, int a2);  // [27] 0x0042e030  // __thiscall, returns int
    virtual void What_Action(int a0, int a1);  // [28] 0x0042dd30  // __thiscall, returns int
    virtual void In_Which_Layer();  // [29] 0x0056eb60  // __cdecl
    virtual void Not_Underground();  // [30] 0x00570ef0  // __cdecl
    virtual void Considered_Vehicle();  // [31] 0x00439a90  // __cdecl
    virtual void Techno_Type_Class();  // [32] 0x0060eb10  // __thiscall
    virtual void Class_Of();  // [33] 0x0043a1e0  // __cdecl
    virtual void Get_Ownable();  // [34] 0x0061b1b0  // __cdecl
    virtual void Full_Name();  // [35] 0x0043a1d0  // __cdecl
    virtual void Can_Repair();  // [36] 0x004359c0  // __thiscall
    virtual void Can_Demolish();  // [37] 0x0042f030  // __thiscall
    virtual void Can_Player_Fire();  // [38] 0x00616410  // __thiscall
    virtual void Can_Player_Move();  // [39] 0x00433440  // __thiscall
    virtual void Target_Coord(int a0);  // [40] 0x00433ef0  // __stdcall
    virtual void Docking_Coord(int a0);  // [41] 0x0042e450  // __thiscall
    virtual void Render_Coord(int a0);  // [42] 0x0043a1f0  // __stdcall
    virtual void Fire_Coord(int a0, int a1);  // [43] 0x00436a30  // __thiscall
    virtual void Exit_Coord(int a0);  // [44] 0x004334c0  // __thiscall
    virtual void Sort_Y();  // [45] 0x0042ef80  // __thiscall
    virtual void Is_Moving_Onto_Bridge(int a0, int a1, int a2, int a3);  // [46] 0x00570db0  // __thiscall
    virtual void Occupies_Cells();  // [47] 0x00416830  // __cdecl
    virtual void Limbo(int a0, int a1, int a2, int a3);  // [48] 0x0042cca0  // __thiscall
    virtual void Unlimbo(int a0);  // [49] 0x00429000  // __thiscall
    virtual void Detach_All(int a0);  // [50] 0x00432f20  // __thiscall
    virtual void Record_The_Kill(int a0);  // [51] 0x00617760  // __thiscall
    virtual void Paradrop(int a0);  // [52] 0x005700c0  // __thiscall
    virtual void Fall_From_Height(int a0);  // [53] 0x0056ea70  // __thiscall
    virtual void Set_Occupy_Bit(int a0);  // [54] 0x00436c10  // __stdcall
    virtual void Clear_Occupy_Bit(int a0);  // [55] 0x00436c70  // __stdcall
    virtual void Delete_Me();  // [56] 0x005709b0  // __thiscall
    virtual void Do_Shimmer();  // [57] 0x00617ce0  // __cdecl
    virtual void Exit_Object(int a0);  // [58] 0x0042b790  // __thiscall
    virtual void Render(int a0, int a1, int a2);  // [59] 0x004265e0  // __thiscall
    virtual void Occupy_List(int a0);  // [60] 0x005701b0  // __thiscall, returns int
    virtual void Draw_Pre_Render(int a0, int a1, int a2, int a3, int a4);  // [61] 0x00610880  // __thiscall
    virtual void Draw_Post_Render(int a0, int a1, int a2, int a3, int a4, int a5);  // [62] 0x0060fb90  // __thiscall
    virtual void Draw_It(int a0, int a1);  // [63] 0x00426980  // __thiscall
    virtual void Editor_Draw_It(int a0, int a1);  // [64] 0x00426760  // __thiscall
    virtual void Hidden();  // [65] 0x0060f590  // __thiscall
    virtual void Look(int a0, int a1);  // [66] 0x0061c1e0  // __thiscall
    virtual void Mark(int a0);  // [67] 0x00428540  // __thiscall
    virtual void Get_Visual_Rect(int a0);  // [68] 0x0056f000  // __thiscall
    virtual void Get_Render_Rect(int a0, int a1, int a2, int a3, int a4, int a5, int a6);  // [69] 0x004386e0  // __thiscall
    virtual void Draw_Radial_Indicator(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11, int a12, int a13, int a14, int a15, int a16, int a17);  // [70] 0x00439240  // __thiscall
    virtual void Mark_For_Redraw();  // [71] 0x0056f680  // __cdecl
    virtual void Active_Click_With(int a0, int a1, int a2);  // [72] 0x0042b410  // __thiscall
    virtual void vfunc_73(int a0, int a1, int a2);  // [73] 0x0042b290  // __thiscall
    virtual void Clicked_As_Target(int a0);  // [74] 0x00439650  // __thiscall
    virtual void Select();  // [75] 0x00613f10  // __thiscall
    virtual void Unselect(int a0);  // [76] 0x0056eda0  // __thiscall
    virtual void In_Range(int a0, int a1);  // [77] 0x00611540  // __thiscall
    virtual void Weapon_Range(int a0);  // [78] 0x006164c0  // __stdcall
    virtual void Take_Damage(int a0, int a1, int a2, int a3, int a4, int a5);  // [79] 0x0042a510  // __thiscall, returns int
    virtual void Scatter(int a0, int a1, int a2);  // [80] 0x0056ec90  // __stdcall
    virtual void Catch_Fire();  // [81] 0x0056eca0  // __cdecl
    virtual void Fire_Out();  // [82] 0x0056ecb0  // __cdecl
    virtual void Value();  // [83] 0x0061a4d0  // __thiscall
    virtual void Get_Mission();  // [84] 0x005468d0  // __cdecl
    virtual void Assign_Mission(int a0);  // [85] 0x00546d60  // __stdcall
    virtual void Per_Cell_Process(int a0);  // [86] 0x0060faa0  // __thiscall
    virtual void Who_Can_Build_Me(int a0, int a1);  // [87] 0x00570240  // __thiscall
    virtual void Receive_Message(int a0, int a1, int a2);  // [88] 0x00425c90  // __thiscall
    virtual void Revealed(int a0);  // [89] 0x00432150  // __thiscall
    virtual void Repair();  // [90] 0x0042db60  // __thiscall
    virtual void Sell_Back(int a0);  // [91] 0x0042dc50  // __thiscall
    virtual void Set_Waypoint_Path(int a0, int a1);  // [92] 0x00570e70  // __stdcall
    virtual void Move(int a0);  // [93] 0x0056ed00  // __thiscall
    virtual void Can_Enter_Cell(int a0, int a1, int a2, int a3, int a4);  // [94] 0x0042efb0  // __stdcall
    virtual void Can_Reach(int a0, int a1, int a2, int a3, int a4);  // [95] 0x00416890  // __stdcall, returns int
    virtual void Get_Coord(int a0);  // [96] 0x0040e6a0  // __stdcall
    virtual void Set_Coord(int a0);  // [97] 0x00570cc0  // __stdcall
    virtual void Get_Cell(int a0);  // [98] 0x0040e6c0  // __stdcall
    virtual void Get_Cell_Ptr();  // [99] 0x00570ce0  // __cdecl
    virtual void Get_Target_Cell(int a0);  // [100] 0x00570d00  // __stdcall
    virtual void Get_Target_Cell_Ptr(int a0);  // [101] 0x00570d50  // __cdecl
    virtual void Get_Height_AGL();  // [102] 0x00570370  // __thiscall
    virtual void Set_Height_AGL(int a0);  // [103] 0x005703b0  // __thiscall
    virtual void Get_Height();  // [104] 0x00570360  // __cdecl
    virtual void Commence(int a0);  // [105] 0x00546d20  // __cdecl
    virtual void Ready_To_Commence();  // [106] 0x00436ea0  // __cdecl
    virtual void Do_MISSION_SLEEP();  // [107] 0x00546750  // __cdecl, returns int
    virtual void Do_MISSION_HARMLESS();  // [108] 0x00546760  // __cdecl, returns int
    virtual void Do_MISSION_AMBUSH();  // [109] 0x00546770  // __cdecl, returns int
    virtual void Do_MISSION_ATTACK();  // [110] 0x00430290  // __thiscall
    virtual void Do_MISSION_CAPTURE();  // [111] 0x00546790  // __cdecl, returns int
    virtual void Do_MISSION_GUARD();  // [112] 0x0042f210  // __thiscall
    virtual void Do_MISSION_GUARD_AREA();  // [113] 0x0042f570
    virtual void Do_MISSION_HARVEST(int a0);  // [114] 0x004307a0
    virtual void Do_MISSION_HUNT();  // [115] 0x005467d0  // __cdecl, returns int
    virtual void Do_MISSION_MOVE();  // [116] 0x005467e0  // __cdecl, returns int
    virtual void Do_MISSION_RETREAT();  // [117] 0x005467f0  // __cdecl, returns int
    virtual void Do_MISSION_RETURN();  // [118] 0x00546800  // __cdecl, returns int
    virtual void Do_MISSION_STOP();  // [119] 0x00546810  // __cdecl, returns int
    virtual void Do_MISSION_UNLOAD(int a0, int a1, int a2, int a3, int a4, int a5);  // [120] 0x00432460  // __thiscall
    virtual void Do_MISSION_ENTER();  // [121] 0x00546830  // __cdecl, returns int
    virtual void Do_MISSION_CONSTRUCTION(int a0);  // [122] 0x0042f580  // __thiscall, returns int
    virtual void Do_MISSION_DECONSTRUCTION(int a0, int a1, int a2, int a3, int a4);  // [123] 0x0042f720  // __thiscall, returns int
    virtual void Do_MISSION_REPAIR(int a0, int a1, int a2);  // [124] 0x004307b0  // __thiscall, returns int
    virtual void Do_MISSION_MISSILE();  // [125] 0x00431600  // __thiscall
    virtual void Do_MISSION_OPEN();  // [126] 0x004329e0  // __thiscall
    virtual void Do_MISSION_RESCUE();  // [127] 0x00546890  // __cdecl, returns int
    virtual void Do_MISSION_PATROL();  // [128] 0x005468a0  // __cdecl, returns int
    virtual void Set_Mission(int a0);  // [129] 0x005468b0  // __stdcall
    virtual void Override_Mission(int a0, int a1, int a2);  // [130] 0x006164e0  // __thiscall
    virtual void Restore_Mission();  // [131] 0x00616520  // __thiscall
    virtual void Has_Suspended_Mission();  // [132] 0x00547110  // __cdecl, returns int
    virtual void Transmit_Message(int a0, int a1);  // [133] 0x005a58a0  // __stdcall
    virtual void Transmit_Message(int a0, int a1, int a2);  // [134] 0x005a57d0  // __thiscall, returns int
    virtual void Is_Move_Override();  // [135] 0x00438860  // __cdecl
    virtual void Is_Allowed_To_Recloak();  // [136] 0x0061cf80  // __cdecl
    virtual void Can_Scatter();  // [137] 0x0060eb20  // __thiscall
    virtual void Is_In_Team();  // [138] 0x0043a110  // __cdecl
    virtual void Should_Self_Heal_Now();  // [139] 0x0061cb60  // __thiscall
    virtual void Is_Voxel_Loaded();  // [140] 0x00612de0  // __thiscall
    virtual void Is_Ready_To_Move();  // [141] 0x0040e710  // __cdecl
    virtual void Is_Ready_To_Cloak(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10);  // [142] 0x00439bf0  // __thiscall
    virtual void Should_Uncloak(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10);  // [143] 0x00439d40  // __thiscall
    virtual void Turret_Facing(int a0);  // [144] 0x0042d180  // __thiscall
    virtual void Is_Weapon_Equipped();  // [145] 0x00616450  // __cdecl
    virtual void Is_On_Elevation(int a0, int a1);  // [146] 0x0061cfa0  // __thiscall, returns int
    virtual void Tiberium_Load();  // [147] 0x0061b1e0  // __cdecl
    virtual void Weed_Load();  // [148] 0x0061d000  // __cdecl
    virtual void Pip_Count();  // [149] 0x00432280  // __thiscall
    virtual void Refund_Amount(int a0);  // [150] 0x0061bf60  // __thiscall
    virtual void Risk();  // [151] 0x0061b1c0  // __cdecl
    virtual void Is_In_Same_Zone_As(int a0);  // [152] 0x0043a120  // __stdcall
    virtual void Barrel_Pitch(int a0, int a1);  // [153] 0x00427d80  // __thiscall
    virtual void Is_In_Same_Zone(int a0);  // [154] 0x0061a660  // __thiscall
    virtual void How_Many_Survivors();  // [155] 0x00434b80  // __cdecl, returns int
    virtual void Scatter_Incoming_Infantry(int a0);  // [156] 0x00439b70  // __thiscall
    virtual void Get_Collateral_Damage(int a0);  // [157] 0x00439a20  // __thiscall
    virtual void Get_Z_Adjust();  // [158] 0x00427d40  // __thiscall
    virtual void Get_Z_Gradient();  // [159] 0x0043a130  // __cdecl, returns int
    virtual void Find_Docking_Bay(int a0, int a1, int a2);  // [160] 0x0061bd30  // __thiscall
    virtual void Find_Exit_Cell(int a0, int a1);  // [161] 0x00433060  // __thiscall
    virtual void Turret_Coord(int a0, int a1);  // [162] 0x00436b20  // __thiscall
    virtual void Desired_Load_Dir(int a0, int a1);  // [163] 0x0061b1f0  // __stdcall, returns int
    virtual void Fire_Direction(int a0);  // [164] 0x004323b0  // __thiscall
    virtual void Crew_Type();  // [165] 0x00432ed0  // __thiscall
    virtual void Can_Attack_Now();  // [166] 0x00616160  // __cdecl
    virtual void Can_Deploy_Now(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7);  // [167] 0x006161b0  // __thiscall
    virtual void Rearm_Delay(int a0);  // [168] 0x006143a0  // __thiscall, returns int
    virtual void Threat_Range(int a0);  // [169] 0x0061a560  // __thiscall
    virtual void Is_Allowed_To_Leave_Map();  // [170] 0x0043a140  // __cdecl
    virtual void Is_Radar_Visible(int a0);  // [171] 0x004397c0  // __thiscall
    virtual void Is_Sensed_By_Player();  // [172] 0x0061db90  // __cdecl
    virtual void Is_Renovator();  // [173] 0x0040e720  // __cdecl
    virtual void Advance_Waypoint_Path();  // [174] 0x0043a150  // __cdecl
    virtual void Response_Select();  // [175] 0x0061b2e0  // __thiscall
    virtual void Response_Move();  // [176] 0x0061b340  // __thiscall
    virtual void Response_Attack();  // [177] 0x0061b3a0  // __thiscall
    virtual void Player_Assign_Mission(int a0);  // [178] 0x00615820  // __thiscall
    virtual void Reduce_Ammunition();  // [179] 0x0061dd50  // __cdecl
    virtual void Target_Something_Nearby(int a0, int a1);  // [180] 0x0061b400  // __thiscall
    virtual void Stun();  // [181] 0x00614290  // __thiscall
    virtual void vfunc_182(int a0, int a1);  // [182] 0x006114b0  // __thiscall
    virtual void Death_Announcement(int a0);  // [183] 0x00432340  // __thiscall
    virtual void Can_Fire(int a0, int a1);  // [184] 0x0042e590  // __thiscall, returns int
    virtual void Greatest_Threat(int a0);  // [185] 0x0042d230  // __thiscall
    virtual void Assign_Target(int a0);  // [186] 0x0042b6e0  // __thiscall
    virtual void Fire_At(int a0, int a1, int a2, int a3, int a4, int a5, int a6);  // [187] 0x006148b0  // __thiscall
    virtual void Captured(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8);  // [188] 0x0042e7f0  // __thiscall
    virtual void Laser_Zap(int a0, int a1, int a2, int a3, int a4, int a5);  // [189] 0x00614530  // __thiscall
    virtual void Rock(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11);  // [190] 0x0061c360  // __thiscall
    virtual void Get_Class_Weapon_Data(int a0);  // [191] 0x00435a70  // __thiscall
    virtual void Is_Turret_Equipped();  // [192] 0x00435b00  // __cdecl
    virtual void Renovate();  // [193] 0x00616550  // __thiscall
    virtual void Cloaking_AI(int a0, int a1, int a2, int a3, int a4, int a5, int a6);  // [194] 0x00437570  // __thiscall
    virtual void Rocking_AI();  // [195] 0x0061c650  // __thiscall
    virtual void Try_To_Cloak(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8);  // [196] 0x0060f8f0  // __thiscall
    virtual void Draw_Action_Line();  // [197] 0x0043a160  // __cdecl
    virtual void Draw_Voxel(int a0, int a1);  // [198] 0x00619490  // __thiscall
    virtual void Draw_Health_Bar_Old(int a0, int a1);  // [199] 0x00610870  // __stdcall
    virtual void Draw_Health_Bar(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11, int a12, int a13, int a14, int a15, int a16);  // [200] 0x00610c50  // __thiscall
    virtual void Draw_Pips(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11);  // [201] 0x0061b4f0  // __thiscall
    virtual void Draw_Text_Overlay(int a0, int a1, int a2);  // [202] 0x0061bc30  // __thiscall
    virtual void Do_Uncloak(int a0);  // [203] 0x00617b90  // __thiscall
    virtual void Do_Cloak(int a0);  // [204] 0x00617c30  // __thiscall
    virtual void Apparent_Brightness(int a0);  // [205] 0x00439790  // __stdcall
    virtual void Is_Ready_To_Random_Animate();  // [206] 0x0061b490  // __cdecl
    virtual void Random_Animate();  // [207] 0x0040e790  // __cdecl
    virtual void Assign_Destination(int a0, int a1);  // [208] 0x00438810  // __thiscall
    virtual void Enter_Idle_Mode(int a0, int a1);  // [209] 0x00432220  // __thiscall
    virtual void Radar_Track();  // [210] 0x00439110  // __thiscall
    virtual void Radar_Untrack(int a0);  // [211] 0x00439170  // __thiscall
    virtual void Plot_On_Radar(int a0, int a1);  // [212] 0x004391d0  // __thiscall
    virtual void Update_Radar_Position(int a0, int a1, int a2, int a3, int a4);  // [213] 0x0061df20  // __thiscall
    virtual void Grand_Opening(int a0, int a1, int a2);  // [214] 0x0042d2c0  // __thiscall
    virtual void Update_Buildables();  // [215] 0x0042caf0
    virtual void Draw_Extras(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11, int a12, int a13, int a14, int a15, int a16);  // [216] 0x00426f30  // __thiscall
    virtual void Aim_Direction(int a0, int a1, int a2);  // [217] 0x00428170  // __thiscall
    virtual void Do_Destruction(int a0, int a1, int a2, int a3);  // [218] 0x00429ce0  // __thiscall
    virtual void Toggle_Primary();  // [219] 0x0042e720  // __thiscall
    virtual void entry_380();  // [220] 0x004355e0  // __cdecl, returns int

    BuildingClass();  // 0x00425530
    // The destructor is declared non-virtual here: slot 0 holds a stub
    // shared with 66 other classes (0x00405790), not the destructor.
    ~BuildingClass();

    // ---- data members (offsets from ctor 0x00425530) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    // vtable-pointer stores are omitted: the compiler supplies the vptr.
    // The machine block begins at +0x214: the compiled end of the
    // base sub-object region. Evidence: TechnoClass at +0x0 (ends +0x214).
    // 2 recovered offset(s) below +0x214 lie inside the base
    // sub-object region and are NOT declared here: they are declared and
    // compiler-checked in the base's own header -- +0x0, +0x4
    unsigned char pad_214[0xa4];
    unsigned int m_2b8;  // +0x2b8 /* ? */; 4-byte store: mov dword ptr [esi + 0x2b8], eax
    unsigned int m_2bc;  // +0x2bc /* ? */; 4-byte store: mov dword ptr [esi + 0x2bc], ecx
    unsigned char pad_2c0[0x4];
    unsigned int m_2c4;  // +0x2c4 /* ? */; 4-byte store: mov dword ptr [esi + 0x2c4], eax
    unsigned int m_2c8;  // +0x2c8 /* ? */; 4-byte store: mov dword ptr [esi + 0x2c8], eax
    unsigned int m_2cc;  // +0x2cc init 0x1; 4-byte store: mov dword ptr [esi + 0x2cc], 1
};
