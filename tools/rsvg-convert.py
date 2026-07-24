#!/usr/bin/env python3
"""Minimal rsvg-convert shim (SVG → PDF) for Quarto on Windows without librsvg."""
from __future__ import annotations

import sys
from pathlib import Path


def main(argv: list[str]) -> int:
    # Quarto: rsvg-convert -f pdf -a -o OUTPUT INPUT
    out = None
    fmt = "pdf"
    args = []
    i = 0
    while i < len(argv):
        a = argv[i]
        if a == "-o" and i + 1 < len(argv):
            out = argv[i + 1]
            i += 2
            continue
        if a == "-f" and i + 1 < len(argv):
            fmt = argv[i + 1]
            i += 2
            continue
        if a == "-a":
            i += 1
            continue
        if a.startswith("-"):
            i += 1
            continue
        args.append(a)
        i += 1

    if not out or not args:
        print("usage: rsvg-convert -f pdf -o OUT.svg.pdf IN.svg", file=sys.stderr)
        return 2

    src = Path(args[-1])
    dst = Path(out)
    if fmt.lower() != "pdf":
        print(f"unsupported format: {fmt}", file=sys.stderr)
        return 1

    from svglib.svglib import svg2rlg
    from reportlab.graphics import renderPDF

    drawing = svg2rlg(str(src))
    if drawing is None:
        print(f"failed to parse SVG: {src}", file=sys.stderr)
        return 1
    dst.parent.mkdir(parents=True, exist_ok=True)
    renderPDF.drawToFile(drawing, str(dst))
    return 0


if __name__ == "__main__":
    raise SystemExit(main(sys.argv[1:]))
