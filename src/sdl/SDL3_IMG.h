#pragma once

#include <SDL2/SDL_image.h>

#if defined(__amigaos__) && !SDL_IMAGE_VERSION_ATLEAST(2, 6, 0)
/* SDL2_image before 2.6 has no animations: the same struct. */
typedef struct
{
  int            w, h;
  int            count;
  SDL_Surface ** frames;
  int *          delays;
} IMG_Animation;
#endif

/* On the Amiga the frames are PNGs (amiga/anim_png.c), not WebP, each
 * read when BM_AnimFrame() first asks for it, and freed with
 * BM_FreeAnimation(). */
IMG_Animation * BM_IMG_LoadWEBPAnimation (const char *file);
#ifdef __amigaos__
SDL_Surface * BM_AnimFrame (IMG_Animation * anim, int slot);
void          BM_FreeAnimation (IMG_Animation * anim);
#endif
