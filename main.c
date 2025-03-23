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

	const char* first = "hello ";
	const char* second = "world";
	dupa = concat_string(&new_arena, first, second);
	printf("%s\n", dupa);

    free_arena(&new_arena);

    return 0;
}
