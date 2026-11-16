/* amiga/anim_order.h puts the PNG frames in the slots upstream's WebP
 * loader fills (libwebp frame numbers count from 1, 0 = the last), so the
 * colour theme gets the colour picture and Smaky 100 the green one.
 * GPL 3 or later, like Blupimania. Copyright (c) 2026 AmiCradle Team. */
#include <stdio.h>
#include "../../amiga/anim_order.h"

static int fails;

static void
expect (int slot, int count, int want)
{
  int got = bm_anim_png_for_slot (slot, count);
  if (got != want)
  {
    printf ("FAIL slot %d of %d: PNG %d, want %d\n", slot, count, got, want);
    fails++;
  }
}

int
main (void)
{
  /* six frames (blupix01-05): PNG 0 colours, 1 green, 2-5 styles */
  expect (0, 6, 5); /* BLUISH slot */
  expect (1, 6, 0); /* normal colours */
  expect (2, 6, 1); /* Smaky 100: green */
  expect (3, 6, 2);
  expect (4, 6, 3);
  expect (5, 6, 4);
  /* two frames (blupix21, 22-54): PNG 0 colours, 1 green */
  expect (0, 2, 1); /* Smaky 100 */
  expect (1, 2, 0); /* colours */
  expect (0, 1, 0);
  if (fails)
    return 1;
  printf ("anim order: PASS\n");
  return 0;
}
