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

614 of the 1209 referenced strings were not matched to a reader in this
pass.  They are either format strings, file extensions, section names,
or keys read by readers not enumerated here.

| key | referenced at VA(s) |
|---|---|
| `%.28s %02d` | `0x4EBBC5` |
| `%02X %02X %02X %02X : %02X %02X %02X %02X %02X %02X` | `0x4E6D90` |
| `%02d` | `0x4EBBC5` |
| `%02x` | `0x410CE6` |
| `%08X` | `0x405D50` |
| `%d  (%d)` | `0x65F900` |
| `%d,%d` | `0x4D7347`, `0x62A6C0`, `0x62A700` |
| `%d,%d,%d` | `0x43FB5D`, `0x4484A0`, `0x4D7527` |
| `%d,%d,%d,%d` | `0x4D6530`, `0x4D66AA` |
| `%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%s,%d,%d,%s,%s:` | `0x65D4B0` |
| `%d,%d,%d,%d,%d,%d,%d,%s` | `0x65DBED` |
| `%d,%d,%d,%s` | `0x4D3670` |
| `%d,%d,%s` | `0x6263B0` |
| `%d,%d,%s,%d,%d,%d,%d,%s` | `0x5FE0F3` |
| `%d,%s` | `0x48EA00`, `0x48EAA0` |
| `%d.%d.%d.%d` | `0x5C2A87`, `0x66F8F0` |
| `%s %s` | `0x66C1B0`, `0x66C1F0` |
| `%s : %s` | `0x669D60`, `0x66B960` |
| `%s%d` | `0x483430` |
| `%s,%d,%d,` | `0x65D4B0` |
| `%s,%s,%s,%d,%d,%d,%d,%d` | `0x62DDFC` |
| `%s,%s,%s,%d,%d,%s,%s,%lf,%lf,%lf,%d,%d,%d,%d,%s,%d,%d,%d` | `0x410CE6` |
| `%s,<none>` | `0x604470` |
| `%s-%s` | `0x65BFC0` |
| `%s.INI` | `0x4EAA00` |
| `%s.MIX` | `0x4DFEE0` |
| `%s.PAL` | `0x4E0045` |
| `%sBARL` | `0x571D41`, `0x571DEC` |
| `.AUD` | `0x627930`, `0x6480C0` |
| `.HVA` | `0x43EFBD`, `0x43F15A`, `0x571902`, `0x571AA5`, `0x571DEC` |
| `.MMT` | `0x4EBF59` |
| `.SED` | `0x5C27C0` |
| `.SHP` | `0x418DD0`, `0x43EB20`, `0x43EBF6`, `0x441F00`, `0x5724A0`, `0x572610`, `0x5729B0`, `0x5771D1`, `0x5E1960`, `0x5F2DB0`, `0x5F318E`, `0x620A13`, `0x6213B0`, `0x625280`, `0x63E8E0`, `0x63ED30` |
| `.VXL` | `0x43EED0`, `0x43F028`, `0x571870`, `0x571A12`, `0x571D41` |
| `0,NONE` | `0x5B56B0` |
| `100%%,100%%,100%%,100%%,100%%` | `0x65187A` |
| `29e3bb2a-2f36-11d3-a72c-0090272fa661` | `0x5E5B80` |
| `<none>` | `0x40F4A0`, `0x413870`, `0x418503`, `0x4186F4`, `0x4187B4`, `0x418D20`, `0x419130`, `0x441870`, `0x446280`, `0x448EB0`, `0x460110`, `0x460160`, `0x4D3600`, `0x4D4020`, `0x577540`, `0x5973F0`, `0x597640`, `0x597E0E`, `0x5981D0`, `0x5C68C0`, `0x5CEA40`, `0x6045F0`, `0x606A70`, `0x625650`, `0x62DA00`, `0x62DA7A`, `0x62E1B0`, `0x62E3D0`, `0x63EDE0`, `0x6426D0`, `0x642880`, `0x647650`, `0x651D30`, `0x6576F0` |
| `<program name unknown>` | `0x68CC4F` |
| `>>> %s` | `0x66BCB0` |
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
| `Accessibility` | `0x52D4F0`, `0x52D760` |
| `Action` | `0x5C68C0`, `0x5C7410`, `0x5F2FB4` |
| `AirburstWeapon` | `0x445E48` |
| `AllowableUnitMaximums` | `0x5C6DFA`, `0x5C7410` |
| `AllowableUnits` | `0x5C6C9A`, `0x5C7410` |
| `AlphaImage` | `0x572610` |
| `Ambient` | `0x52E8B0`, `0x5C6E8E`, `0x5C7410` |
| `AnimHigh` | `0x445EAE` |
| `AnimList` | `0x6516EF` |
| `AnimLow` | `0x445EAE` |
| `AnimPalette` | `0x445EAE` |
| `AnimRate` | `0x445EAE` |
| `AnimationProbability` | `0x6253D8` |
| `AnimationRate` | `0x6253D8` |
| `Armament: %s` | `0x483770` |
| `Armor: %s` | `0x4837B0` |
| `Armory` | `0x43F45F` |
| `Artillary` | `0x43F45F` |
| `AttachedSystem` | `0x6421AE` |
| `Attempted a typeid of NULL pointer!` | `0x682FBD` |
| `AuxBuilding` | `0x5F30A3` |
| `AuxSound1` | `0x61FAE3` |
| `AuxSound2` | `0x61FAE3` |
| `BARRACKS` | `0x44A720` |
| `BATTLE*.INI` | `0x4D9940` |
| `BATTLE.INI` | `0x4D99DF` |
| `BUILDNGZ.SHP` | `0x43E300` |
| `BaseUnit` | `0x5B1F7D` |
| `Battles` | `0x446860` |
| `BibShape` | `0x43EBF6` |
| `Biome` | `0x52D4F0`, `0x52D760` |
| `BlackTile` | `0x4EAA00` |
| `Blue` | `0x5C6E8E`, `0x5C7410` |
| `BlueTint` | `0x52E8B0` |
| `BounceAnim` | `0x418503`, `0x641F4B` |
| `BounceSound` | `0x418503`, `0x641F4B` |
| `Bouncer` | `0x4188DF` |
| `BridgeBottomLeft1` | `0x4EAA00` |
| `BridgeBottomLeft2` | `0x4EAA00` |
| `BridgeBottomRight1` | `0x4EAA00` |
| `BridgeBottomRight2` | `0x4EAA00` |
| `BridgeMiddle1` | `0x4EAA00` |
| `BridgeMiddle2` | `0x4EAA00` |
| `BridgeRepairHut` | `0x43F45F` |
| `BridgeSet` | `0x4EAA00` |
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
| `BuildHelipad` | `0x5B4EA6` |
| `BuildLimit` | `0x61FAE3` |
| `BuildPDefense` | `0x5B4CFD` |
| `BuildPower` | `0x5B4769` |
| `BuildRadar` | `0x5B4F7B` |
| `BuildRefinery` | `0x5B486B` |
| `BuildTech` | `0x5B4A61` |
| `BuildWeapons` | `0x5B4B53` |
| `Burn` | `0x5E1960` |
| `CONIN$` | `0x694F5A` |
| `CONOUT$` | `0x694F79` |
| `C_SHADOW.SHP` | `0x4EAA00` |
| `Carryall` | `0x40F300` |
| `Category` | `0x61FAE3` |
| `CellTags` | `0x47661D` |
| `ChainReaction` | `0x5771D1` |
| `ChargeAnim` | `0x43F45F` |
| `ChargingVoice` | `0x5F2FB4` |
| `Clear` | `0x476390` |
| `ClearTile` | `0x4EAA00` |
| `ClearToGreenLat` | `0x4EAA00` |
| `ClearToPaveLat` | `0x4EAA00` |
| `ClearToRoughLat` | `0x4EAA00` |
| `ClearToSandLat` | `0x4EAA00` |
| `CliffRamps` | `0x4EAA00` |
| `CliffSet` | `0x4EAA00` |
| `CloakGenerator` | `0x43F45F` |
| `CloakRadiusInCells` | `0x43F45F` |
| `CloakStop` | `0x61FAE3` |
| `Color` | `0x445BA0`, `0x4C74D0`, `0x628540` |
| `ColorList` | `0x597D00` |
| `ConcreteWalls` | `0x5B504F` |
| `ConstructionYard` | `0x43F45F` |
| `Conventional` | `0x6516EF` |
| `Cost: %d` | `0x483610` |
| `Crew` | `0x5B2305` |
| `CrushSound` | `0x572610` |
| `Crushable` | `0x572610` |
| `CustomRotor` | `0x40F300` |
| `D:\Projects\TSun\Code\DSAudio.Cpp` | `0x484AD0`, `0x484C90`, `0x484F10`, `0x484F90`, `0x4850E0`, `0x485D40`, `0x485E20`, `0x4861A0`, `0x486500`, `0x486B90` |
| `D:\Projects\TSun\Code\wwmous.cpp` | `0x672A60`, `0x672B66`, `0x672EC0`, `0x6731FF`, `0x6733A2`, `0x673A10`, `0x673AB0` |
| `DamageLevels` | `0x5771D1` |
| `DamageParticleSystems` | `0x620191` |
| `DamageRadius` | `0x418872`, `0x64213B` |
| `DamageSmokeOffset` | `0x62027B` |
| `Debris` | `0x628540` |
| `DebrisMaximums` | `0x61F97F` |
| `Deform` | `0x65187A` |
| `DeformThreshhold` | `0x65187A` |
| `DeployingAnim` | `0x43EBF6` |
| `DeploysInto` | `0x61FF2D` |
| `Description` | `0x4467D0`, `0x52D4F0`, `0x52D760`, `0x52DA30` |
| `DestroyableCliffs` | `0x4EAA00` |
| `Digest` | `0x447B40`, `0x5D1AC6` |
| `DirtRoadCurve` | `0x4EAA00` |
| `DirtRoadJunction` | `0x4EAA00` |
| `DirtRoadSlopes` | `0x4EAA00` |
| `DirtRoadStraight` | `0x4EAA00` |
| `DirtTrackTunnels` | `0x4EAA00` |
| `DirtTunnels` | `0x4EAA00` |
| `Disguise` | `0x5B220A` |
| `Dock` | `0x61FAE3` |
| `DockUnload` | `0x43F45F` |
| `DoorAnim` | `0x43EBF6` |
| `DrawFlat` | `0x5771D1` |
| `DropPodAngle` | `0x5B1622` |
| `DropPodHeight` | `0x5B1622` |
| `DropPodSpeed` | `0x5B1622` |
| `Duration` | `0x641E90` |
| `EMEffect` | `0x65187A` |
| `EMPulseCannon` | `0x43F45F` |
| `ERROR` | `0x658ABB` |
| `EWGates` | `0x5B504F` |
| `Elite` | `0x61FAA4` |
| `EliteAbilities` | `0x62027B` |
| `ExitCoord` | `0x43F45F` |
| `ExpireAnim` | `0x4186F4`, `0x6420BD` |
| `ExpireSound` | `0x418503`, `0x641F4B` |
| `Explosion` | `0x61FFF0` |
| `FACTORY` | `0x44A720` |
| `Factory` | `0x43F250` |
| `FillEarliestTeamProbability` | `0x5B2343` |
| `FinalMovie` | `0x4467D0` |
| `Fire` | `0x6516EF` |
| `FirestormWall` | `0x43F45F` |
| `Foundation` | `0x43FB5D`, `0x625280` |
| `GAPLUG` | `0x4BF674` |
| `GDIBarracks` | `0x43F45F` |
| `GDIFirestormGenerator` | `0x5B1EC1` |
| `GDIGateOne` | `0x5B1C8D` |
| `GDIGateTwo` | `0x5B1CCC` |
| `GDIHunterSeeker` | `0x5B1F00` |
| `GDIPowerPlant` | `0x5B1DC6` |
| `GDIPowerTurbine` | `0x5B1E05` |
| `Gate` | `0x43F45F` |
| `GateCloseDelay` | `0x43F45F` |
| `GetActiveWindow` | `0x691B96` |
| `GetLastActivePopup` | `0x691B96` |
| `Green` | `0x5C6E8E`, `0x5C7410` |
| `GreenTile` | `0x4EAA00` |
| `GreenTint` | `0x52E8B0` |
| `Ground` | `0x52E8B0`, `0x5C6E8E`, `0x5C7410` |
| `Growth` | `0x628540` |
| `GrowthPercentage` | `0x628540` |
| `HarvesterUnit` | `0x5B1FBC` |
| `HasRadialIndicator` | `0x572610` |
| `HasStupidGuardMode` | `0x43F45F` |
| `Height` | `0x43F45F`, `0x52D4F0`, `0x52D760`, `0x5E1960` |
| `HeightBase` | `0x4EAA00` |
| `Hospital` | `0x43F45F` |
| `House` | `0x60D180`, `0x60D510` |
| `HoverPad` | `0x43F45F` |
| `ICBMLauncher` | `0x43F45F` |
| `ISO%s.PAL` | `0x4EAA00` |
| `Ice1Set` | `0x4EAA00` |
| `Ice2Set` | `0x4EAA00` |
| `Ice3Set` | `0x4EAA00` |
| `IceShoreSet` | `0x4EAA00` |
| `IgnoresFirestorm` | `0x572610` |
| `Immune` | `0x572610` |
| `ImpatientVoice` | `0x5F2FB4` |
| `InfDeath` | `0x65187A` |
| `Insignificant` | `0x572610` |
| `Intro` | `0x5C68C0`, `0x5C7410` |
| `InvisibleInGame` | `0x43F45F` |
| `IonAmbient` | `0x5C6E8E`, `0x5C7410` |
| `IonBlue` | `0x5C6E8E`, `0x5C7410` |
| `IonGreen` | `0x5C6E8E`, `0x5C7410` |
| `IonGround` | `0x5C6E8E`, `0x5C7410` |
| `IonLevel` | `0x5C6E8E`, `0x5C7410` |
| `IonRed` | `0x5C6E8E`, `0x5C7410` |
| `IsARock` | `0x5771D1` |
| `IsAnimated` | `0x6253D8` |
| `IsAnimatedTiberium` | `0x418503` |
| `IsFlamingGuy` | `0x418503` |
| `IsFlammable` | `0x625280` |
| `IsMeteor` | `0x418462`, `0x641D80` |
| `IsPlug` | `0x43F45F` |
| `IsPowered` | `0x5F2FB4` |
| `IsTemple` | `0x43F45F` |
| `IsThreatRatingNode` | `0x43F45F` |
| `IsVeinhole` | `0x625280` |
| `IsVeinholeMonster` | `0x5771D1` |
| `IsVeins` | `0x418462`, `0x5771D1` |
| `Joe_Bostic` | `0x65BFC0` |
| `Land` | `0x577090` |
| `Landable` | `0x40F300` |
| `Language.dll` | `0x46EBD0` |
| `LaserColor` | `0x597090` |
| `LaserFence` | `0x43F45F` |
| `LaserFencePost` | `0x43F45F` |
| `LaserInnerColor` | `0x656F60` |
| `LaserOuterColor` | `0x656F60` |
| `LaserOuterSpread` | `0x656F60` |
| `LegalTarget` | `0x572610` |
| `LightBlueTint` | `0x43F45F` |
| `LightGreenTint` | `0x43F45F` |
| `LightIntensity` | `0x43F45F` |
| `LightRedTint` | `0x43F45F` |
| `LightVisibility` | `0x43F45F` |
| `LightningRod` | `0x61FFF0` |
| `Lob_18_` | `0x65FF00`, `0x6610F9`, `0x66A210`, `0x66BB30` |
| `Lob_18_%d` | `0x661070` |
| `LocalSize` | `0x47677D`, `0x52E8B0` |
| `LoopCount` | `0x4181EE` |
| `LoopEnd` | `0x4181EE` |
| `LoopStart` | `0x4181EE` |
| `Lose` | `0x5C68C0`, `0x5C7410` |
| `MMRampBase` | `0x4EAA00` |
| `ManualControl` | `0x5F30D6` |
| `ManualReload` | `0x61FFF0` |
| `MaxAngularVelocity` | `0x641E5F` |
| `MaxDC` | `0x597D00` |
| `MaxEC` | `0x597D00` |
| `MaxZVel` | `0x641E90` |
| `MaximumAIDefensiveTeams` | `0x5B2343` |
| `Medians` | `0x4EAA00` |
| `MessageBoxA` | `0x691B96` |
| `Microsoft Visual C++ Runtime Library` | `0x68CC4F` |
| `MinAngularVelocity` | `0x641D80` |
| `MinimumAIDefensiveTeams` | `0x5B2343` |
| `MiscPaveTile` | `0x4EAA00` |
| `MonorailSlopes` | `0x4EAA00` |
| `MovementRestrictedTo` | `0x63E8E0` |
| `MovementZone` | `0x620A13` |
| `MultiplayerAICM` | `0x5B2343` |
| `MyEffectivenessCoefficient` | `0x620894` |
| `NODBarracks` | `0x43F45F` |
| `NSGates` | `0x5B504F` |
| `Name: %s` | `0x483610` |
| `NaturalParticleLocation` | `0x620191` |
| `NaturalParticleSystem` | `0x620113` |
| `Next` | `0x4181EE` |
| `NextParticleOffset` | `0x597E0E` |
| `No Name` | `0x4EB9AD` |
| `No digest here mate. Nope.` | `0x5D1AC6` |
| `NoUseTileLandType` | `0x5771D1` |
| `NodAdvancedPower` | `0x5B1E82` |
| `NodGateOne` | `0x5B1D0A` |
| `NodGateTwo` | `0x5B1D49` |
| `NodHunterSeeker` | `0x5B1F3E` |
| `NodRegularPower` | `0x5B1E44` |
| `Not available` | `0x4A76E0` |
| `NukeSilo` | `0x43F45F` |
| `NumPlayers` | `0x52D4F0`, `0x52D760` |
| `OnAgeCheck` | `0x66D120` |
| `Owner` | `0x62027B` |
| `POWER` | `0x44A720` |
| `PROGBAR2.SHP` | `0x52DE10` |
| `PadAircraft` | `0x5B2081` |
| `Paratrooper` | `0x5B2189` |
| `Particle` | `0x651670` |
| `PaveTile` | `0x4EAA00` |
| `PavedRoadEnds` | `0x4EAA00` |
| `PavedRoadSlopes` | `0x4EAA00` |
| `PavedRoads` | `0x4EAA00` |
| `Pilot` | `0x5B22C6` |
| `PipScale` | `0x62027B` |
| `PlaceAnywhere` | `0x43F45F` |
| `Player` | `0x52E8B0`, `0x5C4353`, `0x5C6E8E`, `0x5C7410` |
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
| `Primary` | `0x61F97F` |
| `Projectile` | `0x6571BE` |
| `ProneDamage` | `0x65187A` |
| `RADAR` | `0x44A720` |
| `RadarEventDurations` | `0x5B3451` |
| `RadarEventSuppressionDistances` | `0x5B3451` |
| `RadarEventVisibilityDurations` | `0x5B3451` |
| `RadarInvisible` | `0x572610` |
| `RadialColor` | `0x572610` |
| `RampBase` | `0x4EAA00` |
| `RampSmooth` | `0x4EAA00` |
| `RandMap.Sed` | `0x52DA30`, `0x5D1A50`, `0x66B4E1` |
| `RandomRate` | `0x4188DF` |
| `RechargeTime` | `0x5F30D6` |
| `RechargeVoice` | `0x5F2FB4` |
| `Recoilless` | `0x43F45F` |
| `RedTint` | `0x52E8B0` |
| `Refinery` | `0x43F45F` |
| `RegionSize` | `0x52D4F0`, `0x52D760` |
| `Rocker` | `0x6516EF` |
| `Rocks` | `0x4EAA00` |
| `RollAngle` | `0x61F6D0` |
| `Rotates` | `0x445E48` |
| `Rotors` | `0x40F300` |
| `RoughGround` | `0x4EAA00` |
| `RoughTile` | `0x4EAA00` |
| `Ruggedness` | `0x52D4F0`, `0x52D760` |
| `RunningFrames` | `0x418503` |
| `SAVEGAME.NET` | `0x4F91C0` |
| `SCRN%04d.pcx` | `0x4E2770` |
| `SLOP01Z.%s` | `0x4EAA00` |
| `SPEECH%02d.MIX` | `0x4E0240` |
| `SYNC%01d.TXT` | `0x59DAE0` |
| `SandTile` | `0x4EAA00` |
| `Saw game completion due to player defeat` | `0x4B8FCB` |
| `Scenario` | `0x4467D0` |
| `ScrapVoxelDebris` | `0x5B1A64` |
| `Secondary` | `0x61FA65` |
| `Seed` | `0x52D4F0`, `0x52D760` |
| `Selectable` | `0x572610` |
| `SensorArray` | `0x43F45F` |
| `Sequence` | `0x4D3670` |
| `ShareBarrelData` | `0x641F4B` |
| `ShareBodyData` | `0x641F4B` |
| `ShareSource` | `0x642214` |
| `ShareTurretData` | `0x641F4B` |
| `ShorePieces` | `0x4EAA00` |
| `ShouldFogRemove` | `0x418503` |
| `ShouldUseCellDrawer` | `0x4188DF` |
| `Side` | `0x4C74D0` |
| `SidebarImage` | `0x5F3140` |
| `SiloDamage` | `0x43F45F` |
| `Size` | `0x476390`, `0x52E8B0` |
| `SlopeSetPieces` | `0x4EAA00` |
| `SlopeSetPieces2` | `0x4EAA00` |
| `SnowOccupationBits` | `0x6253D8` |
| `Sparky` | `0x6516EF` |
| `SpawnCount` | `0x418462`, `0x641F4B` |
| `SpawnDirection` | `0x597090` |
| `SpawnsTiberium` | `0x625280` |
| `SpecialZOverlay` | `0x43EBF6` |
| `SpeedType` | `0x63E8E0` |
| `Spread` | `0x628540`, `0x651670` |
| `SpreadPercentage` | `0x628540` |
| `Start` | `0x4181EE` |
| `StartColor1` | `0x597E0E` |
| `StartColor2` | `0x597E0E` |
| `StartSound` | `0x418503`, `0x641F4B` |
| `Storage` | `0x61FAE3` |
| `SuperWeapon` | `0x43F45F` |
| `SuperWeapon2` | `0x43F45F` |
| `SuspendVoice` | `0x5F2FB4` |
| `TECH` | `0x44A720` |
| `TILE` | `0x4EB9AD` |
| `TREE%d%d` | `0x5387C0` |
| `TS%d` | `0x647A70`, `0x647AB9` |
| `TargetEffectivenessCoefficient` | `0x6208F4` |
| `TargetSpecialThreatCoefficient` | `0x620954` |
| `TargetStrengthCoefficient` | `0x6209B3` |
| `TeamAddSelect_%d` | `0x4E0C20` |
| `TeamCenter_%d` | `0x4E0D90` |
| `TeamCreate_%d` | `0x4E0900` |
| `TeamDelays` | `0x5B2343` |
| `TeamSelect_%d` | `0x4E0A70` |
| `Technician` | `0x5B2287` |
| `TemperateOccupationBits` | `0x6253D8` |
| `Theme` | `0x5C68C0`, `0x5C7410` |
| `TiberiumLayout` | `0x52D4F0`, `0x52D760` |
| `TiberiumSpawnType` | `0x418462` |
| `TiberiumSpreadRadius` | `0x418462` |
| `TickTank` | `0x43F45F` |
| `Tile%02dAnim` | `0x4EBD12` |
| `Tile%02dAttachesTo` | `0x4EBD12` |
| `Tile%02dXOffset` | `0x4EBD12` |
| `Tile%02dYOffset` | `0x4EBD12` |
| `Tile%02dZAdjust` | `0x4EBD12` |
| `TileSet%04d` | `0x4EAA00`, `0x4EB558` |
| `Tiled` | `0x4188DF` |
| `Time` | `0x52D4F0`, `0x52D760` |
| `TireVoxelDebris` | `0x5B19C4` |
| `ToOverlay` | `0x43F45F` |
| `TogglePower` | `0x43F45F` |
| `TotalAITeamCap` | `0x5B2343` |
| `TrackTunnels` | `0x4EAA00` |
| `TrailerAnim` | `0x4187B4`, `0x6420FC` |
| `TrailerSeperation` | `0x418872` |
| `TrainBridgeSet` | `0x4EAA00` |
| `Tunnels` | `0x4EAA00` |
| `Turret` | `0x61FFF0` |
| `TurretSpins` | `0x61FFF0` |
| `Type` | `0x5F2FB4` |
| `UndeploysInto` | `0x61FFB1` |
| `UnderDoorAnim` | `0x43EBF6` |
| `UnitReload` | `0x43F45F` |
| `UnitRepair` | `0x43F45F` |
| `UrbanPresence` | `0x52D4F0`, `0x52D760` |
| `UseChargeDrain` | `0x5F30D6` |
| `UseNormalLight` | `0x4188DF` |
| `VEINHOLE.%s` | `0x644080` |
| `Value` | `0x628540` |
| `VariableNames` | `0x5C66B0`, `0x5C6720` |
| `Vegetation` | `0x52D4F0`, `0x52D760` |
| `Veinhole` | `0x65187A` |
| `VeinholeTypeClass` | `0x5B3451` |
| `Verses` | `0x65187A` |
| `VeteranAbilities` | `0x62027B` |
| `VoiceAttack` | `0x61FAE3` |
| `VoiceDie` | `0x61FAE3` |
| `VoiceFeedback` | `0x61FAE3` |
| `VoiceMove` | `0x61FAE3` |
| `VoiceSelect` | `0x61FAE3` |
| `Voxel` | `0x572610` |
| `VoxelIndex` | `0x641F4B` |
| `WallTower` | `0x5B1D88` |
| `Water` | `0x476390` |
| `WaterAmount` | `0x52D4F0`, `0x52D760` |
| `WaterCaves` | `0x4EAA00` |
| `WaterCliffs` | `0x4EAA00` |
| `WaterSet` | `0x4EAA00` |
| `WaterfallEast` | `0x4EAA00` |
| `WaterfallNorth` | `0x4EAA00` |
| `WaterfallSouth` | `0x4EAA00` |
| `WaterfallWest` | `0x4EAA00` |
| `Waypoints` | `0x5C7FE0` |
| `WeaponType` | `0x5F2F60` |
| `WeaponsFactory` | `0x43F45F` |
| `Width` | `0x52D4F0`, `0x52D760`, `0x5E1960` |
| `Wood` | `0x6516EF` |
| `XXICON` | `0x6213B0` |
| `YDrawOffset` | `0x418503` |
| `[%s]` | `0x65F132` |
| `[%s] : %s` | `0x66B920` |
| `\StringFileInfo\%04X%04X\ProductVersion` | `0x6466E0` |
| `\VarFileInfo\Translation` | `0x6466E0` |
| `\\.\vwin32` | `0x44D710`, `0x44D830` |
| `arrow_dd.pcx` | `0x5859E0` |
| `arrow_du.pcx` | `0x5859E0` |
| `arrow_ud.pcx` | `0x5859E0` |
| `arrow_uu.pcx` | `0x5859E0` |
| `b%ce_li%d.pcx` | `0x5A90A0` |
| `b%ce_mi%d.pcx` | `0x5A90A0` |
| `b%ce_ri%d.pcx` | `0x5A90A0` |
| `bar_ll.pcx` | `0x5859E0` |
| `bar_lr.pcx` | `0x5859E0` |
| `bar_ul.pcx` | `0x5859E0` |
| `bar_ur.pcx` | `0x5859E0` |
| `bde_li24.pcx` | `0x5859E0` |
| `bde_li30.pcx` | `0x5859E0` |
| `bde_mi24.pcx` | `0x5859E0` |
| `bde_mi30.pcx` | `0x5859E0` |
| `bde_ri24.pcx` | `0x5859E0` |
| `bde_ri30.pcx` | `0x5859E0` |
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
| `ccd_i.pcx` | `0x5859E0` |
| `cce_i.pcx` | `0x5859E0` |
| `cud_i.pcx` | `0x5859E0` |
| `cue_i.pcx` | `0x5859E0` |
| `dWindows` | `0x640867` |
| `dbak6440.pcx` | `0x5859E0` |
| `dlgsys` | `0x5A92F0` |
| `dlgsysa.pcx` | `0x5859E0` |
| `dlgsysi.pcx` | `0x5859E0` |
| `dnarrowp.pcx` | `0x5859E0` |
| `dnarrowr.pcx` | `0x5859E0` |
| `e+000` | `0x685989` |
| `gdii.pcx` | `0x5859E0` |
| `leftbar.pcx` | `0x5859E0` |
| `nodi.pcx` | `0x5859E0` |
| `none` | `0x40F4A0`, `0x418503`, `0x4186F4`, `0x4187B4`, `0x418D20`, `0x419130`, `0x441870`, `0x446280`, `0x4D3600`, `0x4D4020`, `0x577540`, `0x597640`, `0x5CEA40`, `0x6045F0`, `0x606A70`, `0x625650`, `0x62E1B0`, `0x62E3D0`, `0x63EDE0`, `0x6426D0`, `0x642880`, `0x651D30`, `0x6576F0` |
| `rPos` | `0x640867` |
| `rightbar.pcx` | `0x5859E0` |
| `sb_psh_d.pcx` | `0x5859E0` |
| `sb_psh_u.pcx` | `0x5859E0` |
| `sb_rel_d.pcx` | `0x5859E0` |
| `sb_rel_u.pcx` | `0x5859E0` |
| `sbgripb.pcx` | `0x5859E0` |
| `sbgripm.pcx` | `0x5859E0` |
| `sbgript.pcx` | `0x5859E0` |
| `tab_fbl.pcx` | `0x5859E0` |
| `tab_fbm.pcx` | `0x5859E0` |
| `tab_fbr.pcx` | `0x5859E0` |
| `tab_fml.pcx` | `0x5859E0` |
| `tab_fmr.pcx` | `0x5859E0` |
| `tab_ftl.pcx` | `0x5859E0` |
| `tab_ftm.pcx` | `0x5859E0` |
| `tab_ftr.pcx` | `0x5859E0` |
| `tab_tld.pcx` | `0x5859E0` |
| `tab_tlu.pcx` | `0x5859E0` |
| `tab_tmd.pcx` | `0x5859E0` |
| `tab_tmu.pcx` | `0x5859E0` |
| `tab_trd.pcx` | `0x5859E0` |
| `tab_tru.pcx` | `0x5859E0` |
| `trakgrip.pcx` | `0x5859E0` |
| `trofl.pcx` | `0x5859E0` |
| `trofm.pcx` | `0x5859E0` |
| `trofr.pcx` | `0x5859E0` |
| `uparrowp.pcx` | `0x5859E0` |
| `uparrowr.pcx` | `0x5859E0` |
| `user32.dll` | `0x691B96` |
| `wodact.pcx` | `0x5859E0` |
| `wodban.pcx` | `0x5859E0` |
| `wodclan.pcx` | `0x5859E0` |
| `woddcon.pcx` | `0x5859E0` |
| `wodfgame.pcx` | `0x5859E0` |
| `wodfind.pcx` | `0x5859E0` |
| `wodhelp.pcx` | `0x5859E0` |
| `wodkick.pcx` | `0x5859E0` |
| `wodleave.pcx` | `0x5859E0` |
| `wodopt.pcx` | `0x5859E0` |
| `wodref.pcx` | `0x5859E0` |
| `wodsqlch.pcx` | `0x5859E0` |
| `wodtrny.pcx` | `0x5859E0` |
| `wolacpt.pcx` | `0x5859E0` |
| `wolclan.pcx` | `0x5859E0` |
| `wolhost.pcx` | `0x5859E0` |
| `woloper.pcx` | `0x5859E0`, `0x65F265` |
| `wolpriv.pcx` | `0x5859E0` |
| `wolsqlch.pcx` | `0x5859E0`, `0x65F31D` |
| `woltrny.pcx` | `0x5859E0` |
| `woluser.pcx` | `0x5859E0` |
| `wolvoice.pcx` | `0x5859E0` |
| `wouact.pcx` | `0x5859E0` |
| `wouban.pcx` | `0x5859E0` |
| `wouclan.pcx` | `0x5859E0` |
| `woudcon.pcx` | `0x5859E0` |
| `woufgame.pcx` | `0x5859E0` |
| `woufind.pcx` | `0x5859E0` |
| `wouhelp.pcx` | `0x5859E0` |
| `woukick.pcx` | `0x5859E0` |
| `wouleave.pcx` | `0x5859E0` |
| `wouopt.pcx` | `0x5859E0` |
| `wouref.pcx` | `0x5859E0` |
| `wousqlch.pcx` | `0x5859E0` |
| `woutrny.pcx` | `0x5859E0` |
