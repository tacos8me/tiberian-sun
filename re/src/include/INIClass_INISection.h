#pragma once
// ==========================================================================
// INIClass_INISection.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class INIClass::INISection
// vtable       : 0x006a0410  (1 slots)
// constructor  : 0x004d4ad0
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

    INIClass_INISection();  // 0x004d4ad0
    virtual ~INIClass_INISection();

    // ---- data members (offsets from ctor 0x004d4ad0) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    // vtable-pointer stores are omitted: the compiler supplies the vptr.
    // The machine block begins at +0x40: the compiled end of the
    // base sub-object region. Evidence: Node at +0x0 (ends +0x40).
    // 5 recovered offset(s) below +0x40 lie inside the base
    // sub-object region and are NOT declared here: they are declared and
    // compiler-checked in the base's own header -- +0x0, +0x4, +0x8, +0xc, +0x3c
};
