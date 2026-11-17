/* BM_IMG_LoadWEBPAnimation() for the Amiga build of Blupimania: the game's
 * pictures are animated WebP files whose frames are its themes and styles;
 * here each frame is a PNG beside where the WebP would be
 * (image/[lang/]blupixNN.F.png, made from upstream's by
 * tools/webp_frames.py), read with GFOpenUp's SDL2_image.module. A missing
 * frame ends the animation; no frame at all gives NULL, as a missing WebP
 * does upstream. The frames go into the slots in upstream's order
 * (anim_order.h).
 *
 * The game shows one frame of each picture (its theme and style), so a
 * frame is read only when BM_AnimFrame() first asks for it: upstream
 * decodes every frame, six 1280x640 RGBA pictures for each sprite sheet,
 * about 20 MB and many seconds of PNG decoding on a 68040 for frames that
 * are never shown.
 * GPL 3 or later, like Blupimania. Copyright (c) 2026 AmiCradle Team. */
#include <string.h>
#include <SDL2/SDL.h>
#include "../src/sdl/SDL3_IMG.h"
#include "anim_order.h"

#define MAX_FRAMES 16

typedef struct
{
  IMG_Animation anim; /* first: IMG_FreeAnimation() frees the whole */
  SDL_Surface * frames[MAX_FRAMES];
  int           delays[MAX_FRAMES];
  char          base[1024];
} BMAnim;

static void
png_name (char * name, size_t size, const char * base, int frame)
{
  SDL_snprintf (name, size, "%s.%d.png", base, frame);
}

IMG_Animation *
BM_IMG_LoadWEBPAnimation (const char * file)
{
  char     name[1040];
  BMAnim * bm;
  size_t   len = strlen (file);
  int      count = 0;

  bm = (BMAnim *) SDL_calloc (1, sizeof (*bm));
  if (!bm)
  {
    SDL_OutOfMemory ();
    return NULL;
  }
  if (len >= sizeof (bm->base))
  {
    SDL_free (bm);
    return NULL;
  }
  memcpy (bm->base, file, len + 1);
  if (len > 5 && !SDL_strcasecmp (bm->base + len - 5, ".webp"))
    bm->base[len - 5] = '\0';

  /* count the frames without reading them */
  while (count < MAX_FRAMES)
  {
    SDL_RWops * rw;
    png_name (name, sizeof (name), bm->base, count);
    rw = SDL_RWFromFile (name, "rb");
    if (!rw)
      break;
    SDL_RWclose (rw);
    count++;
  }
  if (!count)
  {
    SDL_free (bm);
    return NULL;
  }

  bm->anim.count  = count;
  bm->anim.frames = bm->frames;
  bm->anim.delays = bm->delays;
  return &bm->anim;
}

SDL_Surface *
BM_AnimFrame (IMG_Animation * anim, int slot)
{
  BMAnim * bm = (BMAnim *) anim;
  char     name[1040];

  if (!anim || slot < 0 || slot >= anim->count)
    return NULL;
  if (!bm->frames[slot])
  {
    png_name (
      name, sizeof (name), bm->base,
      bm_anim_png_for_slot (slot, anim->count));
    bm->frames[slot] = IMG_Load (name);
    if (bm->frames[slot] && !anim->w)
    {
      anim->w = bm->frames[slot]->w;
      anim->h = bm->frames[slot]->h;
    }
  }
  return bm->frames[slot];
}

/* Frees the frames read and the animation; the frame and delay arrays
 * are part of it. */
void
BM_FreeAnimation (IMG_Animation * anim)
{
  BMAnim * bm = (BMAnim *) anim;
  int      i;

  if (!anim)
    return;
  for (i = 0; i < MAX_FRAMES; i++)
    SDL_FreeSurface (bm->frames[i]);
  SDL_free (bm);
}
