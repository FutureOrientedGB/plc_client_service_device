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

// protocol
#include "../protocol/protocol.hpp"



int main(int argc, char **argv)
{
	// TCP
	modbus_t *mb_ctx = modbus_new_tcp("127.0.0.1", 1502);
	modbus_set_debug(mb_ctx, TRUE);

	if (modbus_connect(mb_ctx) == -1) {
		SPDLOG_ERROR("Connection failed: {}", modbus_strerror(errno));
		modbus_free(mb_ctx);
		return -1;
	}

	NewPlcModbusOpertion mb_op(mb_ctx);

	{
		LaneIndicatorRegisterValue v = LaneIndicatorRegisterValue::ForwardTurnOnly;
		bool r = mb_op.WriteLaneIndicator(v);
		assert(r && v == mb_op.ReadLaneIndicator());
	}

	{
		TrafficLightRegisterValue v = TrafficLightRegisterValue::RedLightTurnAllowed;
		bool r = mb_op.WriteTrafficLights(v);
		assert(r && v == mb_op.ReadTrafficLights());
	}

	{
		FanRegisterValue v = FanRegisterValue::Reverse;
		bool r = mb_op.WriteFans(v);
		assert(r && v == mb_op.ReadFans());
	}

	{
		RollerShutterRegisterValue v = RollerShutterRegisterValue::Down;
		bool r = mb_op.WriteRollerShutter(v);
		assert(r && v == mb_op.ReadRollerShutter());
	}

	{
		LightingRegisterValue v = LightingRegisterValue::On;
		bool r = mb_op.WriteLighting(v);
		assert(r && v == mb_op.ReadLighting());
	}

	{
		WaterPumpRegisterValue v = WaterPumpRegisterValue::Start;
		bool r = mb_op.WriteWaterPump(v);
		assert(r && v == mb_op.ReadWaterPump());
	}

	{
		uint32_t co = 1000;
		uint32_t vi = 2000;
		uint32_t no = 3000;
		bool r = mb_op.WriteCoViNo(co, vi, no);
		auto result = mb_op.ReadCoViNo();
		assert(r && co == result[0] && vi == result[1] && no == result[2]);
	}

	{
		uint32_t speed = 123456;
		WindDirectionRegisterValue v = WindDirectionRegisterValue::Reverse;
		bool r = mb_op.WriteWindSpeedDirection(speed, v);
		auto result = mb_op.ReadWindSpeedDirection();
		assert(r && speed == result.first && v == result.second);
	}

	{
		uint32_t v = 1024;
		bool r = mb_op.WriteIlluminanceDetection(v);
		assert(r && v == mb_op.ReadIlluminanceDetection());
	}

	{
		uint32_t v = 1024;
		bool r = mb_op.WriteLuminousIntensityDetection(v);
		assert(r && v == mb_op.ReadLuminousIntensityDetection());
	}

	{
		uint32_t v = 1024;
		bool r = mb_op.WriteLiquidLevel(v);
		assert(r && v == mb_op.ReadLiquidLevel());
	}


	// Close the connection
	modbus_close(mb_ctx);
	modbus_free(mb_ctx);

	return 0;
}