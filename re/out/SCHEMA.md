# rules.ini key -> class schema (recovered from GAME.EXE)

Every row is a literal INI key string referenced by the named reader
function.  The reader VA is the function that pushes the key string and
then calls an INIClass accessor; the accessor determines the inferred C++
type (GetBool -> bool, GetInt -> int, GetDouble -> double, GetString ->
char[N]).  Keys whose accessor could not be resolved are typed `/* ? */`.

Accessor VAs (all `__thiscall` on INIClass, `this` in ECX):

| VA | accessor | evidence |
|---|---|---|
| `0x4D60B0` | GetString | 3 args, returns char* |
| `0x4D6960` | GetString | 3 args, returns char* |
| `0x4D6ED0` | GetString | 4 args (buf,len), returns int |
| `0x4D70B0` | GetString | 3 args, returns char* |
| `0x4D54C0` | GetString | 3 args, returns char* |
| `0x4D50F0` | GetString | 3 args, returns char* |
| `0x4D6030` | GetString | 3 args, returns char* |
| `0x4D72B0` | GetString | 3 args, returns char* |
| `0x4D6290` | GetString | 3 args, returns char* |
| `0x4D4250` | GetString | 3 args, returns StringClass |
| `0x4D6B50` | GetInt | 3 args, hashes key via 0x46D710 |
| `0x4D5D50` | GetInt | 3 args, index variant |
| `0x4D6B00` | GetDouble | 3 args, sprintf "%f" via 0x682AAE |
| `0x4D7490` | GetBool | 3 args, returns bool |
| `0x4D5C60` | GetBool | 3 args, returns bool |
| `0x4D7070` | GetBool | pushes "yes" (0x6C9C28) then GetInt |

## Weapon / warhead / anim / bullet type classes

### WeaponTypeClass (reader `0x656d40`)

| INI key | owning class | inferred C++ type | evidence fn VA |
|---|---|---|---|
| `AmbientDamage` | WeaponTypeClass | char[64] | `0x656d40` |
| `Anim` | WeaponTypeClass | char[64] /* id */ | `0x656d40` |
| `Burst` | WeaponTypeClass | char[64] | `0x656d40` |
| `Damage` | WeaponTypeClass | char[64] | `0x656d40` |
| `IsSonic` | WeaponTypeClass | char[64] | `0x656d40` |
| `MinimumRange` | WeaponTypeClass | double | `0x656d40` |
| `Range` | WeaponTypeClass | double | `0x656d40` |
| `Report` | WeaponTypeClass | char[64] /* sound id */ | `0x656d40` |
| `Speed` | WeaponTypeClass | int | `0x656d40` |
| `Supress` | WeaponTypeClass | char[64] | `0x656d40` |

### WarheadTypeClass (reader `0x656f60`)

| INI key | owning class | inferred C++ type | evidence fn VA |
|---|---|---|---|
| `AttachedParticleSystem` | WarheadTypeClass | char[64] | `0x656f60` |
| `Bright` | WarheadTypeClass | char[64] | `0x656f60` |
| `Camera` | WarheadTypeClass | char[64] | `0x656f60` |
| `Charges` | WarheadTypeClass | char[64] | `0x656f60` |
| `IonSensitive` | WarheadTypeClass | char[64] | `0x656f60` |
| `IsBigLaser` | WarheadTypeClass | char[64] | `0x656f60` |
| `IsLaser` | WarheadTypeClass | char[64] | `0x656f60` |
| `IsRailgun` | WarheadTypeClass | char[64] | `0x656f60` |
| `LaserDuration` | WarheadTypeClass | char[64] | `0x656f60` |
| `Lobber` | WarheadTypeClass | char[64] | `0x656f60` |
| `TurboBoost` | WarheadTypeClass | char[64] | `0x656f60` |
| `UseFireParticles` | WarheadTypeClass | char[64] | `0x656f60` |
| `UseSparkParticles` | WarheadTypeClass | char[64] | `0x656f60` |
| `Warhead` | WarheadTypeClass | char[64] | `0x656f60` |

### AnimTypeClass (reader `0x417fd0`)

| INI key | owning class | inferred C++ type | evidence fn VA |
|---|---|---|---|
| `AltPalette` | AnimTypeClass | char[64] | `0x417fd0` |
| `Crater` | AnimTypeClass | char[64] | `0x417fd0` |
| `Flamer` | AnimTypeClass | char[64] | `0x417fd0` |
| `Flat` | AnimTypeClass | char[64] | `0x417fd0` |
| `NewTheater` | AnimTypeClass | char[64] | `0x417fd0` |
| `Normalized` | AnimTypeClass | char[64] | `0x417fd0` |
| `PingPong` | AnimTypeClass | char[64] | `0x417fd0` |
| `Rate` | AnimTypeClass | char[64] | `0x417fd0` |
| `Report` | AnimTypeClass | char[64] /* sound id */ | `0x417fd0` |
| `Reverse` | AnimTypeClass | char[64] | `0x417fd0` |
| `Scorch` | AnimTypeClass | char[64] | `0x417fd0` |
| `Sticky` | AnimTypeClass | char[64] | `0x417fd0` |
| `Surface` | AnimTypeClass | char[64] | `0x417fd0` |
| `Theater` | AnimTypeClass | char[64] | `0x417fd0` |
| `TiberiumChainReaction` | AnimTypeClass | char[64] | `0x417fd0` |
| `Translucent` | AnimTypeClass | char[64] | `0x417fd0` |

### BulletTypeClass (reader `0x445ba0`)

| INI key | owning class | inferred C++ type | evidence fn VA |
|---|---|---|---|
| `Acceleration` | BulletTypeClass | char[64] | `0x445ba0` |
| `Airburst` | BulletTypeClass | char[64] | `0x445ba0` |
| `Arcing` | BulletTypeClass | char[64] | `0x445ba0` |
| `Bouncy` | BulletTypeClass | char[64] | `0x445ba0` |
| `Cluster` | BulletTypeClass | char[64] | `0x445ba0` |
| `Degenerates` | BulletTypeClass | char[64] | `0x445ba0` |
| `Dropping` | BulletTypeClass | char[64] | `0x445ba0` |
| `Elasticity` | BulletTypeClass | char[64] | `0x445ba0` |
| `Floater` | BulletTypeClass | char[64] | `0x445ba0` |
| `High` | BulletTypeClass | char[64] | `0x445ba0` |
| `Image` | BulletTypeClass | char[64] /* id */ | `0x445ba0` |
| `Inaccuate` | BulletTypeClass | char[64] | `0x445ba0` |
| `Inviso` | BulletTypeClass | char[64] | `0x445ba0` |
| `Proximity` | BulletTypeClass | char[64] | `0x445ba0` |
| `Ranged` | BulletTypeClass | char[64] | `0x445ba0` |
| `Shadow` | BulletTypeClass | char[64] | `0x445ba0` |
| `Trailer` | BulletTypeClass | char[64] | `0x445ba0` |
| `VeryHigh` | BulletTypeClass | char[64] | `0x445ba0` |

## RulesClass sections

### RulesClass (reader `0x5b2343`)

| INI key | owning class | inferred C++ type | evidence fn VA |
|---|---|---|---|
| `AIAlternateProductionCreditCutoff` | RulesClass | char[64] | `0x5b2343` |
| `AIBuildsWalls` | RulesClass | char[64] | `0x5b2343` |
| `AIUseTurbineUpgradeProbability` | RulesClass | char[64] | `0x5b2343` |
| `AttackingAircraftSightRange` | RulesClass | char[64] | `0x5b2343` |
| `BaseBias` | RulesClass | char[64] | `0x5b2343` |
| `BaseDefenseDelay` | RulesClass | char[64] | `0x5b2343` |
| `BlendedFog` | RulesClass | char[64] | `0x5b2343` |
| `BuildSpeed` | RulesClass | char[64] | `0x5b2343` |
| `BuildupTime` | RulesClass | char[64] | `0x5b2343` |
| `CloakDelay` | RulesClass | char[64] | `0x5b2343` |
| `CloseEnough` | RulesClass | double | `0x5b2343` |
| `CurleyShuffle` | RulesClass | char[64] | `0x5b2343` |
| `DamageDelay` | RulesClass | char[64] | `0x5b2343` |
| `DissolveUnfilledTeamDelay` | RulesClass | char[64] | `0x5b2343` |
| `FineDiffControl` | RulesClass | char[64] | `0x5b2343` |
| `GameSpeedBias` | RulesClass | char[64] | `0x5b2343` |
| `GrowthRate` | RulesClass | char[64] | `0x5b2343` |
| `HarvesterDumpRate` | RulesClass | char[64] | `0x5b2343` |
| `HarvesterLoadRate` | RulesClass | char[64] | `0x5b2343` |
| `IRepairRate` | RulesClass | char[64] | `0x5b2343` |
| `IRepairStep` | RulesClass | char[64] | `0x5b2343` |
| `IonLightningDamage` | RulesClass | char[64] | `0x5b2343` |
| `IonLightningFrequency` | RulesClass | char[64] | `0x5b2343` |
| `IonLightningRandomness` | RulesClass | char[64] | `0x5b2343` |
| `IonStormDuration` | RulesClass | char[64] | `0x5b2343` |
| `IonStormWarhead` | RulesClass | char[64] | `0x5b2343` |
| `IonStormWarning` | RulesClass | char[64] | `0x5b2343` |
| `LeptonsPerSightIncrease` | RulesClass | char[64] | `0x5b2343` |
| `NodAIBuildsWalls` | RulesClass | char[64] | `0x5b2343` |
| `RefundPercent` | RulesClass | char[64] | `0x5b2343` |
| `ReloadRate` | RulesClass | char[64] | `0x5b2343` |
| `RepairPercent` | RulesClass | char[64] | `0x5b2343` |
| `RepairRate` | RulesClass | char[64] | `0x5b2343` |
| `RepairStep` | RulesClass | char[64] | `0x5b2343` |
| `SeparateAircraft` | RulesClass | char[64] | `0x5b2343` |
| `Stray` | RulesClass | double | `0x5b2343` |
| `SurvivorDivisor` | RulesClass | char[64] | `0x5b2343` |
| `SurvivorRate` | RulesClass | char[64] | `0x5b2343` |
| `SuspendDelay` | RulesClass | char[64] | `0x5b2343` |
| `SuspendPriority` | RulesClass | char[64] | `0x5b2343` |
| `TiberiumTransmogrify` | RulesClass | char[64] | `0x5b2343` |
| `URepairRate` | RulesClass | char[64] | `0x5b2343` |
| `UseMinDefenseRule` | RulesClass | char[64] | `0x5b2343` |

### RulesClass::General (reader `0x5b3451`)

| INI key | owning class | inferred C++ type | evidence fn VA |
|---|---|---|---|
| `AITriggerFailureWeightDelta` | RulesClass::General | char[64] | `0x5b3451` |
| `AITriggerSuccessWeightDelta` | RulesClass::General | char[64] | `0x5b3451` |
| `AITriggerTrackRecordCoefficient` | RulesClass::General | char[64] | `0x5b3451` |
| `BestLowPowerBuildRateCoefficient` | RulesClass::General | char[64] | `0x5b3451` |
| `ChargeToDrainRatio` | RulesClass::General | char[64] | `0x5b3451` |
| `ConditionRedSparkingProbability` | RulesClass::General | char[64] | `0x5b3451` |
| `ConditionYellowSparkingProbability` | RulesClass::General | char[64] | `0x5b3451` |
| `DamageToFirestormDamageCoefficient` | RulesClass::General | char[64] | `0x5b3451` |
| `DumbMyEffectivenessCoefficient` | RulesClass::General | char[64] | `0x5b3451` |
| `DumbTargetDistanceCoefficient` | RulesClass::General | char[64] | `0x5b3451` |
| `DumbTargetEffectivenessCoefficient` | RulesClass::General | char[64] | `0x5b3451` |
| `DumbTargetSpecialThreatCoefficient` | RulesClass::General | char[64] | `0x5b3451` |
| `DumbTargetStrengthCoefficient` | RulesClass::General | char[64] | `0x5b3451` |
| `EnemyHouseThreatBonus` | RulesClass::General | char[64] | `0x5b3451` |
| `FlashFrameTime` | RulesClass::General | char[64] | `0x5b3451` |
| `HunterSeekerAscentSpeed` | RulesClass::General | char[64] | `0x5b3451` |
| `HunterSeekerDescendProximity` | RulesClass::General | char[64] | `0x5b3451` |
| `HunterSeekerDescentSpeed` | RulesClass::General | char[64] | `0x5b3451` |
| `HunterSeekerDetonateProximity` | RulesClass::General | char[64] | `0x5b3451` |
| `HunterSeekerEmergeSpeed` | RulesClass::General | char[64] | `0x5b3451` |
| `MaxVeinholeGrowth` | RulesClass::General | char[64] | `0x5b3451` |
| `MaxWaypointPathLength` | RulesClass::General | char[64] | `0x5b3451` |
| `MaximumQueuedObjects` | RulesClass::General | char[64] | `0x5b3451` |
| `MyEffectivenessCoefficientDefault` | RulesClass::General | char[64] | `0x5b3451` |
| `RadarCombatFlashTime` | RulesClass::General | char[64] | `0x5b3451` |
| `RadarEventColorSpeed` | RulesClass::General | char[64] | `0x5b3451` |
| `RadarEventMinRadius` | RulesClass::General | char[64] | `0x5b3451` |
| `RadarEventRotationSpeed` | RulesClass::General | char[64] | `0x5b3451` |
| `RadarEventSpeed` | RulesClass::General | char[64] | `0x5b3451` |
| `RevealTriggerRadius` | RulesClass::General | char[64] | `0x5b3451` |
| `SpotlightAcceleration` | RulesClass::General | char[64] | `0x5b3451` |
| `SpotlightAngle` | RulesClass::General | char[64] | `0x5b3451` |
| `SpotlightLocationRadius` | RulesClass::General | char[64] | `0x5b3451` |
| `SpotlightMovementRadius` | RulesClass::General | char[64] | `0x5b3451` |
| `SpotlightRadius` | RulesClass::General | char[64] | `0x5b3451` |
| `SpotlightSpeed` | RulesClass::General | char[64] | `0x5b3451` |
| `TargetDistanceCoefficientDefault` | RulesClass::General | char[64] | `0x5b3451` |
| `TargetEffectivenessCoefficientDefault` | RulesClass::General | char[64] | `0x5b3451` |
| `TargetSpecialThreatCoefficientDefault` | RulesClass::General | char[64] | `0x5b3451` |
| `TargetStrengthCoefficientDefault` | RulesClass::General | char[64] | `0x5b3451` |
| `TreeStrength` | RulesClass::General | char[64] | `0x5b3451` |
| `VeinDamage` | RulesClass::General | char[64] | `0x5b3451` |
| `VeinholeGrowthRate` | RulesClass::General | char[64] | `0x5b3451` |
| `VeinholeMonsterStrength` | RulesClass::General | char[64] | `0x5b3451` |
| `VeinholeShrinkRate` | RulesClass::General | char[64] | `0x5b3451` |
| `WallBuildSpeedCoefficient` | RulesClass::General | char[64] | `0x5b3451` |
| `WeedCapacity` | RulesClass::General | char[64] | `0x5b3451` |
| `WorstLowPowerBuildRateCoefficient` | RulesClass::General | char[64] | `0x5b3451` |

### RulesClass::AI (reader `0x5b504f`)

| INI key | owning class | inferred C++ type | evidence fn VA |
|---|---|---|---|
| `AALimit` | RulesClass::AI | char[64] | `0x5b504f` |
| `AARatio` | RulesClass::AI | char[64] | `0x5b504f` |
| `AIBaseSpacing` | RulesClass::AI | char[64] | `0x5b504f` |
| `AirstripLimit` | RulesClass::AI | char[64] | `0x5b504f` |
| `AirstripRatio` | RulesClass::AI | char[64] | `0x5b504f` |
| `AttackDelay` | RulesClass::AI | char[64] | `0x5b504f` |
| `AttackInterval` | RulesClass::AI | char[64] | `0x5b504f` |
| `AutocreateTime` | RulesClass::AI | char[64] | `0x5b504f` |
| `BarracksLimit` | RulesClass::AI | char[64] | `0x5b504f` |
| `BarracksRatio` | RulesClass::AI | char[64] | `0x5b504f` |
| `BaseSizeAdd` | RulesClass::AI | char[64] | `0x5b504f` |
| `BlockagePathDelay` | RulesClass::AI | char[64] | `0x5b504f` |
| `CompEasyBonus` | RulesClass::AI | char[64] | `0x5b504f` |
| `ComputerBaseDefenseResponse` | RulesClass::AI | char[64] | `0x5b504f` |
| `CreditReserve` | RulesClass::AI | char[64] | `0x5b504f` |
| `DefenseLimit` | RulesClass::AI | char[64] | `0x5b504f` |
| `DefenseRatio` | RulesClass::AI | char[64] | `0x5b504f` |
| `GDIBaseDefenseCoefficient` | RulesClass::AI | char[64] | `0x5b504f` |
| `GDIWallDefense` | RulesClass::AI | char[64] | `0x5b504f` |
| `GDIWallDefenseCoefficient` | RulesClass::AI | char[64] | `0x5b504f` |
| `HelipadLimit` | RulesClass::AI | char[64] | `0x5b504f` |
| `HelipadRatio` | RulesClass::AI | char[64] | `0x5b504f` |
| `InfantryBaseMult` | RulesClass::AI | char[64] | `0x5b504f` |
| `InfantryReserve` | RulesClass::AI | char[64] | `0x5b504f` |
| `MaximumBaseDefenseValue` | RulesClass::AI | char[64] | `0x5b504f` |
| `NodBaseDefenseCoefficient` | RulesClass::AI | char[64] | `0x5b504f` |
| `Paranoid` | RulesClass::AI | char[64] | `0x5b504f` |
| `PathDelay` | RulesClass::AI | char[64] | `0x5b504f` |
| `PatrolScan` | RulesClass::AI | char[64] | `0x5b504f` |
| `PowerEmergency` | RulesClass::AI | char[64] | `0x5b504f` |
| `PowerSurplus` | RulesClass::AI | char[64] | `0x5b504f` |
| `RefineryLimit` | RulesClass::AI | char[64] | `0x5b504f` |
| `RefineryRatio` | RulesClass::AI | char[64] | `0x5b504f` |
| `TeslaLimit` | RulesClass::AI | char[64] | `0x5b504f` |
| `TeslaRatio` | RulesClass::AI | char[64] | `0x5b504f` |
| `TiberiumFarScan` | RulesClass::AI | double | `0x5b504f` |
| `TiberiumNearScan` | RulesClass::AI | double | `0x5b504f` |
| `WarLimit` | RulesClass::AI | char[64] | `0x5b504f` |
| `WarRatio` | RulesClass::AI | char[64] | `0x5b504f` |

### RulesClass::Power (reader `0x5b11cd`)

| INI key | owning class | inferred C++ type | evidence fn VA |
|---|---|---|---|
| `AircraftFogReveal` | RulesClass::Power | char[64] | `0x5b11cd` |
| `AllowShroudedSubteranneanMoves` | RulesClass::Power | char[64] | `0x5b11cd` |
| `CraterLevel` | RulesClass::Power | char[64] | `0x5b11cd` |
| `DropPodWeapon` | RulesClass::Power | char[64] | `0x5b11cd` |
| `MinProductionSpeed` | RulesClass::Power | char[64] | `0x5b11cd` |
| `MissileROTVar` | RulesClass::Power | char[64] | `0x5b11cd` |
| `MissileSpeedVar` | RulesClass::Power | char[64] | `0x5b11cd` |
| `MultipleFactory` | RulesClass::Power | char[64] | `0x5b11cd` |
| `RevealByHeight` | RulesClass::Power | char[64] | `0x5b11cd` |
| `TiberiumHeal` | RulesClass::Power | char[64] | `0x5b11cd` |
| `TreeFlammability` | RulesClass::Power | char[64] | `0x5b11cd` |
| `ZoomInFactor` | RulesClass::Power | char[64] | `0x5b11cd` |

### RulesClass::Land (reader `0x5b16c6`)

| INI key | owning class | inferred C++ type | evidence fn VA |
|---|---|---|---|
| `CrewEscape` | RulesClass::Land | char[64] | `0x5b16c6` |
| `ExplosiveVoxelDebris` | RulesClass::Land | char[64] | `0x5b16c6` |
| `HoverAcceleration` | RulesClass::Land | char[64] | `0x5b16c6` |
| `HoverBob` | RulesClass::Land | char[64] | `0x5b16c6` |
| `HoverBoost` | RulesClass::Land | char[64] | `0x5b16c6` |
| `HoverBrake` | RulesClass::Land | char[64] | `0x5b16c6` |
| `HoverDampen` | RulesClass::Land | char[64] | `0x5b16c6` |
| `HoverHeight` | RulesClass::Land | char[64] | `0x5b16c6` |
| `TunnelSpeed` | RulesClass::Land | char[64] | `0x5b16c6` |
| `VeteranArmor` | RulesClass::Land | char[64] | `0x5b16c6` |
| `VeteranCap` | RulesClass::Land | char[64] | `0x5b16c6` |
| `VeteranCombat` | RulesClass::Land | char[64] | `0x5b16c6` |
| `VeteranROF` | RulesClass::Land | char[64] | `0x5b16c6` |
| `VeteranRatio` | RulesClass::Land | char[64] | `0x5b16c6` |
| `VeteranSight` | RulesClass::Land | char[64] | `0x5b16c6` |
| `VeteranSpeed` | RulesClass::Land | char[64] | `0x5b16c6` |

### RulesClass::Audio (reader `0x5c7410`)

| INI key | owning class | inferred C++ type | evidence fn VA |
|---|---|---|---|
| `%s,%d` | RulesClass::Audio | int | `0x5c7410` |
| `AltHomeCell` | RulesClass::Audio | char[64] | `0x5c7410` |
| `AltNextScenario` | RulesClass::Audio | int | `0x5c7410` |
| `Briefing` | RulesClass::Audio | bool | `0x5c7410` |
| `CarryOverCap` | RulesClass::Audio | char[64] | `0x5c7410` |
| `CarryOverMoney` | RulesClass::Audio | double | `0x5c7410` |
| `CivEvac` | RulesClass::Audio | char[64] | `0x5c7410` |
| `EndOfGame` | RulesClass::Audio | bool | `0x5c7410` |
| `FillSilos` | RulesClass::Audio | bool | `0x5c7410` |
| `FreeRadar` | RulesClass::Audio | bool | `0x5c7410` |
| `HomeCell` | RulesClass::Audio | char[64] | `0x5c7410` |
| `IceGrowthEnabled` | RulesClass::Audio | bool | `0x5c7410` |
| `IgnoreGlobalAITriggers` | RulesClass::Audio | bool | `0x5c7410` |
| `InitTime` | RulesClass::Audio | char[64] | `0x5c7410` |
| `Lighting` | RulesClass::Audio | char[64] | `0x5c7410` |
| `MultiplayerOnly` | RulesClass::Audio | char[64] | `0x5c7410` |
| `Name` | RulesClass::Audio | int | `0x5c7410` |
| `NewINIFormat` | RulesClass::Audio | char[64] | `0x5c7410` |
| `NextScenario` | RulesClass::Audio | int | `0x5c7410` |
| `Official` | RulesClass::Audio | bool | `0x5c7410` |
| `OneTimeOnly` | RulesClass::Audio | bool | `0x5c7410` |
| `Percent` | RulesClass::Audio | char[64] | `0x5c7410` |
| `SkipMapSelect` | RulesClass::Audio | bool | `0x5c7410` |
| `SkipScore` | RulesClass::Audio | bool | `0x5c7410` |
| `StartingDropships` | RulesClass::Audio | char[64] | `0x5c7410` |
| `TiberiumDeathToVisceroid` | RulesClass::Audio | bool | `0x5c7410` |
| `TiberiumGrowthEnabled` | RulesClass::Audio | bool | `0x5c7410` |
| `TimerInherit` | RulesClass::Audio | bool | `0x5c7410` |
| `TrainCrate` | RulesClass::Audio | bool | `0x5c7410` |
| `TruckCrate` | RulesClass::Audio | bool | `0x5c7410` |
| `VeinGrowthEnabled` | RulesClass::Audio | bool | `0x5c7410` |

### RulesClass::Visual (reader `0x4eaa00`)

| INI key | owning class | inferred C++ type | evidence fn VA |
|---|---|---|---|
| `General` | RulesClass::Visual | char[64] | `0x4eaa00` |
| `LastTilesInSet` | RulesClass::Visual | char[64] | `0x4eaa00` |
| `TilesInSet` | RulesClass::Visual | char[64] | `0x4eaa00` |

### RulesClass::Building (reader `0x43fb5d`)

| INI key | owning class | inferred C++ type | evidence fn VA |
|---|---|---|---|
| `ActiveAnim` | RulesClass::Building | char[64] /* id */ | `0x43fb5d` |
| `ActiveAnimDamaged` | RulesClass::Building | char[64] | `0x43fb5d` |
| `ActiveAnimFour` | RulesClass::Building | char[64] | `0x43fb5d` |
| `ActiveAnimFourDamaged` | RulesClass::Building | char[64] | `0x43fb5d` |
| `ActiveAnimFourPowered` | RulesClass::Building | char[64] | `0x43fb5d` |
| `ActiveAnimFourPoweredLight` | RulesClass::Building | char[64] | `0x43fb5d` |
| `ActiveAnimFourX` | RulesClass::Building | char[64] | `0x43fb5d` |
| `ActiveAnimFourY` | RulesClass::Building | char[64] | `0x43fb5d` |
| `ActiveAnimFourYSort` | RulesClass::Building | char[64] | `0x43fb5d` |
| `ActiveAnimFourZAdjust` | RulesClass::Building | char[64] | `0x43fb5d` |
| `ActiveAnimPowered` | RulesClass::Building | char[64] | `0x43fb5d` |
| `ActiveAnimPoweredLight` | RulesClass::Building | char[64] | `0x43fb5d` |
| `ActiveAnimThree` | RulesClass::Building | char[64] | `0x43fb5d` |
| `ActiveAnimThreeDamaged` | RulesClass::Building | char[64] | `0x43fb5d` |
| `ActiveAnimThreePowered` | RulesClass::Building | char[64] | `0x43fb5d` |
| `ActiveAnimThreePoweredLight` | RulesClass::Building | char[64] | `0x43fb5d` |
| `ActiveAnimThreeX` | RulesClass::Building | char[64] | `0x43fb5d` |
| `ActiveAnimThreeY` | RulesClass::Building | char[64] | `0x43fb5d` |
| `ActiveAnimThreeYSort` | RulesClass::Building | char[64] | `0x43fb5d` |
| `ActiveAnimThreeZAdjust` | RulesClass::Building | char[64] | `0x43fb5d` |
| `ActiveAnimTwo` | RulesClass::Building | char[64] | `0x43fb5d` |
| `ActiveAnimTwoDamaged` | RulesClass::Building | char[64] | `0x43fb5d` |
| `ActiveAnimTwoPowered` | RulesClass::Building | char[64] | `0x43fb5d` |
| `ActiveAnimTwoPoweredLight` | RulesClass::Building | char[64] | `0x43fb5d` |
| `ActiveAnimTwoX` | RulesClass::Building | char[64] | `0x43fb5d` |
| `ActiveAnimTwoY` | RulesClass::Building | char[64] | `0x43fb5d` |
| `ActiveAnimTwoYSort` | RulesClass::Building | char[64] | `0x43fb5d` |
| `ActiveAnimTwoZAdjust` | RulesClass::Building | char[64] | `0x43fb5d` |
| `ActiveAnimX` | RulesClass::Building | char[64] | `0x43fb5d` |
| `ActiveAnimY` | RulesClass::Building | char[64] | `0x43fb5d` |
| `ActiveAnimYSort` | RulesClass::Building | char[64] | `0x43fb5d` |
| `ActiveAnimZAdjust` | RulesClass::Building | char[64] | `0x43fb5d` |
| `AnimActive` | RulesClass::Building | char[64] | `0x43fb5d` |
| `AnimAux1` | RulesClass::Building | char[64] | `0x43fb5d` |
| `AnimAux2` | RulesClass::Building | char[64] | `0x43fb5d` |
| `AnimIdle` | RulesClass::Building | char[64] | `0x43fb5d` |
| `Buildup` | RulesClass::Building | char[64] | `0x43fb5d` |
| `DamagedDoor` | RulesClass::Building | char[64] | `0x43fb5d` |
| `DemandLoad` | RulesClass::Building | char[64] | `0x43fb5d` |
| `DemandLoadBuildup` | RulesClass::Building | char[64] | `0x43fb5d` |
| `DoorStages` | RulesClass::Building | char[64] | `0x43fb5d` |
| `ExtraDamageStage` | RulesClass::Building | char[64] | `0x43fb5d` |
| `ExtraLight` | RulesClass::Building | char[64] | `0x43fb5d` |
| `FreeBuildup` | RulesClass::Building | char[64] | `0x43fb5d` |
| `GateStages` | RulesClass::Building | char[64] | `0x43fb5d` |
| `MidPoint` | RulesClass::Building | char[64] | `0x43fb5d` |
| `NormalZAdjust` | RulesClass::Building | char[64] | `0x43fb5d` |
| `PowerUp%01dYSort` | RulesClass::Building | char[64] | `0x43fb5d` |
| `PreProductionAnim` | RulesClass::Building | char[64] /* id */ | `0x43fb5d` |
| `PreProductionAnimDamaged` | RulesClass::Building | char[64] | `0x43fb5d` |
| `PreProductionAnimX` | RulesClass::Building | char[64] | `0x43fb5d` |
| `PreProductionAnimY` | RulesClass::Building | char[64] | `0x43fb5d` |
| `PreProductionAnimYSort` | RulesClass::Building | char[64] | `0x43fb5d` |
| `PreProductionAnimZAdjust` | RulesClass::Building | char[64] | `0x43fb5d` |
| `PrimaryFirePixelOffset` | RulesClass::Building | char[64] | `0x43fb5d` |
| `ProductionAnim` | RulesClass::Building | char[64] /* id */ | `0x43fb5d` |
| `ProductionAnimDamaged` | RulesClass::Building | char[64] | `0x43fb5d` |
| `ProductionAnimX` | RulesClass::Building | char[64] | `0x43fb5d` |
| `ProductionAnimY` | RulesClass::Building | char[64] | `0x43fb5d` |
| `ProductionAnimYSort` | RulesClass::Building | char[64] | `0x43fb5d` |
| `ProductionAnimZAdjust` | RulesClass::Building | char[64] | `0x43fb5d` |
| `SecondaryFirePixelOffset` | RulesClass::Building | char[64] | `0x43fb5d` |
| `SpecialAnim` | RulesClass::Building | char[64] /* id */ | `0x43fb5d` |
| `SpecialAnimDamaged` | RulesClass::Building | char[64] | `0x43fb5d` |
| `SpecialAnimPowered` | RulesClass::Building | char[64] | `0x43fb5d` |
| `SpecialAnimPoweredLight` | RulesClass::Building | char[64] | `0x43fb5d` |
| `SpecialAnimThree` | RulesClass::Building | char[64] | `0x43fb5d` |
| `SpecialAnimThreeDamaged` | RulesClass::Building | char[64] | `0x43fb5d` |
| `SpecialAnimThreePowered` | RulesClass::Building | char[64] | `0x43fb5d` |
| `SpecialAnimThreePoweredLight` | RulesClass::Building | char[64] | `0x43fb5d` |
| `SpecialAnimThreeX` | RulesClass::Building | char[64] | `0x43fb5d` |
| `SpecialAnimThreeY` | RulesClass::Building | char[64] | `0x43fb5d` |
| `SpecialAnimThreeYSort` | RulesClass::Building | char[64] | `0x43fb5d` |
| `SpecialAnimThreeZAdjust` | RulesClass::Building | char[64] | `0x43fb5d` |
| `SpecialAnimTwo` | RulesClass::Building | char[64] | `0x43fb5d` |
| `SpecialAnimTwoDamaged` | RulesClass::Building | char[64] | `0x43fb5d` |
| `SpecialAnimTwoPowered` | RulesClass::Building | char[64] | `0x43fb5d` |
| `SpecialAnimTwoPoweredLight` | RulesClass::Building | char[64] | `0x43fb5d` |
| `SpecialAnimTwoX` | RulesClass::Building | char[64] | `0x43fb5d` |
| `SpecialAnimTwoY` | RulesClass::Building | char[64] | `0x43fb5d` |
| `SpecialAnimTwoYSort` | RulesClass::Building | char[64] | `0x43fb5d` |
| `SpecialAnimTwoZAdjust` | RulesClass::Building | char[64] | `0x43fb5d` |
| `SpecialAnimX` | RulesClass::Building | char[64] | `0x43fb5d` |
| `SpecialAnimY` | RulesClass::Building | char[64] | `0x43fb5d` |
| `SpecialAnimYSort` | RulesClass::Building | char[64] | `0x43fb5d` |
| `SpecialAnimZAdjust` | RulesClass::Building | char[64] | `0x43fb5d` |
| `SpecialZOverlayZAdjust` | RulesClass::Building | char[64] | `0x43fb5d` |
| `TerrainPalette` | RulesClass::Building | char[64] | `0x43fb5d` |
| `TurretAnim` | RulesClass::Building | char[64] /* id */ | `0x43fb5d` |
| `TurretAnimDamaged` | RulesClass::Building | char[64] | `0x43fb5d` |
| `TurretAnimIsVoxel` | RulesClass::Building | char[64] | `0x43fb5d` |
| `TurretAnimX` | RulesClass::Building | char[64] | `0x43fb5d` |
| `TurretAnimY` | RulesClass::Building | char[64] | `0x43fb5d` |
| `TurretAnimYSort` | RulesClass::Building | char[64] | `0x43fb5d` |
| `TurretAnimZAdjust` | RulesClass::Building | char[64] | `0x43fb5d` |
| `Upgrades` | RulesClass::Building | char[64] | `0x43fb5d` |
| `ZShapePointMove` | RulesClass::Building | char[64] | `0x43fb5d` |

### RulesClass::Infantry (reader `0x62027b`)

| INI key | owning class | inferred C++ type | evidence fn VA |
|---|---|---|---|
| `Accelerates` | RulesClass::Infantry | char[64] | `0x62027b` |
| `AllowedToStartInMultiplayer` | RulesClass::Infantry | char[64] | `0x62027b` |
| `Ammo` | RulesClass::Infantry | char[64] | `0x62027b` |
| `AutoCrush` | RulesClass::Infantry | char[64] | `0x62027b` |
| `Cloakable` | RulesClass::Infantry | char[64] | `0x62027b` |
| `Cost` | RulesClass::Infantry | char[64] | `0x62027b` |
| `Crewed` | RulesClass::Infantry | char[64] | `0x62027b` |
| `Crusher` | RulesClass::Infantry | char[64] | `0x62027b` |
| `DeployTime` | RulesClass::Infantry | char[64] | `0x62027b` |
| `Disableable` | RulesClass::Infantry | char[64] | `0x62027b` |
| `FireAngle` | RulesClass::Infantry | char[64] | `0x62027b` |
| `HunterSeeker` | RulesClass::Infantry | char[64] | `0x62027b` |
| `ImmuneToVeins` | RulesClass::Infantry | char[64] | `0x62027b` |
| `Invisible` | RulesClass::Infantry | char[64] | `0x62027b` |
| `NoAutoFire` | RulesClass::Infantry | char[64] | `0x62027b` |
| `Nominal` | RulesClass::Infantry | char[64] | `0x62027b` |
| `Passengers` | RulesClass::Infantry | char[64] | `0x62027b` |
| `Points` | RulesClass::Infantry | char[64] | `0x62027b` |
| `RadarVisible` | RulesClass::Infantry | char[64] | `0x62027b` |
| `Repairable` | RulesClass::Infantry | char[64] | `0x62027b` |
| `SelfHealing` | RulesClass::Infantry | char[64] | `0x62027b` |
| `Sensors` | RulesClass::Infantry | char[64] | `0x62027b` |
| `Sight` | RulesClass::Infantry | char[64] | `0x62027b` |
| `Speed` | RulesClass::Infantry | char[64] | `0x62027b` |
| `TargetLaser` | RulesClass::Infantry | char[64] | `0x62027b` |
| `TechLevel` | RulesClass::Infantry | char[64] | `0x62027b` |
| `ThreatPosed` | RulesClass::Infantry | char[64] | `0x62027b` |
| `TiberiumHeal` | RulesClass::Infantry | char[64] | `0x62027b` |
| `TiltsWhenCrushes` | RulesClass::Infantry | char[64] | `0x62027b` |
| `ToProtect` | RulesClass::Infantry | char[64] | `0x62027b` |
| `Trainable` | RulesClass::Infantry | char[64] | `0x62027b` |
| `ZFudgeBridge` | RulesClass::Infantry | char[64] | `0x62027b` |
| `ZFudgeCliff` | RulesClass::Infantry | char[64] | `0x62027b` |
| `ZFudgeColumn` | RulesClass::Infantry | char[64] | `0x62027b` |
| `ZFudgeTunnel` | RulesClass::Infantry | char[64] | `0x62027b` |

### RulesClass::Unit (reader `0x620a13`)

| INI key | owning class | inferred C++ type | evidence fn VA |
|---|---|---|---|
| `Cameo` | RulesClass::Unit | char[64] | `0x620a13` |
| `Normalized` | RulesClass::Unit | char[64] | `0x620a13` |
| `PBarrelLength` | RulesClass::Unit | char[64] | `0x620a13` |
| `PBarrelThickness` | RulesClass::Unit | char[64] | `0x620a13` |
| `PrimaryFireFLH` | RulesClass::Unit | bool | `0x620a13` |
| `Remapable` | RulesClass::Unit | char[64] | `0x620a13` |
| `RotCount` | RulesClass::Unit | char[64] | `0x620a13` |
| `SBarrelLength` | RulesClass::Unit | char[64] | `0x620a13` |
| `SBarrelThickness` | RulesClass::Unit | char[64] | `0x620a13` |
| `SecondaryFireFLH` | RulesClass::Unit | bool | `0x620a13` |
| `ShadowIndex` | RulesClass::Unit | char[64] | `0x620a13` |
| `SpecialThreatValue` | RulesClass::Unit | char[64] | `0x620a13` |
| `TargetDistanceCoefficient` | RulesClass::Unit | char[64] | `0x620a13` |
| `TurretNotExportedOnGround` | RulesClass::Unit | char[64] | `0x620a13` |
| `TurretOffset` | RulesClass::Unit | char[64] | `0x620a13` |
| `VisibleLoad` | RulesClass::Unit | char[64] | `0x620a13` |

### RulesClass::Aircraft (reader `0x61f701`)

| INI key | owning class | inferred C++ type | evidence fn VA |
|---|---|---|---|
| `AccelerationFactor` | RulesClass::Aircraft | char[64] | `0x61f701` |
| `CloakingSpeed` | RulesClass::Aircraft | char[64] | `0x61f701` |
| `DeaccelerationFactor` | RulesClass::Aircraft | char[64] | `0x61f701` |
| `DebrisTypes` | RulesClass::Aircraft | char[64] | `0x61f701` |
| `Locomotor` | RulesClass::Aircraft | char[64] | `0x61f701` |
| `MaxDebris` | RulesClass::Aircraft | char[64] | `0x61f701` |
| `PhysicalSize` | RulesClass::Aircraft | char[64] | `0x61f701` |
| `PitchSpeed` | RulesClass::Aircraft | char[64] | `0x61f701` |
| `SlowdownDistance` | RulesClass::Aircraft | char[64] | `0x61f701` |
| `ThreatAvoidanceCoefficient` | RulesClass::Aircraft | char[64] | `0x61f701` |
| `Weight` | RulesClass::Aircraft | char[64] | `0x61f701` |

## Other type classes

### BuildingTypeClass (reader `0x43f250`)

| INI key | owning class | inferred C++ type | evidence fn VA |
|---|---|---|---|
| `Adjacent` | BuildingTypeClass | char[64] | `0x43f250` |
| `BaseNormal` | BuildingTypeClass | char[64] | `0x43f250` |
| `Capturable` | BuildingTypeClass | char[64] | `0x43f250` |
| `FreeUnit` | BuildingTypeClass | char[64] | `0x43f250` |
| `HalfDamageSmokeLocation1` | BuildingTypeClass | bool | `0x43f250` |
| `HalfDamageSmokeLocation2` | BuildingTypeClass | bool | `0x43f250` |
| `HasSpotlight` | BuildingTypeClass | char[64] | `0x43f250` |
| `Helipad` | BuildingTypeClass | char[64] | `0x43f250` |
| `Powered` | BuildingTypeClass | char[64] | `0x43f250` |
| `Radar` | BuildingTypeClass | char[64] | `0x43f250` |
| `Unsellable` | BuildingTypeClass | char[64] | `0x43f250` |
| `Wall` | BuildingTypeClass | char[64] | `0x43f250` |
| `WaterBound` | BuildingTypeClass | char[64] | `0x43f250` |
| `Weeder` | BuildingTypeClass | char[64] | `0x43f250` |

### InfantryTypeClass (reader `0x4d3980`)

| INI key | owning class | inferred C++ type | evidence fn VA |
|---|---|---|---|
| `Agent` | InfantryTypeClass | char[64] | `0x4d3980` |
| `Civilian` | InfantryTypeClass | char[64] | `0x4d3980` |
| `Crawls` | InfantryTypeClass | char[64] | `0x4d3980` |
| `Cyborg` | InfantryTypeClass | char[64] | `0x4d3980` |
| `Disguised` | InfantryTypeClass | char[64] | `0x4d3980` |
| `Doggie` | InfantryTypeClass | char[64] | `0x4d3980` |
| `Engineer` | InfantryTypeClass | char[64] | `0x4d3980` |
| `Fearless` | InfantryTypeClass | char[64] | `0x4d3980` |
| `FireProne` | InfantryTypeClass | char[64] | `0x4d3980` |
| `FireUp` | InfantryTypeClass | char[64] | `0x4d3980` |
| `Fraidycat` | InfantryTypeClass | char[64] | `0x4d3980` |
| `Infiltrate` | InfantryTypeClass | char[64] | `0x4d3980` |
| `JumpJet` | InfantryTypeClass | char[64] | `0x4d3980` |
| `Thief` | InfantryTypeClass | char[64] | `0x4d3980` |
| `TiberiumProof` | InfantryTypeClass | char[64] | `0x4d3980` |
| `VehicleThief` | InfantryTypeClass | char[64] | `0x4d3980` |
| `VoiceComment` | InfantryTypeClass | char[64] | `0x4d3980` |

### UnitTypeClass (reader `0x63e8e0`)

| INI key | owning class | inferred C++ type | evidence fn VA |
|---|---|---|---|
| `AltImage` | UnitTypeClass | char[64] /* id */ | `0x63e8e0` |
| `CarriesCrate` | UnitTypeClass | char[64] | `0x63e8e0` |
| `CrateGoodie` | UnitTypeClass | char[64] | `0x63e8e0` |
| `DeployToFire` | UnitTypeClass | char[64] | `0x63e8e0` |
| `FiringFrames` | UnitTypeClass | char[64] | `0x63e8e0` |
| `HalfDamageSmokeLocation` | UnitTypeClass | bool | `0x63e8e0` |
| `Harvester` | UnitTypeClass | char[64] | `0x63e8e0` |
| `IsTilter` | UnitTypeClass | char[64] | `0x63e8e0` |
| `LargeVisceroid` | UnitTypeClass | char[64] | `0x63e8e0` |
| `NoMovingFire` | UnitTypeClass | char[64] | `0x63e8e0` |
| `Passive` | UnitTypeClass | char[64] | `0x63e8e0` |
| `SmallVisceroid` | UnitTypeClass | char[64] | `0x63e8e0` |
| `TooBigToFitUnderBridge` | UnitTypeClass | char[64] | `0x63e8e0` |
| `UseTurretShadow` | UnitTypeClass | char[64] | `0x63e8e0` |
| `WalkFrames` | UnitTypeClass | char[64] | `0x63e8e0` |
| `Weeder` | UnitTypeClass | char[64] | `0x63e8e0` |

### AircraftTypeClass (reader `0x61f590`)

| INI key | owning class | inferred C++ type | evidence fn VA |
|---|---|---|---|
| `DoubleOwned` | AircraftTypeClass | char[64] | `0x61f590` |
| `Explodes` | AircraftTypeClass | char[64] | `0x61f590` |
| `FlightLevel` | AircraftTypeClass | char[64] | `0x61f590` |
| `GuardRange` | AircraftTypeClass | double | `0x61f590` |
| `IsDropship` | AircraftTypeClass | char[64] | `0x61f590` |
| `IsTrain` | AircraftTypeClass | char[64] | `0x61f590` |
| `MoveToShroud` | AircraftTypeClass | char[64] | `0x61f590` |
| `PitchAngle` | AircraftTypeClass | char[64] | `0x61f590` |
| `TypeImmune` | AircraftTypeClass | char[64] | `0x61f590` |
| `WalkRate` | AircraftTypeClass | char[64] | `0x61f590` |

### VoxelAnimTypeClass (reader `0x597e0e`)

| INI key | owning class | inferred C++ type | evidence fn VA |
|---|---|---|---|
| `ColorSpeed` | VoxelAnimTypeClass | char[64] | `0x597e0e` |
| `Deacc` | VoxelAnimTypeClass | char[64] | `0x597e0e` |
| `DeleteOnStateLimit` | VoxelAnimTypeClass | char[64] | `0x597e0e` |
| `EndStateAI` | VoxelAnimTypeClass | char[64] | `0x597e0e` |
| `FinalDamageState` | VoxelAnimTypeClass | char[64] | `0x597e0e` |
| `MinZVelocity` | VoxelAnimTypeClass | char[64] | `0x597e0e` |
| `NextParticle` | VoxelAnimTypeClass | char[64] | `0x597e0e` |
| `Normalized` | VoxelAnimTypeClass | char[64] | `0x597e0e` |
| `NumLoopFrames` | VoxelAnimTypeClass | char[64] | `0x597e0e` |
| `Radius` | VoxelAnimTypeClass | char[64] | `0x597e0e` |
| `StartFrame` | VoxelAnimTypeClass | char[64] | `0x597e0e` |
| `StartStateAI` | VoxelAnimTypeClass | char[64] | `0x597e0e` |
| `StateAIAdvance` | VoxelAnimTypeClass | char[64] | `0x597e0e` |
| `Translucency` | VoxelAnimTypeClass | char[64] | `0x597e0e` |
| `Translucent25State` | VoxelAnimTypeClass | char[64] | `0x597e0e` |
| `Translucent50State` | VoxelAnimTypeClass | char[64] | `0x597e0e` |
| `Velocity` | VoxelAnimTypeClass | char[64] | `0x597e0e` |
| `WindEffect` | VoxelAnimTypeClass | char[64] | `0x597e0e` |
| `XVelocity` | VoxelAnimTypeClass | char[64] | `0x597e0e` |
| `YVelocity` | VoxelAnimTypeClass | char[64] | `0x597e0e` |
| `ZVelocityRange` | VoxelAnimTypeClass | char[64] | `0x597e0e` |

### ParticleSystemTypeClass (reader `0x597090`)

| INI key | owning class | inferred C++ type | evidence fn VA |
|---|---|---|---|
| `BehavesLike` | ParticleSystemTypeClass | char[64] | `0x597090` |
| `HoldsWhat` | ParticleSystemTypeClass | char[64] | `0x597090` |
| `Laser` | ParticleSystemTypeClass | char[64] | `0x597090` |
| `Lifetime` | ParticleSystemTypeClass | char[64] | `0x597090` |
| `LightSize` | ParticleSystemTypeClass | char[64] | `0x597090` |
| `MovementPerturbationCoefficient` | ParticleSystemTypeClass | char[64] | `0x597090` |
| `OneFrameLight` | ParticleSystemTypeClass | char[64] | `0x597090` |
| `ParticleCap` | ParticleSystemTypeClass | char[64] | `0x597090` |
| `ParticlesPerCoord` | ParticleSystemTypeClass | char[64] | `0x597090` |
| `PositionPerturbationCoefficient` | ParticleSystemTypeClass | char[64] | `0x597090` |
| `Slowdown` | ParticleSystemTypeClass | char[64] | `0x597090` |
| `SparkSpawnFrames` | ParticleSystemTypeClass | char[64] | `0x597090` |
| `SpawnCutoff` | ParticleSystemTypeClass | char[64] | `0x597090` |
| `SpawnFrames` | ParticleSystemTypeClass | char[64] | `0x597090` |
| `SpawnRadius` | ParticleSystemTypeClass | char[64] | `0x597090` |
| `SpawnSparkPercentage` | ParticleSystemTypeClass | char[64] | `0x597090` |
| `SpawnTranslucencyCutoff` | ParticleSystemTypeClass | char[64] | `0x597090` |
| `Spawns` | ParticleSystemTypeClass | char[64] | `0x597090` |
| `SpiralDeltaPerCoord` | ParticleSystemTypeClass | char[64] | `0x597090` |
| `SpiralRadius` | ParticleSystemTypeClass | char[64] | `0x597090` |
| `VelocityPerturbationCoefficient` | ParticleSystemTypeClass | char[64] | `0x597090` |

### SuperWeaponTypeClass (reader `0x620a13`)

| INI key | owning class | inferred C++ type | evidence fn VA |
|---|---|---|---|
| `Cameo` | SuperWeaponTypeClass | char[64] | `0x620a13` |
| `Normalized` | SuperWeaponTypeClass | char[64] | `0x620a13` |
| `PBarrelLength` | SuperWeaponTypeClass | char[64] | `0x620a13` |
| `PBarrelThickness` | SuperWeaponTypeClass | char[64] | `0x620a13` |
| `PrimaryFireFLH` | SuperWeaponTypeClass | bool | `0x620a13` |
| `Remapable` | SuperWeaponTypeClass | char[64] | `0x620a13` |
| `RotCount` | SuperWeaponTypeClass | char[64] | `0x620a13` |
| `SBarrelLength` | SuperWeaponTypeClass | char[64] | `0x620a13` |
| `SBarrelThickness` | SuperWeaponTypeClass | char[64] | `0x620a13` |
| `SecondaryFireFLH` | SuperWeaponTypeClass | bool | `0x620a13` |
| `ShadowIndex` | SuperWeaponTypeClass | char[64] | `0x620a13` |
| `SpecialThreatValue` | SuperWeaponTypeClass | char[64] | `0x620a13` |
| `TargetDistanceCoefficient` | SuperWeaponTypeClass | char[64] | `0x620a13` |
| `TurretNotExportedOnGround` | SuperWeaponTypeClass | char[64] | `0x620a13` |
| `TurretOffset` | SuperWeaponTypeClass | char[64] | `0x620a13` |
| `VisibleLoad` | SuperWeaponTypeClass | char[64] | `0x620a13` |

### TerrainTypeClass (reader `0x577090`)

| INI key | owning class | inferred C++ type | evidence fn VA |
|---|---|---|---|
| `CellAnim` | TerrainTypeClass | char[64] /* id */ | `0x577090` |
| `Crate` | TerrainTypeClass | char[64] | `0x577090` |
| `CrateTrigger` | TerrainTypeClass | char[64] | `0x577090` |
| `Explodes` | TerrainTypeClass | char[64] | `0x577090` |
| `High` | TerrainTypeClass | char[64] | `0x577090` |
| `Overrides` | TerrainTypeClass | char[64] | `0x577090` |
| `Strength` | TerrainTypeClass | char[64] | `0x577090` |
| `Tiberium` | TerrainTypeClass | char[64] | `0x577090` |
| `Wall` | TerrainTypeClass | char[64] | `0x577090` |

### OverlayTypeClass (reader `0x4eb9ad`)

| INI key | owning class | inferred C++ type | evidence fn VA |
|---|---|---|---|
| `AllowBurrowing` | OverlayTypeClass | char[64] | `0x4eb9ad` |
| `AllowTiberium` | OverlayTypeClass | char[64] | `0x4eb9ad` |
| `AllowToPlace` | OverlayTypeClass | char[64] | `0x4eb9ad` |
| `FileName` | OverlayTypeClass | char[64] /* id */ | `0x4eb9ad` |
| `MarbleMadness` | OverlayTypeClass | char[64] | `0x4eb9ad` |
| `Morphable` | OverlayTypeClass | char[64] | `0x4eb9ad` |
| `NonMarbleMadness` | OverlayTypeClass | char[64] | `0x4eb9ad` |
| `RequiredForRMG` | OverlayTypeClass | char[64] | `0x4eb9ad` |
| `SetName` | OverlayTypeClass | char[64] /* id */ | `0x4eb9ad` |
| `ShadowCaster` | OverlayTypeClass | char[64] | `0x4eb9ad` |
| `ShadowTiles` | OverlayTypeClass | char[64] | `0x4eb9ad` |
| `ToSnowTheater` | OverlayTypeClass | char[64] | `0x4eb9ad` |
| `ToTemperateTheater` | OverlayTypeClass | char[64] | `0x4eb9ad` |

### SmudgeTypeClass (reader `0x4182cf`)

| INI key | owning class | inferred C++ type | evidence fn VA |
|---|---|---|---|
| `DemandLoad` | SmudgeTypeClass | char[64] | `0x4182cf` |
| `DetailLevel` | SmudgeTypeClass | char[64] | `0x4182cf` |
| `Elasticity` | SmudgeTypeClass | char[64] | `0x4182cf` |
| `FreeAfterPlaying` | SmudgeTypeClass | char[64] | `0x4182cf` |
| `IsTiberium` | SmudgeTypeClass | char[64] | `0x4182cf` |
| `MaxXYVel` | SmudgeTypeClass | char[64] | `0x4182cf` |
| `MinZVel` | SmudgeTypeClass | char[64] | `0x4182cf` |
| `RandomLoopDelay` | SmudgeTypeClass | char[64] | `0x4182cf` |
| `Spawns` | SmudgeTypeClass | char[64] | `0x4182cf` |
| `Translucency` | SmudgeTypeClass | char[64] | `0x4182cf` |
| `TranslucencyDetailLevel` | SmudgeTypeClass | char[64] | `0x4182cf` |
| `YSortAdjust` | SmudgeTypeClass | char[64] | `0x4182cf` |

### TiberiumClass (reader `0x5c6e8e`)

| INI key | owning class | inferred C++ type | evidence fn VA |
|---|---|---|---|
| `Basic` | TiberiumClass | char[64] | `0x5c6e8e` |
| `Lighting` | TiberiumClass | char[64] | `0x5c6e8e` |

### HouseTypeClass (reader `0x4c74d0`)

| INI key | owning class | inferred C++ type | evidence fn VA |
|---|---|---|---|
| `Airspeed` | HouseTypeClass | char[64] | `0x4c74d0` |
| `Armor` | HouseTypeClass | char[64] | `0x4c74d0` |
| `BuildTime` | HouseTypeClass | char[64] | `0x4c74d0` |
| `Cost` | HouseTypeClass | char[64] | `0x4c74d0` |
| `Firepower` | HouseTypeClass | char[64] | `0x4c74d0` |
| `Groundspeed` | HouseTypeClass | char[64] | `0x4c74d0` |
| `Multiplay` | HouseTypeClass | char[64] | `0x4c74d0` |
| `MultiplayPassive` | HouseTypeClass | char[64] | `0x4c74d0` |
| `Prefix` | HouseTypeClass | char[64] | `0x4c74d0` |
| `SmartAI` | HouseTypeClass | char[64] | `0x4c74d0` |
| `Suffix` | HouseTypeClass | char[64] | `0x4c74d0` |
| `WallOwner` | HouseTypeClass | char[64] | `0x4c74d0` |

### TeamTypeClass (reader `0x60d180`)

| INI key | owning class | inferred C++ type | evidence fn VA |
|---|---|---|---|
| `Aggressive` | TeamTypeClass | char[64] | `0x60d180` |
| `Annoyance` | TeamTypeClass | char[64] | `0x60d180` |
| `Autocreate` | TeamTypeClass | char[64] | `0x60d180` |
| `Droppod` | TeamTypeClass | char[64] | `0x60d180` |
| `Full` | TeamTypeClass | char[64] | `0x60d180` |
| `GuardSlower` | TeamTypeClass | char[64] | `0x60d180` |
| `Loadable` | TeamTypeClass | char[64] | `0x60d180` |
| `LooseRecruit` | TeamTypeClass | char[64] | `0x60d180` |
| `OnTransOnly` | TeamTypeClass | char[64] | `0x60d180` |
| `Prebuild` | TeamTypeClass | char[64] | `0x60d180` |
| `Priority` | TeamTypeClass | char[64] | `0x60d180` |
| `Recruiter` | TeamTypeClass | char[64] | `0x60d180` |
| `Reinforce` | TeamTypeClass | char[64] | `0x60d180` |
| `Suicide` | TeamTypeClass | char[64] | `0x60d180` |
| `TechLevel` | TeamTypeClass | char[64] | `0x60d180` |
| `VeteranLevel` | TeamTypeClass | char[64] | `0x60d180` |
| `Whiner` | TeamTypeClass | char[64] | `0x60d180` |

### ScriptTypeClass (reader `0x60d510`)

| INI key | owning class | inferred C++ type | evidence fn VA |
|---|---|---|---|
| `Aggressive` | ScriptTypeClass | bool | `0x60d510` |
| `Annoyance` | ScriptTypeClass | bool | `0x60d510` |
| `AreTeamMembersRecruitable` | ScriptTypeClass | bool | `0x60d510` |
| `Autocreate` | ScriptTypeClass | bool | `0x60d510` |
| `AvoidThreats` | ScriptTypeClass | bool | `0x60d510` |
| `Droppod` | ScriptTypeClass | bool | `0x60d510` |
| `Full` | ScriptTypeClass | bool | `0x60d510` |
| `Group` | ScriptTypeClass | char[64] | `0x60d510` |
| `GuardSlower` | ScriptTypeClass | bool | `0x60d510` |
| `IonImmune` | ScriptTypeClass | bool | `0x60d510` |
| `IsBaseDefense` | ScriptTypeClass | bool | `0x60d510` |
| `Loadable` | ScriptTypeClass | bool | `0x60d510` |
| `LooseRecruit` | ScriptTypeClass | bool | `0x60d510` |
| `OnTransOnly` | ScriptTypeClass | bool | `0x60d510` |
| `OnlyTargetHouseEnemy` | ScriptTypeClass | bool | `0x60d510` |
| `Prebuild` | ScriptTypeClass | bool | `0x60d510` |
| `Priority` | ScriptTypeClass | char[64] | `0x60d510` |
| `Recruiter` | ScriptTypeClass | bool | `0x60d510` |
| `Reinforce` | ScriptTypeClass | bool | `0x60d510` |
| `Script` | ScriptTypeClass | int | `0x60d510` |
| `Suicide` | ScriptTypeClass | bool | `0x60d510` |
| `TaskForce` | ScriptTypeClass | int | `0x60d510` |
| `TechLevel` | ScriptTypeClass | char[64] | `0x60d510` |
| `TransportsReturnOnUnload` | ScriptTypeClass | bool | `0x60d510` |
| `VeteranLevel` | ScriptTypeClass | char[64] | `0x60d510` |
| `Waypoint` | ScriptTypeClass | int | `0x60d510` |
| `Whiner` | ScriptTypeClass | bool | `0x60d510` |

### SideClass (reader `0x5e2e90`)

| INI key | owning class | inferred C++ type | evidence fn VA |
|---|---|---|---|
| `DestroyableBridges` | SideClass | bool | `0x5e2e90` |
| `FixedAlliance` | SideClass | bool | `0x5e2e90` |
| `FogOfWar` | SideClass | bool | `0x5e2e90` |
| `HarvesterImmune` | SideClass | bool | `0x5e2e90` |
| `Inert` | SideClass | bool | `0x5e2e90` |
| `InitialVeteran` | SideClass | bool | `0x5e2e90` |
| `IonStorms` | SideClass | bool | `0x5e2e90` |
| `MCVDeploy` | SideClass | bool | `0x5e2e90` |
| `Meteorites` | SideClass | bool | `0x5e2e90` |
| `TiberiumExplosive` | SideClass | bool | `0x5e2e90` |
| `TiberiumGrows` | SideClass | bool | `0x5e2e90` |
| `TiberiumSpreads` | SideClass | bool | `0x5e2e90` |
| `Visceroids` | SideClass | bool | `0x5e2e90` |

### MultiplayerSettingsClass (reader `0x5e3000`)

| INI key | owning class | inferred C++ type | evidence fn VA |
|---|---|---|---|
| `DestroyableBridges` | MultiplayerSettingsClass | char[64] | `0x5e3000` |
| `FixedAlliance` | MultiplayerSettingsClass | char[64] | `0x5e3000` |
| `FogOfWar` | MultiplayerSettingsClass | char[64] | `0x5e3000` |
| `HarvesterImmune` | MultiplayerSettingsClass | char[64] | `0x5e3000` |
| `Inert` | MultiplayerSettingsClass | char[64] | `0x5e3000` |
| `InitialVeteran` | MultiplayerSettingsClass | char[64] | `0x5e3000` |
| `IonStorms` | MultiplayerSettingsClass | char[64] | `0x5e3000` |
| `MCVDeploy` | MultiplayerSettingsClass | char[64] | `0x5e3000` |
| `Meteorites` | MultiplayerSettingsClass | char[64] | `0x5e3000` |
| `TiberiumExplosive` | MultiplayerSettingsClass | char[64] | `0x5e3000` |
| `TiberiumGrows` | MultiplayerSettingsClass | char[64] | `0x5e3000` |
| `TiberiumSpreads` | MultiplayerSettingsClass | char[64] | `0x5e3000` |
| `Visceroids` | MultiplayerSettingsClass | char[64] | `0x5e3000` |

### SpecialClass (reader `0x5b1aa3`)

| INI key | owning class | inferred C++ type | evidence fn VA |
|---|---|---|---|
| `CameraRange` | SpecialClass | double | `0x5b1aa3` |
| `CliffBackImpassability` | SpecialClass | char[64] | `0x5b1aa3` |
| `CloakingStages` | SpecialClass | char[64] | `0x5b1aa3` |
| `FlightLevel` | SpecialClass | char[64] | `0x5b1aa3` |
| `IceBreakingWeight` | SpecialClass | char[64] | `0x5b1aa3` |
| `IceCrackingWeight` | SpecialClass | char[64] | `0x5b1aa3` |
| `PlacementDelay` | SpecialClass | char[64] | `0x5b1aa3` |
| `RepairBay` | SpecialClass | char[64] | `0x5b1aa3` |
| `TrackedDownhill` | SpecialClass | char[64] | `0x5b1aa3` |
| `TrackedUphill` | SpecialClass | char[64] | `0x5b1aa3` |
| `WheeledDownhill` | SpecialClass | char[64] | `0x5b1aa3` |
| `WheeledUphill` | SpecialClass | char[64] | `0x5b1aa3` |
| `WindDirection` | SpecialClass | char[64] | `0x5b1aa3` |

### ColorSchemeClass (reader `0x5c6ae7`)

| INI key | owning class | inferred C++ type | evidence fn VA |
|---|---|---|---|
| `Basic` | ColorSchemeClass | char[64] | `0x5c6ae7` |

### MovieClass (reader `0x5c68c0`)

| INI key | owning class | inferred C++ type | evidence fn VA |
|---|---|---|---|
| `Basic` | MovieClass | char[64] | `0x5c68c0` |
| `Briefing` | MovieClass | int | `0x5c68c0` |

### ScenarioClass (reader `0x52e8b0`)

| INI key | owning class | inferred C++ type | evidence fn VA |
|---|---|---|---|
| `Basic` | ScenarioClass | int | `0x52e8b0` |
| `Fill` | ScenarioClass | int | `0x52e8b0` |
| `Level` | ScenarioClass | char[64] | `0x52e8b0` |
| `Lighting` | ScenarioClass | double | `0x52e8b0` |
| `TechLevel` | ScenarioClass | int | `0x52e8b0` |
| `Theater` | ScenarioClass | int | `0x52e8b0` |

### MapSeedClass (reader `0x52d4f0`)

| INI key | owning class | inferred C++ type | evidence fn VA |
|---|---|---|---|
| `RandomMap` | MapSeedClass | int | `0x52d4f0` |

### RandomMapClass (reader `0x52d760`)

| INI key | owning class | inferred C++ type | evidence fn VA |
|---|---|---|---|
| `RandomMap` | RandomMapClass | char[64] | `0x52d760` |

## Unassigned keys

1281 of the 1876 referenced strings were not matched to a reader in this
pass.  They are either format strings, file extensions, section names,
or keys read by readers not enumerated here.

| key | referenced at VA(s) |
|---|---|
| `                    %x` | `0x45E86D`, `0x45E9F8` |
| `              %x` | `0x45E85A`, `0x45E9E4` |
| ` - %s + %08X` | `0x492354` |
| ` CACHE.MIX` | `0x4DC930`, `0x4DCA9A` |
| ` CONQUER.MIX` | `0x4DCB8B` |
| ` LOCAL.MIX` | `0x4DCA9A` |
| ` MULTI.MIX` | `0x4DCCC5` |
| ` SCORES.MIX` | `0x4DCDDF` |
| ` SOUNDS.MIX` | `0x4DCD52` |
| `%-12s  %x` | `0x45E7F0`, `0x45E979` |
| `%.28s %02d` | `0x4EBBC5` |
| `%02X` | `0x5D48F0` |
| `%02X ` | `0x4921E7` |
| `%02X %02X %02X %02X : %02X %02X %02X %02X %02X %02X` | `0x4E6D90` |
| `%02d` | `0x4EBBC5`, `0x5CB2C6` |
| `%02d - %s [%d:%02d]` | `0x5E2977` |
| `%02d/%02d/%04d - %02d:%02d:%02d` | `0x65BED0` |
| `%02ld:%02ld:%02ld` | `0x5CC600` |
| `%02x` | `0x410CE6` |
| `%02x %02x %02x %02x : %02x %02x %02x %02x %02x %02x` | `0x66E2E0` |
| `%03d` | `0x41EDCE`, `0x41EEC0`, `0x5D4BC0` |
| `%04d` | `0x54DEA0` |
| `%08X` | `0x405D50`, `0x56A1C0` |
| `%08X: ` | `0x49220D` |
| `%08X: %08X` | `0x492354` |
| `%3d%%` | `0x5CA2B8` |
| `%4d  ` | `0x45E7F0`, `0x45E979` |
| `%4d  %d  %d` | `0x45E50E` |
| `%4d %2d  %d` | `0x45E2ED` |
| `%4d %2d %-5s  ` | `0x45E5B0`, `0x45E8DB` |
| `%c%c` | `0x6556F4` |
| `%c:\` | `0x44C730`, `0x44C91A`, `0x44CAFD`, `0x44CCF7`, `0x44CE60`, `0x44D100`, `0x44D2C0`, `0x44D450`, `0x471CD0` |
| `%c:\TS%d.dsk` | `0x471E11` |
| `%d  (%d)` | `0x65F921`, `0x66211B` |
| `%d x %d x 16` | `0x4FE378` |
| `%d%%` | `0x5CB213` |
| `%d,%d` | `0x448090`, `0x448110`, `0x4D7230`, `0x4D73A8`, `0x5D4799`, `0x62A6C0`, `0x62A700` |
| `%d,%d,%d` | `0x41EF47`, `0x43FB5D`, `0x4484A0`, `0x448570`, `0x4485C0`, `0x4486A0`, `0x4D7440`, `0x4D758C`, `0x5D4799`, `0x6263CC` |
| `%d,%d,%d,%d` | `0x4D6530`, `0x4D66B7` |
| `%d,%d,%d,%d,%d` | `0x62EA22` |
| `%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%s,%d,%d,%s,%s:` | `0x5615C0`, `0x65D4B0` |
| `%d,%d,%d,%d,%d,%d,%d,%s` | `0x5FE155`, `0x65D630`, `0x65DBED`, `0x662F10` |
| `%d,%d,%d,%s` | `0x4D3670` |
| `%d,%d,%s` | `0x6263B0` |
| `%d,%d,%s,%d,%d,%d,%d,%s` | `0x5FE0F3` |
| `%d,%s` | `0x48EA00`, `0x48EAA0` |
| `%d,%s,%s` | `0x6044A8` |
| `%d.%d.%d.%d` | `0x4DA510`, `0x5C2A87`, `0x66F8F0` |
| `%f,%f,%f` | `0x4D7640`, `0x4D779C` |
| `%s  %d/%d` | `0x65E598`, `0x664D4A` |
| `%s  (%d)` | `0x65EA89` |
| `%s %d` | `0x65E130`, `0x65F0E0`, `0x65F599`, `0x65FFE1`, `0x66A2C6`, `0x66BBE5` |
| `%s %s` | `0x5D29A0`, `0x6627F1`, `0x66501A`, `0x66526C`, `0x665324`, `0x666D9E`, `0x6681DC`, `0x66C1B0`, `0x66C1F0` |
| `%s %x` | `0x5D2BBA` |
| `%s : %s` | `0x669D60`, `0x66B960` |
| `%s [%s]` | `0x548E30`, `0x54C0F0` |
| `%s%d` | `0x483430`, `0x65B170` |
| `%s's_game` | `0x662F10` |
| `%s,%d,%d` | `0x41EEC0` |
| `%s,%d,%d,` | `0x5615C0`, `0x65D4B0` |
| `%s,%d,%d,%d` | `0x5E10D0` |
| `%s,%s,%d,%d,%d,%d,%s,%d,%d,%d,%d,%d` | `0x433D64` |
| `%s,%s,%d,%d,%d,%d,%s,%d,%s,%d,%d,%d,%d,%d` | `0x4D1207` |
| `%s,%s,%d,%d,%d,%d,%s,%s,%d,%d,%d,%d` | `0x40DB34` |
| `%s,%s,%d,%d,%d,%d,%s,%s,%d,%d,%d,%d,%d,%d` | `0x63B9AE` |
| `%s,%s,%s,%d,%d,%d,%d,%d` | `0x62DDFC` |
| `%s,%s,%s,%d,%d,%s,%s,%lf,%lf,%lf,%d,%d,%d,%d,%s,%d,%d,%d` | `0x410CE6` |
| `%s,<none>` | `0x604470` |
| `%s-%s` | `0x65BFC0` |
| `%s.INI` | `0x4EAA00` |
| `%s.MIX` | `0x4DFEE0` |
| `%s.PAL` | `0x4E0045` |
| `%s.VQA` | `0x5C21BD` |
| `%s/%s` | `0x669A5A` |
| `%s: %d` | `0x665423` |
| `%s:%s` | `0x55E780` |
| `%sBARL` | `0x571D46`, `0x571DEC`, `0x5722C9`, `0x572357` |
| `%sTUR` | `0x571BB0`, `0x571C3B`, `0x572120`, `0x5721F6` |
| `%s\%s` | `0x5E5940`, `0x669A5A` |
| `%s_%d  (%d)` | `0x65ED5C` |
| `%s|%s|%x|%d|%d|%d|%d|%s|%d|%d|%s` | `0x5D4BC0` |
| `%x.%x` | `0x646567` |
| `(%s %d)` | `0x65F217`, `0x65F73E` |
| `(%s) %s` | `0x66B9B9`, `0x66BA48` |
| `*.%3s` | `0x4F8D19`, `0x4F9040` |
| `*.MPR` | `0x5D514D` |
| `*.PKT` | `0x5D4FD7` |
| `-%dX%d` | `0x4DBCBF` |
| `-ATTRACT` | `0x4DBC9C` |
| `-DESTNET` | `0x4DBB4C` |
| `-MESSAGES` | `0x4DBC7F` |
| `-SOCKET` | `0x4DBC0C` |
| `-STEALTH` | `0x4DBC61` |
| `-WIN` | `0x4DBD6D` |
| `..\%s` | `0x65E1BC` |
| `.AUD` | `0x6275D0`, `0x627930`, `0x627A70`, `0x646D50`, `0x646E80`, `0x646F0E`, `0x647484`, `0x6480C0` |
| `.HVA` | `0x43EFBD`, `0x43F15A`, `0x571902`, `0x571AA5`, `0x571C3B`, `0x571DEC`, `0x5721F6`, `0x572357` |
| `.MMS` | `0x4EC064` |
| `.MMT` | `0x4EBF57` |
| `.SED` | `0x5C27C0` |
| `.SHP` | `0x417C9F`, `0x417E4C`, `0x418DD0`, `0x43EB20`, `0x43EBF6`, `0x441F00`, `0x5724A0`, `0x572610`, `0x5729B0`, `0x577007`, `0x5771D1`, `0x577491`, `0x57778E`, `0x5E1960`, `0x5F2DB0`, `0x5F318E`, `0x620A13`, `0x6213B0`, `0x625280`, `0x63E8E0`, `0x63ED30` |
| `.VXL` | `0x43EED0`, `0x43F0D6`, `0x571870`, `0x571A12`, `0x571BB0`, `0x571D46`, `0x572120`, `0x5722C9`, `0x6422DB` |
| `.\sun.exe` | `0x4918C2` |
| `0,NONE` | `0x5B56B0` |
| `000000<IIFooooooooooooooooooooIKKLSSMMHF;AA20000000009IIIoooooooooooooooooooooFILLLMOOKA;;5000000000>ILIFooooooooooooooooooooooHKLOKMMLCA9000000006DIMMHooooooooooooooooooooooooIKOOLKNMC;830016<FIIKMKo` | `0x5D9FA8` |
| `1#IND` | `0x691E74` |
| `1#INF` | `0x691E8E` |
| `1#QNAN` | `0x691EB5` |
| `1#SNAN` | `0x691E4C` |
| `100%%,100%%,100%%,100%%,100%%` | `0x65187A` |
| `29e3bb2a-2f36-11d3-a72c-0090272fa661` | `0x5E5B80` |
| `<none>` | `0x404A30`, `0x40F4A0`, `0x4104B0`, `0x410E00`, `0x413870`, `0x418503`, `0x4186F6`, `0x4187B4`, `0x418D20`, `0x419130`, `0x441870`, `0x445F70`, `0x446280`, `0x448BD0`, `0x448EB0`, `0x44A669`, `0x460110`, `0x460160`, `0x4C7A90`, `0x4D3600`, `0x4D4020`, `0x577540`, `0x5973F0`, `0x597640`, `0x597E0E`, `0x5981D0`, `0x5985B0`, `0x5B5E20`, `0x5C68C0`, `0x5CE7C0`, `0x5CEA40`, `0x5E1B80`, `0x5F32A0`, `0x604200`, `0x6045F0`, `0x606630`, `0x606A70`, `0x60D800`, `0x60D890`, `0x625650`, `0x62DA00`, `0x62DA7A`, `0x62DD00`, `0x62E1B0`, `0x62E3D0`, `0x63EDE0`, `0x6426D0`, `0x642880`, `0x647650`, `0x651D30`, `0x6576F0` |
| `<program name unknown>` | `0x68CC4F` |
| `<title>Hi there.</title>` | `0x65CA70` |
| `>>> %s` | `0x66BCB0` |
| `AARate` | `0x546FEA` |
| `AFPS` | `0x5EFACF`, `0x5EFB1E` |
| `AI.INI` | `0x4DA052` |
| `AIBuildThis` | `0x43F45F` |
| `AIHateDelays` | `0x5B2343` |
| `AIIonCannonAPCValue` | `0x5B2343` |
| `AIIonCannonBaseDefenseValue` | `0x5B2343` |
| `AIIonCannonConYardValue` | `0x5B2343` |
| `AIIonCannonEngineerValue` | `0x5B2343` |
| `AIIonCannonHarvesterValue` | `0x5B2343` |
| `AIIonCannonHelipadValue` | `0x5B2343` |
| `AIIonCannonMCVValue` | `0x5B2343` |
| `AIIonCannonPlugValue` | `0x5B2343` |
| `AIIonCannonPowerValue` | `0x5B2343` |
| `AIIonCannonTempleValue` | `0x5B2343` |
| `AIIonCannonThiefValue` | `0x5B2343` |
| `AIIonCannonWarFactoryValue` | `0x5B2343` |
| `AIPL` | `0x5EF74F` |
| `AIRCRAFT` | `0x5D466F` |
| `ANIM` | `0x5D46E2` |
| `ART.INI` | `0x4D9DBD` |
| `ATE1` | `0x56CD00` |
| `ATQ0` | `0x56CD00` |
| `ATS0=0` | `0x56D3C8` |
| `ATV1` | `0x56CD00` |
| `Access address:%08X ` | `0x4919DE` |
| `Access violation - no RTTI data!` | `0x682F0B`, `0x683065`, `0x683195` |
| `Accessibility` | `0x52D4F0`, `0x52D760` |
| `Action` | `0x5C68C0`, `0x5C7410`, `0x5F2FB4` |
| `ActsLike` | `0x4BC5C0`, `0x4BC8E0` |
| `Address = ` | `0x5D48F0` |
| `AirburstWeapon` | `0x445E75` |
| `Aircraft` | `0x5B59E0` |
| `Aircraft  (Type:%d(%s)) ` | `0x59E778` |
| `Allies` | `0x4BC5C0`, `0x4BCA4A` |
| `AllowFind` | `0x65D090`, `0x65D120`, `0x65DE10` |
| `AllowHiResModes` | `0x573A68` |
| `AllowModeToggle` | `0x573A68`, `0x5E7406` |
| `AllowPage` | `0x65D090`, `0x65D120`, `0x65DE10` |
| `AllowableUnitMaximums` | `0x5C6DFE`, `0x5C7410` |
| `AllowableUnits` | `0x5C6C9A`, `0x5C7410` |
| `AllyReveal` | `0x5AFC74` |
| `AlphaImage` | `0x572610` |
| `Ambient` | `0x52E8B0`, `0x5C6E92`, `0x5C7410` |
| `AmbientChangeRate` | `0x5AFD52` |
| `AmbientChangeStep` | `0x5AFD52` |
| `AmmoCrateDamage` | `0x5B0360` |
| `Amphibious` | `0x5B595F` |
| `Anim      (Type:%d(%s)) ` | `0x59E8C2` |
| `AnimHigh` | `0x445EAE` |
| `AnimList` | `0x6516EF` |
| `AnimLow` | `0x445EAE` |
| `AnimPalette` | `0x445EAE` |
| `AnimRate` | `0x445EAE` |
| `AnimationProbability` | `0x6253E8` |
| `AnimationRate` | `0x6253E8` |
| `Animations` | `0x5B4450` |
| `Anims` | `0x559CEF` |
| `Armament: %s` | `0x483770` |
| `Armament: NONE` | `0x48379E` |
| `Armor: %s` | `0x4837B0` |
| `Armory` | `0x43F45F` |
| `Artillary` | `0x43F45F` |
| `AtmosphereEntry` | `0x5AED36` |
| `AtomDamage` | `0x5B0D2F` |
| `AttachedSystem` | `0x6421AE` |
| `AttachedTo` | `0x54E1C5` |
| `Attempted a typeid of NULL pointer!` | `0x682FBD` |
| `Audio` | `0x573A68`, `0x573D20` |
| `AutoScroll` | `0x5737F0`, `0x573D20` |
| `AuxBuilding` | `0x5F30A3` |
| `AuxSound1` | `0x61FAE3` |
| `AuxSound2` | `0x61FAE3` |
| `BARGRAPH.AUD` | `0x5C9DFF` |
| `BARRACKS` | `0x4490F0`, `0x44A720` |
| `BASE` | `0x5EF69B` |
| `BATTLE*.INI` | `0x4D9940` |
| `BATTLE.INI` | `0x4D99DF`, `0x4D9A6A` |
| `BEST01.SHP` | `0x5C9AEC` |
| `BEST02.SHP` | `0x5C9AEC` |
| `BESTBOX.AUD` | `0x5C9D3F` |
| `BIGFONT.SHP` | `0x5CD8F0` |
| `BLB?` | `0x5F010D` |
| `BLC?` | `0x5F0673` |
| `BLK?` | `0x5F060F` |
| `BLL?` | `0x5F043B` |
| `BUILDING` | `0x5D4707` |
| `BUILDNGZ.SHP` | `0x43E300` |
| `BULLET` | `0x5D4729` |
| `BUSY` | `0x56D6A6`, `0x56DA40` |
| `Back` | `0x5C9EBF`, `0x5CC962` |
| `Bad read pointer - no RTTI data!` | `0x683036` |
| `BallisticScatter` | `0x5B0D2F` |
| `BarGraph` | `0x5C9DFF`, `0x5CC212` |
| `BarrelDebris` | `0x5AE938` |
| `BarrelExplode` | `0x5AE8E5` |
| `BarrelParticle` | `0x5AE9EF` |
| `BaseUnit` | `0x5B1F7D` |
| `Bases` | `0x5B3C70` |
| `Battles` | `0x446860` |
| `Baud` | `0x5D4126`, `0x5D4BC0` |
| `BerzerkAllowed` | `0x5B0D2F` |
| `BestBox` | `0x5C9D3F`, `0x5CAD8F` |
| `BibShape` | `0x43EBF6` |
| `Biome` | `0x52D4F0`, `0x52D760` |
| `BlackTile` | `0x4EAA00` |
| `BlowupSound` | `0x5AF0EF` |
| `Blue` | `0x5C6E92`, `0x5C7410` |
| `BlueTint` | `0x52E8B0` |
| `BombParachute` | `0x5AFBB8` |
| `Bootstrap.....` | `0x4D8B1B` |
| `BounceAnim` | `0x418503`, `0x641F4B` |
| `BounceSound` | `0x418503`, `0x641F4B` |
| `Bouncer` | `0x4188DF` |
| `BridgeBottomLeft1` | `0x4EAA00` |
| `BridgeBottomLeft2` | `0x4EAA00` |
| `BridgeBottomRight1` | `0x4EAA00` |
| `BridgeBottomRight2` | `0x4EAA00` |
| `BridgeDestruction` | `0x5B3C70` |
| `BridgeExplosions` | `0x5AF809` |
| `BridgeMiddle1` | `0x4EAA00` |
| `BridgeMiddle2` | `0x4EAA00` |
| `BridgeRepairHut` | `0x43F45F` |
| `BridgeSet` | `0x4EAA00` |
| `BridgeStrength` | `0x5B0D2F` |
| `BridgeTopLeft1` | `0x4EAA00` |
| `BridgeTopLeft2` | `0x4EAA00` |
| `BridgeTopRight1` | `0x4EAA00` |
| `BridgeTopRight2` | `0x4EAA00` |
| `BridgeVoxelMax` | `0x5B19C4` |
| `Brief` | `0x5C68C0`, `0x5C7410` |
| `BuildAA` | `0x5B4DD1` |
| `BuildBarracks` | `0x5B496E` |
| `BuildConst` | `0x5B4680` |
| `BuildDefense` | `0x5B4C28` |
| `BuildDelay` | `0x5B5CF0` |
| `BuildHelipad` | `0x5B4EA6` |
| `BuildLimit` | `0x61FAE3` |
| `BuildPDefense` | `0x5B4CFD` |
| `BuildPower` | `0x5B4769` |
| `BuildRadar` | `0x5B4F7B` |
| `BuildRefinery` | `0x5B486B` |
| `BuildSlowdown` | `0x5B5CF0` |
| `BuildTech` | `0x5B4A61` |
| `BuildWeapons` | `0x5B4B53` |
| `Buildable` | `0x5B59A3` |
| `Building  (Type:%d(%s)) ` | `0x59E8F6` |
| `BuildingDrop` | `0x5AF0EF` |
| `BuildingSlam` | `0x5AF0EF` |
| `Bullet    (Type:%d(%s)) ` | `0x59E92E` |
| `Bummer` | `0x4916CA` |
| `Burn` | `0x5E1960` |
| `C4Delay` | `0x5B0D2F` |
| `C4Warhead` | `0x5B0ABD` |
| `CACHE.MIX` | `0x4DC930` |
| `CASU01.SHP` | `0x5C9AEC` |
| `CASU02.SHP` | `0x5C9AEC` |
| `CDFS` | `0x647BAC` |
| `CID?` | `0x5EFDF9` |
| `CLSID` | `0x5E5570` |
| `CLSID\` | `0x5E5570`, `0x5E5940` |
| `CMP?` | `0x5EFEA6` |
| `COL?` | `0x5EFF2E` |
| `CONIN$` | `0x694F5A` |
| `CONNECT` | `0x56D9F2` |
| `CONOUT$` | `0x694F79` |
| `CONQUER.MIX` | `0x4DCB10` |
| `CPU %01d86, MMX %s, Vendor: %s` | `0x4DD086` |
| `CRA?` | `0x5F06E8` |
| `CRAT` | `0x5EF713` |
| `CRD?` | `0x5EFF63` |
| `CRED` | `0x5EF66C` |
| `CURR01.SHP` | `0x5C9AEC` |
| `C_SHADOW.SHP` | `0x4EAA00` |
| `CallWaitString` | `0x5D420B`, `0x5D4BC0` |
| `CallWaitStringIndex` | `0x5D420B`, `0x5D4BC0` |
| `CaptureTheFlag` | `0x5B3C70` |
| `Carryall` | `0x40F300` |
| `Category` | `0x61FAE3` |
| `Cell` | `0x5D4799` |
| `CellTags` | `0x47661D` |
| `ChainReaction` | `0x5771D1` |
| `ChargeAnim` | `0x43F45F` |
| `ChargingVoice` | `0x5F2FB4` |
| `CheckHeap` | `0x5D3F90` |
| `Checking hardware region fill capability...` | `0x46F1C0` |
| `Checking overlapped blit capability...` | `0x46F256` |
| `ChuteSound` | `0x5AF0EF` |
| `Class` | `0x54DF91` |
| `Clear` | `0x476390` |
| `ClearTile` | `0x4EAA00` |
| `ClearToGreenLat` | `0x4EAA00` |
| `ClearToPaveLat` | `0x4EAA00` |
| `ClearToRoughLat` | `0x4EAA00` |
| `ClearToSandLat` | `0x4EAA00` |
| `ClickMap` | `0x55A3E7` |
| `CliffRamps` | `0x4EAA00` |
| `CliffSet` | `0x4EAA00` |
| `Climb` | `0x5B5B70` |
| `CloakGenerator` | `0x43F45F` |
| `CloakRadiusInCells` | `0x43F45F` |
| `CloakSound` | `0x5AED75` |
| `CloakStop` | `0x61FAE3` |
| `CollapseChance` | `0x5B0D2F` |
| `Color` | `0x445BA0`, `0x4BC5C0`, `0x4BCA4A`, `0x4C74D0`, `0x5D3F90`, `0x5D4BC0`, `0x628540` |
| `ColorList` | `0x597D00` |
| `ComboDropWin` | `0x57FD96` |
| `Compression` | `0x5D4126`, `0x5D4BC0` |
| `Compression_Off` | `0x54E2DB` |
| `Compression_On` | `0x54E2DB` |
| `ConcreteWalls` | `0x5B504F` |
| `ConditionRed` | `0x5AFC74` |
| `ConditionYellow` | `0x5AFC74` |
| `Connected` | `0x56DB10` |
| `Construction` | `0x5AED75` |
| `ConstructionYard` | `0x43F45F` |
| `ContentScan` | `0x5B59E0`, `0x5B5CF0` |
| `Conventional` | `0x6516EF` |
| `Coord` | `0x5D4799` |
| `Cost: %d` | `0x483610` |
| `Cost: N/A` | `0x48375E` |
| `CrateImg` | `0x5B018F` |
| `CrateMaximum` | `0x5B01CD` |
| `CrateMinimum` | `0x5B01CD` |
| `CrateRadius` | `0x5B01CD` |
| `CrateRegen` | `0x5B01CD` |
| `Craters` | `0x5B087D` |
| `Crates` | `0x5B3C70` |
| `CreditTicks` | `0x5AF0EF` |
| `Credits` | `0x4BC5C0`, `0x4BC8E0` |
| `Crew` | `0x5B2305` |
| `CruiseHeight` | `0x5B5B70` |
| `CrumbleSound` | `0x5AF0EF` |
| `Crush` | `0x5B0D2F` |
| `CrushSound` | `0x572610` |
| `Crushable` | `0x572610` |
| `CurVer` | `0x5E5570` |
| `CustomRotor` | `0x40F300` |
| `D:\Projects\TSun\Code\Connect.CPP` | `0x45EC83` |
| `D:\Projects\TSun\Code\DSAudio.Cpp` | `0x484270`, `0x4843A0`, `0x484AD0`, `0x484C90`, `0x484DDD`, `0x484E56`, `0x484F10`, `0x484F90`, `0x485026`, `0x4850E0`, `0x485D40`, `0x485E20`, `0x4860DD`, `0x4861A0`, `0x486280`, `0x486500`, `0x486590`, `0x486680`, `0x486790`, `0x4868BA`, `0x4869B0`, `0x486AA0`, `0x486B90`, `0x486C80`, `0x486D60` |
| `D:\Projects\TSun\Code\Queue.CPP` | `0x59D4A8` |
| `D:\Projects\TSun\Code\wwmous.cpp` | `0x6725D0`, `0x672697`, `0x672A60`, `0x672B66`, `0x672EC0`, `0x672FA0`, `0x6731FF`, `0x6733A2`, `0x6735A0`, `0x6736F0`, `0x6737BC`, `0x67391C`, `0x673A10`, `0x673AB0`, `0x673C20`, `0x673E80`, `0x673F2F`, `0x674070` |
| `DATE` | `0x5EFCA5` |
| `DDRAW.DLL Error code = %08X` | `0x46F185`, `0x46F39A`, `0x46F40E`, `0x46F472`, `0x46F5F0`, `0x46F629`, `0x46F70D`, `0x46F773`, `0x46F80C`, `0x46F8A4`, `0x46F907`, `0x46F9F8`, `0x46FAC3` |
| `DPOD.HVA` | `0x4DC3CE` |
| `DPOD.VXL` | `0x4DC310` |
| `DURA` | `0x5EF633` |
| `DamageLevels` | `0x5771D1` |
| `DamageParticleSystems` | `0x620191` |
| `DamageRadius` | `0x418872`, `0x64213B` |
| `DamageSmokeOffset` | `0x62027B` |
| `DeadBodies` | `0x5AF670` |
| `Debris` | `0x628540` |
| `DebrisMaximums` | `0x61F97F` |
| `DefaultDebrisSmokeSystem` | `0x5B0D2F` |
| `DefaultFireStreamSystem` | `0x5B0D2F` |
| `DefaultFirestormExplosionSystem` | `0x5B0BB7` |
| `DefaultLargeGreySmokeSystem` | `0x5B0BF6` |
| `DefaultLargeRedSmokeSystem` | `0x5B0CB2` |
| `DefaultRepairParticleSystem` | `0x5B0D2F` |
| `DefaultSmallGreySmokeSystem` | `0x5B0C35` |
| `DefaultSmallRedSmokeSystem` | `0x5B0CF1` |
| `DefaultSparkSystem` | `0x5B0C73` |
| `DefaultTestParticleSystem` | `0x5B0D2F` |
| `Deform` | `0x65187A` |
| `DeformThreshhold` | `0x65187A` |
| `DeploySound` | `0x5AF0EF` |
| `DeployingAnim` | `0x43EBF6` |
| `DeploysInto` | `0x61FF2D` |
| `Description` | `0x4467D0`, `0x52D4F0`, `0x52D760`, `0x52DA30`, `0x55A2C0`, `0x5D55D0` |
| `DestNet` | `0x573A68`, `0x573D20`, `0x5E7406` |
| `DestroyWalls` | `0x5B5CF0` |
| `DestroyableCliffs` | `0x4EAA00` |
| `DialMethod` | `0x5D4126`, `0x5D4BC0` |
| `Difficult` | `0x5B5CA0` |
| `Difficulty` | `0x5737F0`, `0x573D20` |
| `DigSound` | `0x5AEBCC` |
| `Digest` | `0x447B40`, `0x447BBE`, `0x447C70`, `0x447CD6`, `0x447DCA`, `0x447EBC`, `0x5D1AC6`, `0x5D514D`, `0x5D57D4` |
| `Direct Draw operation processed without error` | `0x46F150` |
| `Direct X` | `0x46F185`, `0x46F39A`, `0x46F40E`, `0x46F472`, `0x46F5F0`, `0x46F629`, `0x46F70D`, `0x46F773`, `0x46F80C`, `0x46F8A4`, `0x46F907`, `0x46F9F8`, `0x46FAC3` |
| `DirtRoadCurve` | `0x4EAA00` |
| `DirtRoadJunction` | `0x4EAA00` |
| `DirtRoadSlopes` | `0x4EAA00` |
| `DirtRoadStraight` | `0x4EAA00` |
| `DirtTrackTunnels` | `0x4EAA00` |
| `DirtTunnels` | `0x4EAA00` |
| `Disguise` | `0x5B220A` |
| `DllGetVersion` | `0x478E60` |
| `DllRegisterServer` | `0x5E6132` |
| `Dock` | `0x61FAE3` |
| `DockUnload` | `0x43F45F` |
| `DoorAnim` | `0x43EBF6` |
| `DrawFlat` | `0x5771D1` |
| `DropPod` | `0x5AEB0F` |
| `DropPodAngle` | `0x5B1622` |
| `DropPodHeight` | `0x5B1622` |
| `DropPodPuff` | `0x5AE893` |
| `DropPodSpeed` | `0x5B1622` |
| `DropZoneAnim` | `0x5AFC74` |
| `DropZoneRadius` | `0x5AFC74` |
| `Dropd=%p  Master=%p  Dlg=%p` | `0x5794EB` |
| `Duration` | `0x641E90` |
| `ECACHE*.MIX` | `0x4DC170` |
| `EFFICIEN.AUD` | `0x5C9D9F` |
| `EFIC01.SHP` | `0x5C9AEC` |
| `ELOCAL*.MIX` | `0x4DC170` |
| `EMBLEM.AUD` | `0x5C9CDF` |
| `EMEffect` | `0x65187A` |
| `EMPulseCannon` | `0x43F45F` |
| `EMPulseProjectile` | `0x5AE7AD` |
| `EMPulseSparkles` | `0x5AFD52` |
| `EMPulseWarhead` | `0x5AE76F` |
| `ERRO` | `0x56D70D`, `0x56DAA2` |
| `ERROR` | `0x658ABB` |
| `EWGates` | `0x5B504F` |
| `EXPAND%02d.MIX` | `0x4DC930` |
| `EXPAND.DAT` | `0x4926F0` |
| `Easy` | `0x5B5CA0` |
| `Edge` | `0x4BC5C0`, `0x4BC8E0` |
| `Efficiency` | `0x5C9D9F`, `0x5CA0ED` |
| `Elite` | `0x61FAA4` |
| `EliteAbilities` | `0x62027B` |
| `Emblem` | `0x5C9CDF`, `0x5CA7D7` |
| `EnemyHealth` | `0x5AFD52` |
| `ErrorControl_Off` | `0x54E2DB` |
| `ErrorControl_On` | `0x54E2DB` |
| `ErrorCorrection` | `0x5D4126`, `0x5D4BC0` |
| `ExitCoord` | `0x43F45F` |
| `ExpSpread` | `0x5B0D2F` |
| `ExpireAnim` | `0x4186F6`, `0x6420BD` |
| `ExpireSound` | `0x418503`, `0x641F4B` |
| `Explosion` | `0x61FFF0` |
| `ExtraAircraftLight` | `0x5AFD52` |
| `ExtraInfantryLight` | `0x5AFD52` |
| `ExtraUnitLight` | `0x5AFD52` |
| `FACTORY` | `0x4490F0`, `0x44A720` |
| `FINI` | `0x5EF5F6` |
| `FLAG` | `0x5EF7B8` |
| `FULLFNT3.PAL` | `0x55D6B6` |
| `FULLFNT3.SHP` | `0x55D640`, `0x5CD830` |
| `Factory` | `0x43F250` |
| `FillEarliestTeamProbability` | `0x5B2343` |
| `FinalMovie` | `0x4467D0` |
| `Fire` | `0x6516EF` |
| `FirePower` | `0x5B5CF0` |
| `FireSupress` | `0x5B0D2F` |
| `FirestormActiveAnim` | `0x5AFA40` |
| `FirestormAirAnim` | `0x5AFABD` |
| `FirestormGroundAnim` | `0x5AFAFC` |
| `FirestormIdleAnim` | `0x5AFA7E` |
| `FirestormWall` | `0x43F45F` |
| `FirestormWarhead` | `0x5B0B3A` |
| `FlameDamage` | `0x5B0A1F` |
| `FlameDamage2` | `0x5B0A7E` |
| `FlamingInfantry` | `0x5AEA91` |
| `Float` | `0x5B5914` |
| `FlowControl_Hard` | `0x54E2DB` |
| `FogRate` | `0x5AFD52` |
| `Foot` | `0x5B5849` |
| `Foundation` | `0x43FB5D`, `0x625280` |
| `Frame` | `0x5D466F` |
| `FreeMCV` | `0x5B0110` |
| `FriendlyName` | `0x657B92` |
| `GAPLUG` | `0x4BF674` |
| `GDIBarracks` | `0x43F45F` |
| `GDIFirestormGenerator` | `0x5B1EC1` |
| `GDIGateOne` | `0x5B1C8D` |
| `GDIGateTwo` | `0x5B1CCC` |
| `GDIHunterSeeker` | `0x5B1F00` |
| `GDIPowerPlant` | `0x5B1DC6` |
| `GDIPowerTurbine` | `0x5B1E05` |
| `GSKU` | `0x5EF55B` |
| `GameClosed` | `0x5AED75` |
| `GameForming` | `0x5AED75` |
| `GameSpeed` | `0x5737F0`, `0x573D20` |
| `Gameopt parse failed` | `0x5687E1` |
| `Gate` | `0x43F45F` |
| `GateCloseDelay` | `0x43F45F` |
| `GateDown` | `0x5AED75` |
| `GateUp` | `0x5AED75` |
| `GenericBeep` | `0x5AF0EF` |
| `GenericClick` | `0x5AF0EF` |
| `GetActiveWindow` | `0x691B96` |
| `GetDiskFreeSpaceExA` | `0x45FF30` |
| `GetLastActivePopup` | `0x691B96` |
| `Graph` | `0x55547E`, `0x556A20`, `0x55721C` |
| `Gravity` | `0x5AFD52` |
| `Green` | `0x5C6E92`, `0x5C7410` |
| `GreenTile` | `0x4EAA00` |
| `GreenTint` | `0x52E8B0` |
| `Ground` | `0x52E8B0`, `0x5C6E92`, `0x5C7410` |
| `Growth` | `0x628540` |
| `GrowthPercentage` | `0x628540` |
| `GuardArea` | `0x5B59E0` |
| `GuardSound` | `0x5AF0EF` |
| `HALLFAME.DAT` | `0x5CB051`, `0x5CB56D` |
| `HRV?` | `0x5F075D` |
| `HSBuilding` | `0x5AE5A0` |
| `Handle` | `0x5D3F90`, `0x5D4BC0` |
| `HarvesterUnit` | `0x5B1FBC` |
| `HasRadialIndicator` | `0x572610` |
| `HasStupidGuardMode` | `0x43F45F` |
| `HealCrateSound` | `0x5B01CD` |
| `Height` | `0x43F45F`, `0x52D4F0`, `0x52D760`, `0x5E1960` |
| `HeightBase` | `0x4EAA00` |
| `Hey!` | `0x472000` |
| `HomingScatter` | `0x5B0D2F` |
| `Hospital` | `0x43F45F` |
| `Hotkey` | `0x4DF3E0`, `0x4DF565`, `0x574445` |
| `House` | `0x60D180`, `0x60D510` |
| `Houses` | `0x4BC4F0`, `0x4BC860` |
| `Hover` | `0x5B57E0` |
| `HoverPad` | `0x43F45F` |
| `ICBMLauncher` | `0x43F45F` |
| `IDNO` | `0x5EF500` |
| `IMAGEHLP.DLL` | `0x491830` |
| `INB?` | `0x5EFF9D` |
| `INFANTRY` | `0x5D474B` |
| `INK?` | `0x5F049F` |
| `INL?` | `0x5F0171` |
| `INTRO` | `0x4DA910`, `0x4DADB4`, `0x4DB008`, `0x4DB083`, `0x65D243`, `0x660420` |
| `IONSTORM` | `0x4E44A1` |
| `IPA?` | `0x5EFDC4` |
| `ISO%s.PAL` | `0x4EAA00` |
| `Ice1Set` | `0x4EAA00` |
| `Ice2Set` | `0x4EAA00` |
| `Ice3Set` | `0x4EAA00` |
| `IceCrackSounds` | `0x5AFD52` |
| `IceGrowthRate` | `0x5AFD52` |
| `IceShoreSet` | `0x4EAA00` |
| `IceSolidifyFrameTime` | `0x5AFD52` |
| `IdleActionFrequency` | `0x5AFD52` |
| `IgnoresFirestorm` | `0x572610` |
| `Immune` | `0x572610` |
| `ImpatientVoice` | `0x5F2FB4` |
| `In Get_Free_Video_Memory. About to return %d bytes` | `0x46F940` |
| `Incoming` | `0x5B0D2F` |
| `IncomingMessage` | `0x5AED75` |
| `InfDeath` | `0x65187A` |
| `Infantry  (Type:%d(%s)) ` | `0x59E966` |
| `InfantryExplode` | `0x5AECF8` |
| `Init Secondary Mixfiles.....` | `0x4D9390` |
| `InitStringIndex` | `0x5D420B`, `0x5D4BC0` |
| `InitStrings` | `0x5D420B`, `0x5D4BC0` |
| `InprocServer32` | `0x5E5940` |
| `Insignificant` | `0x572610` |
| `Internal Version %s` | `0x4DD086` |
| `Intro` | `0x4DC7B8`, `0x5C68C0`, `0x5C7410`, `0x5E7BBE` |
| `InvisibleInGame` | `0x43F45F` |
| `IonAmbient` | `0x5C6E92`, `0x5C7410` |
| `IonBeam` | `0x5AECB9` |
| `IonBlast` | `0x5AEC7A` |
| `IonBlue` | `0x5C6E92`, `0x5C7410` |
| `IonCannonDamage` | `0x5B0360` |
| `IonCannonWarhead` | `0x5B0AFB` |
| `IonGreen` | `0x5C6E92`, `0x5C7410` |
| `IonGround` | `0x5C6E92`, `0x5C7410` |
| `IonLevel` | `0x5C6E92`, `0x5C7410` |
| `IonRed` | `0x5C6E92`, `0x5C7410` |
| `IsARock` | `0x5771D1` |
| `IsAnimated` | `0x6253E8` |
| `IsAnimatedTiberium` | `0x418503` |
| `IsFlamingGuy` | `0x418503` |
| `IsFlammable` | `0x625280` |
| `IsMeteor` | `0x418462`, `0x641D80` |
| `IsPlug` | `0x43F45F` |
| `IsPowered` | `0x5F2FB4` |
| `IsProcessorFeaturePresent` | `0x685801` |
| `IsScoreRepeat` | `0x573A68`, `0x573D20` |
| `IsScoreShuffle` | `0x573A68`, `0x573D20` |
| `IsTemple` | `0x43F45F` |
| `IsThreatRatingNode` | `0x43F45F` |
| `IsVeinhole` | `0x625280` |
| `IsVeinholeMonster` | `0x5771D1` |
| `IsVeins` | `0x418462`, `0x5771D1` |
| `Joe_Bostic` | `0x65BF70`, `0x65BF90`, `0x65BFC0` |
| `KERNEL32` | `0x685801` |
| `KERNEL32.DLL` | `0x45FF30` |
| `KEYBOARD.INI` | `0x4DF3E0`, `0x574590` |
| `Keyboard.ini` | `0x574445` |
| `LANGRULE.INI` | `0x4D9EA0`, `0x5ADE50` |
| `LCN?` | `0x5EFEDD` |
| `LOCAL.MIX` | `0x4DCA9A` |
| `LOGO01.SHP` | `0x5C9AEC` |
| `LV%d` | `0x65AE27`, `0x65B09E` |
| `Land` | `0x577090` |
| `Landable` | `0x40F300` |
| `LangFilter` | `0x65D090`, `0x65D120`, `0x65DE10` |
| `Language.dll` | `0x46EBD0`, `0x46EDC0` |
| `Language: %s ` | `0x46EE00` |
| `LargeFire` | `0x5AFC35` |
| `LaserColor` | `0x597090` |
| `LaserFence` | `0x43F45F` |
| `LaserFencePost` | `0x43F45F` |
| `LaserInnerColor` | `0x656F60` |
| `LaserOuterColor` | `0x656F60` |
| `LaserOuterSpread` | `0x656F60` |
| `LegalTarget` | `0x572610` |
| `Length` | `0x627350` |
| `LightBlueTint` | `0x43F45F` |
| `LightGreenTint` | `0x43F45F` |
| `LightIntensity` | `0x43F45F` |
| `LightRedTint` | `0x43F45F` |
| `LightVisibility` | `0x43F45F` |
| `LightningRod` | `0x61FFF0` |
| `LightningSound` | `0x5AF0EF` |
| `LobMusic` | `0x65D090`, `0x65D120`, `0x65DE10` |
| `Lob_18_` | `0x65E04D`, `0x65E9E6`, `0x65FF00`, `0x661113`, `0x66A210`, `0x66BB30` |
| `Lob_18_%d` | `0x661070` |
| `LocalSize` | `0x476824`, `0x476CC0`, `0x52E8B0` |
| `LoopCount` | `0x4181F4` |
| `LoopEnd` | `0x4181F4` |
| `LoopStart` | `0x4181F4` |
| `Lose` | `0x5C68C0`, `0x5C7410` |
| `MADRAP` | `0x48DCFC` |
| `MAIN.MIX` | `0x44C4E9` |
| `MAPS` | `0x54121D` |
| `MAPS%02d.MIX` | `0x44C50C`, `0x4DCBAE` |
| `MAPSEL.INI` | `0x559AC0` |
| `MEMO` | `0x5EFB74` |
| `MISSION.INI` | `0x5A8783`, `0x5C455D` |
| `MISSIONS.PKT` | `0x5D4E90`, `0x5D4FD7` |
| `MMRampBase` | `0x4EAA00` |
| `MOVIES%02d.MIX` | `0x44C628`, `0x4DCE02` |
| `MULTI.MIX` | `0x44C586`, `0x4DCC5B` |
| `MUTANT` | `0x5B9160` |
| `ManualControl` | `0x5F30D6` |
| `ManualReload` | `0x61FFF0` |
| `MapCheck.txt` | `0x51FC20` |
| `MapVQ` | `0x55A3E7` |
| `MaxAngularVelocity` | `0x641E5F` |
| `MaxDC` | `0x597D00` |
| `MaxDamage` | `0x5B0D2F` |
| `MaxEC` | `0x597D00` |
| `MaxIQLevels` | `0x5B59E0` |
| `MaxMoney` | `0x5B3C70` |
| `MaxZVel` | `0x641E90` |
| `MaximumAIDefensiveTeams` | `0x5B2343` |
| `Medians` | `0x4EAA00` |
| `MessageBoxA` | `0x691B96` |
| `MessageDelay` | `0x5AFD52` |
| `MetallicDebris` | `0x5AF73C` |
| `Microsoft Visual C++ Runtime Library` | `0x68CC4F` |
| `MinAngularVelocity` | `0x641D80` |
| `MinDamage` | `0x5B0D2F` |
| `MinPlayers` | `0x5D55D0` |
| `MinimumAIDefensiveTeams` | `0x5B2343` |
| `MiscPaveTile` | `0x4EAA00` |
| `Model` | `0x54E1C5` |
| `Modem` | `0x54DF91` |
| `ModemName` | `0x5D3F90`, `0x5D4BC0` |
| `Money` | `0x5B3C70` |
| `MonorailSlopes` | `0x4EAA00` |
| `MoveFlash` | `0x5AFB3A` |
| `MovementRestrictedTo` | `0x63E8E0` |
| `MovementZone` | `0x620A13` |
| `MovieTime` | `0x5AFD52` |
| `MultiMaps` | `0x5D4E90`, `0x5D4FD7` |
| `MultiPlayer` | `0x5D3F90`, `0x5D4BC0` |
| `MultiplayerAICM` | `0x5B2343` |
| `MyEffectivenessCoefficient` | `0x620894` |
| `NAM?` | `0x5EFD92` |
| `NO C` | `0x56D6C5`, `0x56DA71` |
| `NO D` | `0x56D6E9` |
| `NODBarracks` | `0x43F45F` |
| `NONE` | `0x5D466F` |
| `NSGates` | `0x5B504F` |
| `NULL` | `0x58030A` |
| `Name: %s` | `0x483610` |
| `NamedCivilians` | `0x5AFD52` |
| `NaturalParticleLocation` | `0x620191` |
| `NaturalParticleSystem` | `0x620113` |
| `NetCard` | `0x573A68`, `0x573D20` |
| `Network` | `0x573A68`, `0x573D20`, `0x5E7406` |
| `New Player` | `0x565201` |
| `Next` | `0x4181F4` |
| `NextParticleOffset` | `0x597E0E` |
| `No Digest` | `0x5D514D` |
| `No Name` | `0x4EB9AD`, `0x5D514D` |
| `No digest here mate. Nope.` | `0x5D1AC6` |
| `NoThreat` | `0x546EC7` |
| `NoUseTileLandType` | `0x5771D1` |
| `NodAdvancedPower` | `0x5B1E82` |
| `NodGateOne` | `0x5B1D0A` |
| `NodGateTwo` | `0x5B1D49` |
| `NodHunterSeeker` | `0x5B1F3E` |
| `NodRegularPower` | `0x5B1E44` |
| `NodeCount` | `0x41ED70`, `0x41EEC0` |
| `Normal` | `0x5B5CA0`, `0x627350` |
| `Not available` | `0x491C74`, `0x4A76E0`, `0x4DD086` |
| `Not same scenario...` | `0x5687E1` |
| `NotInsertable` | `0x5E5940` |
| `Note` | `0x46F150` |
| `NukeDown` | `0x5AE730` |
| `NukeProjectile` | `0x5AE6F1` |
| `NukeSilo` | `0x43F45F` |
| `NukeWarhead` | `0x5AE683` |
| `NumPlayers` | `0x52D4F0`, `0x52D760`, `0x56937A` |
| `OOSY` | `0x5EF5B9` |
| `Object %d: %x ` | `0x59E5D0`, `0x59E778` |
| `OnAgeCheck` | `0x66D120` |
| `OnFire` | `0x5AF8D6` |
| `Options` | `0x5737F0`, `0x573D20` |
| `OptionsChanged` | `0x5AED75` |
| `Overlay   (Type:%d(%s)) ` | `0x59E99E` |
| `OverlayDataPack` | `0x575D19`, `0x575E80` |
| `OverlayPack` | `0x575A30`, `0x575E80` |
| `Overlays` | `0x55A3E7` |
| `Owner` | `0x62027B` |
| `PATH` | `0x5D3D30`, `0x5D5560` |
| `PLB?` | `0x5F00A9` |
| `PLK?` | `0x5F05AB` |
| `PLL?` | `0x5F03D7` |
| `PLRS` | `0x5EF9D7` |
| `PNGR` | `0x5EF8AF` |
| `PNGS` | `0x5EF880` |
| `PORTNAME` | `0x54E2DB` |
| `POWER` | `0x4490F0`, `0x44A720` |
| `PROC` | `0x5EFB3D` |
| `PROGBAR2.SHP` | `0x52DE10`, `0x5D0C90`, `0x5D15C3` |
| `PadAircraft` | `0x5B2081` |
| `Palette` | `0x559CEF` |
| `Parachute` | `0x5AFB79` |
| `Paralyzed` | `0x546F42` |
| `Paratrooper` | `0x5B2189` |
| `Particle` | `0x651670` |
| `ParticleSystems` | `0x5B4610` |
| `Particles` | `0x5B45A0` |
| `PaveTile` | `0x4EAA00` |
| `PavedRoadEnds` | `0x4EAA00` |
| `PavedRoadSlopes` | `0x4EAA00` |
| `PavedRoads` | `0x4EAA00` |
| `PercentBuilt` | `0x41ED70`, `0x41EEC0` |
| `PhoneBook` | `0x5D420B`, `0x5D4BC0` |
| `PhoneIndex` | `0x5D3F90`, `0x5D4BC0` |
| `Pilot` | `0x5B22C6` |
| `Ping = %d ms` | `0x65E792`, `0x664EEA` |
| `Ping = %dms` | `0x65F839` |
| `PipScale` | `0x62027B` |
| `PlaceAnywhere` | `0x43F45F` |
| `PlaySide%02d` | `0x4DC7B8`, `0x5E7BBE` |
| `Player` | `0x52E8B0`, `0x5C4353`, `0x5C6E92`, `0x5C7410` |
| `Player Addr[%d] = ` | `0x5D48F0` |
| `PlayerAutoCrush` | `0x5B0D2F` |
| `PlayerControl` | `0x4BC5C0`, `0x4BC8E0` |
| `PlayerJoined` | `0x5AED75` |
| `PlayerLeft` | `0x5AED75` |
| `PlayerReturnFire` | `0x5B0D2F` |
| `PlayerScatter` | `0x5B0D2F` |
| `Players` | `0x5ADE50`, `0x5B3DE0` |
| `Please insert disk %d` | `0x472000` |
| `PnPAttachedTo` | `0x54E1C5` |
| `Port` | `0x5D4126`, `0x5D4BC0` |
| `PoseDir` | `0x5AE893` |
| `PostScore` | `0x5C68C0`, `0x5C7410` |
| `Power` | `0x43F45F`, `0x628540` |
| `PowerUp%01dAnim` | `0x43FB5D` |
| `PowerUp%01dDamagedAnim` | `0x43FB5D` |
| `PowerUp%01dLocXX` | `0x43FB5D` |
| `PowerUp%01dLocYY` | `0x43FB5D` |
| `PowerUp%01dLocZZ` | `0x43FB5D` |
| `PowersUpBuilding` | `0x43F45F` |
| `PowersUpToLevel` | `0x43F45F` |
| `PreMapSelect` | `0x5C68C0`, `0x5C7410` |
| `Prerequisite` | `0x62027B` |
| `PrerequisiteBarracks` | `0x5B11CD` |
| `PrerequisiteFactory` | `0x5B11CD` |
| `PrerequisitePower` | `0x5B11CD` |
| `PrerequisiteRadar` | `0x5B11CD` |
| `PrerequisiteTech` | `0x5B11CD` |
| `Preview.bin` | `0x54DBD4`, `0x56A00A` |
| `Primary` | `0x61F97F` |
| `PrintCRC` | `0x5D4799` |
| `PrivateKey` | `0x4D79A0`, `0x4D7A6F` |
| `Production` | `0x5B59E0` |
| `ProgID` | `0x5E5940` |
| `Projectile` | `0x6571BE` |
| `ProneDamage` | `0x65187A` |
| `PublicKey` | `0x4D79A0`, `0x4D7A90` |
| `R%d,%d` | `0x566E58`, `0x6683E6` |
| `RADAR` | `0x4490F0`, `0x44A720` |
| `RECORD.BIN` | `0x5D36A0` |
| `RIGHT` | `0x5737F0` |
| `RING` | `0x56D850` |
| `RULE*.INI` | `0x4D9B00` |
| `RULES.INI` | `0x4D9BA5`, `0x4D9CC8` |
| `RadarEventDurations` | `0x5B3451` |
| `RadarEventSuppressionDistances` | `0x5B3451` |
| `RadarEventVisibilityDurations` | `0x5B3451` |
| `RadarInvisible` | `0x572610` |
| `RadarOff` | `0x5AF0EF` |
| `RadarOn` | `0x5AF0EF` |
| `RadialColor` | `0x572610` |
| `RailgunDamageRadius` | `0x5B0360` |
| `RampBase` | `0x4EAA00` |
| `RampSmooth` | `0x4EAA00` |
| `RandMap.Sed` | `0x52DA30`, `0x5495FC`, `0x54C460`, `0x54C640`, `0x54C6FB`, `0x54DA70`, `0x563E26`, `0x563F5A`, `0x563FD1`, `0x56402B`, `0x566917`, `0x569D70`, `0x5D1A50`, `0x5DD49C`, `0x5DD669`, `0x66721D`, `0x6672FD`, `0x667373`, `0x6673CE`, `0x668EBA`, `0x66B4E1` |
| `RandMap.img` | `0x52C6F0`, `0x54C538`, `0x54C87D`, `0x563F07`, `0x563FD1`, `0x5DD49C`, `0x5DD757`, `0x6672AC`, `0x667373` |
| `RandomRate` | `0x4188DF` |
| `RatioAITriggerTeam` | `0x4BC5C0` |
| `RatioTeamAircraft` | `0x4BC5C0` |
| `RatioTeamInfantry` | `0x4BC5C0` |
| `RatioTeamUnits` | `0x4BC5C0` |
| `RechargeTime` | `0x5F30D6` |
| `RechargeVoice` | `0x5F2FB4` |
| `Recoilless` | `0x43F45F` |
| `Recruitable` | `0x546F19` |
| `RedTint` | `0x52E8B0` |
| `Refinery` | `0x43F45F` |
| `RegPath` | `0x65FEB0` |
| `RegionSize` | `0x52D4F0`, `0x52D760` |
| `Release Build: %s by %s - %s` | `0x4DD086` |
| `RepairDelay` | `0x5B5CF0` |
| `RepairSell` | `0x5B59E0` |
| `Repeat` | `0x627350` |
| `Retaliate` | `0x546F6B` |
| `Rocker` | `0x6516EF` |
| `Rocks` | `0x4EAA00` |
| `RollAngle` | `0x61F6D0` |
| `Rotates` | `0x445E48` |
| `Rotors` | `0x40F300` |
| `RoughGround` | `0x4EAA00` |
| `RoughTile` | `0x4EAA00` |
| `Ruggedness` | `0x52D4F0`, `0x52D760` |
| `RunningFrames` | `0x418503` |
| `SAVE%04lX.%3s` | `0x4F882D`, `0x4F8AA0` |
| `SAVEGAME.NET` | `0x4F9040`, `0x4F907E`, `0x4F91C0` |
| `SCEN` | `0x5EF853` |
| `SCOLD8.AUD` | `0x5C9EBF` |
| `SCORE` | `0x5C9920` |
| `SCORES.MIX` | `0x44C5D7`, `0x4DCD75` |
| `SCRN%04d.pcx` | `0x4E2770` |
| `SHAD` | `0x5EF77E` |
| `SID?` | `0x5EFE2E` |
| `SIDEC%02d.MIX` | `0x4E03DA` |
| `SIDECD%02d.MIX` | `0x4E0584` |
| `SIDEFNT3.PAL` | `0x55D640`, `0x5C9920` |
| `SIDENC%02d.MIX` | `0x4E04A9` |
| `SLOP01Z.%s` | `0x4EAA00` |
| `SOFTWARE\Westwood\Tiberian Sun` | `0x65FEB0`, `0x669A5A` |
| `SOUND.INI` | `0x4D956A` |
| `SOUNDS.MIX` | `0x4DCCE8` |
| `SPED` | `0x5EFC2A` |
| `SPEECH%02d.MIX` | `0x4E0240` |
| `SPID` | `0x5EF9F6` |
| `SUN.EXE` | `0x5BC874` |
| `SUN.HLP` | `0x65ACB0`, `0x65ACD0` |
| `SUN.INI` | `0x4DC7B8`, `0x573D20`, `0x5D3F90`, `0x5D4BC0`, `0x5E7294`, `0x65D120` |
| `SYNC%01d.TXT` | `0x59DAE0` |
| `SandTile` | `0x4EAA00` |
| `SavourDelay` | `0x5AFD52` |
| `Saw game completion due to player defeat` | `0x4B8FDE` |
| `Scatter` | `0x546F94`, `0x5B59E0` |
| `ScatterSound` | `0x5AF0EF` |
| `Scenario` | `0x4467D0`, `0x55A2C0`, `0x627350` |
| `ScoldSound` | `0x5AF0EF` |
| `Scorches` | `0x5B0360` |
| `Scorches1` | `0x5B04DB` |
| `Scorches2` | `0x5B05D6` |
| `Scorches3` | `0x5B06C1` |
| `Scorches4` | `0x5B07AB` |
| `ScoreVolume` | `0x573A68`, `0x573D20` |
| `ScrapVoxelDebris` | `0x5B1A64` |
| `ScreenHeight` | `0x5737F0`, `0x573D20`, `0x5E7406` |
| `ScreenWidth` | `0x5737F0`, `0x573D20`, `0x5E7406` |
| `ScriptTypes` | `0x5CE7C0` |
| `ScrollMethod` | `0x5737F0`, `0x573D20` |
| `ScrollMultiplier` | `0x5AED75` |
| `ScrollRate` | `0x5737F0`, `0x573D20` |
| `Scrollbar` | `0x58124D` |
| `Secondary` | `0x61FA65` |
| `Seed` | `0x52D4F0`, `0x52D760` |
| `Selectable` | `0x572610` |
| `SellBack` | `0x5B59E0` |
| `SellSound` | `0x5AED75` |
| `Sending game results because I quit, but didn't see completion` | `0x59D299` |
| `SensorArray` | `0x43F45F` |
| `Sequence` | `0x4D3670` |
| `Serial` | `0x561990` |
| `SerialDefaults` | `0x5D3F90`, `0x5D4126`, `0x5D420B`, `0x5D4BC0` |
| `Server %d` | `0x65E36C` |
| `ShadowGrow` | `0x5B3C70` |
| `ShakeScreen` | `0x5AED75` |
| `ShareBarrelData` | `0x641F4B` |
| `ShareBodyData` | `0x641F4B` |
| `ShareSource` | `0x642214` |
| `ShareTurretData` | `0x641F4B` |
| `ShorePieces` | `0x4EAA00` |
| `ShouldFogRemove` | `0x418503` |
| `ShouldUseCellDrawer` | `0x4188DF` |
| `ShowAll` | `0x65D090`, `0x65D120`, `0x65DE10` |
| `ShroudGrow` | `0x5AED75` |
| `ShroudRate` | `0x5AFD52` |
| `Side` | `0x4BCA4A`, `0x4C74D0`, `0x5D3F90`, `0x5D4BC0`, `0x627350` |
| `SidebarCameoText` | `0x5737F0`, `0x573D20` |
| `SidebarImage` | `0x5F3140` |
| `SiloDamage` | `0x43F45F` |
| `SilverCrate` | `0x5B02D4` |
| `Size` | `0x476390`, `0x476CC0`, `0x52E8B0`, `0x5952A0`, `0x5954F0` |
| `SlopeSetPieces` | `0x4EAA00` |
| `SlopeSetPieces2` | `0x4EAA00` |
| `SmallFire` | `0x5AFBF6` |
| `Smoke` | `0x5AF9A2`, `0x5AFA01` |
| `Smudge    (Type:%d(%s)) ` | `0x59E9D6` |
| `SnowOccupationBits` | `0x6253E8` |
| `Socket` | `0x573A68`, `0x573D20`, `0x5E7406` |
| `SoloCrateMoney` | `0x5B02D4` |
| `SoundLatency` | `0x573A68`, `0x573D20` |
| `SoundList` | `0x6473D0` |
| `SoundVolume` | `0x573A68`, `0x573D20` |
| `Sounds` | `0x559F10` |
| `Sparky` | `0x6516EF` |
| `SpawnCount` | `0x418462`, `0x641F4B` |
| `SpawnDirection` | `0x597090` |
| `SpawnsTiberium` | `0x625280` |
| `SpeakDelay` | `0x5AFD52` |
| `SpecialZOverlay` | `0x43EBF6` |
| `SpeedType` | `0x63E8E0` |
| `SplashList` | `0x5B094A` |
| `Spread` | `0x628540`, `0x651670` |
| `SpreadPercentage` | `0x628540` |
| `Start` | `0x4181F4` |
| `StartColor1` | `0x597E0E` |
| `StartColor2` | `0x597E0E` |
| `StartSound` | `0x418503`, `0x641F4B` |
| `StopSound` | `0x5AF0EF` |
| `Storage` | `0x61FAE3` |
| `StretchMovies` | `0x5737F0`, `0x573D20` |
| `SuperWeapon` | `0x43F45F` |
| `SuperWeapon2` | `0x43F45F` |
| `SuperWeapons` | `0x5B59E0` |
| `SuspendVoice` | `0x5F2FB4` |
| `SyncBug` | `0x5D466F`, `0x5D4799` |
| `SystemError` | `0x5AED75` |
| `System\CurrentControlSet\Control\Class` | `0x54DF54` |
| `System\CurrentControlSet\Services\Class\Modem` | `0x54DEA0` |
| `TARGET1.SHP` | `0x54147E` |
| `TARGET2.SHP` | `0x5414DB` |
| `TECH` | `0x44A720`, `0x5EF824` |
| `TEXT1.AUD` | `0x55547E`, `0x55D828`, `0x5CD330` |
| `TEXT2.AUD` | `0x55D828`, `0x5CD330` |
| `TEXT3.AUD` | `0x55D828`, `0x5CD330` |
| `THEME.INI` | `0x4D9741` |
| `TIBR` | `0x5EF6D7` |
| `TIBSUN.MIX` | `0x4DC930` |
| `TID?` | `0x5EFE63` |
| `TILE` | `0x4EB9AD` |
| `TIME` | `0x5EFAAA` |
| `TIME01.SHP` | `0x5C9AEC` |
| `TMCJ4F.INI` | `0x5C4957` |
| `TREE%d%d` | `0x5387C0` |
| `TRNY` | `0x5EF58A` |
| `TRUCKA` | `0x6078CB`, `0x60B950` |
| `TRUCKB` | `0x607867`, `0x60B8E0` |
| `TS%d` | `0x471DC8`, `0x647A70` |
| `TSCREDIT.txt` | `0x48D8E0` |
| `TUTORIAL.INI` | `0x4D956A` |
| `TV%d` | `0x65AD6F`, `0x65AFBB` |
| `TYPE.AUD` | `0x5C9E5F` |
| `Target` | `0x5D4799` |
| `TargetEffectivenessCoefficient` | `0x6208F4` |
| `TargetSpecialThreatCoefficient` | `0x620954` |
| `TargetStrengthCoefficient` | `0x6209B3` |
| `Targets` | `0x55A3E7` |
| `TaskForces` | `0x606630`, `0x6067F0` |
| `TeamAddSelect_%d` | `0x4E0C20` |
| `TeamCenter_%d` | `0x4E0D90` |
| `TeamCreate_%d` | `0x4E0900` |
| `TeamDelays` | `0x5B2343` |
| `TeamSelect_%d` | `0x4E0A70` |
| `TeamTypes` | `0x60D890`, `0x60D990` |
| `Technician` | `0x5B2287` |
| `TemperateOccupationBits` | `0x6253E8` |
| `Terrain   (Type:%d(%s)) ` | `0x59EA0B` |
| `TeslaCharge` | `0x5AF0EF` |
| `TeslaZap` | `0x5AF0EF` |
| `Text%d` | `0x55A3E7` |
| `TextRect` | `0x559CEF` |
| `Theme` | `0x5C68C0`, `0x5C7410` |
| `Themes` | `0x627420` |
| `TibPass99` | `0x6615B4` |
| `TibSun` | `0x6615B4` |
| `Tiberian Sun` | `0x46EDC0`, `0x5E5570`, `0x65C040`, `0x65C157` |
| `TiberiumExplosionDamage` | `0x5B0360` |
| `TiberiumLayout` | `0x52D4F0`, `0x52D760` |
| `TiberiumSpawnType` | `0x418462` |
| `TiberiumSpreadRadius` | `0x418462` |
| `TiberiumStrength` | `0x5B0360` |
| `Tiberiums` | `0x6287E0` |
| `TickTank` | `0x43F45F` |
| `Tile%02dAnim` | `0x4EBD12` |
| `Tile%02dAttachesTo` | `0x4EBD12` |
| `Tile%02dXOffset` | `0x4EBD12` |
| `Tile%02dYOffset` | `0x4EBD12` |
| `Tile%02dZAdjust` | `0x4EBD12` |
| `TileSet%04d` | `0x4EAA00` |
| `Tiled` | `0x4188DF` |
| `Time` | `0x52D4F0`, `0x52D760` |
| `TimerWarning` | `0x5AFD52` |
| `TireVoxelDebris` | `0x5B19C4` |
| `ToOverlay` | `0x43F45F` |
| `ToTile` | `0x44198F`, `0x442170` |
| `TogglePower` | `0x43F45F` |
| `Tool Tip` | `0x57AE40`, `0x57C447` |
| `ToolTips` | `0x5737F0`, `0x573D20` |
| `TotalAITeamCap` | `0x5B2343` |
| `Track` | `0x5B588D` |
| `TrackTunnels` | `0x4EAA00` |
| `TrailerAnim` | `0x4187B4`, `0x6420FC` |
| `TrailerSeperation` | `0x418872` |
| `TrainBridgeSet` | `0x4EAA00` |
| `TreeFire` | `0x5AF0EF` |
| `TreeTargeting` | `0x5B0D2F` |
| `Tunnels` | `0x4EAA00` |
| `TurnRate` | `0x5B5B70` |
| `Turret` | `0x61FFF0` |
| `TurretSpins` | `0x61FFF0` |
| `Tutorial` | `0x4D956A` |
| `Type` | `0x5C9E5F`, `0x5CCB77`, `0x5D466F`, `0x5F2FB4` |
| `Typelib` | `0x5E5940` |
| `UNB?` | `0x5F0045` |
| `UNIT` | `0x5D476D`, `0x5EF7F5` |
| `UNK?` | `0x5F0547` |
| `UNL?` | `0x5F0373` |
| `UndeploysInto` | `0x61FFB1` |
| `UnderDoorAnim` | `0x43EBF6` |
| `Unit      (Type:%d(%s)) ` | `0x59EA40` |
| `UnitActionLines` | `0x5737F0`, `0x573D20` |
| `UnitCount` | `0x5B3C70` |
| `UnitCrateType` | `0x5B01CD` |
| `UnitReload` | `0x43F45F` |
| `UnitRepair` | `0x43F45F` |
| `UnloadingHarvester` | `0x5AE830` |
| `UrbanPresence` | `0x52D4F0`, `0x52D760` |
| `UseChargeDrain` | `0x5F30D6` |
| `UseNormalLight` | `0x4188DF` |
| `VEINHOLE.%s` | `0x644080` |
| `VERS` | `0x5EFC57` |
| `VERSION.TXT` | `0x646070`, `0x64610B`, `0x6461AC`, `0x646270`, `0x646330`, `0x6463E0`, `0x646489`, `0x6465A0` |
| `VIDM` | `0x5EFBBD` |
| `Value` | `0x628540` |
| `VariableNames` | `0x5C4413`, `0x5C4470`, `0x5C6410`, `0x5C6442`, `0x5C66B0`, `0x5C6720` |
| `Vegetation` | `0x52D4F0`, `0x52D760` |
| `VeinAttack` | `0x5AEAD0` |
| `VeinGrowthRate` | `0x5AFD52` |
| `Veinhole` | `0x65187A` |
| `VeinholeTypeClass` | `0x5B3451` |
| `VeinholeWarhead` | `0x5B0B79` |
| `Verses` | `0x65187A` |
| `Version %s` | `0x4DD086` |
| `VersionIndependentProgID` | `0x5E5940` |
| `VeteranAbilities` | `0x62027B` |
| `Video` | `0x5737F0`, `0x573A68`, `0x573D20`, `0x5E7406` |
| `VideoBackBuffer` | `0x5E7406` |
| `VoiceAttack` | `0x61FAE3` |
| `VoiceDie` | `0x61FAE3` |
| `VoiceFeedback` | `0x61FAE3` |
| `VoiceMove` | `0x61FAE3` |
| `VoiceOver` | `0x55A3E7` |
| `VoiceSelect` | `0x61FAE3` |
| `VoiceVolume` | `0x573A68`, `0x573D20` |
| `Volume` | `0x646E80` |
| `Voxel` | `0x572610` |
| `VoxelAnims` | `0x5B44C0` |
| `VoxelIndex` | `0x641F4B` |
| `WIPE.AUD` | `0x5C9AEC` |
| `WOnline` | `0x65D090`, `0x65D120`, `0x65DE10` |
| `WW.TiberianSun` | `0x5E5570`, `0x5E5940` |
| `WW.TiberianSun.1` | `0x5E5570`, `0x5E5940` |
| `WW.TiberianSun.1\CLSID` | `0x5E5940` |
| `WW.TiberianSun\CLSID` | `0x5E5940` |
| `WW.TiberianSun\CurVer` | `0x5E5940` |
| `Wake` | `0x5AEA56` |
| `WallTower` | `0x5B1D88` |
| `Warheads` | `0x5AE250`, `0x5B4530` |
| `Water` | `0x476390` |
| `WaterAmount` | `0x52D4F0`, `0x52D760` |
| `WaterCaves` | `0x4EAA00` |
| `WaterCliffs` | `0x4EAA00` |
| `WaterSet` | `0x4EAA00` |
| `WaterfallEast` | `0x4EAA00` |
| `WaterfallNorth` | `0x4EAA00` |
| `WaterfallSouth` | `0x4EAA00` |
| `WaterfallWest` | `0x4EAA00` |
| `WaypointAnimationSpeed` | `0x5AE8E5` |
| `Waypoints` | `0x56937A`, `0x5C7FE0` |
| `WeaponType` | `0x5F2F60` |
| `WeaponsFactory` | `0x43F45F` |
| `Wheel` | `0x5B58D1` |
| `Width` | `0x52D4F0`, `0x52D760`, `0x5E1960` |
| `Wipe` | `0x5C9AEC`, `0x5C9F91` |
| `WobbleDeviation` | `0x5B5B70` |
| `WobblesPerSecond` | `0x5B5B70` |
| `Wood` | `0x6516EF` |
| `WoodCrate` | `0x5B02D4` |
| `WoodCrateImg` | `0x5B0110` |
| `XXICON` | `0x6213B0` |
| `YDrawOffset` | `0x418503` |
| `You can't create a channel here` | `0x663289` |
| `Zombie` | `0x546EF0` |
| `[%s]` | `0x5913B7`, `0x65F132`, `0x65F5EB` |
| `[%s] %s` | `0x548470`, `0x548C7F`, `0x5497FA`, `0x549EC4`, `0x549F47`, `0x54B964`, `0x54BF5F`, `0x54CDC5`, `0x54D284`, `0x54D307`, `0x562CEB`, `0x5630D0`, `0x56317D`, `0x56607B`, `0x566C49`, `0x6628C1`, `0x66557C`, `0x666E58`, `0x668283` |
| `[%s] : %s` | `0x66B920` |
| `[Digest]` | `0x5D57D4` |
| `[Map]` | `0x5958C0` |
| `[open] %s` | `0x65CA70` |
| `\StringFileInfo\%04X%04X\FileVersion` | `0x46EE00` |
| `\StringFileInfo\%04X%04X\InternalName` | `0x46EE00` |
| `\StringFileInfo\%04X%04X\ProductVersion` | `0x6466E0` |
| `\VarFileInfo\Translation` | `0x46EE00`, `0x6466E0` |
| `\\.\%c:` | `0x44C77C`, `0x44C781`, `0x44C966`, `0x44C96B`, `0x44CB43`, `0x44CB48`, `0x44CD3D`, `0x44CD42`, `0x44CEB4`, `0x44CEB9`, `0x44D158`, `0x44D15D`, `0x44D315`, `0x44D31A`, `0x44D4A5`, `0x44D4AA` |
| `\\.\MONO` | `0x54E730` |
| `\\.\vwin32` | `0x44D5E0`, `0x44D710`, `0x44D830`, `0x44DC10` |
| `\\Lore\Projects\Projects\C&C2\ip_pool\*.*` | `0x4DA340` |
| `____  _  _ ` | `0x45E56B` |
| `____ __                            _` | `0x45E8B6`, `0x45EA42` |
| `____ __  _ ` | `0x45E4DE` |
| `am/pm` | `0x692C76` |
| `arrow_dd.pcx` | `0x5859E0`, `0x661E56` |
| `arrow_du.pcx` | `0x5859E0`, `0x661E56` |
| `arrow_ud.pcx` | `0x5859E0`, `0x661E56` |
| `arrow_uu.pcx` | `0x5859E0`, `0x661E56` |
| `b%c%c_li%d.pcx` | `0x57DA3C` |
| `b%c%c_mi%d.pcx` | `0x57DA3C` |
| `b%c%c_ri%d.pcx` | `0x57DA3C` |
| `b%ce_li%d.pcx` | `0x5A90A0` |
| `b%ce_mi%d.pcx` | `0x5A90A0` |
| `b%ce_ri%d.pcx` | `0x5A90A0` |
| `b350c6d2-2f36-11d3-a72c-0090272fa661` | `0x5E6065` |
| `badcount.txt` | `0x59B670` |
| `bar_ll.pcx` | `0x5859E0`, `0x588D11` |
| `bar_lr.pcx` | `0x5859E0`, `0x588D11` |
| `bar_ul.pcx` | `0x5859E0`, `0x588D11` |
| `bar_ur.pcx` | `0x5859E0`, `0x588D11` |
| `bde_li24.pcx` | `0x5859E0` |
| `bde_li30.pcx` | `0x5859E0` |
| `bde_mi24.pcx` | `0x5859E0` |
| `bde_mi30.pcx` | `0x5859E0` |
| `bde_ri24.pcx` | `0x5859E0` |
| `bde_ri30.pcx` | `0x5859E0` |
| `bits_a.pcx` | `0x588A60` |
| `bits_i.pcx` | `0x588A60` |
| `bst_chkd.pcx` | `0x5859E0` |
| `bst_chkg.pcx` | `0x5859E0` |
| `bst_uchk.pcx` | `0x5859E0` |
| `bst_uckg.pcx` | `0x5859E0` |
| `bud_li24.pcx` | `0x5859E0` |
| `bud_li30.pcx` | `0x5859E0` |
| `bud_mi24.pcx` | `0x5859E0` |
| `bud_mi30.pcx` | `0x5859E0` |
| `bud_ri24.pcx` | `0x5859E0` |
| `bud_ri30.pcx` | `0x5859E0` |
| `bue_li24.pcx` | `0x5859E0` |
| `bue_li30.pcx` | `0x5859E0` |
| `bue_mi24.pcx` | `0x5859E0` |
| `bue_mi30.pcx` | `0x5859E0` |
| `bue_ri24.pcx` | `0x5859E0` |
| `bue_ri30.pcx` | `0x5859E0` |
| `c%ce_i.pcx` | `0x57F7B9` |
| `cap%04d.pcx` | `0x4FC056` |
| `ccd_i.pcx` | `0x5859E0` |
| `cce_i.pcx` | `0x5859E0` |
| `comctl32.dll` | `0x5E6132` |
| `cud_i.pcx` | `0x5859E0` |
| `cue_i.pcx` | `0x5859E0` |
| `dWindows` | `0x640867` |
| `dbak6440.pcx` | `0x5859E0`, `0x588D11` |
| `dlgsys` | `0x5798AB`, `0x57DD23`, `0x57DFBC`, `0x57F3C3`, `0x57F7B9`, `0x58030A`, `0x584CFD`, `0x5A92F0` |
| `dlgsysa.pcx` | `0x5859E0` |
| `dlgsysi.pcx` | `0x5859E0` |
| `dnarrow%c.pcx` | `0x5868AE` |
| `dnarrow.pcx` | `0x661E56` |
| `dnarrowp.pcx` | `0x5859E0` |
| `dnarrowr.pcx` | `0x5859E0` |
| `e+000` | `0x685989` |
| `except.txt` | `0x492570` |
| `gdii.pcx` | `0x560E0A`, `0x5859E0`, `0x65F6C3` |
| `gt%d.bmp` | `0x65E850`, `0x669E6C` |
| `gt-1.bmp` | `0x65E1BC`, `0x65E36C`, `0x65E489` |
| `gt18.bmp` | `0x65E711`, `0x65EA89`, `0x65EA9F`, `0x664E6B` |
| `http://apiregister.westwood.com/consent_form/index_%d.html` | `0x66197B` |
| `http://apiregister.westwood.com/support/banned_%d.html` | `0x660852` |
| `http://apiregister.westwood.com/support/disabled_%d.html` | `0x660885` |
| `http://apiregister.westwood.com/support/serialbanned_%d.html` | `0x660899` |
| `http://apiregister.westwood.com/support/serialdup_%d.html` | `0x6608AD` |
| `http://battleclans.westwood.com/tibsun/index_%d.html` | `0x663DF2` |
| `http://www.westwood.com/westwoodonline/tournaments/tibsun/index_%d.html` | `0x665B1D` |
| `http://www.westwood.com/westwoodonline/tournaments/tibsun/index_clan_%d.html` | `0x665AB5` |
| `iveWindow` | `0x4037B0` |
| `leftbar.pcx` | `0x57CC4F`, `0x5859E0`, `0x588D11` |
| `mPoint` | `0x403720` |
| `nodi.pcx` | `0x560E67`, `0x5859E0`, `0x65F790` |
| `none` | `0x404A30`, `0x40F4A0`, `0x4104FF`, `0x410E00`, `0x418503`, `0x4186F6`, `0x4187B4`, `0x418D20`, `0x419130`, `0x441870`, `0x446280`, `0x4C7A90`, `0x4D3600`, `0x4D4020`, `0x577540`, `0x597640`, `0x5985B0`, `0x5CE7C0`, `0x5CEA40`, `0x5E1B80`, `0x5F32A0`, `0x604200`, `0x6045F0`, `0x606630`, `0x606A70`, `0x60D800`, `0x60D890`, `0x625650`, `0x62DD00`, `0x62E1B0`, `0x62E3D0`, `0x63EDE0`, `0x6426D0`, `0x642880`, `0x651D30`, `0x6576F0` |
| `rPos` | `0x619DD0`, `0x640867` |
| `rightbar.pcx` | `0x57CC4F`, `0x5859E0`, `0x588D11` |
| `sb_psh_d.pcx` | `0x5859E0` |
| `sb_psh_u.pcx` | `0x5859E0` |
| `sb_rel_d.pcx` | `0x5859E0` |
| `sb_rel_u.pcx` | `0x5859E0` |
| `sbgripb.pcx` | `0x583AAD`, `0x5859E0` |
| `sbgripm.pcx` | `0x583AAD`, `0x5859E0` |
| `sbgript.pcx` | `0x583AAD`, `0x5859E0` |
| `tab_fbl.pcx` | `0x57DFBC`, `0x5859E0` |
| `tab_fbm.pcx` | `0x57DFBC`, `0x5859E0` |
| `tab_fbr.pcx` | `0x57DFBC`, `0x5859E0` |
| `tab_fml.pcx` | `0x57DFBC`, `0x5859E0` |
| `tab_fmr.pcx` | `0x57DFBC`, `0x5859E0` |
| `tab_ftl.pcx` | `0x57DFBC`, `0x5859E0` |
| `tab_ftm.pcx` | `0x57DFBC`, `0x5859E0` |
| `tab_ftr.pcx` | `0x57DFBC`, `0x5859E0` |
| `tab_tl%c.pcx` | `0x57DFBC` |
| `tab_tld.pcx` | `0x5859E0` |
| `tab_tlu.pcx` | `0x57DF3C`, `0x57DFBC`, `0x5859E0` |
| `tab_tm%c.pcx` | `0x57DFBC` |
| `tab_tmd.pcx` | `0x5859E0` |
| `tab_tmu.pcx` | `0x5859E0` |
| `tab_tr%c.pcx` | `0x57DFBC` |
| `tab_trd.pcx` | `0x5859E0` |
| `tab_tru.pcx` | `0x5859E0` |
| `trakgrip.pcx` | `0x584995`, `0x5859E0` |
| `trofl.pcx` | `0x58030A`, `0x584995`, `0x5859E0` |
| `trofm.pcx` | `0x58030A`, `0x584995`, `0x5859E0` |
| `trofr.pcx` | `0x58030A`, `0x584995`, `0x5859E0` |
| `uparrow%c.pcx` | `0x586880` |
| `uparrow.pcx` | `0x661E56` |
| `uparrowp.pcx` | `0x5859E0` |
| `uparrowr.pcx` | `0x5859E0` |
| `user32.dll` | `0x691B96` |
| `voxels.vpl` | `0x4D8C5C` |
| `wodact.pcx` | `0x5859E0`, `0x661E56` |
| `wodban.pcx` | `0x5859E0`, `0x663950` |
| `wodclan.pcx` | `0x5859E0`, `0x663950` |
| `woddcon.pcx` | `0x5859E0` |
| `wodfgame.pcx` | `0x5859E0`, `0x663950` |
| `wodfind.pcx` | `0x5859E0`, `0x663950` |
| `wodhelp.pcx` | `0x5859E0`, `0x663950` |
| `wodkick.pcx` | `0x56416B`, `0x5859E0`, `0x663950` |
| `wodleave.pcx` | `0x5859E0` |
| `wodopt.pcx` | `0x5859E0`, `0x663950` |
| `wodref.pcx` | `0x5859E0`, `0x661E56`, `0x6645DC` |
| `wodsqlch.pcx` | `0x5859E0`, `0x663950` |
| `wodtrny.pcx` | `0x5859E0`, `0x663950` |
| `wolacpt.pcx` | `0x560F61`, `0x5859E0`, `0x65E429`, `0x65F80C` |
| `wolclan.pcx` | `0x5859E0`, `0x65E6E9`, `0x664E43` |
| `woldata.key` | `0x561990` |
| `wolhost.pcx` | `0x560E8B`, `0x5859E0`, `0x65F7BB` |
| `woloper.pcx` | `0x5859E0`, `0x65F265`, `0x65F26E` |
| `wolpriv.pcx` | `0x5859E0`, `0x65E724`, `0x664E7E` |
| `wolsqlch.pcx` | `0x5859E0`, `0x65F31D` |
| `woltrny.pcx` | `0x5859E0`, `0x65E598`, `0x664D4A` |
| `woluser.pcx` | `0x5859E0` |
| `wolvoice.pcx` | `0x5859E0`, `0x65F2DC` |
| `wouact.pcx` | `0x5859E0`, `0x661E56` |
| `wouban.pcx` | `0x5859E0`, `0x663950` |
| `wouclan.pcx` | `0x5859E0`, `0x663950` |
| `woudcon.pcx` | `0x5859E0` |
| `woufgame.pcx` | `0x5859E0`, `0x663950` |
| `woufind.pcx` | `0x5859E0`, `0x663950` |
| `wouhelp.pcx` | `0x5859E0`, `0x663950` |
| `woukick.pcx` | `0x56416B`, `0x5859E0`, `0x663950` |
| `wouleave.pcx` | `0x5859E0` |
| `wouopt.pcx` | `0x5859E0`, `0x663950` |
| `wouref.pcx` | `0x5859E0`, `0x661E56`, `0x6645DC` |
| `wousqlch.pcx` | `0x5859E0`, `0x663950` |
| `woutrny.pcx` | `0x5859E0`, `0x663950` |
| `zyxabc` | `0x647BAC` |
