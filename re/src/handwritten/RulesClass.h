// RulesClass.h — reconstructed from GAME.EXE (C&C Tiberian Sun, Westwood 1999)
//
// Evidence:
//   RTTI:  no .?AVRulesClass@@ record survives in out/classes.json (the
//          class is not polymorphic / has no vtable), so the class is
//          identified purely from its rules.ini reader functions.
//   Reader: 0x005B2343 — the top-level rules.ini loader.  It reads the
//          [General]/[AI]/[Power]/[Land]/[Audio]/[Visual]/[Building]/
//          [Infantry]/[Unit]/[Aircraft] sections and dispatches to the
//          per-section readers below.
//   Sub-readers (each is a separate function called from 0x005B2343 or
//   from the section dispatcher):
//     0x005B3451  General      (49 keys)
//     0x005B504F  AI           (38 keys)
//     0x005B11CD  Power        (12 keys)
//     0x005B16C6  Land         (16 keys)
//     0x005C7410  Audio        (43 keys)
//     0x004EAA00  Visual       (62 keys)
//     0x0043FB5D  Building     (99 keys)
//     0x0062027B  Infantry     (36 keys)
//     0x00620A13  Unit         (17 keys)
//     0x0061F701  Aircraft     (11 keys)
//   The per-section readers are the same functions that also serve as the
//   TechnoTypeClass/BuildingTypeClass readers (e.g. 0x0062027B is the
//   InfantryTypeClass reader and 0x00620A13 the UnitTypeClass reader);
//   RulesClass calls them with the global rules INI.
//
// RulesClass is a plain aggregate (no vtable).  Its members are the
// scalar/string rules values; the per-type tables live in the
// TypeList<T*> members of the global RulesClass instance.

#ifndef RULESCLASS_H
#define RULESCLASS_H

#include "CCINIClass.h"

class RulesClass
{
public:
    RulesClass();
    ~RulesClass();

    // ---- top-level loader (0x005B2343) ---------------------------------
    // Reads the whole rules.ini.  Returns true if the file was loaded.
    bool Read_INI(CCINIClass& ini);

    // ---- per-section readers -------------------------------------------
    // Each returns true on success and fills the corresponding member
    // group.  VAs are the reader functions; the key lists are in SCHEMA.md.
    bool ReadGeneral(CCINIClass& ini);    // 0x005B3451
    bool ReadAI(CCINIClass& ini);         // 0x005B504F
    bool ReadPower(CCINIClass& ini);      // 0x005B11CD
    bool ReadLand(CCINIClass& ini);       // 0x005B16C6
    bool ReadAudio(CCINIClass& ini);      // 0x005C7410
    bool ReadVisual(CCINIClass& ini);     // 0x004EAA00
    bool ReadBuilding(CCINIClass& ini);   // 0x0043FB5D
    bool ReadInfantry(CCINIClass& ini);   // 0x0062027B
    bool ReadUnit(CCINIClass& ini);       // 0x00620A13
    bool ReadAircraft(CCINIClass& ini);   // 0x0061F701

    // ---- [General] section (0x005B3451) --------------------------------
    // /* ? */ member offsets not isolated; names are the INI keys.
    double SpotlightMovementRadius;       // INI "SpotlightMovementRadius"
    double SpotlightLocationRadius;       // INI "SpotlightLocationRadius"
    double SpotlightSpeed;                // INI "SpotlightSpeed"
    double SpotlightAcceleration;         // INI "SpotlightAcceleration"
    double SpotlightAngle;                // INI "SpotlightAngle"
    double SpotlightRadius;               // INI "SpotlightRadius"
    double RevealTriggerRadius;           // INI "RevealTriggerRadius"
    double ChargeToDrainRatio;            // INI "ChargeToDrainRatio"
    double DamageToFirestormDamageCoefficient; // INI "DamageToFirestormDamageCoefficient"
    double WallBuildSpeedCoefficient;     // INI "WallBuildSpeedCoefficient"
    double WorstLowPowerBuildRateCoefficient;  // INI "WorstLowPowerBuildRateCoefficient"
    double BestLowPowerBuildRateCoefficient;   // INI "BestLowPowerBuildRateCoefficient"
    double ConditionYellowSparkingProbability; // INI "ConditionYellowSparkingProbability"
    double ConditionRedSparkingProbability;    // INI "ConditionRedSparkingProbability"
    double AITriggerSuccessWeightDelta;   // INI "AITriggerSuccessWeightDelta"
    double AITriggerFailureWeightDelta;   // INI "AITriggerFailureWeightDelta"
    double AITriggerTrackRecordCoefficient; // INI "AITriggerTrackRecordCoefficient"
    int    WeedCapacity;                  // INI "WeedCapacity"
    int    FlashFrameTime;                // INI "FlashFrameTime"
    int    RadarCombatFlashTime;          // INI "RadarCombatFlashTime"
    int    RadarEventSpeed;               // INI "RadarEventSpeed"
    int    RadarEventRotationSpeed;       // INI "RadarEventRotationSpeed"
    int    RadarEventMinRadius;           // INI "RadarEventMinRadius"
    int    RadarEventColorSpeed;          // INI "RadarEventColorSpeed"
    int    HunterSeekerDetonateProximity; // INI "HunterSeekerDetonateProximity"
    int    HunterSeekerDescendProximity;  // INI "HunterSeekerDescendProximity"
    int    HunterSeekerDescentSpeed;      // INI "HunterSeekerDescentSpeed"
    int    HunterSeekerAscentSpeed;       // INI "HunterSeekerAscentSpeed"
    int    HunterSeekerEmergeSpeed;       // INI "HunterSeekerEmergeSpeed"
    double MyEffectivenessCoefficientDefault;      // INI "MyEffectivenessCoefficientDefault"
    double TargetEffectivenessCoefficientDefault;  // INI "TargetEffectivenessCoefficientDefault"
    double TargetSpecialThreatCoefficientDefault;  // INI "TargetSpecialThreatCoefficientDefault"
    double TargetStrengthCoefficientDefault;       // INI "TargetStrengthCoefficientDefault"
    double TargetDistanceCoefficientDefault;       // INI "TargetDistanceCoefficientDefault"
    double DumbMyEffectivenessCoefficient;         // INI "DumbMyEffectivenessCoefficient"
    double DumbTargetEffectivenessCoefficient;     // INI "DumbTargetEffectivenessCoefficient"
    double DumbTargetSpecialThreatCoefficient;     // INI "DumbTargetSpecialThreatCoefficient"
    double DumbTargetStrengthCoefficient;          // INI "DumbTargetStrengthCoefficient"
    double DumbTargetDistanceCoefficient;          // INI "DumbTargetDistanceCoefficient"
    double EnemyHouseThreatBonus;         // INI "EnemyHouseThreatBonus"
    int    VeinholeMonsterStrength;       // INI "VeinholeMonsterStrength"
    int    MaxVeinholeGrowth;             // INI "MaxVeinholeGrowth"
    int    VeinholeGrowthRate;            // INI "VeinholeGrowthRate"
    int    VeinholeShrinkRate;            // INI "VeinholeShrinkRate"
    int    VeinDamage;                    // INI "VeinDamage"
    char   VeinholeTypeClass[64];         // INI "VeinholeTypeClass"
    int    MaximumQueuedObjects;          // INI "MaximumQueuedObjects"
    int    MaxWaypointPathLength;         // INI "MaxWaypointPathLength"
    int    TreeStrength;                  // INI "TreeStrength"

    // ---- [AI] section (0x005B504F) -------------------------------------
    int    AttackInterval;                // INI "AttackInterval"
    int    AttackDelay;                   // INI "AttackDelay"
    int    PatrolScan;                    // INI "PatrolScan"
    int    CreditReserve;                 // INI "CreditReserve"
    int    PathDelay;                     // INI "PathDelay"
    int    BlockagePathDelay;             // INI "BlockagePathDelay"
    int    TiberiumFarScan;               // INI "TiberiumFarScan"
    int    AutocreateTime;                // INI "AutocreateTime"
    int    InfantryReserve;               // INI "InfantryReserve"
    int    InfantryBaseMult;              // INI "InfantryBaseMult"
    int    PowerSurplus;                  // INI "PowerSurplus"
    int    BaseSizeAdd;                   // INI "BaseSizeAdd"
    int    RefineryRatio;                 // INI "RefineryRatio"
    int    RefineryLimit;                 // INI "RefineryLimit"
    int    BarracksRatio;                 // INI "BarracksRatio"
    int    BarracksLimit;                 // INI "BarracksLimit"
    int    WarRatio;                      // INI "WarRatio"
    int    WarLimit;                      // INI "WarLimit"
    int    DefenseRatio;                  // INI "DefenseRatio"
    int    DefenseLimit;                  // INI "DefenseLimit"
    int    AARatio;                       // INI "AARatio"
    int    AALimit;                       // INI "AALimit"
    int    TeslaRatio;                    // INI "TeslaRatio"
    int    TeslaLimit;                    // INI "TeslaLimit"
    int    HelipadRatio;                  // INI "HelipadRatio"
    int    HelipadLimit;                  // INI "HelipadLimit"
    int    AirstripRatio;                 // INI "AirstripRatio"
    int    AirstripLimit;                 // INI "AirstripLimit"
    int    CompEasyBonus;                 // INI "CompEasyBonus"
    bool   Paranoid;                      // INI "Paranoid"
    bool   PowerEmergency;                // INI "PowerEmergency"
    int    AIBaseSpacing;                 // INI "AIBaseSpacing"
    bool   GDIWallDefense;                // INI "GDIWallDefense"
    double GDIWallDefenseCoefficient;     // INI "GDIWallDefenseCoefficient"
    double NodBaseDefenseCoefficient;     // INI "NodBaseDefenseCoefficient"
    double GDIBaseDefenseCoefficient;     // INI "GDIBaseDefenseCoefficient"
    int    MaximumBaseDefenseValue;       // INI "MaximumBaseDefenseValue"
    int    ComputerBaseDefenseResponse;   // INI "ComputerBaseDefenseResponse"

    // ---- [Power] section (0x005B11CD) ----------------------------------
    bool   TiberiumHeal;                  // INI "TiberiumHeal"
    double ZoomInFactor;                  // INI "ZoomInFactor"
    bool   RevealByHeight;                // INI "RevealByHeight"
    bool   AllowShroudedSubteranneanMoves;// INI "AllowShroudedSubteranneanMoves"
    bool   AircraftFogReveal;             // INI "AircraftFogReveal"
    int    MinProductionSpeed;            // INI "MinProductionSpeed"
    int    MultipleFactory;               // INI "MultipleFactory"
    int    CraterLevel;                   // INI "CraterLevel"
    double TreeFlammability;              // INI "TreeFlammability"
    double MissileROTVar;                 // INI "MissileROTVar"
    double MissileSpeedVar;               // INI "MissileSpeedVar"
    char   DropPodWeapon[64];             // INI "DropPodWeapon"

    // ---- [Land] section (0x005B16C6) -----------------------------------
    bool   CrewEscape;                    // INI "CrewEscape"
    double TunnelSpeed;                   // INI "TunnelSpeed"
    double HoverDampen;                   // INI "HoverDampen"
    double HoverBob;                      // INI "HoverBob"
    double HoverHeight;                   // INI "HoverHeight"
    double HoverBoost;                    // INI "HoverBoost"
    double HoverAcceleration;             // INI "HoverAcceleration"
    double HoverBrake;                    // INI "HoverBrake"
    double VeteranRatio;                  // INI "VeteranRatio"
    double VeteranCombat;                 // INI "VeteranCombat"
    double VeteranSpeed;                  // INI "VeteranSpeed"
    double VeteranSight;                  // INI "VeteranSight"
    double VeteranArmor;                  // INI "VeteranArmor"
    double VeteranROF;                    // INI "VeteranROF"
    int    VeteranCap;                    // INI "VeteranCap"
    bool   ExplosiveVoxelDebris;          // INI "ExplosiveVoxelDebris"

    // ---- [Audio] section (0x005C7410) ----------------------------------
    // /* ? */ the reader also handles the [Basic] scenario keys
    // (CivEvac/Briefing/NextScenario/...) and the [Lighting] keys.
    char   CivEvac[64];                   // INI "CivEvac"
    char   Briefing[64];                  // INI "Briefing"
    char   NextScenario[64];              // INI "NextScenario"
    char   AltNextScenario[64];           // INI "AltNextScenario"
    char   Name[64];                      // INI "Name"
    int    NewINIFormat;                  // INI "NewINIFormat"
    int    CarryOverCap;                  // INI "CarryOverCap"
    bool   EndOfGame;                     // INI "EndOfGame"
    bool   SkipScore;                     // INI "SkipScore"
    bool   OneTimeOnly;                   // INI "OneTimeOnly"
    bool   SkipMapSelect;                 // INI "SkipMapSelect"
    bool   Official;                      // INI "Official"
    bool   IgnoreGlobalAITriggers;        // INI "IgnoreGlobalAITriggers"
    bool   TruckCrate;                    // INI "TruckCrate"
    bool   TrainCrate;                    // INI "TrainCrate"
    int    Percent;                       // INI "Percent"
    char   Theme[64];                     // INI "Theme"
    int    CarryOverMoney;                // INI "CarryOverMoney"
    bool   TimerInherit;                  // INI "TimerInherit"
    bool   FillSilos;                     // INI "FillSilos"
    int    StartingDropships;             // INI "StartingDropships"
    int    HomeCell;                      // INI "HomeCell"
    int    AltHomeCell;                   // INI "AltHomeCell"
    bool   MultiplayerOnly;               // INI "MultiplayerOnly"
    bool   TiberiumGrowthEnabled;         // INI "TiberiumGrowthEnabled"
    bool   VeinGrowthEnabled;             // INI "VeinGrowthEnabled"
    bool   IceGrowthEnabled;              // INI "IceGrowthEnabled"
    bool   TiberiumDeathToVisceroid;      // INI "TiberiumDeathToVisceroid"
    bool   FreeRadar;                     // INI "FreeRadar"
    int    InitTime;                      // INI "InitTime"
    // [Lighting] sub-keys read by the same function
    int    Lighting;                      // INI "Lighting"
    int    Ambient;                       // INI "Ambient"
    int    Green;                         // INI "Green"
    int    Blue;                          // INI "Blue"
    int    Ground;                        // INI "Ground"
    int    Level;                         // INI "Level"
    int    IonAmbient;                    // INI "IonAmbient"
    int    IonRed;                        // INI "IonRed"
    int    IonGreen;                      // INI "IonGreen"
    int    IonBlue;                       // INI "IonBlue"
    int    IonGround;                     // INI "IonGround"
    int    IonLevel;                      // INI "IonLevel"

    // ---- [Visual] section (0x004EAA00) ---------------------------------
    // Tile-set / bridge / cliff piece name tables.  Each is a
    // DynamicVectorClass<char*> or a fixed name list; /* ? */ exact
    // container types not proven.
    char   RampBase[64];                  // INI "RampBase"
    char   RampSmooth[64];                // INI "RampSmooth"
    char   MMRampBase[64];                // INI "MMRampBase"
    char   ClearTile[64];                 // INI "ClearTile"
    char   RoughTile[64];                 // INI "RoughTile"
    char   SandTile[64];                  // INI "SandTile"
    char   GreenTile[64];                 // INI "GreenTile"
    char   PaveTile[64];                  // INI "PaveTile"
    char   MiscPaveTile[64];              // INI "MiscPaveTile"
    char   ClearToRoughLat[64];           // INI "ClearToRoughLat"
    char   ClearToSandLat[64];            // INI "ClearToSandLat"
    char   ClearToGreenLat[64];           // INI "ClearToGreenLat"
    char   ClearToPaveLat[64];            // INI "ClearToPaveLat"
    char   HeightBase[64];                // INI "HeightBase"
    char   BlackTile[64];                 // INI "BlackTile"
    char   BridgeSet[64];                 // INI "BridgeSet"
    char   TrainBridgeSet[64];            // INI "TrainBridgeSet"
    char   CliffSet[64];                  // INI "CliffSet"
    char   ShorePieces[64];               // INI "ShorePieces"
    char   WaterSet[64];                  // INI "WaterSet"
    char   Ice1Set[64];                   // INI "Ice1Set"
    char   Ice2Set[64];                   // INI "Ice2Set"
    char   Ice3Set[64];                   // INI "Ice3Set"
    char   IceShoreSet[64];               // INI "IceShoreSet"
    char   SlopeSetPieces[64];            // INI "SlopeSetPieces"
    char   SlopeSetPieces2[64];           // INI "SlopeSetPieces2"
    char   MonorailSlopes[64];            // INI "MonorailSlopes"
    char   Tunnels[64];                   // INI "Tunnels"
    char   TrackTunnels[64];              // INI "TrackTunnels"
    char   DirtTunnels[64];               // INI "DirtTunnels"
    char   DirtTrackTunnels[64];          // INI "DirtTrackTunnels"
    char   WaterfallEast[64];             // INI "WaterfallEast"
    char   WaterfallWest[64];             // INI "WaterfallWest"
    char   WaterfallNorth[64];            // INI "WaterfallNorth"
    char   WaterfallSouth[64];            // INI "WaterfallSouth"
    char   CliffRamps[64];                // INI "CliffRamps"
    char   PavedRoads[64];                // INI "PavedRoads"
    char   PavedRoadEnds[64];             // INI "PavedRoadEnds"
    char   Medians[64];                   // INI "Medians"
    char   RoughGround[64];               // INI "RoughGround"
    char   DirtRoadJunction[64];          // INI "DirtRoadJunction"
    char   DirtRoadCurve[64];             // INI "DirtRoadCurve"
    char   DirtRoadStraight[64];          // INI "DirtRoadStraight"
    char   DestroyableCliffs[64];         // INI "DestroyableCliffs"
    char   WaterCaves[64];                // INI "WaterCaves"
    char   WaterCliffs[64];               // INI "WaterCliffs"
    char   PavedRoadSlopes[64];           // INI "PavedRoadSlopes"
    char   DirtRoadSlopes[64];            // INI "DirtRoadSlopes"
    char   Rocks[64];                     // INI "Rocks"
    char   BridgeTopLeft2[64];            // INI "BridgeTopLeft2"
    char   BridgeBottomRight1[64];        // INI "BridgeBottomRight1"
    char   BridgeBottomRight2[64];        // INI "BridgeBottomRight2"
    char   BridgeTopRight1[64];           // INI "BridgeTopRight1"
    char   BridgeTopRight2[64];           // INI "BridgeTopRight2"
    char   BridgeBottomLeft1[64];         // INI "BridgeBottomLeft1"
    char   BridgeBottomLeft2[64];         // INI "BridgeBottomLeft2"
    char   BridgeMiddle1[64];             // INI "BridgeMiddle1"
    char   BridgeMiddle2[64];             // INI "BridgeMiddle2"
    int    TilesInSet;                    // INI "TilesInSet"
    int    LastTilesInSet;                // INI "LastTilesInSet"

    // ---- [Building] section (0x0043FB5D) -------------------------------
    // /* ? */ the reader also drives the BuildingTypeClass per-type
    // ActiveAnim/SpecialAnim/ProductionAnim/TurretAnim tables.
    int    Foundation;                    // INI "Foundation"
    int    MidPoint;                      // INI "MidPoint"
    int    DoorStages;                    // INI "DoorStages"
    int    DamagedDoor;                   // INI "DamagedDoor"
    char   TerrainPalette[64];            // INI "TerrainPalette"
    int    GateStages;                    // INI "GateStages"
    int    PrimaryFirePixelOffset;        // INI "PrimaryFirePixelOffset"
    int    SecondaryFirePixelOffset;      // INI "SecondaryFirePixelOffset"
    int    ExtraDamageStage;              // INI "ExtraDamageStage"
    int    SpecialZOverlayZAdjust;        // INI "SpecialZOverlayZAdjust"
    int    NormalZAdjust;                 // INI "NormalZAdjust"
    int    ZShapePointMove;               // INI "ZShapePointMove"
    int    ExtraLight;                    // INI "ExtraLight"
    bool   DemandLoad;                    // INI "DemandLoad"
    bool   DemandLoadBuildup;             // INI "DemandLoadBuildup"
    bool   FreeBuildup;                   // INI "FreeBuildup"
    bool   Buildup;                       // INI "Buildup"
    char   AnimIdle[64];                  // INI "AnimIdle"
    char   AnimActive[64];                // INI "AnimActive"
    char   AnimAux1[64];                  // INI "AnimAux1"
    char   AnimAux2[64];                  // INI "AnimAux2"
    char   ActiveAnim[64];                // INI "ActiveAnim"
    char   ActiveAnimDamaged[64];         // INI "ActiveAnimDamaged"
    int    ActiveAnimX;                   // INI "ActiveAnimX"
    int    ActiveAnimY;                   // INI "ActiveAnimY"
    int    ActiveAnimZAdjust;             // INI "ActiveAnimZAdjust"
    int    ActiveAnimYSort;               // INI "ActiveAnimYSort"
    bool   ActiveAnimPowered;             // INI "ActiveAnimPowered"
    bool   ActiveAnimPoweredLight;        // INI "ActiveAnimPoweredLight"
    char   ActiveAnimTwo[64];             // INI "ActiveAnimTwo"
    char   ActiveAnimTwoDamaged[64];      // INI "ActiveAnimTwoDamaged"
    int    ActiveAnimTwoX;                // INI "ActiveAnimTwoX"
    int    ActiveAnimTwoY;                // INI "ActiveAnimTwoY"
    int    ActiveAnimTwoZAdjust;          // INI "ActiveAnimTwoZAdjust"
    int    ActiveAnimTwoYSort;            // INI "ActiveAnimTwoYSort"
    bool   ActiveAnimTwoPowered;          // INI "ActiveAnimTwoPowered"
    bool   ActiveAnimTwoPoweredLight;     // INI "ActiveAnimTwoPoweredLight"
    char   ActiveAnimThree[64];           // INI "ActiveAnimThree"
    char   ActiveAnimThreeDamaged[64];    // INI "ActiveAnimThreeDamaged"
    int    ActiveAnimThreeX;              // INI "ActiveAnimThreeX"
    int    ActiveAnimThreeY;              // INI "ActiveAnimThreeY"
    int    ActiveAnimThreeZAdjust;        // INI "ActiveAnimThreeZAdjust"
    int    ActiveAnimThreeYSort;          // INI "ActiveAnimThreeYSort"
    bool   ActiveAnimThreePowered;        // INI "ActiveAnimThreePowered"
    bool   ActiveAnimThreePoweredLight;   // INI "ActiveAnimThreePoweredLight"
    char   ActiveAnimFour[64];            // INI "ActiveAnimFour"
    char   ActiveAnimFourDamaged[64];     // INI "ActiveAnimFourDamaged"
    int    ActiveAnimFourX;               // INI "ActiveAnimFourX"
    int    ActiveAnimFourY;               // INI "ActiveAnimFourY"
    int    ActiveAnimFourZAdjust;         // INI "ActiveAnimFourZAdjust"
    int    ActiveAnimFourYSort;           // INI "ActiveAnimFourYSort"
    bool   ActiveAnimFourPowered;         // INI "ActiveAnimFourPowered"
    bool   ActiveAnimFourPoweredLight;    // INI "ActiveAnimFourPoweredLight"
    char   SpecialAnim[64];               // INI "SpecialAnim"
    char   SpecialAnimDamaged[64];        // INI "SpecialAnimDamaged"
    int    SpecialAnimX;                  // INI "SpecialAnimX"
    int    SpecialAnimY;                  // INI "SpecialAnimY"
    int    SpecialAnimZAdjust;            // INI "SpecialAnimZAdjust"
    int    SpecialAnimYSort;              // INI "SpecialAnimYSort"
    bool   SpecialAnimPowered;            // INI "SpecialAnimPowered"
    bool   SpecialAnimPoweredLight;       // INI "SpecialAnimPoweredLight"
    char   SpecialAnimTwo[64];            // INI "SpecialAnimTwo"
    char   SpecialAnimTwoDamaged[64];     // INI "SpecialAnimTwoDamaged"
    int    SpecialAnimTwoX;               // INI "SpecialAnimTwoX"
    int    SpecialAnimTwoY;               // INI "SpecialAnimTwoY"
    int    SpecialAnimTwoZAdjust;         // INI "SpecialAnimTwoZAdjust"
    int    SpecialAnimTwoYSort;           // INI "SpecialAnimTwoYSort"
    bool   SpecialAnimTwoPowered;         // INI "SpecialAnimTwoPowered"
    bool   SpecialAnimTwoPoweredLight;    // INI "SpecialAnimTwoPoweredLight"
    char   SpecialAnimThree[64];          // INI "SpecialAnimThree"
    char   SpecialAnimThreeDamaged[64];   // INI "SpecialAnimThreeDamaged"
    int    SpecialAnimThreeX;             // INI "SpecialAnimThreeX"
    int    SpecialAnimThreeY;             // INI "SpecialAnimThreeY"
    int    SpecialAnimThreeZAdjust;       // INI "SpecialAnimThreeZAdjust"
    int    SpecialAnimThreeYSort;         // INI "SpecialAnimThreeYSort"
    bool   SpecialAnimThreePowered;       // INI "SpecialAnimThreePowered"
    bool   SpecialAnimThreePoweredLight;  // INI "SpecialAnimThreePoweredLight"
    char   ProductionAnim[64];            // INI "ProductionAnim"
    char   ProductionAnimDamaged[64];     // INI "ProductionAnimDamaged"
    int    ProductionAnimX;               // INI "ProductionAnimX"
    int    ProductionAnimY;               // INI "ProductionAnimY"
    int    ProductionAnimZAdjust;         // INI "ProductionAnimZAdjust"
    int    ProductionAnimYSort;           // INI "ProductionAnimYSort"
    char   PreProductionAnim[64];         // INI "PreProductionAnim"
    char   PreProductionAnimDamaged[64];  // INI "PreProductionAnimDamaged"
    int    PreProductionAnimX;            // INI "PreProductionAnimX"
    int    PreProductionAnimY;            // INI "PreProductionAnimY"
    int    PreProductionAnimZAdjust;      // INI "PreProductionAnimZAdjust"
    int    PreProductionAnimYSort;        // INI "PreProductionAnimYSort"
    char   TurretAnim[64];                // INI "TurretAnim"
    char   TurretAnimDamaged[64];         // INI "TurretAnimDamaged"
    int    TurretAnimX;                   // INI "TurretAnimX"
    int    TurretAnimY;                   // INI "TurretAnimY"
    int    TurretAnimZAdjust;             // INI "TurretAnimZAdjust"
    int    TurretAnimYSort;               // INI "TurretAnimYSort"
    bool   TurretAnimIsVoxel;             // INI "TurretAnimIsVoxel"
    int    Upgrades;                      // INI "Upgrades"

    // ---- [Infantry] section (0x0062027B) -------------------------------
    // These are the TechnoTypeClass/InfantryTypeClass defaults applied to
    // every infantry type.
    int    Nominal;                       // INI "Nominal"
    bool   Cloakable;                     // INI "Cloakable"
    bool   Sensors;                       // INI "Sensors"
    int    Sight;                         // INI "Sight"
    int    TechLevel;                     // INI "TechLevel"
    double Speed;                         // INI "Speed"
    int    Cost;                          // INI "Cost"
    int    Ammo;                          // INI "Ammo"
    int    Points;                        // INI "Points"
    int    ThreatPosed;                   // INI "ThreatPosed"
    char   Owner[64];                     // INI "Owner"
    bool   Trainable;                     // INI "Trainable"
    bool   Crewed;                        // INI "Crewed"
    bool   Repairable;                    // INI "Repairable"
    bool   Invisible;                     // INI "Invisible"
    bool   RadarVisible;                  // INI "RadarVisible"
    bool   SelfHealing;                   // INI "SelfHealing"
    bool   NoAutoFire;                    // INI "NoAutoFire"
    int    Passengers;                    // INI "Passengers"
    int    InfantryFireAngle;             // INI "FireAngle" ([Infantry])
    int    DeployTime;                    // INI "DeployTime"
    bool   Disableable;                   // INI "Disableable"
    bool   ToProtect;                     // INI "ToProtect"
    bool   InfantryTiberiumHeal;          // INI "TiberiumHeal" ([Infantry])
    bool   ImmuneToVeins;                 // INI "ImmuneToVeins"
    bool   AllowedToStartInMultiplayer;   // INI "AllowedToStartInMultiplayer"
    bool   TargetLaser;                   // INI "TargetLaser"
    bool   HunterSeeker;                  // INI "HunterSeeker"
    bool   Crusher;                       // INI "Crusher"
    bool   AutoCrush;                     // INI "AutoCrush"
    bool   TiltsWhenCrushes;              // INI "TiltsWhenCrushes"
    bool   Accelerates;                   // INI "Accelerates"
    int    ZFudgeCliff;                   // INI "ZFudgeCliff"
    int    ZFudgeColumn;                  // INI "ZFudgeColumn"
    int    ZFudgeTunnel;                  // INI "ZFudgeTunnel"
    int    ZFudgeBridge;                  // INI "ZFudgeBridge"

    // ---- [Unit] section (0x00620A13) -----------------------------------
    double TargetDistanceCoefficient;     // INI "TargetDistanceCoefficient"
    int    SpecialThreatValue;            // INI "SpecialThreatValue"
    int    FireAngle;                     // INI "FireAngle"
    int    TurretOffset;                  // INI "TurretOffset"
    int    RotCount;                      // INI "RotCount"
    bool   Remapable;                     // INI "Remapable"
    bool   Normalized;                    // INI "Normalized"
    bool   VisibleLoad;                   // INI "VisibleLoad"
    int    ShadowIndex;                   // INI "ShadowIndex"
    char   Cameo[64];                     // INI "Cameo"
    int    PrimaryFireFLH;                // INI "PrimaryFireFLH"
    int    PBarrelLength;                 // INI "PBarrelLength"
    int    PBarrelThickness;              // INI "PBarrelThickness"
    int    SecondaryFireFLH;              // INI "SecondaryFireFLH"
    int    SBarrelLength;                 // INI "SBarrelLength"
    int    SBarrelThickness;              // INI "SBarrelThickness"
    bool   TurretNotExportedOnGround;     // INI "TurretNotExportedOnGround"

    // ---- [Aircraft] section (0x0061F701) -------------------------------
    double PitchSpeed;                    // INI "PitchSpeed"
    char   Locomotor[64];                 // INI "Locomotor"
    double CloakingSpeed;                 // INI "CloakingSpeed"
    double ThreatAvoidanceCoefficient;    // INI "ThreatAvoidanceCoefficient"
    double SlowdownDistance;              // INI "SlowdownDistance"
    double DeaccelerationFactor;          // INI "DeaccelerationFactor"
    double AccelerationFactor;            // INI "AccelerationFactor"
    int    Weight;                        // INI "Weight"
    int    PhysicalSize;                  // INI "PhysicalSize"
    int    MaxDebris;                     // INI "MaxDebris"
    char   DebrisTypes[64];               // INI "DebrisTypes"

    // ---- global type tables --------------------------------------------
    // /* ? */ the global RulesClass instance owns the TypeList<T*> tables
    // for every type class; the vtables for those lists are in
    // out/vtables.txt (e.g. TypeList<TechnoTypeClass*> 0x006A3C64).
};

#endif // RULESCLASS_H
