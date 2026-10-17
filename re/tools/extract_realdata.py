#!/usr/bin/env python3
"""Extract the shipped INI control files out of the retail MIX archive.

Why this exists
---------------
`realdata/` is committed so that `tools/e2e_rules.py` can run on a fresh clone
without the 2.7 GB of CD images. The extraction therefore has to be a *tool*
with recorded byte ranges, not an ad-hoc heredoc: the first attempt at this
scanned for a marker and took the surrounding printable run, which silently
walked past the end of one file and into the next. That produced a
`realdata/art.ini` that was art.ini + rules.ini concatenated, and a
`realdata/ai.ini` that swallowed art.ini whole.

What the archive actually looks like
------------------------------------
Only the MIX *header* (the filename -> offset table) is encrypted; the file
bodies are stored in the clear, back to back, with no alignment padding. The
control files are plain CRLF text. Their regions can therefore be read
directly, and the only real work is getting the boundaries right: each file
starts at the byte after the previous one's last byte and ends where the next
one starts.

The ranges below were established by reading the archive, not by guessing:

  * each range's first bytes are the file's own text (its `; <NAME>.INI` banner
    sits in the short gap immediately before the range, see MARKERS);
  * each range's last bytes are the last line of that file;
  * the ranges are asserted pairwise disjoint before anything is written.

`ai.ini` is the one approximate start: the region before it is binary, and its
first byte (0x62 'b') is stray data that would otherwise be glued onto the
`[TaskForces]` section header and make the header unparseable. The stray byte
is dropped and the file starts at the header.

Usage:
  python3 tools/extract_realdata.py [--mix work/cd1/INSTALL/TIBSUN.MIX]
                                    [--out realdata] [--check]
`--check` re-reads the committed files and verifies they still match the
archive byte for byte (used to catch hand-edits of generated data).
"""
import argparse
import os
import sys

RE = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

# (name, start, end) -- end exclusive. See the module docstring for how these
# were derived. Sizes are printed by the tool; they are not duplicated here.
REGIONS = [
    # AI.INI: team/task force configuration for the mission builder. The start
    # is approximate (0x1517627 holds a stray 'b'); text begins at 0x1517628.
    ("ai.ini",     0x1517628, 0x155bef8),
    ("art.ini",    0x155bf03, 0x1571165),
    ("rules.ini",  0x1571165, 0x15afb68),
    ("sound.ini",  0x15afb68, 0x15b2e48),
    ("theme.ini",  0x15b2e48, 0x15be809),
    # A separate plaintext region later in the archive holds the campaign and
    # briefing data; the two files are adjacent with 3 bytes (';==', the
    # fragment of a banner) between them, which belong to neither.
    ("BATTLE.INI",  0x167c8a8, 0x167cd65),
    ("MISSION.INI", 0x167cd68, 0x1682b99),
]

# Markers that must appear at the start of the region or in the gap just
# before it. These are the file's own banner lines, and checking them is what
# proves a range boundary is a real file boundary.
MARKERS = {
    "art.ini":     b"; ART.INI",
    "rules.ini":   b"; *** Tiberian Sun Rules ***",
    "sound.ini":   b"; SOUND.INI",
    "theme.ini":   b"; THEME.INI",
    "BATTLE.INI":  b"; BATTLE.INI (English)",
    "MISSION.INI": b"; MISSION.INI (ENGLISH)",
    # ai.ini has no banner in the archive; its first section is the evidence.
    "ai.ini":      b"[TaskForces]",
}


def sections(text):
    """Count INI section headers, i.e. lines that are exactly `[Name]`."""
    n = 0
    for line in text.splitlines():
        line = line.strip()
        if line.startswith("[") and line.endswith("]") and len(line) > 2:
            n += 1
    return n


def trim(blob):
    """Drop trailing bytes that are not text.

    A region's end is the start of the next file, which can be a few bytes of
    binary (theme.ini is followed by structured data). Trailing NULs and other
    control bytes are not part of the INI.
    """
    end = len(blob)
    while end and (blob[end - 1] < 0x20 and blob[end - 1] not in (9, 10, 13)):
        end -= 1
    return blob[:end]


def check_regions():
    ordered = sorted(REGIONS, key=lambda r: r[1])
    for (n1, _, e1), (n2, s2, _) in zip(ordered, ordered[1:]):
        if e1 > s2:
            raise SystemExit(
                f"region overlap: {n1} ends at 0x{e1:x}, {n2} starts at "
                f"0x{s2:x} ({e1 - s2} bytes)")


def main():
    ap = argparse.ArgumentParser()
    # Paths resolve against the repo root, not the caller's cwd. The defaults
    # used to be relative ("work/cd1/...", "realdata"), so running the tool
    # from re/ -- which is where every other tool is run from -- failed with
    # "missing MIX archive" even though the archive was there.
    root = os.path.dirname(RE)
    ap.add_argument("--mix", default=os.path.join(root, "work", "cd1", "INSTALL",
                                                  "TIBSUN.MIX"))
    ap.add_argument("--out", default=os.path.join(RE, "realdata"))
    ap.add_argument("--check", action="store_true",
                    help="verify committed files match the archive")
    args = ap.parse_args()

    check_regions()

    if not os.path.exists(args.mix):
        sys.stderr.write(
            f"missing MIX archive: {args.mix}\n"
            f"  it lives in the (gitignored) ISO tree; extract the CD first, "
            f"or use --mix to point at it\n")
        return 2
    data = open(args.mix, "rb").read()

    if not args.out == "" and not args.check:
        os.makedirs(args.out, exist_ok=True)

    print(f"{args.mix}: {len(data)} bytes")
    print(f"{'file':<12} {'range':<24} {'bytes':>8}  sections")
    bad = 0
    for name, start, end in REGIONS:
        if end > len(data):
            raise SystemExit(f"{name}: range 0x{start:x}-0x{end:x} past EOF")
        blob = trim(data[start:end])
        marker = MARKERS[name]
        # The banner is the first thing in the file, but it is preceded by a
        # rule line, so look at the file's opening block, not the first bytes.
        where = data[max(0, start - 64):start + 256]
        if marker not in where:
            sys.stderr.write(f"{name}: marker {marker!r} not found at "
                             f"0x{start:x} (or in the 64 bytes before it)\n")
            bad += 1
        text = blob.decode("latin1")
        path = os.path.join(args.out, name)
        if args.check:
            if not os.path.exists(path):
                sys.stderr.write(f"{name}: missing {path}\n")
                bad += 1
                continue
            old = open(path, "rb").read()
            if old != blob:
                sys.stderr.write(f"{name}: {path} differs from archive "
                                 f"({len(old)} bytes on disk, {len(blob)} in "
                                 f"archive)\n")
                bad += 1
                continue
            status = "ok"
        else:
            open(path, "wb").write(blob)
            status = "wrote"
        print(f"{name:<12} 0x{start:08x}-0x{start + len(blob):08x} "
              f"{len(blob):>8}  {sections(text):>4}   {status}")

    return 1 if bad else 0


if __name__ == "__main__":
    sys.exit(main())
