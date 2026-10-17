#pragma once
// ==========================================================================
// UnitClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class UnitClass
// vtable       : 0x006a66a8  (239 slots)
// constructor  : 0x00630cc0
// vtable store : 0x00630d2e  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "FootClass.h"

class UnitClass : public FootClass
{
public:
    // ---- vtable 0x006a66a8 (239 slots) ----
    virtual void QueryInterface(int a0, int a1, int a2);  // [0] 0x00405790  // __stdcall, returns int
    virtual void Init(int a0);  // [1] 0x00405830  // __stdcall, returns int
    virtual void Shutdown(int a0);  // [2] 0x00405840  // __stdcall, returns int
    virtual void vfunc_3(int a0, int a1);  // [3] 0x0063e140  // __stdcall, returns int
    virtual void Release(int a0);  // [4] 0x00405990  // __stdcall
    virtual void vfunc_5(int a0, int a1);  // [5] 0x0063c430  // __stdcall
    virtual void vfunc_6(int a0, int a1, int a2);  // [6] 0x0063c5b0  // __stdcall
    virtual void GetType(int a0, int a1);  // [7] 0x00405910  // __stdcall, returns int
    virtual void vfunc_8(int a0);  // [8] 0x0063e1f0  // __thiscall
    virtual void vfunc_9();  // [9] 0x0060f180  // __cdecl
    virtual void vfunc_10(int a0, int a1);  // [10] 0x0063c6a0  // __thiscall
    virtual void vfunc_11();  // [11] 0x0063e180  // __cdecl, returns int
    virtual void vfunc_12();  // [12] 0x0063e130  // __cdecl, returns int
    virtual void vfunc_13(int a0);  // [13] 0x0063c5d0  // __thiscall
    virtual void vfunc_14();  // [14] 0x00612db0  // __cdecl
    virtual void vfunc_15();  // [15] 0x00612dc0  // __cdecl
    virtual void Compute_CRC();  // [16] 0x004059f0  // __cdecl, returns int
    virtual void vfunc_17();  // [17] 0x00570a20  // __cdecl, returns int
    virtual void vfunc_18(int a0);  // [18] 0x00570960  // __stdcall
    virtual void vfunc_19(int a0);  // [19] 0x004a26e0  // __thiscall
    virtual void vfunc_20();  // [20] 0x00570e80  // __cdecl
    virtual void vfunc_21(int a0, int a1);  // [21] 0x004a4a30  // __thiscall
    virtual void vfunc_22(int a0);  // [22] 0x00405a80  // __stdcall
    virtual void vfunc_23();  // [23] 0x006317f0  // __thiscall
    virtual void vfunc_24();  // [24] 0x0060eb70  // __thiscall
    virtual void vfunc_25(int a0, int a1);  // [25] 0x004a1690  // __thiscall
    virtual void vfunc_26();  // [26] 0x0056e820
    virtual void vfunc_27(int a0, int a1, int a2);  // [27] 0x00639450  // __thiscall, returns int
    virtual void vfunc_28(int a0, int a1, int a2);  // [28] 0x00639020  // __thiscall
    virtual void vfunc_29();  // [29] 0x004a2140  // __thiscall
    virtual void vfunc_30();  // [30] 0x00570ef0  // __cdecl
    virtual void vfunc_31();  // [31] 0x0063e110  // __cdecl
    virtual void vfunc_32();  // [32] 0x0060eb10  // __thiscall
    virtual void vfunc_33();  // [33] 0x0063a090  // __cdecl
    virtual void vfunc_34();  // [34] 0x0061b1b0  // __cdecl
    virtual void vfunc_35();  // [35] 0x0063e190  // __cdecl
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
    virtual void vfunc_48(int a0);  // [48] 0x0063c120  // __thiscall
    virtual void vfunc_49(int a0, int a1);  // [49] 0x00632b00  // __thiscall
    virtual void vfunc_50(int a0);  // [50] 0x004a0a00  // __thiscall
    virtual void vfunc_51(int a0);  // [51] 0x0063c6e0  // __thiscall
    virtual void vfunc_52(int a0);  // [52] 0x005700c0  // __thiscall
    virtual void vfunc_53(int a0);  // [53] 0x0056ea70  // __thiscall
    virtual void vfunc_54(int a0);  // [54] 0x0063c1b0  // __stdcall
    virtual void vfunc_55(int a0);  // [55] 0x0063c210  // __stdcall
    virtual void vfunc_56();  // [56] 0x004a4a10  // __thiscall
    virtual void vfunc_57();  // [57] 0x00617ce0  // __cdecl
    virtual void vfunc_58(int a0);  // [58] 0x0061b480  // __stdcall, returns int
    virtual void vfunc_59(int a0, int a1, int a2);  // [59] 0x00635060  // __thiscall
    virtual void vfunc_60(int a0);  // [60] 0x005701b0  // __thiscall, returns int
    virtual void vfunc_61(int a0, int a1, int a2, int a3, int a4);  // [61] 0x00610880  // __thiscall
    virtual void vfunc_62(int a0, int a1, int a2, int a3, int a4, int a5);  // [62] 0x0060fb90  // __thiscall
    virtual void vfunc_63(int a0, int a1, int a2, int a3, int a4, int a5, int a6);  // [63] 0x00636b90  // __thiscall
    virtual void vfunc_64(int a0, int a1);  // [64] 0x00570990  // __stdcall
    virtual void vfunc_65();  // [65] 0x0060f590  // __thiscall
    virtual void vfunc_66(int a0, int a1);  // [66] 0x0061c1e0  // __thiscall
    virtual void vfunc_67(int a0);  // [67] 0x0049c590  // __thiscall
    virtual void vfunc_68(int a0);  // [68] 0x0056f000  // __thiscall
    virtual void vfunc_69(int a0);  // [69] 0x0056f160  // __thiscall
    virtual void vfunc_70();  // [70] 0x0040e690  // __cdecl
    virtual void vfunc_71();  // [71] 0x0056f680  // __cdecl
    virtual void vfunc_72(int a0, int a1, int a2);  // [72] 0x00633590  // __stdcall
    virtual void vfunc_73(int a0, int a1);  // [73] 0x00633520  // __thiscall
    virtual void vfunc_74(int a0);  // [74] 0x00612dd0  // __stdcall
    virtual void vfunc_75();  // [75] 0x00613f10  // __thiscall
    virtual void vfunc_76(int a0);  // [76] 0x0056eda0  // __thiscall
    virtual void vfunc_77(int a0, int a1);  // [77] 0x00611540  // __thiscall
    virtual void vfunc_78(int a0);  // [78] 0x006164c0  // __stdcall
    virtual void vfunc_79(int a0, int a1, int a2, int a3, int a4, int a5);  // [79] 0x00632bf0  // __thiscall
    virtual void vfunc_80(int a0, int a1, int a2, int a3, int a4, int a5, int a6);  // [80] 0x0063bc30  // __thiscall
    virtual void vfunc_81();  // [81] 0x0056eca0  // __cdecl
    virtual void vfunc_82();  // [82] 0x0056ecb0  // __cdecl
    virtual void vfunc_83();  // [83] 0x0061a4d0  // __thiscall
    virtual void vfunc_84();  // [84] 0x005468d0  // __cdecl
    virtual void vfunc_85(int a0);  // [85] 0x00546d60  // __stdcall
    virtual void vfunc_86(int a0);  // [86] 0x00634350  // __thiscall
    virtual void vfunc_87(int a0, int a1);  // [87] 0x00570240  // __thiscall
    virtual void vfunc_88(int a0, int a1);  // [88] 0x00632600  // __thiscall
    virtual void vfunc_89(int a0);  // [89] 0x0060f490  // __thiscall
    virtual void vfunc_90(int a0);  // [90] 0x0056ece0  // __stdcall
    virtual void vfunc_91(int a0);  // [91] 0x004a1160  // __thiscall
    virtual void vfunc_92(int a0);  // [92] 0x004a2cd0  // __thiscall
    virtual void vfunc_93(int a0);  // [93] 0x0056ed00  // __thiscall
    virtual void vfunc_94(int a0, int a1, int a2, int a3, int a4);  // [94] 0x00638400  // __thiscall
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
    virtual void vfunc_106();  // [106] 0x0063c270  // __thiscall
    virtual void vfunc_107();  // [107] 0x00546750  // __cdecl, returns int
    virtual void vfunc_108();  // [108] 0x00546760  // __cdecl, returns int
    virtual void vfunc_109();  // [109] 0x00546770  // __cdecl, returns int
    virtual void vfunc_110();  // [110] 0x0063c760
    virtual void vfunc_111(int a0);  // [111] 0x0049d8e0  // __thiscall
    virtual void vfunc_112();  // [112] 0x00639690  // __thiscall
    virtual void vfunc_113();  // [113] 0x0063c170
    virtual void vfunc_114();  // [114] 0x00637b50  // __thiscall, returns int
    virtual void vfunc_115();  // [115] 0x00638340  // __thiscall
    virtual void vfunc_116();  // [116] 0x006397f0  // __thiscall
    virtual void vfunc_117();  // [117] 0x004a1480  // __thiscall
    virtual void vfunc_118();  // [118] 0x00546800  // __cdecl, returns int
    virtual void vfunc_119();  // [119] 0x00546810  // __cdecl, returns int
    virtual void vfunc_120();  // [120] 0x006371e0  // __thiscall
    virtual void vfunc_121();  // [121] 0x004a0660  // __thiscall
    virtual void vfunc_122();  // [122] 0x00546840  // __cdecl, returns int
    virtual void vfunc_123();  // [123] 0x00546850  // __cdecl, returns int
    virtual void vfunc_124();  // [124] 0x00639c80  // __thiscall
    virtual void vfunc_125();  // [125] 0x00546870  // __cdecl, returns int
    virtual void vfunc_126();  // [126] 0x00546880  // __cdecl, returns int
    virtual void vfunc_127();  // [127] 0x004a4420  // __thiscall
    virtual void vfunc_128();  // [128] 0x00639840  // __thiscall
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
    virtual void vfunc_141();  // [141] 0x0063c180  // __cdecl
    virtual void vfunc_142(int a0, int a1, int a2);  // [142] 0x00613d60  // __thiscall
    virtual void vfunc_143(int a0, int a1, int a2);  // [143] 0x00613c60  // __thiscall
    virtual void vfunc_144(int a0);  // [144] 0x0063e1a0  // __stdcall
    virtual void vfunc_145();  // [145] 0x00616450  // __cdecl
    virtual void vfunc_146(int a0, int a1);  // [146] 0x0061cfa0  // __thiscall, returns int
    virtual void vfunc_147();  // [147] 0x0063a0a0  // __thiscall
    virtual void vfunc_148();  // [148] 0x0063c770  // __thiscall
    virtual void vfunc_149();  // [149] 0x00639b80  // __cdecl
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
    virtual void vfunc_162(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7);  // [162] 0x0060efa0  // __thiscall
    virtual void vfunc_163(int a0, int a1, int a2, int a3, int a4);  // [163] 0x00639890  // __thiscall
    virtual void vfunc_164(int a0);  // [164] 0x00639d20  // __stdcall
    virtual void vfunc_165();  // [165] 0x00639c70
    virtual void vfunc_166();  // [166] 0x00616160  // __cdecl
    virtual void vfunc_167(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7);  // [167] 0x006161b0  // __thiscall
    virtual void vfunc_168(int a0);  // [168] 0x006143a0  // __thiscall, returns int
    virtual void vfunc_169(int a0);  // [169] 0x0061a560  // __thiscall
    virtual void vfunc_170();  // [170] 0x004a13b0  // __thiscall
    virtual void vfunc_171(int a0);  // [171] 0x0061d9d0  // __thiscall
    virtual void vfunc_172();  // [172] 0x0061db90  // __cdecl
    virtual void vfunc_173();  // [173] 0x0040e720  // __cdecl
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
    virtual void vfunc_184(int a0, int a1);  // [184] 0x00639d70  // __thiscall, returns int
    virtual void vfunc_185(int a0, int a1, int a2);  // [185] 0x0063b3d0  // __thiscall, returns int
    virtual void vfunc_186(int a0, int a1);  // [186] 0x006142e0  // __thiscall
    virtual void vfunc_187(int a0, int a1);  // [187] 0x00639fc0  // __thiscall, returns int
    virtual void vfunc_188(int a0);  // [188] 0x0063e0c0  // __thiscall
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
    virtual void vfunc_206();  // [206] 0x0061b490  // __cdecl
    virtual void vfunc_207();  // [207] 0x0040e790  // __cdecl
    virtual void vfunc_208(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7);  // [208] 0x0063a420  // __thiscall
    virtual void vfunc_209(int a0, int a1, int a2, int a3);  // [209] 0x006335b0  // __thiscall
    virtual void vfunc_210();  // [210] 0x0061d4c0  // __thiscall
    virtual void vfunc_211();  // [211] 0x0061d4f0  // __thiscall
    virtual void vfunc_212();  // [212] 0x0061d520  // __cdecl
    virtual void vfunc_213(int a0, int a1, int a2, int a3, int a4);  // [213] 0x0061df20  // __thiscall
    virtual void vfunc_214();  // [214] 0x004a2c50  // __thiscall
    virtual void vfunc_215();  // [215] 0x004a2800  // __thiscall
    virtual void vfunc_216();  // [216] 0x00631f90  // __thiscall
    virtual void vfunc_217(int a0);  // [217] 0x004a11e0  // __stdcall
    virtual void vfunc_218();  // [218] 0x004a2850  // __thiscall
    virtual void vfunc_219();  // [219] 0x0040e7d0  // __cdecl
    virtual void vfunc_220(int a0);  // [220] 0x0049dd90  // __thiscall
    virtual void vfunc_221();  // [221] 0x0049dd60  // __thiscall
    virtual void vfunc_222();  // [222] 0x004a22c0  // __thiscall
    virtual void vfunc_223(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11, int a12, int a13, int a14);  // [223] 0x0040e7e0  // __stdcall
    virtual void vfunc_224();  // [224] 0x0040e840  // __cdecl
    virtual void vfunc_225();  // [225] 0x0040e850  // __cdecl
    virtual void vfunc_226();  // [226] 0x0040e860  // __cdecl
    virtual void vfunc_227(int a0);  // [227] 0x0040e870  // __stdcall
    virtual void vfunc_228();  // [228] 0x004a0e10  // __cdecl, returns int
    virtual void vfunc_229(int a0, int a1);  // [229] 0x0063a1b0  // __thiscall
    virtual void vfunc_230(int a0, int a1);  // [230] 0x004a1c20  // __thiscall
    virtual void vfunc_231();  // [231] 0x0063a0e0  // __thiscall
    virtual void vfunc_232(int a0);  // [232] 0x0040e890  // __stdcall
    virtual void vfunc_233(int a0, int a1);  // [233] 0x0049c520  // __stdcall
    virtual void vfunc_234();  // [234] 0x004a2330  // __cdecl
    virtual void vfunc_235(int a0);  // [235] 0x004a4140  // __thiscall
    virtual void vfunc_236(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8);  // [236] 0x006354d0  // __thiscall
    virtual void vfunc_237(int a0, int a1, int a2, int a3, int a4, int a5);  // [237] 0x00636230  // __thiscall
    virtual void vfunc_238(int a0, int a1);  // [238] 0x006350f0  // __thiscall

    UnitClass();  // 0x00630cc0
    // The destructor is declared non-virtual here: slot 0 holds a stub
    // shared with 66 other classes (0x00405790), not the destructor.
    ~UnitClass();

    // ---- data members (offsets from ctor 0x00630cc0) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    // vtable-pointer stores are omitted: the compiler supplies the vptr.
    unsigned char pad_004[0x24];
    unsigned int m_028;  // +0x028 /* ? */; 4-byte store: mov dword ptr [esi + 0x28], eax
    unsigned char pad_02c[0x108];
    unsigned int m_134;  // +0x134 /* ? */; 4-byte store: mov dword ptr [esi + 0x134], ecx
    unsigned char pad_138[0xc0];
    unsigned char m_1f8;  // +0x1f8 /* ? */; 1-byte store: mov byte ptr [esi + 0x1f8], dl
    unsigned char pad_1f9[0x14f];
    unsigned int m_348;  // +0x348 /* ? */; 4-byte store: mov dword ptr [esi + 0x348], eax
    unsigned int m_34c;  // +0x34c /* ? */; 4-byte store: mov dword ptr [esi + 0x34c], ecx
    unsigned int m_350;  // +0x350 /* ? */; 4-byte store: mov dword ptr [esi + 0x350], edx
    unsigned int m_354;  // +0x354 /* ? */; 4-byte store: mov dword ptr [esi + 0x354], edi
    unsigned int m_358;  // +0x358 /* ? */; 4-byte store: mov dword ptr [esi + 0x358], edx
    unsigned int m_35c;  // +0x35c /* ? */; 4-byte store: mov dword ptr [esi + 0x35c], edi
    unsigned int m_360;  // +0x360 /* ? */; 4-byte store: mov dword ptr [esi + 0x360], eax
    unsigned char m_364;  // +0x364 zero-initialised; 1-byte store: mov byte ptr [esi + 0x364], 0
    unsigned char m_365;  // +0x365 zero-initialised; 1-byte store: mov byte ptr [esi + 0x365], 0
    unsigned char m_366;  // +0x366 zero-initialised; 1-byte store: mov byte ptr [esi + 0x366], 0
    unsigned char m_367;  // +0x367 zero-initialised; 1-byte store: mov byte ptr [esi + 0x367], 0
    unsigned int m_368;  // +0x368 /* ? */; 4-byte store: mov dword ptr [esi + 0x368], eax
};
