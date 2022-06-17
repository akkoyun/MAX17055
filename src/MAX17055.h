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

// Define Gauge Parameters
#define _MAX17055_Design_Capacity_		2000	// Battery Capacity
#define _MAX17055_Resistor_				0.01	// Shunt Resistor

class MAX17055 : public I2C_Functions {

	private:

		/**
		 * @brief Set battery empty voltage function.
		 * @param _Empty_Voltage Empty voltage value.
		 * @return true Function succes.
		 * @return false Function fail.
		 */
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
			Read_Multiple_Register(0x3A, MAX17055_Current_Data, 2, true);

			// Clear Current Value
			MAX17055_Current_Data[0] &= 0b01111111;
			MAX17055_Current_Data[1] &= 0b00000000;

			// Define Data Variable
			uint8_t MAX17055_Handle_Data[2];

			// Handle Data
			MAX17055_Handle_Data[0] = MAX17055_Current_Data[0] | MAX17055_RAW_Data[0];
			MAX17055_Handle_Data[1] = MAX17055_Current_Data[1] | MAX17055_RAW_Data[1];

			// Set Register
			bool _Result = Write_Multiple_Register(0x3A, MAX17055_Handle_Data, 2);

			// End Function
			return(_Result);

		}

		/**
		 * @brief Set battery recovery voltage function.
		 * @param _Recovery_Voltage Recovery voltage value.
		 * @return true Function succes.
		 * @return false Function fail.
		 */
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
			Read_Multiple_Register(0x3A, MAX17055_Current_Data, 2, true);

			// Clear Current Value
			MAX17055_Current_Data[0] &= 0b10000000;
			MAX17055_Current_Data[1] &= 0b11111111;

			// Define Data Variable
			uint8_t MAX17055_Handle_Data[2];

			// Handle Data
			MAX17055_Handle_Data[0] = MAX17055_Current_Data[0] | MAX17055_RAW_Data[0];
			MAX17055_Handle_Data[1] = MAX17055_Current_Data[1] | MAX17055_RAW_Data[1];

			// Set Register
			bool _Result = Write_Multiple_Register(0x3A, MAX17055_Handle_Data, 2);

			// End Function
			return(_Result);

		}

		/**
		 * @brief Set battery minimum voltage function.
		 * @param _Minimum_Voltage Minimum voltage value.
		 * @return true Function succes.
		 * @return false Function fail.
		 */
		bool Set_Min_Voltage(float _Minimum_Voltage) {

			// Set Voltage Value
			uint8_t _Raw_Voltage = (uint8_t)(_Minimum_Voltage * 1000 / 20);

			// Define Data Variable
			uint8_t MAX17055_Current_Data[2];

			// Read Current Register
			Read_Multiple_Register(0x01, MAX17055_Current_Data, 2, true);

			// Set Voltage Value
			MAX17055_Current_Data[0] = _Raw_Voltage;

			// Set Register
			bool _Result = Write_Multiple_Register(0x01, MAX17055_Current_Data, 2);

			// End Function
			return(_Result);

		}

		/**
		 * @brief Set battery maximum voltage function.
		 * @param _Maximum_Voltage Maximum voltage value.
		 * @return true Function succes.
		 * @return false Function fail.
		 */
		bool Set_Max_Voltage(float _Maximum_Voltage) {

			// Set Voltage Value
			uint8_t _Raw_Voltage = (uint8_t)(_Maximum_Voltage * 1000 / 20);

			// Define Data Variable
			uint8_t MAX17055_Current_Data[2];

			// Read Current Register
			Read_Multiple_Register(0x01, MAX17055_Current_Data, 2, true);

			// Set Voltage Value
			MAX17055_Current_Data[1] = _Raw_Voltage;

			// Set Register
			bool _Result = Write_Multiple_Register(0x01, MAX17055_Current_Data, 2);

			// End Function
			return(_Result);

		}

		/**
		 * @brief Set battery maximum current function.
		 * @param _Maximum_Current Maximum current value.
		 * @return true Function succes.
		 * @return false Function fail.
		 */
		bool Set_Max_Current(float _Maximum_Current) {

			// Set Current Value
			uint8_t _Raw_Current = (uint8_t)(_Maximum_Current * 1000 / 40);

			// Define Data Variable
			uint8_t MAX17055_Current_Data[2];

			// Read Current Register
			Read_Multiple_Register(0xB4, MAX17055_Current_Data, 2, true);

			// Set Current Value
			MAX17055_Current_Data[1] = _Raw_Current;

			// Set Register
			bool _Result = Write_Multiple_Register(0xB4, MAX17055_Current_Data, 2);

			// End Function
			return(_Result);

		}

		/**
		 * @brief Set battery charge termination current function.
		 * @param _Charge_Termination_Current Charge termination current value.
		 * @return true Function succes.
		 * @return false Function fail.
		 */
		bool Set_Charge_Termination_Current(float _Charge_Termination_Current) {

			// Handle Raw Data
			uint16_t _RAW_Data = (uint16_t)(_Charge_Termination_Current * 1000000 * _MAX17055_Resistor_ / 1.5625);

			// Declare Default Data Array
			uint8_t _Data[2];

			// Set Data Low/High Byte
			_Data[0] = ((_RAW_Data & (uint16_t)0x00FF));
			_Data[1] = ((_RAW_Data & (uint16_t)0xFF00) >> 8);

			// Set Register
			bool _Result = Write_Multiple_Register(0x1E, _Data, 2);

			// End Function
			return(_Result);

		}

		/**
		 * @brief Set battery design capacity function.
		 * @param _Capacity Battery design capacity value.
		 * @return true Function succes.
		 * @return false Function fail.
		 */
		bool Set_Design_Capacity(const uint16_t _Capacity) {

			// Set Raw
			uint16_t _Raw_Cap = (uint16_t)_Capacity * 2;

			// Declare Default Data Array
			uint8_t _Data[2];

			// Set Data Low/High Byte
			_Data[0] = ((_Raw_Cap & (uint16_t)0x00FF));
			_Data[1] = ((_Raw_Cap & (uint16_t)0xFF00) >> 8);

			// Set Register
			bool _Result = Write_Multiple_Register(0x18, _Data, 2);

			// End Function
			return(_Result);

		}

		/**
		 * @brief Set battery minimum SOC function.
		 * @param _Minimum_SOC Minimum SOC value.
		 * @return true Function succes.
		 * @return false Function fail.
		 */
		bool Set_Min_SOC(uint8_t _Minimum_SOC) {

			// Define Data Variable
			uint8_t MAX17055_Current_Data[2];

			// Read Current Register
			Read_Multiple_Register(0x03, MAX17055_Current_Data, 2, true);

			// Set Voltage Value
			MAX17055_Current_Data[0] = _Minimum_SOC;

			// Set Register
			bool _Result = Write_Multiple_Register(0x03, MAX17055_Current_Data, 2);

			// End Function
			return(_Result);

		}

		/**
		 * @brief Set battery maximum SOC function.
		 * @param _Maximum_SOC Maximum SOC value.
		 * @return true Function succes.
		 * @return false Function fail.
		 */
		bool Set_Max_SOC(uint8_t _Maximum_SOC) {

			// Define Data Variable
			uint8_t MAX17055_Current_Data[2];

			// Read Current Register
			Read_Multiple_Register(0x03, MAX17055_Current_Data, 2, true);

			// Set Voltage Value
			MAX17055_Current_Data[1] = _Maximum_SOC;

			// Set Register
			bool _Result = Write_Multiple_Register(0x03, MAX17055_Current_Data, 2);

			// End Function
			return(_Result);

		}

		/**
		 * @brief Set battery minimum temperature function.
		 * @param _Minimum_Temperature Minimum temperature value.
		 * @return true Function succes.
		 * @return false Function fail.
		 */
		bool Set_Min_Temperature(uint8_t _Minimum_Temperature) {

			// Define Data Variable
			uint8_t MAX17055_Current_Data[2];

			// Read Current Register
			Read_Multiple_Register(0x02, MAX17055_Current_Data, 2, true);

			// Set Voltage Value
			MAX17055_Current_Data[0] = _Minimum_Temperature;

			// Set Register
			bool _Result = Write_Multiple_Register(0x02, MAX17055_Current_Data, 2);

			// End Function
			return(_Result);

		}

		/**
		 * @brief Set battery maximum temperature function.
		 * @param _Maximum_Temperature Maximum temperature value.
		 * @return true Function succes.
		 * @return false Function fail.
		 */
		bool Set_Max_Temperature(uint8_t _Maximum_Temperature) {

			// Define Data Variable
			uint8_t MAX17055_Current_Data[2];

			// Read Current Register
			Read_Multiple_Register(0x02, MAX17055_Current_Data, 2, true);

			// Set Voltage Value
			MAX17055_Current_Data[1] = _Maximum_Temperature;

			// Set Register
			bool _Result = Write_Multiple_Register(0x02, MAX17055_Current_Data, 2);

			// End Function
			return(_Result);

		}

		bool Set_dQAcc(const uint16_t _Capacity) {

			// Declare Default Data Array
			uint8_t _Data[2];

			// Set Data Low/High Byte
			_Data[0] = (((_Capacity / 32) & (uint16_t)0x00FF));
			_Data[1] = (((_Capacity / 32) & (uint16_t)0xFF00) >> 8);

			// Set Register
			bool _Result = Write_Multiple_Register(0x45, _Data, 2);

			// End Function
			return(_Result);

		}

		bool Set_dPAcc(const uint16_t _Capacity) {

			// Declare dQAcc Variable
			uint8_t _dQAcc[2];

			// Get dQAcc Register
			Read_Multiple_Register(0x45, _dQAcc, 2, false);

			// Combine Read Bytes
			uint16_t dQAcc = ((uint16_t)_dQAcc[1] << 8) | (uint16_t)_dQAcc[0];

			// Handle dPAcc Variable
			uint16_t dPAcc = dQAcc * 51200 / _Capacity;

			// Declare Default Data Array
			uint8_t _Data[2];

			// Set Data Low/High Byte
			_Data[0] = ((dPAcc & (uint16_t)0x00FF));
			_Data[1] = ((dPAcc & (uint16_t)0xFF00) >> 8);

			// Set Register
			bool _Result = Write_Multiple_Register(0x46, _Data, 2);

			// End Function
			return(_Result);

		}

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
			bool _Result = Write_Multiple_Register(0xDB, _Data, 2);

			// End Function
			return(_Result);

		}

		bool Set_HibCFG(const uint16_t _Config) {

			// Declare Default Data Array
			uint8_t _Data[2];

			// Set Data Low/High Byte
			_Data[0] = ((_Config & (uint16_t)0x00FF));
			_Data[1] = ((_Config & (uint16_t)0xFF00) >> 8);

			// Set Register
			bool _Result = Write_Multiple_Register(0xBA, _Data, 2);

			// End Function
			return(_Result);

		}

		bool Set_Config(const uint8_t _Channel, const uint16_t _Config) {

			// Declare Default Data Array
			uint8_t _Data[2];

			// Declare Result Variable
			bool _Result;

			// Set Data Low/High Byte
			_Data[0] = ((_Config & (uint16_t)0x00FF));
			_Data[1] = ((_Config & (uint16_t)0xFF00) >> 8);

			// Config1 Setting
			if (_Channel == 1) _Result = Write_Multiple_Register(0x1D, _Data, 2);
			if (_Channel == 2) _Result = Write_Multiple_Register(0xBB, _Data, 2);

			// End Function
			return(_Result);

		}


	public:

		// Declare Global Variable
		struct Status_Struct {
			bool Power_on_Reset = false;
			bool Voltage_Min = false;
			bool Voltage_Max = false;
			bool Current_Min = false;
			bool Current_Max = false;
			bool Temperature_Min = false;
			bool Temperature_Max = false;
			bool SOC_Min = false;
			bool SOC_Max = false;
			bool Battery_Status = false;
			bool SOC_Change = false;
			bool Battery_Insertion = false;
			bool Battery_Removal = false;
		} Status;

		MAX17055(bool _Multiplexer_Enable, uint8_t _Multiplexer_Channel) : I2C_Functions(__I2C_Addr_MAX17055__, _Multiplexer_Enable, _Multiplexer_Channel) {

			// Set Configuration
			this->Set_Config(1, 0x0000);
			this->Set_Config(2, 0x0218);
			this->Set_HibCFG(0x0000);

			// Design Capacity Register
			this->Set_Design_Capacity(2000);

			// ModelCfg Register
			this->Set_ModelCfg(2);

			// IChgTerm Register
			this->Set_Charge_Termination_Current(0.250);

			// VEmpty Register
			this->Set_Empty_Voltage(3.000);

			// VRecovery Register
			this->Set_Recovery_Voltage(3.880);

			// Set Minimum Voltage
			this->Set_Min_Voltage(3.800);

			// Set Maximum Voltage
			this->Set_Max_Voltage(4.200);

			// Set Maximum Current
			this->Set_Max_Current(1.000);

			// Set Minimum SOC
			this->Set_Min_SOC(20);

			// Set Maximum SOC
			this->Set_Max_SOC(90);

			// Set Minimum Temperature
			this->Set_Min_Temperature(10);

			// Set Maximum Temperature
			this->Set_Max_Temperature(50);

			// I2C Delay
			delay(5);
			
		}

		float Instant_Voltage(void) {

			// Define Data Variable
			uint8_t MAX17055_Data[2];

			// Get Data from IC
			Read_Multiple_Register(0x09, MAX17055_Data, 2, false);

			// Combine Read Bytes
			uint16_t Measurement_Raw = ((uint16_t)MAX17055_Data[1] << 8) | (uint16_t)MAX17055_Data[0];

			// Calculate Measurement
			float _Value = ((double)Measurement_Raw * 1.25 / 16) / 1000;

			// End Function
			return(_Value);

		}

		float Average_Voltage(void) {

			// Define Data Variable
			uint8_t MAX17055_Data[2];

			// Get Data from IC
			Read_Multiple_Register(0x19, MAX17055_Data, 2, false);

			// Combine Read Bytes
			uint16_t Measurement_Raw = ((uint16_t)MAX17055_Data[1] << 8) | (uint16_t)MAX17055_Data[0];

			// Calculate Measurement
			float _Value = ((double)Measurement_Raw * 1.25 / 16) / 1000;

			// End Function
			return(_Value);

		}

		float Get_Empty_Voltage(void) {

			// Define Data Variable
			uint8_t MAX17055_Data[2];

			// Get Data from IC
			Read_Multiple_Register(0x3A, MAX17055_Data, 2, false);

			// Combine Read Bytes
			uint16_t Measurement_Raw = ((uint16_t)MAX17055_Data[1] << 8) | (uint16_t)MAX17055_Data[0];
			Measurement_Raw = ((Measurement_Raw & 0xFF80) >> 7);

			// Calculate Measurement
			float _Value = ((double)Measurement_Raw * 10) / 1000;

			// End Function
			return(_Value);

		}

		float Recovery_Voltage(void) {

			// Define Data Variable
			uint8_t MAX17055_Data[2];

			// Get Data from IC
			Read_Multiple_Register(0x3A, MAX17055_Data, 2, false);

			// Combine Read Bytes
			uint16_t Measurement_Raw = ((uint16_t)MAX17055_Data[1] << 8) | (uint16_t)MAX17055_Data[0];
			Measurement_Raw = (Measurement_Raw & 0x7F);

			// Calculate Measurement
			float _Value = ((double)Measurement_Raw * 40) / 1000;

			// End Function
			return(_Value);

		}

		float Instant_Current(void) {

			// Define Data Variable
			uint8_t MAX17055_Data[2];

			// Get Data from IC
			Read_Multiple_Register(0x0A, MAX17055_Data, 2, false);

			// Combine Read Bytes
			uint16_t Measurement_Raw = ((uint16_t)MAX17055_Data[1] << 8) | (uint16_t)MAX17055_Data[0];

			// Declare Variables
			bool _Signiture = false;

			// Control for Negative Value
			if ((Measurement_Raw >> 12) == 0xF) {
				Measurement_Raw = 0xFFFF - Measurement_Raw;
				_Signiture = true;
			} 

			// Calculate Data
			float _Value = (((double)Measurement_Raw * 1.5625) / _MAX17055_Resistor_) / 1000;
			
			// Assign Signiture
			if (_Signiture) _Value *= -1;

			// End Function
			return(_Value);

		}

		float Average_Current(void) {

			// Define Data Variable
			uint8_t MAX17055_Data[2];

			// Get Data from IC
			Read_Multiple_Register(0x0B, MAX17055_Data, 2, false);

			// Combine Read Bytes
			uint16_t Measurement_Raw = ((uint16_t)MAX17055_Data[1] << 8) | (uint16_t)MAX17055_Data[0];

			// Declare Variables
			bool _Signiture = false;

			// Control for Negative Value
			if ((Measurement_Raw >> 12) == 0xF) {
				Measurement_Raw = 0xFFFF - Measurement_Raw;
				_Signiture = true;
			} 

			// Calculate Data
			float _Value = (((double)Measurement_Raw * 1.5625) / _MAX17055_Resistor_) / 1000;
			
			// Assign Signiture
			if (_Signiture) _Value *= -1;

			// End Function
			return(_Value);

		}

		float State_Of_Charge(void) {

			// Define Data Variable
			uint8_t MAX17055_Data[2];

			// Get Data from IC
			Read_Multiple_Register(0x06, MAX17055_Data, 2, false);

			// Calculate Measurement
			float _Value = ((double)MAX17055_Data[1] + (double)MAX17055_Data[0] / 256);

			// End Functiom
			return(_Value);

		}

		float Average_State_Of_Charge(void) {

			// Define Data Variable
			uint8_t MAX17055_Data[2];

			// Get Data from IC
			Read_Multiple_Register(0x0E, MAX17055_Data, 2, false);

			// Calculate Measurement
			float _Value = ((double)MAX17055_Data[1] + (double)MAX17055_Data[0] / 256);

			// End Function
			return(_Value);

		}

		uint16_t Instant_Capacity(void) {

			// Define Data Variable
			uint8_t MAX17055_Data[2];

			// Get Data from IC
			Read_Multiple_Register(0x05, MAX17055_Data, 2, false);

			// Combine Read Bytes
			uint16_t Measurement_Raw = ((uint16_t)MAX17055_Data[1] << 8) | (uint16_t)MAX17055_Data[0];

			// Calculate Data
			uint16_t _Measurement = Measurement_Raw * 5 / 1000 / _MAX17055_Resistor_;

			// End Function
			return(_Measurement);

		}

		uint16_t Design_Capacity(void) {

			// Define Data Variable
			uint8_t MAX17055_Data[2];

			// Get Data from IC
			Read_Multiple_Register(0x18, MAX17055_Data, 2, false);

			// Combine Read Bytes
			uint16_t Measurement_Raw = ((uint16_t)MAX17055_Data[1] << 8) | (uint16_t)MAX17055_Data[0];

			// Calculate Data
			uint16_t _Measurement = Measurement_Raw * 5 / 1000 / _MAX17055_Resistor_;

			// End Function
			return(_Measurement);

		}

		uint16_t Full_Capacity(void) {

			// Define Data Variable
			uint8_t MAX17055_Data[2];

			// Get Data from IC
			Read_Multiple_Register(0x35, MAX17055_Data, 2, false);

			// Combine Read Bytes
			uint16_t Measurement_Raw = ((uint16_t)MAX17055_Data[1] << 8) | (uint16_t)MAX17055_Data[0];

			// Calculate Data
			uint16_t _Measurement = Measurement_Raw * 5 / 1000 / _MAX17055_Resistor_;

			// End Function
			return(_Measurement);

		}

		float Temperature(void) {

			// Define Data Variable
			uint8_t MAX17055_Data[2];

			// Get Data from IC
			Read_Multiple_Register(0x08, MAX17055_Data, 2, false);

			// Combine Read Bytes
			uint16_t Measurement_Raw = ((uint16_t)MAX17055_Data[1] << 8) | (uint16_t)MAX17055_Data[0];

			// Declare Variables
			bool _Signiture = false;
			
			// Control for Negative Value
			if ((Measurement_Raw >> 12) == 0xF) {
				Measurement_Raw = 0xFFFF - Measurement_Raw;
				_Signiture = true;
			}

			// Calculate Data
			float _Value = (double)Measurement_Raw / 256;

			// Assign Signiture
			if (_Signiture) _Value *= -1;

			// End Function
			return(_Value);

		}

		uint16_t Time_To_Empty(void) {

			// Define Data Variable
			uint8_t MAX17055_Data[2];

			// Get Data from IC
			Read_Multiple_Register(0x11, MAX17055_Data, 2, false);

			// Combine Read Bytes
			uint16_t Measurement_Raw = ((uint16_t)MAX17055_Data[1] << 8) | (uint16_t)MAX17055_Data[0];

			// Calculate Data
			float _Value = (uint16_t)Measurement_Raw * 5.625 / 60 / 60;

			// End Function
			return(_Value);

		}

		uint16_t Time_To_Full(void) {

			// Define Data Variable
			uint8_t MAX17055_Data[2];

			// Get Data from IC
			Read_Multiple_Register(0x20, MAX17055_Data, 2, false);

			// Combine Read Bytes
			uint16_t Measurement_Raw = ((uint16_t)MAX17055_Data[1] << 8) | (uint16_t)MAX17055_Data[0];

			// Calculate Data
			float _Value = (uint16_t)Measurement_Raw * 5.625 / 60 / 60;

			// End Function
			return(_Value);

		}

		uint16_t Battery_Age(void) {

			// Define Data Variable
			uint8_t MAX17055_Data[2];

			// Get Data from IC
			Read_Multiple_Register(0x07, MAX17055_Data, 2, false);

			// Combine Read Bytes
			uint16_t Measurement_Raw = ((uint16_t)MAX17055_Data[1] << 8) | (uint16_t)MAX17055_Data[0];

			// End Function
			return(Measurement_Raw);

		}

		uint16_t Charge_Cycle(void) {

			// Define Data Variable
			uint8_t MAX17055_Data[2];

			// Get Data from IC
			Read_Multiple_Register(0x17, MAX17055_Data, 2, false);

			// Combine Read Bytes
			uint16_t Measurement_Raw = ((uint16_t)MAX17055_Data[1] << 8) | (uint16_t)MAX17055_Data[0];

			// Calculate Data
			float _Value = (uint16_t)Measurement_Raw;

			// End Function
			return(_Value);

		}

		void Status_Control(void) {

			// Define Data Variable
			uint8_t _Status_Register[2];

			// Read Status Register
			Read_Multiple_Register(0x00, _Status_Register, 2, false);

			// Set Value
			this->Status.Power_on_Reset = bitRead(_Status_Register[0], 1);
			this->Status.Current_Min = bitRead(_Status_Register[0], 2);
			this->Status.Battery_Status = bitRead(_Status_Register[0], 3);
			this->Status.Current_Max = bitRead(_Status_Register[0], 6);
			this->Status.SOC_Change = bitRead(_Status_Register[0], 7);
			this->Status.Voltage_Min = bitRead(_Status_Register[1], 0);
			this->Status.Temperature_Min = bitRead(_Status_Register[1], 1);
			this->Status.SOC_Min = bitRead(_Status_Register[1], 2);
			this->Status.Battery_Insertion = bitRead(_Status_Register[1], 3);
			this->Status.Voltage_Max = bitRead(_Status_Register[1], 4);
			this->Status.Temperature_Max = bitRead(_Status_Register[1], 5);
			this->Status.SOC_Max = bitRead(_Status_Register[1], 6);
			this->Status.Battery_Removal = bitRead(_Status_Register[1], 7);

			// Clear Bits
			_Status_Register[0] = 0x00;
			_Status_Register[1] = 0x00;

			// Write Status Register
			Write_Multiple_Register(0x00, _Status_Register, 2);

		}

		float Get_Charge_Termination_Current(void) {

			// Define Data Variable
			uint8_t MAX17055_Data[2];

			// Get Data from IC
			Read_Multiple_Register(0x1E, MAX17055_Data, 2, false);

			// Combine Read Bytes
			uint16_t Measurement_Raw = ((uint16_t)MAX17055_Data[1] << 8) | (uint16_t)MAX17055_Data[0];

			// Calculate Data
			float Value = (((double)Measurement_Raw * 1.5625) / _MAX17055_Resistor_) / 1000;

			// End Function
			return(Value);

		}

};

#endif /* defined(__MAX17055__) */
