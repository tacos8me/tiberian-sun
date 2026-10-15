// Shared prelude for the reconstructed Tiberian Sun headers.
// Supplies ONLY the types that no reconstructed header defines: core geometry
// and the out-of-scope UI gadget bases. Everything else (IUnknown, IPersist,
// IPersistStream, IRTTITypeInfo, IHouse, RadioClass, ...) is defined by the
// reconstructed headers themselves from real vtable evidence.
#pragma once
#include <windows.h>

// --- core geometry types (original: COORD.H / DEFINES.H) -------------------
struct CoordStruct { int X, Y, Z; };
struct CellStruct  { short X, Y; };

// --- COM / OLE bits --------------------------------------------------------
typedef GUID CLSID;
struct ULARGE_INTEGER { unsigned long LowPart; unsigned long HighPart; };
struct IStream;
struct ISequentialStream {
    virtual HRESULT __stdcall _dummy_ISS() = 0;
};
struct ILinkStream {
    virtual HRESULT __stdcall _dummy_ILS() = 0;
};
struct ISwizzle {
    virtual HRESULT __stdcall _dummy_ISwizzle() = 0;
};
struct IApplication {
    virtual HRESULT __stdcall _dummy_IApp() = 0;
};
struct IGameMap {
    virtual HRESULT __stdcall _dummy_IGameMap() = 0;
};
struct IFlyControl {
    virtual HRESULT __stdcall _dummy_IFly() = 0;
};
struct IClassFactory {
    virtual HRESULT __stdcall _dummy_ICF() = 0;
};
struct IConnectionPoint {
    virtual HRESULT __stdcall _dummy_ICP() = 0;
};
struct IEnumConnectionPoints {
    virtual HRESULT __stdcall _dummy_IECP() = 0;
};
struct IEnumConnections {
    virtual HRESULT __stdcall _dummy_IEC() = 0;
};

// --- UI gadget bases (original: GADGET.H / CONTROL.H) ----------------------
struct GScreenClass {
    virtual ~GScreenClass() {}
    virtual void _dummy_GScreen() {}
};
struct GadgetClass : public GScreenClass {
    virtual void _dummy_Gadget() {}
};
struct ControlClass : public GadgetClass {
    virtual void _dummy_Control() {}
};
struct PowerClass {
    virtual ~PowerClass() {}
};
struct LinkClass {
    virtual ~LinkClass() {}
};
struct VGadgetClass : public GadgetClass {};
struct VControlClass : public ControlClass {};
struct VLinkClass : public LinkClass {};
struct TabClass {
    virtual ~TabClass() {}
};
struct ScrollClass {
    virtual ~ScrollClass() {}
};
struct MouseClass {
    virtual ~MouseClass() {}
};
struct LayerClass {
    virtual ~LayerClass() {}
};
struct LogicClass {
    virtual ~LogicClass() {}
};
