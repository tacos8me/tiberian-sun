// CCINIClass.h — reconstructed from GAME.EXE (C&C Tiberian Sun, Westwood 1999)
//
// Evidence:
//   RTTI:  .?AVCCINIClass@@  (out/classes.json chd_va 0x6A8A40)
//          bases: CCINIClass (mdisp 0), INIClass (mdisp 0)
//   vtable: 0x006988BC (out/vtables.txt "class CCINIClass")
//   vtable[0] = 0x00403A20 (size 30) — the only virtual slot.
//   VectorClass<CCINIClass*>  vtable 0x006A0468
//   DynamicVectorClass<CCINIClass*> vtable 0x006A0484
//
// CCINIClass is the game-side INI wrapper: it derives from the WWLib
// INIClass and adds the "current file" bookkeeping used by the rules
// loader.  The single virtual is the destructor.

#ifndef CCINICLASS_H
#define CCINICLASS_H

#include "INIClass.h"

class CCINIClass : public INIClass
{
public:
    CCINIClass();
    virtual ~CCINIClass();               // vtable 0x006988BC[0] = 0x00403A20

    // ---- file loading --------------------------------------------------
    // /* ? */ Load/Reload entry points are the non-virtual members that
    // drive INIClass::Load; not isolated in this pass.
    int  Load(const char* filename);
    int  Reload(void);

    // ---- section enumeration -------------------------------------------
    // /* ? */ SectionCount / GetSectionName used by the rules loader.
    int  SectionCount(void) const;
    const char* GetSectionName(int index) const;

private:
    // /* ? */ member layout not proven; the class adds file-name state
    // on top of the INIClass section list.
    char  m_filename[260];   // /* ? */

    // ---- data members (offsets from ctor 0x004d993c) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x6988bc
    unsigned char pad_004[0x18];
    unsigned long m_01c;  // /* ? */
    unsigned long m_020;  // /* ? */
    unsigned long m_024;  // /* ? */
    unsigned long m_028;  // /* ? */
    unsigned long m_02c;  // /* ? */
    unsigned long m_030;  // /* ? */
    unsigned long m_034;  // /* ? */

};

#endif // CCINICLASS_H
