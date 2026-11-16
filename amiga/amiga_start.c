/* The Amiga port's start: the stack, the version string and PROGDIR:User
 * (the players' files and saved games, smaky.c's getLocationPattern()),
 * made before main().
 * GPL 3 or later, like Blupimania. Copyright (c) 2026 AmiCradle Team. */
#include <sys/stat.h>

/* A Shell gives 4 KB of stack; SDL 2 and the game's big locals want more.
 * libnix's startup sets this much up. */
unsigned long __stack = 512 * 1024;
static const char version[] __attribute__((used)) =
    "$VER: Blupimania 1.6.3 (16.11.2026)";

static void __attribute__((constructor)) amiga_user_drawer(void)
{
	mkdir("PROGDIR:User", 0755);
}
