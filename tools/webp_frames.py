#!/usr/bin/env python3
"""Upstream's pictures (resources/image/**/blupixNN.webp, animated WebP
whose frames are the game's themes and styles) as one PNG per frame,
blupixNN.F.png, for the Amiga build: GFOpenUp's SDL2_image.module reads
PNG, and the game's own WebP loader needs libwebp. A frame of 256 colours
or fewer is stored as a palette PNG (see-through kept), the others as RGBA.

    python3 tools/webp_frames.py resources/image amiga/image

Needs Pillow with WebP. GPL 3 or later, like Blupimania.
Copyright (c) 2026 AmiCradle Team."""
import sys
from pathlib import Path
from PIL import Image

src, dst = Path(sys.argv[1]), Path(sys.argv[2])
total = 0
for webp in sorted(src.rglob("blupix*.webp")):
    rel = webp.relative_to(src).with_suffix("")
    im = Image.open(webp)
    for i in range(getattr(im, "n_frames", 1)):
        im.seek(i)
        frame = im.convert("RGBA")
        out = dst / f"{rel}.{i}.png"
        out.parent.mkdir(parents=True, exist_ok=True)
        colours = frame.getcolors(256)
        if colours:
            pal = Image.new("P", frame.size)
            table = [c for _, c in colours]
            index = {c: n for n, c in enumerate(table)}
            pal.putdata([index[p] for p in frame.getdata()])
            flat = []
            for r, g, b, a in table:
                flat += [r, g, b]
            pal.putpalette(flat + [0] * (768 - len(flat)))
            alphas = bytes(a for *_, a in table)
            if any(a < 255 for a in alphas):
                pal.save(out, optimize=True, transparency=alphas)
            else:
                pal.save(out, optimize=True)
        else:
            frame.save(out, optimize=True)
        total += out.stat().st_size
print(f"{dst}: {total} bytes")
