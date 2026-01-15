/* ================================== *\
 @file     tcp.h
 @project  chatter
 @author   moosm
 @date     1/15/2026
*\ ================================== */

#ifndef CHATTER_TCP_H
#define CHATTER_TCP_H

#include <stdint.h>

typedef struct tcp_state
{
    void* inc;
    uint64_t inc_size;

    void* out;
    uint64_t out_size;

} tcp_state;

tcp_state tcp_state_init();

// can be null.
const void* tcp_state_out_get(tcp_state* state);

void tcp_state_free(tcp_state* state);

// 0=OK , 1=ERROR
int tcp_state_out_append(tcp_state* state, void* mem, uint64_t size);

// 0=OK , 1=ERROR
int tcp_state_out_consume(tcp_state* state, uint64_t size);

// can be null.
const void* tcp_state_inc_get(tcp_state* state);

// 0=OK , 1=ERROR
int tcp_state_inc_append(tcp_state* state, void* mem, uint64_t size);

// 0=OK , 1=ERROR
int tcp_state_inc_consume(tcp_state* state, uint64_t size);

#endif //CHATTER_TCP_H