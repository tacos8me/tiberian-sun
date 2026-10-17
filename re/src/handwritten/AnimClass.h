#pragma once
// ============================================================================
// AnimClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// Evidence:
//   RTTI class descriptor  : 0x6a9b40  (CompleteObjectLocator for AnimClass)
//   TypeDescriptor name    : 0x6bfc48 + 8 -> ".?AVAnimClass@@"
//   vtable (primary)       : 0x699890   (COL ptr slot 0x69988c)
//   vtable (IRTTITypeInfo) : 0x699874   (COL ptr slot 0x699870, mdisp=4)
//   constructor            : 0x412f30   (writes vtbl 0x699890; full member init)
//   ctor (from stream)     : 0x415860   (writes vtbl 0x699890)
//   ctor (copy)            : 0x413700   (writes vtbl 0x699890)
//   destructor             : 0x405790   (vtable slot 0)
//
// AnimClass derives from ObjectClass (RTTI base list: ObjectClass mdisp 0,
// AbstractClass mdisp 0, IPersistStream/IPersist/IUnknown mdisp 0,
// IRTTITypeInfo/IUnknown mdisp 4, StageClass mdisp 76).  It is a playing
// animation object (explosions, smoke, tiberium effects, ...).
//
// INI keys read by the anim type reader (out/ini_keys.json):
//   "AnimLow", "AnimHigh", "AnimRate", "AnimPalette" -> 0x445eae.
//   "IsAnimated", "AnimationRate", "AnimationProbability",
//   "TemperateOccupationBits", "SnowOccupationBits" -> 0x6253d8.
//   "IsTiberium", "IsMeteor", "SpawnCount", "IsVeins",
//   "TiberiumSpreadRadius", "TiberiumSpawnType" -> 0x418462.
//   "IsAnimatedTiberium", "ShouldFogRemove", "IsFlamingGuy", "RunningFrames",
//   "YDrawOffset", "StartSound", "BounceSound", "ExpireSound" -> 0x418503.
//   "ExpireAnim" -> 0x4186f4, "TrailerAnim" -> 0x4187b4.
//   "Bouncer", "Tiled", "ShouldUseCellDrawer", "UseNormalLight",
//   "RandomRate" -> 0x4188df.
// ============================================================================

#include "ObjectClass.h"
#include "StageClass.h"

// StageClass is a real base of AnimClass: RTTI lists "StageClass (mdisp 76)",
// i.e. the sub-object sits at +0x4c. It was previously documented in comments
// below but never declared, so the emitted layout was missing that base.
class AnimClass : public ObjectClass, public StageClass
{
public:
    // ---- vtable 0x699890 (primary, 107 entries) ---------------------------
    // Slot 6 is the RTTI CompleteObjectLocator pointer (0x69988c), not code.
    // Slot 0 is a stub shared with 65 other classes (0x405790), not
    // the destructor, so the destructor is declared without claiming a
    // vtable position and slot 0 is named for what it is. Declaring
    // `virtual ~X()` here made the destructor look like slot 0.
    ~AnimClass();
    virtual void  vfunc_0();                        // [0] 0x405790 (shared stub, 66 classes)
    virtual void  Init();                                           // [1] 0x405830
    virtual void  Shutdown();                                       // [2] 0x405840
    virtual long  QueryInterface(const void*, void**);              // [3] 0x4168b0
    virtual unsigned long AddRef();                                 // [4] 0x405990
    virtual long  Load(IStream* pStm);                              // [5] 0x415860
    virtual long  Save(IStream* pStm, int fClearDirty);             // [6] 0x4158c0
    virtual int   GetType();                                        // [7] 0x405910
    virtual long  GetClassID(void* pClassID);                       // [8] 0x416900
    virtual void  Set_ID(int id);                                   // [9] 0x4059b0
    virtual int   Get_RTTI();                                       // [10] 0x415740
    virtual long  IsDirty();                                        // [11] 0x4168f0
    virtual long  GetSizeMax(unsigned long* pcbSize);               // [12] 0x4168a0
    virtual void  Compute_CRC();                                    // [13] 0x4158e0
    virtual long  vfunc_14();                                       // [14] 0x4059d0
    virtual long  vfunc_15();                                       // [15] 0x4059e0
    virtual void  vfunc_16();                                       // [16] 0x4059f0
    virtual void  vfunc_17();                                       // [17] 0x570a20
    virtual void  vfunc_18();                                       // [18] 0x413910
    virtual void  vfunc_19();                                       // [19] 0x405a30
    virtual void  vfunc_20();                                       // [20] 0x570e80
    virtual void  vfunc_21();                                       // [21] 0x570ea0
    virtual void  vfunc_22();                                       // [22] 0x405a80
    virtual void  vfunc_23();                                       // [23] 0x414250
    virtual void  vfunc_24();                                       // [24] 0x4167b0
    virtual void  vfunc_25();                                       // [25] 0x4167c0
    virtual void  vfunc_26();                                       // [26] 0x56e820
    virtual void  vfunc_27();                                       // [27] 0x56eb50
    virtual void  vfunc_28();                                       // [28] 0x56eb40
    virtual void  vfunc_29();                                       // [29] 0x4150e0
    virtual void  vfunc_30();                                       // [30] 0x570ef0
    virtual void  vfunc_31();                                       // [31] 0x4167d0
    virtual void  vfunc_32();                                       // [32] 0x570ec0
    virtual void  vfunc_33();                                       // [33] 0x4159f0
    virtual void  vfunc_34();                                       // [34] 0x56eba0
    virtual void  vfunc_35();                                       // [35] 0x4167e0
    virtual void  vfunc_36();                                       // [36] 0x56ebb0
    virtual void  vfunc_37();                                       // [37] 0x56ebc0
    virtual void  vfunc_38();                                       // [38] 0x56ebd0
    virtual void  vfunc_39();                                       // [39] 0x56ebe0
    virtual void  vfunc_40();                                       // [40] 0x40e5d0
    virtual void  vfunc_41();                                       // [41] 0x40e600
    virtual void  vfunc_42();                                       // [42] 0x40e630
    virtual void  vfunc_43();                                       // [43] 0x4167f0
    virtual void  vfunc_44();                                       // [44] 0x40e660
    virtual void  vfunc_45();                                       // [45] 0x413900
    virtual void  vfunc_46();                                       // [46] 0x570db0
    virtual void  vfunc_47();                                       // [47] 0x416830
    virtual void  vfunc_48();                                       // [48] 0x415a00
    virtual void  vfunc_49();                                       // [49] 0x56f810
    virtual void  vfunc_50();                                       // [50] 0x56fb40
    virtual void  vfunc_51();                                       // [51] 0x56ebf0
    virtual void  vfunc_52();                                       // [52] 0x5700c0
    virtual void  vfunc_53();                                       // [53] 0x56ea70
    virtual void  vfunc_54();                                       // [54] 0x570480
    virtual void  vfunc_55();                                       // [55] 0x570500
    virtual void  vfunc_56();                                       // [56] 0x415b70
    virtual void  vfunc_57();                                       // [57] 0x56ec00
    virtual void  vfunc_58();                                       // [58] 0x56ec10
    virtual void  vfunc_59();                                       // [59] 0x413990
    virtual void  vfunc_60();                                       // [60] 0x414050
    virtual void  vfunc_61();                                       // [61] 0x416840
    virtual void  vfunc_62();                                       // [62] 0x416850
    virtual void  vfunc_63();                                       // [63] 0x4139c0
    virtual void  vfunc_64();                                       // [64] 0x570990
    virtual void  vfunc_65();                                       // [65] 0x56ec20
    virtual void  vfunc_66();                                       // [66] 0x56ec30
    virtual void  vfunc_67();                                       // [67] 0x414030
    virtual void  vfunc_68();                                       // [68] 0x56f000
    virtual void  vfunc_69();                                       // [69] 0x56f160
    virtual void  vfunc_70();                                       // [70] 0x40e690
    virtual void  vfunc_71();                                       // [71] 0x56f680
    virtual void  vfunc_72();                                       // [72] 0x56ec50
    virtual void  vfunc_73();                                       // [73] 0x56ec40
    virtual void  vfunc_74();                                       // [74] 0x56ec60
    virtual void  vfunc_75();                                       // [75] 0x56ee20
    virtual void  vfunc_76();                                       // [76] 0x56eda0
    virtual void  vfunc_77();                                       // [77] 0x56ec70
    virtual void  vfunc_78();                                       // [78] 0x56ec80
    virtual void  vfunc_79();                                       // [79] 0x56fbe0
    virtual void  vfunc_80();                                       // [80] 0x56ec90
    virtual void  vfunc_81();                                       // [81] 0x56eca0
    virtual void  vfunc_82();                                       // [82] 0x56ecb0
    virtual void  vfunc_83();                                       // [83] 0x56ecc0
    virtual void  vfunc_84();                                       // [84] 0x56ecd0
    virtual void  vfunc_85();                                       // [85] 0x416860
    virtual void  vfunc_86();                                       // [86] 0x416870
    virtual void  vfunc_87();                                       // [87] 0x570240
    virtual void  vfunc_88();                                       // [88] 0x56fbc0
    virtual void  vfunc_89();                                       // [89] 0x5700b0
    virtual void  vfunc_90();                                       // [90] 0x56ece0
    virtual void  vfunc_91();                                       // [91] 0x56ecf0
    virtual void  vfunc_92();                                       // [92] 0x570e70
    virtual void  vfunc_93();                                       // [93] 0x56ed00
    virtual void  vfunc_94();                                       // [94] 0x416880
    virtual void  vfunc_95();                                       // [95] 0x416890
    virtual void  vfunc_96();                                       // [96] 0x40e6a0
    virtual void  vfunc_97();                                       // [97] 0x570cc0
    virtual void  vfunc_98();                                       // [98] 0x40e6c0
    virtual void  vfunc_99();                                       // [99] 0x570ce0
    virtual void  vfunc_100();                                      // [100] 0x570d00
    virtual void  vfunc_101();                                      // [101] 0x570d50
    virtual void  vfunc_102();                                      // [102] 0x570370
    virtual void  vfunc_103();                                      // [103] 0x5703b0
    virtual void  vfunc_104();                                      // [104] 0x415b90
    virtual void  vfunc_105();                                      // [105] 0x4140b0
    virtual void  vfunc_106();                                      // [106] 0x4159e0

    // ---- data members -----------------------------------------------------
    // Offsets recovered from constructor 0x412f30.  The ObjectClass base
    // occupies +0x00..+0x4b; the StageClass base is at mdisp 76 (+0x4c).
    //
    // +0x00  vftable (0x699890)
    // +0x04  IRTTITypeInfo sub-object vftable (0x699874)
    // +0x08  ObjectClass / AbstractClass base members (see ObjectClass.h)
    // +0x4c  StageClass base sub-object (mdisp 76)  (0x412f30: [esi+0x4c]=0)
    // +0x50  int   m_Stage /* ? */            (0x412f30: [esi+0x50]=eax)
    // +0x58  int   m_LoopCount /* ? */        (0x412f30: [esi+0x58]=0)
    // +0x5c  int   m_LoopRemaining /* ? */    (0x412f30: [esi+0x5c]=0)
    // +0x60  int   m_IsActive /* ? */         (0x412f30: [esi+0x60]=1)
    // +0x64  int   m_AnimType /* ? */         (0x412f30: [esi+0x64]=ecx)
    // +0x68  int   m_AnimImage /* ? */        (0x412f30: [esi+0x68]=0)
    // +0x6c  int   m_Owner /* ? */            (0x412f30: [esi+0x6c]=edi)
    // +0x70  int   m_OwnerData /* ? */        (0x412f30: [esi+0x70]=0)
    // +0x74  int   m_Rate /* ? */             (0x412f30: [esi+0x74]=0x3e8)
    // +0x78  int   m_Delay /* ? */            (0x412f30: [esi+0x78]=edx)
    // +0x7c  int   m_Flags /* ? */            (0x412f30: [esi+0x7c]=0)
    // +0x8c  int   m_LoopDelay /* ? */        (0x412f30: [esi+0x8c]=0)
    // +0x98  int   m_StartSound /* ? */       (0x412f30: [esi+0x98]=0)
    // +0x9c  int   m_BounceSound /* ? */      (0x412f30: [esi+0x9c]=0)
    // +0xa0  int   m_ExpireSound /* ? */      (0x412f30: [esi+0xa0]=0)
    // +0xa4  int   m_ExpireAnim /* ? */       (0x412f30: [esi+0xa4]=0)
    // +0xa8  int   m_TrailerAnim /* ? */      (0x412f30: [esi+0xa8]=0)
    // +0xac  int   m_TrailerData /* ? */      (0x412f30: [esi+0xac]=0)
    // +0xec  int   m_Tiberium /* ? */         (0x412f30: [esi+0xec]=edx)
    // +0xf0  int   m_TiberiumData /* ? */     (0x412f30: [esi+0xf0]=0)
    // +0xf4  float m_Elasticity /* ? */       (0x412f30: [esi+0xf4]=0x3ff00000 = 1.0f)
    // +0xf8  int   m_Bounce /* ? */           (0x412f30: [esi+0xf8]=eax)
    // +0x144 int   m_NextAnim /* ? */         (0x412f30: [eax+0x144]=ecx)
    //
    // The stream ctor 0x415860 clears +0x70; the copy ctor 0x413700 clears
    // +0x64 and +0x68.

    // ---- data members (offsets from ctor 0x00412f30) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    // vtable-pointer stores are omitted: the compiler supplies the vptr.
    // The machine block begins at +0x50: the compiled end of the
    // base sub-object region. Evidence: ObjectClass at +0x0 (ends +0x4c), StageClass at +0x4c (ends +0x50).
    // 3 recovered offset(s) below +0x50 lie inside the base
    // sub-object region and are NOT declared here: they are declared and
    // compiler-checked in the base's own header -- +0x0, +0x35, +0x4c
    unsigned int m_050;  // +0x050 /* ? */; 4-byte store: mov dword ptr [esi + 0x50], eax
    unsigned char pad_054[0x4];
    unsigned int m_058;  // +0x058 /* ? */; 4-byte store: mov dword ptr [esi + 0x58], ebx
    unsigned int m_05c;  // +0x05c /* ? */; 4-byte store: mov dword ptr [esi + 0x5c], ebx
    unsigned int m_060;  // +0x060 init 0x1; 4-byte store: mov dword ptr [esi + 0x60], 1
    unsigned int m_064;  // +0x064 /* ? */; 4-byte store: mov dword ptr [esi + 0x64], ecx
    unsigned int m_068;  // +0x068 /* ? */; 4-byte store: mov dword ptr [esi + 0x68], ebx
    unsigned int m_06c;  // +0x06c /* ? */; 4-byte store: mov dword ptr [esi + 0x6c], edi
    unsigned int m_070;  // +0x070 /* ? */; 4-byte store: mov dword ptr [esi + 0x70], ebx
    unsigned int m_074;  // +0x074 init 0x3e8; 4-byte store: mov dword ptr [esi + 0x74], 0x3e8
    unsigned int m_078;  // +0x078 /* ? */; 4-byte store: mov dword ptr [esi + 0x78], edx
    unsigned int m_07c;  // +0x07c /* ? */; 4-byte store: mov dword ptr [esi + 0x7c], ebx
    unsigned char pad_080[0xc];
    unsigned int m_08c;  // +0x08c /* ? */; 4-byte store: mov dword ptr [esi + 0x8c], ebx
    unsigned char m_090;  // +0x090 /* ? */; 1-byte store: mov byte ptr [esi + 0x90], bl
    unsigned char pad_091[0x7];
    unsigned int m_098;  // +0x098 /* ? */; 4-byte store: mov dword ptr [esi + 0x98], ebx
    unsigned int m_09c;  // +0x09c /* ? */; 4-byte store: mov dword ptr [esi + 0x9c], ebx
    unsigned int m_0a0;  // +0x0a0 /* ? */; 4-byte store: mov dword ptr [esi + 0xa0], ebx
    unsigned int m_0a4;  // +0x0a4 /* ? */; 4-byte store: mov dword ptr [esi + 0xa4], ebx
    unsigned int m_0a8;  // +0x0a8 /* ? */; 4-byte store: mov dword ptr [esi + 0xa8], ebx
    unsigned int m_0ac;  // +0x0ac /* ? */; 4-byte store: mov dword ptr [esi + 0xac], ebx
    unsigned char pad_0b0[0x38];
    unsigned char m_0e8;  // +0x0e8 /* ? */; 1-byte store: mov byte ptr [esi + 0xe8], bl
    unsigned char pad_0e9[0x3];
    unsigned int m_0ec;  // +0x0ec /* ? */; 4-byte store: mov dword ptr [esi + 0xec], edx
    unsigned int m_0f0;  // +0x0f0 /* ? */; 4-byte store: mov dword ptr [esi + 0xf0], ebx
    unsigned int m_0f4;  // +0x0f4 init 0x3ff00000; 4-byte store: mov dword ptr [esi + 0xf4], 0x3ff00000
    unsigned int m_0f8;  // +0x0f8 /* ? */; 4-byte store: mov dword ptr [esi + 0xf8], eax
    unsigned char m_0fc;  // +0x0fc /* ? */; 1-byte store: mov byte ptr [esi + 0xfc], bl
    unsigned char m_0fd;  // +0x0fd init 0x1; 1-byte store: mov byte ptr [esi + 0xfd], 1
    unsigned char m_0fe;  // +0x0fe /* ? */; 1-byte store: mov byte ptr [esi + 0xfe], bl
    unsigned char m_0ff;  // +0x0ff /* ? */; 1-byte store: mov byte ptr [esi + 0xff], bl
    unsigned char m_100;  // +0x100 /* ? */; 1-byte store: mov byte ptr [esi + 0x100], bl
    unsigned char m_101;  // +0x101 /* ? */; 1-byte store: mov byte ptr [esi + 0x101], bl
    unsigned char m_102;  // +0x102 /* ? */; 1-byte store: mov byte ptr [esi + 0x102], bl
    unsigned char m_103;  // +0x103 /* ? */; 1-byte store: mov byte ptr [esi + 0x103], bl
    unsigned char m_104;  // +0x104 init 0x1; 1-byte store: mov byte ptr [esi + 0x104], 1
    unsigned char m_105;  // +0x105 /* ? */; 1-byte store: mov byte ptr [esi + 0x105], bl
    unsigned char m_106;  // +0x106 /* ? */; 1-byte store: mov byte ptr [esi + 0x106], bl

};
