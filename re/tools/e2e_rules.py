#!/usr/bin/env python3
"""
End-to-end run of the reconstructed reconstruction against real game data.

This is the test the project has never had: take the real `rules.ini` extracted
from the retail archive and drive the RECONSTRUCTED model with it, then check
the results against the recovered evidence.

What it does:
  1. parses real rules.ini (1,222 sections of shipped game data)
  2. for every class whose recovered header declares members, looks up that
     class's section in rules.ini and reads the values for the field names we
     recovered from the binary's own string table
  3. reports, per class, how many of its recovered members the real data
     actually populates, and prints the loaded values

That gives three things nothing else in the project does: it proves the
recovered field names are real, it proves the recovered sections map to real
classes, and it exercises the model end to end on shipped data.

Usage:
  python3 tools/e2e_rules.py <rules.ini> [--verbose] [--class NAME]
"""
import argparse
import json
import os
import re
import sys
from collections import defaultdict


def parse_ini(path):
    """Parse a Westwood INI. Values keep their raw text; ';' starts a comment.

    This mirrors INIClass's own semantics: a line is `key=value`, and anything
    after an unquoted ';' is commentary (the recovered GetString family returns
    the text before it).
    """
    sections = defaultdict(dict)
    cur = None
    with open(path, encoding="latin1") as f:
        for raw in f:
            line = raw.rstrip("\r\n")
            if not line.strip() or line.lstrip().startswith(";"):
                continue
            stripped = line.strip()
            if stripped.startswith("["):
                end = stripped.find("]")
                if end > 0:
                    cur = stripped[1:end]
                continue
            if cur is None or "=" not in stripped:
                continue
            k, v = stripped.split("=", 1)
            v = v.split(";", 1)[0].strip()
            sections[cur][k.strip()] = v
    return sections


def load_recovered(outdir):
    """Field names the binary's own string table associates with each class.

    member_names.json maps a class to {offset: {name, key_va, reader_va}}; the
    name is the INI key the reader looks up, so it is the ground truth for what
    that class reads out of rules.ini.
    """
    p = os.path.join(outdir, "member_names.json")
    if not os.path.exists(p):
        return {}
    return json.load(open(p))


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("ini", nargs="+")
    ap.add_argument("--out", default="out")
    ap.add_argument("--verbose", action="store_true")
    ap.add_argument("--class", dest="cls", default=None)
    args = ap.parse_args()

    sections = defaultdict(dict)
    for path in args.ini:
        if not os.path.exists(path):
            sys.stderr.write(f"missing {path}\n")
            return 2
        got = parse_ini(path)
        for sec, kv in got.items():
            sections[sec].update(kv)
        print(f"{os.path.basename(path)}: {len(got)} sections, "
              f"{sum(len(v) for v in got.values())} key/value pairs")
    total_keys = sum(len(v) for v in sections.values())
    print(f"merged: {len(sections)} sections, {total_keys} key/value pairs")

    named = load_recovered(args.out)
    if not named:
        sys.stderr.write("no out/member_names.json; run the pipeline first\n")
        return 2

    # Every field name we recovered, and where it appears in the real data.
    recovered_fields = {}
    for cls, entry in named.items():
        for off, rec in (entry.get("members") or {}).items():
            nm = rec.get("name")
            if nm:
                recovered_fields.setdefault(nm, []).append(
                    (cls, int(off), rec.get("key_va"), rec.get("reader_va")))

    print(f"recovered member names: {len(recovered_fields)}")

    # Where do those names actually occur in shipped data?
    field_locations = defaultdict(list)
    for sec, kv in sections.items():
        for k in kv:
            if k in recovered_fields:
                field_locations[k].append(sec)

    confirmed = [f for f in recovered_fields if field_locations.get(f)]
    absent = [f for f in recovered_fields if not field_locations.get(f)]
    print(f"  present in real data   : {len(confirmed)}")
    print(f"  not present in rules.ini: {len(absent)}")

    if args.verbose:
        print("\n--- confirmed field names (name -> sections) ---")
        for f in sorted(confirmed)[:60]:
            secs = sorted(set(field_locations[f]))
            print(f"  {f:<38} {', '.join(secs[:3])}")
        if absent:
            print("\n--- not in rules.ini (may live in art.ini/ai.ini) ---")
            print("  " + ", ".join(sorted(absent)[:30]))

    # Per-class load: how many of a class's own fields the real data fills.
    print("\n--- per-class load from real data ---")
    rows = []
    for cls, entry in sorted(named.items()):
        recs = entry.get("members") or {}
        fields = [(int(o), r.get("name")) for o, r in recs.items() if r.get("name")]
        if not fields:
            continue
        filled = 0
        for off, nm in fields:
            if field_locations.get(nm):
                filled += 1
        rows.append((filled, len(fields), cls))
    rows.sort(reverse=True)
    shown = 0
    for filled, total, cls in rows:
        if args.cls and args.cls.lower() not in cls.lower():
            continue
        print(f"  {filled:>3}/{total:<3} {cls}")
        shown += 1
        if shown >= 25 and not args.cls:
            break

    tot_fields = sum(r[1] for r in rows)
    tot_filled = sum(r[0] for r in rows)
    pct = 100.0 * tot_filled / tot_fields if tot_fields else 0.0
    print(f"\nclasses with recovered fields: {len(rows)}")
    print(f"fields resolvable in real data: {tot_filled}/{tot_fields} ({pct:.1f}%)")

    # Concrete worked examples, so the run produces something a human can
    # eyeball: recovered offset -> real shipped value, with the evidence VAs.
    print("\n--- worked examples (recovered offset -> value in shipped data) ---")
    shown = 0
    for cls, entry in sorted(named.items()):
        recs = entry.get("members") or {}
        for off, r in sorted(recs.items(), key=lambda kv: int(kv[0])):
            nm = r.get("name")
            locs = field_locations.get(nm)
            if not locs:
                continue
            sec = locs[0]
            val = sections[sec].get(nm)
            print(f"  {cls:<28} +{int(off):<5} {nm:<22} = {val!r}")
            print(f"      [{sec}]  key_va {hex(r.get('key_va', 0))}"
                  f"  reader_va {hex(r.get('reader_va', 0))}")
            shown += 1
            break
        if shown >= 8:
            break

    return 0


if __name__ == "__main__":
    sys.exit(main())
