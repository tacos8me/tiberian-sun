# realdata — shipped game data used to validate the reconstruction

These files are extracted from the retail CD images. They are **committed**
(~1 MB total) rather than ignored, deliberately: the validation in
`tools/e2e_rules.py` is only meaningful if a fresh clone can run it without
first owning the CD images. The 2.7 GB of raw images and ISO trees stay
ignored; these are the small derived text files that make the check portable.

| File | Source | What it is |
|---|---|---|
| `rules.ini` | `INSTALL/TIBSUN.MIX` | the game's rules of engagement: 1,052 sections |
| `ai.ini` | `INSTALL/TIBSUN.MIX` | AI/team configuration: 1,798 sections |

## How they were extracted

Tiberian Sun's MIX archives encrypt their *header* but store file bodies in the
clear, so the INI text can be recovered by scanning the archive for its section
markers and taking the surrounding printable region:

```bash
python3 - <<'PY'
d = open('work/cd1/INSTALL/TIBSUN.MIX','rb').read()
for marker, out in ((b'[General]','rules.ini'), (b'[TeamTypes]','ai.ini')):
    i = d.find(marker); s = i
    while s > 0 and 8 < d[s-1] < 127: s -= 1
    e = i
    while e < len(d) and 8 < d[e] < 127: e += 1
    open('realdata/'+out,'wb').write(d[s:e])
PY
```

`art.ini` is not recoverable this way — its bodies are compressed — which is
why a handful of recovered field names (`CustomRotor`, `Unsellable`,
`Degenerates`) resolve only as *comments* in `ai.ini`. That is a real limit of
this dataset, not a defect in the recovery: the strings exist in the binary.

## What validation uses them for

`tools/e2e_rules.py` parses these files and drives the reconstructed model with
them, checking that the field names recovered from the binary are real keys in
shipped data.
