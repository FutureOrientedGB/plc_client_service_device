// fmt
#include <fmt/core.h>

// libmobus
#include <modbus.h>

// spdlog
#include <spdlog/spdlog.h>



int main()
{
    int error = 0;

    // setup modbus tcp server
    const char *ip = "127.0.0.1";
    const int port = 5002;
    modbus_t *mobus_context = modbus_new_tcp(ip, port);
    if (mobus_context == nullptr)
    {
        SPDLOG_ERROR("modbus_new_tcp({}, {}) error", ip, port);
        return --error;
    }

    // listen with max 64 connections
    const int max_connections = 64;
    int master_socket = modbus_tcp_listen(mobus_context, max_connections);
    if (master_socket == -1)
    {
        SPDLOG_ERROR("modbus_tcp_listen({}, {}) error, details: {}", fmt::ptr(mobus_context), max_connections, modbus_strerror(errno));
        modbus_free(mobus_context);
        return --error;
    }

    // create mobus 32 registers map
    const int max_bits = 0;
    const int max_input_bits = 0;
    const int max_registers = 32;
    const int max_input_registers = 0;
    modbus_mapping_t *mb_mapping = modbus_mapping_new(max_bits, max_input_bits, max_registers, max_input_registers);
    if (mb_mapping == nullptr)
    {
        SPDLOG_ERROR("modbus_mapping_new({}, {}, {}, {}) error", max_bits, max_input_bits, max_registers, max_input_registers);
        modbus_free(mobus_context);
        return --error;
    }

    while (true)
    {
        // accept
        int client_socket;
        if (modbus_tcp_accept(mobus_context, &client_socket) == -1)
        {
            SPDLOG_ERROR("modbus_tcp_accept({}) error", fmt::ptr(mobus_context));
            continue;
        }

        while (true)
        {
            // receive
            uint8_t query[MODBUS_TCP_MAX_ADU_LENGTH];
            int rc = modbus_receive(mobus_context, query);
            if (rc > 0)
            {
                // reply
                modbus_reply(mobus_context, query, rc, mb_mapping);
            }
            else if (rc == -1)
            {
                SPDLOG_ERROR("modbus_receive({}) error, details: {}", fmt::ptr(mobus_context), modbus_strerror(errno));
                return --error;
            }
        }

        // close connections
        modbus_close(mobus_context);
    }

    // free resources
    modbus_mapping_free(mb_mapping);
    modbus_free(mobus_context);

    return 0;
}