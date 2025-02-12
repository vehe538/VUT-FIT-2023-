#include <assert.h>
#include <string.h>
#define NR_TESTS 1000
#define TEST_SIZE 101

#include "hashtable.h"
#include "test_util.h"
#include <stdio.h>
#include <stdlib.h>

#define INSERT_TEST_DATA(TABLE)                                                \
    ht_insert_many(TABLE, TEST_DATA, sizeof(TEST_DATA) / sizeof(TEST_DATA[0]));

const ht_item_t TEST_DATA[15] = {
    {"Bitcoin", 53247.71, NULL},    {"Ethereum", 3208.67, NULL},
    {"Binance Coin", 409.15, NULL}, {"Cardano", 1.82, NULL},
    {"Tether", 0.86, NULL},         {"XRP", 0.93, NULL},
    {"Solana", 134.50, NULL},       {"Polkadot", 34.99, NULL},
    {"Dogecoin", 0.22, NULL},       {"USD Coin", 0.86, NULL},
    {"Uniswap", 21.68, NULL},       {"Terra", 30.67, NULL},
    {"Litecoin", 156.87, NULL},     {"Avalanche", 47.03, NULL},
    {"Chainlink", 21.90, NULL},
};

void init_test() {
    printf("Hash Table - testing script\n");
    printf("---------------------------\n");
    HT_SIZE = 13;
    printf("\nSetting HT_SIZE to prime number (%i)\n", HT_SIZE);
    printf("\n");
}

TEST(test_table_init, "Initialize the table")
ht_init(test_table);
ENDTEST

TEST(test_search_nonexist, "Search for a non-existing item")
ht_init(test_table);
ht_search(test_table, "Ethereum");
ENDTEST

TEST(test_insert_simple, "Insert a new item")
ht_init(test_table);
ht_insert(test_table, "Ethereum", 3208.67);
ENDTEST

TEST(test_search_exist, "Search for an existing item")
ht_init(test_table);
ht_insert(test_table, "Ethereum", 3208.67);
ht_search(test_table, "Ethereum");
ENDTEST

TEST(test_insert_many, "Insert many new items")
ht_init(test_table);
INSERT_TEST_DATA(test_table)
ENDTEST

TEST(test_search_collision, "Search for an item with colliding hash")
ht_init(test_table);
INSERT_TEST_DATA(test_table)
ht_search(test_table, "Terra");
ENDTEST

TEST(test_insert_update, "Update an item")
ht_init(test_table);
INSERT_TEST_DATA(test_table)
ht_insert(test_table, "Ethereum", 12.34);
ENDTEST

TEST(test_get, "Get an item's value")
ht_init(test_table);
INSERT_TEST_DATA(test_table)
ht_get(test_table, "Ethereum");
ENDTEST

TEST(test_delete, "Delete an item")
ht_init(test_table);
INSERT_TEST_DATA(test_table)
ht_delete(test_table, "Terra");
ENDTEST

TEST(test_delete_all, "Delete all the items")
ht_init(test_table);
INSERT_TEST_DATA(test_table)
ht_delete_all(test_table);
ENDTEST

// ipt enjoyers can check in how many cases this will return zero
float rand_nonzero() { return rand() + rand() + rand(); }

char rand_ascii() { return (rand() % 127) + 1; }

void check_values(ht_table_t *table, char keys[TEST_SIZE][6], float *results) {
    for (int i = 0; i < TEST_SIZE; i++) {
        ht_item_t *search = ht_search(table, keys[i]);
        float *get = ht_get(table, keys[i]);

        if (results[i] == 0.0) {
            assert(search == NULL);
            assert(get == NULL);
        } else {
            assert(!strcmp(search->key, keys[i]));
            assert(search->value == results[i]);
            assert(*get == results[i]);

            // lets test the ptr is actually writable
            float newval = rand_nonzero();
            *get = newval;
            results[i] = newval;
        }
    }
}


int main() {
    srand(42);

    init_uninitialized_item();
    init_test();

    test_table_init();
    test_search_nonexist();
    test_insert_simple();
    test_search_exist();
    test_insert_many();
    test_search_collision();
    test_insert_update();
    test_get();
    test_delete();
    test_delete_all();

    free(uninitialized_item);
}
