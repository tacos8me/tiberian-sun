#pragma once
// ==========================================================================
// INIClass_INISection.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class INIClass::INISection
// vtable       : 0x006a0410  (1 slots)
// constructor  : 0x004d4870
// vtable store : 0x004d4b1e  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "INIClass.h"
#include "INIClass.h"

class INISection;  // RTTI formal param, no type evidence
class INIClass_INISection : public Node<INIClass::INISection*, INIClass>
{
public:
    // ---- vtable 0x006a0410 (1 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x004d7f80  // __thiscall

    INIClass_INISection();  // 0x004d4870
    virtual ~INIClass_INISection();

    // ---- data members (offsets from ctor 0x004d6b4b) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x6a0418
    unsigned long m_004;  // /* ? */
    unsigned char pad_008[0x4];
    unsigned long m_00c;  // /* ? */
    unsigned long m_010;  // /* ? */
    unsigned char pad_014[0x28];
    unsigned long m_03c;  // /* ? */
};
