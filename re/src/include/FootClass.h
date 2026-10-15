#pragma once
// ==========================================================================
// FootClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class FootClass
// vtable       : 0x0069e324  (6 slots)
// constructor  : 0x0049c2ca
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "TechnoClass.h"

class FootClass : public TechnoClass
{
public:
    // ---- vtable 0x0069e324 (6 slots) ----
    virtual void vfunc_0();  // [0] 0x00405af0
    virtual void vfunc_1();  // [1] 0x00405b00
    virtual void vfunc_2();  // [2] 0x00405b10
    virtual void vfunc_3();  // [3] 0x00405740
    virtual void vfunc_4();  // [4] 0x00405750
    virtual void vfunc_5();  // [5] 0x00405760

    FootClass();  // 0x0049c2ca
    virtual ~FootClass();

    // ---- data members (offsets from ctor 0x0049c0f0) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69e340
    unsigned long m_004;  // init 0x69e324
    unsigned char pad_008[0x210];
    unsigned long m_218;  // init 0xffffffff
    unsigned long m_21c;  // /* ? */
    unsigned long m_220;  // /* ? */
    unsigned char pad_224[0x4];
    unsigned long m_228;  // /* ? */
    unsigned long m_22c;  // /* ? */
    unsigned long m_230;  // /* ? */
    unsigned long m_234;  // /* ? */
    unsigned long m_238;  // /* ? */
    unsigned long m_23c;  // /* ? */
    unsigned long m_240;  // /* ? */
    unsigned long m_244;  // /* ? */
    unsigned long m_248;  // /* ? */
    unsigned long m_24c;  // /* ? */
    unsigned long m_250;  // /* ? */
    unsigned long m_254;  // init 0x3ff00000
    unsigned char pad_258[0x18];
    unsigned long m_270;  // /* ? */
    unsigned long m_274;  // /* ? */
    unsigned char pad_278[0x18];
    unsigned long m_290;  // /* ? */
    unsigned long m_294;  // /* ? */
    unsigned long m_298;  // /* ? */
    unsigned long m_29c;  // init 0xffffffff
    unsigned char pad_2a0[0x5c];
    unsigned long m_2fc;  // /* ? */
    unsigned char pad_300[0x4];
    unsigned long m_304;  // /* ? */
    unsigned long m_308;  // init 0xa
    unsigned long m_30c;  // /* ? */
    unsigned char pad_310[0x4];
    unsigned long m_314;  // /* ? */
    unsigned long m_318;  // /* ? */
    unsigned char pad_31c[0x4];
    unsigned long m_320;  // /* ? */
    unsigned long m_324;  // /* ? */
    unsigned long m_328;  // /* ? */
    unsigned char pad_32c[0x8];
    unsigned long m_334;  // init 0xff
    unsigned long m_338;  // /* ? */
    unsigned long m_33c;  // /* ? */
    unsigned long m_340;  // /* ? */
    unsigned long m_344;  // /* ? */
};
