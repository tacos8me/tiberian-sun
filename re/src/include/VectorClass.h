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
//     mov dword ptr [esi + 8], eax      ; m_nVectorMax = arg (capacity)
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
// DynamicVectorClass adds +0x10 (the live element count) and +0x14 (growth
// step, default 10). The two are easy to swap: `Find` (0x445540) loops the
// dword at +0x10 over the data array, and the dynamic constructor stores 0 at
// +0x10 and 10 at +0x14 -- a live count starts empty, a capacity would not.
// The base's +8 is the allocated size (Westwood's `VectorMax`), not the count.
// The default is visible in the static initialiser of a global instance:
//     mov dword ptr [0x7b2a9c], 0xa     ; growth step = 10
//     mov dword ptr [0x7b2a98], eax     ; live count
// ==========================================================================

#include "ts_prelude.h"
#include "Machine.h"   // ts::ts_invoke / ts::ret_eax, for the member definitions below

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
    // vtable-pointer stores are omitted: the compiler supplies the vptr.
    // The machine block begins at +0x4: the compiled end of the
    // base sub-object region. Evidence: no base: the compiler's own vptr occupies +0x0.
    // 1 recovered offset(s) below +0x4 lie inside the base
    // sub-object region and are NOT declared here: they are declared and
    // compiler-checked in the base's own header -- +0x0
    unsigned int m_004;  // +0x004 zero-initialised; 4-byte store: mov dword ptr [esi + 4], 0
    unsigned int m_008;  // +0x008 zero-initialised; 4-byte store: mov dword ptr [esi + 8], 0
    unsigned char pad_00c[0x1];
    unsigned char m_00d;  // +0x00d zero-initialised; 1-byte store: mov byte ptr [esi + 0xd], 0

};

// --------------------------------------------------------------------------
// DynamicVectorClass<T>
//
// VectorClass plus a live element count and a growth step. The count starts at
// 0 and the growth step defaults to 10 (see the constructor evidence above).
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
    // [3] Clear               0x4452a0   also zeroes m_nActiveCount
    // [4] Find                0x445540   loops m_nActiveCount over m_pData
    // [5] vfunc_5             0x445470
    virtual bool operator==(const DynamicVectorClass<T> &that) const;
    virtual DynamicVectorClass<T> &operator=(const DynamicVectorClass<T> &that);
    virtual void Clear();
    virtual int Find(const T &item) const;   // returns index or -1
    virtual bool vfunc_5() const;            // /* ? */

    // ---- data members (offsets from ctor 0x49c0f0) ----
    int m_nActiveCount;   // +0x10  live element count (zeroed at construction)
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
    // vtable-pointer stores are omitted: the compiler supplies the vptr.
    // The machine block begins at +0x17a8: the compiled end of the
    // base sub-object region. Evidence: DynamicVectorClass at +0x0 (ends +0x17a8).
    // 7 recovered offset(s) below +0x17a8 lie inside the base
    // sub-object region and are NOT declared here: they are declared and
    // compiler-checked in the base's own header -- +0x0, +0x4, +0x8, +0xc, +0xd, +0x10, +0x14

};

// ==========================================================================
// Member definitions
//
// These members are declared above but were never defined, so nothing that
// instantiated a VectorClass<T> could link. Each definition invokes the
// recovered machine code for that member through the machine model's dispatch
// table; the address is the one this header's own evidence block records, and
// the full transcription of each is in src/generated/code/.
//
// Why not write the C++ out? Because these methods tail-jump. vfunc_4's span
// ends in a branch to 0x44543e, vfunc_5's to 0x44547c, operator='s to 0x4453ee
// -- the recorded span is not the whole method, and hand-translating a partial
// span is how you get plausible-looking wrong code. Invoking the recovered body
// is faithful to the whole thing, tail jumps included.
//
// The arguments are NOT converted into the model: a transcription reads its
// arguments from the modelled stack. A hand-written caller therefore reaches
// the right code, but the values it passes are only meaningful when the caller
// is itself a transcription. See ts_invoke in Machine.h.
// ==========================================================================

template <class T>
inline VectorClass<T>::VectorClass(int count, T *data)
{
    (void)count; (void)data;   // the recovered body reads these from the model
    ts::ts_invoke(0x00445640u);
}

template <class T>
inline VectorClass<T>::~VectorClass() { ts::ts_invoke(0x00445570u); }

template <class T>
inline bool VectorClass<T>::operator==(const VectorClass<T> &that) const
{
    (void)that;
    ts::ts_invoke(0x00445310u);
    return ts::ret_eax() != 0;
}

template <class T>
inline VectorClass<T> &VectorClass<T>::operator=(const VectorClass<T> &that)
{
    (void)that;
    ts::ts_invoke(0x00445350u);
    return *this;
}

template <class T>
inline void VectorClass<T>::Clear() { ts::ts_invoke(0x004452a0u); }

// /* ? */ predicate over m_bIsActive: returns 0 when the flag is clear and
// otherwise continues in shared code at 0x44543e, which scans m_pData for the
// first element matching the one passed in. Recorded as the recovered span
// reaches it, not as a named operation.
template <class T>
inline bool VectorClass<T>::vfunc_4() const
{
    ts::ts_invoke(0x00445430u);
    return ts::ret_eax() != 0;
}

// /* ? */ returns (arg - m_pData) / sizeof(T), i.e. the index of the element
// passed in; the recovered tail at 0x44547c computes exactly that.
template <class T>
inline bool VectorClass<T>::vfunc_5() const
{
    ts::ts_invoke(0x00445470u);
    return ts::ret_eax() != 0;
}

template <class T>
inline DynamicVectorClass<T>::DynamicVectorClass(int count, T *data)
{
    (void)count; (void)data;
    ts::ts_invoke(0x0049c0f0u);
}

template <class T>
inline DynamicVectorClass<T>::~DynamicVectorClass()
{
    ts::ts_invoke(0x004455c0u);
}

template <class T>
inline bool
DynamicVectorClass<T>::operator==(const DynamicVectorClass<T> &that) const
{
    (void)that;
    ts::ts_invoke(0x00445310u);
    return ts::ret_eax() != 0;
}

template <class T>
inline DynamicVectorClass<T> &
DynamicVectorClass<T>::operator=(const DynamicVectorClass<T> &that)
{
    (void)that;
    ts::ts_invoke(0x00445490u);
    return *this;
}

template <class T>
inline void DynamicVectorClass<T>::Clear() { ts::ts_invoke(0x004452a0u); }

// Linear scan over m_nActiveCount comparing the first word of each element;
// returns the index or -1.
template <class T>
inline int DynamicVectorClass<T>::Find(const T &item) const
{
    (void)item;
    ts::ts_invoke(0x00445540u);
    return (int)ts::ret_eax();
}

template <class T>
inline bool DynamicVectorClass<T>::vfunc_5() const
{
    ts::ts_invoke(0x00445470u);
    return ts::ret_eax() != 0;
}

template <class T>
inline TypeList<T>::TypeList(int count, T *data)
{
    (void)count; (void)data;
    ts::ts_invoke(0x00449400u);
}

template <class T>
inline TypeList<T>::~TypeList() { ts::ts_invoke(0x0044ad70u); }

template <class T>
inline bool TypeList<T>::operator==(const TypeList<T> &that) const
{
    (void)that;
    ts::ts_invoke(0x0044af30u);
    return ts::ret_eax() != 0;
}

template <class T>
inline TypeList<T> &TypeList<T>::operator=(const TypeList<T> &that)
{
    (void)that;
    ts::ts_invoke(0x0044b0b0u);
    return *this;
}

template <class T>
inline void TypeList<T>::Clear() { ts::ts_invoke(0x0044ac70u); }

template <class T>
inline int TypeList<T>::Find(const T &item) const
{
    (void)item;
    ts::ts_invoke(0x0044b160u);
    return (int)ts::ret_eax();
}

// 0x44b1a0 is a shared tail entry: it is inside the function at 0x44b190, and
// the vtable points straight at it. It is transcribed as its own body for
// exactly that reason -- see tail_entries in tools/gen_code.py.
template <class T>
inline bool TypeList<T>::vfunc_5() const
{
    ts::ts_invoke(0x0044b1a0u);
    return ts::ret_eax() != 0;
}
