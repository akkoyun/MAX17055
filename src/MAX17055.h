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

// Define Gauge Parameters
#define _MAX17055_Design_Capacity_		2000	// Battery Capacity
#define _MAX17055_Resistor_				0.01	// Shunt Resistor
#define _MAX17055_Empty_Voltage_		3.0		// Empty Voltage
#define _MAX17055_Recovery_Voltage_		4.1		// Recovery Voltage
#define _MAX17055_Min_Voltage_			3.0		// Min Voltage
#define _MAX17055_Max_Voltage_			4.0		// Max Voltage

class MAX17055 : public I2C_Functions {

	private:

		bool Set_Empty_Recovery_Voltage(const float _Empty_Voltage, const float _Recovery_Voltage) {

			// Set Empty Raw
			uint32_t _Raw_Empty_Voltage = (uint32_t(_Empty_Voltage * 1000) / 10);
			_Raw_Empty_Voltage = (_Raw_Empty_Voltage << 7) & 0xFF80;

			// Set Recovery Raw
			uint32_t _Raw_Recovery_Voltage = ((uint32_t(_Recovery_Voltage * 1000) / 40) & 0x7F);

			// Set Raw Data
			uint32_t _Raw_Voltage = _Raw_Empty_Voltage | _Raw_Recovery_Voltage;

			// Declare Default Data Array
			uint8_t _Data[2];

			// Set Data Low/High Byte
			_Data[0] = ((_Raw_Voltage & (uint16_t)0x00FF));
			_Data[1] = ((_Raw_Voltage & (uint16_t)0xFF00) >> 8);

			// Set Register
			bool _Result = Write_Multiple_Register(0x3A, _Data, 2);

			// End Function
			return(_Result);

		}

		bool Set_Max_Min_Voltage(const float _Min_Voltage, const float _Max_Voltage) {

			// Set Empty Raw
			uint32_t _Raw_Max_Voltage = ((uint32_t)_Max_Voltage * 1000) / 20;
			_Raw_Max_Voltage = (_Raw_Max_Voltage << 7) & 0xFF80;

			// Set Recovery Raw
			uint32_t _Raw_Min_Voltage = (((uint32_t)_Min_Voltage * 1000) / 20) & 0x7F;

			// Set Raw Data
			uint32_t _Raw_Voltage = _Raw_Max_Voltage | _Raw_Min_Voltage;

			// Declare Default Data Array
			uint8_t _Data[2];

			// Set Data Low/High Byte
			_Data[0] = ((_Raw_Voltage & (uint16_t)0x00FF));
			_Data[1] = ((_Raw_Voltage & (uint16_t)0xFF00) >> 8);

			// Set Register
			bool _Result = Write_Multiple_Register(0x3A, _Data, 2);

			// End Function
			return(_Result);

		}

		float Charge_Termination_Current(void) {

			// Define Statistical Variables
			uint8_t _Read_Count = 1;

			// Define Measurement Read Array
			float _Measurement_Array[_Read_Count];

			// Read Loop For Read Count
			for (uint8_t _Read_ID = 0; _Read_ID < _Read_Count; _Read_ID++) {

				// Define Data Variable
				uint8_t MAX17055_Data[2];

				// Get Data from IC
				Read_Multiple_Register(0x1E, MAX17055_Data, 2, false);

				// Combine Read Bytes
				uint16_t Measurement_Raw = ((uint16_t)MAX17055_Data[1] << 8) | (uint16_t)MAX17055_Data[0];

				// Calculate Data
				_Measurement_Array[_Read_ID] = (((double)Measurement_Raw * 1.5625) / _MAX17055_Resistor_) / 1000;
				
			}
			
			// Construct Object
			Array_Stats<float> Data_Array(_Measurement_Array, _Read_Count);

			// End Function
			return(Data_Array.Arithmetic_Average());

		}

		bool Set_Charge_Termination_Current(void) {

			// Set Empty Raw
			uint32_t _Raw_Termination_Voltage = 0x0280;

			// Declare Default Data Array
			uint8_t _Data[2];

			// Set Data Low/High Byte
			_Data[0] = ((_Raw_Termination_Voltage & (uint16_t)0x00FF));
			_Data[1] = ((_Raw_Termination_Voltage & (uint16_t)0xFF00) >> 8);

			// Set Register
			bool _Result = Write_Multiple_Register(0x1E, _Data, 2);

			// End Function
			return(_Result);

		}

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

		bool Set_ModelCfg(const uint8_t _Model_ID, const bool _VChg) {

			// Declare Variable
			uint16_t _Data_Set = 0x00;

			// Set Charge Voltage
			if (_VChg == true) bitSet(_Data_Set, 10);
			if (_VChg == false) bitClear(_Data_Set, 10);

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

		MAX17055(bool _Multiplexer_Enable, uint8_t _Multiplexer_Channel) : I2C_Functions(__I2C_Addr_MAX17055__, _Multiplexer_Enable, _Multiplexer_Channel) {

			// Set Configuration
			this->Set_Config(1, 0x0000);
			this->Set_Config(2, 0x0218);
			this->Set_HibCFG(0x0000);

			// Design Capacity Register
			this->Set_Design_Capacity(_MAX17055_Design_Capacity_);

			// ModelCfg Register
			this->Set_ModelCfg(2, true);

			// VEmpty Register
			this->Set_Empty_Recovery_Voltage(_MAX17055_Empty_Voltage_, _MAX17055_Recovery_Voltage_);

			// Max Min Voltage Register
			this->Set_Max_Min_Voltage(_MAX17055_Min_Voltage_, _MAX17055_Max_Voltage_);

			// IChgTerm Register
			this->Set_Charge_Termination_Current();

			// I2C Delay
			delay(5);
			
		}

		float Instant_Voltage(void) {

			// Define Statistical Variables
			const uint8_t _Read_Count = 1;

			// Define Measurement Read Array
			float _Measurement_Array[_Read_Count];

			// Read Loop For Read Count
			for (uint8_t _Read_ID = 0; _Read_ID < _Read_Count; _Read_ID++) {

				// Define Data Variable
				uint8_t MAX17055_Data[2];

				// Get Data from IC
				Read_Multiple_Register(0x09, MAX17055_Data, 2, false);

				// Combine Read Bytes
				uint16_t Measurement_Raw = ((uint16_t)MAX17055_Data[1] << 8) | (uint16_t)MAX17055_Data[0];

				// Calculate Measurement
				_Measurement_Array[_Read_ID] = ((double)Measurement_Raw * 1.25 / 16) / 1000;

			}

			// Construct Object
			Array_Stats<float> Data_Array(_Measurement_Array, _Read_Count);

			// End Function
			return(Data_Array.Arithmetic_Average());

		}

		float Average_Voltage(void) {

			// Define Statistical Variables
			const uint8_t _Read_Count = 1;

			// Define Measurement Read Array
			float _Measurement_Array[_Read_Count];

			// Read Loop For Read Count
			for (uint8_t _Read_ID = 0; _Read_ID < _Read_Count; _Read_ID++) {

				// Define Data Variable
				uint8_t MAX17055_Data[2];

				// Get Data from IC
				Read_Multiple_Register(0x19, MAX17055_Data, 2, false);

				// Combine Read Bytes
				uint16_t Measurement_Raw = ((uint16_t)MAX17055_Data[1] << 8) | (uint16_t)MAX17055_Data[0];

				// Calculate Measurement
				_Measurement_Array[_Read_ID] = ((double)Measurement_Raw * 1.25 / 16) / 1000;

			}

			// Construct Object
			Array_Stats<float> Data_Array(_Measurement_Array, _Read_Count);

			// End Function
			return(Data_Array.Arithmetic_Average());

		}

		float Empty_Voltage(void) {

			// Define Statistical Variables
			uint8_t _Read_Count = 1;

			// Define Measurement Read Array
			float _Measurement_Array[_Read_Count];

			// Read Loop For Read Count
			for (uint8_t _Read_ID = 0; _Read_ID < _Read_Count; _Read_ID++) {

				// Define Data Variable
				uint8_t MAX17055_Data[2];

				// Get Data from IC
				Read_Multiple_Register(0x3A, MAX17055_Data, 2, false);

				// Combine Read Bytes
				uint16_t Measurement_Raw = ((uint16_t)MAX17055_Data[1] << 8) | (uint16_t)MAX17055_Data[0];
				Measurement_Raw = ((Measurement_Raw & 0xFF80) >> 7);

				// Calculate Measurement
				_Measurement_Array[_Read_ID] = ((double)Measurement_Raw * 10) / 1000;

			}
			
			// Construct Object
			Array_Stats<float> Data_Array(_Measurement_Array, _Read_Count);

			// End Function
			return(Data_Array.Arithmetic_Average());

		}

		float Recovery_Voltage(void) {

			// Define Statistical Variables
			uint8_t _Read_Count = 1;

			// Define Measurement Read Array
			float _Measurement_Array[_Read_Count];

			// Read Loop For Read Count
			for (uint8_t _Read_ID = 0; _Read_ID < _Read_Count; _Read_ID++) {

				// Define Data Variable
				uint8_t MAX17055_Data[2];

				// Get Data from IC
				Read_Multiple_Register(0x3A, MAX17055_Data, 2, false);

				// Combine Read Bytes
				uint16_t Measurement_Raw = ((uint16_t)MAX17055_Data[1] << 8) | (uint16_t)MAX17055_Data[0];
				Measurement_Raw = (Measurement_Raw & 0x7F);

				// Calculate Measurement
				_Measurement_Array[_Read_ID] = ((double)Measurement_Raw * 40) / 1000;

			}
			
			// Construct Object
			Array_Stats<float> Data_Array(_Measurement_Array, _Read_Count);

			// End Function
			return(Data_Array.Arithmetic_Average());

		}

		float Instant_Current(void) {

			// Define Statistical Variables
			uint8_t _Read_Count = 5;

			// Define Measurement Read Array
			float _Measurement_Array[_Read_Count];

			// Read Loop For Read Count
			for (uint8_t _Read_ID = 0; _Read_ID < _Read_Count; _Read_ID++) {

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
				_Measurement_Array[_Read_ID] = (((double)Measurement_Raw * 1.5625) / _MAX17055_Resistor_) / 1000;
				
				// Assign Signiture
				if (_Signiture) _Measurement_Array[_Read_ID] *= -1;

			}
			
			// Construct Object
			Array_Stats<float> Data_Array(_Measurement_Array, _Read_Count);

			// End Function
			return(Data_Array.Arithmetic_Average());

		}

		float Average_Current(void) {

			// Define Statistical Variables
			uint8_t _Read_Count = 5;

			// Define Measurement Read Array
			float _Measurement_Array[_Read_Count];

			// Read Loop For Read Count
			for (uint8_t _Read_ID = 0; _Read_ID < _Read_Count; _Read_ID++) {

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
				_Measurement_Array[_Read_ID] = (((double)Measurement_Raw * 1.5625) / _MAX17055_Resistor_) / 1000;
				
				// Assign Signiture
				if (_Signiture) _Measurement_Array[_Read_ID] *= -1;

			}
			
			// Construct Object
			Array_Stats<float> Data_Array(_Measurement_Array, _Read_Count);

			// End Function
			return(Data_Array.Arithmetic_Average());

		}

		float State_Of_Charge(void) {

			// Define Statistical Variables
			uint8_t _Read_Count = 1;

			// Define Measurement Read Array
			float _Measurement_Array[_Read_Count];

			// Read Loop For Read Count
			for (uint8_t _Read_ID = 0; _Read_ID < _Read_Count; _Read_ID++) {

				// Define Data Variable
				uint8_t MAX17055_Data[2];

				// Get Data from IC
				Read_Multiple_Register(0x06, MAX17055_Data, 2, false);

				// Calculate Measurement
				_Measurement_Array[_Read_ID] = ((double)MAX17055_Data[1] + (double)MAX17055_Data[0] / 256);

			}
			
			// Construct Object
			Array_Stats<float> Data_Array(_Measurement_Array, _Read_Count);

			// End Function
			return(Data_Array.Arithmetic_Average());

		}

		float Average_State_Of_Charge(void) {

			// Define Statistical Variables
			uint8_t _Read_Count = 5;

			// Define Measurement Read Array
			float _Measurement_Array[_Read_Count];

			// Read Loop For Read Count
			for (uint8_t _Read_ID = 0; _Read_ID < _Read_Count; _Read_ID++) {

				// Define Data Variable
				uint8_t MAX17055_Data[2];

				// Get Data from IC
				Read_Multiple_Register(0x0E, MAX17055_Data, 2, false);

				// Calculate Measurement
				_Measurement_Array[_Read_ID] = ((double)MAX17055_Data[1] + (double)MAX17055_Data[0] / 256);

			}
			
			// Construct Object
			Array_Stats<float> Data_Array(_Measurement_Array, _Read_Count);

			// End Function
			return(Data_Array.Arithmetic_Average());

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

			// Define Statistical Variables
			uint8_t _Read_Count = 5;

			// Define Measurement Read Array
			float _Measurement_Array[_Read_Count];

			// Read Loop For Read Count
			for (size_t _Read_ID = 0; _Read_ID < _Read_Count; _Read_ID++) {

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
				_Measurement_Array[_Read_ID] = (double)Measurement_Raw / 256;

				// Assign Signiture
				if (_Signiture) _Measurement_Array[_Read_ID] *= -1;

			}
			
			// Construct Object
			Array_Stats<float> Data_Array(_Measurement_Array, _Read_Count);

			// End Function
			return(Data_Array.Arithmetic_Average());

		}

		uint16_t Time_To_Empty(void) {

			// Define Statistical Variables
			uint8_t _Read_Count = 5;
			uint8_t _Average_Type = 1;

			// Define Measurement Read Array
			float _Measurement_Array[_Read_Count];

			// Read Loop For Read Count
			for (uint8_t _Read_ID = 0; _Read_ID < _Read_Count; _Read_ID++) {

				// Define Data Variable
				uint8_t MAX17055_Data[2];

				// Get Data from IC
				Read_Multiple_Register(0x11, MAX17055_Data, 2, false);

				// Combine Read Bytes
				uint16_t Measurement_Raw = ((uint16_t)MAX17055_Data[1] << 8) | (uint16_t)MAX17055_Data[0];

				// Calculate Data
				_Measurement_Array[_Read_ID] = (uint16_t)Measurement_Raw * 5.625 / 60 / 60;

			}
			
			// Construct Object
			Array_Stats<float> Data_Array(_Measurement_Array, sizeof(_Measurement_Array) / sizeof(_Measurement_Array[0]));

			// End Function
			return(Data_Array.Average(_Average_Type));

		}

		uint16_t Time_To_Full(void) {

			// Define Statistical Variables
			uint8_t _Read_Count = 5;
			uint8_t _Average_Type = 1;

			// Define Measurement Read Array
			float _Measurement_Array[_Read_Count];

			// Read Loop For Read Count
			for (uint8_t _Read_ID = 0; _Read_ID < _Read_Count; _Read_ID++) {

				// Define Data Variable
				uint8_t MAX17055_Data[2];

				// Get Data from IC
				Read_Multiple_Register(0x20, MAX17055_Data, 2, false);

				// Combine Read Bytes
				uint16_t Measurement_Raw = ((uint16_t)MAX17055_Data[1] << 8) | (uint16_t)MAX17055_Data[0];

				// Calculate Data
				_Measurement_Array[_Read_ID] = (uint16_t)Measurement_Raw * 5.625 / 60 / 60;

			}
			
			// Construct Object
			Array_Stats<float> Data_Array(_Measurement_Array, sizeof(_Measurement_Array) / sizeof(_Measurement_Array[0]));

			// End Function
			return(Data_Array.Average(_Average_Type));

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

			// Define Statistical Variables
			uint8_t _Read_Count = 5;
			uint8_t _Average_Type = 1;

			// Define Measurement Read Array
			float _Measurement_Array[_Read_Count];

			// Read Loop For Read Count
			for (uint8_t _Read_ID = 0; _Read_ID < _Read_Count; _Read_ID++) {

				// Define Data Variable
				uint8_t MAX17055_Data[2];

				// Get Data from IC
				Read_Multiple_Register(0x17, MAX17055_Data, 2, false);

				// Combine Read Bytes
				uint16_t Measurement_Raw = ((uint16_t)MAX17055_Data[1] << 8) | (uint16_t)MAX17055_Data[0];

				// Calculate Data
				_Measurement_Array[_Read_ID] = (uint16_t)Measurement_Raw;

			}
			
			// Construct Object
			Array_Stats<float> Data_Array(_Measurement_Array, sizeof(_Measurement_Array) / sizeof(_Measurement_Array[0]));

			// End Function
			return(Data_Array.Average(_Average_Type));

		}

		bool is_Power_on_Reset(void) {
			
			// Get Status Bit
			bool _Result = Read_Register_Bit(0x00, 0x01);

			// Reset Bit
			Clear_Register_Bit(0x00, 0x01, false);

			// End Function
			return(_Result);
			
		}

		bool is_Min_Current(void) {
			
			// Get Status Bit
			bool _Result = Read_Register_Bit(0x00, 0x02);

			// Reset Bit
			Clear_Register_Bit(0x00, 0x02, false);

			// End Function
			return(_Result);
			
		}

		bool is_Max_Current(void) {
			
			// Get Status Bit
			bool _Result = Read_Register_Bit(0x00, 0x06);

			// Reset Bit
			Clear_Register_Bit(0x00, 0x06, false);

			// End Function
			return(_Result);
			
		}

		bool is_Min_Voltage(void) {
			
			// Get Status Bit
			bool _Result = Read_Register_Bit(0x00, 0x08);

			// Reset Bit
			Clear_Register_Bit(0x00, 0x08, false);

			// End Function
			return(_Result);
			
		}

		bool is_Max_Voltage(void) {
			
			// Get Status Bit
			bool _Result = Read_Register_Bit(0x00, 0x12);

			// Reset Bit
			Clear_Register_Bit(0x00, 0x12, false);

			// End Function
			return(_Result);
			
		}

		bool is_Min_Temperature(void) {
			
			// Get Status Bit
			bool _Result = Read_Register_Bit(0x00, 0x09);

			// Reset Bit
			Clear_Register_Bit(0x00, 0x09, false);

			// End Function
			return(_Result);
			
		}

		bool is_Max_Temperature(void) {
			
			// Get Status Bit
			bool _Result = Read_Register_Bit(0x00, 0x13);

			// Reset Bit
			Clear_Register_Bit(0x00, 0x13, false);

			// End Function
			return(_Result);
			
		}

		bool is_Min_SOC(void) {
			
			// Get Status Bit
			bool _Result = Read_Register_Bit(0x00, 0x10);

			// Reset Bit
			Clear_Register_Bit(0x00, 0x10, false);

			// End Function
			return(_Result);
			
		}

		bool is_Max_SOC(void) {
			
			// Get Status Bit
			bool _Result = Read_Register_Bit(0x00, 0x14);

			// Reset Bit
			Clear_Register_Bit(0x00, 0x14, false);

			// End Function
			return(_Result);
			
		}

		bool is_Battery_Present(void) {
			
			// Get Status Bit
			bool _Result = Read_Register_Bit(0x00, 0x03);

			// Reset Bit
			Clear_Register_Bit(0x00, 0x03, false);

			// End Function
			return(_Result);
			
		}

		bool is_SOC_Change(void) {
			
			// Get Status Bit
			bool _Result = Read_Register_Bit(0x00, 0x07);

			// Reset Bit
			Clear_Register_Bit(0x00, 0x07, false);

			// End Function
			return(_Result);
			
		}

		bool is_Battery_Insertion(void) {
			
			// Get Status Bit
			bool _Result = Read_Register_Bit(0x00, 0x11);

			// Reset Bit
			Clear_Register_Bit(0x00, 0x11, false);

			// End Function
			return(_Result);
			
		}

		bool is_Battery_Removal(void) {
			
			// Get Status Bit
			bool _Result = Read_Register_Bit(0x00, 0x15);

			// Reset Bit
			Clear_Register_Bit(0x00, 0x15, false);

			// End Function
			return(_Result);
			
		}

};

#endif /* defined(__MAX17055__) */
