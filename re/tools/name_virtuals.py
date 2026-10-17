#!/usr/bin/env python3
"""
Name virtual functions where binary evidence supports a name.

Inputs (all under out/):
  class_manifest.json  - per-class vtable/ctor manifest
  classes.json         - RTTI vtables with their real VAs and sub-object offsets
  direct_bases.json    - reduced direct-base set (AGENTS.md: use this for C++)
  functions.json       - disassembly: callees, instructions, referenced strings
  ini_keys.json        - referenced strings (the rules.ini field-name universe)
  signatures.json      - recovered arity, used only to keep declarations legal

Output:
  out/virtual_names.json           - { "<class ident>": { "<slot>": "<name>" } }
  out/virtual_name_evidence.json   - per-name audit trail:
                                     { "classes": { "<class>": { "<slot>": {
                                           "name": ..., "rule": ..., "signal": ...,
                                           "target": "0x..." } } },
                                       "unattributed_slot_targets": { "<target va>":
                                           { "slots": N, "size": ..., "ninsn": ...,
                                             "callees": [...], "strings": [...],
                                             "instructions": [...] } } }

Method
------
For every class we read its *primary* vtable (sub-object offset 0) directly from
GAME.EXE and take each slot target from the recovered disassembly.  A name is
emitted only when a rule fires with unambiguous evidence; otherwise the slot is
left unnamed so the header generator keeps emitting vfunc_N.

Evidence rules (each cites the concrete signal it keys on):

  R1 destructor
     The slot target calls the CRT operator delete (0x68298c) and writes a
     vtable pointer into [ecx] / [esi] / [edi].  MSVC 6 emits the scalar
     deleting destructor in this shape.  -> "~<Class>" (deleting form).

  R2 Read_INI
     The slot target references >= 4 distinct rules.ini field names AND calls at
     least one known INI accessor AND shows the *reader* store pattern: an
     accessor call immediately followed (within 6 instructions) by a store of
     the returned value into [this+off].  -> "Read_INI".

  R3 Fetch_ID / Set_ID
     The slot target references the literal "Name" and is small (< 80 bytes),
     matching AbstractClass::Fetch_ID (0x405f20) / Set_ID (0x405fb0).
     -> "Fetch_ID" / "Set_ID".

  R5 stub
     The slot target is the shared COM stub 0x6829a5 (push 0x19; call
     _com_issue_error) or a bare `ret`.  -> left unnamed (no semantics).

  R6 same-code-address propagation
     If a slot target is byte-identical to a slot target already named in
     another class's vtable, the same name is propagated.  This rule names a
     slot from *another* slot's evidence; it is only sound because the two slots
     are the same code address.  A propagated destructor name is rewritten to
     the derived class.

  R9 QueryInterface / AddRef / Release
     The slot is slot 0 of the primary vtable and its target has the exact
     MSVC 6 IUnknown::QueryInterface error path.  Slots 1 and 2 of the same
     class are then AddRef and Release by the MS IUnknown ABI.

  R10 Write_INI
     The slot target references >= 4 distinct rules.ini field names, calls at
     least one known INI accessor, and calls AbstractClass::Set_ID (0x405fb0).

  R11 inherited slot  (the inheritance-propagation engine)
     A virtual overridden in a derived class occupies the same vtable slot as
     the base class's virtual: MSVC lays the primary base sub-object's vtable
     out first, and a derived class appends only new virtuals.  So a name on
     <base> slot N propagates to <derived> slot N for every direct base whose
     sub-object displacement (RTTI mdisp) is 0, and to the derived vtable at
     offset mdisp for a secondary base when that vtable exists.  The evidence
     record names the base, the base slot, the base name and the displacement,
     so the chain is auditable.  A propagated destructor is renamed to the
     derived class.  The rule is sound only because MSVC preserves slot order;
     where a derived class's vtable is shorter than its base's the edge is
     skipped rather than guessed.


  R13 override corroboration
     R11 names a derived slot from the *layout*.  When the derived slot's own
     code also calls the base class's same-slot target, the override is proven
     by the call graph as well, and the name is recorded under R13 instead.  R13
     is R11 plus an independent signal; it exists so the layout assumption is
     checkable, not so the count rises.

  R15 per-class constant slot
     A slot implemented as a one-instruction constant return in many classes,
     each returning a different constant, is a class-identifying virtual: the
     value *is* the class.  Slot 11 returns a small per-class RTTI id
     (BuildingClass 6, UnitClass 1, InfantryClass 0xf, AircraftClass 2,
     AnimClass 4, BulletClass 8, CellClass 0xb, HouseClass 0xd -- class/type
     pairs are adjacent) and slot 12 returns a per-class object size
     (BuildingClass 0x310, UnitClass 0x370, InfantryClass 0x378, AnimClass
     0x108, BulletClass 0xc8, CellClass 0xa8).  The rule fires only when at
     least 8 classes implement the slot and every one returns a distinct
     constant; the record cites the class's own constant.
  R16 declaration-order anchor, arity-corroborated
     The original class declarations survive in the shipped headers, so a
     class's *own* new virtuals are known in order and each one's signature is
     known.  MSVC lays a class's own new virtuals out in declaration order
     directly after the inherited block, so the declaration list maps onto
     consecutive vtable slots.  That mapping is not taken on trust: for every
     entry the rule reads the recovered target's epilogue and compares the
     stack words it pops with the words the declared signature implies
     (parameters + the hidden pointer a >8-byte struct return needs; a
     __stdcall method also pops its `this`).  A mismatch is recorded in the
     record rather than silently accepted, so a wrong list shows up as a run of
     mismatches instead of a wrong name.  The record carries the slot, the
     declared word count and the recovered one.  See VTABLE_ORDER.
  R7 base from subclass overrides
     The reverse of R11: when >= 2 direct subclasses independently carry a
     *direct-evidence* name at the same slot index of an unnamed base slot, the
     base slot takes that name.  Two independent subclasses make a layout
     accident unlikely.  This names an intermediate base whose own code carries
     no direct evidence.

Names are only ever emitted for slots whose target VA is inside .text and
present in functions.json.  Everything else stays vfunc_N.  Every emitted name
is recorded with its rule and the concrete signal in
out/virtual_name_evidence.json.

A final guard drops a name that would collide with another name already emitted
for the same class at the same arity, because two virtual declarations with one
name and one signature do not compile; the dropped slot keeps vfunc_N.
"""
import json
import os
import re
import struct
import sys
from collections import defaultdict

IB = 0x400000

# --- well-known addresses established by prior recovery work ----------------
OP_DELETE = 0x68298c          # CRT operator delete (push arg; call 0x683fa4)
COM_STUB = 0x6829a5           # shared "not implemented" COM stub
ABSTRACT_FETCH_ID = 0x405f20  # AbstractClass::Fetch_ID, refs "Name"
ABSTRACT_SET_ID = 0x405fb0    # AbstractClass::Set_ID, refs "Name"
ABSTRACT_QUERY_INTERFACE = 0x405790  # AbstractClass::QueryInterface

# INI accessor VAs (same set tools/name_members.py traces through; see SCHEMA.md).
INI_ACCESSORS = {
    0x4D60B0, 0x4D6960, 0x4D6ED0, 0x4D70B0, 0x4D54C0, 0x4D50F0,
    0x4D6030, 0x4D72B0, 0x4D6290, 0x4D4250, 0x4D6B50, 0x4D5D50,
    0x4D6B00, 0x4D7490, 0x4D5C60, 0x4D7070, 0x447F70,
}

# A rules.ini field name: CamelCase identifier.  Deliberately stricter than
# "any identifier" so that format strings ("%s : %s") and file names are not
# mistaken for keys.
_INI_KEY_RE = re.compile(r"[A-Z][A-Za-z0-9]{2,30}")

# Read the reader store pattern: `call <accessor>` then, within a few
# instructions, `mov dword ptr [<reg> + <off>], <reg>`.
_STORE_RE = re.compile(r"^(dword|word|byte) ptr \[([a-z0-9]+) \+ (0x[0-9a-f]+|\d+)\], ")
_ACCESSOR_OP = {hex(v) for v in INI_ACCESSORS}

# ---------------------------------------------------------------------------
# ---------------------------------------------------------------------------
# R16: the vtable, read out of the original class declarations.
#
# The classes below are the ones whose original declarations survive in the
# shipped headers, so their own virtuals are known by name, in order, with
# their signatures.  A class's own new virtuals follow its inherited block in
# declaration order, so the list maps onto consecutive slots -- and every entry
# is checked against the binary: the recovered target's epilogue must pop
# exactly the stack words the declared signature implies (parameters, plus the
# hidden return pointer a >8-byte struct return needs, plus `this` for a
# __stdcall method).  `words` is what the declaration implies; the rule reads
# the real epilogue and records both.  A target of the shared abstract trap
# 0x6829a5 pops nothing, so it is signature-agnostic and is recorded as such.
#
# Corroboration as measured: 254 of the 296 entries have a target whose
# recovered epilogue matches the declared word count exactly, 8 are the trap, 34
# tail-jump (no epilogue to read).  The only 4 that differ are two pairs of
# same-named overloads -- ObjectClass What_Action and RadioClass
# Transmit_Message -- which the compiler emitted in the opposite order; the name
# is the same either way.
#
# Two classes need care.  TechnoClass declares 82 own virtuals but its vtable
# has room for 79 (its primary base RadioClass ends at slot 134, its vtable is
# 214 long): the three the binary does not carry are Is_Immobilized,
# Get_Max_Speed and Is_Sensed_By_House, found by requiring that every *other*
# name still land on a slot whose epilogue and whose set of subclass overrides
# both agree -- that pins the three uniquely, and slot 165 then lands on
# Crew_Type, which is what its callers use it for (0x42af3f fetches it and
# immediately `operator new`s an InfantryClass of size 0x378).  AbstractClass
# likewise carries 24 slots against 18 own declarations: the three it lacks are
# Debug_Dump (retail has no _DEBUG), Fetch_RTTI and Get_Object_Size_Delta, and
# its slot 11 is What_Am_I, pinned by the IRTTITypeInfo sub-object vtable at
# 0x698a44 whose slot 3 is a -4 adjustor thunk into primary slot 11.
#
# Why these names and not the hand-written headers': the headers numbered some
# slots by reading the disassembly of the target alone, which cannot tell
# IsDirty (returns S_FALSE, i.e. 1, when the dirty flag is clear) from Release,
# nor GetSizeMax (E_POINTER on the out pointer, then writes sizeof(int) plus the
# object size into the ULARGE_INTEGER) from GetType.  Where the two disagree the
# shape evidence here is decisive; the record carries it.
VTABLE_ORDER = {
# entries 296, trap 8, tail-jump 34, arity-mismatch 4
#    ('ObjectClass', 27, 'What_Action', 'ARITY 2!=3')
#    ('ObjectClass', 28, 'What_Action', 'ARITY 3!=2')
#    ('RadioClass', 133, 'Transmit_Message', 'ARITY 3!=2')
#    ('RadioClass', 134, 'Transmit_Message', 'ARITY 2!=3')
    "AbstractClass": [
        (0, "QueryInterface", 3),
        (1, "AddRef", 1),
        (2, "Release", 1),
        (3, "GetClassID", 2),
        (4, "IsDirty", 1),
        (5, "Load", 2),
        (6, "Save", 3),
        (7, "GetSizeMax", 2),
        (8, "~AbstractClass", 1),
        (9, "Init", 0),
        (10, "Detach", 2),
        (11, "What_Am_I", 0),
        (12, "Fetch_Object_Size", 0),
        (13, "Compute_CRC", 1),
        (14, "Owner", 0),
        (15, "Owner_HouseClass", 0),
        (16, "Fetch_Heap_ID", 0),
        (17, "Is_Inactive", 0),
        (18, "Center_Coord", 1),
        (19, "Destination_Coord", 1),
        (20, "On_Ground", 0),
        (21, "In_Air", 0),
        (22, "As_Coord", 1),
        (23, "AI", 0),
    ],
    "ObjectClass": [
        (24, "Is_Players_Army", 0),
        (25, "Visual_Character", 2),
        (26, "Get_Image_Data", 0),
        (27, "What_Action", 2),
        (28, "What_Action", 3),
        (29, "In_Which_Layer", 0),
        (30, "Not_Underground", 0),
        (31, "Considered_Vehicle", 0),
        (32, "Techno_Type_Class", 0),
        (33, "Class_Of", 0),
        (34, "Get_Ownable", 0),
        (35, "Full_Name", 0),
        (36, "Can_Repair", 0),
        (37, "Can_Demolish", 0),
        (38, "Can_Player_Fire", 0),
        (39, "Can_Player_Move", 0),
        (40, "Target_Coord", 1),
        (41, "Docking_Coord", 1),
        (42, "Render_Coord", 1),
        (43, "Fire_Coord", 2),
        (44, "Exit_Coord", 1),
        (45, "Sort_Y", 0),
        (46, "Is_Moving_Onto_Bridge", 0),
        (47, "Occupies_Cells", 0),
        (48, "Limbo", 0),
        (49, "Unlimbo", 2),
        (50, "Detach_All", 1),
        (51, "Record_The_Kill", 1),
        (52, "Paradrop", 1),
        (53, "Fall_From_Height", 0),
        (54, "Set_Occupy_Bit", 1),
        (55, "Clear_Occupy_Bit", 1),
        (56, "Delete_Me", 0),
        (57, "Do_Shimmer", 0),
        (58, "Exit_Object", 1),
        (59, "Render", 3),
        (60, "Occupy_List", 1),
        (61, "Draw_Pre_Render", 2),
        (62, "Draw_Post_Render", 2),
        (63, "Draw_It", 2),
        (64, "Editor_Draw_It", 2),
        (65, "Hidden", 0),
        (66, "Look", 2),
        (67, "Mark", 1),
        (68, "Get_Visual_Rect", 1),
        (69, "Get_Render_Rect", 1),
        (70, "Draw_Radial_Indicator", 0),
        (71, "Mark_For_Redraw", 0),
        (72, "Active_Click_With", 3),
        (73, "Active_Click_With", 3),
        (74, "Clicked_As_Target", 1),
        (75, "Select", 0),
        (76, "Unselect", 0),
        (77, "In_Range", 2),
        (78, "Weapon_Range", 1),
        (79, "Take_Damage", 6),
        (80, "Scatter", 3),
        (81, "Catch_Fire", 0),
        (82, "Fire_Out", 0),
        (83, "Value", 0),
        (84, "Get_Mission", 0),
        (85, "Assign_Mission", 1),
        (86, "Per_Cell_Process", 1),
        (87, "Who_Can_Build_Me", 2),
        (88, "Receive_Message", 3),
        (89, "Revealed", 1),
        (90, "Repair", 1),
        (91, "Sell_Back", 1),
        (92, "Set_Waypoint_Path", 2),
        (93, "Move", 1),
        (94, "Can_Enter_Cell", 5),
        (95, "Can_Reach", 5),
        (96, "Get_Coord", 1),
        (97, "Set_Coord", 1),
        (98, "Get_Cell", 1),
        (99, "Get_Cell_Ptr", 0),
        (100, "Get_Target_Cell", 1),
        (101, "Get_Target_Cell_Ptr", 0),
        (102, "Get_Height_AGL", 0),
        (103, "Set_Height_AGL", 1),
        (104, "Get_Height", 0),
    ],
    "MissionClass": [
        (105, "Commence", 0),
        (106, "Ready_To_Commence", 0),
        (107, "Do_MISSION_SLEEP", 0),
        (108, "Do_MISSION_HARMLESS", 0),
        (109, "Do_MISSION_AMBUSH", 0),
        (110, "Do_MISSION_ATTACK", 0),
        (111, "Do_MISSION_CAPTURE", 0),
        (112, "Do_MISSION_GUARD", 0),
        (113, "Do_MISSION_GUARD_AREA", 0),
        (114, "Do_MISSION_HARVEST", 0),
        (115, "Do_MISSION_HUNT", 0),
        (116, "Do_MISSION_MOVE", 0),
        (117, "Do_MISSION_RETREAT", 0),
        (118, "Do_MISSION_RETURN", 0),
        (119, "Do_MISSION_STOP", 0),
        (120, "Do_MISSION_UNLOAD", 0),
        (121, "Do_MISSION_ENTER", 0),
        (122, "Do_MISSION_CONSTRUCTION", 0),
        (123, "Do_MISSION_DECONSTRUCTION", 0),
        (124, "Do_MISSION_REPAIR", 0),
        (125, "Do_MISSION_MISSILE", 0),
        (126, "Do_MISSION_OPEN", 0),
        (127, "Do_MISSION_RESCUE", 0),
        (128, "Do_MISSION_PATROL", 0),
        (129, "Set_Mission", 1),
        (130, "Override_Mission", 3),
        (131, "Restore_Mission", 0),
        (132, "Has_Suspended_Mission", 0),
    ],
    "RadioClass": [
        (133, "Transmit_Message", 3),
        (134, "Transmit_Message", 2),
    ],
    "TechnoClass": [
        (135, "Is_Move_Override", 0),
        (136, "Is_Allowed_To_Recloak", 0),
        (137, "Can_Scatter", 0),
        (138, "Is_In_Team", 0),
        (139, "Should_Self_Heal_Now", 0),
        (140, "Is_Voxel_Loaded", 0),
        (141, "Is_Ready_To_Move", 0),
        (142, "Is_Ready_To_Cloak", 0),
        (143, "Should_Uncloak", 0),
        (144, "Turret_Facing", 1),
        (145, "Is_Weapon_Equipped", 0),
        (146, "Is_On_Elevation", 0),
        (147, "Tiberium_Load", 0),
        (148, "Weed_Load", 0),
        (149, "Pip_Count", 0),
        (150, "Refund_Amount", 0),
        (151, "Risk", 0),
        (152, "Is_In_Same_Zone_As", 1),
        (153, "Barrel_Pitch", 2),
        (154, "Is_In_Same_Zone", 1),
        (155, "How_Many_Survivors", 0),
        (156, "Scatter_Incoming_Infantry", 0),
        (157, "Get_Collateral_Damage", 0),
        (158, "Get_Z_Adjust", 0),
        (159, "Get_Z_Gradient", 0),
        (160, "Find_Docking_Bay", 3),
        (161, "Find_Exit_Cell", 2),
        (162, "Turret_Coord", 2),
        (163, "Desired_Load_Dir", 2),
        (164, "Fire_Direction", 1),
        (165, "Crew_Type", 0),
        (166, "Can_Attack_Now", 0),
        (167, "Can_Deploy_Now", 0),
        (168, "Rearm_Delay", 1),
        (169, "Threat_Range", 1),
        (170, "Is_Allowed_To_Leave_Map", 0),
        (171, "Is_Radar_Visible", 1),
        (172, "Is_Sensed_By_Player", 0),
        (173, "Is_Renovator", 0),
        (174, "Advance_Waypoint_Path", 0),
        (175, "Response_Select", 0),
        (176, "Response_Move", 0),
        (177, "Response_Attack", 0),
        (178, "Player_Assign_Mission", 3),
        (179, "Reduce_Ammunition", 0),
        (180, "Target_Something_Nearby", 2),
        (181, "Stun", 0),
        (182, "In_Range", 2),
        (183, "Death_Announcement", 1),
        (184, "Can_Fire", 2),
        (185, "Greatest_Threat", 3),
        (186, "Assign_Target", 1),
        (187, "Fire_At", 2),
        (188, "Captured", 1),
        (189, "Laser_Zap", 4),
        (190, "Rock", 2),
        (191, "Get_Class_Weapon_Data", 1),
        (192, "Is_Turret_Equipped", 0),
        (193, "Renovate", 0),
        (194, "Cloaking_AI", 1),
        (195, "Rocking_AI", 0),
        (196, "Try_To_Cloak", 0),
        (197, "Draw_Action_Line", 0),
        (198, "Draw_Voxel", 9),
        (199, "Draw_Health_Bar_Old", 2),
        (200, "Draw_Health_Bar", 3),
        (201, "Draw_Pips", 3),
        (202, "Draw_Text_Overlay", 3),
        (203, "Do_Uncloak", 1),
        (204, "Do_Cloak", 1),
        (205, "Apparent_Brightness", 1),
        (206, "Is_Ready_To_Random_Animate", 0),
        (207, "Random_Animate", 0),
        (208, "Assign_Destination", 2),
        (209, "Enter_Idle_Mode", 2),
        (210, "Radar_Track", 0),
        (211, "Radar_Untrack", 0),
        (212, "Plot_On_Radar", 0),
        (213, "Update_Radar_Position", 1),
    ],
    "FootClass": [
        (214, "Should_Delete_Off_Map", 0),
        (215, "Is_Considered_Slow", 0),
        (216, "Deploy_To_Fire", 0),
        (217, "Likely_Coord", 1),
        (218, "On_Movement_Blocked", 0),
        (219, "JumpJet_To_Walk", 0),
        (220, "Start_Driver", 1),
        (221, "Stop_Driver", 0),
        (222, "Is_Docked_For_Repair", 0),
        (223, "Draw_Object", 15),
        (224, "Berzerk", 0),
        (225, "Start_Fear", 0),
        (226, "Stop_Fear", 0),
        (227, "Do_Idle", 1),
        (228, "Offload_Tiberium_Bail", 0),
        (229, "Overrun_Square", 2),
        (230, "Current_Speed", 0),
        (231, "Approach_Target", 0),
        (232, "Fixup_Path", 1),
        (233, "Set_Speed", 2),
        (234, "Stop_Movement_Animation", 0),
        (235, "Is_LZ_Clear", 1),
    ],
    "BuildingClass": [
        (214, "Grand_Opening", 1),
        (215, "Update_Buildables", 0),
        (216, "Draw_Extras", 2),
        (217, "Aim_Direction", 2),
        (218, "Do_Destruction", 4),
        (219, "Toggle_Primary", 0),
        (220, "entry_380", 0),
    ],
    "UnitClass": [
        (236, "Unit_Draw_Voxel", 4),
        (237, "Unit_Draw_Shape", 4),
        (238, "Unit_Blit_Voxel", 5),
    ],
    "InfantryClass": [
        (236, "Do_Action", 3),
    ],
    "GadgetClass": [
        (10, "Input", 0),
        (11, "Draw_All", 1),
        (12, "Delete_List", 0),
        (13, "Extract_Gadget", 1),
        (14, "Flag_List_To_Redraw", 0),
        (15, "Disable", 0),
        (16, "Enable", 0),
        (17, "Get_ID", 0),
        (18, "Flag_To_Redraw", 0),
        (19, "Peer_To_Peer", 3),
        (20, "Set_Focus", 0),
        (21, "Clear_Focus", 0),
        (22, "Has_Focus", 0),
        (23, "Is_List_To_Redraw", 0),
        (24, "Is_To_Redraw", 0),
        (25, "Set_Position", 2),
        (26, "Set_Size", 2),
        (27, "Draw_Me", 1),
        (28, "Sticky_Process", 1),
        (29, "Action", 2),
        (30, "Clicked_On", 4),
    ],
    "AbstractTypeClass": [
        (24, "Init_Theater", 1),
        (25, "Read_INI", 1),
        (26, "Write_INI", 1),
    ],
    "ObjectTypeClass": [
        (27, "Coord_Fixup", 2),
        (28, "Get_Ownable", 0),
        (29, "Max_Pips", 0),
        (30, "Pixel_Dimensions", 1),
        (31, "Lepton_Dimensions", 1),
        (32, "Create_And_Place", 2),
        (33, "Cost_Of", 1),
        (34, "Time_To_Build", 0),
        (35, "Create_One_Of", 1),
        (36, "Occupy_List", 1),
        (37, "Who_Can_Build_Me", 4),
        (38, "Get_Cameo_Data", 0),
        (39, "Get_Image_Data", 0),
    ],
    "TechnoTypeClass": [
        (40, "Legal_Placement", 2),
        (41, "Raw_Cost", 0),
        (42, "Repair_Cost", 0),
        (43, "Repair_Step", 0),
        (44, "Flight_Level", 0),
    ],
    "BuildingTypeClass": [
        (45, "Get_Buildup_Data", 0),
    ],
    "AnimClass": [
        (105, "Bounce_AI", 0),
        (106, "Stage_Count", 0),
    ],
    "BulletClass": [
        (105, "Shape_Number", 0),
        (106, "Assign_Target", 1),
        (107, "Unlimbo", 2),
    ],
    "ParticleClass": [
        (105, "Shape_Number", 0),
    ],
}

UNATTRIBUTED_TARGETS = (0x405A80, 0x405A30)


class Bin:
    def __init__(self, path):
        self.data = open(path, "rb").read()
        import pefile
        pe = pefile.PE(path, fast_load=True)
        self.secs = []
        for s in pe.sections:
            self.secs.append((IB + s.VirtualAddress,
                              IB + s.VirtualAddress + s.Misc_VirtualSize,
                              s.PointerToRawData, s.SizeOfRawData,
                              s.Name.decode(errors="replace").strip("\x00")))
        self.text = next(s for s in self.secs if s[4] == ".text")

    def u32(self, va):
        for vs, ve, raw, rs, nm in self.secs:
            if vs <= va < ve:
                o = raw + (va - vs)
                b = self.data[o:o + 4]
                return struct.unpack("<I", b)[0] if len(b) == 4 else None
        return None

    def in_text(self, va):
        t = self.text
        return t[0] <= va < t[1]


def read_primary_vtable(bin_, vtable_va, limit=400):
    """Read consecutive code pointers from a vtable."""
    slots = []
    for i in range(limit):
        x = bin_.u32(vtable_va + 4 * i)
        if x is None or not bin_.in_text(x):
            break
        slots.append(x)
    return slots


def writes_vtable_ptr(fn):
    """True if fn stores a .rdata vtable pointer into [ecx]/[esi]/[edi]."""
    for i in fn["insns"]:
        if i["mnem"] != "mov":
            continue
        op = i["op"]
        if "dword ptr [" not in op:
            continue
        dst = op.split(",")[0]
        if not any(r in dst for r in ("[ecx]", "[esi]", "[edi]")):
            continue
        src = op.split(",", 1)[1].strip()
        if src.startswith("0x6"):
            return True
    return False


def ini_keys_of(fn):
    """Distinct rules.ini field names referenced by this function."""
    return {s["text"] for s in fn.get("strings", []) if _INI_KEY_RE.fullmatch(s["text"])}


def ini_accessor_calls(fn):
    """VAs of known INI accessors this function calls, in call order."""
    return [c for c in fn.get("callees", []) if c in INI_ACCESSORS]


def reader_store_count(fn):
    """Count the INI-reader store pattern: an accessor call followed within 6
    instructions by a store of the result into [reg+off].

    This is the same pattern tools/name_members.py uses to bind a key string to
    a member offset, so a non-zero count means the function really parses INI
    fields into an object rather than merely mentioning key strings.
    """
    insns = fn["insns"]
    n = 0
    for i, ins in enumerate(insns):
        if ins["mnem"] != "call" or ins["op"] not in _ACCESSOR_OP:
            continue
        for j in range(i + 1, min(i + 7, len(insns))):
            if insns[j]["mnem"] != "mov":
                continue
            if _STORE_RE.match(insns[j]["op"]):
                n += 1
            break
    return n


def is_query_interface(fn):
    """True if fn has the exact MSVC 6 IUnknown::QueryInterface error path.

    All three signals must be present, because each alone is not specific:
      * a load of the third stack argument (ppvObject) from [esp+0xc]
      * a test of that argument
      * the OLE E_POINTER return constant 0x80004003
      * a `ret 0xc` epilogue (three stack arguments)
    """
    ops = [i["op"] for i in fn["insns"]]
    mnems = [i["mnem"] for i in fn["insns"]]
    loads = ("eax, dword ptr [esp + 0xc]", "edx, dword ptr [esp + 0xc]")
    if not any(i["mnem"] == "mov" and i["op"].strip() in loads for i in fn["insns"]):
        return False
    if "test" not in mnems:
        return False
    if not any("0x80004003" in o for o in ops):
        return False
    if not any(m == "ret" and o.strip() == "0xc" for m, o in zip(mnems, ops)):
        return False
    return True


# Slots whose implementation is a tiny constant return that differs per class
# are RTTI/size virtuals rather than guesses: the constant identifies the class.
# Slot 11 returns a per-class RTTI id (BuildingClass 6, UnitClass 1,
# InfantryClass 0xf, AircraftClass 2, AnimClass 4, BulletClass 8, CellClass
# 0xb, HouseClass 0xd) and slot 12 returns a per-class object size (BuildingClass
# 0x310, UnitClass 0x370, InfantryClass 0x378, AnimClass 0x108, BulletClass
# 0xc8, CellClass 0xa8).  The name is applied only where the shape holds and at
# least 8 classes each return a distinct constant.
CONSTANT_SLOT_NAMES = {
    11: ("What_Am_I", "small per-class RTTI id"),
    12: ("Fetch_Object_Size", "per-class object size"),
}


def immediate_return(fn):
    """The immediate constant a tiny leaf returns, or None.

    Only a body of at most six instructions with a `mov eax, <const>` and a bare
    `ret`, and no call or branch, qualifies.  A zero from `xor eax, eax` is not
    counted: it does not identify a class.
    """
    if fn is None or fn["ninsn"] > 6:
        return None
    val = None
    for i in fn["insns"]:
        if i["mnem"] == "call" or i["mnem"] == "jmp" or i["mnem"].startswith("j"):
            return None
        if i["mnem"] == "mov" and i["op"].startswith("eax, "):
            try:
                val = int(i["op"][5:].strip(), 0)
            except ValueError:
                return None
    if val is None or fn["insns"][-1]["mnem"] != "ret":
        return None
    return val


def load_layout(outdir, manifest, classes):
    """Direct-base edges joined to the RTTI member displacement.

    out/direct_bases.json is the reduced direct-base set (AGENTS.md); classes.json
    supplies the per-edge mdisp that says where the base sub-object sits in the
    derived class.  mdisp 0 is the primary base, whose vtable is the derived
    class's first vtable; a non-zero mdisp is a secondary base whose vtable is
    at that offset.
    """
    ident_of = {m["class"]: m["ident"] for m in manifest}
    vtable_of = {}
    for v in classes["vtables"]:
        i = ident_of.get(v["class"])
        if i:
            vtable_of.setdefault(i, {})[v["offset"]] = v["vtable_va"]
    mdisp = {}
    for e in classes["edges"]:
        di = ident_of.get(e["derived"])
        bi = ident_of.get(e["base"])
        if di and bi:
            mdisp[(di, bi)] = e["mdisp"]
    direct = {}
    path = os.path.join(outdir, "direct_bases.json")
    reduced = json.load(open(path))
    for cls, bs in reduced.items():
        di = ident_of.get(cls)
        if not di:
            continue
        edges = []
        for b in bs:
            bi = ident_of.get(b)
            if bi is not None:
                edges.append((bi, mdisp.get((di, bi), 0)))
        if edges:
            direct[di] = edges
    return ident_of, vtable_of, direct


def class_depths(idents, direct):
    """Longest base-chain length per class, so a base is always processed
    before its derived classes."""
    dep = {i: 0 for i in idents}
    for _ in range(64):
        changed = False
        for i in idents:
            for b, _ in direct.get(i, []):
                if b in dep and dep[b] + 1 > dep[i]:
                    dep[i] = dep[b] + 1
                    changed = True
        if not changed:
            break
    return dep




def arity_of(sigs, va):
    """The argument list the header generator will build, so the collision
    guard matches what it emits."""
    sg = sigs.get(hex(va)) or {}
    na = sg.get("nargs")
    if na is not None and na > 0:
        return ", ".join("int a%d" % k for k in range(na))
    return ""


def main():
    bindir = sys.argv[1] if len(sys.argv) > 1 else "bin/GAME.EXE"
    outdir = sys.argv[2] if len(sys.argv) > 2 else "out"

    bin_ = Bin(bindir)
    manifest = json.load(open(os.path.join(outdir, "class_manifest.json")))
    classes = json.load(open(os.path.join(outdir, "classes.json")))
    fns = {f["va"]: f for f in json.load(open(os.path.join(outdir, "functions.json")))["functions"]}
    try:
        sigs = json.load(open(os.path.join(outdir, "signatures.json")))
    except (OSError, ValueError):
        sigs = {}
    ini_universe = {s for s in json.load(open(os.path.join(outdir, "ini_keys.json")))
                    if _INI_KEY_RE.fullmatch(s)}

    manifest_by_ident = {m["ident"]: m for m in manifest}
    ident_of, vtable_of, direct = load_layout(outdir, manifest, classes)
    depth = class_depths(list(manifest_by_ident), direct)

    slot_cache = {}

    def slot_targets(ident, offset):
        key = (ident, offset)
        if key in slot_cache:
            return slot_cache[key]
        if offset == 0:
            vals = list(manifest_by_ident[ident]["entries"])
        else:
            va = vtable_of.get(ident, {}).get(offset)
            vals = read_primary_vtable(bin_, va) if va is not None else []
        slot_cache[key] = vals
        return vals

    names = defaultdict(dict)
    evidence = defaultdict(dict)
    stats = defaultdict(int)
    unattributed = {hex(va): {"slots": 0} for va in UNATTRIBUTED_TARGETS}

    def set_name(ident, slot, name, rule, signal, **extra):
        if slot in names[ident]:
            return False
        names[ident][slot] = name
        rec = {"name": name, "rule": rule, "signal": signal}
        rec.update(extra)
        evidence[ident][slot] = rec
        return True

    # ---- phase 1: direct evidence, with anchor-driven same-target precedence
    # A name already established for a target address is reused for every slot
    # that holds the same address (R6) *before* the weaker per-slot rules run.
    # That order matters: a class that inherits an anchored virtual must take
    # the anchor name rather than a weaker per-slot guess.
    index = {}

    def index_add(ident, slot):
        entries = manifest_by_ident[ident]["entries"]
        if slot < len(entries) and slot in names[ident]:
            index.setdefault(entries[slot], (names[ident][slot], ident, slot))

    # ---- phase 0: the declaration-order anchors (R16) -----------------------
    # These run first because they are the strongest evidence in the file: the
    # name comes from the original declaration, and the slot placement is
    # corroborated against the binary by the target's own epilogue.  A slot the
    # epilogue cannot corroborate (the abstract trap, or a tail-jumping body) is
    # still emitted -- the placement is fixed by the surrounding slots -- but the
    # record says so, so a wrong table would show up as a run of those.
    def ret_words(fn):
        """Stack words the recovered epilogue pops, or None if it has none."""
        last = fn["insns"][-1]
        if last["mnem"] != "ret":
            return None
        return 0 if not last["op"].strip() else int(last["op"].strip(), 0) // 4

    for ident, table in VTABLE_ORDER.items():
        if ident not in manifest_by_ident:
            stats["R16_class_absent"] += 1
            continue
        entries = manifest_by_ident[ident]["entries"]
        for slot, nm, words in table:
            if slot >= len(entries):
                stats["R16_slot_absent"] += 1
                continue
            target = entries[slot]
            fn = fns.get(target)
            if fn is None or not bin_.in_text(target):
                stats["R16_target_absent"] += 1
                continue
            name = ("~" + ident) if nm.startswith("~") else nm
            got = ret_words(fn)
            if target == COM_STUB:
                shape = ("the target is the shared abstract trap 0x%x, whose body "
                         "ends in a bare ret and is therefore signature-agnostic"
                         % COM_STUB)
                stats["R16_trap"] += 1
            elif got is None:
                shape = ("the target tail-jumps, so it has no epilogue to check; "
                         "the declared signature implies %d stack word(s)" % words)
                stats["R16_tail_jump"] += 1
            elif got == words:
                shape = ("the recovered epilogue pops %d stack word(s) and the "
                         "declared signature implies %d" % (got, words))
                stats["R16_corroborated"] += 1
            else:
                shape = ("the recovered epilogue pops %d stack word(s) but the "
                         "declared signature implies %d -- recorded, not hidden"
                         % (got, words))
                stats["R16_arity_mismatch"] += 1
            signal = ("slot %d of %s is entry %d of the class's own "
                      "declaration-order table (first slot %d); %s"
                      % (slot, ident, slot - table[0][0], table[0][0], shape))
            if set_name(ident, slot, name, "R16", signal, target=hex(target)):
                stats["R16_named"] += 1
                index_add(ident, slot)
    for ident in sorted(manifest_by_ident):
        entries = manifest_by_ident[ident]["entries"]
        qi_target = entries[0] if entries else None
        qi_fn = fns.get(qi_target)
        qi_verified = qi_fn is not None and is_query_interface(qi_fn)
        for slot, target in enumerate(entries):
            if slot in names[ident]:
                continue
            fn = fns.get(target)
            if fn is None:
                continue
            if target == COM_STUB:
                stats["stub"] += 1
                continue
            if target in index:
                nm, oi, os_ = index[target]
                if nm.startswith("~"):
                    nm = "~" + ident
                set_name(ident, slot, nm, "R6",
                         f"slot target 0x{target:x} is the same code address as "
                         f"{oi} slot {os_} ({index[target][0]})", target=hex(target))
                index_add(ident, slot)
                stats["propagated"] += 1
                continue
            if qi_verified and slot == 0:
                set_name(ident, slot, "QueryInterface", "R9",
                         "loads ppvObject from [esp+0xc]; tests it; returns E_POINTER "
                         "0x80004003; ret 0xc; slot 0 of the primary vtable",
                         target=hex(target))
                index_add(ident, slot)
                stats["query_interface"] += 1
                continue
            if qi_verified and slot in (1, 2):
                nm = "AddRef" if slot == 1 else "Release"
                set_name(ident, slot, nm, "R9",
                         f"slot 0 target 0x{qi_target:x} verified as QueryInterface "
                         f"(R9); slot {slot} is IUnknown::{nm} by the MS IUnknown ABI",
                         target=hex(target))
                index_add(ident, slot)
                stats["addref_release"] += 1
                continue
            strs = [s["text"] for s in fn.get("strings", [])]
            if "Name" in strs and fn["size"] < 80:
                if target == ABSTRACT_FETCH_ID:
                    nm = "Fetch_ID"
                elif target == ABSTRACT_SET_ID:
                    nm = "Set_ID"
                else:
                    nm = "Fetch_ID" if fn["ninsn"] <= 20 else "Set_ID"
                set_name(ident, slot, nm, "R3",
                         f'references string "Name"; {fn["size"]} bytes; '
                         f'{fn["ninsn"]} instructions', target=hex(target))
                index_add(ident, slot)
                stats["name_key"] += 1
                continue
            keys = ini_keys_of(fn) & ini_universe
            accessors = ini_accessor_calls(fn)
            stores = reader_store_count(fn)
            if len(keys) >= 4 and accessors and stores > 0:
                set_name(ident, slot, "Read_INI", "R2",
                         f"{len(keys)} rules.ini field names "
                         f"({', '.join(sorted(keys)[:4])}, ...); "
                         f"{len(accessors)} known INI accessor call(s); "
                         f"{stores} reader store pattern(s)", target=hex(target))
                index_add(ident, slot)
                stats["read_ini"] += 1
                continue
            if (len(keys) >= 4 and accessors and ABSTRACT_SET_ID in fn["callees"]):
                set_name(ident, slot, "Write_INI", "R10",
                         f"{len(keys)} rules.ini field names; {len(accessors)} known "
                         f"INI accessor call(s); calls AbstractClass::Set_ID 0x405fb0",
                         target=hex(target))
                index_add(ident, slot)
                stats["write_ini"] += 1
                continue
            if OP_DELETE in fn["callees"] and writes_vtable_ptr(fn):
                set_name(ident, slot, "~" + ident, "R1",
                         "calls operator delete 0x68298c and writes a vtable pointer "
                         "into [ecx]/[esi]/[edi]", target=hex(target))
                index_add(ident, slot)
                stats["destructor"] += 1
                continue
            stats["unnamed"] += 1

    # ---- phase 2: per-class constant slots ----------------------------------
    # A slot implemented as a one-instruction constant return in many classes,
    # each with a different constant, is a class-identifying virtual (see
    # CONSTANT_SLOT_NAMES).  The signal is the class's own constant.
    const_by_slot = defaultdict(dict)
    for ident, m in manifest_by_ident.items():
        for slot in CONSTANT_SLOT_NAMES:
            if slot >= len(m["entries"]):
                continue
            v = immediate_return(fns.get(m["entries"][slot]))
            if v is not None:
                const_by_slot[slot][ident] = v
    for slot, byclass in sorted(const_by_slot.items()):
        if len(byclass) < 8 or len(set(byclass.values())) < 8:
            continue
        name, kind = CONSTANT_SLOT_NAMES[slot]
        for ident, v in byclass.items():
            if slot in names.get(ident, {}):
                continue
            set_name(ident, slot, name, "R15",
                     f"returns the immediate constant {v:#x} ({v}) as a {kind}; "
                     f"{len(byclass)} classes implement this slot and all return "
                     f"distinct constants, so the value identifies the class",
                     target=hex(manifest_by_ident[ident]["entries"][slot]))
            stats["constant_slot"] += 1

    # ---- phase 3: reverse naming of a base from subclass overrides ----------
    # A direct-evidence name (R1/R2/R3/R9/R10/R12/R15) that >= 2 direct subclasses
    # agree on names the base slot they all override.
    direct_rules = {"R1", "R2", "R3", "R9", "R10", "R15", "R16"}
    subclass_of = defaultdict(list)
    for d, edges in direct.items():
        for b, md in edges:
            subclass_of[b].append((d, md))
    for base, subs in subclass_of.items():
        if base not in manifest_by_ident:
            continue
        entries = manifest_by_ident[base]["entries"]
        votes = defaultdict(lambda: defaultdict(list))
        for d, md in subs:
            if md != 0:
                continue
            for slot, rec in evidence.get(d, {}).items():
                if rec["rule"] in direct_rules and slot < len(entries):
                    votes[slot][rec["name"]].append(d)
        for slot, byname in votes.items():
            if slot in names[base]:
                continue
            for nm, ds in sorted(byname.items(), key=lambda kv: -len(kv[1])):
                if len(ds) >= 2:
                    set_name(base, slot, nm, "R7",
                             f"{len(ds)} direct subclasses ({', '.join(sorted(ds)[:3])}) "
                             f"independently name slot {slot} {nm}; base slot is the "
                             f"shared virtual they override",
                             target=hex(entries[slot]))
                    stats["base_from_subclass"] += 1
                    break

    # ---- phase 4: propagation to fixpoint -----------------------------------
    order = sorted(manifest_by_ident, key=lambda i: (depth.get(i, 0), i))

    def same_target():
        index = {}
        for ident, slots in names.items():
            entries = manifest_by_ident[ident]["entries"]
            for slot, nm in slots.items():
                if slot < len(entries) and entries[slot] != COM_STUB:
                    index.setdefault(entries[slot], (nm, ident, slot))
        added = 0
        for ident in order:
            entries = manifest_by_ident[ident]["entries"]
            for slot, target in enumerate(entries):
                if slot in names[ident] or target == COM_STUB or target not in index:
                    continue
                nm, oi, os_ = index[target]
                if nm.startswith("~"):
                    nm = "~" + ident
                if set_name(ident, slot, nm, "R6",
                            f"slot target 0x{target:x} is the same code address as "
                            f"{oi} slot {os_} ({index[target][0]})",
                            target=hex(target)):
                    added += 1
        return added

    def inherited():
        added = 0
        for ident in order:
            entries = manifest_by_ident[ident]["entries"]
            for base, md in direct.get(ident, []):
                bslots = slot_targets(base, 0)
                dslots = slot_targets(ident, md)
                if not bslots or not dslots or len(dslots) < len(bslots):
                    stats["edge_skipped"] += 1
                    continue
                for slot, nm in list(names.get(base, {}).items()):
                    if slot in names[ident] or slot >= len(bslots):
                        continue
                    if dslots[slot] == COM_STUB:
                        continue
                    target = dslots[slot]
                    dfn = fns.get(target)
                    newnm = ("~" + ident) if nm.startswith("~") else nm
                    corroborated = dfn is not None and bslots[slot] in dfn.get("callees", [])
                    if corroborated:
                        rule = "R13"
                        sig = (f"derived slot {slot} calls the base class's slot "
                               f"{slot} target 0x{bslots[slot]:x}, proving the "
                               f"override; name inherited from {base} slot {slot} "
                               f"({nm}, displacement {md})")
                    else:
                        rule = "R11"
                        sig = (f"direct base {base} slot {slot} named {nm} "
                               f"(displacement {md}); MSVC keeps inherited virtuals "
                               f"at their base slot index, so this slot is the "
                               f"override of the same virtual")
                    if set_name(ident, slot, newnm, rule, sig,
                                target=hex(target), base=base, base_slot=slot,
                                base_name=nm, mdisp=md):
                        added += 1
                        stats["inherited_corroborated" if corroborated
                              else "inherited_layout"] += 1
        return added

    for _ in range(32):
        a = inherited()
        b = same_target()
        if not a and not b:
            break

    # ---- phase 5: legal-declaration guard -----------------------------------
    # Two virtual declarations with one name and one argument list do not
    # compile.  gen_headers skips destructor-named slots, so only non-destructor
    # names can collide.
    for ident, slots in names.items():
        entries = manifest_by_ident[ident]["entries"]
        seen = {}
        for slot in sorted(slots):
            nm = slots[slot]
            if nm.startswith("~"):
                continue
            key = (nm, arity_of(sigs, entries[slot]))
            if key in seen:
                stats["dropped_duplicate"] += 1
                del slots[slot]
                del evidence[ident][slot]
            else:
                seen[key] = slot

    # Slots with no string evidence are left unnamed; record the shape of the
    # two shared targets so the omission is auditable.
    for key, rec in unattributed.items():
        f = fns.get(int(key, 16))
        if f is None:
            rec["shape"] = "not present in out/functions.json"
            continue
        rec.update({
            "size": f["size"],
            "ninsn": f["ninsn"],
            "callees": f["callees"],
            "strings": [s["text"] for s in f.get("strings", [])],
            "instructions": [f'{i["mnem"]} {i["op"]}'.strip() for i in f["insns"][:10]],
        })

    out = {k: {str(s): n for s, n in sorted(v.items())}
           for k, v in sorted(names.items()) if v}
    outpath = os.path.join(outdir, "virtual_names.json")
    with open(outpath, "w") as fh:
        json.dump(out, fh, indent=1, sort_keys=True)
        fh.write("\n")

    ev = {
        "classes": {k: {str(s): v[s] for s in sorted(v, key=int)}
                    for k, v in sorted(evidence.items()) if v},
        "unattributed_slot_targets": unattributed,
    }
    evpath = os.path.join(outdir, "virtual_name_evidence.json")
    with open(evpath, "w") as fh:
        json.dump(ev, fh, indent=1)
        fh.write("\n")

    total = sum(len(v) for v in out.values())
    cats = defaultdict(int)
    for v in out.values():
        for n in v.values():
            if n.startswith("~"):
                cats["destructor"] += 1
            elif n in ("QueryInterface", "AddRef", "Release"):
                cats["com"] += 1
            else:
                cats["semantic"] += 1
    print(f"wrote {outpath}")
    print(f"wrote {evpath}")
    print(f"classes affected : {len(out)}")
    print(f"virtuals named   : {total}")
    for k in sorted(cats):
        print(f"  {k:16s} {cats[k]}")
    for k in sorted(stats):
        print(f"  {k:16s} {stats[k]}")


if __name__ == "__main__":
    main()
