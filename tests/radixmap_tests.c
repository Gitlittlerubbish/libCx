#include "minunit.h"
#include "radixmap.h"
#include <time.h>

#define N 200

static int make_random(RadixMap *map)
{
    size_t i = 0;

    for (i = 0; i < map->max - 1; i++) {
        uint32_t key = (uint32_t) (rand() | rand() << 16);
        check(RadixMap_add(map, key, i) == 0, "Failed to add key to map.");
    }

    return i;
error:
    return 0;
}

static int check_order(RadixMap *map)
{
    RMElement d1, d2;
    size_t i = 0;

    for (i = 0; map->end > 0 && i < map->end -1; i++) {
        d1 = map->contents[i];
        d2 = map->contents[i + 1];

        if (d1.data.key > d2.data.key) {
            debug("FAIL: i = %zu, key: %u, value: %u, equals max? %d\n",
                i, d1.data.key, d1.data.value, 
                d2.data.key == UINT32_MAX);
            return 0;
        }
    }

    return 1;
}

static int test_search(RadixMap *map) 
{
    size_t i = 0;
    RMElement *d = NULL;
    RMElement *found = NULL;

    for (i = map->end / 2; i < map->end; i++) {
        d = &map->contents[i];
        found = RadixMap_find(map, d->data.key);
        check(found != NULL, "Didn't find %u at %zu.", d->data.key, i);
        check(found->data.key == d->data.key,
                "Got wrong key: %u, looking for: %u.",
                found->data.key, d->data.key);
    }

    return 1;
error:
    return 0;
}

char *test_operations()
{
     RadixMap *map = RadixMap_create(N);
     mu_assert(map != NULL, "Failed to create a map.");
     mu_assert(make_random(map), "Failed to make a random map.");

     RadixMap_sort(map);
     mu_assert(check_order(map),
                    "Failed to sort correctly.");

     mu_assert(test_search(map), "Failed to search.");
     mu_assert(check_order(map),
                "Map failed to stay sorted after searched.")

     while (map->end > 0) {
        RMElement *ele = RadixMap_find(map,
                        map->contents[map->end/2].data.key);
        mu_assert(ele != NULL, "Should get an element.");

        size_t old_end = map->end;

        mu_assert(RadixMap_delete(map, ele) == 0, 
                    "Didn't delete correctly.");
        mu_assert(old_end -1 == map->end, 
                    "Wrong end after deleting.");

        mu_assert(check_order(map), 
                    "Map didn't stay sorted after deleting.");

     }

     RadixMap_destroy(map);

     return NULL;
}

char *all_tests()
{
    mu_suite_start();
    //set real random time values
    srand(time(NULL));

    mu_run_test(test_operations);

    return NULL;
}

RUN_TESTS(all_tests);


