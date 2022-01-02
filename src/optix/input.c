#include "input.h"

#include <stdint.h>
#include <debug.h>
#include <tice.h>
#include <keypadc.h>

#include "gui_control.h"

void optix_UpdateKeyState(struct optix_key *key) {
    if (kb_IsDown(key->key)) {
        //if the key was active last loop, it was held
        key->state = (key->state == KEY_PRESSED || key->state == KEY_HELD) ? KEY_HELD : KEY_PRESSED;
    } else {
        //if the key was active last loop, it was released
        key->state = (key->state && key->state != KEY_RELEASED) ? KEY_RELEASED : KEY_INACTIVE;
    }
}


void optix_UpdateInput(void) {
    int i = 0;
    kb_Scan();
    current_context->data->key = os_GetCSC();
    for (i = 0; i < NUM_DEFAULT_KEYS; i++) {
        //dbg_sprintf(dbgout, "Key %d State %d\n", i, current_context->input->default_key[i]->state);
        optix_UpdateKeyState(&(current_context->input->default_key[i]));
    }
    /*
    i = 0;
    while (current_context->input->key && current_context->input->key[i]) {
        optix_UpdateKeyState(current_context->input->key[i]);
        i++;
    }*/
}

//returns the state of the default key at index
uint8_t optix_DefaultKeyIsDown(int index) {
    return current_context->input->default_key[index].state;
}

//returns the state of the custom key at index
uint8_t optix_KeyIsDown(int index) {
    return current_context->input->key[index]->state;
}

//sets the state of a default key
void optix_SetDefaultKeyState(int index, uint8_t state) {
    current_context->input->default_key[index].state = state;
}

void optix_InitializeInput(struct optix_input *input) {
    //having these be static is okay, I think
    struct optix_key enter = {.key = kb_KeyEnter};
    struct optix_key up    = {.key = kb_KeyUp};
    struct optix_key down  = {.key = kb_KeyDown};
    struct optix_key left  = {.key = kb_KeyLeft};
    struct optix_key right = {.key = kb_KeyRight};
    struct optix_key add = {.key = kb_KeyAdd};
    struct optix_key sub = {.key = kb_KeySub};
    struct optix_key *default_key[NUM_DEFAULT_KEYS] = {
        &enter,
        &up,
        &down,
        &left,
        &right,
        &add,
        &sub,
    };
    dbg_sprintf(dbgout, "Initializing input...\n");
    for (int i = 0; i < NUM_DEFAULT_KEYS; i++) {
        dbg_sprintf(dbgout, "Loop %d\n", i);
        input->default_key[i].key = default_key[i]->key;
        input->default_key[i].state = KEY_INACTIVE;
    }
    input->key = 0;
    dbg_sprintf(dbgout, "Finished.\n");
}