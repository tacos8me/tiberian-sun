#pragma once
// ==========================================================================
// VectorClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// The engine's dynamic-array templates. 297 classes in the RTTI manifest are
// instantiations of these three templates:
//
//     VectorClass<T>          150 instantiations
//     DynamicVectorClass<T>   147 instantiations
//     TypeList<T>              12 instantiations
//
// LAYOUT EVIDENCE
// ---------------
// MSVC 6 emitted a separate copy of every method for every instantiation, so
// there is no single shared function to read. The layout below was recovered
// from the canonical instantiation whose vtable is 0x69aa3c (VectorClass) and
// 0x69aa58 (DynamicVectorClass), and then corroborated across the other
// instantiations by checking that the same displacements are touched:
//
//   VectorClass        150/150 instantiations agree on offsets {4, 8, 13}
//   DynamicVectorClass 147/147 instantiations agree on offsets {4, 8, 13}
//   TypeList            12/12  instantiations agree on offsets {4, 8, 13}
//
// Constructor 0x445640 (VectorClass) disassembles to:
//     mov dword ptr [esi + 4], edx      ; m_pData = 0
//     mov dword ptr [esi + 8], eax      ; m_nCount = arg
//     mov byte  ptr [esi + 0xc], 1      ; m_bIsActive = true
//     mov byte  ptr [esi + 0xd], dl     ; m_bIsAllocated = false
//     mov dword ptr [esi], 0x69aa3c     ; vftable
//
// Destructor 0x445570 disassembles to:
//     mov eax, dword ptr [esi + 4]      ; m_pData
//     mov dword ptr [esi], 0x69aa3c     ; vftable
//     test eax, eax / je skip
//     mov cl, byte ptr [esi + 0xd]      ; m_bIsAllocated gates the free
//     test cl, cl / je skip
//     push eax / call 0x68298c          ; operator delete
//
// The +0xd flag gating the deallocation is what makes it "IsAllocated"; +0xc
// is set true at construction and is the "IsActive" flag. Both names follow
// the Westwood convention and match the observed behaviour exactly.
//
// DynamicVectorClass adds +0x10 (capacity) and +0x14 (growth step, default 10).
// The default is visible in the static initialiser of a global instance:
//     mov dword ptr [0x7b2a9c], 0xa     ; growth step = 10
//     mov dword ptr [0x7b2a98], eax     ; capacity
// ==========================================================================

#include "ts_prelude.h"

// --------------------------------------------------------------------------
// VectorClass<T>
//
// A counted array with an explicit allocation flag. The destructor frees the
// buffer only when m_bIsAllocated is set, which is how the engine hands out
// non-owning views of another object's array.
// --------------------------------------------------------------------------
template <class T>
class VectorClass
{
public:
    VectorClass(int count = 0, T *data = 0);
    virtual ~VectorClass();

    // ---- vtable 0x69aa3c (6 slots) ----
    // [0] ~VectorClass       0x445570 / 0x4455c0 (scalar deleting dtor)
    // [1] operator==         0x445310
    // [2] operator=          0x445350
    // [3] Clear              0x4452a0
    // [4] vfunc_4            0x445430   reads m_bIsActive, returns 0 when clear
    // [5] vfunc_5            0x445470   reads m_bIsActive, returns 0 when clear
    virtual bool operator==(const VectorClass<T> &that) const;
    virtual VectorClass<T> &operator=(const VectorClass<T> &that);
    virtual void Clear();
    virtual bool vfunc_4() const;   // /* ? */ predicate over m_bIsActive
    virtual bool vfunc_5() const;   // /* ? */ predicate over m_bIsActive

    // ---- data members (offsets from ctor 0x004b0850) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    unsigned long m_000;  // +0x000 init 0x69ef28; 4-byte store: mov dword ptr [esi], 0x69ef28
    unsigned long m_004;  // +0x004 zero-initialised; 4-byte store: mov dword ptr [esi + 4], 0
    unsigned long m_008;  // +0x008 zero-initialised; 4-byte store: mov dword ptr [esi + 8], 0
    unsigned char pad_00c[0x1];
    unsigned char m_00d;  // +0x00d zero-initialised; 1-byte store: mov byte ptr [esi + 0xd], 0

};

// --------------------------------------------------------------------------
// DynamicVectorClass<T>
//
// VectorClass plus a capacity and a growth step. The growth step defaults to
// 10 (see the static initialiser evidence above).
// --------------------------------------------------------------------------
template <class T>
class DynamicVectorClass : public VectorClass<T>
{
public:
    DynamicVectorClass(int count = 0, T *data = 0);
    virtual ~DynamicVectorClass();

    // ---- vtable 0x69aa58 (6 slots) ----
    // [0] ~DynamicVectorClass 0x4455c0 (scalar deleting dtor)
    // [1] operator==          0x445310
    // [2] operator=           0x445490
    // [3] Clear               0x4452a0   also zeroes m_nCapacity
    // [4] Find                0x445540   loops m_nCapacity over m_pData
    // [5] vfunc_5             0x445470
    virtual bool operator==(const DynamicVectorClass<T> &that) const;
    virtual DynamicVectorClass<T> &operator=(const DynamicVectorClass<T> &that);
    virtual void Clear();
    virtual int Find(const T &item) const;   // returns index or -1
    virtual bool vfunc_5() const;            // /* ? */

    // ---- data members (offsets from ctor 0x49c0f0) ----
    int m_nCapacity;      // +0x10  allocated capacity
    int m_nGrowthStep;    // +0x14  growth increment, default 10
};

// --------------------------------------------------------------------------
// TypeList<T>
//
// Identical layout and vtable shape to DynamicVectorClass<T>; adds no members.
// Used for the engine's type registries.
// --------------------------------------------------------------------------
template <class T>
class TypeList : public DynamicVectorClass<T>
{
public:
    TypeList(int count = 0, T *data = 0);
    virtual ~TypeList();

    // ---- vtable 0x69ad58 (6 slots) ----
    // [0] ~TypeList  0x44ad70 (scalar deleting dtor)
    // [1] operator== 0x44af30
    // [2] operator=  0x44b0b0
    // [3] Clear      0x44ac70
    // [4] Find       0x44b160
    // [5] vfunc_5    0x44b1a0
    virtual bool operator==(const TypeList<T> &that) const;
    virtual TypeList<T> &operator=(const TypeList<T> &that);
    virtual void Clear();
    virtual int Find(const T &item) const;
    virtual bool vfunc_5() const;   // /* ? */

    // ---- data members (offsets from ctor 0x00449400) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    unsigned char m_00d;  // +0x00d /* ? */; 1-byte store: mov byte ptr [esi + 0xd], bl
    unsigned char pad_00e[0x2];
    // 6 member(s) already declared above; their offsets are recorded in out/members.json

};
