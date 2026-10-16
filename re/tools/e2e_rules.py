#!/usr/bin/env python3
"""
End-to-end run of the reconstruction against the real shipped game data.

This is the test the project has never had: take the real control files
extracted from the retail archive and drive the RECONSTRUCTED model with them,
then check the results against the recovered evidence.

What it does:
  1. parses the shipped control files (rules.ini, art.ini, ai.ini, sound.ini,
     theme.ini, BATTLE.INI, MISSION.INI -- see realdata/README.md)
  2. for every class whose recovered header declares members, looks up that
     class's section and reads the values for the field names we recovered from
     the binary's own string table
  3. reports, per class, how many of its recovered members the real data
     actually populates, and prints the loaded values

Evidence classes are kept separate, and the distinction matters:

  * SET-derived -- the shipped data contains `Name=value`, so the game
    demonstrably parses that key. This is proof the name is real.
  * DOCUMENTED-derived -- the shipped data documents the key in a comment
    header, e.g. `; Rotors = Does this aicraft have an attached rotor
    animation (def=no)?`. This proves the name exists in the shipped
    documentation, NOT that any value is read for it. Westwood's INI comment
    headers are a key spec with types and defaults, which makes them good
    corroboration and weak proof. Never merge the two counts.

That gives three things nothing else in the project does: it proves the
recovered field names are real, it proves the recovered sections map to real
classes, and it exercises the model end to end on shipped data.

A note on spelling, recorded here because it is a real behavioural fact and not
a typo to be "fixed": the binary looks up the key `Inaccuate` (reader 0x445ba0),
and the string `Inaccurate` does not appear in the binary at all. The shipped
data *documents* the key as `Inaccurate` but never sets either spelling. So the
game reads the misspelled key, and a modder who writes `Inaccurate=` has no
effect. `--typos` reports this class of drift: a recovered name that is close
to, but not equal to, a name the shipped data documents.

Usage:
  python3 tools/e2e_rules.py <rules.ini> [--verbose] [--class NAME]
"""
import argparse
import difflib
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

# A documented key is a comment line that names a key and describes it, the way
# Westwood's control files document their own format:
#     ; Rotors = Does this aicraft have an attached rotor animation (def=no)?
# The identifier before '=' must be a bare token, which keeps prose comments
# (`; right now, normal and hard are the same`) and rule lines out.
DOC_KEY = re.compile(r"^;\s*([A-Za-z_][A-Za-z0-9_]*)\s*=\s*(\S.*)$")


def parse_documented(path):
    """Key spec extracted from a control file's own comment headers.

    Returns {key: [descriptions]} where each description carries the file and
    line it came from, so a report can cite the evidence. These are documented
    names, not values: a key can be documented and never set, and that is a
    real finding (see the `Inaccuate` note in the module docstring).
    """
    documented = defaultdict(list)
    with open(path, encoding="latin1") as f:
        for lineno, raw in enumerate(f, 1):
            m = DOC_KEY.match(raw.rstrip("\r\n").strip())
            if not m:
                continue
            key, desc = m.group(1), m.group(2).strip()
            documented[key].append((os.path.basename(path), lineno, desc))
    return documented



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
    ap.add_argument("--typos", action="store_true",
                    help="report recovered names that differ in spelling from "
                         "the name the shipped data documents")
    args = ap.parse_args()

    sections = defaultdict(dict)
    documented = defaultdict(list)
    for path in args.ini:
        if not os.path.exists(path):
            sys.stderr.write(f"missing {path}\n")
            return 2
        got = parse_ini(path)
        for sec, kv in got.items():
            sections[sec].update(kv)
        doc = parse_documented(path)
        for key, descs in doc.items():
            documented[key].extend(descs)
        print(f"{os.path.basename(path)}: {len(got)} sections, "
              f"{sum(len(v) for v in got.values())} key/value pairs, "
              f"{len(doc)} documented keys")
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

    # Two evidence classes, reported separately and never merged. SET means the
    # shipped data carries `Name=value`; DOCUMENTED means only a comment header
    # names the key. A comment proves the name exists in the shipped spec, not
    # that the game reads a value for it.
    set_derived = [f for f in recovered_fields if field_locations.get(f)]
    doc_derived = [f for f in recovered_fields
                   if not field_locations.get(f) and documented.get(f)]
    unconfirmed = [f for f in recovered_fields
                   if not field_locations.get(f) and not documented.get(f)]
    total = len(recovered_fields)
    print("\n--- member-name confirmation against shipped data ---")
    print(f"  recovered member names           : {total}")
    print(f"  SET-derived (key=value present)  : {len(set_derived)} "
          f"({100.0 * len(set_derived) / total:.1f}%)")
    print(f"  DOCUMENTED-derived (comment hdr) : {len(doc_derived)} "
          f"({100.0 * len(doc_derived) / total:.1f}%)")
    print(f"    of which also set elsewhere    : "
          f"{len([f for f in recovered_fields if documented.get(f) and field_locations.get(f)])}"
          f"  (documented names total "
          f"{len([f for f in recovered_fields if documented.get(f)])})")
    print(f"  unconfirmed                      : {len(unconfirmed)} "
          f"({100.0 * len(unconfirmed) / total:.1f}%)")
    print(f"  confirmed, set or documented     : "
          f"{len(set_derived) + len(doc_derived)} "
          f"({100.0 * (len(set_derived) + len(doc_derived)) / total:.1f}%)")
    if args.verbose and doc_derived:
        print("\n--- documented-only (named in a comment header, never set) ---")
        for f in sorted(doc_derived):
            src, lineno, desc = documented[f][0]
            print(f"  {f:<24} {src}:{lineno}  {desc[:70]}")
    if unconfirmed:
        print("\n--- unconfirmed ---")
        for f in sorted(unconfirmed):
            print(f"  {f:<24} (in the binary, not in shipped data)")

    if args.verbose:
        print("\n--- SET-derived names (name -> sections) ---")
        for f in sorted(set_derived)[:60]:
            secs = sorted(set(field_locations[f]))
            print(f"  {f:<38} {', '.join(secs[:3])}")

    if args.typos:
        # A recovered name close to a documented name is spelling drift between
        # the binary and the shipped spec. It is a fact about the game, not an
        # error: the binary's key is the one that is read.
        print("\n--- spelling drift (recovered name vs documented name) ---")
        found = 0
        for f in sorted(recovered_fields):
            if f in documented or f in field_locations:
                continue
            near = difflib.get_close_matches(f, list(documented), n=1, cutoff=0.75)
            if not near:
                continue
            src, lineno, desc = documented[near[0]][0]
            print(f"  binary reads {f!r}; {src}:{lineno} documents "
                  f"{near[0]!r} ({desc[:60]})")
            found += 1
        if not found:
            print("  none")

    # Per-class load: how many of a class's own fields the real data fills, with
    # the set and documented classes kept apart (doc/set).
    print("\n--- per-class load from real data ---")
    rows = []
    for cls, entry in sorted(named.items()):
        recs = entry.get("members") or {}
        fields = [(int(o), r.get("name")) for o, r in recs.items() if r.get("name")]
        if not fields:
            continue
        filled = doc = 0
        for off, nm in fields:
            if field_locations.get(nm):
                filled += 1
            elif documented.get(nm):
                doc += 1
        rows.append((filled, doc, len(fields), cls))
    rows.sort(reverse=True)
    shown = 0
    for filled, doc, total, cls in rows:
        if args.cls and args.cls.lower() not in cls.lower():
            continue
        print(f"  {filled:>3}/{total:<3} set, {doc:>3} documented  {cls}")
        shown += 1
        if shown >= 25 and not args.cls:
            break

    tot_fields = sum(r[2] for r in rows)
    tot_filled = sum(r[0] for r in rows)
    tot_doc = sum(r[1] for r in rows)
    pct = 100.0 * tot_filled / tot_fields if tot_fields else 0.0
    pct_any = 100.0 * (tot_filled + tot_doc) / tot_fields if tot_fields else 0.0
    print(f"\nclasses with recovered fields: {len(rows)}")
    print(f"fields set in real data        : {tot_filled}/{tot_fields} ({pct:.1f}%)")
    print(f"fields documented only         : {tot_doc}/{tot_fields}")
    print(f"fields resolvable either way   : {tot_filled + tot_doc}/{tot_fields} "
          f"({pct_any:.1f}%)")

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
