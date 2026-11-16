Blupimania for GFOS
===================

A port of Blupimania (Daniel Roux and EPSITEC SA, 1994-1996, made free
by Mathieu Schroeter) from https://github.com/blupi-games/blupimania at
commit cd143907a82f2ba277e7eff00a6d9a1949a026e6 (version 1.6.3), for GFOS:
AmigaOS 3.2 with GFOpenUp, on a 68040 or 68060 with an FPU, on GFOpenUp's
SDL 2.

Blupi has let go of his balloon. Change the world around him (fences,
bridges, barrels, magnets) so that every Blupi finds another one: more
than 120 riddles in four levels, and a fifth to build your own.

Status: built in Cradle's os32-gcc16-open6 stove; see the pull request
for the lab run.


What you need
-------------

- GFOS: AmigaOS 3.2 with GFOpenUp 0.8.26 or later, which installs
  LIBS:GFOpenGPU/SDL2.module, SDL2_image.module and SDL2_mixer.module.
- A 68040 or 68060 with an FPU (not an LC040 or EC040), an RTG screen of
  640 x 480 or more (or the AmiCradle lab), and about 24 MB of free
  memory.
- About 21 MB on the disk: the pictures, worlds, sounds and Ogg music are
  in the drawer.


How to play
-----------

Run Install-Blupimania (or copy the Blupimania drawer where you want it),
then double-click Blupimania, or from a Shell:

    cd Blupimania
    Blupimania

Most of the game is played with the mouse. The keys are in the drawer's
Controls file (the Help key shows it). Your progress and your own worlds
are kept in the drawer's User drawer; Install-Blupimania keeps it when it
updates the game.


Building
--------

    make -f amiga/Makefile.amigaos -j4 STOVE=/path/to/os32-gcc16-open6

or, in Cradle, stove_build with the os32-gcc16-open6 stove and target
package (package/build-amiga.sh), which also packs
Blupimania-VERSION-68040.lha.


What was changed
----------------

- src/blupimania.h, src/play.c, src/smaky.c: the data are read from the
  program's drawer (PROGDIR:, BM_SHARE) instead of ../share/blupimania/;
  the players' file is PROGDIR:User/blupixa.dat.
- src/play.c: the worlds are stored big-endian (the Smaky was a 68000),
  so they are swapped only on a little-endian machine.
  tests/host/test_worlds.sh: all 474 worlds read as big-endian (colour
  schemes 0 to 4) and none as little-endian.
- The pictures are animated WebP upstream. tools/webp_frames.py splits
  each into PNG frames (amiga/image/), which amiga/anim_png.c loads in
  place of src/sdl/SDL3_IMG_webp.c; src/sdl/SDL3_IMG.h gives SDL2_image
  before 2.6 the IMG_Animation struct. Upstream's loader fills slot i with
  libwebp frame i, and libwebp counts frames from 1 (0 is the last), so
  slot 0 holds the last frame and slot i frame i-1; amiga/anim_order.h
  does the same with the PNGs (tests/host/test_anim_order.c), or the
  colour theme shows the green Smaky 100 pictures.
  Only the frame the game shows is read (BM_AnimFrame() in src/smaky.c
  LoadImage): upstream decodes all six 1280x640 frames of each sheet.
- src/smaky.c: no sys/file.h (libnix has none).
- src/play.c: the 50 ms frame timer sends a tick only when the last one
  has been taken. A frame can take longer than 50 ms on a 68040, and the
  queued ticks made the game answer clicks and keys seconds late (lab).
  Lab only: it is timing on the real machine, no host test reaches it.
- amiga/amiga_start.c: 512 KB of stack, the $VER string, PROGDIR:User made
  at start.
- amiga/: the Makefile, the GlowIcons (our own art), the Installer script
  and its spec, Controls, Licences.txt. package/ and tools/ pack the
  release archive.

The AmiCradle Team.
