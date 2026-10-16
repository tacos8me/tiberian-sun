# realdata — shipped game data used to validate the reconstruction

These files are extracted from the retail CD images. They are **committed**
(~700 KB total) rather than ignored, deliberately: the validation in
`tools/e2e_rules.py` is only meaningful if a fresh clone can run it without
first owning the CD images. The 2.7 GB of raw images and ISO trees stay
ignored; these are the small derived text files that make the check portable.

They are **generated**, not hand-edited. Regenerate with:

```bash
python3 tools/extract_realdata.py --mix work/cd1/INSTALL/TIBSUN.MIX
python3 tools/extract_realdata.py --check          # verify committed copies
```

## Inventory

All ranges are `[start, end)` byte offsets in `work/cd1/INSTALL/TIBSUN.MIX`
(75,094,736 bytes). Section counts are lines that are exactly `[Name]`.

| File | Range | Bytes | Sections | Notes |
|---|---|---|---|---|
| `ai.ini` | `0x1517628`–`0x155bef8` | 280,784 | 470 | AI/team configuration. **Start is approximate**: the region before it is binary and `0x1517627` holds a stray `0x62` (`b`) byte, dropped because it would otherwise glue itself to the `[TaskForces]` header and make that section unparseable. |
| `art.ini` | `0x155bf03`–`0x1571165` | 86,626 | 653 | artwork/animation control file; plaintext |
| `rules.ini` | `0x1571165`–`0x15afb68` | 256,515 | 762 | rules of engagement; plaintext |
| `sound.ini` | `0x15afb68`–`0x15b2e48` | 13,024 | 274 | sound definitions; plaintext |
| `theme.ini` | `0x15b2e48`–`0x15be809` | 47,553 | 186 | music theme list; plaintext |
| `BATTLE.INI` | `0x167c8a8`–`0x167cd65` | 1,213 | 3 | battle campaigns (separate text region) |
| `MISSION.INI` | `0x167cd68`–`0x1682b99` | 24,113 | 77 | mission briefings (same region) |

Boundaries are established by reading the archive: each file's first bytes are
its own text, its last bytes are its last line, and the files are stored back to
back with no padding. The tool asserts the ranges are pairwise disjoint before
writing anything, and verifies each file's own banner marker (`; ART.INI`,
`; SOUND.INI`, `; THEME.INI`, `; BATTLE.INI (English)`,
`; MISSION.INI (ENGLISH)`, `; *** Tiberian Sun Rules ***`).

Three bytes at `0x167cd65` (`;==`, the fragment of a banner) belong to neither
`BATTLE.INI` nor `MISSION.INI` and are excluded from both. The rest of that
region is further files (`[MultiMaps]` and beyond) that this tool does not
extract.

## Correction: art.ini is *not* compressed

An earlier note in this file claimed `art.ini` was unrecoverable because its
bodies were compressed. **That was wrong.** Only the MIX *header* (the
filename→offset index) is encrypted, flagged by `0x00020000` in the archive's
flag word; every file body is stored in the clear. `art.ini` is plaintext and
reads:

> `; This control file provides the information necessary to handle the`
> `; artwork in Tiberian Sun.`

The earlier extraction was wrong in a different way: it scanned for a marker and
took the surrounding printable run, which walked straight past the end of one
file into the next. The result was a `realdata/art.ini` that was `art.ini` +
`rules.ini` concatenated (`[General]` at 81,518, `[InfantryTypes]` at 107,513),
and a `realdata/ai.ini` that swallowed `art.ini` whole. Both are fixed by the
byte ranges above.

`tools/mix.py` is the archive reader. It reads the headerless TD/RA1-style index
correctly (`SETUP/SETUP.MIX`, 61 entries) and fails *explicitly* on the
encrypted TS index rather than reporting garbage — see the note in that tool.

## What validation uses them for

`tools/e2e_rules.py` parses these files and drives the reconstructed model with
them, checking that field names recovered from the binary are real keys in
shipped data. It reports two evidence classes separately and never merges them:

* **SET-derived** — the shipped data carries `Name=value`.
* **DOCUMENTED-derived** — the shipped data only *documents* the name in a
  comment header, e.g. `; Rotors = Does this aicraft have an attached rotor
  animation (def=no)?`. Westwood's comment headers are a key spec with types and
  defaults; a comment proves the name exists, not that the game reads a value
  for it.

Measured on the files above (`python3 tools/e2e_rules.py realdata/*.ini
realdata/*.INI`): 112 recovered member names, 101 (90.2%) SET-derived, 9 (8.0%)
documented-only, 2 unconfirmed, 110 (98.2%) confirmed by either class.

## A spelling defect that is faithful behaviour, not a bug to fix

The binary looks up the key `Inaccuate` (reader `0x445ba0`, key string VA
`0x6c151c` area). The string `Inaccurate` does not appear in the binary at all.
The shipped data *documents* the key as `Inaccurate` (`rules.ini:7859`) but
never sets either spelling, so the key is read and simply never found. A modder
who writes `Inaccurate=` has no effect; only the misspelled `Inaccuate=` works.
`tools/e2e_rules.py --typos` reports this class of drift by name similarity, and
the reconstruction preserves the binary's spelling.
