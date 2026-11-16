/* Which PNG frame fills slot i of a picture's IMG_Animation.
 * Upstream reads its WebP with WebPDemuxGetFrame (dmuxer, i, ...) for
 * i = 0..count-1, and libwebp counts frames from 1, taking 0 as the last
 * one; so slot 0 holds the last frame and slot i frame i-1. The game picks
 * its slots on that order (src/smaky.c LoadImage: 1 normal colours, 2 or 0
 * the green Smaky 100 theme, 3 to 5 styles), so the PNGs, numbered as the
 * WebP stores its frames (tools/webp_frames.py), are put in the same way.
 * GPL 3 or later, like Blupimania. Copyright (c) 2026 AmiCradle Team. */
#ifndef BM_ANIM_ORDER_H
#define BM_ANIM_ORDER_H

static inline int
bm_anim_png_for_slot (int slot, int count)
{
  return slot == 0 ? count - 1 : slot - 1;
}

#endif
