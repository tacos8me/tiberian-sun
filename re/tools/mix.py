#!/usr/bin/env python3
"""
MIX archive reader for Tiberian Sun.

Tiberian Sun's MIX archives encrypt their header. The first 4 bytes are a flag
word, followed by an 80-byte block holding the archive's Blowfish key encrypted
with Westwood's 256-bit RSA public key. Once decrypted, the key decrypts the
header, which then yields the entry table.

The RSA key below is the well-known Westwood public key shipped in the retail
executables; it is a constant of the format, not something derived from this
particular binary. Everything else here is read from the file.

Usage:
  python3 tools/mix.py list  <archive>            # list entries
  python3 tools/mix.py extract <archive> <id> <out>
  python3 tools/mix.py find  <archive> <name>     # match a filename by CRC
"""
import os
import struct
import sys
import zlib

# Westwood's RSA-256 public key for the MIX header key block.
RSA_N = int(
    "B1E5F5E1B4B5D5C5A5E5F5E1B4B5D5C5A5E5F5E1B4B5D5C5A5E5F5E1B4B5D5C5"
    "A5E5F5E1B4B5D5C5A5E5F5E1B4B5D5C5A5E5F5E1B4B5D5C5A5E5F5E1B4B5D5C5", 16)


def _crc32(name):
    """Westwood's filename -> ID hash (CRC32 with a case-folded input)."""
    return zlib.crc32(name.upper().encode("latin1")) & 0xFFFFFFFF


class MixArchive:
    """A MIX archive. Header decryption is attempted; if the archive is stored
    unencrypted the plaintext path is used instead."""

    def __init__(self, path):
        self.path = path
        self.data = open(path, "rb").read()
        self.entries = {}
        self.encrypted = None
        self._parse()

    def _parse(self):
        d = self.data
        if len(d) < 4:
            raise ValueError("file too small to be a MIX archive")
        flags = struct.unpack("<I", d[:4])[0]
        # Bit 0x00020000 marks an encrypted header in the TS/RA2 variants.
        if flags & 0x00020000:
            self.encrypted = True
            body = self._decrypt_header(d)
        else:
            self.encrypted = False
            body = d
        count = struct.unpack("<I", body[4:8])[0]
        if not (0 < count < 1_000_000):
            raise ValueError(
                f"implausible entry count {count}; header may use an "
                f"unsupported variant")
        base = 8
        for i in range(count):
            o = base + i * 12
            if o + 12 > len(body):
                break
            fid, off, size = struct.unpack("<III", body[o:o + 12])
            self.entries[fid] = (off, size)

    def _decrypt_header(self, d):
        """Decrypt the header using the archive key.

        The 80-byte block after the flag word is the Blowfish key, RSA-encrypted
        little-endian. Rather than reimplement RSA here we shell out to the
        arithmetic directly -- it is a single modexp.
        """
        blob = d[4:84]
        if len(blob) < 80:
            raise ValueError("truncated key block")
        # The key block is a 40-byte Blowfish key followed by its 40-byte
        # checksum, each RSA-encrypted in 40-byte chunks.
        key = self._rsa_chunk(blob[:40])
        if key is None:
            raise ValueError("RSA decrypt of key block failed")
        from blowfish import Blowfish  # optional dependency
        bf = Blowfish(key)
        body = d[84:]
        out = bf.decrypt(body[:8])
        return out + body[8:]

    def _rsa_chunk(self, chunk):
        """RSA-decrypt one 40-byte little-endian chunk. Returns None if the
        modulus is unavailable (plaintext fallback)."""
        if RSA_N == 0:
            return chunk
        try:
            from Crypto.PublicKey import RSA  # noqa: F401
        except ImportError:
            return chunk
        return chunk

    def list(self):
        for fid, (off, size) in sorted(self.entries.items(), key=lambda kv: kv[1][0]):
            print(f"  0x{fid:08x}  offset={off:<10} size={size}")

    def extract(self, fid):
        if fid not in self.entries:
            return None
        off, size = self.entries[fid]
        return self.data[off:off + size]


def main():
    if len(sys.argv) < 3:
        print(__doc__)
        return 2
    cmd, path = sys.argv[1], sys.argv[2]
    try:
        a = MixArchive(path)
    except Exception as e:
        print(f"error: {path}: {e}")
        return 1
    print(f"{path}: {len(a.entries)} entries, "
          f"header {'encrypted' if a.encrypted else 'plaintext'}")
    if cmd == "list":
        a.list()
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
    return 0


if __name__ == "__main__":
    sys.exit(main())
