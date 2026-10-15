# Map / display / UI layer — reconstruction index

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

| Class | Base class | vtable VA | #virtuals | #members recovered | Confidence |
|---|---|---|---|---|---|
| CellClass | AbstractClass | 0x69ae20 (primary), 0x69ae04 (IRTTITypeInfo) | 24 + 31 | 24 | medium |
| MapClass | GScreenClass | 0x6a19f0 | 127 | 22 | medium |
| DisplayClass | MapClass | 0x69bb08 | 82 | 0 (base only) | medium |
| DisplayClass::TacticalClass | GadgetClass, LinkClass | 0x69ba88 | 114 | 0 | low |
| RadarClass | DisplayClass | 0x6a35fc | 51 | 6 | medium |
| RadarClass::RTacticalClass | GadgetClass, LinkClass | 0x6a357c, 0x704a04 | 83 + 54 | 0 | low |
| SidebarClass | PowerClass | 0x6a4480 | 51 | 5 | medium |
| SidebarClass::SBGadgetClass | GadgetClass, LinkClass | 0x6a437c | 116 | 0 | low |
| SidebarClass::StripClass::SelectClass | ControlClass, GadgetClass, LinkClass | 0x6a43fc | 84 | 0 | low |
| AnimClass | ObjectClass, StageClass | 0x699890 (primary), 0x699874 (IRTTITypeInfo) | 107 + 114 | 25 | medium |
| BulletClass | ObjectClass | 0x69a84c (primary), 0x69a830 (IRTTITypeInfo) | 108 + 115 | 2 | medium |

"#virtuals" lists the primary vtable entry count followed by the
IRTTITypeInfo sub-object vtable entry count.  The primary count includes the
RTTI CompleteObjectLocator pointer slot (slot 6), which is data, not code.

## RTTI / vtable evidence

| Class | RTTI COL VA | TypeDescriptor name string |
|---|---|---|
| CellClass | 0x6aab90 | `.?AVCellClass@@` @ 0x6c1430 |
| MapClass | 0x6b2228 | `.?AVMapClass@@` @ 0x6be698 |
| DisplayClass | 0x6ad4f8 | `.?AVDisplayClass@@` @ 0x6be6b0 |
| DisplayClass::TacticalClass | 0x6ad4b8 | `.?AVTacticalClass@DisplayClass@@` @ 0x6be6d0 |
| RadarClass | 0x6b3a78 | `.?AVRadarClass@@` @ 0x6be6d0 |
| RadarClass::RTacticalClass | 0x6b3a30 | `.?AVRTacticalClass@RadarClass@@` @ 0x6be6e8 |
| SidebarClass | 0x6b4cf8 | `.?AVSidebarClass@@` @ 0x6be710 |
| SidebarClass::SBGadgetClass | 0x6b4c50 | `.?AVSBGadgetClass@SidebarClass@@` @ 0x6be728 |
| SidebarClass::StripClass::SelectClass | 0x6b4ca8 | `.?AVSelectClass@StripClass@SidebarClass@@` @ 0x6be748 |
| AnimClass | 0x6a9b40 | `.?AVAnimClass@@` @ 0x6bfc50 |
| BulletClass | 0x6aa580 | `.?AVBulletClass@@` @ 0x6c13f0 |

## Constructors (member-offset evidence)

| Class | Constructor VA | Notes |
|---|---|---|
| CellClass | 0x44f110 | full member init; stream ctor 0x456900, copy ctor 0x45a7e0 |
| MapClass | 0x501b90 | full member init; stream ctor 0x501a40 |
| DisplayClass | 0x5dcd70 | writes vtable only |
| RadarClass | 0x5a4e20 | full member init; stream ctor 0x5a0980 |
| SidebarClass | 0x5d8940 | writes RadarClass vtbl then SidebarClass vtbl |
| AnimClass | 0x412f30 | full member init; stream ctor 0x415860, copy ctor 0x413700 |
| BulletClass | 0x444e40 | writes vtable only; stream ctor 0x445200 |

## INI keys referenced by this layer

| INI key | Referencing function | Belongs to |
|---|---|---|
| `CellTags` | 0x47661d | map trigger loader (CellClass tag list) |
| `RadarEventColorSpeed`, `RadarEventDurations`, `RadarEventMinRadius`, `RadarEventRotationSpeed`, `RadarEventSpeed`, `RadarEventSuppressionDistances`, `RadarEventVisibilityDurations`, `RadarCombatFlashTime` | 0x5b3451 | rules reader (radar event config) |
| `FreeRadar` | 0x5c6e8e, 0x5c7410 | scenario reader |
| `BuildRadar` | 0x5b4f7b | rules reader |
| `SidebarImage` | 0x5f3140 | sidebar art loader |
| `AnimLow`, `AnimHigh`, `AnimRate`, `AnimPalette` | 0x445eae | anim type reader |
| `IsAnimated`, `AnimationRate`, `AnimationProbability`, `TemperateOccupationBits`, `SnowOccupationBits` | 0x6253d8 | anim/terrain type reader |
| `IsTiberium`, `IsMeteor`, `SpawnCount`, `IsVeins`, `TiberiumSpreadRadius`, `TiberiumSpawnType` | 0x418462 | anim type reader |
| `IsAnimatedTiberium`, `ShouldFogRemove`, `IsFlamingGuy`, `RunningFrames`, `YDrawOffset`, `StartSound`, `BounceSound`, `ExpireSound` | 0x418503 | anim type reader |
| `ExpireAnim` | 0x4186f4 | anim type reader |
| `TrailerAnim` | 0x4187b4 | anim type reader |
| `Bouncer`, `Tiled`, `ShouldUseCellDrawer`, `UseNormalLight`, `RandomRate` | 0x4188df | anim type reader |
| `Projectile` | 0x6571be | weapon type reader (BulletClass spawn) |
| `AmbientDamage`, `IsSonic`, `Supress`, `Burst`, `Damage`, `Speed`, `Range`, `MinimumRange` | 0x656d40 | weapon type reader |

## Unresolved items

- **DisplayClass data members**: constructor 0x5dcd70 writes only the vtable.
  The DisplayClass-specific members are initialised by the derived
  RadarClass/SidebarClass constructors, so their exact boundaries are not
  separable from the base MapClass layout without further analysis.
- **BulletClass members beyond +0x50**: neither constructor initialises the
  velocity/damage/warhead fields; they are set by the firing code.  Offsets
  unresolved.
- **SidebarClass StripClass array**: the object is at least 0x1514 bytes, but
  the layout of the StripClass array between +0x18 and +0x1508 is not resolved.
- **Nested gadget classes** (TacticalClass, RTacticalClass, SBGadgetClass,
  SelectClass): their vtables are fully enumerated, but no constructor was
  located by the vtable-store search (they are constructed inline by their
  owners), so no member offsets are reported.
- **RadarClass::RTacticalClass second vtable 0x704a04** (in `.data`, 54
  entries): entries point into `.data` (0x6b3c6d..0x6bafac), i.e. a
  secondary/alternate dispatch table rather than the live vtable.  Its role is
  not determined.
- **MapClass vtable slots 29/36/43/50/57/64/71/78/85/92/99/106/113/120** are
  RTTI CompleteObjectLocator pointers (0x6b2190, 0x6b21f8, 0x6b22a0, 0x6b22d0,
  0x6b2300, 0x6b2330, 0x6b2398, 0x6b2400, 0x6b2430, 0x6b2498, 0x6b24e8,
  0x6b2518, 0x6b2548), not code.  The same pattern applies to the other
  multi-inheritance classes.
- **Member names** are inferred from surrounding map/INI semantics; every one
  is marked `/* ? */` in the headers because no debug symbols survive.
