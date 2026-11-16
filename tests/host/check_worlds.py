#!/usr/bin/env python3
"""The Amiga port reads the worlds (resources/data/*/blupix*.dat) as they
are stored, without swapping: they must be big-endian. Each world is a
Monde (src/blupimania.h): 20 x 20 cells, 100 palette entries, 500 bytes of
text, freq, color, 100 reserved shorts = 1704 bytes on every machine.
Read big-endian, every world's colour scheme is a small number and its
text is printable; read little-endian, the colour numbers are not."""
import struct, sys, glob, os

SIZE = 20 * 20 * 2 + 100 * 2 + 500 + 2 + 2 + 100 * 2
root = sys.argv[1] if len(sys.argv) > 1 else "resources/data"
files = sorted(glob.glob(os.path.join(root, "*", "blupix*.dat")))
if not files:
    sys.exit("no worlds under " + root)
worlds = bad_be = bad_le = 0
colors = set()
for f in files:
    data = open(f, "rb").read()
    if len(data) % SIZE:
        sys.exit(f"{f}: {len(data)} bytes is not a whole number of worlds")
    for k in range(len(data) // SIZE):
        w = data[k * SIZE:(k + 1) * SIZE]
        worlds += 1
        tail = 800 + 200 + 500
        be_freq, be_color = struct.unpack(">hh", w[tail:tail + 4])
        le_freq, le_color = struct.unpack("<hh", w[tail:tail + 4])
        if not (0 <= be_color < 64 and 0 <= be_freq < 1000):
            bad_be += 1
        if not (0 <= le_color < 64 and 0 <= le_freq < 1000):
            bad_le += 1
        colors.add(be_color)
print(f"{len(files)} files, {worlds} worlds; big-endian out of range: {bad_be}; "
      f"little-endian out of range: {bad_le}; colours {sorted(colors)}")
if bad_be:
    sys.exit("FAIL: worlds do not read as big-endian")
if bad_le == 0:
    sys.exit("FAIL: the test cannot tell the byte orders apart")
print("PASS")
