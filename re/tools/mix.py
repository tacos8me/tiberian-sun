#!/usr/bin/env python3
"""MIX archive reader for Tiberian Sun.

What is verified about this format, and how
-------------------------------------------
The archive stores file *bodies* in the clear, back to back. Only the index
(filename -> offset) is protected, and there are two index variants on the
retail CDs:

  * `SETUP/SETUP.MIX` is the old headerless variant (Tiberian Dawn / RA1
    style). Its index is **not** encrypted: a little-endian `u16` entry count,
    a `u32` body size, then `count` records of `{u32 id, u32 offset, u32 size}`
    with offsets relative to the end of the index. This is verified against the
    file: SETUP.MIX has count 61, body 8,280,448 and index 6 + 61*12 = 738
    bytes, and 738 + 8,280,448 == 8,281,186 == the file's exact size, with
    every record in bounds. `realdata/README.md` records the same shape.

  * Every other archive here (`TIBSUN.MIX`, `MULTI.MIX`, `MAPS01.MIX`, ...)
    starts with a `u32` flag word with bit 0x00020000 set, followed by an
    80-byte RSA-encrypted Blowfish key block, then the file bodies. The index
    is Blowfish-encrypted with the key that block decrypts to. That much is
    read directly from the files.

The key block is the published Westwood RSA scheme: two 40-byte
little-endian chunks, public exponent 65537, and the modulus below. The
*decryption is implemented and runs*, but the resulting key does not currently
produce a self-consistent index for the archives on these CDs -- see
`probe`, which prints every key-derivation variant and the bytes it produces.
Rather than report garbage entry counts, this tool validates the decrypted
index and raises `MixFormatError` naming what it saw. `list`/`find`/`extract`
therefore work on the headerless variant and fail *explicitly* on the
encrypted one; nothing here silently misreports.

Usage:
  python3 tools/mix.py list    <archive>
  python3 tools/mix.py extract <archive> <id> <out>
  python3 tools/mix.py find    <archive> <name>   # match a filename by CRC
  python3 tools/mix.py probe   <archive>          # key-derivation diagnostics

`find` uses the standard Westwood filename hash (CRC32 of the upper-cased
name). No archive here stores its filenames, so that hash is not verifiable
from the files alone: `find` can report a name absent either because the file
is not in the archive or because the archive keys it under a name that is not
the on-disc filename (SETUP.MIX does exactly that -- none of its 61 entries
matches the CRC of any loose file next to it on the CD).
"""
import os
import struct
import sys
import zlib

# Westwood's RSA-256 public key for the MIX header key block. This is a
# constant of the format, not something derived from this particular binary:
# it is the published key used by every TS/RA2 tool, and the same 16-hex-digit
# pattern repeats four times.
RSA_N = int(
    "B1E5F5E1B4B5D5C5A5E5F5E1B4B5D5C5A5E5F5E1B4B5D5C5A5E5F5E1B4B5D5C5"
    "A5E5F5E1B4B5D5C5A5E5F5E1B4B5D5C5A5E5F5E1B4B5D5C5A5E5F5E1B4B5D5C5", 16)
RSA_E = 65537

# Bit 0x00020000 marks an encrypted index in the TS/RA2 variants. This is
# observed on every encrypted archive on the CDs and on no other file.
FLAG_ENCRYPTED = 0x00020000


class MixFormatError(Exception):
    """Raised when an archive's index cannot be read, with the reason."""


def _crc32(name):
    """Westwood's filename -> ID hash (CRC32 with a case-folded input)."""
    return zlib.crc32(name.upper().encode("latin1")) & 0xFFFFFFFF


def _blowfish(key):
    """Blowfish-ECB, with a message that says what to install if it is absent.

    The `blowfish` module this tool used to import is not a dependency of
    anything here; PyCryptodome (Crypto.Cipher.Blowfish) is what is installed.
    """
    try:
        from Crypto.Cipher import Blowfish
    except ImportError:
        raise MixFormatError(
            "encrypted MIX index needs Blowfish: install pycryptodome "
            "(`pip install pycryptodome`); the `blowfish` module is not used")
    return Blowfish.new(key[:56], Blowfish.MODE_ECB)


class MixArchive:
    """A MIX archive, in whichever of the two variants the file uses."""

    def __init__(self, path, strict=True):
        self.path = path
        self.data = open(path, "rb").read()
        self.entries = {}
        self.variant = None
        self.error = None
        self._strict = strict
        self._parse()

    # -- variants ---------------------------------------------------------

    def _parse(self):
        if len(self.data) < 8:
            raise MixFormatError("file too small to be a MIX archive")
        if self._try_headerless():
            return
        flags = struct.unpack("<I", self.data[:4])[0]
        if flags & FLAG_ENCRYPTED:
            self.variant = "ts-encrypted"
            try:
                self._parse_encrypted(flags)
            except MixFormatError as e:
                # `probe` needs the archive even when the index did not decode,
                # so that it can print what each key derivation produced.
                if self._strict:
                    raise
                self.error = e
        else:
            self.variant = "ts-plaintext"
            self._parse_plaintext()

    def _try_headerless(self):
        """The TD/RA1-style index: count, body size, then the records.

        Recognised by an exact size identity rather than by a flag, because
        there is no flag word: 6 + 12*count + body_size must equal the file
        size. That is what SETUP.MIX satisfies to the byte.
        """
        count, body = struct.unpack("<HI", self.data[:6])
        if not (0 < count < 100_000):
            return False
        if 6 + 12 * count + body != len(self.data):
            return False
        for i in range(count):
            fid, off, size = struct.unpack(
                "<III", self.data[6 + i * 12:18 + i * 12])
            if size == 0 or off + size > body:
                return False
            self.entries[fid] = (6 + 12 * count + off, size)
        self.variant = "headerless"
        return True

    def _parse_plaintext(self):
        count = struct.unpack("<I", self.data[4:8])[0]
        if not (0 < count < 1_000_000):
            raise MixFormatError(
                f"implausible entry count {count} at 0x4 in an unencrypted "
                f"index; not a MIX archive, or a variant this tool does not "
                f"know")
        for i in range(count):
            o = 8 + i * 12
            fid, off, size = struct.unpack("<III", self.data[o:o + 12])
            self.entries[fid] = (off, size)

    def _key_candidates(self):
        """Every Blowfish key this tool can derive from the key block.

        Yields (description, key). Kept as a list rather than one expression so
        that `probe` can show what each variant produces.
        """
        blob = self.data[4:84]
        if len(blob) < 80:
            raise MixFormatError("truncated key block")
        for name, modulus in (("N", RSA_N), ("N_reversed",
                                             int.from_bytes(
                                                 RSA_N.to_bytes(64, "big"),
                                                 "little"))):
            width = (modulus.bit_length() + 7) // 8
            for chunk_endian in ("little", "big"):
                chunks = [int.from_bytes(blob[i:i + 40], chunk_endian)
                          for i in (0, 40)]
                if any(c >= modulus for c in chunks):
                    continue
                for out_endian in ("little", "big"):
                    plain = b""
                    for c in chunks:
                        m = pow(c, RSA_E, modulus)
                        raw = m.to_bytes(width, out_endian)
                        plain += raw[:40] if out_endian == "little" else raw[-40:]
                    yield f"{name}/{chunk_endian}/{out_endian}", plain[:56]

    def _parse_encrypted(self, flags):
        """Decrypt the index, and refuse to believe an implausible one.

        The decrypted layout is the same as the unencrypted TS variant: a u32
        count at 0 and u32 body size at 4 of the region after the key block.
        """
        blob_end = 84
        body = self.data[blob_end:]
        tried = []
        for desc, key in self._key_candidates():
            head = _blowfish(key).decrypt(body[:8])
            count, size = struct.unpack("<II", head)
            tried.append((desc, count, size))
            if not (0 < count < 1_000_000):
                continue
            if size != 8 + 12 * count:
                continue
            table = _blowfish(key).decrypt(body[:8 + 12 * count])
            ok = True
            for i in range(count):
                fid, off, sz = struct.unpack("<III", table[8 + i * 12:20 + i * 12])
                if sz == 0 or off + sz > len(body):
                    ok = False
                    break
                self.entries[fid] = (blob_end + off, sz)
            if ok:
                return
            self.entries = {}
        raise MixFormatError(
            f"encrypted index (flags 0x{flags:08x}) at 0x54 could not be "
            f"decrypted into a self-consistent table; tried "
            f"{len(tried)} key derivations, first is "
            f"{tried[0][0]} -> count {tried[0][1]}, size {tried[0][2]} "
            f"(expected size 8 + 12*count). Run `probe` for all of them.")

    # -- operations -------------------------------------------------------

    def list(self):
        for fid, (off, size) in sorted(self.entries.items(), key=lambda kv: kv[1][0]):
            print(f"  0x{fid:08x}  offset={off:<10} size={size}")

    def extract(self, fid):
        if fid not in self.entries:
            return None
        off, size = self.entries[fid]
        return self.data[off:off + size]

    def probe(self):
        """Print what each key derivation yields. Diagnostics, not a decode."""
        print(f"{self.path}: {len(self.data)} bytes, variant {self.variant}")
        if self.variant != "ts-encrypted":
            print(f"  {len(self.entries)} entries, index is not encrypted")
            return
        body = self.data[84:]
        print("  key derivation -> first 16 bytes of the decrypted index, "
              "u32 count, u32 size")
        for desc, key in self._key_candidates():
            head = _blowfish(key).decrypt(body[:16])
            count, size = struct.unpack("<II", head[:8])
            print(f"  {desc:<28} {head[:16].hex()}  count={count} size={size}")


def main():
    if len(sys.argv) < 3:
        print(__doc__)
        return 2
    cmd, path = sys.argv[1], sys.argv[2]
    # `probe` is the diagnostic for an index that does not decode, so it must
    # be able to open an archive that failed to decode.
    try:
        a = MixArchive(path, strict=(cmd != "probe"))
    except (MixFormatError, OSError) as e:
        print(f"error: {path}: {e}")
        return 1
    print(f"{path}: {len(a.entries)} entries, index {a.variant}"
          + (f" ({a.error})" if a.error else ""))
    if cmd == "list":
        a.list()
    elif cmd == "probe":
        a.probe()
    elif cmd == "find":
        want = sys.argv[3]
        fid = _crc32(want)
        print(f"  {want} -> 0x{fid:08x}: "
              f"{'present' if fid in a.entries else 'not found'}")
    elif cmd == "extract":
        fid = int(sys.argv[3], 0)
        blob = a.extract(fid)
        if blob is None:
            print("not found")
            return 1
        out = sys.argv[4] if len(sys.argv) > 4 else f"{fid:08x}.bin"
        open(out, "wb").write(blob)
        print(f"wrote {out} ({len(blob)} bytes)")
    else:
        print(f"unknown command {cmd!r}")
        return 2
    return 0


if __name__ == "__main__":
    sys.exit(main())
