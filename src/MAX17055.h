/* *******************************************************************************
 *  Copyright (C) 2014-2019 Mehmet Gunce Akkoyun Can not be copied and/or
 *	distributed without the express permission of Mehmet Gunce Akkoyun
 *
 *	Library				: Battery Management Library
 *	Code Developer		: Mehmet Gunce Akkoyun (akkoyun@me.com)
 *********************************************************************************/

#ifndef __MAX17055__
#define __MAX17055__

	// Include Arduino Library
	#ifndef Arduino_h
		#include <Arduino.h>
	#endif

	// Include I2C Functions Library
	#ifndef __I2C_Functions__
		#include <I2C_Functions.h>
	#endif

	// Load Configuration
	#include "Config.h"
	#include "Definitions.h"

	// Define MAX17055 Class
	class MAX17055 : private I2C_Functions {

		// Private Context
		private:

			// Set battery empty voltage function.
			bool Set_Empty_Voltage(float _Empty_Voltage) {

				// Set Voltage Value
				uint16_t _Raw_Voltage = ((uint16_t)((uint16_t)_Empty_Voltage * 100) << 7 ) & 0b1111111110000000;

				// Define Data Variable
				uint8_t MAX17055_RAW_Data[2];

				// Handle Data
				MAX17055_RAW_Data[1] = (uint8_t)(_Raw_Voltage >> 8);
				MAX17055_RAW_Data[0] = (uint8_t)(_Raw_Voltage & 0x00FF);

				// Define Data Variable
				uint8_t MAX17055_Current_Data[2];

				// Read Current Register
				I2C_Functions::Read_Multiple_Register(0x3A, MAX17055_Current_Data, 2, true);

				// Clear Current Value
				MAX17055_Current_Data[0] &= 0b01111111;
				MAX17055_Current_Data[1] &= 0b00000000;

				// Define Data Variable
				uint8_t MAX17055_Handle_Data[2];

				// Handle Data
				MAX17055_Handle_Data[0] = MAX17055_Current_Data[0] | MAX17055_RAW_Data[0];
				MAX17055_Handle_Data[1] = MAX17055_Current_Data[1] | MAX17055_RAW_Data[1];

				// Set Register
				bool _Result = I2C_Functions::Write_Multiple_Register(0x3A, MAX17055_Handle_Data, 2);

				// End Function
				return(_Result);

			}

			// Set battery recovery voltage function.
			bool Set_Recovery_Voltage(float _Recovery_Voltage) {

				// Handle Value
				_Recovery_Voltage = _Recovery_Voltage * 1000 / 40;

				// Set Voltage Value
				uint16_t _Raw_Voltage = ((uint16_t)_Recovery_Voltage);

				// Define Data Variable
				uint8_t MAX17055_RAW_Data[2];

				// Handle Data
				MAX17055_RAW_Data[1] = (uint8_t)(_Raw_Voltage >> 8);
				MAX17055_RAW_Data[0] = (uint8_t)(_Raw_Voltage & 0x00FF);

				// Define Data Variable
				uint8_t MAX17055_Current_Data[2];

				// Read Current Register
				I2C_Functions::Read_Multiple_Register(0x3A, MAX17055_Current_Data, 2, true);

				// Clear Current Value
				MAX17055_Current_Data[0] &= 0b10000000;
				MAX17055_Current_Data[1] &= 0b11111111;

				// Define Data Variable
				uint8_t MAX17055_Handle_Data[2];

				// Handle Data
				MAX17055_Handle_Data[0] = MAX17055_Current_Data[0] | MAX17055_RAW_Data[0];
				MAX17055_Handle_Data[1] = MAX17055_Current_Data[1] | MAX17055_RAW_Data[1];

				// Set Register
				bool _Result = I2C_Functions::Write_Multiple_Register(0x3A, MAX17055_Handle_Data, 2);

				// End Function
				return(_Result);

			}

			// Set battery minimum voltage function.
			bool Set_Min_Voltage(float _Minimum_Voltage) {

				// Set Voltage Value
				uint8_t _Raw_Voltage = (uint8_t)(_Minimum_Voltage * 1000 / 20);

				// Define Data Variable
				uint8_t MAX17055_Current_Data[2];

				// Read Current Register
				I2C_Functions::Read_Multiple_Register(0x01, MAX17055_Current_Data, 2, true);

				// Set Voltage Value
				MAX17055_Current_Data[0] = _Raw_Voltage;

				// Set Register
				bool _Result = I2C_Functions::Write_Multiple_Register(0x01, MAX17055_Current_Data, 2);

				// End Function
				return(_Result);

			}

			// Set battery maximum voltage function.
			bool Set_Max_Voltage(float _Maximum_Voltage) {

				// Set Voltage Value
				uint8_t _Raw_Voltage = (uint8_t)(_Maximum_Voltage * 1000 / 20);

				// Define Data Variable
				uint8_t MAX17055_Current_Data[2];

				// Read Current Register
				I2C_Functions::Read_Multiple_Register(0x01, MAX17055_Current_Data, 2, true);

				// Set Voltage Value
				MAX17055_Current_Data[1] = _Raw_Voltage;

				// Set Register
				bool _Result = I2C_Functions::Write_Multiple_Register(0x01, MAX17055_Current_Data, 2);

				// End Function
				return(_Result);

			}

			// Set battery maximum current function.
			bool Set_Max_Current(float _Maximum_Current) {

				// Set Current Value
				uint8_t _Raw_Current = (uint8_t)(_Maximum_Current * 1000 / 40);

				// Define Data Variable
				uint8_t MAX17055_Current_Data[2];

				// Read Current Register
				I2C_Functions::Read_Multiple_Register(0xB4, MAX17055_Current_Data, 2, true);

				// Set Current Value
				MAX17055_Current_Data[1] = _Raw_Current;

				// Set Register
				bool _Result = I2C_Functions::Write_Multiple_Register(0xB4, MAX17055_Current_Data, 2);

				// End Function
				return(_Result);

			}

			// Set battery charge termination current function.
			bool Set_Charge_Termination_Current(float _Charge_Termination_Current) {

				// Handle Raw Data
				uint16_t _RAW_Data = (uint16_t)(_Charge_Termination_Current * 1000000 * __MAX17055_Resistor__ / 1.5625);

				// Declare Default Data Array
				uint8_t _Data[2];

				// Set Data Low/High Byte
				_Data[0] = ((_RAW_Data & (uint16_t)0x00FF));
				_Data[1] = ((_RAW_Data & (uint16_t)0xFF00) >> 8);

				// Set Register
				bool _Result = I2C_Functions::Write_Multiple_Register(0x1E, _Data, 2);

				// End Function
				return(_Result);

			}

			// Set battery design capacity function.
			bool Set_Design_Capacity(const uint16_t _Capacity) {

				// Set Raw
				uint16_t _Raw_Cap = (uint16_t)_Capacity * 2;

				// Declare Default Data Array
				uint8_t _Data[2];

				// Set Data Low/High Byte
				_Data[0] = ((_Raw_Cap & (uint16_t)0x00FF));
				_Data[1] = ((_Raw_Cap & (uint16_t)0xFF00) >> 8);

				// Set Register
				bool _Result = I2C_Functions::Write_Multiple_Register(0x18, _Data, 2);

				// End Function
				return(_Result);

			}

			// Set battery minimum SOC function.
			bool Set_Min_SOC(uint8_t _Minimum_SOC) {

				// Define Data Variable
				uint8_t MAX17055_Current_Data[2];

				// Read Current Register
				I2C_Functions::Read_Multiple_Register(0x03, MAX17055_Current_Data, 2, true);

				// Scale Value
				uint8_t _MinSOC = (_Minimum_SOC / 100) * 255;

				// Set Voltage Value
				MAX17055_Current_Data[0] = _MinSOC;

				// Set Register
				bool _Result = I2C_Functions::Write_Multiple_Register(0x03, MAX17055_Current_Data, 2);

				// End Function
				return(_Result);

			}

			// Set battery maximum SOC function.
			bool Set_Max_SOC(uint8_t _Maximum_SOC) {

				// Define Data Variable
				uint8_t MAX17055_Current_Data[2];

				// Read Current Register
				I2C_Functions::Read_Multiple_Register(0x03, MAX17055_Current_Data, 2, true);

				// Scale Value
				uint8_t _MaxSOC = (_Maximum_SOC / 100) * 255;

				// Set Voltage Value
				MAX17055_Current_Data[1] = _MaxSOC;

				// Set Register
				bool _Result = I2C_Functions::Write_Multiple_Register(0x03, MAX17055_Current_Data, 2);

				// End Function
				return(_Result);

			}

			// Set battery minimum temperature function.
			bool Set_Min_Temperature(uint8_t _Minimum_Temperature) {

				// Define Data Variable
				uint8_t MAX17055_Current_Data[2];

				// Read Current Register
				I2C_Functions::Read_Multiple_Register(0x02, MAX17055_Current_Data, 2, true);

				// Set Voltage Value
				MAX17055_Current_Data[0] = _Minimum_Temperature;

				// Set Register
				bool _Result = I2C_Functions::Write_Multiple_Register(0x02, MAX17055_Current_Data, 2);

				// End Function
				return(_Result);

			}

			// Set battery maximum temperature function.
			bool Set_Max_Temperature(uint8_t _Maximum_Temperature) {

				// Define Data Variable
				uint8_t MAX17055_Current_Data[2];

				// Read Current Register
				I2C_Functions::Read_Multiple_Register(0x02, MAX17055_Current_Data, 2, true);

				// Set Voltage Value
				MAX17055_Current_Data[1] = _Maximum_Temperature;

				// Set Register
				bool _Result = I2C_Functions::Write_Multiple_Register(0x02, MAX17055_Current_Data, 2);

				// End Function
				return(_Result);

			}

			// Set battery model function.
			bool Set_ModelCfg(const uint8_t _Model_ID) {

				// Declare Variable
				uint16_t _Data_Set = 0x00;

				// Set Charge Voltage
				bitSet(_Data_Set, 10);

				// Set Battery Model
				if (_Model_ID == 0) {
					bitClear(_Data_Set, 4);
					bitClear(_Data_Set, 5);
					bitClear(_Data_Set, 6);
					bitClear(_Data_Set, 7);
				}
				if (_Model_ID == 2) {
					bitClear(_Data_Set, 4);
					bitSet(_Data_Set, 5);
					bitClear(_Data_Set, 6);
					bitClear(_Data_Set, 7);
				}
				if (_Model_ID == 6) {
					bitClear(_Data_Set, 4);
					bitSet(_Data_Set, 5);
					bitSet(_Data_Set, 6);
					bitClear(_Data_Set, 7);
				}

				// Declare Default Data Array
				uint8_t _Data[2];

				// Set Data Low/High Byte
				_Data[0] = ((_Data_Set & (uint16_t)0x00FF));
				_Data[1] = ((_Data_Set & (uint16_t)0xFF00) >> 8);

				// Set Register
				bool _Result = I2C_Functions::Write_Multiple_Register(0xDB, _Data, 2);

				// End Function
				return(_Result);

			}

			// Set HibCFG function.
			bool Set_HibCFG(const uint16_t _Config) {

				// Declare Default Data Array
				uint8_t _Data[2];

				// Set Data Low/High Byte
				_Data[0] = ((_Config & (uint16_t)0x00FF));
				_Data[1] = ((_Config & (uint16_t)0xFF00) >> 8);

				// Set Register
				bool _Result = I2C_Functions::Write_Multiple_Register(0xBA, _Data, 2);

				// End Function
				return(_Result);

			}

			// Set Config function.
			void Config(void) {

				// Declare Default Data Array
				uint8_t _Config1[2];
				uint8_t _Config2[2];

				// Set Default Data
				_Config1[0] = 0b00000000;
				_Config1[1] = 0b00000000;
				_Config2[0] = 0b00011000;
				_Config2[1] = 0b00000110;

				// Set Configuration bits [Config1]
				if (MAX17055_Ber) bitSet(_Config1[0], 0);
				if (MAX17055_Bei) bitSet(_Config1[0], 1);
				if (MAX17055_Aen) bitSet(_Config1[0], 2);
				if (MAX17055_FTHRM) bitSet(_Config1[0], 3);
				if (MAX17055_ETHRM) bitSet(_Config1[0], 4);
				if (MAX17055_COMMSH) bitSet(_Config1[0], 6);
				if (MAX17055_SHDN) bitSet(_Config1[0], 7);
				if (MAX17055_Tex) bitSet(_Config1[1], 0);
				if (MAX17055_Ten) bitSet(_Config1[1], 1);
				if (MAX17055_AINSH) bitSet(_Config1[1], 2);
				if (MAX17055_IS) bitSet(_Config1[1], 3);
				if (MAX17055_VS) bitSet(_Config1[1], 4);
				if (MAX17055_TS) bitSet(_Config1[1], 5);
				if (MAX17055_SS) bitSet(_Config1[1], 6);
				if (MAX17055_TSel) bitSet(_Config1[1], 7);
				
				// Set Configuration bits [Config2]
				if (MAX17055_CPMode) bitSet(_Config2[0], 1);
				if (MAX17055_LDMDL) bitSet(_Config2[0], 5);
				if (MAX17055_TAIrtEN) bitSet(_Config2[0], 6);
				if (MAX17055_dSOCen) bitSet(_Config2[0], 7);
				if (MAX17055_DPEn) bitSet(_Config2[1], 4);
				if (MAX17055_AtRateEn) bitSet(_Config2[1], 5);

				// Config1 Setting
				I2C_Functions::Write_Multiple_Register(0x1D, _Config1, 2);
				I2C_Functions::Write_Multiple_Register(0xBB, _Config2, 2);

			}

		// Public Context
		public:

			// Library Constructor
			explicit MAX17055(const bool _Multiplexer_Enable = false, const uint8_t _Multiplexer_Channel = 0) : I2C_Functions(__I2C_Addr_MAX17055__, _Multiplexer_Enable, _Multiplexer_Channel) {
				
			}

			// Begin Function
			bool Begin(void) {

				// Start I2C
				I2C_Functions::Begin();

				// Control for Device
				if (I2C_Functions::Detect()) {

					// Set Configuration
					this->Config();
					this->Set_HibCFG(0x0000);

					// Design Capacity Register
					this->Set_Design_Capacity(__MAX17055_Design_Capacity__);

					// ModelCfg Register
					this->Set_ModelCfg(__MAX17055_Battery_Model__);

					// IChgTerm Register
					this->Set_Charge_Termination_Current(__MAX17055_Termination_Current__);

					// VEmpty Register
					this->Set_Empty_Voltage(__MAX17055_Empty_Voltage__);

					// VRecovery Register
					this->Set_Recovery_Voltage(__MAX17055_Recovery_Voltage__);

					// Set Minimum Voltage
					this->Set_Min_Voltage(__MAX17055_Min_Voltage__);

					// Set Maximum Voltage
					this->Set_Max_Voltage(__MAX17055_Max_Voltage__);

					// Set Maximum Current
					this->Set_Max_Current(__MAX17055_Max_Current__);

					// Set Minimum SOC
					this->Set_Min_SOC(__MAX17055_Min_SOC__);

					// Set Maximum SOC
					this->Set_Max_SOC(__MAX17055_Max_SOC__);

					// Set Minimum Temperature
					this->Set_Min_Temperature(__MAX17055_Min_Temperature__);

					// Set Maximum Temperature
					this->Set_Max_Temperature(__MAX17055_Max_Temperature__);

					// Clear Bits
					this->Status_Clear();

					// I2C Delay
					delay(5);
										
					// End Function
					return(true);

				} else {

					// End Function
					return(false);

				}

			}

			// Get battery instant voltage function.
			float Instant_Voltage(void) {

				// Define Data Variable
				uint8_t _MAX17055_Data[2];

				// Get Data from IC
				I2C_Functions::Read_Multiple_Register(0x09, _MAX17055_Data, 2, false);

				// Combine Read Bytes
				uint16_t _Measurement_Raw = ((uint16_t)_MAX17055_Data[1] << 8) | (uint16_t)_MAX17055_Data[0];

				// Calculate Measurement
				float _Value = ((double)_Measurement_Raw * 1.25 / 16) / 1000;

				// End Function
				return(_Value);

			}

			// Get battery average voltage function.
			float Average_Voltage(void) {

				// Define Data Variable
				uint8_t _MAX17055_Data[2];

				// Get Data from IC
				I2C_Functions::Read_Multiple_Register(0x19, _MAX17055_Data, 2, false);

				// Combine Read Bytes
				uint16_t _Measurement_Raw = ((uint16_t)_MAX17055_Data[1] << 8) | (uint16_t)_MAX17055_Data[0];

				// Calculate Measurement
				float _Value = ((double)_Measurement_Raw * 1.25 / 16) / 1000;

				// End Function
				return(_Value);

			}

			// Get battery empty voltage function.
			float Empty_Voltage(void) {

				// Define Data Variable
				uint8_t _MAX17055_Data[2];

				// Get Data from IC
				I2C_Functions::Read_Multiple_Register(0x3A, _MAX17055_Data, 2, false);

				// Combine Read Bytes
				uint16_t _Measurement_Raw = ((uint16_t)_MAX17055_Data[1] << 8) | (uint16_t)_MAX17055_Data[0];
				_Measurement_Raw = ((_Measurement_Raw & 0xFF80) >> 7);

				// Calculate Measurement
				float _Value = ((double)_Measurement_Raw * 10) / 1000;

				// End Function
				return(_Value);

			}

			// Get battery recovery voltage function.
			float Recovery_Voltage(void) {

				// Define Data Variable
				uint8_t _MAX17055_Data[2];

				// Get Data from IC
				I2C_Functions::Read_Multiple_Register(0x3A, _MAX17055_Data, 2, false);

				// Combine Read Bytes
				uint16_t _Measurement_Raw = ((uint16_t)_MAX17055_Data[1] << 8) | (uint16_t)_MAX17055_Data[0];
				_Measurement_Raw = (_Measurement_Raw & 0x7F);

				// Calculate Measurement
				float _Value = ((double)_Measurement_Raw * 40) / 1000;

				// End Function
				return(_Value);

			}

			// Get battery instant current function.
			float Instant_Current(void) {

				// Define Data Variable
				uint8_t _MAX17055_Data[2];

				// Get Data from IC
				I2C_Functions::Read_Multiple_Register(0x0A, _MAX17055_Data, 2, false);

				// Combine Read Bytes
				uint16_t _Measurement_Raw = ((uint16_t)_MAX17055_Data[1] << 8) | (uint16_t)_MAX17055_Data[0];

				// Declare Variables
				bool _Signiture = false;

				// Control for Negative Value
				if ((_Measurement_Raw >> 12) == 0xF) {
					
					// Calculate Data
					_Measurement_Raw = 0xFFFF - _Measurement_Raw;
					
					// Assign Signiture
					_Signiture = true;

				} 

				// Calculate Data
				float _Value = (((double)_Measurement_Raw * 1.5625) / __MAX17055_Resistor__) / 1000;
				
				// Assign Signiture
				if (_Signiture) _Value *= -1;

				// End Function
				return(_Value);

			}

			// Get battery average current function.
			float Average_Current(void) {

				// Define Data Variable
				uint8_t _MAX17055_Data[2];

				// Get Data from IC
				I2C_Functions::Read_Multiple_Register(0x0B, _MAX17055_Data, 2, false);

				// Combine Read Bytes
				uint16_t _Measurement_Raw = ((uint16_t)_MAX17055_Data[1] << 8) | (uint16_t)_MAX17055_Data[0];

				// Declare Variables
				bool _Signiture = false;

				// Control for Negative Value
				if ((_Measurement_Raw >> 12) == 0xF) {
					
					// Calculate Data
					_Measurement_Raw = 0xFFFF - _Measurement_Raw;
					
					// Assign Signiture
					_Signiture = true;

				} 

				// Calculate Data
				float _Value = (((double)_Measurement_Raw * 1.5625) / __MAX17055_Resistor__) / 1000;
				
				// Assign Signiture
				if (_Signiture) _Value *= -1;

				// End Function
				return(_Value);

			}

			// Get battery state of charge function.
			float State_Of_Charge(void) {

				// Define Data Variable
				uint8_t _MAX17055_Data[2];

				// Get Data from IC
				I2C_Functions::Read_Multiple_Register(0x06, _MAX17055_Data, 2, false);

				// Combine Read Bytes
				uint16_t _Measurement_Raw = ((uint16_t)_MAX17055_Data[1] << 8) | (uint16_t)_MAX17055_Data[0];

				// Calculate Measurement
				float _Value = ((double)_Measurement_Raw / 256);

				// End Functiom
				return(_Value);

			}

			// Get battery average state of charge function.
			float Average_State_Of_Charge(void) {

				// Define Data Variable
				uint8_t _MAX17055_Data[2];

				// Get Data from IC
				I2C_Functions::Read_Multiple_Register(0x0E, _MAX17055_Data, 2, false);

				// Combine Read Bytes
				uint16_t _Measurement_Raw = ((uint16_t)_MAX17055_Data[1] << 8) | (uint16_t)_MAX17055_Data[0];

				// Calculate Measurement
				float _Value = ((double)_Measurement_Raw / 256);

				// End Function
				return(_Value);

			}

			// Get battery instant capacity function.
			uint16_t Instant_Capacity(void) {

				// Define Data Variable
				uint8_t _MAX17055_Data[2];

				// Get Data from IC
				I2C_Functions::Read_Multiple_Register(0x05, _MAX17055_Data, 2, false);

				// Combine Read Bytes
				uint16_t _Measurement_Raw = ((uint16_t)_MAX17055_Data[1] << 8) | (uint16_t)_MAX17055_Data[0];

				// Calculate Data
				uint16_t _Value = _Measurement_Raw * 5 / 1000 / __MAX17055_Resistor__;

				// End Function
				return(_Value);

			}

			// Get battery design capacity function.
			uint16_t Design_Capacity(void) {

				// Define Data Variable
				uint8_t _MAX17055_Data[2];

				// Get Data from IC
				I2C_Functions::Read_Multiple_Register(0x18, _MAX17055_Data, 2, false);

				// Combine Read Bytes
				uint16_t _Measurement_Raw = ((uint16_t)_MAX17055_Data[1] << 8) | (uint16_t)_MAX17055_Data[0];

				// Calculate Data
				uint16_t _Value = _Measurement_Raw * 5 / 1000 / __MAX17055_Resistor__;

				// End Function
				return(_Value);

			}

			// Get battery full capacity function.
			uint16_t Full_Capacity(void) {

				// Define Data Variable
				uint8_t _MAX17055_Data[2];

				// Get Data from IC
				I2C_Functions::Read_Multiple_Register(0x35, _MAX17055_Data, 2, false);

				// Combine Read Bytes
				uint16_t _Measurement_Raw = ((uint16_t)_MAX17055_Data[1] << 8) | (uint16_t)_MAX17055_Data[0];

				// Calculate Data
				uint16_t _Value = _Measurement_Raw * 5 / 1000 / __MAX17055_Resistor__;

				// End Function
				return(_Value);

			}

			// Get battery temperature function.
			float IC_Temperature(void) {

				// Define Data Variable
				uint8_t _MAX17055_Data[2];

				// Get Data from IC
				I2C_Functions::Read_Multiple_Register(0x08, _MAX17055_Data, 2, false);

				// Combine Read Bytes
				uint16_t _Measurement_Raw = ((uint16_t)_MAX17055_Data[1] << 8) | (uint16_t)_MAX17055_Data[0];

				// Declare Variables
				bool _Signiture = false;
				
				// Control for Negative Value
				if ((_Measurement_Raw >> 12) == 0xF) {

					// Calculate Data
					_Measurement_Raw = 0xFFFF - _Measurement_Raw;
					
					// Assign Signiture
					_Signiture = true;

				}

				// Calculate Data
				float _Value = (double)_Measurement_Raw / 256;

				// Assign Signiture
				if (_Signiture) _Value *= -1;

				// End Function
				return(_Value);

			}

			// Get battery time to empty function.
			uint16_t Time_To_Empty(void) {

				// Define Data Variable
				uint8_t _MAX17055_Data[2];

				// Get Data from IC
				I2C_Functions::Read_Multiple_Register(0x11, _MAX17055_Data, 2, false);

				// Combine Read Bytes
				uint16_t _Measurement_Raw = ((uint16_t)_MAX17055_Data[1] << 8) | (uint16_t)_MAX17055_Data[0];

				// Calculate Data
				float _Value = (uint16_t)_Measurement_Raw * 5.625 / 60 / 60;

				// End Function
				return(_Value);

			}

			// Get battery time to full function.
			uint16_t Time_To_Full(void) {

				// Define Data Variable
				uint8_t _MAX17055_Data[2];

				// Get Data from IC
				I2C_Functions::Read_Multiple_Register(0x20, _MAX17055_Data, 2, false);

				// Combine Read Bytes
				uint16_t _Measurement_Raw = ((uint16_t)_MAX17055_Data[1] << 8) | (uint16_t)_MAX17055_Data[0];

				// Calculate Data
				float _Value = (uint16_t)_Measurement_Raw * 5.625 / 60 / 60;

				// End Function
				return(_Value);

			}

			// Get battery age function.
			uint16_t Battery_Age(void) {

				// Define Data Variable
				uint8_t _MAX17055_Data[2];

				// Get Data from IC
				I2C_Functions::Read_Multiple_Register(0x07, _MAX17055_Data, 2, false);

				// Combine Read Bytes
				uint16_t _Measurement_Raw = ((uint16_t)_MAX17055_Data[1] << 8) | (uint16_t)_MAX17055_Data[0];

				// End Function
				return(_Measurement_Raw);

			}

			// Get battery charge cycle function.
			uint16_t Charge_Cycle(void) {

				// Define Data Variable
				uint8_t _MAX17055_Data[2];

				// Get Data from IC
				I2C_Functions::Read_Multiple_Register(0x17, _MAX17055_Data, 2, false);

				// Combine Read Bytes
				uint16_t _Measurement_Raw = ((uint16_t)_MAX17055_Data[1] << 8) | (uint16_t)_MAX17055_Data[0];

				// End Function
				return(_Measurement_Raw);

			}

			// Get battery status function.
			bool Status_Control(const uint8_t _Status) {

				// Define Data Variable
				uint8_t _Status_Register[2] = {0x00, 0x00};

				// Read Status Register
				I2C_Functions::Read_Multiple_Register(0x00, _Status_Register, 2, false);

				// Control for Status
				if (_Status == MAX17055_POR) return(bitRead(_Status_Register[0], 1));
				else if (_Status == MAX17055_IMin) return(bitRead(_Status_Register[0], 2));
				else if (_Status == MAX17055_IMax) return(bitRead(_Status_Register[0], 6));
				else if (_Status == MAX17055_VMin) return(bitRead(_Status_Register[1], 0));
				else if (_Status == MAX17055_VMax) return(bitRead(_Status_Register[1], 4));
				else if (_Status == MAX17055_TMin) return(bitRead(_Status_Register[1], 1));
				else if (_Status == MAX17055_TMax) return(bitRead(_Status_Register[1], 5));
				else if (_Status == MAX17055_SOC_Min) return(bitRead(_Status_Register[1], 2));
				else if (_Status == MAX17055_SOC_Max) return(bitRead(_Status_Register[1], 6));
				else if (_Status == MAX17055_SOC_Change) return(bitRead(_Status_Register[0], 7));
				else if (_Status == MAX17055_Bat_Status) return(bitRead(_Status_Register[0], 3));
				else if (_Status == MAX17055_Bat_Insert) return(bitRead(_Status_Register[1], 3));
				else if (_Status == MAX17055_Bat_Remove) return(bitRead(_Status_Register[1], 7));

				// End Function
				return(false);

			}

			// Clear battery status function.
			void Status_Clear(void) {

				// Define Data Variable
				const uint8_t _Status_Register[2] = {0x00, 0x00};

				// Write Status Register
				I2C_Functions::Write_Multiple_Register(0x00, _Status_Register, 2);

			}

			// Get battery charge termination current function.
			float Charge_Termination_Current(void) {

				// Define Data Variable
				uint8_t _MAX17055_Data[2];

				// Get Data from IC
				I2C_Functions::Read_Multiple_Register(0x1E, _MAX17055_Data, 2, false);

				// Combine Read Bytes
				uint16_t _Measurement_Raw = ((uint16_t)_MAX17055_Data[1] << 8) | (uint16_t)_MAX17055_Data[0];

				// Calculate Data
				float Value = (((double)_Measurement_Raw * 1.5625) / __MAX17055_Resistor__) / 1000;

				// End Function
				return(Value);

			}

	};

#endif /* defined(__MAX17055__) */
