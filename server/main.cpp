// c
#ifndef _MSC_VER
#include <unistd.h>
#endif
#include <errno.h>
#include <stdlib.h>

// libmobus
#include <modbus.h>

// fmt
#include <fmt/core.h>

// spdlog
#include <spdlog/spdlog.h>


#ifdef _MSC_VER
#define close(h) CloseHandle((HANDLE)h)
#endif // _MSC_VER


int main(void)
{
    modbus_t *mb_ctx = modbus_new_tcp("127.0.0.1", 1502);
    // modbus_set_debug(ctx, TRUE);

    modbus_mapping_t *mb_mapping = modbus_mapping_new(500, 500, 500, 500);
    if (mb_mapping == NULL) {
        SPDLOG_ERROR("Failed to allocate the mapping: {}", modbus_strerror(errno));
        modbus_free(mb_ctx);
        return -1;
    }

    int sock = modbus_tcp_listen(mb_ctx, 1);
    modbus_tcp_accept(mb_ctx, &sock);

    for (;;) {
        uint8_t query[MODBUS_TCP_MAX_ADU_LENGTH];

        int rc = modbus_receive(mb_ctx, query);
        if (rc > 0) {
            // rc is the query size
            modbus_reply(mb_ctx, query, rc, mb_mapping);
        }
        else if (rc == -1) {
            // Connection closed by the client or error
            break;
        }
    }

    if (sock != -1) {
        close(sock);
    }
    modbus_mapping_free(mb_mapping);
    modbus_close(mb_ctx);
    modbus_free(mb_ctx);

    return 0;
}