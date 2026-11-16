#!/bin/sh
# The worlds are big-endian, so the Amiga reads them without a swap
# (src/play.c convshort()).
set -eu
cd "$(dirname "$0")/../.."
python3 tests/host/check_worlds.py resources/data
cc -std=c99 -Wall -Werror -o "${TMPDIR:-/tmp}/bm_test_anim_order" tests/host/test_anim_order.c
"${TMPDIR:-/tmp}/bm_test_anim_order"
