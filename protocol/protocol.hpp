#pragma once

// c
#include <stdint.h>

// c++
#include <map>
#include <vector>

// modbus
#include <modbus.h>


/*
* 2024-08-27
*
* 1. 地址分段
* 地址范围	内容
* 0x0000~0x6FFF	bit操作
* 0x7000~0xDFFF	寄存器操作
* 0xE000~0xFFFF	特殊操作
*
*
* 2. 控制器协议
* 控制器设备可分为逻辑类设备和模拟量类设备两种：
* 逻辑类设备占1个寄存器(2bytes)
* 模拟量类设备占6个寄存器(12bytes)；
* 设备类型	地址范围
* 逻辑类设备	0x7000 - 0x7FFF
* 模拟量类设备	0x8000 - 0xDFFF
*
* 逻辑类设备反馈与控制寄存器地址码相同，反馈bit分配如下
* Bit	含义
* Bit 15	手自动状态
* Bit 14	故障状态(设备本身)
* Bit 13	故障状态(逻辑判断)
* Bit 12 - 0	工况反馈
*

*
* 9. CO / VI(模拟类)
* 输入寄存器1 - 2
* 寄存器bit	含义
* 0 - 32 bit	CO
* 输入寄存器3 - 4
* 寄存器bit	含义
* 0 - 32 bit	VI
* 输入寄存器5 - 6
* 寄存器bit	含义
* 0 - 32 bit	NO
*
*
* 10. 风速风向(模拟类 + 逻辑类)
* 输入寄存器1 - 2
* 寄存器bit	含义
* 0 - 32 bit	风速
* 输入寄存器3
* 寄存器值	含义
* 0x0001	风向：正
* 0x0001	风向：反
*
*
* 11. 照度检测(模拟类)
* 输入寄存器1 - 2
* 寄存器bit	含义
* 0 - 32 bit	照度
*
*
* 12. 光强检测(模拟类)
* 输入寄存器1 - 2
* 寄存器bit	含义
* 0 - 32 bit	光强
*
*
* 13. 液位(模拟类)
* 输入寄存器1 - 2
* 寄存器bit	含义
* 0 - 32 bit	液位
*/



// 寄存器地址
enum class NewPlcModbusAddress {
	LaneIndicator = 0x7000,  // 车道指示器(逻辑类)
	TrafficLights,  // 信号灯(逻辑类)
	Fans,  // 风机(逻辑类)
	RollerShutter,  // 卷帘门(逻辑类)
	Lighting,  // 照明(逻辑类)
	WaterPump,  // 水泵(逻辑类)
	CoViNo = 0x7006,  // CO / VI / NO(模拟类) * 6个输入寄存器
	WindSpeedDirection = 0x700c,  // 风速风向(模拟类 + 逻辑类) * 3个输入寄存器
	IlluminanceDetection = 0x700f,  // 照度检测(模拟类) * 2个输入寄存器
	LuminousIntensityDetection = 0x70a1,  // 光强检测(模拟类) * 2个输入寄存器
	LiquidLevel = 0x70a3,  // 液位(模拟类) * 2个输入寄存器
};


// 寄存器个数
static std::map<NewPlcModbusAddress, int> NewPlcModbusReginsters{
	{ NewPlcModbusAddress::LaneIndicator, 1 },
	{ NewPlcModbusAddress::TrafficLights, 1 },
	{ NewPlcModbusAddress::Fans, 1 },
	{ NewPlcModbusAddress::RollerShutter, 1 },
	{ NewPlcModbusAddress::Lighting, 1 },
	{ NewPlcModbusAddress::WaterPump, 1 },
	{ NewPlcModbusAddress::CoViNo, 6 },
	{ NewPlcModbusAddress::WindSpeedDirection, 3 },
	{ NewPlcModbusAddress::IlluminanceDetection, 2 },
	{ NewPlcModbusAddress::LuminousIntensityDetection, 2 },
	{ NewPlcModbusAddress::LiquidLevel, 2 },
};


// 车道指示器(逻辑类)
enum class LaneIndicatorRegisterValue {
	Invalid,
	ForwardTrafficTurnProhibited,  // 正向通车,禁止转向
	ReverseTrafficTurnProhibited,	// 反向通车, 禁止转向
	ForwardTrafficTurnAllowed,	// 正向通车, 允许转向
	ReverseTrafficTurnAllowed,	// 反向通车, 允许转向
	ForwardTurnOnly,  // 仅允许正向转向
	ReverseTurnOnly,  // 仅允许反向转向
	TrafficProhibited,  // 禁止通行
};


// 信号灯(逻辑类)
enum class TrafficLightRegisterValue {
	Invalid,
	RedLightTurnProhibited,  // 红灯,禁止转向
	YellowLightTurnProhibited,// 黄灯,禁止转向
	GreenLightTurnProhibited, // 绿灯,禁止转向
	RedLightTurnAllowed,  // 红灯,允许转向
	YellowLightTurnAllowed,  // 黄灯,允许转向
	GreenLightTurnAllowed,  // 绿灯,允许转向
	TurnAllowed,  // 允许转向
};


// 风机(逻辑类)
enum class FanRegisterValue {
	Invalid,
	Forward,  // 正转
	Reverse,  // 反转
	Stop,  // 停止
};


// 卷帘门(逻辑类)
enum class RollerShutterRegisterValue {
	Invalid,
	Up,  // 上升
	Down,  // 下降
	Stop,  // 停止
	Reset,  // 复位
};


// 照明(逻辑类)
enum class LightingRegisterValue {
	Invalid,
	On,  // 开
	Off,  // 关
};


// 水泵(逻辑类)
enum class WaterPumpRegisterValue {
	Invalid,
	Start,  // 启动
	Stop,  // 停止
};


// CO / VI / NO(模拟类)
enum class CoViNoRegisterBits {
	Invalid,
	Co,
	Vi,
	No,
};


// CO / VI / NO 寄存器个数
static std::map<CoViNoRegisterBits, int> CoViNoRegisters{
	{ CoViNoRegisterBits::Co, 2 },
	{ CoViNoRegisterBits::Vi, 2 },
	{ CoViNoRegisterBits::No, 2 },
};


// 风速风向(模拟类 + 逻辑类)
enum class WindDirectionRegisterValue {
	Invalid,
	Forward,  // 正转
	Reverse,  // 反转
};


// 风速风向(模拟类 + 逻辑类)
enum class WindSpeedDirectionBits {
	Speed,
	Direction,
};


// 风速风向(模拟类 + 逻辑类) 寄存器个数
static std::map<WindSpeedDirectionBits, int> WindSpeedDirectionRegisters{
	{ WindSpeedDirectionBits::Speed, 2 },
	{ WindSpeedDirectionBits::Direction, 1 },
};


// 照度检测(模拟类)
enum class IlluminanceDetection {
	Illuminance,
};


// 照度检测 寄存器个数
static std::map<IlluminanceDetection, int> IlluminanceDetectionRegisters{
	{ IlluminanceDetection::Illuminance, 2 },
};


// 光强检测(模拟类)
enum class LuminousIntensityDetectionBits {
	LuminousIntensity,
};


// 光强检测 寄存器个数
static std::map<LuminousIntensityDetectionBits, int> LuminousIntensityDetectionRegisters{
	{ LuminousIntensityDetectionBits::LuminousIntensity, 2 },
};


// 液位(模拟类)
enum class LiquidLevelBits {
	LiquidLevel,
};


// 液位 寄存器个数
static std::map<LiquidLevelBits, int> LiquidLevelRegisters{
	{ LiquidLevelBits::LiquidLevel, 2 },
};


class NewPlcModbusOpertion {
public:
	NewPlcModbusOpertion(modbus_t *ctx);
	~NewPlcModbusOpertion();

	// 车道指示器(逻辑类)
	bool WriteLaneIndicator(LaneIndicatorRegisterValue v);
	LaneIndicatorRegisterValue ReadLaneIndicator();

	// 信号灯(逻辑类)
	bool WriteTrafficLights(TrafficLightRegisterValue v);
	TrafficLightRegisterValue ReadTrafficLights();

	// 风机(逻辑类)
	bool WriteFans(FanRegisterValue v);
	FanRegisterValue ReadFans();

	// 卷帘门(逻辑类)
	bool WriteRollerShutter(RollerShutterRegisterValue v);
	RollerShutterRegisterValue ReadRollerShutter();

	// 照明(逻辑类)
	bool WriteLighting(LightingRegisterValue v);
	LightingRegisterValue ReadLighting();

	// 水泵(逻辑类)
	bool WriteWaterPump(WaterPumpRegisterValue v);
	WaterPumpRegisterValue ReadWaterPump();

	// CO / VI / NO(模拟类) * 6个输入寄存器
	bool WriteCoViNo(uint32_t co, uint32_t vi, uint32_t no);
	std::vector<uint32_t> ReadCoViNo();

	// 风速风向(模拟类 + 逻辑类) * 3个输入寄存器
	bool WriteWindSpeedDirection(uint32_t speed, WindDirectionRegisterValue direction);
	std::pair<uint32_t, WindDirectionRegisterValue> ReadWindSpeedDirection();

	// 照度检测(模拟类) * 2个输入寄存器
	bool WriteIlluminanceDetection(uint32_t v);
	uint32_t ReadIlluminanceDetection();

	// 光强检测(模拟类) * 2个输入寄存器
	bool WriteLuminousIntensityDetection(uint32_t v);
	uint32_t ReadLuminousIntensityDetection();

	// 液位(模拟类) * 2个输入寄存器
	bool WriteLiquidLevel(uint32_t v);
	uint32_t ReadLiquidLevel();


private:
	modbus_t *m_mb_ctx;
};
