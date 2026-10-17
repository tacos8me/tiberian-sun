#pragma once
// ============================================================================
// TechnoClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// Evidence:
//   RTTI class descriptor  : 0x6b6ce0
//   TypeDescriptor name    : 0x6bf590 + 8 -> ".?AVTechnoClass@@"
//   vtable (primary)       : 0x6a57e0   (COL ptr slot 0x6b6cf0)
//   vtable (IRTTITypeInfo) : 0x6a57c4   (COL ptr slot 0x6b6c90, mdisp=4)
//   constructor            : 0x60e7b0   (writes vtbl 0x6a57e0 / 0x6a57c4)
//   RTTI base list         : RadioClass (mdisp 0), MissionClass (mdisp 0),
//                            ObjectClass (mdisp 0), FlasherClass (mdisp 124),
//                            StageClass (mdisp 132)
//
// TechnoClass is the base for all armed/mobile objects (BuildingClass,
// UnitClass, InfantryClass, AircraftClass, VesselClass).
// ============================================================================

#include "ObjectClass.h"
#include "StageClass.h"

// ---------------------------------------------------------------------------
// RadioClass / MissionClass — intermediate bases of TechnoClass.
// Evidence: RTTI base list of TechnoClass (classes.json, chd_va 0x6b6ce0):
//   RadioClass (mdisp 0), MissionClass (mdisp 0), ObjectClass (mdisp 0).
// Declared here because their own headers are outside this task's file set.
// ---------------------------------------------------------------------------
class MissionClass : public ObjectClass
{
public:
    // Slot 0 is a stub shared with 65 other classes (0x405790), not
    // the destructor, so the destructor is declared without claiming a
    // vtable position and slot 0 is named for what it is. Declaring
    // `virtual ~X()` here made the destructor look like slot 0.
    ~MissionClass();
    virtual void  vfunc_0();                        // [0] 0x405790 (shared stub, 66 classes)
    virtual void Init();                                            // [1] 0x405830
    virtual void Shutdown();                                        // [2] 0x405840

    // ---- data members (offsets from ctor 0x00546710) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    // vtable-pointer stores are omitted: the compiler supplies the vptr.
    // The machine block begins at +0x4c: the compiled end of the
    // base sub-object region. Evidence: ObjectClass at +0x0 (ends +0x4c).
    // 2 recovered offset(s) below +0x4c lie inside the base
    // sub-object region and are NOT declared here: they are declared and
    // compiler-checked in the base's own header -- +0x0, +0x4
    unsigned int m_04c;  // +0x04c /* ? */; 4-byte store: mov dword ptr [esi + 0x4c], eax
    unsigned int m_050;  // +0x050 /* ? */; 4-byte store: mov dword ptr [esi + 0x50], eax
    unsigned int m_054;  // +0x054 /* ? */; 4-byte store: mov dword ptr [esi + 0x54], eax
    unsigned int m_058;  // +0x058 /* ? */; 4-byte store: mov dword ptr [esi + 0x58], eax
    unsigned char m_05c;  // +0x05c /* ? */; 1-byte store: mov byte ptr [esi + 0x5c], al
    unsigned char pad_05d[0x3];
    unsigned int m_060;  // +0x060 /* ? */; 4-byte store: mov dword ptr [esi + 0x60], ecx
    unsigned char pad_064[0x4];
    unsigned int m_068;  // +0x068 /* ? */; 4-byte store: mov dword ptr [esi + 0x68], eax

};

class RadioClass : public MissionClass
{
public:
    // Slot 0 is a stub shared with 65 other classes (0x405790), not
    // the destructor, so the destructor is declared without claiming a
    // vtable position and slot 0 is named for what it is. Declaring
    // `virtual ~X()` here made the destructor look like slot 0.
    ~RadioClass();
    virtual void  vfunc_0();                        // [0] 0x405790 (shared stub, 66 classes)
    virtual void Init();                                            // [1] 0x405830
    virtual void Shutdown();                                        // [2] 0x405840

    // ---- data members (offsets from ctor 0x005a5710) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    // vtable-pointer stores are omitted: the compiler supplies the vptr.
    // The machine block begins at +0x6c: the compiled end of the
    // base sub-object region. Evidence: MissionClass at +0x0 (ends +0x6c).
    // 2 recovered offset(s) below +0x6c lie inside the base
    // sub-object region and are NOT declared here: they are declared and
    // compiler-checked in the base's own header -- +0x0, +0x4
    unsigned int m_06c;  // +0x06c /* ? */; 4-byte store: mov dword ptr [esi + 0x6c], eax
    unsigned int m_070;  // +0x070 /* ? */; 4-byte store: mov dword ptr [esi + 0x70], eax
    unsigned int m_074;  // +0x074 /* ? */; 4-byte store: mov dword ptr [esi + 0x74], eax
    unsigned int m_078;  // +0x078 zero-initialised; 4-byte store: mov dword ptr [esi + 0x78], 0

};

// ---------------------------------------------------------------------------
// FlasherClass — mix-in base embedded at a fixed displacement.
// Evidence: RTTI base entry "FlasherClass (mdisp=124)" in TechnoClass's base
// list. Declared here because it has no header of its own yet.
// StageClass (mdisp=132) is the other mix-in base; it now lives in
// StageClass.h, because AnimClass, TerrainClass and FactoryClass also derive
// from it and could not previously name it.
// ---------------------------------------------------------------------------
class FlasherClass
{
public:
    virtual ~FlasherClass();                                        // [0] 0x61e510
    virtual void vfunc_1();                                         // [1] 0x61e2b0
    virtual void vfunc_2();                                         // [2] 0x61e2f0
    virtual void vfunc_3();                                         // [3] 0x61e3a0
    virtual void vfunc_4();                                         // [4] 0x61e3d0
    virtual void vfunc_5();                                         // [5] 0x61e410
};

// StageClass now has its own header (src/handwritten/StageClass.h) because
// other classes — AnimClass, TerrainClass, FactoryClass — also derive from it
// and previously could not name it. Included above.

class TechnoClass : public RadioClass, public FlasherClass, public StageClass
{
public:
    // ---- vtable 0x6a57e0 (primary, 228 entries) ---------------------------
    // Slot 6 is the RTTI CompleteObjectLocator pointer (0x6b6cf0), not code.
    // Slot 0 is a stub shared with 65 other classes (0x405790), not
    // the destructor, so the destructor is declared without claiming a
    // vtable position and slot 0 is named for what it is. Declaring
    // `virtual ~X()` here made the destructor look like slot 0.
    ~TechnoClass();
    virtual void  vfunc_0();                        // [0] 0x405790 (shared stub, 66 classes)
    virtual void  Init();                                           // [1] 0x405830
    virtual void  Shutdown();                                       // [2] 0x405840
    virtual long  QueryInterface(const void*, void**);              // [3] 0x6829a5 (stub)
    virtual unsigned long AddRef();                                 // [4] 0x405990
    virtual void  vfunc_5();                                        // [5] 0x61cc30
    virtual void  vfunc_6();                                        // [6] 0x6829a5 (stub)
    virtual int   GetType();                                        // [7] 0x405910
    virtual void  vfunc_8();                                        // [8] 0x61e1d0
    virtual void  vfunc_9();                                        // [9] 0x60f180
    virtual void  vfunc_10();                                       // [10] 0x61a290
    virtual long  GetClassID(void* pClassID);                       // [11] 0x6829a5 (stub)
    virtual long  IsDirty();                                        // [12] 0x6829a5 (stub)
    virtual long  Load(IStream* pStm);                              // [13] 0x61ccf0
    virtual long  Save(IStream* pStm, int fClearDirty);             // [14] 0x612db0
    virtual long  GetSizeMax(unsigned long* pcbSize);               // [15] 0x612dc0
    virtual void  Compute_CRC();                                    // [16] 0x4059f0
    virtual void  vfunc_17();                                       // [17] 0x570a20
    virtual void  vfunc_18();                                       // [18] 0x570960
    virtual void  vfunc_19();                                       // [19] 0x405a30
    virtual void  vfunc_20();                                       // [20] 0x570e80
    virtual void  vfunc_21();                                       // [21] 0x570ea0
    virtual void  vfunc_22();                                       // [22] 0x405a80
    virtual void  vfunc_23();                                       // [23] 0x612e10
    virtual void  vfunc_24();                                       // [24] 0x60eb70
    virtual void  vfunc_25();                                       // [25] 0x617cf0
    virtual void  vfunc_26();                                       // [26] 0x56e820
    virtual void  vfunc_27();                                       // [27] 0x615c40
    virtual void  vfunc_28();                                       // [28] 0x6158d0
    virtual void  vfunc_29();                                       // [29] 0x56eb60
    virtual void  vfunc_30();                                       // [30] 0x570ef0
    virtual void  vfunc_31();                                       // [31] 0x4167d0
    virtual void  vfunc_32();                                       // [32] 0x60eb10
    virtual void  vfunc_33();                                       // [33] 0x4a4b00
    virtual void  vfunc_34();                                       // [34] 0x61b1b0
    virtual void  vfunc_35();                                       // [35] 0x4167e0
    virtual void  vfunc_36();                                       // [36] 0x616470
    virtual void  vfunc_37();                                       // [37] 0x56ebc0
    virtual void  vfunc_38();                                       // [38] 0x616410
    virtual void  vfunc_39();                                       // [39] 0x616130
    virtual void  vfunc_40();                                       // [40] 0x40e5d0
    virtual void  vfunc_41();                                       // [41] 0x40e600
    virtual void  vfunc_42();                                       // [42] 0x40e630
    virtual void  vfunc_43();                                       // [43] 0x60edd0
    virtual void  vfunc_44();                                       // [44] 0x40e660
    virtual void  vfunc_45();                                       // [45] 0x570ed0
    virtual void  vfunc_46();                                       // [46] 0x570db0
    virtual void  vfunc_47();                                       // [47] 0x416830
    virtual void  vfunc_48();                                       // [48] 0x611220
    virtual void  vfunc_49();                                       // [49] 0x6112c0
    virtual void  vfunc_50();                                       // [50] 0x56fb40
    virtual void  vfunc_51();                                       // [51] 0x617760
    virtual void  vfunc_52();                                       // [52] 0x5700c0
    virtual void  vfunc_53();                                       // [53] 0x56ea70
    virtual void  vfunc_54();                                       // [54] 0x570480
    virtual void  vfunc_55();                                       // [55] 0x570500
    virtual void  vfunc_56();                                       // [56] 0x5709b0
    virtual void  vfunc_57();                                       // [57] 0x617ce0
    virtual void  vfunc_58();                                       // [58] 0x61b480
    virtual void  vfunc_59();                                       // [59] 0x56f480
    virtual void  vfunc_60();                                       // [60] 0x5701b0
    virtual void  vfunc_61();                                       // [61] 0x610880
    virtual void  vfunc_62();                                       // [62] 0x60fb90
    virtual void  vfunc_63();                                       // [63] 0x547120
    // Slots 64..227 are further virtuals; many targets were not recovered as
    // standalone function records by the disassembler.  /* ? */

    // ---- data members -----------------------------------------------------
    // Evidence: constructor 0x60e7b0 writes these offsets.
    //<UNCOMMENT>    int   m_TechnoType;         // +0x7c  ctor: mov dword ptr [esi+0x7c], ebx (0)  // commented out: it documents 0x7c, which C++ supplies itself (the compiler's own vptr or base sub-object is already there)
    //<UNCOMMENT>    bool  m_IsSelected;         // +0x80  ctor: mov byte ptr [esi+0x80], bl (0)  // commented out: it documents 0x80, which C++ supplies itself (the compiler's own vptr or base sub-object is already there)
    // NOT m_Owner: ObjectClass already declares an m_Owner at +0x28 and the
    // nearest declaration wins for name lookup, so a second m_Owner here
    // cannot be reached by offsetof on any class derived from both.
    int   m_Field84;            // +0x84  ctor: mov dword ptr [esi+0x84], ebx (0)
    int   m_Field88;            // +0x88  ctor: mov dword ptr [esi+0x88], eax
    unsigned char pad_doc_gap_08c[0x4];  // holds the gap before m_Field90, which documents 0x90 (the compiler would put it at 0x8c)
    int   m_Field90;            // +0x90  ctor: mov dword ptr [esi+0x90], ebx (0)
    int   m_Field94;            // +0x94  ctor: mov dword ptr [esi+0x94], ebx (0)
    int   m_Field98;            // +0x98  ctor: mov dword ptr [esi+0x98], 1
    int   m_Field9c;            // +0x9c  ctor: mov dword ptr [esi+0x9c], ebp
    int   m_FieldA0;            // +0xa0  ctor: mov dword ptr [esi+0xa0], ebx (0)
    int   m_FieldA4;            // +0xa4  ctor: mov dword ptr [esi+0xa4], ebx (0)
    unsigned char pad_doc_gap_0a8[0x8];  // holds the gap before m_FieldB0, which documents 0xb0 (the compiler would put it at 0xa8)
    int   m_FieldB0;            // +0xb0  ctor: mov dword ptr [esi+0xb0], ebx (0)
    int   m_FieldB4;            // +0xb4  ctor: mov dword ptr [esi+0xb4], eax
    int   m_FieldB8;            // +0xb8  ctor: mov dword ptr [esi+0xb8], ebx (0)
    int   m_FieldBc;            // +0xbc  ctor: mov dword ptr [esi+0xbc], eax
    int   m_FieldC0;            // +0xc0  ctor: mov dword ptr [esi+0xc0], ecx
    unsigned char pad_doc_gap_0c4[0x4];  // holds the gap before m_FieldC8, which documents 0xc8 (the compiler would put it at 0xc4)
    int   m_FieldC8;            // +0xc8  ctor: mov dword ptr [esi+0xc8], ebx (0)
    int   m_FieldCc;            // +0xcc  ctor: mov dword ptr [esi+0xcc], edx
    unsigned char pad_doc_gap_0d0[0x4];  // holds the gap before m_FieldD4, which documents 0xd4 (the compiler would put it at 0xd0)
    int   m_FieldD4;            // +0xd4  ctor: mov dword ptr [esi+0xd4], ebx (0)
    int   m_FieldD8;            // +0xd8  ctor: mov dword ptr [esi+0xd8], ebx (0)
    int   m_FieldDc;            // +0xdc  ctor: mov dword ptr [esi+0xdc], ebx (0)
    int   m_FieldE0;            // +0xe0  ctor: mov dword ptr [esi+0xe0], ebx (0)
    int   m_FieldE4;            // +0xe4  ctor: mov dword ptr [esi+0xe4], ebp
    int   m_FieldE8;            // +0xe8  ctor: mov dword ptr [esi+0xe8], ebx (0)
    int   m_FieldEc;            // +0xec  ctor: mov dword ptr [esi+0xec], edi
    int   m_FieldF0;            // +0xf0  ctor: mov dword ptr [esi+0xf0], ebx (0)
    int   m_FieldF4;            // +0xf4  ctor: mov dword ptr [esi+0xf4], ebx (0)
    int   m_FieldF8;            // +0xf8  ctor: mov dword ptr [esi+0xf8], eax
    unsigned char pad_doc_gap_0fc[0x4];  // holds the gap before m_Field100, which documents 0x100 (the compiler would put it at 0xfc)
    int   m_Field100;           // +0x100 ctor: mov dword ptr [esi+0x100], ebx (0)
    int   m_Field104;           // +0x104 ctor: mov dword ptr [esi+0x104], ebx (0)
    int   m_Field108;           // +0x108 ctor: mov dword ptr [esi+0x108], 1
    int   m_Field10c;           // +0x10c ctor: mov dword ptr [esi+0x10c], ecx
    unsigned char pad_doc_gap_110[0x4];  // holds the gap before m_Field114, which documents 0x114 (the compiler would put it at 0x110)
    int   m_Field114;           // +0x114 ctor: mov dword ptr [esi+0x114], ebx (0)
    int   m_Field118;           // +0x118 ctor: mov dword ptr [esi+0x118], ebx (0)
    int   m_Field11c;           // +0x11c ctor: mov dword ptr [esi+0x11c], ebx (0)
    int   m_Field120;           // +0x120 ctor: mov dword ptr [esi+0x120], ebx (0)
    int   m_Field124;           // +0x124 ctor: mov dword ptr [esi+0x124], ebx (0)
    int   m_Field128;           // +0x128 ctor: mov dword ptr [esi+0x128], edx
    unsigned char pad_doc_gap_12c[0x4];  // holds the gap before m_Field130, which documents 0x130 (the compiler would put it at 0x12c)
    int   m_Field130;           // +0x130 ctor: mov dword ptr [esi+0x130], ebx (0)
    int   m_Field134;           // +0x134 ctor: mov dword ptr [esi+0x134], ebp
    int   m_Field138;           // +0x138 ctor: mov dword ptr [esi+0x138], ebx (0)
    int   m_Field13c;           // +0x13c ctor: mov dword ptr [esi+0x13c], edx
    int   m_Field140;           // +0x140 ctor: mov dword ptr [esi+0x140], edx
    int   m_Field144;           // +0x144 ctor: mov dword ptr [esi+0x144], edx
    int   m_Field148;           // +0x148 ctor: mov dword ptr [esi+0x148], edx
    int   m_Field14c;           // +0x14c ctor: mov dword ptr [esi+0x14c], edx
    int   m_Field150;           // +0x150 ctor: mov dword ptr [esi+0x150], ebx (0)
    int   m_Field154;           // +0x154 ctor: mov dword ptr [esi+0x154], ebx (0)
    int   m_Field158;           // +0x158 ctor: mov dword ptr [esi+0x158], ebx (0)
    int   m_Field15c;           // +0x15c ctor: mov dword ptr [esi+0x15c], ebx (0)
    int   m_Field160;           // +0x160 ctor: mov dword ptr [esi+0x160], ebx (0)
    int   m_Field164;           // +0x164 ctor: mov dword ptr [esi+0x164], ebp
    // +0x168..+0x1e0: embedded sub-objects (lea ecx,[esi+0x168], +0x178,
    //   +0x198, +0x1b0, +0x1c8) constructed in place.  /* ? */
    unsigned char pad_doc_gap_168[0x78];  // holds the gap before m_Field1e0, which documents 0x1e0 (the compiler would put it at 0x168)
    int   m_Field1e0;           // +0x1e0 ctor: mov dword ptr [esi+0x1e0], ebx (0)
    int   m_Field1e4;           // +0x1e4 ctor: mov dword ptr [esi+0x1e4], eax
    unsigned char pad_doc_gap_1e8[0x4];  // holds the gap before m_Field1ec, which documents 0x1ec (the compiler would put it at 0x1e8)
    int   m_Field1ec;           // +0x1ec ctor: mov dword ptr [esi+0x1ec], ebx (0)
    short m_Field1f0;           // +0x1f0 ctor: mov word ptr [esi+0x1f0], ax
    short m_Field1f2;           // +0x1f2 ctor: mov word ptr [esi+0x1f2], bx (0)
    bool  m_Field1f4;           // +0x1f4 ctor: mov byte ptr [esi+0x1f4], bl (0)
    bool  m_Field1f5;           // +0x1f5 ctor: mov byte ptr [esi+0x1f5], bl (0)
    bool  m_Field1f6;           // +0x1f6 ctor: mov byte ptr [esi+0x1f6], bl (0)
    bool  m_Field1f7;           // +0x1f7 ctor: mov byte ptr [esi+0x1f7], bl (0)
    bool  m_Field1f8;           // +0x1f8 ctor: mov byte ptr [esi+0x1f8], bl (0)
    bool  m_Field1f9;           // +0x1f9 ctor: mov byte ptr [esi+0x1f9], bl (0)
    bool  m_Field1fa;           // +0x1fa ctor: mov byte ptr [esi+0x1fa], bl (0)
    bool  m_Field1fb;           // +0x1fb ctor: mov byte ptr [esi+0x1fb], bl (0)
    bool  m_Field1fc;           // +0x1fc ctor: mov byte ptr [esi+0x1fc], bl (0)
    bool  m_Field1fd;           // +0x1fd ctor: mov byte ptr [esi+0x1fd], bl (0)
    bool  m_Field1fe;           // +0x1fe ctor: mov byte ptr [esi+0x1fe], bl (0)
    bool  m_Field1ff;           // +0x1ff ctor: mov byte ptr [esi+0x1ff], bl (0)
    bool  m_Field200;           // +0x200 ctor: mov byte ptr [esi+0x200], bl (0)
    bool  m_Field201;           // +0x201 ctor: mov byte ptr [esi+0x201], bl (0)
    bool  m_Field202;           // +0x202 ctor: mov byte ptr [esi+0x202], bl (0)
    bool  m_Field203;           // +0x203 ctor: mov byte ptr [esi+0x203], bl (0)
    bool  m_Field204;           // +0x204 ctor: mov byte ptr [esi+0x204], bl (0)
    bool  m_Field205;           // +0x205 ctor: mov byte ptr [esi+0x205], 1
    bool  m_Field206;           // +0x206 ctor: mov byte ptr [esi+0x206], 1
    bool  m_Field207;           // +0x207 ctor: mov byte ptr [esi+0x207], bl (0)
    bool  m_Field208;           // +0x208 ctor: mov byte ptr [esi+0x208], bl (0)
    bool  m_Field209;           // +0x209 ctor: mov byte ptr [esi+0x209], bl (0)
    bool  m_Field20a;           // +0x20a ctor: mov byte ptr [esi+0x20a], bl (0)
    bool  m_Field20b;           // +0x20b ctor: mov byte ptr [esi+0x20b], bl (0)
    int   m_Field20c;           // +0x20c ctor: mov dword ptr [esi+0x20c], ebx (0)
    int   m_Field210;           // +0x210 ctor: mov dword ptr [esi+0x210], ebx (0)

    // ---- data members (offsets from ctor 0x0060e7b0) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    // vtable-pointer stores are omitted: the compiler supplies the vptr.
    // The machine block begins at +0x214: the compiled end of the
    // base sub-object region (+0x84) and of the curated block
    // above. Base evidence: RadioClass at +0x0 (ends +0x7c), FlasherClass at +0x7c (ends +0x80), StageClass at +0x80 (ends +0x84).
    // 4 recovered offset(s) below +0x84 lie inside the base
    // sub-object region and are NOT declared here: they are declared and
    // compiler-checked in the base's own header -- +0x0, +0x4, +0x7c, +0x80
    // 80 recovered offset(s) are inside the curated hand-written block
    // above (its span is +0x84..+0x214) and are NOT repeated here:
    // the curated block wins -- +0x84, +0x88, +0x90, +0x94, +0x98, +0x9c, +0xa0, +0xa4 ...

};
