#pragma once
// ==========================================================================
// BuildingClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class BuildingClass
// vtable       : 0x0069a2d0  (221 slots)
// constructor  : 0x00425569
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "TechnoClass.h"

class BuildingClass : public TechnoClass
{
public:
    // ---- vtable 0x0069a2d0 (221 slots) ----
    virtual void QueryInterface(int a0, int a1, int a2);  // [0] 0x00405790  // __stdcall, returns int
    virtual void Init(int a0);  // [1] 0x00405830  // __stdcall, returns int
    virtual void Shutdown(int a0);  // [2] 0x00405840  // __stdcall, returns int
    virtual void vfunc_3(int a0, int a1);  // [3] 0x0043a180  // __stdcall, returns int
    virtual void Release(int a0);  // [4] 0x00405990  // __stdcall
    virtual void vfunc_5(int a0, int a1);  // [5] 0x00436cd0  // __stdcall
    virtual void vfunc_6(int a0, int a1, int a2);  // [6] 0x00436e80  // __stdcall, returns int
    virtual void GetType(int a0, int a1);  // [7] 0x00405910  // __stdcall, returns int
    virtual void vfunc_8(int a0);  // [8] 0x0043a230  // __thiscall
    virtual void vfunc_9();  // [9] 0x0042adc0  // __thiscall
    virtual void vfunc_10(int a0, int a1);  // [10] 0x00432dd0  // __thiscall
    virtual void vfunc_11();  // [11] 0x0043a1c0  // __cdecl, returns int
    virtual void vfunc_12();  // [12] 0x0043a170  // __cdecl, returns int
    virtual void vfunc_13(int a0);  // [13] 0x00436eb0  // __thiscall
    virtual void vfunc_14();  // [14] 0x00612db0  // __cdecl
    virtual void vfunc_15();  // [15] 0x00612dc0  // __cdecl
    virtual void Compute_CRC();  // [16] 0x004059f0  // __cdecl, returns int
    virtual void vfunc_17();  // [17] 0x00570a20  // __cdecl, returns int
    virtual void vfunc_18(int a0);  // [18] 0x0042e3e0  // __thiscall
    virtual void vfunc_19(int a0);  // [19] 0x0042e540
    virtual void vfunc_20();  // [20] 0x00570e80  // __cdecl
    virtual void vfunc_21();  // [21] 0x00570ea0  // __cdecl
    virtual void vfunc_22(int a0);  // [22] 0x00405a80  // __stdcall
    virtual void vfunc_23(int a0);  // [23] 0x00428c30  // __thiscall
    virtual void vfunc_24();  // [24] 0x0060eb70  // __thiscall
    virtual void vfunc_25(int a0, int a1);  // [25] 0x004370c0  // __stdcall, returns int
    virtual void vfunc_26(int a0, int a1);  // [26] 0x00434c20  // __thiscall
    virtual void vfunc_27(int a0, int a1, int a2);  // [27] 0x0042e030  // __thiscall, returns int
    virtual void vfunc_28(int a0, int a1);  // [28] 0x0042dd30  // __thiscall, returns int
    virtual void vfunc_29();  // [29] 0x0056eb60  // __cdecl
    virtual void vfunc_30();  // [30] 0x00570ef0  // __cdecl
    virtual void vfunc_31();  // [31] 0x00439a90  // __cdecl
    virtual void vfunc_32();  // [32] 0x0060eb10  // __thiscall
    virtual void vfunc_33();  // [33] 0x0043a1e0  // __cdecl
    virtual void vfunc_34();  // [34] 0x0061b1b0  // __cdecl
    virtual void vfunc_35();  // [35] 0x0043a1d0  // __cdecl
    virtual void vfunc_36();  // [36] 0x004359c0  // __thiscall
    virtual void vfunc_37();  // [37] 0x0042f030  // __thiscall
    virtual void vfunc_38();  // [38] 0x00616410  // __thiscall
    virtual void vfunc_39();  // [39] 0x00433440  // __thiscall
    virtual void vfunc_40(int a0);  // [40] 0x00433ef0  // __stdcall
    virtual void vfunc_41(int a0);  // [41] 0x0042e450  // __thiscall
    virtual void vfunc_42(int a0);  // [42] 0x0043a1f0  // __stdcall
    virtual void vfunc_43(int a0, int a1);  // [43] 0x00436a30  // __thiscall
    virtual void vfunc_44(int a0);  // [44] 0x004334c0  // __thiscall
    virtual void vfunc_45();  // [45] 0x0042ef80  // __thiscall
    virtual void vfunc_46(int a0, int a1, int a2, int a3);  // [46] 0x00570db0  // __thiscall
    virtual void vfunc_47();  // [47] 0x00416830  // __cdecl
    virtual void vfunc_48(int a0, int a1, int a2, int a3);  // [48] 0x0042cca0  // __thiscall
    virtual void vfunc_49(int a0);  // [49] 0x00429000  // __thiscall
    virtual void vfunc_50(int a0);  // [50] 0x00432f20  // __thiscall
    virtual void vfunc_51(int a0);  // [51] 0x00617760  // __thiscall
    virtual void vfunc_52(int a0);  // [52] 0x005700c0  // __thiscall
    virtual void vfunc_53(int a0);  // [53] 0x0056ea70  // __thiscall
    virtual void vfunc_54(int a0);  // [54] 0x00436c10  // __stdcall
    virtual void vfunc_55(int a0);  // [55] 0x00436c70  // __stdcall
    virtual void vfunc_56();  // [56] 0x005709b0  // __thiscall
    virtual void vfunc_57();  // [57] 0x00617ce0  // __cdecl
    virtual void vfunc_58(int a0);  // [58] 0x0042b790  // __thiscall
    virtual void vfunc_59(int a0, int a1, int a2);  // [59] 0x004265e0  // __thiscall
    virtual void vfunc_60(int a0);  // [60] 0x005701b0  // __thiscall, returns int
    virtual void vfunc_61(int a0, int a1, int a2, int a3, int a4);  // [61] 0x00610880  // __thiscall
    virtual void vfunc_62(int a0, int a1, int a2, int a3, int a4, int a5);  // [62] 0x0060fb90  // __thiscall
    virtual void vfunc_63(int a0, int a1);  // [63] 0x00426980  // __thiscall
    virtual void vfunc_64();  // [64] 0x00426760
    virtual void vfunc_65();  // [65] 0x0060f590
    virtual void vfunc_66();  // [66] 0x0061c1e0
    virtual void vfunc_67();  // [67] 0x00428540
    virtual void vfunc_68();  // [68] 0x0056f000
    virtual void vfunc_69();  // [69] 0x004386e0
    virtual void vfunc_70(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11, int a12, int a13, int a14, int a15, int a16, int a17);  // [70] 0x00439240  // __thiscall
    virtual void vfunc_71();  // [71] 0x0056f680
    virtual void vfunc_72();  // [72] 0x0042b410
    virtual void vfunc_73();  // [73] 0x0042b290
    virtual void vfunc_74();  // [74] 0x00439650
    virtual void vfunc_75();  // [75] 0x00613f10
    virtual void vfunc_76();  // [76] 0x0056eda0
    virtual void vfunc_77();  // [77] 0x00611540
    virtual void vfunc_78();  // [78] 0x006164c0
    virtual void vfunc_79();  // [79] 0x0042a510
    virtual void vfunc_80();  // [80] 0x0056ec90
    virtual void vfunc_81();  // [81] 0x0056eca0
    virtual void vfunc_82();  // [82] 0x0056ecb0
    virtual void vfunc_83();  // [83] 0x0061a4d0
    virtual void vfunc_84();  // [84] 0x005468d0
    virtual void vfunc_85();  // [85] 0x00546d60
    virtual void vfunc_86();  // [86] 0x0060faa0
    virtual void vfunc_87();  // [87] 0x00570240
    virtual void vfunc_88();  // [88] 0x00425c90
    virtual void vfunc_89();  // [89] 0x00432150
    virtual void vfunc_90();  // [90] 0x0042db60
    virtual void vfunc_91();  // [91] 0x0042dc50
    virtual void vfunc_92();  // [92] 0x00570e70
    virtual void vfunc_93();  // [93] 0x0056ed00
    virtual void vfunc_94();  // [94] 0x0042efb0
    virtual void vfunc_95();  // [95] 0x00416890
    virtual void vfunc_96();  // [96] 0x0040e6a0
    virtual void vfunc_97(int a0);  // [97] 0x00570cc0  // __stdcall
    virtual void vfunc_98();  // [98] 0x0040e6c0
    virtual void vfunc_99();  // [99] 0x00570ce0
    virtual void vfunc_100();  // [100] 0x00570d00
    virtual void vfunc_101();  // [101] 0x00570d50
    virtual void vfunc_102();  // [102] 0x00570370
    virtual void vfunc_103();  // [103] 0x005703b0
    virtual void vfunc_104();  // [104] 0x00570360
    virtual void vfunc_105();  // [105] 0x00546d20
    virtual void vfunc_106();  // [106] 0x00436ea0
    virtual void vfunc_107();  // [107] 0x00546750
    virtual void vfunc_108();  // [108] 0x00546760
    virtual void vfunc_109();  // [109] 0x00546770
    virtual void vfunc_110();  // [110] 0x00430290
    virtual void vfunc_111();  // [111] 0x00546790
    virtual void vfunc_112();  // [112] 0x0042f210
    virtual void vfunc_113();  // [113] 0x0042f570
    virtual void vfunc_114();  // [114] 0x004307a0
    virtual void vfunc_115();  // [115] 0x005467d0
    virtual void vfunc_116();  // [116] 0x005467e0
    virtual void vfunc_117();  // [117] 0x005467f0
    virtual void vfunc_118();  // [118] 0x00546800
    virtual void vfunc_119();  // [119] 0x00546810
    virtual void vfunc_120();  // [120] 0x00432460
    virtual void vfunc_121();  // [121] 0x00546830
    virtual void vfunc_122();  // [122] 0x0042f580
    virtual void vfunc_123();  // [123] 0x0042f720
    virtual void vfunc_124();  // [124] 0x004307b0
    virtual void vfunc_125();  // [125] 0x00431600  // __thiscall
    virtual void vfunc_126();  // [126] 0x004329e0
    virtual void vfunc_127();  // [127] 0x00546890
    virtual void vfunc_128();  // [128] 0x005468a0
    virtual void vfunc_129();  // [129] 0x005468b0
    virtual void vfunc_130();  // [130] 0x006164e0
    virtual void vfunc_131();  // [131] 0x00616520
    virtual void vfunc_132();  // [132] 0x00547110
    virtual void vfunc_133();  // [133] 0x005a58a0
    virtual void vfunc_134();  // [134] 0x005a57d0
    virtual void vfunc_135();  // [135] 0x00438860
    virtual void vfunc_136();  // [136] 0x0061cf80
    virtual void vfunc_137();  // [137] 0x0060eb20
    virtual void vfunc_138();  // [138] 0x0043a110
    virtual void vfunc_139();  // [139] 0x0061cb60
    virtual void vfunc_140();  // [140] 0x00612de0
    virtual void vfunc_141();  // [141] 0x0040e710
    virtual void vfunc_142();  // [142] 0x00439bf0
    virtual void vfunc_143();  // [143] 0x00439d40
    virtual void vfunc_144();  // [144] 0x0042d180
    virtual void vfunc_145();  // [145] 0x00616450
    virtual void vfunc_146();  // [146] 0x0061cfa0
    virtual void vfunc_147();  // [147] 0x0061b1e0
    virtual void vfunc_148();  // [148] 0x0061d000
    virtual void vfunc_149();  // [149] 0x00432280
    virtual void vfunc_150();  // [150] 0x0061bf60
    virtual void vfunc_151();  // [151] 0x0061b1c0
    virtual void vfunc_152();  // [152] 0x0043a120
    virtual void vfunc_153(int a0, int a1);  // [153] 0x00427d80  // __thiscall
    virtual void vfunc_154();  // [154] 0x0061a660
    virtual void vfunc_155();  // [155] 0x00434b80
    virtual void vfunc_156();  // [156] 0x00439b70
    virtual void vfunc_157();  // [157] 0x00439a20
    virtual void vfunc_158();  // [158] 0x00427d40
    virtual void vfunc_159();  // [159] 0x0043a130
    virtual void vfunc_160();  // [160] 0x0061bd30
    virtual void vfunc_161();  // [161] 0x00433060
    virtual void vfunc_162();  // [162] 0x00436b20
    virtual void vfunc_163();  // [163] 0x0061b1f0
    virtual void vfunc_164();  // [164] 0x004323b0
    virtual void vfunc_165();  // [165] 0x00432ed0
    virtual void vfunc_166();  // [166] 0x00616160
    virtual void vfunc_167();  // [167] 0x006161b0
    virtual void vfunc_168();  // [168] 0x006143a0
    virtual void vfunc_169();  // [169] 0x0061a560
    virtual void vfunc_170();  // [170] 0x0043a140
    virtual void vfunc_171();  // [171] 0x004397c0
    virtual void vfunc_172();  // [172] 0x0061db90
    virtual void vfunc_173();  // [173] 0x0040e720
    virtual void vfunc_174();  // [174] 0x0043a150
    virtual void vfunc_175();  // [175] 0x0061b2e0
    virtual void vfunc_176();  // [176] 0x0061b340
    virtual void vfunc_177();  // [177] 0x0061b3a0
    virtual void vfunc_178();  // [178] 0x00615820
    virtual void vfunc_179();  // [179] 0x0061dd50
    virtual void vfunc_180();  // [180] 0x0061b400
    virtual void vfunc_181();  // [181] 0x00614290
    virtual void vfunc_182();  // [182] 0x006114b0
    virtual void vfunc_183();  // [183] 0x00432340
    virtual void vfunc_184();  // [184] 0x0042e590
    virtual void vfunc_185();  // [185] 0x0042d230
    virtual void vfunc_186();  // [186] 0x0042b6e0
    virtual void vfunc_187(int a0, int a1, int a2, int a3, int a4, int a5, int a6);  // [187] 0x006148b0  // __thiscall
    virtual void vfunc_188();  // [188] 0x0042e7f0
    virtual void vfunc_189();  // [189] 0x00614530
    virtual void vfunc_190(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11);  // [190] 0x0061c360  // __thiscall
    virtual void vfunc_191();  // [191] 0x00435a70
    virtual void vfunc_192();  // [192] 0x00435b00
    virtual void vfunc_193();  // [193] 0x00616550
    virtual void vfunc_194(int a0, int a1, int a2, int a3, int a4, int a5, int a6);  // [194] 0x00437570  // __thiscall
    virtual void vfunc_195();  // [195] 0x0061c650
    virtual void vfunc_196();  // [196] 0x0060f8f0
    virtual void vfunc_197();  // [197] 0x0043a160
    virtual void vfunc_198();  // [198] 0x00619490
    virtual void vfunc_199();  // [199] 0x00610870
    virtual void vfunc_200();  // [200] 0x00610c50
    virtual void vfunc_201();  // [201] 0x0061b4f0
    virtual void vfunc_202();  // [202] 0x0061bc30
    virtual void vfunc_203();  // [203] 0x00617b90
    virtual void vfunc_204();  // [204] 0x00617c30
    virtual void vfunc_205();  // [205] 0x00439790
    virtual void vfunc_206();  // [206] 0x0061b490
    virtual void vfunc_207();  // [207] 0x0040e790
    virtual void vfunc_208();  // [208] 0x00438810
    virtual void vfunc_209();  // [209] 0x00432220
    virtual void vfunc_210();  // [210] 0x00439110
    virtual void vfunc_211();  // [211] 0x00439170
    virtual void vfunc_212();  // [212] 0x004391d0
    virtual void vfunc_213();  // [213] 0x0061df20
    virtual void vfunc_214();  // [214] 0x0042d2c0
    virtual void vfunc_215();  // [215] 0x0042caf0
    virtual void vfunc_216();  // [216] 0x00426f30
    virtual void vfunc_217();  // [217] 0x00428170
    virtual void vfunc_218();  // [218] 0x00429ce0
    virtual void vfunc_219();  // [219] 0x0042e720
    virtual void vfunc_220();  // [220] 0x004355e0

    BuildingClass();  // 0x00425569
    virtual ~BuildingClass();

    // ---- data members (offsets from ctor 0x0042564e) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69a2b4
};
