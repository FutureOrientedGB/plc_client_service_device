// fmt
#include <fmt/core.h>

// libmobus
#include <modbus.h>

// spdlog
#include <spdlog/spdlog.h>



int main()
{
    int error = 0;

    // setup modbus tcp client
    const char *ip = "127.0.0.1";
    const int port = 5002;
    modbus_t *mobus_context = modbus_new_tcp(ip, port);
    if (mobus_context == nullptr)
    {
        SPDLOG_ERROR("modbus_new_tcp({}, {}) error", ip, port);
        return --error;
    }

    // connect to server
    if (modbus_connect(mobus_context) == -1)
    {
        {
            SPDLOG_ERROR("modbus_connect({}) error, details: {}", fmt::ptr(mobus_context), modbus_strerror(errno));
            modbus_free(mobus_context);
            return --error;
        }

        // read registers
        uint16_t tab_reg[32];
        const int addr = 0;
        const int registers = 10;
        int rc = modbus_read_registers(mobus_context, addr, registers, tab_reg); // read 10 registers from 0 address
        if (rc == -1)
        {
            SPDLOG_ERROR("modbus_read_registers({}, {}, {}) error, details: {}", fmt::ptr(mobus_context), addr, registers, modbus_strerror(errno));
        }
        else
        {
            for (int i = 0; i < rc; i++)
            {
                SPDLOG_INFO("{}", tab_reg[i]);
            }
        }

        // write register
        uint16_t value = 42;
        rc = modbus_write_register(mobus_context, addr, value);
        if (rc == -1)
        {
            SPDLOG_ERROR("modbus_write_register({}, {}, {}) error, details: {}", fmt::ptr(mobus_context), addr, value, modbus_strerror(errno));
        }

        // close connections
        modbus_close(mobus_context);

        // free resources;
        modbus_free(mobus_context);

        return 0;
    }
}
