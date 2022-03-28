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

		// Public Functions
		bool Begin(uint16_t _Design_Capacity, float _Resistor);

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

		// Config Functions
		bool Set_dQAcc(uint16_t _Capacity);
		bool Set_dPAcc(uint16_t _Capacity);
		bool Set_ModelCfg(uint8_t _Model_ID, bool _VChg);
		bool Set_HibCFG(uint16_t _Config);
		bool Set_Config(uint8_t _Channel, uint16_t _Config);

		String Serial_ID(void);

		// Sense Resistor Variable
		float _Sense_Resistor;

};

extern MAX17055 Battery;

#endif /* defined(__MAX17055__) */
