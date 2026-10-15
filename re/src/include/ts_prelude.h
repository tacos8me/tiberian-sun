#pragma once
// ============================================================================
// ts_prelude.h — shared types for the reconstructed Tiberian Sun headers.
//
// This file supplies ONLY the types that the generated per-class headers do
// not define themselves:
//   - core geometry types (CoordStruct, CellStruct)
//   - COM / OLE interfaces (IUnknown, IPersist*, IStream, ...)
//   - ATL base classes used by the WOL networking layer
//   - a small number of classes that have no vtable of their own and so are
//     not emitted by the generator
//
// Everything else is defined by its own generated header from real RTTI and
// vtable evidence. Do not add a class here if the generator emits it.
// ============================================================================

#include <windows.h>

// --- core geometry types (original: COORD.H / DEFINES.H) -------------------
struct CoordStruct { int X, Y, Z; };
struct CellStruct  { short X; short Y; };

// --- COM / OLE -------------------------------------------------------------
typedef GUID CLSID;
struct ULARGE_INTEGER { unsigned long LowPart; unsigned long HighPart; };
struct LARGE_INTEGER  { long LowPart; long HighPart; };
struct IStream;

struct IUnknown {
    virtual HRESULT __stdcall QueryInterface(REFIID riid, void** ppv) = 0;
    virtual ULONG   __stdcall AddRef() = 0;
    virtual ULONG   __stdcall Release() = 0;
};
struct IPersist;   // defined by its own reconstructed header
struct IPersistStream;   // defined by its own reconstructed header
struct ISequentialStream {
    virtual HRESULT __stdcall Read(void* pv, ULONG cb, ULONG* pcbRead) = 0;
    virtual HRESULT __stdcall Write(const void* pv, ULONG cb, ULONG* pcbWritten) = 0;
};
struct IStream : public ISequentialStream {
    virtual HRESULT __stdcall Seek(LARGE_INTEGER dlibMove, DWORD dwOrigin,
                                   ULARGE_INTEGER* plibNewPosition) = 0;
    virtual HRESULT __stdcall SetSize(ULARGE_INTEGER libNewSize) = 0;
    virtual HRESULT __stdcall CopyTo(IStream* pstm, ULARGE_INTEGER cb,
                                     ULARGE_INTEGER* pcbRead,
                                     ULARGE_INTEGER* pcbWritten) = 0;
    virtual HRESULT __stdcall Commit(DWORD grfCommitFlags) = 0;
    virtual HRESULT __stdcall Revert() = 0;
    virtual HRESULT __stdcall LockRegion(ULARGE_INTEGER libOffset,
                                         ULARGE_INTEGER cb, DWORD dwLockType) = 0;
    virtual HRESULT __stdcall UnlockRegion(ULARGE_INTEGER libOffset,
                                           ULARGE_INTEGER cb, DWORD dwLockType) = 0;
    virtual HRESULT __stdcall Stat(void* pstatstg, DWORD grfStatFlag) = 0;
    virtual HRESULT __stdcall Clone(IStream** ppstm) = 0;
};
struct ILinkStream {
    virtual HRESULT __stdcall _dummy_ILinkStream() = 0;
};
struct ISwizzle {
    virtual HRESULT __stdcall _dummy_ISwizzle() = 0;
};
struct IApplication {
    virtual HRESULT __stdcall _dummy_IApplication() = 0;
};
struct IGameMap {
    virtual HRESULT __stdcall _dummy_IGameMap() = 0;
};
struct IFlyControl {
    virtual HRESULT __stdcall _dummy_IFlyControl() = 0;
};
struct IClassFactory {
    virtual HRESULT __stdcall CreateInstance(IUnknown* pUnkOuter, REFIID riid,
                                             void** ppvObject) = 0;
    virtual HRESULT __stdcall LockServer(BOOL fLock) = 0;
};
struct IConnectionPoint {
    virtual HRESULT __stdcall _dummy_IConnectionPoint() = 0;
};
struct IConnectionPointContainer;   // defined by its own reconstructed header
struct IEnumConnectionPoints {
    virtual HRESULT __stdcall _dummy_IEnumConnectionPoints() = 0;
};
struct IEnumConnections {
    virtual HRESULT __stdcall _dummy_IEnumConnections() = 0;
};
struct IRTTITypeInfo;   // defined by its own reconstructed header
struct IPiggyback;   // defined by its own reconstructed header
struct ILocomotion;   // defined by its own reconstructed header
struct IHouse;   // defined by its own reconstructed header
struct IPublicHouse;   // defined by its own reconstructed header
// --- WOL (Westwood Online) event interfaces --------------------------------
namespace WOL {
    struct IChatEvent {
        virtual HRESULT __stdcall _dummy_IChatEvent() = 0;
    };
    struct IDownloadEvent {
        virtual HRESULT __stdcall _dummy_IDownloadEvent() = 0;
    };
    struct INetUtilEvent {
        virtual HRESULT __stdcall _dummy_INetUtilEvent() = 0;
    };
}

// --- ATL bases used by the WOL COM layer -----------------------------------
namespace ATL {
    struct CComMultiThreadModel {};
    struct CComObjectRootBase {
        virtual ~CComObjectRootBase() {}
    };
    template <class ThreadModel, class T>
    struct CComObjectRootEx : public CComObjectRootBase {};
}

// --- classes with no vtable of their own (not emitted by the generator) ----
struct BounceClass {
    virtual ~BounceClass() {}
};
struct DiskSwap {
    virtual ~DiskSwap() {}
};
struct PowerClass;   // defined by its own reconstructed header
struct GScreenClass {
    virtual ~GScreenClass() {}
    virtual void _dummy_GScreen() {}
};
struct FlasherClass;   // defined by TechnoClass.h (richer vtable detail)
struct StageClass;   // defined by TechnoClass.h (richer vtable detail)
struct CDownloadEventSink {
    virtual ~CDownloadEventSink() {}
};
struct CNetUtilEventSink {
    virtual ~CNetUtilEventSink() {}
};

// --- container node templates referenced as bases --------------------------
// Node<T,Key> is defined by INIClass.h (hand-written header).
