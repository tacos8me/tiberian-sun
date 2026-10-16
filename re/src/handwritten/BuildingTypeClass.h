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
    // Slot 0 is a stub shared with 65 other classes (0x405790), not
    // the destructor, so the destructor is declared without claiming a
    // vtable position and slot 0 is named for what it is. Declaring
    // `virtual ~X()` here made the destructor look like slot 0.
    ~BuildingTypeClass();
    virtual void  vfunc_0();                        // [0] 0x405790 (shared stub, 66 classes)
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
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    unsigned long m_000;  // +0x000 init 0x69a758; 4-byte store: mov dword ptr [esi], 0x69a758
    unsigned char pad_004[0x432];
    unsigned char m_436;  // +0x436 /* ? */; 1-byte store: mov byte ptr [esi + 0x436], bl
    unsigned char pad_437[0x39];
    unsigned long m_470;  // +0x470 /* ? */; 4-byte store: mov dword ptr [esi + 0x470], edi
    unsigned long m_474;  // +0x474 /* ? */; 4-byte store: mov dword ptr [esi + 0x474], ebx
    unsigned long m_478;  // +0x478 /* ? */; 4-byte store: mov dword ptr [esi + 0x478], ebx
    unsigned char pad_47c[0x1c];
    unsigned long m_498;  // +0x498 /* ? */; 4-byte store: mov dword ptr [esi + 0x498], ebx
    unsigned long m_49c;  // +0x49c /* ? */; 4-byte store: mov dword ptr [esi + 0x49c], ebx
    unsigned long m_4a0;  // +0x4a0 init 0x1388; 4-byte store: mov dword ptr [esi + 0x4a0], 0x1388
    unsigned long m_4a4;  // +0x4a4 /* ? */; 4-byte store: mov dword ptr [esi + 0x4a4], ebx
    unsigned long m_4a8;  // +0x4a8 /* ? */; 4-byte store: mov dword ptr [esi + 0x4a8], eax
    unsigned long m_4ac;  // +0x4ac /* ? */; 4-byte store: mov dword ptr [esi + 0x4ac], eax
    unsigned long m_4b0;  // +0x4b0 /* ? */; 4-byte store: mov dword ptr [esi + 0x4b0], eax
    unsigned long m_4b4;  // +0x4b4 /* ? */; 4-byte store: mov dword ptr [esi + 0x4b4], eax
    unsigned long m_4b8;  // +0x4b8 /* ? */; 4-byte store: mov dword ptr [esi + 0x4b8], ecx
    unsigned long m_4bc;  // +0x4bc /* ? */; 4-byte store: mov dword ptr [esi + 0x4bc], eax
    unsigned long m_4c0;  // +0x4c0 /* ? */; 4-byte store: mov dword ptr [esi + 0x4c0], ecx
    unsigned long m_4c4;  // +0x4c4 /* ? */; 4-byte store: mov dword ptr [esi + 0x4c4], ebx
    unsigned long m_4c8;  // +0x4c8 /* ? */; 4-byte store: mov dword ptr [esi + 0x4c8], ebx
    unsigned char m_4cc;  // +0x4cc /* ? */; 1-byte store: mov byte ptr [esi + 0x4cc], bl
    unsigned char pad_4cd[0xf];
    unsigned char m_4dc;  // +0x4dc /* ? */; 1-byte store: mov byte ptr [esi + 0x4dc], bl
    unsigned char pad_4dd[0x17];
    unsigned long m_4f4;  // +0x4f4 /* ? */; 4-byte store: mov dword ptr [esi + 0x4f4], ebx
    unsigned long m_4f8;  // +0x4f8 /* ? */; 4-byte store: mov dword ptr [esi + 0x4f8], edi
    unsigned long Adjacent;  // +0x4fc (INI key); 4-byte store: mov dword ptr [esi + 0x4fc], 3
    unsigned long m_500;  // +0x500 /* ? */; 4-byte store: mov dword ptr [esi + 0x500], ebx
    unsigned long m_504;  // +0x504 /* ? */; 4-byte store: mov dword ptr [esi + 0x504], ebx
    unsigned long m_508;  // +0x508 /* ? */; 4-byte store: mov dword ptr [esi + 0x508], ebx
    unsigned long m_50c;  // +0x50c /* ? */; 4-byte store: mov dword ptr [esi + 0x50c], ebx
    unsigned long m_510;  // +0x510 /* ? */; 4-byte store: mov dword ptr [esi + 0x510], ebx
    unsigned long m_514;  // +0x514 /* ? */; 4-byte store: mov dword ptr [esi + 0x514], ebx
    unsigned long m_518;  // +0x518 /* ? */; 4-byte store: mov dword ptr [esi + 0x518], ebx
    unsigned long m_51c;  // +0x51c /* ? */; 4-byte store: mov dword ptr [esi + 0x51c], ebx
    unsigned long m_520;  // +0x520 /* ? */; 4-byte store: mov dword ptr [esi + 0x520], ebx
    unsigned long m_524;  // +0x524 /* ? */; 4-byte store: mov dword ptr [esi + 0x524], ebp
    unsigned long m_528;  // +0x528 /* ? */; 4-byte store: mov dword ptr [esi + 0x528], ebx
    unsigned long m_52c;  // +0x52c /* ? */; 4-byte store: mov dword ptr [esi + 0x52c], ebx
    unsigned long m_530;  // +0x530 /* ? */; 4-byte store: mov dword ptr [esi + 0x530], ebx
    unsigned long m_534;  // +0x534 /* ? */; 4-byte store: mov dword ptr [esi + 0x534], ebp
    unsigned long m_538;  // +0x538 /* ? */; 4-byte store: mov dword ptr [esi + 0x538], ebx
    unsigned long m_53c;  // +0x53c /* ? */; 4-byte store: mov dword ptr [esi + 0x53c], ebx
    unsigned long m_540;  // +0x540 /* ? */; 4-byte store: mov dword ptr [esi + 0x540], ebp
    unsigned long m_544;  // +0x544 /* ? */; 4-byte store: mov dword ptr [esi + 0x544], ebx
    unsigned long m_548;  // +0x548 /* ? */; 4-byte store: mov dword ptr [esi + 0x548], ebx
    unsigned long m_54c;  // +0x54c /* ? */; 4-byte store: mov dword ptr [esi + 0x54c], ebp
    unsigned long m_550;  // +0x550 /* ? */; 4-byte store: mov dword ptr [esi + 0x550], ebx
    unsigned char pad_554[0xc];
    unsigned long m_560;  // +0x560 /* ? */; 4-byte store: mov dword ptr [esi + 0x560], ebx
    unsigned long m_564;  // +0x564 /* ? */; 4-byte store: mov dword ptr [esi + 0x564], ebp
    unsigned long m_568;  // +0x568 /* ? */; 4-byte store: mov dword ptr [esi + 0x568], ebx
    unsigned long m_56c;  // +0x56c /* ? */; 4-byte store: mov dword ptr [esi + 0x56c], ebx
    unsigned long m_570;  // +0x570 /* ? */; 4-byte store: mov dword ptr [esi + 0x570], ebp
    unsigned long m_574;  // +0x574 /* ? */; 4-byte store: mov dword ptr [esi + 0x574], ebx
    unsigned char pad_578[0x23c];
    unsigned long m_7b4;  // +0x7b4 /* ? */; 4-byte store: mov dword ptr [esi + 0x7b4], ebx
    unsigned long m_7b8;  // +0x7b8 /* ? */; 4-byte store: mov dword ptr [esi + 0x7b8], ebx
    unsigned long m_7bc;  // +0x7bc /* ? */; 4-byte store: mov dword ptr [esi + 0x7bc], ebx
    unsigned long m_7c0;  // +0x7c0 /* ? */; 4-byte store: mov dword ptr [esi + 0x7c0], ebx
    unsigned long m_7c4;  // +0x7c4 /* ? */; 4-byte store: mov dword ptr [esi + 0x7c4], ebx
    unsigned long m_7c8;  // +0x7c8 /* ? */; 4-byte store: mov dword ptr [esi + 0x7c8], ebx
    unsigned long m_7cc;  // +0x7cc /* ? */; 4-byte store: mov dword ptr [esi + 0x7cc], ebx
    unsigned long m_7d0;  // +0x7d0 /* ? */; 4-byte store: mov dword ptr [esi + 0x7d0], ebx
    unsigned long m_7d4;  // +0x7d4 /* ? */; 4-byte store: mov dword ptr [esi + 0x7d4], ebx
    unsigned long m_7d8;  // +0x7d8 /* ? */; 4-byte store: mov dword ptr [esi + 0x7d8], ebx
    unsigned long m_7dc;  // +0x7dc /* ? */; 4-byte store: mov dword ptr [esi + 0x7dc], ebx
    unsigned long m_7e0;  // +0x7e0 /* ? */; 4-byte store: mov dword ptr [esi + 0x7e0], eax
    unsigned long m_7e4;  // +0x7e4 /* ? */; 4-byte store: mov dword ptr [esi + 0x7e4], ebx
    unsigned char pad_7e8[0x10];
    unsigned short m_7f8;  // +0x7f8 /* ? */; 2-byte store: mov word ptr [esi + 0x7f8], bx
    unsigned char m_7fa;  // +0x7fa init 0x1; 1-byte store: mov byte ptr [esi + 0x7fa], 1
    unsigned char m_7fb;  // +0x7fb /* ? */; 1-byte store: mov byte ptr [esi + 0x7fb], bl
    unsigned char m_7fc;  // +0x7fc /* ? */; 1-byte store: mov byte ptr [esi + 0x7fc], bl
    unsigned char m_7fd;  // +0x7fd /* ? */; 1-byte store: mov byte ptr [esi + 0x7fd], bl
    unsigned char m_7fe;  // +0x7fe /* ? */; 1-byte store: mov byte ptr [esi + 0x7fe], bl
    unsigned char BaseNormal;  // +0x7ff (INI key); 1-byte store: mov byte ptr [esi + 0x7ff], 1
    unsigned char Bib;  // +0x800 (INI key); 1-byte store: mov byte ptr [esi + 0x800], bl
    unsigned char Wall;  // +0x801 (INI key); 1-byte store: mov byte ptr [esi + 0x801], bl
    unsigned char Capturable;  // +0x802 (INI key); 1-byte store: mov byte ptr [esi + 0x802], bl
    unsigned char Powered;  // +0x803 (INI key); 1-byte store: mov byte ptr [esi + 0x803], bl
    unsigned char Unsellable;  // +0x804 (INI key); 1-byte store: mov byte ptr [esi + 0x804], bl
    unsigned char Radar;  // +0x805 (INI key); 1-byte store: mov byte ptr [esi + 0x805], bl
    unsigned char m_806;  // +0x806 /* ? */; 1-byte store: mov byte ptr [esi + 0x806], bl
    unsigned char m_807;  // +0x807 /* ? */; 1-byte store: mov byte ptr [esi + 0x807], bl
    unsigned char m_808;  // +0x808 /* ? */; 1-byte store: mov byte ptr [esi + 0x808], bl
    unsigned char m_809;  // +0x809 /* ? */; 1-byte store: mov byte ptr [esi + 0x809], bl
    unsigned char m_80a;  // +0x80a /* ? */; 1-byte store: mov byte ptr [esi + 0x80a], bl
    unsigned char m_80b;  // +0x80b /* ? */; 1-byte store: mov byte ptr [esi + 0x80b], bl
    unsigned char m_80c;  // +0x80c /* ? */; 1-byte store: mov byte ptr [esi + 0x80c], bl
    unsigned char m_80d;  // +0x80d init 0x1; 1-byte store: mov byte ptr [esi + 0x80d], 1
    unsigned char m_80e;  // +0x80e /* ? */; 1-byte store: mov byte ptr [esi + 0x80e], bl
    unsigned char m_80f;  // +0x80f /* ? */; 1-byte store: mov byte ptr [esi + 0x80f], bl
    unsigned char m_810;  // +0x810 /* ? */; 1-byte store: mov byte ptr [esi + 0x810], bl
    unsigned char m_811;  // +0x811 /* ? */; 1-byte store: mov byte ptr [esi + 0x811], bl
    unsigned char m_812;  // +0x812 /* ? */; 1-byte store: mov byte ptr [esi + 0x812], bl
    unsigned char m_813;  // +0x813 /* ? */; 1-byte store: mov byte ptr [esi + 0x813], bl
    unsigned char Weeder;  // +0x814 (INI key); 1-byte store: mov byte ptr [esi + 0x814], bl
    unsigned char m_815;  // +0x815 /* ? */; 1-byte store: mov byte ptr [esi + 0x815], bl
    unsigned char m_816;  // +0x816 /* ? */; 1-byte store: mov byte ptr [esi + 0x816], bl
    unsigned char m_817;  // +0x817 /* ? */; 1-byte store: mov byte ptr [esi + 0x817], bl
    unsigned char m_818;  // +0x818 /* ? */; 1-byte store: mov byte ptr [esi + 0x818], bl
    unsigned char m_819;  // +0x819 /* ? */; 1-byte store: mov byte ptr [esi + 0x819], bl
    unsigned char m_81a;  // +0x81a /* ? */; 1-byte store: mov byte ptr [esi + 0x81a], bl
    unsigned char m_81b;  // +0x81b /* ? */; 1-byte store: mov byte ptr [esi + 0x81b], bl
    unsigned char m_81c;  // +0x81c /* ? */; 1-byte store: mov byte ptr [esi + 0x81c], bl
    unsigned char m_81d;  // +0x81d /* ? */; 1-byte store: mov byte ptr [esi + 0x81d], bl
    unsigned char m_81e;  // +0x81e /* ? */; 1-byte store: mov byte ptr [esi + 0x81e], bl
    unsigned char m_81f;  // +0x81f /* ? */; 1-byte store: mov byte ptr [esi + 0x81f], bl
    unsigned char m_820;  // +0x820 /* ? */; 1-byte store: mov byte ptr [esi + 0x820], bl
    unsigned char m_821;  // +0x821 /* ? */; 1-byte store: mov byte ptr [esi + 0x821], bl
    unsigned char Helipad;  // +0x822 (INI key); 1-byte store: mov byte ptr [esi + 0x822], bl
    unsigned char m_823;  // +0x823 /* ? */; 1-byte store: mov byte ptr [esi + 0x823], bl
    unsigned char m_824;  // +0x824 /* ? */; 1-byte store: mov byte ptr [esi + 0x824], bl
    unsigned char pad_825[0x3];
    unsigned long m_828;  // +0x828 /* ? */; 4-byte store: mov dword ptr [esi + 0x828], edi
    unsigned long m_82c;  // +0x82c /* ? */; 4-byte store: mov dword ptr [esi + 0x82c], edi
    unsigned long m_830;  // +0x830 init 0x9; 4-byte store: mov dword ptr [esi + 0x830], 9
    unsigned long m_834;  // +0x834 /* ? */; 4-byte store: mov dword ptr [esi + 0x834], edi
    unsigned char m_838;  // +0x838 /* ? */; 1-byte store: mov byte ptr [esi + 0x838], bl
    unsigned char m_839;  // +0x839 /* ? */; 1-byte store: mov byte ptr [esi + 0x839], bl
    unsigned char m_83a;  // +0x83a /* ? */; 1-byte store: mov byte ptr [esi + 0x83a], bl
    unsigned char m_83b;  // +0x83b /* ? */; 1-byte store: mov byte ptr [esi + 0x83b], bl
    unsigned char m_83c;  // +0x83c init 0x1; 1-byte store: mov byte ptr [esi + 0x83c], 1
    unsigned char m_83d;  // +0x83d /* ? */; 1-byte store: mov byte ptr [esi + 0x83d], bl
    unsigned char m_83e;  // +0x83e /* ? */; 1-byte store: mov byte ptr [esi + 0x83e], bl
    unsigned char m_83f;  // +0x83f init 0x14; 1-byte store: mov byte ptr [esi + 0x83f], 0x14
    unsigned char m_840;  // +0x840 /* ? */; 1-byte store: mov byte ptr [esi + 0x840], bl
    unsigned char m_841;  // +0x841 /* ? */; 1-byte store: mov byte ptr [esi + 0x841], bl
    unsigned char m_842;  // +0x842 /* ? */; 1-byte store: mov byte ptr [esi + 0x842], bl
    unsigned char m_843;  // +0x843 /* ? */; 1-byte store: mov byte ptr [esi + 0x843], bl
    unsigned char m_844;  // +0x844 /* ? */; 1-byte store: mov byte ptr [esi + 0x844], bl

};
