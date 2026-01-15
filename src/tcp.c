/* ================================== *\
 @file     tcp.c
 @project  chatter
 @author   moosm
 @date     1/15/2026
*\ ================================== */

#include "shared.h"

#include <stdlib.h>
#include <string.h>

tcp_state tcp_state_init()
{
    tcp_state state;
    state.inc = NULL;
    state.inc_size = 0;
    state.out = NULL;
    state.out_size = 0;
    return state;
}

void tcp_state_free(tcp_state* state)
{
    if (state->inc) {
        free(state->inc);
        state->inc = NULL;
        state->inc_size = 0;
    }
    if (state->out) {
        free(state->out);
        state->out = NULL;
        state->out_size = 0;
    }
}

const void* tcp_state_out_get(tcp_state* state)
{
    return state->out;
}

int tcp_state_out_append(tcp_state* state, void* mem, uint64_t size)
{
    if (!mem || size == 0) return 1;

    void* new_buf = realloc(state->out, state->out_size + size);
    if (!new_buf) return 1;

    memcpy((uint8_t*)new_buf + state->out_size, mem, size);
    state->out = new_buf;
    state->out_size += size;
    return 0;
}

int tcp_state_out_consume(tcp_state* state, uint64_t size)
{
    if (size > state->out_size) return 1;

    if (size == state->out_size) {
        free(state->out);
        state->out = NULL;
        state->out_size = 0;
        return 0;
    }

    uint64_t remaining = state->out_size - size;
    memmove(state->out, (uint8_t*)state->out + size, remaining);

    void* new_buf = realloc(state->out, remaining);
    if (new_buf) {
        state->out = new_buf;
    }
    state->out_size = remaining;
    return 0;
}

const void* tcp_state_inc_get(tcp_state* state)
{
    return state->inc;
}

int tcp_state_inc_append(tcp_state* state, void* mem, uint64_t size)
{
    if (!mem || size == 0) return 1;

    void* new_buf = realloc(state->inc, state->inc_size + size);
    if (!new_buf) return 1;

    memcpy((uint8_t*)new_buf + state->inc_size, mem, size);
    state->inc = new_buf;
    state->inc_size += size;
    return 0;
}

int tcp_state_inc_consume(tcp_state* state, uint64_t size)
{
    if (size > state->inc_size) return 1;

    if (size == state->inc_size) {
        free(state->inc);
        state->inc = NULL;
        state->inc_size = 0;
        return 0;
    }

    uint64_t remaining = state->inc_size - size;
    memmove(state->inc, (uint8_t*)state->inc + size, remaining);

    void* new_buf = realloc(state->inc, remaining);
    if (new_buf) {
        state->inc = new_buf;
    }
    state->inc_size = remaining;
    return 0;
}
