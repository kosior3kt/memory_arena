#ifndef __ARENA__
#define __ARENA__

#define DEFAULT_CAPACITY 1024

typedef struct region {
    struct region* next;
    uint8_t *data;
    unsigned max_size;
    unsigned current_size;
} region_t;

typedef struct arena {
    region_t* start;
    region_t* end;
} arena_t;

static void init_region(region_t* _region, const unsigned _capacity)
{
    _region->data = (uint8_t*)malloc(_capacity * sizeof(uint8_t));
    memset(_region->data, '\0', _capacity);
    _region->current_size = 0;
    _region->max_size     = _capacity;
    _region->next         = NULL;
}

static void free_region(region_t** _region)
{
    if (*_region != NULL) {
        free((*_region)->data);
        free(*_region);
        *_region = NULL;
    }
}

static void init_arena(arena_t* _arena)
{
    region_t* starting_region = (region_t*)malloc(sizeof(region_t));
    init_region(starting_region, DEFAULT_CAPACITY);
    _arena->start = starting_region;
    _arena->end   = starting_region;
}

static void free_arena(arena_t* _arena)
{
    region_t* current_region = _arena->start;
    while (current_region)
    {
        region_t* to_be_removed = current_region;
        current_region = current_region->next;
        free_region(&to_be_removed);
    }
    _arena->start = _arena->end = NULL;
}

static void* give_me_memory(arena_t* _arena, const unsigned _amount)
{
    region_t* current_region = _arena->start;
    while (current_region->current_size + _amount > current_region->max_size)
    {
        if (current_region->next != NULL)
        {
            current_region = current_region->next;
			break;
        }
		region_t* new_region = (region_t*)malloc(sizeof(region_t));
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

#undef DEFAULT_CAPACITY

#endif// __ARENA__
