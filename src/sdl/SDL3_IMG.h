#pragma once

#include <SDL2/SDL_image.h>

#if defined(__amigaos__) && !SDL_IMAGE_VERSION_ATLEAST(2, 6, 0)
/* SDL2_image before 2.6 has no animations: the same struct, and its
 * IMG_FreeAnimation() in amiga/anim_png.c. */
typedef struct
{
  int            w, h;
  int            count;
  SDL_Surface ** frames;
  int *          delays;
} IMG_Animation;
void IMG_FreeAnimation (IMG_Animation * anim);
#endif

/* On the Amiga the frames are PNGs (amiga/anim_png.c), not WebP. */
IMG_Animation * BM_IMG_LoadWEBPAnimation (const char *file);
