#pragma once
// ============================================================================
// BuildingTypeClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// Evidence:
//   RTTI class descriptor  : 0x6aa500
//   TypeDescriptor name    : 0x6c0820 + 8 -> ".?AVBuildingTypeClass@@"
//   vtable (primary)       : 0x6aa510 is the COL ptr; primary vtable 0x69a758
//   vtable (IRTTITypeInfo) : 0x69a73c   (COL ptr slot 0x6aa4a0, mdisp=4)
//   constructor            : 0x43dc10   (writes vtbl 0x69a758 / 0x69a73c)
//   INI reader (general)   : 0x43f250   (vtable slot 25)
//   INI reader (anims)     : 0x43fb5d   (referenced from 0x43f250)
//   INI reader (flags)     : 0x43f45f   (referenced from 0x43f250)
// ============================================================================

#include "TechnoTypeClass.h"

class BuildingTypeClass : public TechnoTypeClass
{
public:
    // ---- vtable 0x69a758 (primary, 46 entries) ----------------------------
    // Slot 6 is the RTTI CompleteObjectLocator pointer (0x6aa510), not code.
    virtual ~BuildingTypeClass();                                   // [0] 0x405790
    virtual void  Init();                                           // [1] 0x405830
    virtual void  Shutdown();                                       // [2] 0x405840
    virtual long  QueryInterface(const void*, void**);              // [3] 0x441830
    virtual unsigned long AddRef();                                 // [4] 0x405990
    virtual long  Read_INI(void* ini, const char* section);         // [5] 0x441740
    virtual void  vfunc_6();                                        // [6] 0x441810
    virtual void  vfunc_7();                                        // [7] 0x621bf0
    virtual void  vfunc_8();                                        // [8] 0x442220
    virtual void  Set_ID(int id);                                   // [9] 0x4059b0
    virtual int   Get_RTTI();                                       // [10] 0x4059c0
    virtual long  GetClassID(void* pClassID);                       // [11] 0x4421f0
    virtual long  IsDirty();                                        // [12] 0x442200
    virtual long  Load(IStream* pStm);                              // [13] 0x4413f0
    virtual long  Save(IStream* pStm, int fClearDirty);             // [14] 0x4059d0
    virtual long  GetSizeMax(unsigned long* pcbSize);               // [15] 0x4059e0
    virtual void  Compute_CRC();                                    // [16] 0x442210
    virtual void  vfunc_17();                                       // [17] 0x405970
    virtual void  vfunc_18();                                       // [18] 0x405a00
    virtual void  vfunc_19();                                       // [19] 0x405a30
    virtual void  vfunc_20();                                       // [20] 0x405a60
    virtual void  vfunc_21();                                       // [21] 0x405a70
    virtual void  vfunc_22();                                       // [22] 0x405a80
    virtual void  vfunc_23();                                       // [23] 0x405ab0
    virtual int   GetType_2();                                      // [24] 0x406010
    virtual long  Read_INI_2(void* ini, const char* section);       // [25] 0x43f250  (INI reader)
    virtual int   Fetch_ID_2();                                     // [26] 0x405fb0  (refs "Name")
    virtual int   Get_RTTI_2();                                     // [28] 0x441320
    virtual void  vfunc_29();                                       // [29] 0x61f470
    virtual void  vfunc_30();                                       // [30] 0x43e740
    virtual void  vfunc_31();                                       // [31] 0x43e600
    virtual void  vfunc_32();                                       // [32] 0x4413a0
    virtual void  vfunc_33();                                       // [33] 0x43e210
    virtual void  vfunc_34();                                       // [34] 0x43e830
    virtual void  vfunc_35();                                       // [35] 0x61f490
    virtual void  vfunc_36();                                       // [36] 0x43e290
    virtual void  vfunc_37();                                       // [37] 0x43e680
    virtual void  vfunc_38();                                       // [38] 0x571710
    virtual void  vfunc_39();                                       // [39] 0x61f4f0
    virtual void  vfunc_40();                                       // [40] 0x43eaa0
    virtual void  vfunc_41();                                       // [41] 0x441370
    virtual void  vfunc_42();                                       // [42] 0x43e7b0
    virtual void  vfunc_43();                                       // [43] 0x61f500
    virtual void  vfunc_44();                                       // [44] 0x61f550
    virtual void  vfunc_45();                                       // [45] 0x622180

    // ---- data members -----------------------------------------------------
    // Evidence: constructor 0x43dc10 writes these offsets; INI readers
    // 0x43f250 / 0x43f45f / 0x43fb5d reference the named keys.
    int   m_Field470;           // +0x470 ctor: mov dword ptr [esi+0x470], edi
    int   m_Field474;           // +0x474 ctor: mov dword ptr [esi+0x474], ebx (0)
    int   m_Field478;           // +0x478 ctor: mov dword ptr [esi+0x478], ebx (0)
    int   m_Field47c;           // +0x47c ctor: lea ecx,[esi+0x47c] (sub-object)
    int   m_Field498;           // +0x498 ctor: mov dword ptr [esi+0x498], ebx (0)
    int   m_Field49c;           // +0x49c ctor: mov dword ptr [esi+0x49c], ebx (0)
    int   m_Field4a0;           // +0x4a0 ctor: mov dword ptr [esi+0x4a0], 0x1388
    int   m_Field4a4;           // +0x4a4 ctor: mov dword ptr [esi+0x4a4], ebx (0)
    int   m_Field4a8;           // +0x4a8 ctor: mov dword ptr [esi+0x4a8], eax
    int   m_Field4ac;           // +0x4ac ctor: mov dword ptr [esi+0x4ac], eax
    int   m_Field4b0;           // +0x4b0 ctor: mov dword ptr [esi+0x4b0], eax
    int   m_Field4b4;           // +0x4b4 ctor: mov dword ptr [esi+0x4b4], eax
    int   m_Field4b8;           // +0x4b8 ctor: mov dword ptr [esi+0x4b8], ecx
    int   m_Field4bc;           // +0x4bc ctor: mov dword ptr [esi+0x4bc], eax
    int   m_Field4c0;           // +0x4c0 ctor: mov dword ptr [esi+0x4c0], ecx
    int   m_Field4c4;           // +0x4c4 ctor: mov dword ptr [esi+0x4c4], ebx (0)
    int   m_Field4c8;           // +0x4c8 ctor: mov dword ptr [esi+0x4c8], ebx (0)
    int   m_Field4f4;           // +0x4f4 ctor: mov dword ptr [esi+0x4f4], ebx (0)
    int   m_Field4f8;           // +0x4f8 ctor: mov dword ptr [esi+0x4f8], edi
    int   m_Field4fc;           // +0x4fc ctor: mov dword ptr [esi+0x4fc], 3
    int   m_Field500;           // +0x500 ctor: mov dword ptr [esi+0x500], ebx (0)
    int   m_Field504;           // +0x504 ctor: mov dword ptr [esi+0x504], ebx (0)
    int   m_Field508;           // +0x508 ctor: mov dword ptr [esi+0x508], ebx (0)
    int   m_Field50c;           // +0x50c ctor: mov dword ptr [esi+0x50c], ebx (0)
    int   m_Field510;           // +0x510 ctor: mov dword ptr [esi+0x510], ebx (0)
    int   m_Field514;           // +0x514 ctor: mov dword ptr [esi+0x514], ebx (0)
    int   m_Field518;           // +0x518 ctor: mov dword ptr [esi+0x518], ebx (0)
    int   m_Field51c;           // +0x51c ctor: mov dword ptr [esi+0x51c], ebx (0)
    int   m_Field520;           // +0x520 ctor: mov dword ptr [esi+0x520], ebx (0)
    int   m_Field524;           // +0x524 ctor: mov dword ptr [esi+0x524], ebp
    int   m_Field528;           // +0x528 ctor: mov dword ptr [esi+0x528], ebx (0)
    int   m_Field52c;           // +0x52c ctor: mov dword ptr [esi+0x52c], ebx (0)
    int   m_Field7b4;           // +0x7b4 ctor: mov dword ptr [esi+0x7b4], ebx (0)
    int   m_Field7b8;           // +0x7b8 ctor: mov dword ptr [esi+0x7b8], ebx (0)
    int   m_Field7bc;           // +0x7bc ctor: mov dword ptr [esi+0x7bc], ebx (0)
    int   m_Field7c0;           // +0x7c0 ctor: mov dword ptr [esi+0x7c0], ebx (0)
    int   m_Field7c4;           // +0x7c4 ctor: mov dword ptr [esi+0x7c4], ebx (0)
    int   m_Field7c8;           // +0x7c8 ctor: mov dword ptr [esi+0x7c8], ebx (0)
    int   m_Field7cc;           // +0x7cc ctor: mov dword ptr [esi+0x7cc], ebx (0)
    int   m_Field7d0;           // +0x7d0 ctor: mov dword ptr [esi+0x7d0], ebx (0)
    int   m_Field7d4;           // +0x7d4 ctor: mov dword ptr [esi+0x7d4], ebx (0)
    int   m_Field7d8;           // +0x7d8 ctor: mov dword ptr [esi+0x7d8], ebx (0)
    int   m_Field7dc;           // +0x7dc ctor: mov dword ptr [esi+0x7dc], ebx (0)
    int   m_Field7e0;           // +0x7e0 ctor: mov dword ptr [esi+0x7e0], eax
    int   m_Field7e4;           // +0x7e4 ctor: mov dword ptr [esi+0x7e4], ebx (0)
    short m_Field7f8;           // +0x7f8 ctor: mov word ptr [esi+0x7f8], bx (0)
    bool  m_Field7fa;           // +0x7fa ctor: mov byte ptr [esi+0x7fa], 1
    bool  m_Field7fb;           // +0x7fb ctor: mov byte ptr [esi+0x7fb], bl (0)
    bool  m_Field7fc;           // +0x7fc ctor: mov byte ptr [esi+0x7fc], bl (0)
    bool  m_Field7fd;           // +0x7fd ctor: mov byte ptr [esi+0x7fd], bl (0)
    bool  m_Field7fe;           // +0x7fe ctor: mov byte ptr [esi+0x7fe], bl (0)
    bool  m_Field7ff;           // +0x7ff ctor: mov byte ptr [esi+0x7ff], 1
    bool  m_Field800;           // +0x800 ctor: mov byte ptr [esi+0x800], bl (0)
    bool  m_Field801;           // +0x801 ctor: mov byte ptr [esi+0x801], bl (0)
    bool  m_Field802;           // +0x802 ctor: mov byte ptr [esi+0x802], bl (0)
    bool  m_Field803;           // +0x803 ctor: mov byte ptr [esi+0x803], bl (0)
    bool  m_Field804;           // +0x804 ctor: mov byte ptr [esi+0x804], bl (0)
    bool  m_Field805;           // +0x805 ctor: mov byte ptr [esi+0x805], bl (0)
    bool  m_Field806;           // +0x806 ctor: mov byte ptr [esi+0x806], bl (0)
    bool  m_Field807;           // +0x807 ctor: mov byte ptr [esi+0x807], bl (0)
    bool  m_Field808;           // +0x808 ctor: mov byte ptr [esi+0x808], bl (0)
    bool  m_Field809;           // +0x809 ctor: mov byte ptr [esi+0x809], bl (0)
    bool  m_Field80a;           // +0x80a ctor: mov byte ptr [esi+0x80a], bl (0)
    bool  m_Field80b;           // +0x80b ctor: mov byte ptr [esi+0x80b], bl (0)
    bool  m_Field812;           // +0x812 ctor: mov byte ptr [esi+0x812], bl (0)

    // ---- INI keys read by 0x43f250 / 0x43f45f / 0x43fb5d -------------------
    // These are the original rules.ini field names; their exact member offsets
    // within the +0x470..+0x812 block are not yet individually pinned.  /* ? */
    //   "Adjacent", "BaseNormal", "Capturable", "Factory", "FreeUnit",
    //   "HalfDamageSmokeLocation1", "HalfDamageSmokeLocation2", "HasSpotlight",
    //   "Helipad", "Powered", "Radar", "Unsellable", "Wall", "WaterBound",
    //   "Weeder", "AIBuildThis", "Armory", "Artillary", "BridgeRepairHut",
    //   "ChargeAnim", "CloakGenerator", "CloakRadiusInCells",
    //   "ConstructionYard", "DockUnload", "EMPulseCannon", "ExitCoord",
    //   "FirestormWall", "Flat", "GDIBarracks", "Gate", "GateCloseDelay",
    //   "HasStupidGuardMode", "Height", "Hospital", "HoverPad", "ICBMLauncher",
    //   "InvisibleInGame", "IsBaseDefense", "IsPlug", "IsTemple",
    //   "IsThreatRatingNode", "LaserFence", "LaserFencePost", "LightBlueTint",
    //   "LightGreenTint", "LightIntensity", "LightRedTint", "LightVisibility",
    //   "NODBarracks", "NukeSilo", "PlaceAnywhere", "Power",
    //   "PowersUpBuilding", "PowersUpToLevel", "Recoilless", "Refinery",
    //   "SensorArray", "SiloDamage", "SuperWeapon", "SuperWeapon2", "TickTank",
    //   "ToOverlay", "TogglePower", "UnitReload", "UnitRepair",
    //   "WeaponsFactory", "ActiveAnim*", "SpecialAnim*", "TurretAnim*",
    //   "ProductionAnim*", "PreProductionAnim*", "Buildup", "Foundation",
    //   "DoorStages", "GateStages", "Upgrades", "ZShapePointMove", ...

    // ---- data members (offsets from ctor 0x0043dc10) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    // 91 member(s) already declared above; their offsets are recorded in out/members.json

};
