// self
#include "protocol.hpp"


#ifndef LOW_UINT32
#define LOW_UINT32(x) ((uint16_t)(x & 0xffff))
#endif

#ifndef HIGH_UINT32
#define HIGH_UINT32(x) ((uint16_t)((x >> 16) & 0xffff))
#endif

#ifndef COMBINE_UINT32
#define COMBINE_UINT32(low, high) (((uint32_t)high << 16) | ((uint32_t)low & 0xffff))
#endif


NewPlcModbusOpertion::NewPlcModbusOpertion(modbus_t *ctx)
	: m_mb_ctx(ctx)
{
}


NewPlcModbusOpertion::~NewPlcModbusOpertion()
{
	m_mb_ctx = nullptr;
}


bool NewPlcModbusOpertion::WriteLaneIndicator(LaneIndicatorRegisterValue v)
{
	if (nullptr == m_mb_ctx) {
		return false;
	}

	return 1 == modbus_write_register(
		m_mb_ctx,
		(int)NewPlcModbusAddress::LaneIndicator,
		(uint16_t)v
	);
}


LaneIndicatorRegisterValue NewPlcModbusOpertion::ReadLaneIndicator()
{
	if (nullptr == m_mb_ctx) {
		return LaneIndicatorRegisterValue::Invalid;
	}

	uint16_t data[] = { 0 };
	modbus_read_registers(
		m_mb_ctx,
		(int)NewPlcModbusAddress::LaneIndicator,
		NewPlcModbusReginsters[NewPlcModbusAddress::LaneIndicator],
		data
	);
	return (LaneIndicatorRegisterValue)data[0];
}


bool NewPlcModbusOpertion::WriteTrafficLights(TrafficLightRegisterValue v)
{
	if (nullptr == m_mb_ctx) {
		return false;
	}

	return 1 == modbus_write_register(
		m_mb_ctx,
		(int)NewPlcModbusAddress::TrafficLights,
		(uint16_t)v
	);
}


TrafficLightRegisterValue NewPlcModbusOpertion::ReadTrafficLights()
{
	if (nullptr == m_mb_ctx) {
		return TrafficLightRegisterValue::Invalid;
	}

	uint16_t data[] = { 0 };
	modbus_read_registers(
		m_mb_ctx,
		(int)NewPlcModbusAddress::TrafficLights,
		NewPlcModbusReginsters[NewPlcModbusAddress::TrafficLights],
		data
	);
	return (TrafficLightRegisterValue)data[0];
}


bool NewPlcModbusOpertion::WriteFans(FanRegisterValue v)
{
	if (nullptr == m_mb_ctx) {
		return false;
	}

	return 1 == modbus_write_register(
		m_mb_ctx,
		(int)NewPlcModbusAddress::Fans,
		(uint16_t)v
	);
}


FanRegisterValue NewPlcModbusOpertion::ReadFans()
{
	if (nullptr == m_mb_ctx) {
		return FanRegisterValue::Invalid;
	}

	uint16_t data[] = { 0 };
	modbus_read_registers(
		m_mb_ctx,
		(int)NewPlcModbusAddress::Fans,
		NewPlcModbusReginsters[NewPlcModbusAddress::Fans],
		data
	);
	return (FanRegisterValue)data[0];
}


bool NewPlcModbusOpertion::WriteRollerShutter(RollerShutterRegisterValue v)
{
	if (nullptr == m_mb_ctx) {
		return false;
	}

	return 1 == modbus_write_register(
		m_mb_ctx,
		(int)NewPlcModbusAddress::RollerShutter,
		(uint16_t)v
	);
}


RollerShutterRegisterValue NewPlcModbusOpertion::ReadRollerShutter()
{
	if (nullptr == m_mb_ctx) {
		return RollerShutterRegisterValue::Invalid;
	}

	uint16_t data[] = { 0 };
	modbus_read_registers(
		m_mb_ctx,
		(int)NewPlcModbusAddress::RollerShutter,
		NewPlcModbusReginsters[NewPlcModbusAddress::RollerShutter],
		data
	);
	return (RollerShutterRegisterValue)data[0];
}


bool NewPlcModbusOpertion::WriteLighting(LightingRegisterValue v)
{
	if (nullptr == m_mb_ctx) {
		return false;
	}

	return 1 == modbus_write_register(
		m_mb_ctx,
		(int)NewPlcModbusAddress::Lighting,
		(uint16_t)v
	);
}


LightingRegisterValue NewPlcModbusOpertion::ReadLighting()
{
	if (nullptr == m_mb_ctx) {
		return LightingRegisterValue::Invalid;
	}

	uint16_t data[] = { 0 };
	modbus_read_registers(
		m_mb_ctx,
		(int)NewPlcModbusAddress::Lighting,
		NewPlcModbusReginsters[NewPlcModbusAddress::Lighting],
		data
	);
	return (LightingRegisterValue)data[0];
}


bool NewPlcModbusOpertion::WriteWaterPump(WaterPumpRegisterValue v)
{
	if (nullptr == m_mb_ctx) {
		return false;
	}

	return 1 == modbus_write_register(
		m_mb_ctx,
		(int)NewPlcModbusAddress::WaterPump,
		(uint16_t)v
	);
}


WaterPumpRegisterValue NewPlcModbusOpertion::ReadWaterPump()
{
	if (nullptr == m_mb_ctx) {
		return WaterPumpRegisterValue::Invalid;
	}

	uint16_t data[] = { 0 };
	modbus_read_registers(
		m_mb_ctx,
		(int)NewPlcModbusAddress::WaterPump,
		NewPlcModbusReginsters[NewPlcModbusAddress::WaterPump],
		data
	);
	return (WaterPumpRegisterValue)data[0];
}


bool NewPlcModbusOpertion::WriteCoViNo(uint32_t co, uint32_t vi, uint32_t no)
{
	if (nullptr == m_mb_ctx) {
		return false;
	}

	const uint16_t data[] = {
		LOW_UINT32(co), HIGH_UINT32(co),
		LOW_UINT32(vi), HIGH_UINT32(vi),
		LOW_UINT32(no), HIGH_UINT32(no)
	};
	return NewPlcModbusReginsters[NewPlcModbusAddress::CoViNo] == modbus_write_registers(
		m_mb_ctx,
		(int)NewPlcModbusAddress::CoViNo,
		NewPlcModbusReginsters[NewPlcModbusAddress::CoViNo],
		data
	);
}


std::vector<uint32_t> NewPlcModbusOpertion::ReadCoViNo()
{
	if (nullptr == m_mb_ctx) {
		return {};
	}

	uint16_t data[] = { 0, 0, 0, 0, 0, 0 };
	modbus_read_registers(
		m_mb_ctx,
		(int)NewPlcModbusAddress::CoViNo,
		NewPlcModbusReginsters[NewPlcModbusAddress::CoViNo],
		data
	);
	return {
		COMBINE_UINT32(data[0], data[1]),
		COMBINE_UINT32(data[2], data[3]),
		COMBINE_UINT32(data[4], data[5]),
	};
}


bool NewPlcModbusOpertion::WriteWindSpeedDirection(uint32_t speed, WindDirectionRegisterValue direction)
{
	if (nullptr == m_mb_ctx) {
		return false;
	}

	const uint16_t data[] = { LOW_UINT32(speed), HIGH_UINT32(speed), (uint16_t)direction };
	return NewPlcModbusReginsters[NewPlcModbusAddress::WindSpeedDirection] = modbus_write_registers(
		m_mb_ctx,
		(int)NewPlcModbusAddress::WindSpeedDirection,
		NewPlcModbusReginsters[NewPlcModbusAddress::WindSpeedDirection],
		data
	);
}


std::pair<uint32_t, WindDirectionRegisterValue> NewPlcModbusOpertion::ReadWindSpeedDirection()
{
	if (nullptr == m_mb_ctx) {
		return std::pair<uint32_t, WindDirectionRegisterValue>();
	}

	uint16_t data[] = { 0, 0, 0 };
	modbus_read_registers(
		m_mb_ctx,
		(int)NewPlcModbusAddress::WindSpeedDirection,
		NewPlcModbusReginsters[NewPlcModbusAddress::WindSpeedDirection],
		data
	);
	return std::make_pair(
		COMBINE_UINT32(data[0], data[1]), (WindDirectionRegisterValue)data[2]
	);
}


bool NewPlcModbusOpertion::WriteIlluminanceDetection(uint32_t v)
{
	if (nullptr == m_mb_ctx) {
		return false;
	}

	const uint16_t data[] = { LOW_UINT32(v), HIGH_UINT32(v) };
	return IlluminanceDetectionRegisters[IlluminanceDetection::Illuminance] == modbus_write_registers(
		m_mb_ctx,
		(int)NewPlcModbusAddress::IlluminanceDetection,
		IlluminanceDetectionRegisters[IlluminanceDetection::Illuminance],
		data
	);
}


uint32_t NewPlcModbusOpertion::ReadIlluminanceDetection()
{
	if (nullptr == m_mb_ctx) {
		return 0;
	}

	uint16_t data[] = { 0, 0 };
	modbus_read_registers(
		m_mb_ctx,
		(int)NewPlcModbusAddress::IlluminanceDetection,
		IlluminanceDetectionRegisters[IlluminanceDetection::Illuminance],
		data
	);
	return COMBINE_UINT32(data[0], data[1]);
}


bool NewPlcModbusOpertion::WriteLuminousIntensityDetection(uint32_t v)
{
	if (nullptr == m_mb_ctx) {
		return false;
	}

	const uint16_t data[] = { LOW_UINT32(v), HIGH_UINT32(v) };
	return LuminousIntensityDetectionRegisters[LuminousIntensityDetectionBits::LuminousIntensity] == modbus_write_registers(
		m_mb_ctx,
		(int)NewPlcModbusAddress::LuminousIntensityDetection,
		LuminousIntensityDetectionRegisters[LuminousIntensityDetectionBits::LuminousIntensity],
		data
	);
}


uint32_t NewPlcModbusOpertion::ReadLuminousIntensityDetection()
{
	if (nullptr == m_mb_ctx) {
		return 0;
	}

	uint16_t data[] = { 0, 0 };
	modbus_read_registers(
		m_mb_ctx,
		(int)NewPlcModbusAddress::LuminousIntensityDetection,
		LuminousIntensityDetectionRegisters[LuminousIntensityDetectionBits::LuminousIntensity],
		data
	);
	return COMBINE_UINT32(data[0], data[1]);
}


bool NewPlcModbusOpertion::WriteLiquidLevel(uint32_t v)
{
	if (nullptr == m_mb_ctx) {
		return false;
	}

	const uint16_t data[] = { LOW_UINT32(v), HIGH_UINT32(v) };
	return LiquidLevelRegisters[LiquidLevelBits::LiquidLevel] == modbus_write_registers(
		m_mb_ctx,
		(int)NewPlcModbusAddress::LiquidLevel,
		LiquidLevelRegisters[LiquidLevelBits::LiquidLevel],
		data
	);
}


uint32_t NewPlcModbusOpertion::ReadLiquidLevel()
{
	if (nullptr == m_mb_ctx) {
		return 0;
	}

	uint16_t data[] = { 0, 0 };
	modbus_read_registers(
		m_mb_ctx,
		(int)NewPlcModbusAddress::LiquidLevel,
		LiquidLevelRegisters[LiquidLevelBits::LiquidLevel],
		data
	);
	return COMBINE_UINT32(data[0], data[1]);
}

