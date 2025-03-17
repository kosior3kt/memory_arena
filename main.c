#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <assert.h>
#include "arena.h"

int main()
{
    arena_t new_arena;
    init_arena(&new_arena);
    char* dupa;
    dupa = (char*)give_me_memory(&new_arena, 256);
    strcpy(dupa, "does this work?");
    printf("%s\n", dupa);
    free_arena(&new_arena);

    return 0;
}
