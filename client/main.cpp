// c
#ifndef _MSC_VER
#include <unistd.h>
#endif
#include <errno.h>
#include <stdlib.h>
#include <string.h>

// modbus
#include <modbus.h>

// fmt
#include <fmt/core.h>

// spdlog
#include <spdlog/spdlog.h>


 /* 
	The goal of this program is to check all major functions of
	libmodbus:
	- write_coil
	- read_bits
	- write_coils
	- write_register
	- read_registers
	- write_registers
	- read_registers

	All these functions are called with random values on a address
	range defined by the following defines.
 */
#define MAX_LOOPS     1
#define ADDRESS_START 0
#define ADDRESS_END   99

 /* 
  * At each loop, the program works in the range ADDRESS_START to
  * ADDRESS_END then ADDRESS_START + 1 to ADDRESS_END and so on.
  */
int main(void)
{
	// TCP
	modbus_t *mb_ctx = modbus_new_tcp("127.0.0.1", 1502);
	modbus_set_debug(mb_ctx, TRUE);

	if (modbus_connect(mb_ctx) == -1) {
		SPDLOG_ERROR("Connection failed: {}", modbus_strerror(errno));
		modbus_free(mb_ctx);
		return -1;
	}

	// Allocate and initialize the different memory spaces
	int count = ADDRESS_END - ADDRESS_START;

	uint8_t *array_req_bits = (uint8_t *)malloc(count * sizeof(uint8_t));
	memset(array_req_bits, 0, count * sizeof(uint8_t));

	uint8_t *array_rsp_bits = (uint8_t *)malloc(count * sizeof(uint8_t));
	memset(array_rsp_bits, 0, count * sizeof(uint8_t));

	uint16_t *array_req_registers = (uint16_t *)malloc(count * sizeof(uint16_t));
	memset(array_req_registers, 0, count * sizeof(uint16_t));

	uint16_t *array_rsp_registers = (uint16_t *)malloc(count * sizeof(uint16_t));
	memset(array_rsp_registers, 0, count * sizeof(uint16_t));

	uint16_t *array_rw_req_registers = (uint16_t *)malloc(count * sizeof(uint16_t));
	memset(array_rw_req_registers, 0, count * sizeof(uint16_t));

	int fails = 0;
	int loops = 0;
	while (loops++ < MAX_LOOPS) {
		for (int addr = ADDRESS_START; addr < ADDRESS_END; addr++) {
			// Random numbers (short)
			for (int i = 0; i < count; i++) {
				array_req_registers[i] = (uint16_t)(65535.0 * rand() / (RAND_MAX + 1.0));
				array_rw_req_registers[i] = ~array_req_registers[i];
				array_req_bits[i] = array_req_registers[i] % 2;
			}
			count = ADDRESS_END - addr;

			// WRITE BIT
			int rc = modbus_write_bit(mb_ctx, addr, array_req_bits[0]);
			if (rc != 1) {
				SPDLOG_ERROR("modbus_write_bit ({})", rc);
				SPDLOG_INFO("Address = {}, value = {}", addr, array_req_bits[0]);
				fails++;
			}
			else {
				rc = modbus_read_bits(mb_ctx, addr, 1, array_rsp_bits);
				if (rc != 1 || array_req_bits[0] != array_rsp_bits[0]) {
					SPDLOG_ERROR("modbus_read_bits single ({})", rc);
					SPDLOG_INFO("address = {}", addr);
					fails++;
				}
			}

			// MULTIPLE BITS
			rc = modbus_write_bits(mb_ctx, addr, count, array_req_bits);
			if (rc != count) {
				SPDLOG_ERROR("modbus_write_bits ({})", rc);
				SPDLOG_INFO("Address = {}, nb = {}", addr, count);
				fails++;
			}
			else {
				rc = modbus_read_bits(mb_ctx, addr, count, array_rsp_bits);
				if (rc != count) {
					SPDLOG_ERROR("modbus_read_bits");
					SPDLOG_INFO("Address = {}, nb = {}", addr, count);
					fails++;
				}
				else {
					for (int i = 0; i < count; i++) {
						if (array_rsp_bits[i] != array_req_bits[i]) {
							SPDLOG_ERROR("modbus_read_bits");
							SPDLOG_INFO("Address = {}, value {} (0x{:X}) != {} (0x{:X})",
								addr,
								array_req_bits[i],
								array_req_bits[i],
								array_rsp_bits[i],
								array_rsp_bits[i]);
							fails++;
						}
					}
				}
			}

			// SINGLE REGISTER
			rc = modbus_write_register(mb_ctx, addr, array_req_registers[0]);
			if (rc != 1) {
				SPDLOG_ERROR("modbus_write_register ({})", rc);
				SPDLOG_INFO("Address = {}, value = {} (0x{:X})",
					addr,
					array_req_registers[0],
					array_req_registers[0]);
				fails++;
			}
			else {
				rc = modbus_read_registers(mb_ctx, addr, 1, array_rsp_registers);
				if (rc != 1) {
					SPDLOG_ERROR("modbus_read_registers single ({})", rc);
					SPDLOG_INFO("Address = {}", addr);
					fails++;
				}
				else {
					if (array_req_registers[0] != array_rsp_registers[0]) {
						SPDLOG_ERROR("modbus_read_registers single");
						SPDLOG_INFO("Address = {}, value = {} (0x{:X}) != {} (0x{:X})",
							addr,
							array_req_registers[0],
							array_req_registers[0],
							array_rsp_registers[0],
							array_rsp_registers[0]);
						fails++;
					}
				}
			}

			// MULTIPLE REGISTERS
			rc = modbus_write_registers(mb_ctx, addr, count, array_req_registers);
			if (rc != count) {
				SPDLOG_ERROR("modbus_write_registers ({})", rc);
				SPDLOG_INFO("Address = {}, nb = {}", addr, count);
				fails++;
			}
			else {
				rc = modbus_read_registers(mb_ctx, addr, count, array_rsp_registers);
				if (rc != count) {
					SPDLOG_ERROR("modbus_read_registers ({})", rc);
					SPDLOG_INFO("Address = {}, nb = {}", addr, count);
					fails++;
				}
				else {
					for (int i = 0; i < count; i++) {
						if (array_req_registers[i] != array_rsp_registers[i]) {
							SPDLOG_ERROR("modbus_read_registers");
							SPDLOG_INFO("Address = {}, value {} (0x{:X}) != {} (0x{:X})",
								addr,
								array_req_registers[i],
								array_req_registers[i],
								array_rsp_registers[i],
								array_rsp_registers[i]);
							fails++;
						}
					}
				}
			}
			// R/W MULTIPLE REGISTERS
			rc = modbus_write_and_read_registers(
				mb_ctx, addr, count, array_rw_req_registers, addr, count, array_rsp_registers);
			if (rc != count) {
				SPDLOG_ERROR("modbus_read_and_write_registers ({})", rc);
				SPDLOG_INFO("Address = {}, nb = {}", addr, count);
				fails++;
			}
			else {
				for (int i = 0; i < count; i++) {
					if (array_rsp_registers[i] != array_rw_req_registers[i]) {
						SPDLOG_ERROR("modbus_read_and_write_registers READ");
						SPDLOG_INFO("Address = {}, value {} (0x{:X}) != {} (0x{:X})",
							addr,
							array_rsp_registers[i],
							array_rw_req_registers[i],
							array_rsp_registers[i],
							array_rw_req_registers[i]);
						fails++;
					}
				}

				rc = modbus_read_registers(mb_ctx, addr, count, array_rsp_registers);
				if (rc != count) {
					SPDLOG_ERROR("modbus_read_registers ({})", rc);
					SPDLOG_INFO("Address = {}, nb = {}", addr, count);
					fails++;
				}
				else {
					for (int i = 0; i < count; i++) {
						if (array_rw_req_registers[i] != array_rsp_registers[i]) {
							SPDLOG_ERROR("modbus_read_and_write_registers WRITE");
							SPDLOG_INFO("Address = {}, value {} (0x{:X}) != {} (0x{:X})",
								addr,
								array_rw_req_registers[i],
								array_rw_req_registers[i],
								array_rsp_registers[i],
								array_rsp_registers[i]);
							fails++;
						}
					}
				}
			}
		}

		SPDLOG_INFO("Test: ");
		if (fails)
			SPDLOG_INFO("{} FAILS", fails);
		else
			SPDLOG_INFO("SUCCESS");
	}

	// Free the memory
	free(array_req_bits);
	free(array_rsp_bits);
	free(array_req_registers);
	free(array_rsp_registers);
	free(array_rw_req_registers);

	// Close the connection
	modbus_close(mb_ctx);
	modbus_free(mb_ctx);

	return 0;
}