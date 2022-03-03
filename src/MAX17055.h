/* *******************************************************************************
 *  Copyright (C) 2014-2019 Mehmet Gunce Akkoyun Can not be copied and/or
 *	distributed without the express permission of Mehmet Gunce Akkoyun
 *
 *	Library				: Battery Management Library
 *	Code Developer		: Mehmet Gunce Akkoyun (akkoyun@me.com)
 *********************************************************************************/

#ifndef __MAX17055__
#define __MAX17055__

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

class MAX17055 {

	public:

		// Function Variables
		char MAX17055_Version[9] = "02.01.01";
		
		// Public Functions
		bool Begin(uint16_t _Design_Capacity, float _Resistor);
		String Serial_ID(void);

		// Config Functions
		bool Set_dQAcc(uint16_t _Capacity);
		bool Set_dPAcc(uint16_t _Capacity);
		bool Set_ModelCfg(uint8_t _Model_ID, bool _VChg);
		bool Set_HibCFG(uint16_t _Config);
		bool Set_Config(uint8_t _Channel, uint16_t _Config);

		// Voltage Functions
		float Instant_Voltage(void);
		float Average_Voltage(void);
		float Empty_Voltage(void);
		float Recovery_Voltage(void);
		bool Set_Empty_Recovery_Voltage(float _Empty_Voltage, float _Recovery_Voltage);
		bool Set_Max_Min_Voltage(float _Max_Voltage, float _Min_Voltage);

		// Current Functions
		float Instant_Current(void);
		float Average_Current(void);
		float Charge_Termination_Current(void);
		bool Set_Charge_Termination_Current(void);

		// State of Charge Functions
		float State_Of_Charge(void);
		float Average_State_Of_Charge(void);

		// Capacity Functions
		uint16_t Instant_Capacity(void);
		uint16_t Design_Capacity(void);
		uint16_t Full_Capacity(void);
		bool Set_Design_Capacity(uint16_t _Capacity);

		// Temperature Functions
		float Temperature(void);

		// Time Functions
		uint16_t Time_To_Empty(void);
		uint16_t Time_To_Full(void);

		// Battery Properties Functions
		uint16_t Battery_Age(void);
		uint16_t Charge_Cycle(void);

		// Control Functions
		bool is_Power_on_Reset(void);
		bool is_Min_Current(void);
		bool is_Max_Current(void);
		bool is_Min_Voltage(void);
		bool is_Max_Voltage(void);
		bool is_Min_Temperature(void);
		bool is_Max_Temperature(void);
		bool is_Min_SOC(void);
		bool is_Max_SOC(void);
		bool is_Battery_Present(void);
		bool is_SOC_Change(void);
		bool is_Battery_Insertion(void);
		bool is_Battery_Removal(void);

	private:

		// MAX17055 I2C Address
		uint8_t _I2C_Address = 0x36;

		// Sense Resistor Variable
		float _Sense_Resistor;

		// I2C Register Definitions
		enum MAX17055_Registers {
			Register_Status			= (uint8_t)0x00,
			Register_VAlrtTh		= (uint8_t)0x01,
			Register_TAlrtTh		= (uint8_t)0x02,
			Register_SAlrtTh		= (uint8_t)0x03,
			Register_AtRate			= (uint8_t)0x04,
			Register_RepCap			= (uint8_t)0x05,
			Register_RepSOC			= (uint8_t)0x06,
			Register_Age			= (uint8_t)0x07,
			Register_Temp			= (uint8_t)0x08,
			Register_VCell			= (uint8_t)0x09,
			Register_Current		= (uint8_t)0x0A,
			Register_AvgCurrent		= (uint8_t)0x0B,
			Register_QResidual		= (uint8_t)0x0C,
			Register_MixSOC			= (uint8_t)0x0D,
			Register_AvSOC			= (uint8_t)0x0E,
			Register_MixCap			= (uint8_t)0x0F,
			Register_FullCapRep		= (uint8_t)0x10,
			Register_TTE			= (uint8_t)0x11,
			Register_QRTable00		= (uint8_t)0x12,
			Register_FullSocThr		= (uint8_t)0x13,
			Register_RCell			= (uint8_t)0x14,
			Register_AvgTA			= (uint8_t)0x16,
			Register_Cycles			= (uint8_t)0x17,
			Register_DesignCap		= (uint8_t)0x18,
			Register_AvgVCell		= (uint8_t)0x19,
			Register_MaxMinTemp		= (uint8_t)0x1A,
			Register_MaxMinVolt		= (uint8_t)0x1B,
			Register_MaxMinCurr		= (uint8_t)0x1C,
			Register_Config			= (uint8_t)0x1D,
			Register_IChgTerm		= (uint8_t)0x1E,
			Register_AvCap			= (uint8_t)0x1F,
			Register_TTF			= (uint8_t)0x20,
			Register_DevName		= (uint8_t)0x21,
			Register_QRTable10		= (uint8_t)0x22,
			Register_FullCapNom		= (uint8_t)0x23,
			Register_AIN			= (uint8_t)0x27,
			Register_LearnCfg		= (uint8_t)0x28,
			Register_FilterCfg		= (uint8_t)0x29,
			Register_RelaxCfg		= (uint8_t)0x2A,
			Register_MiscCfg		= (uint8_t)0x2B,
			Register_TGain			= (uint8_t)0x2C,
			Register_TOff			= (uint8_t)0x2D,
			Register_CGain			= (uint8_t)0x2E,
			Register_COff			= (uint8_t)0x2F,
			Register_QRTable20		= (uint8_t)0x32,
			Register_DieTemp		= (uint8_t)0x34,
			Register_FullCap		= (uint8_t)0x35,
			Register_RComp0			= (uint8_t)0x38,
			Register_TempCo			= (uint8_t)0x39,
			Register_VEmpty			= (uint8_t)0x3A,
			Register_FStat			= (uint8_t)0x3D,
			Register_Timer			= (uint8_t)0x3E,
			Register_ShdnTimer		= (uint8_t)0x3F,
			Register_UserMem1		= (uint8_t)0x40,
			Register_QRTable30		= (uint8_t)0x42,
			Register_RGain			= (uint8_t)0x43,
			Register_dQAcc			= (uint8_t)0x45,
			Register_dPAcc			= (uint8_t)0x46,
			Register_ConvgCfg		= (uint8_t)0x49,
			Register_VFRemCap		= (uint8_t)0x4A,
			Register_QH				= (uint8_t)0x4D,
			Register_Status2		= (uint8_t)0xB0,
			Register_Power			= (uint8_t)0xB1,
			Register_UserMem2		= (uint8_t)0xB2,
			Register_AvgPower		= (uint8_t)0xB3,
			Register_IAlrtTh		= (uint8_t)0xB4,
			Register_TTFCfg			= (uint8_t)0xB5,
			Register_CVMixCap		= (uint8_t)0xB6,
			Register_CVHalfTime		= (uint8_t)0xB7,
			Register_CGTempCo		= (uint8_t)0xB8,
			Register_Curve			= (uint8_t)0xB9,
			Register_HibCfg			= (uint8_t)0xBA,
			Register_Config2		= (uint8_t)0xBB,
			Register_VRipple		= (uint8_t)0xBC,
			Register_RippleCfg		= (uint8_t)0xBD,
			Register_TimerH			= (uint8_t)0xBE,
			Register_UserMem3		= (uint8_t)0xD0,
			Register_ScOcvLim		= (uint8_t)0xD1,
			Register_SOCHold		= (uint8_t)0xD3,
			Register_MaxPeakPower	= (uint8_t)0xD4,
			Register_SusPeakPower	= (uint8_t)0xD5,
			Register_PackResistance	= (uint8_t)0xD6,
			Register_SysResistance	= (uint8_t)0xD7,
			Register_MinSysVoltage	= (uint8_t)0xD8,
			Register_MPPCurrent		= (uint8_t)0xD9,
			Register_SPPCurrent		= (uint8_t)0xDA,
			Register_ModelCfg		= (uint8_t)0xDB,
			Register_AtQResidual	= (uint8_t)0xDC,
			Register_AtTTE			= (uint8_t)0xDD,
			Register_AtAvSOC		= (uint8_t)0xDE,
			Register_AtAvCap		= (uint8_t)0xDF
		};

		// Status Register Bit Definitions
		enum STATUS_Register_Bits {
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

		// Config 1 register Bits
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

		// Config 2 register Bits
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

};

extern MAX17055 Battery;

#endif /* defined(__MAX17055__) */
