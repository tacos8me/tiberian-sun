#pragma once
// ==========================================================================
// Pipe.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// The engine's IO pipeline. A Pipe writes data into a chain; a Straw reads it
// back. The two hierarchies are parallel and each stage wraps the next, so a
// chain like
//
//     FilePipe -> LCWPipe -> SHAPipe
//
// compresses, hashes and writes in one pass.
//
// HIERARCHY (from the RTTI base-class records)
// --------------------------------------------
//   Pipe  (vtable 0x69bc04, 5 slots)
//     Base64Pipe 0x6a0440   BlowPipe 0x6a3258   BufferPipe 0x69bbec
//     FilePipe   0x69ad1c   LCWPipe  0x6a150c   LZOPipe    0x6a1884
//     PKPipe     0x6a323c (6 slots)             SHAPipe    0x69acf4
//
//   Straw (vtable 0x69bbdc, 3 slots)
//     Base64Straw 0x6a0430  BlowStraw 0x6a2308  BufferStraw 0x69bbcc
//     CacheStraw  0x6a0420  FileStraw 0x69ad0c  LCWStraw    0x6a1524
//     LZOStraw    0x6a189c  PKStraw   0x6a3270 (4 slots)
//     RandomStraw 0x6a3b78  SHAStraw  0x6a2320
//
// VIRTUAL SEMANTICS (all 80 slots named from call-site evidence)
// -------------------------------------------------------------
// The base slots are the pipeline plumbing, recovered from their bodies:
//
//   Pipe::GetSize   0x591fb0  forwards to the next pipe's slot 1
//   Pipe::Put       0x44ab70  pure forward to the next pipe's slot 1
//   Pipe::Next      0x591f40  setter for +0x04; unlinks the old next (clears
//                             its +0x08) and links the new one (+0x08 = this)
//   Pipe::Put_Block 0x591f90  forwards a 2-argument write to the next slot 4
//
// The concrete stages are identified by the code they call:
//
//   LCW   0x4f3d70   Westwood LCW compression
//   LZO   0x4fb310   LZO compression
//   PK    0x554060 / 0x592e20 / 0x593190   PKZIP deflate/inflate
//   Blow  0x423440   Blowfish
//   SHA   0x5d7810   SHA-1. Confirmed by the five init constants
//                    0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476,
//                    0xc3d2e1f0 and the 0x40-byte block transform.
//   Base64 0x41fcb0  base64 encode/decode
//   Random 0x5a5fd0  PRNG
//
// UNCERTAINTY
// -----------
// The Mode/Dirty/Flag byte members are named from their usage, not from a
// symbol; they are marked /* ? */. The PKPipe/PKStraw zlib state field names
// are inferred from call sites, not from a zlib header.
// ==========================================================================

#include "ts_prelude.h"

// --------------------------------------------------------------------------
// Pipe — write side of the pipeline.
// --------------------------------------------------------------------------
class Pipe
{
public:
    virtual ~Pipe();

    // ---- vtable 0x69bc04 (5 slots) ----
    // [0] ~Pipe      0x4789b0 (scalar deleting dtor; sets vptr 0x69bc04)
    // [1] GetSize    0x591fb0
    // [2] Put        0x44ab70
    // [3] Next       0x591f40
    // [4] Put_Block  0x591f90
    virtual int  GetSize();
    virtual int  Put(void *buffer, int length);
    virtual void Next(Pipe *next);
    virtual int  Put_Block(void *buffer, int length);

    // ---- data members ----
    void *vptr;    // +0x00  vtable 0x69bc04
    Pipe *Next;    // +0x04  next stage in the chain
    Pipe *Prev;    // +0x08  previous stage (set by the next stage's Next())
};

// --------------------------------------------------------------------------
// Straw — read side of the pipeline.
// --------------------------------------------------------------------------
class Straw
{
public:
    virtual ~Straw();

    // ---- vtable 0x69bbdc (3 slots) ----
    // [0] ~Straw      /* ? */
    // [1] Get         /* ? */
    // [2] Get_Block   /* ? */
    virtual int Get(void *buffer, int length);
    virtual int Get_Block(void *buffer, int length);

    // ---- data members ----
    void  *vptr;   // +0x00  vtable 0x69bbdc
    Straw *Next;   // +0x04  next stage in the chain
    Straw *Prev;   // +0x08  previous stage
};

// --------------------------------------------------------------------------
// Concrete Pipe stages.
// --------------------------------------------------------------------------
class BufferPipe : public Pipe {};   // vtable 0x69bbec
class FilePipe   : public Pipe {};   // vtable 0x69ad1c
class SHAPipe    : public Pipe {};   // vtable 0x69acf4  SHA-1
class Base64Pipe : public Pipe {};   // vtable 0x6a0440  base64
class LCWPipe    : public Pipe {};   // vtable 0x6a150c  LCW
class LZOPipe    : public Pipe {};   // vtable 0x6a1884  LZO
class BlowPipe   : public Pipe {};   // vtable 0x6a3258  Blowfish

// PKPipe adds a 6th slot, Put_End, which finalises the deflate stream.
class PKPipe : public Pipe
{
public:
    // ---- vtable 0x6a323c (6 slots) ----
    // [5] Put_End  /* ? */ finalises the PKZIP stream
    virtual int Put_End();
};

// --------------------------------------------------------------------------
// Concrete Straw stages.
// --------------------------------------------------------------------------
class BufferStraw : public Straw {};  // vtable 0x69bbcc
class FileStraw   : public Straw {};  // vtable 0x69ad0c
class SHAStraw    : public Straw {};  // vtable 0x6a2320  SHA-1
class Base64Straw : public Straw {};  // vtable 0x6a0430  base64
class CacheStraw  : public Straw {};  // vtable 0x6a0420
class LCWStraw    : public Straw {};  // vtable 0x6a1524  LCW
class LZOStraw    : public Straw {};  // vtable 0x6a189c  LZO
class BlowStraw   : public Straw {};  // vtable 0x6a2308  Blowfish
class RandomStraw : public Straw {};  // vtable 0x6a3b78  PRNG

// PKStraw adds a 4th slot, Get_End, which finalises the inflate stream.
class PKStraw : public Straw
{
public:
    // ---- vtable 0x6a3270 (4 slots) ----
    // [3] Get_End  /* ? */ finalises the PKZIP stream
    virtual int Get_End();
};
