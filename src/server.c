/* ================================== *\
 @file     server.c
 @project  chatter
 @author   moosm
 @date     1/15/2026
*\ ================================== */

#include "shared.h"
#include <stdio.h>

int main(void)
{
    if (mnet_initialize() != mnet_ok)
    {
        printf("mnet_initialize() failed\n");
        return 1;
    }

    if (mcli_initialize() != mcli_ok)
    {
        printf("mcli_initialize() failed\n");
        return 1;
    }

    mnet_socket_t server = mnet_socket(
        mnet_af_inet,
        mnet_sock_stream,
        mnet_ipproto_default);

    mnet_sockaddr_in_t addr;
    if (mnet_addr_any_ipv4(&addr, PORTN) != mnet_ok)
    {
        printf("failed to create address\n");
        mnet_close(server);
        return 1;
    }

    if (mnet_bind(server, MNET_SOCKADDR(addr), sizeof(addr)) != mnet_ok)
    {
        printf("failed to bind: %s\n", mnet_error_string(mnet_get_platform_error()));
        mnet_close(server);
        return 1;
    }

    if (mnet_listen(server, 1024))
    {
        printf("failed to listen: %s\n", mnet_error_string(mnet_get_platform_error()));
        mnet_close(server);
        return 1;
    }

    if (mnet_set_reuseaddr(server, 1) != mnet_ok)
    {
        printf("failed to set reuseaddr\n");
        mnet_close(server);
        return 1;
    }

    if (mnet_set_blocking(server, 0) != mnet_ok)
    {
        printf("failed to set blocking\n");
        mnet_close(server);
        return 1;
    }

    printf("server listening on port %d\n", PORTN);

    mnet_socket_t client = MNET_INVALID_SOCKET;
    tcp_state s = tcp_state_init();
    char msg[] = "hello client";
    tcp_state_out_append(&s, msg, sizeof(msg));


    int should_run = 1;
    while (should_run == 1)
    {
        mnet_socket_t nclient = mnet_accept(server, 0, 0);

        if (mnet_socket_is_valid(nclient))
        {
            printf("client connected\n");
            client = nclient;
            if (mnet_set_blocking(client, 0) != mnet_ok)
            {
                printf("failed to set blocking for client\n");
            }
        }

        if (mnet_socket_is_valid(client))
        {
            int r = mnet_send(client, tcp_state_out_get(&s), s.out_size, 0);

            if ( r == -1)
            {
                if (mnet_get_platform_error() != mnet_ewouldblock)
                {
                    printf("mnet_send() failed:\n");
                    printf(" - %s\n", mnet_error_string(mnet_get_platform_error()));
                }
            }

            if (r > 0)
            {
                tcp_state_out_consume(&s, r);
                printf("sent %d bytes\n", r);
            }
        }
    }

    tcp_state_free(&s);
    if (mnet_socket_is_valid(client)) mnet_close(client);
    mnet_cleanup();
    mcli_cleanup();
}