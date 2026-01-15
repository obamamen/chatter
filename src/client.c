/* ================================== *\
 @file     client.c
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

    mnet_socket_t client = mnet_socket(
        mnet_af_inet,
        mnet_sock_stream,
        mnet_ipproto_default);

    tcp_state s = tcp_state_init();

    mnet_sockaddr_in_t addr;
    if (mnet_addr_ipv4(&addr, "127.0.0.1", PORTN) != mnet_ok)
    {
        printf("failed to create address\n");
        mnet_close(client);
        return 1;
    }

    if (mnet_connect(client, MNET_SOCKADDR(addr), sizeof(addr)) != mnet_ok)
    {
        printf("failed to connect: %s\n", mnet_error_string(mnet_get_platform_error()));
        mnet_close(client);
        return 1;
    }

    while (1)
    {
        char buff[1024];
        int r = mnet_recv(client, buff, sizeof(buff), mnet_msg_none);
        printf("r=%d\n", r);
        if (r == -1)
        {
            printf("mnet_recv() failed\n");
            printf("error=%s\n", mnet_error_string(mnet_get_platform_error()));
        }
        printf("%s\n", buff);
    }

    if (mnet_socket_is_valid(client)) mnet_close(client);
    mnet_cleanup();
    mcli_cleanup();
}