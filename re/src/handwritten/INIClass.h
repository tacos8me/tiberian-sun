// INIClass.h — reconstructed from GAME.EXE (C&C Tiberian Sun, Westwood 1999)
//
// Evidence:
//   RTTI:  .?AVINIClass@@  (out/classes.json chd_va 0x6A8A78)
//   vtable: 0x006A0400 (out/vtables.txt "class INIClass")
//   Nested types: INIClass::INIEntry (vtable 0x006A0408),
//                 INIClass::INISection (vtable 0x006A0410)
//   Node<INISection*,INIClass> vtable 0x006A0418
//   Original module: WWLib INIClass (Westwood "INI.CPP").
//
// The class is a section/key/value store backed by a linked list of
// INISection nodes, each holding a list of INIEntry nodes.  All accessors
// are __thiscall with `this` in ECX; the reader functions in the type
// classes call them with (section, key, default) in that order.
//
// Accessor VAs recovered from the call sites in the type-class readers
// (see SCHEMA.md).  They are listed here as the public API surface.

#ifndef INICLASS_H
#define INICLASS_H

#include <windows.h>

// Node<T,Key> is the WWLib intrusive list node used by the section/entry
// lists.  Evidence: RTTI "class Node<INISection*,INIClass>" (vtable
// 0x006A0418) and "class Node<INIEntry*,INIClass>"; both derive from
// GenericNode (vtable 0x006988C4).  Only the base shape is proven here.
class GenericNode
{
public:
    // The class has a vtable of its own (RTTI 0x006988C4, one slot) and its
    // constructor 0x00403960 writes it at +0x0, so the vptr is not optional
    // here: without it the compiler puts the links at +0x0 and +0x4 and every
    // derived class's base sub-object is 4 bytes low (INIClass::INISection's
    // recovered +0x8 and +0x3c landed at +0xc and +0x40).
    virtual ~GenericNode();
    GenericNode* Next;   // +0x4 /* ? */ list link
    GenericNode* Prev;   // +0x8 /* ? */ list link

    // ---- data members (offsets from ctor 0x00403960) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    // vtable-pointer stores are omitted: the compiler supplies the vptr.
    // The machine block begins at +0xc: the compiled end of the
    // base sub-object region (+0x4) and of the curated block
    // above. Base evidence: no base: the compiler's own vptr occupies +0x0.
    // 1 recovered offset(s) below +0x4 lie inside the base
    // sub-object region and are NOT declared here: they are declared and
    // compiler-checked in the base's own header -- +0x0
    // 2 recovered offset(s) are inside the curated hand-written block
    // above (its span is +0x4..+0xc) and are NOT repeated here:
    // the curated block wins -- +0x4, +0x8

};

template <class T, class Key> class Node : public GenericNode
{
public:
    T    Item;           // /* ? */ payload (INISection* / INIEntry*)

    // ---- data members (offsets from ctor 0x004d4ad0) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    // vtable-pointer stores are omitted: the compiler supplies the vptr.
    // The machine block begins at +0x10: the compiled end of the
    // base sub-object region (+0xc) and of the curated block
    // above. Base evidence: GenericNode at +0x0 (ends +0xc).
    // 3 recovered offset(s) below +0xc lie inside the base
    // sub-object region and are NOT declared here: they are declared and
    // compiler-checked in the base's own header -- +0x0, +0x4, +0x8
    // 1 recovered offset(s) are inside the curated hand-written block
    // above (its span is +0xc..+0x10) and are NOT repeated here:
    // the curated block wins -- +0xc
    unsigned char pad_010[0x2c];
    unsigned int m_03c;  // +0x03c /* ? */; 4-byte store: mov dword ptr [ebx + 0x3c], ecx

};

class INIClass
{
public:
    // ---- nested record types (RTTI: INIClass::INIEntry / INIClass::INISection)
    struct INIEntry : public Node<INIEntry*, INIClass>
    {
        char*  Name;        // key name
        char*  Value;       // value text
        // /* ? */ exact field order beyond the Node base is not proven
    };

    struct INISection : public Node<INISection*, INIClass>
    {
        char*  Name;        // section name
        // /* ? */ entry list head follows the Node base
    };

public:
    INIClass();
    virtual ~INIClass();                 // vtable 0x006A0400

    // ---- section management -------------------------------------------
    // 0x4D44C0 — clears the whole store (called from ~INIClass 0x4D4430)
    void  Clear();

    // ---- string accessors ---------------------------------------------
    // All of these are __thiscall(section, key, default) -> char*.
    // 0x4D60B0 / 0x4D6960 / 0x4D70B0 / 0x4D54C0 / 0x4D50F0 / 0x4D6030 /
    // 0x4D72B0 / 0x4D6290 are the GetString family (differing only in the
    // default-value handling / buffer size).
    const char* GetString(const char* section, const char* key, const char* def) const;
    // 0x4D6ED0 — GetString with an explicit destination buffer + length
    int         GetString(const char* section, const char* key, char* buf, int len) const;
    // 0x4D4250 — GetString into a StringClass (calls 0x44AE60 StringClass ctor)
    int         GetString(const char* section, const char* key, void* stringClass) const;

    // ---- scalar accessors ---------------------------------------------
    // 0x4D6B50 — GetInt(section, key, def); hashes the key via 0x46D710
    int         GetInt(const char* section, const char* key, int def) const;
    // 0x4D5D50 — GetInt variant (index-based)
    int         GetInt(const char* section, const char* key, int def, int idx) const;
    // 0x4D6B00 — GetDouble(section, key, def); formats via sprintf "%f" (0x682AAE)
    double      GetDouble(const char* section, const char* key, double def) const;
    // 0x4D7490 / 0x4D5C60 / 0x4D7070 — GetBool(section, key, def).
    // 0x4D7070 pushes the literal "yes" (0x6C9C28) then calls GetInt.
    bool        GetBool(const char* section, const char* key, bool def) const;

    // ---- writers (evidence: symmetric bodies next to the getters) ------
    // /* ? */ exact writer VAs not isolated in this pass; the store is
    // mutated through the INISection/INIEntry node lists.
    bool        PutString(const char* section, const char* key, const char* value);
    bool        PutInt(const char* section, const char* key, int value);
    bool        PutBool(const char* section, const char* key, bool value);

    // ---- file I/O ------------------------------------------------------
    // /* ? */ Load/Write entry points exist but were not isolated here.
    int         Load(const char* filename);
    int         Save(const char* filename);

private:
    // /* ? */ member layout: the section list head is reached through the
    // node helpers 0x4D8440 / 0x4D86B0 which walk `this + 0x1C`.
    void*       m_sectionList;   // /* ? */ offset 0x1C per 0x4D6B50/0x4D72B0

    // ---- data members (offsets from ctor 0x004d4430) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    // vtable-pointer stores are omitted: the compiler supplies the vptr.
    // The machine block begins at +0x8: the compiled end of the
    // base sub-object region (+0x4) and of the curated block
    // above. Base evidence: no base: the compiler's own vptr occupies +0x0.
    // 1 recovered offset(s) below +0x4 lie inside the base
    // sub-object region and are NOT declared here: they are declared and
    // compiler-checked in the base's own header -- +0x0
    unsigned char pad_008[0x14];
    unsigned int m_01c;  // +0x01c /* ? */; 4-byte store: mov dword ptr [esi + 0x1c], ebx
    unsigned int m_020;  // +0x020 /* ? */; 4-byte store: mov dword ptr [esi + 0x20], ebx
    unsigned int m_024;  // +0x024 /* ? */; 4-byte store: mov dword ptr [esi + 0x24], ebx
    unsigned char m_028;  // +0x028 /* ? */; 1-byte store: mov byte ptr [esi + 0x28], bl
    unsigned char pad_029[0x3];
    unsigned int m_02c;  // +0x02c /* ? */; 4-byte store: mov dword ptr [esi + 0x2c], ebx

};

#endif // INICLASS_H
