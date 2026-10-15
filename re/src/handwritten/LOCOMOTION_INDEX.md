# Locomotion subsystem index

Reconstructed from `bin/GAME.EXE` (C&C Tiberian Sun, Westwood Studios 1999).
All addresses are virtual addresses (ImageBase 0x400000).

## How this was recovered

* Class identity comes from the MSVC RTTI CompleteObjectLocator (COL) pointer
  stored immediately before each vtable.  Resolving COL+0x0C -> TypeDescriptor
  -> name yields the original mangled class name (e.g. `.?AVDriveLocomotionClass@@`).
* Vtable extents are the byte distance to the next vtable in `.rdata`; the
  ILocomotion interface is 50 slots (0x00..0xC4) and the IPersistStream
  interface is 9 slots (0x00..0x20).  The final slot of each vtable is the COL
  pointer, not a function.
* Override sets are computed by diffing each concrete ILocomotion vtable against
  the abstract `LocomotionClass` vtable at 0x69fd5c.
* Object layouts come from the constructors (which write the vtable pointers and
  initialise members) and the scalar-deleting destructors (which free members).
* Movement parameters are **not** stored in the locomotion objects.  They are
  read from the owning `*TypeClass` INI (TechnoTypeClass / UnitTypeClass /
  InfantryTypeClass / AircraftTypeClass) by the readers listed below.  The INI
  key strings are the original field names.

## ILocomotion interface (vtable 0x69fd5c, 50 slots)

| slot | offset | base impl | inferred meaning |
|------|--------|-----------|------------------|
| 0  | 0x00 | 0x4f9c50 | QueryInterface (E_POINTER on null) |
| 1  | 0x04 | 0x4f9bf0 | AddRef (refcount at 0x7d3104) |
| 2  | 0x08 | 0x4f9c10 | Release (refcount at 0x7d3104) |
| 3  | 0x0C | 0x4f99c0 | Init/SetOwner (stores arg at this+4) |
| 4  | 0x10 | 0x4f9f70 | Is_Moving /* ? */ (base false) |
| 5  | 0x14 | 0x4f9f00 | Head_To / Destination /* ? */ |
| 6  | 0x18 | 0x4f9f30 | Stop_Moving /* ? */ |
| 7  | 0x1C | 0x4f9e80 | Do_Turn /* ? */ (base 0) |
| 8  | 0x20 | 0x4f9e70 | Is_Turning /* ? */ (base true) |
| 9  | 0x24 | 0x4f99d0 | Mark_All_Occupation_Bits /* ? */ |
| 10 | 0x28 | 0x4f9a70 | Is_Moving_Now /* ? */ |
| 11 | 0x2C | 0x4f9e60 | Force_Track /* ? */ |
| 12 | 0x30 | 0x4f9b60 | Get_Speed /* ? */ (owner vtbl+0x198) |
| 13 | 0x34 | 0x4f9e50 | Is_Not_Moving /* ? */ |
| 14 | 0x38 | 0x4f9e30 | Get_Status /* ? */ |
| 15 | 0x3C | 0x4f9e40 | Get_Status_2 /* ? */ (base 2) |
| 16 | 0x40 | 0x4f9ef0 | Is_Ok_To_End /* ? */ (base true) |
| 17 | 0x44 | 0x4f9ee0 | (base ret 0x10) |
| 18 | 0x48 | 0x4f9ed0 | (base ret 4) |
| 19 | 0x4C | 0x4f9ec0 | (base ret 8) |
| 20 | 0x50 | 0x4f9eb0 | (base ret 4) |
| 21 | 0x54 | 0x4f9e20 | (base ret 4) |
| 22 | 0x58 | 0x4f9b90 | Set_Moving_Flag (byte[this+8]=1) |
| 23 | 0x5C | 0x4f9bb0 | Clear_Moving_Flag (byte[this+8]=0) |
| 24 | 0x60 | 0x4f9bd0 | Get_Moving_Flag (byte[this+8]) |
| 25 | 0x64 | 0x4f9be0 | (base false) |
| 26 | 0x68 | 0x4f9e00 | (base false) |
| 27 | 0x6C | 0x4f9e10 | (base false) |
| 28 | 0x70 | 0x4f9ea0 | (base ret 0x14) |
| 29 | 0x74 | 0x6829a5 | pure-call stub |
| 30 | 0x78 | 0x4f9e90 | (base ret 0x10) |
| 31 | 0x7C | 0x4f9f80 | (base ret 8) |
| 32 | 0x80 | 0x4803b0 | forwards to owner vtbl+0x10 |
| 33 | 0x84 | 0x4f9fb0 | forwards to owner vtbl+0x398 |
| 34 | 0x88 | 0x4f9f90 | (base 0) |
| 35 | 0x8C | 0x4f9fa0 | (base 0) |
| 36 | 0x90 | 0x47ea20 | (base 0) |
| 37 | 0x94 | 0x47ea30 | (base ret 4) |
| 38 | 0x98 | 0x47ea40 | (base false) |
| 39 | 0x9C | 0x4803c0 | (base ret 8) |
| 40 | 0xA0 | 0x4803d0 | (base false) |
| 41 | 0xA4 | 0x4803e0 | (base false) |
| 42 | 0xA8 | 0x47ea10 | forwards to owner vtbl+0x80 |
| 43 | 0xAC | 0x47ea50 | (base ret 4) |
| 44 | 0xB0 | 0x4803f0 | (base ret 4) |
| 45 | 0xB4 | 0x480400 | (base ret 4) |
| 46 | 0xB8 | 0x480410 | (base -1) |
| 47 | 0xBC | 0x480420 | (base -1) |
| 48 | 0xC0 | 0x480430 | (base -1) |
| 49 | 0xC4 | COL 0x6b0310 | RTTI CompleteObjectLocator |

## IPiggyback interface (vtable 0x69d6b8, 9 slots)

Implemented by Drive, DropPod and Walk locomotion classes.

| slot | offset | impl | inferred meaning |
|------|--------|------|------------------|
| 0 | 0x00 | 0x47eb70 | QueryInterface (this-0x14) |
| 1 | 0x04 | 0x47eb80 | AddRef |
| 2 | 0x08 | 0x47eb90 | Release |
| 3 | 0x0C | 0x479a30 | Begin_Piggyback /* ? */ |
| 4 | 0x10 | 0x479a80 | End_Piggyback /* ? */ |
| 5 | 0x14 | 0x479ac0 | Is_Ok_To_End /* ? */ |
| 6 | 0x18 | 0x479760 | Piggyback_Available /* ? */ |
| 7 | 0x1C | 0x47ea80 | Is_Piggybacking /* ? */ (dword[this+0x54]) |
| 8 | 0x20 | COL | RTTI CompleteObjectLocator |

## Locomotion classes

### LocomotionClass (abstract base)
* RTTI COL 0x6b0310 -> `.?AVLocomotionClass@@`
* ILocomotion vtable 0x69fd5c (50 slots), IPersistStream vtable 0x69fe24 (9 slots)
* ctor 0x4f99a0, scalar deleting dtor 0x4cb060
* layout: +0x00 IPersistStream vtbl, +0x04 ILocomotion vtbl, +0x08 bool m_bIsMoving
* overrides: none (defines the interface)

### DriveLocomotionClass
* RTTI COL 0x6ad798 -> `.?AVDriveLocomotionClass@@`
* ILocomotion vtable 0x69d6dc (50 slots), IPersistStream 0x69d7a4, IPiggyback 0x69d6b8
* ctor 0x4798d0, scalar deleting dtor 0x479870
* layout: +0x14 IPiggyback vtbl, +0x68 void* m_pPiggybackedObject /* ? */
* overrides (27): 0,1,2,4,5,6,9,14,15,16,17,18,19,20,28,29,31,32,39,40,41,44,45,46,47,48
* movement parameters (TechnoTypeClass/UnitTypeClass INI):
  `Speed` (0x6c3334), `Locomotor` (0x6dba30), `AccelerationFactor` (0x6db9c4),
  `DeaccelerationFactor` (0x6db9d8), `Weight` (0x6db9bc),
  `TrackedUphill` (0x6d6958), `TrackedDownhill` (0x6d6948),
  `WheeledUphill` (0x6d6938), `WheeledDownhill` (0x6d6928),
  `IceBreakingWeight` (0x6d6990), `IceCrackingWeight` (0x6d69a4),
  `MovementZone` (0x6db5xx), `SpeedType` (0x6dc108)

### DropPodLocomotionClass
* RTTI COL 0x6ad990 -> `.?AVDropPodLocomotionClass@@`
* ILocomotion vtable 0x69da78 (50 slots), IPersistStream 0x69db40, IPiggyback 0x69da54
* ctor 0x47ffe0, scalar deleting dtor 0x480210
* layout: +0x14 IPiggyback vtbl, +0x28 void* m_pPiggybackedObject /* ? */
* overrides (11): 0,1,2,4,5,16,17,18,29,34
* movement parameters (TechnoTypeClass INI):
  `DropPodSpeed` (0x6d6af4), `DropPodAngle` (0x6d6ae4), `DropPodHeight` (0x6d6b04),
  `DropPodWeapon` (0x6d...), `Droppod` (0x6db388)

### FlyLocomotionClass
* RTTI COL 0x6ade98 -> `.?AVFlyLocomotionClass@@`
* ILocomotion vtable 0x69e0f8 (50 slots), IPersistStream 0x69e1c0
* ctor 0x498ed0, scalar deleting dtor 0x498e90
* layout: base only (0x0C bytes)
* overrides (19): 4,5,9,10,11,12,16,17,18,19,23,24,25,29,32,33,36,37
* movement parameters (AircraftTypeClass INI):
  `Airspeed` (0x6d...), `PitchSpeed` (0x6dba3c), `PitchAngle` (0x6db...),
  `Locomotor` (0x6dba30), `Weight` (0x6db9bc), `AccelerationFactor` (0x6db9c4),
  `DeaccelerationFactor` (0x6db9d8)

### HoverLocomotionClass
* RTTI COL 0x6b02b0 -> `.?AVHoverLocomotionClass@@`
* ILocomotion vtable 0x69fc5c (50 slots), IPersistStream 0x69fd24
* ctor 0x4cae20, scalar deleting dtor 0x4cade0
* layout: +0x2C member initialised by 0x492750 /* ? */
* overrides (19): 3,4,5,6,9,16,17,18,19,23,24,25,26,27,29,32,39,40
* movement parameters (TechnoTypeClass INI):
  `HoverAcceleration` (0x6d6a88), `HoverBrake` (0x6d6a7c), `HoverBoost` (0x6d6a9c),
  `HoverBob` (0x6d6ab4), `HoverDampen` (0x6d6ac0), `HoverHeight` (0x6d6aa8),
  `HoverPad` (0x6c1318), `Locomotor` (0x6dba30), `Speed` (0x6c3334)

### JumpjetLocomotionClass
* RTTI COL 0x6b1b00 -> `.?AVJumpjetLocomotionClass@@`
* ILocomotion vtable 0x6a13bc (50 slots), IPersistStream 0x6a1484
* ctor 0x4f0bb0, scalar deleting dtor 0x4f0b70
* layout: base only (0x0C bytes)
* overrides (11): 4,5,6,16,17,18,19,29,32,39
* movement parameters (TechnoTypeClass INI):
  `JumpJet` (0x6d...), `Locomotor` (0x6dba30), `Speed` (0x6c3334),
  `PitchSpeed` (0x6dba3c), `Weight` (0x6db9bc), `AccelerationFactor` (0x6db9c4),
  `DeaccelerationFactor` (0x6db9d8)

### MechLocomotionClass
* RTTI COL 0x6b27a8 -> `.?AVMechLocomotionClass@@`
* ILocomotion vtable 0x6a1fc4 (50 slots), IPersistStream 0x6a208c
* ctor 0x5453b0, scalar deleting dtor 0x545370
* layout: base only (0x0C bytes)
* overrides (13): 4,5,6,16,17,18,19,29,30,32,39,40
* movement parameters (TechnoTypeClass INI):
  `Locomotor` (0x6dba30), `Speed` (0x6c3334), `Weight` (0x6db9bc),
  `AccelerationFactor` (0x6db9c4), `DeaccelerationFactor` (0x6db9d8),
  `TrackedUphill` (0x6d6958), `TrackedDownhill` (0x6d6948)

### TeleportLocomotionClass
* RTTI COL 0x6b6eb8 -> `.?AVTeleportLocomotionClass@@`
* ILocomotion vtable 0x6a5cd4 (50 slots), IPersistStream 0x6a5d9c
* ctor 0x622930, scalar deleting dtor 0x6228f0
* layout: base only (0x0C bytes)
* overrides (7): 4,5,16,17,18,29
* movement parameters (TechnoTypeClass INI):
  `Locomotor` (0x6dba30), `Speed` (0x6c3334)

### TunnelLocomotionClass
* RTTI COL 0x6b7538 -> `.?AVTunnelLocomotionClass@@`
* ILocomotion vtable 0x6a64dc (50 slots), IPersistStream 0x6a65a4
* ctor 0x6307d0, scalar deleting dtor 0x630790
* layout: +0x24 dword from [0x7b2e54] /* ? */, +0x2C dword /* ? */, +0x30 dword /* ? */
* overrides (17): 4,5,7,8,9,13,14,15,16,17,18,19,29,32,35,38
* movement parameters (TechnoTypeClass INI):
  `TunnelSpeed` (0x6d6acc), `Tunnels` (0x6d...), `DirtTunnels`, `DirtTrackTunnels`,
  `TrackTunnels` (read by 0x4eaa00), `ZFudgeTunnel` (0x6d...),
  `Locomotor` (0x6dba30), `Speed` (0x6c3334)

### WalkLocomotionClass
* RTTI COL 0x6b7ae8 -> `.?AVWalkLocomotionClass@@`
* ILocomotion vtable 0x6a7118 (50 slots), IPersistStream 0x6a71e0, IPiggyback 0x6a70f4
* ctor 0x650b50, scalar deleting dtor 0x650cc0
* layout: +0x14 IPiggyback vtbl, +0x34 void* m_pPiggybackedObject /* ? */
* overrides (18): 0,1,2,4,5,6,16,17,18,19,29,30,32,39,40,42,43
* movement parameters (InfantryTypeClass INI):
  `WalkFrames` (0x6dc0a0), `WalkRate` (0x6d...), `SpeedType` (0x6dc108),
  `MovementRestrictedTo` (0x6d...), `Locomotor` (0x6dba30), `Speed` (0x6c3334)

## Class factory bindings (TClassFactory)

| factory vtable | class |
|----------------|-------|
| 0x6a4cd0 | DriveLocomotionClass |
| 0x6a4ce8 | JumpjetLocomotionClass |
| 0x6a4d00 | HoverLocomotionClass |
| 0x6a4d18 | TunnelLocomotionClass |
| 0x6a4d30 | WalkLocomotionClass |
| 0x6a4d48 | DropPodLocomotionClass |
| 0x6a4d60 | FlyLocomotionClass |
| 0x6a4d78 | TeleportLocomotionClass |
| 0x6a4d90 | MechLocomotionClass |

## Unresolved items

* ILocomotion slots 17..21, 25..28, 30..31, 34..48 have no call-site evidence
  that pins their exact semantics; they are named `SlotNN` and marked `/* ? */`.
* The exact meaning of the `+0x2C` (Hover) and `+0x24/+0x2C/+0x30` (Tunnel)
  members is not determined; they are marked `/* ? */`.
* `LocomotionClass` has no `IPiggyback` subobject; only Drive, DropPod and Walk
  expose one (confirmed by the presence of a third vtable in their ctors).
