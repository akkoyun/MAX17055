#ifndef __MAX17055_DEFINATION__
#define __MAX17055_DEFINATION__

// Define Arduino Library
#ifndef __Arduino__
#include <Arduino.h>
#endif

// Define I2C Functions Library
#ifndef __I2C_Functions__
#include <I2C_Functions.h>
#endif

// Define Statistical Library
#ifndef __Statistical__
#include <Statistical.h>
#endif

// I2C Constants
#define I2C_Address				0x36

// IC Registers
#define Register_Status			0x00	//
#define Register_VAlrtTh		0x01
#define Register_TAlrtTh		0x02
#define Register_SAlrtTh		0x03
#define Register_AtRate			0x04
#define Register_RepCap			0x05	//
#define Register_RepSOC			0x06	//
#define Register_Age			0x07	//
#define Register_Temp			0x08	//
#define Register_VCell			0x09
#define Register_Current		0x0A
#define Register_AvgCurrent		0x0B
#define Register_QResidual		0x0C
#define Register_MixSOC			0x0D
#define Register_AvSOC			0x0E
#define Register_MixCap			0x0F
#define Register_FullCapRep		0x10	//
#define Register_TTE			0x11	//
#define Register_QRTable00		0x12
#define Register_FullSocThr		0x13
#define Register_RCell			0x14
#define Register_AvgTA			0x16
#define Register_Cycles			0x17
#define Register_DesignCap		0x18
#define Register_AvgVCell		0x19
#define Register_MaxMinTemp		0x1A
#define Register_MaxMinVolt		0x1B
#define Register_MaxMinCurr		0x1C
#define Register_Config			0x1D
#define Register_IChgTerm		0x1E	//
#define Register_AvCap			0x1F
#define Register_TTF			0x20	//
#define Register_DevName		0x21
#define Register_QRTable10		0x22
#define Register_FullCapNom		0x23
#define Register_AIN			0x27
#define Register_LearnCfg		0x28
#define Register_FilterCfg		0x29
#define Register_RelaxCfg		0x2A
#define Register_MiscCfg		0x2B
#define Register_TGain			0x2C
#define Register_TOff			0x2D
#define Register_CGain			0x2E
#define Register_COff			0x2F
#define Register_QRTable20		0x32
#define Register_DieTemp		0x34
#define Register_FullCap		0x35
#define Register_RComp0			0x38
#define Register_TempCo			0x39
#define Register_VEmpty			0x3A	//
#define Register_FStat			0x3D
#define Register_Timer			0x3E
#define Register_ShdnTimer		0x3F
#define Register_UserMem1		0x40
#define Register_QRTable30		0x42
#define Register_RGain			0x43
#define Register_dQAcc			0x45	//
#define Register_dPAcc			0x46
#define Register_ConvgCfg		0x49
#define Register_VFRemCap		0x4A
#define Register_QH				0x4D
#define Register_Status2		0xB0
#define Register_Power			0xB1
#define Register_UserMem2		0xB2
#define Register_AvgPower		0xB3
#define Register_IAlrtTh		0xB4
#define Register_TTFCfg			0xB5
#define Register_CVMixCap		0xB6
#define Register_CVHalfTime		0xB7
#define Register_CGTempCo		0xB8
#define Register_Curve			0xB9
#define Register_HibCfg			0xBA
#define Register_Config2		0xBB
#define Register_VRipple		0xBC
#define Register_RippleCfg		0xBD
#define Register_TimerH			0xBE
#define Register_UserMem3		0xD0
#define Register_ScOcvLim		0xD1
#define Register_SOCHold		0xD3
#define Register_MaxPeakPower	0xD4
#define Register_SusPeakPower	0xD5
#define Register_PackResistance	0xD6
#define Register_SysResistance	0xD7
#define Register_MinSysVoltage	0xD8
#define Register_MPPCurrent		0xD9
#define Register_SPPCurrent		0xDA
#define Register_ModelCfg		0xDB
#define Register_AtQResidual	0xDC
#define Register_AtTTE			0xDD
#define Register_AtAvSOC		0xDE
#define Register_AtAvCap		0xDF

enum STAT_Bits {
	POR			= 1,
	Imn			= 2,
	Bst			= 3,
	Imx			= 6,
	dSOCi		= 7,
	Vmn			= 8,
	Tmn			= 9,
	Smn			= 10,
	Bi			= 11,
	Vmx			= 12,
	Tmx			= 13,
	Smx			= 14,
	Br			= 15
};

enum CONFIG1_Bits { 	// [0x2210] - 0x0000
	Ber			= 0,	// [0] Enable alert on battery removal - 0
	Bei			= 1,	// [0] Enable alert on battery insertion - 0
	Aen			= 2,	// [0] Enable Alert on Fuel-Gauge Outputs - 0
	FTHRM		= 3,	// [0] Force Thermistor Bias Switch - 0
	ETHRM		= 4,	// [1] Enable Thermistor - 0
	COMMSH		= 6,	// [0] Communication Shutdown - 0
	SHDN		= 7,	// [0] Shutdown - 0
	Tex			= 8,	// [0] Temperature External - 0
	Ten			= 9,	// [1] Enable Temperature Channel - 0
	AINSH		= 10,	// [0] AIN Pin Shutdown - 0
	IS			= 11,	// [0] Current ALRT Sticky - 0
	VS			= 12,	// [0] Voltage ALRT Sticky - 0
	TS			= 13,	// [1] Temperature ALRT Sticky - 0
	SAS			= 14,	// [0] SOC ALRT Sticky - 0
	TSel		= 15	// [0] internal die temperature - 0
};

enum CONFIG2_Bits { 	// [0x3658] - 0x0218
	CPMode		= 1,	// [0] Constant Power Mode - 0
	LDMDL		= 5,	// [0] Load Model - 0
	TAlrtEn		= 6,	// [1] Temperature Alert Enable - 0
	dSOCen		= 7,	// [0] SOC Change Alert Enable - 0
	POWR0		= 8,	// [0] 0 
	POWR1		= 9,	// [1] 1
	POWR2		= 10,	// [1] 0
	POWR3		= 11,	// [0] 0
	DPEn		= 12,	// [1] Dynamic Power Enable - 0
	AtRateEn	= 13	// [1] AtRate Enable - 0
};

#endif