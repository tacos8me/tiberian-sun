# Core class hierarchy — reconstruction index

Reconstructed from `bin/GAME.EXE` (C&C Tiberian Sun, 1999, MSVC 6.0, PE32 i386,
ImageBase 0x400000).  Every declaration in the headers under this directory
cites its evidence (RTTI class descriptor VA, vtable VA, constructor VA, or
INI key string).  Items that could not be pinned down are marked `/* ? */`.

## Method

1. Inheritance and base displacements were taken verbatim from the MSVC RTTI
   base-class arrays in `out/classes.json` (`bases[].mdisp/pdisp/vdisp`).
2. Virtual function order was read directly from the binary's vtables
   (`out/vtables.txt` for the vtable VA, then the raw vtable entries from
   `bin/GAME.EXE`).  Each entry was resolved against `out/functions.json`.
3. Data-member offsets were recovered from each class's constructor, which
   writes the vtable pointer and then initialises every member in order.
   Constructors were located by searching the binary for the store of the
   class's vtable address.
4. INI field names were taken from `out/ini_keys.json`; the function that
   references a given key string is that class's `Read_INI` implementation.

## Summary table

| Class | Base class | vtable VA | #virtuals | #data members recovered | Confidence |
|---|---|---|---|---|---|
| AbstractClass | IPersistStream, IRTTITypeInfo | 0x698a60 (primary), 0x698a44 (IRTTITypeInfo) | 24 + 31 | 3 | high |
| AbstractTypeClass | AbstractClass | 0x698ae8 (primary), 0x698acc (IRTTITypeInfo) | 27 + 34 | 2 | high |
| ObjectTypeClass | AbstractTypeClass | 0x6a2a90 (primary), 0x6a2a74 (IRTTITypeInfo) | 40 + 47 | 17 | medium |
| TechnoTypeClass | ObjectTypeClass | 0x6a5bf0 (primary), 0x6a5bd4 (IRTTITypeInfo) | 52 + 59 | 10 | medium |
| ObjectClass | AbstractClass | 0x6a2880 (primary), 0x6a2864 (IRTTITypeInfo) | 105 + 112 | 22 | high |
| TechnoClass | RadioClass, FlasherClass, StageClass | 0x6a57e0 (primary), 0x6a57c4 (IRTTITypeInfo) | 228 + 235 | 78 | medium |
| BuildingTypeClass | TechnoTypeClass | 0x69a758 (primary), 0x69a73c (IRTTITypeInfo) | 46 + 53 | 64 | medium |
| UnitTypeClass | TechnoTypeClass | 0x6a6a90 (primary), 0x6a6a74 (IRTTITypeInfo) | 45 + 52 | 23 | medium |
| InfantryTypeClass | TechnoTypeClass | 0x6a0348 (primary), 0x6a032c (IRTTITypeInfo) | 67 + 74 | 28 | medium |
| AircraftTypeClass | TechnoTypeClass | 0x699168 (primary), 0x69914c (IRTTITypeInfo) | 45 + 52 | 5 | high |
| HouseClass | AbstractClass, IHouse, IPublicHouse, IConnectionPointContainer | 0x69fa18 (primary), 0x69f994 (IRTTITypeInfo), 0x69f9fc (IHouse), 0x69f9bc (IPublicHouse), 0x69f97c (IConnPtCont) | 80 + 113 + 119 + 103 + 87 | 20 | medium |
| HouseTypeClass | AbstractTypeClass | 0x69fbe0 (primary), 0x69fbc4 (IRTTITypeInfo) | 27 + 34 | 24 | medium |

"#virtuals" lists the primary vtable entry count followed by the
IRTTITypeInfo sub-object vtable entry count.  The primary count includes the
RTTI CompleteObjectLocator pointer slot (slot 6), which is data, not code.

## RTTI / vtable evidence

| Class | RTTI COL VA | TypeDescriptor name string |
|---|---|---|
| AbstractClass | 0x6a94d8 | `.?AVAbstractClass@@` @ 0x6bf0f8 |
| AbstractTypeClass | 0x6a9550 | `.?AVAbstractTypeClass@@` @ 0x6bf118 |
| ObjectTypeClass | 0x6b3488 | `.?AVObjectTypeClass@@` @ 0x6bf9d0 |
| TechnoTypeClass | 0x6b6e30 | `.?AVTechnoTypeClass@@` @ 0x6bf9f0 |
| ObjectClass | 0x6b3388 | `.?AVObjectClass@@` @ 0x6bf538 |
| TechnoClass | 0x6b6ce0 | `.?AVTechnoClass@@` @ 0x6bf598 |
| BuildingTypeClass | 0x6aa500 | `.?AVBuildingTypeClass@@` @ 0x6c0828 |
| UnitTypeClass | 0x6b7698 | `.?AVUnitTypeClass@@` @ 0x6dc030 |
| InfantryTypeClass | 0x6b0420 | `.?AVInfantryTypeClass@@` @ 0x6c99f0 |
| AircraftTypeClass | 0x6a9880 | `.?AVAircraftTypeClass@@` @ 0x6bfa10 |
| HouseClass | 0x6afe60 | `.?AVHouseClass@@` @ 0x6c9148 |
| HouseTypeClass | 0x6b0228 | `.?AVHouseTypeClass@@` @ 0x6c9818 |

## Constructors (member-offset evidence)

| Class | Constructor VA | Notes |
|---|---|---|
| AbstractClass | 0x4056f0 | writes vtbl 0x698a60 / 0x698a44 |
| AbstractTypeClass | 0x405d50 | writes vtbl 0x698ae8 / 0x698acc |
| ObjectTypeClass | 0x571220 | writes vtbl 0x6a2a90 / 0x6a2a74 |
| TechnoTypeClass | 0x61ed9c | writes vtbl 0x6a5bf0 / 0x6a5bd4 |
| ObjectClass | 0x56e420 | writes vtbl 0x6a2880 / 0x6a2864 |
| TechnoClass | 0x60e7b0 | writes vtbl 0x6a57e0 / 0x6a57c4 |
| BuildingTypeClass | 0x43dc10 | writes vtbl 0x69a758 / 0x69a73c |
| UnitTypeClass | 0x63e470 | writes vtbl 0x6a6a90 / 0x6a6a74 |
| InfantryTypeClass | 0x4d3200 | writes vtbl 0x6a0348 / 0x6a032c |
| AircraftTypeClass | 0x40f4a0 | writes vtbl 0x699168 / 0x69914c |
| HouseClass | 0x4b5100 | writes all five vtbl pointers |
| HouseTypeClass | 0x4c7290 | writes vtbl 0x69fbe0 / 0x69fbc4 |

## INI readers (rules.ini field-name evidence)

| Class | Read_INI VA | INI keys referenced |
|---|---|---|
| ObjectTypeClass | 0x572610 | Image, AlphaImage, CrushSound, Crushable, RadarInvisible, Selectable, LegalTarget, Strength, Armor, Immune, Insignificant, HasRadialIndicator, IgnoresFirestorm, NewTheater, Theater, Voxel, RadialColor, `.SHP` |
| TechnoTypeClass | 0x61f590 | GuardRange, WalkRate, TypeImmune, MoveToShroud, IsTrain, DoubleOwned, Explodes, FlightLevel, IsDropship, PitchAngle |
| TechnoTypeClass | 0x6213b0 | Cameo, XXICON, `.SHP` |
| BuildingTypeClass | 0x43f250 | HasSpotlight, HalfDamageSmokeLocation1/2, Radar, WaterBound, Adjacent, Capturable, Unsellable, Wall, Powered, Helipad, Factory, FreeUnit, BaseNormal, Weeder |
| BuildingTypeClass | 0x43f45f | AIBuildThis, Armory, Artillary, BridgeRepairHut, ChargeAnim, CloakGenerator, CloakRadiusInCells, ConstructionYard, DockUnload, EMPulseCannon, ExitCoord, FirestormWall, Flat, GDIBarracks, Gate, GateCloseDelay, HasStupidGuardMode, Height, Hospital, HoverPad, ICBMLauncher, InvisibleInGame, IsBaseDefense, IsPlug, IsTemple, IsThreatRatingNode, LaserFence, LaserFencePost, LightBlueTint, LightGreenTint, LightIntensity, LightRedTint, LightVisibility, NODBarracks, NukeSilo, PlaceAnywhere, Power, PowersUpBuilding, PowersUpToLevel, Recoilless, Refinery, SensorArray, SiloDamage, SuperWeapon, SuperWeapon2, TickTank, ToOverlay, TogglePower, UnitReload, UnitRepair, WeaponsFactory |
| BuildingTypeClass | 0x43fb5d | ActiveAnim*, SpecialAnim*, TurretAnim*, ProductionAnim*, PreProductionAnim*, Buildup, Foundation, DoorStages, GateStages, Upgrades, ZShapePointMove, TerrainPalette, ExtraLight, ... |
| UnitTypeClass | 0x63e8e0 | CrateGoodie, NoMovingFire, DeployToFire, Harvester, Weeder, SpeedType, AltImage, CarriesCrate, FiringFrames, HalfDamageSmokeLocation, IsTilter, LargeVisceroid, MovementRestrictedTo, Passive, SmallVisceroid, TooBigToFitUnderBridge, UseTurretShadow, WalkFrames |
| InfantryTypeClass | 0x4d3980 | VoiceComment, Cyborg, Fearless, Fraidycat, Infiltrate, Civilian, Agent, Crawls, Disguised, Doggie, Engineer, FireProne, FireUp, JumpJet, Thief, TiberiumProof, VehicleThief |
| AircraftTypeClass | 0x40f300 | Landable, Carryall, Rotors, CustomRotor |
| HouseTypeClass | 0x4c74d0 | Suffix, Color, Prefix, Firepower, Groundspeed, Airspeed, Armor, BuildTime, Cost, Multiplay, MultiplayPassive, Side, SmartAI, WallOwner |

## Unresolved / low-confidence items

- **TechnoClass virtuals 64..227** — the vtable entries are present and their
  target VAs are known, but the disassembler did not emit standalone function
  records for many of them, so no behavioural summary (strings/callees) could
  be attached.  They are declared as `vfunc_N()` placeholders.
- **ObjectClass virtuals 64..104** — same reason (targets 0x56ec20..0x570d50).
- **Data-member names** — offsets are exact (from constructors), but only the
  members whose INI key could be matched to a specific offset are given real
  names.  The rest are `m_FieldNNN` placeholders.  In particular the large
  `+0x470..+0x812` blocks of the four concrete type classes are initialised by
  their constructors but the individual INI-key-to-offset mapping was not
  recovered, so the key names are listed in a comment block instead.
- **HouseClass** — the class is ~0x10e20 bytes; only the members written by the
  constructor were recovered.  The embedded container sub-objects at
  +0x358..+0x3c8, +0x4ec, +0x514, +0x52c are noted but not expanded.
- **RadioClass / MissionClass / FlasherClass / StageClass / AbstractTypeClass /
  IHouse / IPublicHouse / IConnectionPointContainer** — these bases are
  declared inline in the header that needs them because their own headers are
  outside this task's file set.  Their vtable slot lists are abbreviated to the
  slots that are actually overridden in the derived class.

## Files

- `AbstractClass.h`
- `ObjectTypeClass.h` (also declares `AbstractTypeClass`)
- `TechnoTypeClass.h`
- `ObjectClass.h`
- `TechnoClass.h` (also declares `RadioClass`, `MissionClass`, `FlasherClass`, `StageClass`)
- `BuildingTypeClass.h`
- `UnitTypeClass.h`
- `InfantryTypeClass.h`
- `AircraftTypeClass.h`
- `HouseClass.h` (also declares `IHouse`, `IPublicHouse`, `IConnectionPointContainer`)
- `HouseTypeClass.h`
- `INDEX.md`

All headers compile cleanly with `g++ -std=c++03 -fsyntax-only`.
