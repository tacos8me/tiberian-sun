#!/usr/bin/env python3
"""
MSVC 6.0 RTTI type-name demangler.

Handles the subset of MSVC name mangling that appears in RTTI type
descriptors (.?AV / .?AU), including nested templates, pointer/reference
template arguments, and scope qualification.

Grammar observed in GAME.EXE:
    .?A<kind> <name> @@
    name      := component ('@' component)*
    component := '?$' template-name ('@' arg)*
               | plain-identifier
    arg       := 'P' arg            (pointer)
               | 'A' arg            (reference)
               | 'U' arg            (struct)
               | 'V' arg            (class)
               | '?$' template ...
               | plain-identifier
"""
import sys


class Parser:
    def __init__(self, s):
        self.s = s
        self.i = 0

    def peek(self, n=1):
        return self.s[self.i:self.i + n]

    def eof(self):
        return self.i >= len(self.s)

    def parse_ident(self):
        """Read a plain identifier up to '@' or end."""
        start = self.i
        while self.i < len(self.s) and self.s[self.i] not in "@":
            self.i += 1
        return self.s[start:self.i]

    def parse_arg(self):
        """Parse a single template argument."""
        if self.eof():
            return ""
        c = self.s[self.i]
        if c == "P":
            self.i += 1
            return self.parse_arg() + "*"
        if c == "B":
            # 'BV'/'BU' is the const/volatile class type code.
            if self.peek(2) in ("BV", "BU"):
                self.i += 2
                return self.parse_arg()
            self.i += 1
            return self.parse_arg()
        if c == "A":
            # 'AV'/'AU' is the class/struct type code, not a reference.
            # After consuming it, the remainder is a plain name or template.
            if self.peek(2) in ("AV", "AU"):
                self.i += 2
                if self.peek(2) == "?$":
                    return self.parse_template()
                return self.parse_ident()
            self.i += 1
            return self.parse_arg() + "&"
        if c == "U":
            self.i += 1
            if self.peek(2) == "?$":
                return "struct " + self.parse_template()
            return "struct " + self.parse_ident()
        if c == "V":
            self.i += 1
            if self.peek(2) == "?$":
                return "class " + self.parse_template()
            return "class " + self.parse_ident()
        if c == "?" and self.peek(2) == "?$":
            return self.parse_template()
        if c == "$":
            # '$' prefix marks a non-type / special arg
            self.i += 1
            return self.parse_arg()
        return self.parse_ident()

    def parse_template(self):
        """Parse '?$Name@arg@arg@@' -> 'Name<arg, arg>'."""
        assert self.peek(2) == "?$"
        self.i += 2
        name = self.parse_ident()
        args = []
        while not self.eof():
            if self.peek(2) == "@@":
                self.i += 2
                break
            if self.peek(1) == "@":
                # separator between arguments
                self.i += 1
                if self.peek(2) == "@@":
                    self.i += 2
                    break
            a = self.parse_arg()
            if a:
                args.append(a)
            else:
                break
        if args:
            return f"{name}<{', '.join(args)}>"
        return name

    def parse_component(self):
        if self.peek(2) == "?$":
            return self.parse_template()
        return self.parse_ident()

    def parse(self):
        comps = []
        while not self.eof():
            if self.peek(2) == "@@":
                self.i += 2
                break
            if self.peek(1) == "@":
                self.i += 1
                continue
            c = self.parse_component()
            if not c:
                break
            comps.append(c)
        return comps


def demangle(name):
    if not name.startswith(".?"):
        return name
    kind = name[2:3]
    body = name[4:]
    p = Parser(body)
    comps = p.parse()
    if not comps:
        return name
    # MSVC emits innermost scope first; reverse for C++ order.
    comps = list(reversed(comps))
    full = "::".join(comps)
    prefix = "struct " if kind == "U" else "class "
    return prefix + full


def main():
    for line in sys.stdin:
        line = line.strip()
        if line:
            print(demangle(line))


if __name__ == "__main__":
    main()
