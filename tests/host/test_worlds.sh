#!/bin/sh
# The worlds are big-endian, so the Amiga reads them without a swap
# (src/play.c convshort()).
set -eu
cd "$(dirname "$0")/../.."
python3 tests/host/check_worlds.py resources/data
