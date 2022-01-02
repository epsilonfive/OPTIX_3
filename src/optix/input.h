#ifndef INPUT_H
#define INPUT_H

#include <keypadc.h>
#include <stdint.h>

#define KEY_INACTIVE     0
#define KEY_PRESSED      (1 << 0)
#define KEY_HELD         (1 << 1)
#define KEY_RELEASED     (1 << 2)

//the default keys, these shouldn't be changed
#define NUM_DEFAULT_KEYS 7
#define KEY_ENTER        0
#define KEY_UP           1
#define KEY_DOWN         2
#define KEY_LEFT         3
#define KEY_RIGHT        4
#define KEY_ADD          5
#define KEY_SUB          6

struct optix_key {
    kb_lkey_t key;
    uint8_t state;
    struct optix_key_chain *chain;
};

//this will be included in the optix_key struct-if it's defined, then the key can only be activated if
//the optix_key pointed to by key is in the state specified by state
struct optix_key_chain {
    struct optix_key *key;
    uint8_t state;
};

struct optix_input {
    struct optix_key default_key[NUM_DEFAULT_KEYS];
    struct optix_key **key;
};

void optix_UpdateInput(void);

//initializes the default_key array in input, according to a structure defined in that function
void optix_InitializeInput(struct optix_input *input);

uint8_t optix_DefaultKeyIsDown(int index);
uint8_t optix_KeyIsDown(int index);
void optix_SetDefaultKeyState(int index, uint8_t state);

#endif