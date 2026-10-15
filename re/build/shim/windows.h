// Minimal windows.h shim so the reconstructed headers compile on Linux.
#pragma once
typedef unsigned char      BYTE;
typedef unsigned short     WORD;
typedef unsigned long      DWORD;
typedef unsigned long      ULONG;
typedef long               LONG;
typedef long               HRESULT;
typedef int                BOOL;
typedef void*              HANDLE;
typedef void*              HWND;
typedef void*              HINSTANCE;
typedef char               CHAR;
typedef unsigned short     WCHAR;
typedef const char*        LPCSTR;
typedef char*              LPSTR;
typedef unsigned int       UINT;
typedef unsigned long long ULONGLONG;
typedef long long          LONGLONG;
#define TRUE 1
#define FALSE 0
#define S_OK 0
#define E_FAIL ((HRESULT)0x80004005L)
#define __stdcall
#define STDMETHODCALLTYPE
typedef struct _GUID { unsigned long a; unsigned short b, c; unsigned char d[8]; } GUID;
typedef GUID IID;
typedef const GUID& REFIID;
typedef const GUID& REFGUID;
typedef const GUID& REFCLSID;
