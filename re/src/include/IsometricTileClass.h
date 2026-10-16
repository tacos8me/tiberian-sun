#pragma once
// ==========================================================================
// IsometricTileClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class IsometricTileClass
// vtable       : 0x006a0be8  (105 slots)
// constructor  : 0x004e9090
// vtable store : 0x004e90b1  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "ObjectClass.h"

class IsometricTileClass : public ObjectClass
{
public:
    // ---- vtable 0x006a0be8 (105 slots) ----
    virtual void QueryInterface(int a0, int a1, int a2);  // [0] 0x00405790  // __stdcall, returns int
    virtual void Init(int a0);  // [1] 0x00405830  // __stdcall, returns int
    virtual void Shutdown(int a0);  // [2] 0x00405840  // __stdcall, returns int
    virtual void vfunc_3(int a0, int a1);  // [3] 0x004e9390  // __stdcall, returns int
    virtual void Release(int a0);  // [4] 0x00405990  // __stdcall
    virtual void vfunc_5(int a0, int a1);  // [5] 0x004e9280  // __stdcall
    virtual void vfunc_6(int a0, int a1, int a2);  // [6] 0x004e92d0  // __stdcall, returns int
    virtual void GetType(int a0, int a1);  // [7] 0x00405910  // __stdcall, returns int
    virtual void Set_ID();  // [9] 0x004059b0  // __cdecl
    virtual void vfunc_10(int a0, int a1);  // [10] 0x0056fb00  // __stdcall
    virtual void vfunc_11();  // [11] 0x004e9380  // __cdecl, returns int
    virtual void vfunc_12();  // [12] 0x004e9370  // __cdecl, returns int
    virtual void vfunc_13(int a0);  // [13] 0x00570630  // __thiscall
    virtual void Save();  // [14] 0x004059d0  // __cdecl
    virtual void GetSizeMax();  // [15] 0x004059e0  // __cdecl, returns int
    virtual void Compute_CRC();  // [16] 0x004059f0  // __cdecl, returns int
    virtual void vfunc_17();  // [17] 0x00570a20  // __cdecl, returns int
    virtual void vfunc_18(int a0);  // [18] 0x00570960  // __stdcall
    virtual void vfunc_19(int a0);  // [19] 0x00405a30  // __stdcall
    virtual void vfunc_20();  // [20] 0x00570e80  // __cdecl
    virtual void vfunc_21();  // [21] 0x00570ea0  // __cdecl
    virtual void vfunc_22(int a0);  // [22] 0x00405a80  // __stdcall
    virtual void vfunc_23();  // [23] 0x0056e860  // __thiscall
    virtual void vfunc_24();  // [24] 0x004167b0  // __cdecl
    virtual void vfunc_25(int a0, int a1);  // [25] 0x004167c0  // __stdcall, returns int
    virtual void vfunc_26();  // [26] 0x0056e820
    virtual void vfunc_27(int a0, int a1, int a2);  // [27] 0x0056eb50  // __stdcall, returns int
    virtual void vfunc_28(int a0, int a1);  // [28] 0x0056eb40  // __stdcall, returns int
    virtual void vfunc_29();  // [29] 0x0056eb60  // __cdecl
    virtual void vfunc_30();  // [30] 0x00570ef0  // __cdecl
    virtual void vfunc_31();  // [31] 0x004167d0  // __cdecl
    virtual void vfunc_32();  // [32] 0x00570ec0  // __cdecl, returns int
    virtual void vfunc_33();  // [33] 0x004e93d0  // __cdecl
    virtual void vfunc_34();  // [34] 0x0056eba0  // __cdecl, returns int
    virtual void vfunc_35();  // [35] 0x004167e0  // __cdecl, returns int
    virtual void vfunc_36();  // [36] 0x0056ebb0  // __cdecl
    virtual void vfunc_37();  // [37] 0x0056ebc0  // __cdecl
    virtual void vfunc_38();  // [38] 0x0056ebd0  // __cdecl
    virtual void vfunc_39();  // [39] 0x0056ebe0  // __cdecl
    virtual void vfunc_40(int a0);  // [40] 0x0040e5d0  // __stdcall
    virtual void vfunc_41(int a0);  // [41] 0x0040e600  // __stdcall
    virtual void vfunc_42(int a0);  // [42] 0x0040e630  // __stdcall
    virtual void vfunc_43(int a0, int a1);  // [43] 0x004167f0  // __stdcall
    virtual void vfunc_44(int a0);  // [44] 0x0040e660  // __stdcall
    virtual void vfunc_45();  // [45] 0x00570ed0  // __cdecl
    virtual void vfunc_46(int a0, int a1, int a2, int a3);  // [46] 0x00570db0  // __thiscall
    virtual void vfunc_47();  // [47] 0x00416830  // __cdecl
    virtual void vfunc_48();  // [48] 0x004e9320  // __thiscall
    virtual void vfunc_49(int a0, int a1);  // [49] 0x004e92f0  // __thiscall
    virtual void vfunc_50(int a0);  // [50] 0x0056fb40  // __thiscall
    virtual void vfunc_51(int a0);  // [51] 0x0056ebf0  // __stdcall
    virtual void vfunc_52(int a0);  // [52] 0x005700c0  // __thiscall
    virtual void vfunc_53(int a0);  // [53] 0x0056ea70  // __thiscall
    virtual void vfunc_54(int a0);  // [54] 0x00570480  // __stdcall
    virtual void vfunc_55(int a0);  // [55] 0x00570500  // __stdcall
    virtual void vfunc_56();  // [56] 0x005709b0  // __thiscall
    virtual void vfunc_57();  // [57] 0x0056ec00  // __cdecl
    virtual void vfunc_58(int a0);  // [58] 0x0056ec10  // __stdcall, returns int
    virtual void vfunc_59(int a0, int a1, int a2);  // [59] 0x0056f480  // __thiscall
    virtual void vfunc_60(int a0);  // [60] 0x005701b0  // __thiscall, returns int
    virtual void vfunc_61(int a0, int a1);  // [61] 0x00416840  // __stdcall
    virtual void vfunc_62(int a0, int a1);  // [62] 0x00416850  // __stdcall
    virtual void vfunc_63(int a0, int a1);  // [63] 0x004e93e0  // __stdcall
    virtual void vfunc_64(int a0, int a1);  // [64] 0x00570990  // __stdcall
    virtual void vfunc_65();  // [65] 0x0056ec20  // __cdecl
    virtual void vfunc_66(int a0, int a1);  // [66] 0x0056ec30  // __stdcall
    virtual void vfunc_67(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7);  // [67] 0x004e8d50  // __thiscall
    virtual void vfunc_68(int a0);  // [68] 0x0056f000  // __thiscall
    virtual void vfunc_69(int a0);  // [69] 0x0056f160  // __thiscall
    virtual void vfunc_70();  // [70] 0x0040e690  // __cdecl
    virtual void vfunc_71();  // [71] 0x0056f680  // __cdecl
    virtual void vfunc_72(int a0, int a1, int a2);  // [72] 0x0056ec50  // __stdcall
    virtual void vfunc_73(int a0, int a1, int a2);  // [73] 0x0056ec40  // __stdcall
    virtual void vfunc_74(int a0);  // [74] 0x0056ec60  // __stdcall
    virtual void vfunc_75();  // [75] 0x0056ee20  // __thiscall
    virtual void vfunc_76(int a0);  // [76] 0x0056eda0  // __thiscall
    virtual void vfunc_77(int a0, int a1);  // [77] 0x0056ec70  // __stdcall
    virtual void vfunc_78(int a0);  // [78] 0x0056ec80  // __stdcall, returns int
    virtual void vfunc_79(int a0, int a1, int a2, int a3, int a4, int a5);  // [79] 0x0056fbe0  // __thiscall, returns int
    virtual void vfunc_80(int a0, int a1, int a2);  // [80] 0x0056ec90  // __stdcall
    virtual void vfunc_81();  // [81] 0x0056eca0  // __cdecl
    virtual void vfunc_82();  // [82] 0x0056ecb0  // __cdecl
    virtual void vfunc_83();  // [83] 0x0056ecc0  // __cdecl, returns int
    virtual void vfunc_84();  // [84] 0x0056ecd0  // __cdecl
    virtual void vfunc_85(int a0);  // [85] 0x00416860  // __stdcall
    virtual void vfunc_86(int a0);  // [86] 0x00416870  // __stdcall
    virtual void vfunc_87(int a0, int a1);  // [87] 0x00570240  // __thiscall
    virtual void vfunc_88(int a0, int a1, int a2);  // [88] 0x0056fbc0  // __stdcall, returns int
    virtual void vfunc_89(int a0);  // [89] 0x005700b0  // __stdcall, returns int
    virtual void vfunc_90(int a0);  // [90] 0x0056ece0  // __stdcall
    virtual void vfunc_91(int a0);  // [91] 0x0056ecf0  // __stdcall
    virtual void vfunc_92(int a0, int a1);  // [92] 0x00570e70  // __stdcall
    virtual void vfunc_93(int a0);  // [93] 0x0056ed00  // __thiscall
    virtual void vfunc_94(int a0, int a1, int a2, int a3, int a4);  // [94] 0x00416880  // __stdcall, returns int
    virtual void vfunc_95(int a0, int a1, int a2, int a3, int a4);  // [95] 0x00416890  // __stdcall, returns int
    virtual void vfunc_96(int a0);  // [96] 0x0040e6a0  // __stdcall
    virtual void vfunc_97(int a0);  // [97] 0x00570cc0  // __stdcall
    virtual void vfunc_98(int a0);  // [98] 0x0040e6c0  // __stdcall
    virtual void vfunc_99();  // [99] 0x00570ce0  // __cdecl
    virtual void vfunc_100(int a0);  // [100] 0x00570d00  // __stdcall
    virtual void vfunc_101(int a0);  // [101] 0x00570d50  // __cdecl
    virtual void vfunc_102();  // [102] 0x00570370  // __thiscall
    virtual void vfunc_103(int a0);  // [103] 0x005703b0  // __thiscall
    virtual void vfunc_104();  // [104] 0x00570360  // __cdecl

    IsometricTileClass();  // 0x004e9090
    // The destructor is declared non-virtual here: slot 0 holds a stub
    // shared with 66 other classes (0x00405790), not the destructor.
    ~IsometricTileClass();

    // ---- data members (offsets from ctor 0x004e9090) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    unsigned long m_000;  // +0x000 init 0x6a0be8; 4-byte store: mov dword ptr [esi], 0x6a0be8
    unsigned long m_004;  // +0x004 init 0x6a0bcc; 4-byte store: mov dword ptr [esi + 4], 0x6a0bcc
    unsigned char pad_008[0x27];
    unsigned char m_02f;  // +0x02f zero-initialised; 1-byte store: mov byte ptr [esi + 0x2f], 0
    unsigned char pad_030[0x1c];
    unsigned long m_04c;  // +0x04c /* ? */; 4-byte store: mov dword ptr [esi + 0x4c], edx
};
