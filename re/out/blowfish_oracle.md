# `blowfish.dll!DllRegisterServer`: what the retail binary actually does

Status: measured 2026-10-17 against the retail binary under Wine 9.0 on this
host. This is the oracle for the model blocker in `todo.md` ("`blowfish.dll!
DllRegisterServer` fails, so the game takes its error box"). Every value below
is either a gdb read at the named VA, a Wine trace line, or a byte read from the
two binaries; the command that reproduces each class of evidence is in
[Reproduction](#7-reproduction).

Binaries:

| File | SHA256 |
|---|---|
| `gamedir/GAME.EXE` (= `re/bin/GAME.EXE`, `work/cd1/INSTALL/GAME.EXE`) | `2941b01998da0cd2bb0b7db30125d4130e3876a74d6cc5588b630c6d5fac5a12` |
| `gamedir/BLOWFISH.DLL` (= `work/cd1/INSTALL/BLOWFISH.DLL`) | `8597147f8a2f8263f0fd295ffc16ef6ae5b46e16eb7ab8812ae2c6d8c4a98de3` |

Environment: Xvfb `640x480x16`, `d:` mapped to `work/cd1` (label `TS1`,
`TS1.DSK` present), Wine prefix `.wine32`, no Westwood install key. This is the
same environment `tools/wine_env.sh` sets up and in which the retail game
reaches its main menu.

---

## 0. The answer for the implementing lane

1. **The first `CoCreateInstance` fails, by design, on a cold machine.** The
   retail game calls
   `CoCreateInstance({1440AD10-6AA8-11D1-B6F9-00A024DDAFD1}, NULL, 7,
   IID_IUnknown, &p)` at `0x5e620c`; it returns **`0x80040154`
   (`REGDB_E_CLASSNOTREG`)** and writes `NULL`. The model must reproduce that
   failure when the class is not registered, not return `S_OK`. (The current
   generic stub returns `S_OK` without writing `ppv`; the wrapper's temporary
   then stays `NULL` — it is the `pUnkOuter` argument slot — and `0x5e87f3`
   executes `mov (%eax),%ecx` on `NULL`.)
2. **The fallback is exactly the todo's path.** `LoadLibraryA("blowfish.dll")`
   returns **`0x11000000`**; `GetProcAddress(0x11000000, "DllRegisterServer")`
   returns **`0x11001050`**; `DllRegisterServer` returns **`S_OK` (0)** and
   writes three registry keys (section 3); the retry `CoCreateInstance` then
   returns **`S_OK`** and writes a real object pointer.
3. **The object is a real Blowfish cipher, and the game uses it.** During
   startup the game calls `SetKey(56, key)` **9 times** and `Decrypt(8, in, out)`
   **1586 times** on this object. The 9 keys are byte-identical to the keys
   derived from 9 shipped MIX archives, and 1586 is exactly the sum of those
   nine MIX index sizes in 8-byte blocks. A stub that returns `S_OK` and does
   not decrypt will make the MIX index garbage; **implement real Blowfish**.
4. The registration-loop object is released immediately. Later, the game's
   `BlowPipe` class creates its own object per archive
   (`CoCreateInstance(..., IID_IUnknown)` then
   `QueryInterface(IID {E0113100-6A7C-11D1-B6F9-00A024DDAFD1})`), calls
   `SetKey`, streams the encrypted MIX index through `Decrypt`, and releases.
5. The object's vtable is 8 entries, no COM base-class magic:
   `QueryInterface, AddRef, Release, SetKey, GetKeySize, GetBlockSize, Encrypt,
   Decrypt` (section 4.1). `OleRun` succeeds (`S_OK`) even though the object
   does not implement `IRunnableObject`.

Minimal faithful implementation: special-case the CLSID in `CoCreateInstance`
(a per-process "registered" flag), implement `DllRegisterServer`/
`DllUnregisterServer` as `S_OK` and set/clear that flag, and back the object
with a standard Blowfish key schedule. If the class is made to succeed without
the fallback, the observable game state is the same (section 6), but the
`LoadLibraryA`/`GetProcAddress`/`DllRegisterServer` path is then never exercised.

---

## 1. The game's registration loop, statically

All addresses are VAs in `GAME.EXE` at its preferred base `0x400000`.

```
5e61e6  xor  %edi,%edi
5e61e8  push %edi
5e61e9  call *0x698550              ; OleInitialize(NULL)
5e61ef  mov  $0x6a4848,%esi         ; table of {CLSID*, dll-name*} pairs
5e61f4  mov  0x69816c,%ebx          ; LoadLibraryA
5e61fa  mov  %edi,-0x604(%ebp)      ; out = NULL
5e6200  push $0x7                   ; CLSCTX = INPROC_SERVER|INPROC_HANDLER|LOCAL_SERVER
5e6202  push %edi                   ; pUnkOuter = NULL
5e6203  mov  (%esi),%eax            ; rclsid
5e6205  push %eax
5e6206  lea  -0x604(%ebp),%ecx
5e620c  call 0x5e87a0               ; thiscall CoCreateInstance wrapper
5e6211  cmp  %edi,%eax
5e6213  setl %al                    ; flag = (hr < 0)
5e6216  mov  %al,-0x5f4(%ebp)
5e621c  test %al,%al
5e621e  je   0x5e6272              ; success: skip the fallback
5e6220  movb $0,-0x5f4(%ebp)        ; clear the flag before retrying
5e6227  mov  0x4(%esi),%ecx         ; dll name
5e622a  push %ecx
5e622b  call *%ebx                  ; LoadLibraryA(name)
5e622d  mov  %eax,%edi
5e622f  test %edi,%edi
5e6231  je   0x5e626b              ; NULL -> flag = 1
5e6233  push $0x6d9b34              ; "DllRegisterServer"
5e6238  push %edi
5e6239  call *0x698174              ; GetProcAddress(hm, name)
5e623f  test %eax,%eax
5e6241  je   0x5e625b              ; NULL -> flag = 1
5e6243  call *%eax                  ; DllRegisterServer(); return value ignored
5e6245  push $0x7                   ; retry CoCreateInstance, same arguments
...
5e6257  test %eax,%eax
5e6259  jge  0x5e6262              ; success -> leave the flag 0
5e625b  movb $1,-0x5f4(%ebp)
5e6262  push %edi
5e6263  call *0x6981f8              ; FreeLibrary(hm)
```

The table is one entry; `0x6a4848` holds `0x0069e960` and `0x6a484c` holds
`0x006d99d4`. The loop bound is `0x6a4850`, so it runs once.

| Address | Bytes | Meaning |
|---|---|---|
| `0x6a4848` | `60 e9 69 00` | `0x0069e960`, pointer to the CLSID |
| `0x6a484c` | `d4 99 6d 00` | `0x006d99d4`, pointer to `"blowfish.dll"` |
| `0x69e960` | `10 ad 40 14 a8 6a d1 11 b6 f9 00 a0 24 dd af d1` | `{1440AD10-6AA8-11D1-B6F9-00A024DDAFD1}` |
| `0x6d99d4` | ASCII | `"blowfish.dll"` |
| `0x6d9b34` | ASCII | `"DllRegisterServer"` |
| `0x6bf5f8` | `00*8 c0 00 00 00 00 00 00 46` | `IID_IUnknown {00000000-0000-0000-C000-000000000046}` |

The second CLSID in the todo, `0x69e970`
(`{B45A4A80-86DA-11D1-B706-00A024DDAFD1}`), is **not** part of this path: the
table contains only `0x69e960`. `0x69e980`
(`{B45A4A81-86DA-11D1-B706-00A024DDAFD1}`) is used by a different routine
(`0x5e5570`, a class-object registration), not here.

### 1.1 The wrapper at `0x5e87a0`

Signature (thiscall): `HRESULT wrap(IUnknown** out, REFCLSID rclsid, IUnknown*
pUnkOuter, DWORD dwClsContext)`, `ret $0xc`. It releases any existing `*out`,
then:

* if `(dwClsContext & 0x14) != 0` (a server context): `CoCreateInstance` into a
  temporary, `OleRun(temp)`, `temp->QueryInterface(IID_IUnknown, out)`,
  `temp->Release()`;
* otherwise: `CoCreateInstance` straight into `out`.

The game passes `dwClsContext = 7`, so `7 & 0x14 == 4` and the `OleRun` path is
taken. IAT slots: `0x69857c = ole32!CoCreateInstance`, `0x698580 =
ole32!OleRun`, `0x69816c = kernel32!LoadLibraryA`, `0x698174 =
kernel32!GetProcAddress`, `0x6981f8 = kernel32!FreeLibrary`.

### 1.2 The failure this produces

`-0x5f4(%ebp)` is the flag tested after init. If it is non-zero the game calls
`MessageBoxA(NULL, ..., ..., 0x30)` at `0x5e72f5` and `ExitProcess(1)` at
`0x5e7301`:

```
5e72d1  mov  -0x5f4(%ebp),%bl
5e72d7  test %bl,%bl
5e72d9  je   0x5e7306
...
5e72f5  call *0x698460              ; MessageBoxA, type 0x30
5e72fb  test %bl,%bl
5e72fd  je   0x5e7306
5e72ff  push $0x1
5e7301  call 0x684446               ; ExitProcess(1)
```

---

## 2. The measured cold-start sequence

Key deleted first (`wine reg delete HKCR\CLSID\{1440AD10-...} /f`), game
started and stopped by `SIGSTOP` before init so gdb can break in the loop (a
normal attach lands after it). Evidence: `/tmp/blowfish_oracle_cold_gdb.log`,
generated by section 7.

| # | Site | Measured value | Evidence |
|---|---|---|---|
| 1 | first `CoCreateInstance` (`0x5e6211`) | `hr = 0x80040154`, `out = 0` | `wrapper_CoCreate #1 hr=80040154 tmp=00000000`; `FIRST_CoCreate #1 hr=80040154 out=00000000` |
| 2 | `LoadLibraryA` (`0x5e622d`) | `hm = 0x11000000` | `LoadLibraryA #1 hm=11000000 name=blowfish.dll`; relay `Ret KERNEL32.LoadLibraryA() retval=11000000 ret=005e622d` (`/tmp/blowfish_relay.log:13567`) |
| 3 | `GetProcAddress` (`0x5e623f`) | `proc = 0x11001050` | `GetProcAddress #1 proc=11001050`; relay `retval=11001050 ret=005e623f` (`:13569`) |
| 4 | `DllRegisterServer` (`0x5e6245`) | `hr = 0` (`S_OK`) | `DllRegisterServer #1 hr=00000000` |
| 5 | retry `CoCreateInstance` (`0x5e6257`) | `hr = 0`, `out = 0x01ea2a08` | `wrapper_CoCreate #2 hr=00000000 tmp=01ea2a08`; `RETRY_CoCreate #1 hr=00000000 out=01ea2a08` |
| 6 | `OleRun` inside wrapper | `hr = 0` | `wrapper_OleRun #1 hr=00000000`; relay `Call ole32.OleRun(01fa2a18)` / `retval=00000000 ret=005e87e2` (`:13586-13588`) |
| 7 | `QueryInterface(IID_IUnknown)` | `hr = 0`, object `0x01ea2a08` | `wrapper_QueryInt #1 hr=00000000 out=0031f8c0 obj=01ea2a08 vptr=1101e480` |
| 8 | game releases the object | object `0x01ea2a08`, vptr `0x1101e480` | `Release_created #1 obj=01ea2a08 vptr=1101e480` |

The +ole trace shows the two `CoCreateInstance` calls and the reason the first
fails (`/tmp/blowfish_relay.log`):

```
13459 trace:ole:CoCreateInstance {1440ad10-6aa8-11d1-b6f9-00a024ddafd1}, 00000000, 0x7, {00000000-0000-0000-c000-000000000046}, 0031F520.
13464 err:ole:com_get_class_object class {1440ad10-...} not registered
13465 err:ole:com_get_class_object no class object {1440ad10-...} could be created for context 0x7
13534 Call KERNEL32.LoadLibraryA(006d99d4 "blowfish.dll") ret=005e622d
13560 trace:module:load_dll Loaded module ...blowfish.dll at 11000000
13567 Ret  KERNEL32.LoadLibraryA() retval=11000000 ret=005e622d
13568 Call KERNEL32.GetProcAddress(11000000,006d9b34 "DllRegisterServer") ret=005e623f
13569 Ret  KERNEL32.GetProcAddress() retval=11001050 ret=005e623f
13572 trace:ole:CoCreateInstance {1440ad10-...}, 00000000, 0x7, {00000000-...-c000-000000000046}, 0031F520.
13586 Call ole32.OleRun(01fa2a18) ret=005e87e2
13588 Ret  ole32.OleRun() retval=00000000 ret=005e87e2
```

`DllRegisterServer` is called through a `GetProcAddress` pointer, so Wine's
relay (which hooks import thunks) cannot log it; the gdb breakpoint at
`0x5e6245` is the direct measurement of its return value. `BLOWFISH.DLL` is
loaded by `LoadLibraryA` at line 13560, before the retry.

**Warm path, measured for contrast.** With the CLSID key already present, the
first `CoCreateInstance` returns `S_OK` and the fallback is skipped entirely:
`FIRST_CoCreate #1 hr=00000000 out=01ea2a08`, and no `LoadLibraryA`,
`GetProcAddress`, `DllRegisterServer` or retry breakpoint fires (evidence:
`/tmp/blowfish_oracle_warm_gdb.log`). The model starts with no registry, so the
cold path is the one it must match if it wants to exercise the export.

---

## 3. `DllRegisterServer`: what it does

`BLOWFISH.DLL` ImageBase `0x11000000`; export table:

| Export | RVA | Body |
|---|---|---|
| `DllCanUnloadNow` | `0x1037` | `0x11002353` |
| `DllGetClassObject` | `0x10af` | `0x1100228c` |
| `DllRegisterServer` | `0x1050` | `0x1100243a` |
| `DllUnregisterServer` | `0x106e` | `0x11002511` |

`DllRegisterServer` (thiscall-free `HRESULT WINAPI`, no args):

1. `GetModuleFileNameA(hInstance, path, 0x104)` — the DLL's own full path.
2. `StringFromGUID2(&CLSID, guid, 0x27)` → `{1440AD10-6AA8-11D1-B6F9-00A024DDAFD1}`.
3. Builds `"CLSID\" + guid` and calls a helper (`0x1100236a`) three times:
   * `RegCreateKeyExA(HKEY_CLASSES_ROOT, "CLSID\{1440AD10-...}", ..., KEY_ALL_ACCESS=0xF003F, ...)`
     then `RegSetValueExA(hkey, NULL, 0, REG_SZ, "Blowfish Block Cypher Engine")`;
   * the same with `\NotInsertable` and no value;
   * the same with `\InprocServer32` and the module path as the default value.
4. Returns `[ebp-0x160]`, initialised to `0` at `0x11002443` and never written:
   **`S_OK` unconditionally**.

The runtime proof that this ran on a cold start is the registry subtree it
leaves behind (the only writer of it is this export). Captured after the cold
run:

```
HKEY_CLASSES_ROOT\CLSID\{1440AD10-6AA8-11D1-B6F9-00A024DDAFD1}
    (Default)    REG_SZ    Blowfish Block Cypher Engine
HKEY_CLASSES_ROOT\CLSID\{1440AD10-6AA8-11D1-B6F9-00A024DDAFD1}\NotInsertable
HKEY_CLASSES_ROOT\CLSID\{1440AD10-6AA8-11D1-B6F9-00A024DDAFD1}\InprocServer32
    (Default)    REG_SZ    Z:\mnt\nvme-1\projects\tiberian\gamedir\blowfish.dll
```

The `InprocServer32` path is whatever `GetModuleFileNameA` returned, i.e. where
the DLL was loaded from; it is not a constant. The game itself ignores
`DllRegisterServer`'s return value (`0x5e6243` is a bare `call *%eax`).

### 3.1 How OLE gets the object (`DllGetClassObject`)

The retry succeeds because the registration made OLE able to load the in-proc
server. Wine's trace shows the sequence:

```
apartment_add_dll ...gamedir\blowfish.dll
apartment_getclassobject calling DllGetClassObject 110010AF
```

`DllGetClassObject` (`0x1100228c`) compares `rclsid` with the CLSID constant at
`0x1101e460`; on a mismatch it returns `0x80040111`
(`CLASS_E_CLASSNOTAVAILABLE`). On a match it allocates an 8-byte class factory
(constructor `0x110013a1`, vptr `0x1101d070`, refcount at `+4`) and calls its
`QueryInterface(riid, ppv)`.

Class factory vtable `0x1101d070`:

| Index | Address | Method | Behaviour |
|---|---|---|---|
| 0 | `0x11001170` | `QueryInterface` | accepts `IID_IUnknown` (`0x1101e4e0`) and `IID_IClassFactory` (`0x1101e4d0`); returns `this`; else `E_NOINTERFACE` |
| 1 | `0x110011f0` | `AddRef` | `InterlockedIncrement(this+4)` |
| 2 | `0x11001204` | `Release` | decrement; free at 0 |
| 3 | `0x11001251` | `CreateInstance(pUnkOuter, riid, ppv)` | `pUnkOuter != NULL` → `0x80040110` (`CLASS_E_NOAGGREGATION`); `riid` must be `{E0113100-6A7C-11D1-B6F9-00A024DDAFD1}` (`0x1101d058`) or `IID_IUnknown`; allocates `0x1099` bytes, constructs the cipher object, calls its `QueryInterface` |
| 4 | `0x11001372` | `LockServer(BOOL)` | increments/decrements the global object count; `S_OK` |

The class factory is internal to OLE and is not something the model has to
model if its `CoCreateInstance` special-case constructs the cipher object
directly.

---

## 4. The object `CoCreateInstance` returns

### 4.1 Vtable

Measured from the object pointer at `0x5e629b` / `0x5e87f7`:
`vptr = 0x1101e480`, first 8 entries:

```
11001005 1100102d 11001023 11001078 110010a0 11001073 1100104b 11001064
```

Each entry is a 5-byte `jmp` stub into the real method:

| Index | Stub | Method | Semantics |
|---|---|---|---|
| 0 | `0x11001005` | `0x11001f30` | `QueryInterface(this, riid, ppv)`: accepts `IID_IUnknown` (`0x1101e4e0`) and `{E0113100-6A7C-11D1-B6F9-00A024DDAFD1}` (`0x1101e440`); writes `this`, `AddRef`s, returns `S_OK`; `ppv==NULL` → `E_POINTER`; else `E_NOINTERFACE` |
| 1 | `0x1100102d` | `0x11001fb0` | `AddRef(this)`: `InterlockedIncrement(this+4)` |
| 2 | `0x11001023` | `0x11001fc4` | `Release(this)`: decrement; free at 0 |
| 3 | `0x11001078` | `0x11002011` | `SetKey(this, int size, const void* key)`: `key==NULL` → `E_POINTER`; `size` outside `0..56` → `E_INVALIDARG`; builds the schedule; `S_OK` |
| 4 | `0x110010a0` | `0x1100204d` | `GetKeySize(this, DWORD* out)`: `*out = 56`; `S_OK` |
| 5 | `0x11001073` | `0x1100206c` | `GetBlockSize(this, DWORD* out)`: `*out = 8`; `S_OK` |
| 6 | `0x1100104b` | `0x1100208b` | `Encrypt(this, int len, const void* in, void* out)`: processes `len/8` blocks, copies the tail verbatim, `S_OK` |
| 7 | `0x11001064` | `0x110020cb` | `Decrypt(this, int len, const void* in, void* out)`: same, with the reversed P-array |

Index 8 is a `0` terminator. `Encrypt`/`Decrypt` reject a NULL `in` or `out`
with `E_POINTER` and a negative `len` with `E_INVALIDARG`; `in == out` is
supported (the game always uses it).

### 4.2 It is standard Blowfish

The key schedule (`0x110015c2`) copies the initial P-array from `0x1101d0b0`
(18 dwords, first is `0x243f6a88`, the standard Blowfish constant) and the
S-boxes from `0x1101d0f8` (4 x 256 dwords), then runs the standard Blowfish
key schedule. It builds two P-arrays:

* forward P at object `+9` (used by method 6, `Encrypt`, via `0x1100183c`);
* reversed P at object `+0x51` (used by method 7, `Decrypt`, via `0x11001919`);
* shared S-boxes at object `+0x99`.

Object size is `0x1099` bytes (`CreateInstance` allocates `0x1099`); layout:
`+0` vptr, `+4` refcount, `+8` initialised byte, `+9` forward P, `+0x51`
reverse P, `+0x99` S-boxes. The key is whatever the caller supplies; the game
derives it from each MIX header, so the stub does **not** need the Westwood RSA
step.

Method 6/7 take `(len, in, out)` and return `S_OK`; the game ignores the return
(`0x423439`/`0x423469` return the output pointer, not the HRESULT).

`SetKey` also maintains the initialised byte at object `+8`: a successful key
setup sets it to 1 (`0x11001832`), and a call with `key == NULL` or `size == 0`
clears it (`0x11001639`). When it is 0, Encrypt/Decrypt copy input to output
unchanged (`0x110018f4`/`0x110019d1`). The game always sets a key first, so this
only matters for a faithful no-key path.

---

## 5. The object is used for decryption during startup

### 5.1 The second consumer: `BlowPipe`

The game does not only create the object in the registration loop. It has a
class `BlowPipe` (MSVC RTTI `.?AVBlowPipe@@`; COL `0x6b37d0`, vtable
`0x6a3258`) whose constructor helper `0x423360` does:

```
423360  push %ecx                     ; local slot for the temp object
423367  push &local
423368  push $0x6bf5f8                ; IID_IUnknown
42336f  push $0x7                     ; CLSCTX
423371  push $0
423373  push $0x69e960                ; the same CLSID
42337e  call *0x69857c                ; CoCreateInstance
42338a  ...
42338f  call *0x698580                ; OleRun(temp)
42339b  ...
4233a0  push $0x6c0430                ; IID {E0113100-6A7C-11D1-B6F9-00A024DDAFD1}
4233a8  call *(%edx)                  ; temp->QueryInterface(IID_Blowfish, out)
4233b3  call *0x8(%ecx)               ; temp->Release()
```

The interface IID is the 16 bytes at `0x6c0430`:
`{E0113100-6A7C-11D1-B6F9-00A024DDAFD1}`. `BlowPipe::Put` (`0x4234b0`) buffers
input in 8-byte blocks; when a block is full it calls, through the interface
vtable:

* mode `[this+0x1c] == 1` → `call *0x1c(%ecx)` = index 7 = `Decrypt`;
* otherwise → `call *0x18(%ecx)` = index 6 = `Encrypt`;

with `(len=8, in=block, out=block)`. The game decrypts MIX indexes, so mode is
1 and only `Decrypt` is called. The lazily-created interface is reached through
a 4-byte holder whose pointer sits at `BlowPipe+0xc`; `SetKey` is called through
index 3 from the `0x4235e0`/`0x423710` methods with `(56, key)`.

### 5.2 Measured: 9 keys, 1586 blocks, and they are the nine MIX indexes

Cold run, full startup (all nine indexes finish in under 20 s): `BF_SetKey =
9`, `BF_Decrypt = 1586`, `BF_Encrypt = 0`, `BlowPipe_CoCreate = 9`, each with
`hr = 0`. The key bytes and per-key block counts from the gdb log match the
shipped archives exactly:

| # | key (first 8 bytes) | blocks since previous key | archive | index blocks |
|---|---|---|---|---|
| 1 | `82 06 03 23 1a 9d b6 43` | 0 (first) | `TIBSUN.MIX` (17 entries) | 27 |
| 2 | `77 08 f7 24 c5 f1 3e 89` | 27 | `TIBSUN.MIX` entry `0x3b5a96de` (26 entries) | 40 |
| 3 | `84 85 f0 3b bb 2d 11 af` | 40 | `TIBSUN.MIX` entry `0xa8548fd9` (317 entries) | 477 |
| 4 | `c0 3b 5a 61 57 ab be d4` | 477 | `TIBSUN.MIX` entry `0x55de03cc` (307 entries) | 462 |
| 5 | `59 22 5b 4b f1 a5 1e db` | 462 | `MAPS01.MIX` (21 entries) | 33 |
| 6 | `41 ea 81 f7 e4 7c 55 7c` | 33 | `MULTI.MIX` (25 entries) | 39 |
| 7 | `51 13 58 79 19 c2 1a 71` | 39 | `TIBSUN.MIX` entry `0xa74702a9` (275 entries) | 414 |
| 8 | `7a 4f 81 ed af c9 31 88` | 414 | `SCORES.MIX` (20 entries) | 31 |
| 9 | `d0 ab 89 fb 32 09 22 d9` | 31 | `MOVIES01.MIX` (41 entries) | 63 |

Sum of index blocks: **1586**, the measured `BF_Decrypt` count. Each key was
derived independently from the archive's 80-byte RSA key block
(`tools/mix.py`'s `_blowfish_key()`), so this is a closed loop: the game read
the header, derived the key, called `SetKey(56, key)`, and decrypted exactly
the index of each archive, nothing else. `MAPS01.MIX`, `MULTI.MIX`,
`SCORES.MIX` and `MOVIES01.MIX` are loose archives on the CD; the four
sub-archives are entries inside `TIBSUN.MIX` (the MIX stores no filenames, so
they are identified by entry id). `SIDECD01.MIX` is on the CD but is not
decrypted during this startup window.

**Consequence for the model:** the transcribed `BlowPipe` code will call
`SetKey` and `Decrypt` through the fake object. If the stub does not implement
Blowfish, every MIX index is garbage, the game will not find its data, and the
next failure will be downstream and misleading. Implement the cipher, not just
`S_OK`.

---

## 6. Implementable spec for the model

Behaviour to reproduce, in the order the retail binary does it:

1. **`CoCreateInstance(rclsid, pUnkOuter, dwClsContext, riid, ppv)`**, when
   `rclsid == {1440AD10-6AA8-11D1-B6F9-00A024DDAFD1}`:
   * if the class has not been "registered" yet: `*ppv = NULL`, return
     `0x80040154` (`REGDB_E_CLASSNOTREG`). This is what the cold retail binary
     does and what makes the fallback run.
   * otherwise: `*ppv = <fake cipher object>`, add a reference, return `S_OK`.
     Accept `riid == IID_IUnknown` and
     `riid == {E0113100-6A7C-11D1-B6F9-00A024DDAFD1}`; return
     `0x80004002` (`E_NOINTERFACE`) for anything else. The game passes
     `pUnkOuter = NULL` and `dwClsContext = 7`.
2. **`LoadLibraryA("blowfish.dll")`** → a non-zero handle (retail
   `0x11000000`).
3. **`GetProcAddress(handle, "DllRegisterServer")`** → a non-zero stub
   address (retail `0x11001050`). `DllUnregisterServer`, `DllGetClassObject`
   and `DllCanUnloadNow` are the other exports; only `DllRegisterServer` is
   called on this path.
4. **`DllRegisterServer()`** → `S_OK` and set the per-process registered flag.
   `DllUnregisterServer()` → `S_OK` and clear it. The game ignores the return
   value, but the flag is what makes the retry succeed.
5. **`FreeLibrary(handle)`** → `TRUE`; the game frees the handle immediately
   after the retry.
6. **`OleRun(p)`** → `S_OK`. Retail `OleRun` queries `IRunnableObject`
   (`{00000126-0000-0000-C000-000000000046}`); the object does not implement it
   (static QI at `0x11001f30` accepts only IUnknown and the Blowfish IID), and
   `OleRun` still returns `S_OK`.
7. **The fake object's vtable** (indices 0..7) as in section 4.1:
   `QueryInterface`, `AddRef`, `Release`, `SetKey(size, key)`,
   `GetKeySize(out)`, `GetBlockSize(out)`, `Encrypt(len, in, out)`,
   `Decrypt(len, in, out)`.
   * `SetKey` must read the 56-byte key from model memory and build a standard
     Blowfish key schedule (forward P for encrypt, reversed P for decrypt).
   * `Decrypt` must read `len/8` blocks from `in` (model memory), decrypt each
     with the reversed P-array, write them to `out` (in-place is the normal
     case), copy the `len%8` tail verbatim, and return `S_OK`.
   * `GetKeySize` writes 56; `GetBlockSize` writes 8.
8. If the model chooses the warm shortcut (make `CoCreateInstance` succeed
   immediately), the registration flag still stays 0 and the game proceeds,
   but the `LoadLibraryA`/`GetProcAddress`/`DllRegisterServer` code is then
   never exercised. The retail cold path is the one measured above and the one
   the todo asks to model.

A no-op `Decrypt` is not enough: the first `SetKey`/`Decrypt` pair is
`TIBSUN.MIX`'s index, and the game will parse ciphertext as a file table.

---

## 7. Reproduction

```bash
cd re
export GAMEDIR=/mnt/nvme-1/projects/tiberian/gamedir
export WINEPREFIX=/mnt/nvme-1/projects/tiberian/.wine32
export TSCD=/mnt/nvme-1/projects/tiberian/work/cd1

# cold path + cipher use, gdb breakpoints; writes /tmp/blowfish_oracle_gdb.log,
# /tmp/blowfish_oracle_counts.json. --early SIGSTOPs GAME.EXE the instant it
# appears, because the registration loop runs before a normal attach lands.
python3 tools/oracle_blowfish.py --cold --early --secs 22

# cold path from Wine's own traces (+relay filtered by function name, +ole,
# +module); writes /tmp/blowfish_relay.log and prints the relevant lines.
./tools/wine_blowfish_trace.sh
```

`--cold` deletes only `HKCR\CLSID\{1440AD10-6AA8-11D1-B6F9-00A024DDAFD1}`
before the run; the game recreates it. Without `--cold` the warm path is
measured instead. The gdb script installs its `BLOWFISH.DLL` breakpoints from
the first hit inside the game (after the DLL is mapped at `0x11000000`), so the
absolute method addresses are valid.

The `tools/oracle_blowfish.py` breakpoints and the exact values they produced
are in `/tmp/blowfish_oracle_cold_gdb.log` (cold) and
`/tmp/blowfish_oracle_warm_gdb.log` (warm). The MIX key/block counts in section
5.2 come from `tools/mix.py`'s key derivation applied to the shipped archives;
the four `TIBSUN.MIX` entries are extracted by the same tool.

---

## 8. Remaining uncertainty

* **The class factory is not directly observed.** It is internal to Wine's
  OLE32. The trace shows `apartment_getclassobject calling DllGetClassObject
  110010AF`, and the DLL's own `DllGetClassObject`/factory code is read from
  the binary; the model does not need to reproduce it if its
  `CoCreateInstance` constructs the cipher object directly.
* **The object address is allocator-dependent.** The measured
  `0x01ea2a08` is a Wine heap address, not a constant. All nine `BlowPipe`
  objects reused it because each was released before the next was created; a
  model may use one static object.
* **`DllRegisterServer`'s return value is `S_OK` by construction** (the local
  at `[ebp-0x160]` is zero and never written) and measured `hr=0` at
  `0x5e6245`. The game ignores it either way.
* **The first `CoCreateInstance` HRESULT is measured at the game's call site**
  (`0x80040154`), not from Wine's own return trace (relay does not log
  `ole32.CoCreateInstance` here, only `ole32.OleRun`). The +ole trace
  independently reports "class ... not registered" for the same call.
* **Only the startup path was measured.** The game keeps running after the
  main menu; no claim is made here about later use of the object beyond the
  nine MIX indexes decrypted during startup. The count matched the nine
  indexes exactly, which is why the claim is limited to them.
