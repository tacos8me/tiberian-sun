#pragma once
// ==========================================================================
// InfantryClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class InfantryClass
// vtable       : 0x0069ff50  (237 slots)
// constructor  : 0x004cb520
// vtable store : 0x004cb599  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "FootClass.h"

class InfantryClass : public FootClass
{
public:
    // ---- vtable 0x0069ff50 (237 slots) ----
    virtual void QueryInterface(int a0, int a1, int a2);  // [0] 0x00405790  // __stdcall, returns int
    virtual void Init(int a0);  // [1] 0x00405830  // __stdcall, returns int
    virtual void Shutdown(int a0);  // [2] 0x00405840  // __stdcall, returns int
    virtual void vfunc_3(int a0, int a1);  // [3] 0x004d2e40  // __stdcall, returns int
    virtual void Release(int a0);  // [4] 0x00405990  // __stdcall
    virtual void vfunc_5(int a0, int a1);  // [5] 0x004d2460  // __stdcall
    virtual void vfunc_6(int a0, int a1, int a2);  // [6] 0x004d25d0  // __stdcall
    virtual void GetType(int a0, int a1);  // [7] 0x00405910  // __stdcall, returns int
    virtual void vfunc_8(int a0);  // [8] 0x004d2eb0  // __thiscall
    virtual void vfunc_9();  // [9] 0x004cb7c0  // __thiscall
    virtual void vfunc_10(int a0, int a1);  // [10] 0x004cd470  // __thiscall
    virtual void vfunc_11();  // [11] 0x004d2e80  // __cdecl, returns int
    virtual void vfunc_12();  // [12] 0x004d2e30  // __cdecl, returns int
    virtual void vfunc_13(int a0);  // [13] 0x004d2740  // __thiscall
    virtual void vfunc_14();  // [14] 0x00612db0  // __cdecl
    virtual void vfunc_15();  // [15] 0x00612dc0  // __cdecl
    virtual void Compute_CRC();  // [16] 0x004059f0  // __cdecl, returns int
    virtual void vfunc_17();  // [17] 0x00570a20  // __cdecl, returns int
    virtual void vfunc_18(int a0);  // [18] 0x00570960  // __stdcall
    virtual void vfunc_19(int a0);  // [19] 0x004a26e0  // __thiscall
    virtual void vfunc_20();  // [20] 0x00570e80  // __cdecl
    virtual void vfunc_21(int a0, int a1);  // [21] 0x004a4a30  // __thiscall
    virtual void vfunc_22(int a0);  // [22] 0x00405a80  // __stdcall
    virtual void vfunc_23();  // [23] 0x004ce250  // __thiscall
    virtual void vfunc_24();  // [24] 0x0060eb70  // __thiscall
    virtual void vfunc_25(int a0, int a1);  // [25] 0x004a1690  // __thiscall
    virtual void vfunc_26();  // [26] 0x004d2240  // __cdecl
    virtual void vfunc_27(int a0, int a1, int a2);  // [27] 0x004d0ae0  // __thiscall, returns int
    virtual void vfunc_28(int a0, int a1);  // [28] 0x004d0290  // __thiscall
    virtual void vfunc_29();  // [29] 0x004a2140  // __thiscall
    virtual void vfunc_30();  // [30] 0x00570ef0  // __cdecl
    virtual void vfunc_31();  // [31] 0x004167d0  // __cdecl
    virtual void vfunc_32();  // [32] 0x0060eb10  // __thiscall
    virtual void vfunc_33();  // [33] 0x004d0d40  // __cdecl
    virtual void vfunc_34();  // [34] 0x0061b1b0  // __cdecl
    virtual void vfunc_35();  // [35] 0x004d09d0  // __thiscall
    virtual void vfunc_36();  // [36] 0x00616470  // __thiscall
    virtual void vfunc_37(int a0, int a1, int a2, int a3, int a4);  // [37] 0x004a1080  // __thiscall
    virtual void vfunc_38();  // [38] 0x00616410  // __thiscall
    virtual void vfunc_39();  // [39] 0x00616130  // __thiscall
    virtual void vfunc_40(int a0);  // [40] 0x0040e5d0  // __stdcall
    virtual void vfunc_41(int a0);  // [41] 0x0040e600  // __stdcall
    virtual void vfunc_42(int a0);  // [42] 0x0040e630  // __stdcall
    virtual void vfunc_43(int a0, int a1);  // [43] 0x0060edd0  // __thiscall
    virtual void vfunc_44(int a0);  // [44] 0x0040e660  // __stdcall
    virtual void vfunc_45();  // [45] 0x00570ed0  // __cdecl
    virtual void vfunc_46();  // [46] 0x004a4120  // __cdecl
    virtual void vfunc_47();  // [47] 0x0040e7c0  // __cdecl
    virtual void vfunc_48();  // [48] 0x004cfe00  // __thiscall
    virtual void vfunc_49(int a0, int a1, int a2, int a3, int a4, int a5, int a6);  // [49] 0x004cfeb0  // __thiscall
    virtual void vfunc_50(int a0);  // [50] 0x004a0a00  // __thiscall
    virtual void vfunc_51(int a0);  // [51] 0x00617760  // __thiscall
    virtual void vfunc_52(int a0);  // [52] 0x004d22f0  // __thiscall
    virtual void vfunc_53(int a0);  // [53] 0x0056ea70  // __thiscall
    virtual void vfunc_54(int a0);  // [54] 0x004d2340  // __thiscall
    virtual void vfunc_55(int a0);  // [55] 0x004d23d0  // __stdcall
    virtual void vfunc_56();  // [56] 0x004a4a10  // __thiscall
    virtual void vfunc_57();  // [57] 0x00617ce0  // __cdecl
    virtual void vfunc_58(int a0);  // [58] 0x0061b480  // __stdcall, returns int
    virtual void vfunc_59(int a0, int a1, int a2);  // [59] 0x0056f480  // __thiscall
    virtual void vfunc_60(int a0);  // [60] 0x005701b0  // __thiscall, returns int
    virtual void vfunc_61(int a0, int a1, int a2, int a3, int a4);  // [61] 0x00610880  // __thiscall
    virtual void vfunc_62(int a0, int a1, int a2, int a3, int a4, int a5);  // [62] 0x0060fb90  // __thiscall
    virtual void vfunc_63(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11, int a12, int a13, int a14);  // [63] 0x004cc200  // __thiscall
    virtual void vfunc_64(int a0, int a1);  // [64] 0x00570990  // __stdcall
    virtual void vfunc_65();  // [65] 0x0060f590  // __thiscall
    virtual void vfunc_66(int a0, int a1);  // [66] 0x0061c1e0  // __thiscall
    virtual void vfunc_67(int a0);  // [67] 0x0049c590  // __thiscall
    virtual void vfunc_68(int a0);  // [68] 0x0056f000  // __thiscall
    virtual void vfunc_69(int a0);  // [69] 0x0056f160  // __thiscall
    virtual void vfunc_70();  // [70] 0x0040e690  // __cdecl
    virtual void vfunc_71();  // [71] 0x0056f680  // __cdecl
    virtual void vfunc_72(int a0, int a1, int a2);  // [72] 0x004d2e90  // __stdcall
    virtual void vfunc_73(int a0, int a1, int a2);  // [73] 0x004d0940  // __thiscall
    virtual void vfunc_74(int a0);  // [74] 0x00612dd0  // __stdcall
    virtual void vfunc_75();  // [75] 0x00613f10  // __thiscall
    virtual void vfunc_76(int a0);  // [76] 0x0056eda0  // __thiscall
    virtual void vfunc_77(int a0, int a1);  // [77] 0x00611540  // __thiscall
    virtual void vfunc_78(int a0);  // [78] 0x006164c0  // __stdcall
    virtual void vfunc_79(int a0, int a1, int a2, int a3, int a4, int a5);  // [79] 0x004cb9d0  // __thiscall
    virtual void vfunc_80(int a0);  // [80] 0x004cf3c0  // __thiscall
    virtual void vfunc_81();  // [81] 0x0056eca0  // __cdecl
    virtual void vfunc_82();  // [82] 0x0056ecb0  // __cdecl
    virtual void vfunc_83();  // [83] 0x0061a4d0  // __thiscall
    virtual void vfunc_84();  // [84] 0x005468d0  // __cdecl
    virtual void vfunc_85(int a0);  // [85] 0x00546d60  // __stdcall
    virtual void vfunc_86(int a0, int a1, int a2, int a3, int a4, int a5);  // [86] 0x004cc6f0  // __thiscall
    virtual void vfunc_87(int a0, int a1);  // [87] 0x00570240  // __thiscall
    virtual void vfunc_88(int a0, int a1);  // [88] 0x004a03a0  // __thiscall
    virtual void vfunc_89(int a0);  // [89] 0x0060f490  // __thiscall
    virtual void vfunc_90(int a0);  // [90] 0x0056ece0  // __stdcall
    virtual void vfunc_91(int a0);  // [91] 0x004a1160  // __thiscall
    virtual void vfunc_92(int a0);  // [92] 0x004a2cd0  // __thiscall
    virtual void vfunc_93(int a0);  // [93] 0x0056ed00  // __thiscall
    virtual void vfunc_94(int a0, int a1);  // [94] 0x004ce5b0  // __thiscall
    virtual void vfunc_95(int a0, int a1, int a2, int a3, int a4);  // [95] 0x004a0e70  // __stdcall, returns int
    virtual void vfunc_96(int a0);  // [96] 0x0040e6a0  // __stdcall
    virtual void vfunc_97(int a0);  // [97] 0x004a2170  // __thiscall
    virtual void vfunc_98(int a0);  // [98] 0x0040e6c0  // __stdcall
    virtual void vfunc_99();  // [99] 0x00570ce0  // __cdecl
    virtual void vfunc_100(int a0);  // [100] 0x00570d00  // __stdcall
    virtual void vfunc_101(int a0);  // [101] 0x00570d50  // __cdecl
    virtual void vfunc_102();  // [102] 0x00570370  // __thiscall
    virtual void vfunc_103(int a0);  // [103] 0x005703b0  // __thiscall
    virtual void vfunc_104();  // [104] 0x00570360  // __cdecl
    virtual void vfunc_105(int a0);  // [105] 0x00546d20  // __cdecl
    virtual void vfunc_106();  // [106] 0x004d2610  // __thiscall
    virtual void vfunc_107();  // [107] 0x00546750  // __cdecl, returns int
    virtual void vfunc_108();  // [108] 0x00546760  // __cdecl, returns int
    virtual void vfunc_109();  // [109] 0x00546770  // __cdecl, returns int
    virtual void vfunc_110();  // [110] 0x004d0a30  // __thiscall, returns int
    virtual void vfunc_111(int a0);  // [111] 0x0049d8e0  // __thiscall
    virtual void vfunc_112(int a0);  // [112] 0x0049da90  // __thiscall
    virtual void vfunc_113(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9);  // [113] 0x0049e7f0  // __thiscall
    virtual void vfunc_114();  // [114] 0x005467c0  // __cdecl, returns int
    virtual void vfunc_115();  // [115] 0x0049dbe0  // __thiscall
    virtual void vfunc_116();  // [116] 0x0049cfb0  // __thiscall, returns int
    virtual void vfunc_117();  // [117] 0x004a1480  // __thiscall
    virtual void vfunc_118();  // [118] 0x00546800  // __cdecl, returns int
    virtual void vfunc_119();  // [119] 0x00546810  // __cdecl, returns int
    virtual void vfunc_120();  // [120] 0x00546820  // __cdecl, returns int
    virtual void vfunc_121();  // [121] 0x004a0660  // __thiscall
    virtual void vfunc_122();  // [122] 0x00546840  // __cdecl, returns int
    virtual void vfunc_123();  // [123] 0x00546850  // __cdecl, returns int
    virtual void vfunc_124();  // [124] 0x00546860  // __cdecl, returns int
    virtual void vfunc_125();  // [125] 0x00546870  // __cdecl, returns int
    virtual void vfunc_126();  // [126] 0x00546880  // __cdecl, returns int
    virtual void vfunc_127();  // [127] 0x004a4420  // __thiscall
    virtual void vfunc_128();  // [128] 0x0049d080  // __thiscall, returns int
    virtual void vfunc_129(int a0);  // [129] 0x005468b0  // __stdcall
    virtual void vfunc_130(int a0, int a1, int a2);  // [130] 0x004a0330  // __thiscall
    virtual void vfunc_131();  // [131] 0x004a0370  // __thiscall
    virtual void vfunc_132();  // [132] 0x00547110  // __cdecl, returns int
    virtual void vfunc_133(int a0, int a1);  // [133] 0x005a58a0  // __stdcall
    virtual void vfunc_134(int a0, int a1, int a2);  // [134] 0x005a57d0  // __thiscall, returns int
    virtual void vfunc_135();  // [135] 0x0040e700  // __cdecl
    virtual void vfunc_136();  // [136] 0x004a2690  // __thiscall
    virtual void vfunc_137();  // [137] 0x0060eb20  // __thiscall
    virtual void vfunc_138();  // [138] 0x0040e7a0  // __cdecl, returns int
    virtual void vfunc_139();  // [139] 0x0061cb60  // __thiscall
    virtual void vfunc_140();  // [140] 0x00612de0  // __thiscall
    virtual void vfunc_141();  // [141] 0x0040e710  // __cdecl
    virtual void vfunc_142(int a0, int a1, int a2);  // [142] 0x00613d60  // __thiscall
    virtual void vfunc_143(int a0, int a1, int a2);  // [143] 0x00613c60  // __thiscall
    virtual void vfunc_144(int a0);  // [144] 0x004a4b20  // __stdcall
    virtual void vfunc_145();  // [145] 0x00616450  // __cdecl
    virtual void vfunc_146(int a0, int a1);  // [146] 0x0061cfa0  // __thiscall, returns int
    virtual void vfunc_147();  // [147] 0x0061b1e0  // __cdecl
    virtual void vfunc_148();  // [148] 0x0061d000  // __cdecl
    virtual void vfunc_149();  // [149] 0x0061b210  // __thiscall, returns int
    virtual void vfunc_150(int a0);  // [150] 0x0061bf60  // __thiscall
    virtual void vfunc_151();  // [151] 0x0061b1c0  // __cdecl
    virtual void vfunc_152(int a0);  // [152] 0x004a2340  // __thiscall
    virtual void vfunc_153(int a0, int a1);  // [153] 0x00614770  // __thiscall
    virtual void vfunc_154(int a0);  // [154] 0x0049c620  // __thiscall
    virtual void vfunc_155();  // [155] 0x0060ed00  // __thiscall
    virtual void vfunc_156();  // [156] 0x0061df10  // __cdecl
    virtual void vfunc_157();  // [157] 0x0061dd70  // __cdecl
    virtual void vfunc_158(int a0, int a1, int a2);  // [158] 0x004a1a50  // __thiscall
    virtual void vfunc_159();  // [159] 0x004a1b30  // __thiscall
    virtual void vfunc_160(int a0, int a1, int a2);  // [160] 0x0061bd30  // __thiscall
    virtual void vfunc_161(int a0, int a1);  // [161] 0x0061bf10  // __stdcall
    virtual void vfunc_162(int a0, int a1);  // [162] 0x004d27e0  // __stdcall
    virtual void vfunc_163(int a0, int a1);  // [163] 0x0061b1f0  // __stdcall, returns int
    virtual void vfunc_164(int a0);  // [164] 0x0061b2c0  // __stdcall
    virtual void vfunc_165();  // [165] 0x0061a450  // __thiscall, returns int
    virtual void vfunc_166();  // [166] 0x00616160  // __cdecl
    virtual void vfunc_167(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7);  // [167] 0x006161b0  // __thiscall
    virtual void vfunc_168(int a0);  // [168] 0x006143a0  // __thiscall, returns int
    virtual void vfunc_169(int a0);  // [169] 0x0061a560  // __thiscall
    virtual void vfunc_170();  // [170] 0x004a13b0  // __thiscall
    virtual void vfunc_171(int a0);  // [171] 0x0061d9d0  // __thiscall
    virtual void vfunc_172();  // [172] 0x0061db90  // __cdecl
    virtual void vfunc_173();  // [173] 0x004d2e20  // __cdecl
    virtual void vfunc_174();  // [174] 0x004a49c0  // __thiscall
    virtual void vfunc_175();  // [175] 0x0061b2e0  // __thiscall
    virtual void vfunc_176();  // [176] 0x0061b340  // __thiscall
    virtual void vfunc_177();  // [177] 0x0061b3a0  // __thiscall
    virtual void vfunc_178(int a0);  // [178] 0x00615820  // __thiscall
    virtual void vfunc_179();  // [179] 0x0061dd50  // __cdecl
    virtual void vfunc_180(int a0, int a1);  // [180] 0x0061b400  // __thiscall
    virtual void vfunc_181();  // [181] 0x0049de00  // __thiscall
    virtual void vfunc_182(int a0, int a1);  // [182] 0x006114b0  // __thiscall
    virtual void vfunc_183(int a0);  // [183] 0x004a0bb0  // __stdcall
    virtual void vfunc_184(int a0, int a1);  // [184] 0x004cee00  // __thiscall, returns int
    virtual void vfunc_185(int a0, int a1, int a2);  // [185] 0x004cfff0  // __thiscall
    virtual void vfunc_186(int a0);  // [186] 0x004cdb20  // __thiscall
    virtual void vfunc_187(int a0, int a1);  // [187] 0x004cfe30  // __thiscall
    virtual void vfunc_188(int a0);  // [188] 0x004a27c0  // __thiscall
    virtual void vfunc_189(int a0, int a1, int a2, int a3, int a4, int a5);  // [189] 0x00614530  // __thiscall
    virtual void vfunc_190(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11);  // [190] 0x0061c360  // __thiscall
    virtual void vfunc_191(int a0);  // [191] 0x0040e730  // __thiscall
    virtual void vfunc_192();  // [192] 0x0040e780  // __cdecl
    virtual void vfunc_193();  // [193] 0x00616550  // __thiscall
    virtual void vfunc_194(int a0);  // [194] 0x00613760  // __thiscall
    virtual void vfunc_195();  // [195] 0x0061c650  // __thiscall
    virtual void vfunc_196(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8);  // [196] 0x0060f8f0  // __thiscall
    virtual void vfunc_197(int a0, int a1, int a2, int a3, int a4, int a5, int a6);  // [197] 0x004a2880  // __thiscall
    virtual void vfunc_198(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8);  // [198] 0x004a19a0  // __thiscall
    virtual void vfunc_199(int a0, int a1);  // [199] 0x00610870  // __stdcall
    virtual void vfunc_200(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11, int a12, int a13, int a14, int a15, int a16);  // [200] 0x00610c50  // __thiscall
    virtual void vfunc_201(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11);  // [201] 0x0061b4f0  // __thiscall
    virtual void vfunc_202(int a0, int a1, int a2);  // [202] 0x0061bc30  // __thiscall
    virtual void vfunc_203(int a0);  // [203] 0x00617b90  // __thiscall
    virtual void vfunc_204(int a0);  // [204] 0x00617c30  // __thiscall
    virtual void vfunc_205(int a0);  // [205] 0x0061d9a0  // __stdcall
    virtual void vfunc_206();  // [206] 0x004d2280  // __thiscall
    virtual void vfunc_207(int a0, int a1);  // [207] 0x004cf0b0  // __thiscall
    virtual void vfunc_208(int a0, int a1, int a2, int a3, int a4, int a5, int a6);  // [208] 0x004cd4a0  // __thiscall
    virtual void vfunc_209(int a0, int a1);  // [209] 0x004ceef0  // __thiscall
    virtual void vfunc_210();  // [210] 0x0061d4c0  // __thiscall
    virtual void vfunc_211();  // [211] 0x0061d4f0  // __thiscall
    virtual void vfunc_212();  // [212] 0x0061d520  // __cdecl
    virtual void vfunc_213(int a0, int a1, int a2, int a3, int a4);  // [213] 0x0061df20  // __thiscall
    virtual void vfunc_214();  // [214] 0x004a2c50  // __thiscall
    virtual void vfunc_215();  // [215] 0x004a2800  // __thiscall
    virtual void vfunc_216();  // [216] 0x0040e7b0  // __cdecl
    virtual void vfunc_217(int a0);  // [217] 0x004a11e0  // __stdcall
    virtual void vfunc_218(int a0, int a1);  // [218] 0x004d2870  // __thiscall
    virtual void vfunc_219(int a0, int a1, int a2, int a3);  // [219] 0x004d2950  // __thiscall
    virtual void vfunc_220(int a0);  // [220] 0x004cfae0  // __thiscall
    virtual void vfunc_221(int a0);  // [221] 0x004cfa30  // __thiscall
    virtual void vfunc_222();  // [222] 0x004a22c0  // __thiscall
    virtual void vfunc_223(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11, int a12, int a13, int a14);  // [223] 0x0040e7e0  // __stdcall
    virtual void vfunc_224();  // [224] 0x004d2b90  // __cdecl
    virtual void vfunc_225();  // [225] 0x004d26c0  // __thiscall
    virtual void vfunc_226();  // [226] 0x004d26f0  // __cdecl
    virtual void vfunc_227(int a0);  // [227] 0x004d2710  // __stdcall
    virtual void vfunc_228();  // [228] 0x004a0e10  // __cdecl, returns int
    virtual void vfunc_229(int a0, int a1);  // [229] 0x0040e880  // __stdcall
    virtual void vfunc_230();  // [230] 0x004d2820  // __thiscall
    virtual void vfunc_231();  // [231] 0x004d2de0  // __cdecl
    virtual void vfunc_232(int a0);  // [232] 0x0040e890  // __stdcall
    virtual void vfunc_233(int a0, int a1);  // [233] 0x0049c520  // __stdcall
    virtual void vfunc_234();  // [234] 0x004d25f0  // __cdecl
    virtual void vfunc_235(int a0);  // [235] 0x004a4140  // __thiscall
    virtual void vfunc_236(int a0, int a1, int a2);  // [236] 0x004cf920  // __thiscall

    InfantryClass();  // 0x004cb520
    virtual ~InfantryClass();

    // ---- data members (offsets from ctor 0x004cb520) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69ff50
    unsigned char pad_004[0x24];
    unsigned long m_028;  // /* ? */
    unsigned char pad_02c[0x108];
    unsigned long m_134;  // /* ? */
    unsigned char pad_138[0xc0];
    unsigned long m_1f8;  // /* ? */
    unsigned char pad_1fc[0x14c];
    unsigned long m_348;  // /* ? */
    unsigned long m_34c;  // init 0xffffffff
    unsigned long m_350;  // /* ? */
    unsigned char pad_354[0x4];
    unsigned long m_358;  // /* ? */
    unsigned long m_35c;  // /* ? */
    unsigned long m_360;  // /* ? */
    unsigned long m_364;  // /* ? */
    unsigned long m_368;  // /* ? */
    unsigned char pad_36c[0x4];
    unsigned long m_370;  // /* ? */
};
