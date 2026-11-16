/* BM_IMG_LoadWEBPAnimation() for the Amiga build of Blupimania: the game's
 * pictures are animated WebP files whose frames are its themes and styles;
 * here each frame is a PNG beside where the WebP would be
 * (image/[lang/]blupixNN.F.png, made from upstream's by
 * tools/webp_frames.py), read with GFOpenUp's SDL2_image.module. A missing
 * frame ends the animation; no frame at all gives NULL, as a missing WebP
 * does upstream.
 * GPL 3 or later, like Blupimania. Copyright (c) 2026 AmiCradle Team. */
#include <string.h>
#include <SDL2/SDL.h>
#include "../src/sdl/SDL3_IMG.h"

#define MAX_FRAMES 16

IMG_Animation *
BM_IMG_LoadWEBPAnimation (const char * file)
{
  char           base[1024];
  char           name[1040];
  SDL_Surface *  frames[MAX_FRAMES];
  IMG_Animation * anim;
  size_t         len = strlen (file);
  int            count = 0;

  if (len >= sizeof (base))
    return NULL;
  memcpy (base, file, len + 1);
  if (len > 5 && !SDL_strcasecmp (base + len - 5, ".webp"))
    base[len - 5] = '\0';

  while (count < MAX_FRAMES)
  {
    SDL_snprintf (name, sizeof (name), "%s.%d.png", base, count);
    frames[count] = IMG_Load (name);
    if (!frames[count])
      break;
    count++;
  }
  if (!count)
    return NULL;

  anim = (IMG_Animation *) SDL_calloc (1, sizeof (*anim));
  if (anim)
  {
    anim->frames = (SDL_Surface **) SDL_calloc (count, sizeof (SDL_Surface *));
    anim->delays = (int *) SDL_calloc (count, sizeof (int));
  }
  if (!anim || !anim->frames || !anim->delays)
  {
    while (count--)
      SDL_FreeSurface (frames[count]);
    if (anim)
    {
      SDL_free (anim->frames);
      SDL_free (anim->delays);
      SDL_free (anim);
    }
    SDL_OutOfMemory ();
    return NULL;
  }
  anim->w     = frames[0]->w;
  anim->h     = frames[0]->h;
  anim->count = count;
  memcpy (anim->frames, frames, count * sizeof (SDL_Surface *));
  return anim;
}

#if !SDL_IMAGE_VERSION_ATLEAST(2, 6, 0)
void
IMG_FreeAnimation (IMG_Animation * anim)
{
  int i;
  if (!anim)
    return;
  for (i = 0; i < anim->count; i++)
    SDL_FreeSurface (anim->frames[i]);
  SDL_free (anim->frames);
  SDL_free (anim->delays);
  SDL_free (anim);
}
#endif
