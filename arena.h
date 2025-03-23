#ifndef __ARENA__
#define __ARENA__

#define DEFAULT_CAPACITY 1024

typedef struct region {
    struct region* next;
    uint8_t *data;
    unsigned max_size;
    unsigned current_size;
} region_s;

typedef struct arena {
    region_s* start;
    region_s* end;
} arena_s;

static void init_region(region_s* _region, const unsigned _capacity)
{
    _region->data = (uint8_t*)malloc(_capacity * sizeof(uint8_t));
    memset(_region->data, '\0', _capacity);
    _region->current_size = 0;
    _region->max_size     = _capacity;
    _region->next         = NULL;
}

static void free_region(region_s** _region)
{
    if (*_region != NULL) {
        free((*_region)->data);
        free(*_region);
        *_region = NULL;
    }
}

static void init_arena(arena_s* _arena)
{
    region_s* starting_region = (region_s*)malloc(sizeof(region_s));
    init_region(starting_region, DEFAULT_CAPACITY);
    _arena->start = starting_region;
    _arena->end   = starting_region;
}

static void free_arena(arena_s* _arena)
{
    region_s* current_region = _arena->start;
    while (current_region) {
        region_s* to_be_removed = current_region;
        current_region = current_region->next;
        free_region(&to_be_removed);
    }
    _arena->start = _arena->end = NULL;
}

static void* give_me_raw_memory(arena_s* _arena, const unsigned _amount)
{
    region_s* current_region = _arena->start;
    while (current_region->current_size + _amount > current_region->max_size) {
        if (current_region->next != NULL) {
            current_region = current_region->next;
			break;
        }
		region_s* new_region = (region_s*)malloc(sizeof(region_s));
		{
			if (_amount > DEFAULT_CAPACITY) init_region(new_region, _amount);
			else							init_region(new_region, DEFAULT_CAPACITY);
		}
		current_region->next = new_region;
		_arena->end = new_region;
		current_region = new_region;
    }
    unsigned temp_size = current_region->current_size;
    current_region->current_size += _amount;
    return (void*)(current_region->data + temp_size);
}


static char* concat_string(arena_s* _arena, const char* _first, const char* _second)
{
	///if these are not null terminated then unlucky
	unsigned alloc_size = strlen(_first) + strlen(_second);
	char* new_string = give_me_raw_memory(_arena, alloc_size);

	unsigned i = 0;
	for(i = 0; _first[i] != '\0'; ++i) {
		new_string[i] = _first[i];
	}
	unsigned j = 0;
	for(j = 0; _second[j] != '\0'; ++j) {
		new_string[i + j] = _second[j];
	}
	new_string[i + j + 1] = '\0';
	return new_string;
}


#undef DEFAULT_CAPACITY

#endif// __ARENA__
