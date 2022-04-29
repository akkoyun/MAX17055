/* *******************************************************************************
 *  Copyright (C) 2014-2019 Mehmet Gunce Akkoyun Can not be copied and/or
 *	distributed without the express permission of Mehmet Gunce Akkoyun
 *
 *	Library				: Battery Management Library
 *	Code Developer		: Mehmet Gunce Akkoyun (akkoyun@me.com)
 *********************************************************************************/

#include "MAX17055.h"

// Configuration Functions
bool MAX17055::Begin(const uint16_t _Design_Capacity, const float _Resistor) {

	// Set Configuration
	Set_Config(1, 0x0000);
	Set_Config(2, 0x0218);
	Set_HibCFG(0x0000);

	// Design Capacity Register
	Set_Design_Capacity(_Design_Capacity);

	// ModelCfg Register
	Set_ModelCfg(2, true);

	// VEmpty Register
	Set_Empty_Recovery_Voltage(3.0, 4.1);

	// Max Min Voltage Register
	Set_Max_Min_Voltage(3.0, 4.0);

	// IChgTerm Register
	Set_Charge_Termination_Current();

	// Set Resistor Variable
	_Sense_Resistor = _Resistor;

	// I2C Delay
	delay(5);
	
}

// Voltage Functions
float MAX17055::Instant_Voltage(void) {

	// Define Statistical Variables
	const uint8_t _Read_Count = 1;

	// Define Measurement Read Array
	float _Measurement_Array[_Read_Count];

	// Read Loop For Read Count
	for (uint8_t _Read_ID = 0; _Read_ID < _Read_Count; _Read_ID++) {

		// Define Data Variable
		uint8_t MAX17055_Data[2];

		// Get Data from IC
		I2C.Read_Multiple_Register(__I2C__MAX17055__Addr__, 0x09, MAX17055_Data, 2, false);

		// Combine Read Bytes
		uint16_t Measurement_Raw = ((uint16_t)MAX17055_Data[1] << 8) | (uint16_t)MAX17055_Data[0];

		// Calculate Measurement
		_Measurement_Array[_Read_ID] = ((double)Measurement_Raw * 1.25 / 16) / 1000;

		// Debug Terminal
		//Serial.println(_Measurement_Array[_Read_ID]);

	}

	// Construct Object
	Array_Stats<float> Data_Array(_Measurement_Array, _Read_Count);

	// End Function
	return(Data_Array.Arithmetic_Average());

}
float MAX17055::Average_Voltage(void) {

	// Define Statistical Variables
	const uint8_t _Read_Count = 1;

	// Define Measurement Read Array
	float _Measurement_Array[_Read_Count];

	// Read Loop For Read Count
	for (uint8_t _Read_ID = 0; _Read_ID < _Read_Count; _Read_ID++) {

		// Define Data Variable
		uint8_t MAX17055_Data[2];

		// Get Data from IC
		I2C.Read_Multiple_Register(__I2C__MAX17055__Addr__, 0x19, MAX17055_Data, 2, false);

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
float MAX17055::Empty_Voltage(void) {

	// Define Statistical Variables
	uint8_t _Read_Count = 1;

	// Define Measurement Read Array
	float _Measurement_Array[_Read_Count];

	// Read Loop For Read Count
	for (uint8_t _Read_ID = 0; _Read_ID < _Read_Count; _Read_ID++) {

		// Define Data Variable
		uint8_t MAX17055_Data[2];

		// Get Data from IC
		I2C.Read_Multiple_Register(__I2C__MAX17055__Addr__, 0x3A, MAX17055_Data, 2, false);

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
float MAX17055::Recovery_Voltage(void) {

	// Define Statistical Variables
	uint8_t _Read_Count = 1;

	// Define Measurement Read Array
	float _Measurement_Array[_Read_Count];

	// Read Loop For Read Count
	for (uint8_t _Read_ID = 0; _Read_ID < _Read_Count; _Read_ID++) {

		// Define Data Variable
		uint8_t MAX17055_Data[2];

		// Get Data from IC
		I2C.Read_Multiple_Register(__I2C__MAX17055__Addr__, 0x3A, MAX17055_Data, 2, false);

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
bool MAX17055::Set_Empty_Recovery_Voltage(const float _Empty_Voltage, const float _Recovery_Voltage) {

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
	bool _Result = I2C.Write_Multiple_Register(__I2C__MAX17055__Addr__, 0x3A, _Data, 2);

	// End Function
	return(_Result);

}
bool MAX17055::Set_Max_Min_Voltage(const float _Min_Voltage, const float _Max_Voltage) {

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
	bool _Result = I2C.Write_Multiple_Register(__I2C__MAX17055__Addr__, 0x3A, _Data, 2);

	// End Function
	return(_Result);

}

// Current Functions
float MAX17055::Instant_Current(void) {

	// Define Statistical Variables
	uint8_t _Read_Count = 5;

	// Define Measurement Read Array
	float _Measurement_Array[_Read_Count];

	// Read Loop For Read Count
	for (uint8_t _Read_ID = 0; _Read_ID < _Read_Count; _Read_ID++) {

		// Define Data Variable
		uint8_t MAX17055_Data[2];

		// Get Data from IC
		I2C.Read_Multiple_Register(__I2C__MAX17055__Addr__, 0x0A, MAX17055_Data, 2, false);

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
		_Measurement_Array[_Read_ID] = (((double)Measurement_Raw * 1.5625) / _Sense_Resistor) / 1000;
		
		// Assign Signiture
		if (_Signiture) _Measurement_Array[_Read_ID] *= -1;

	}
	
	// Construct Object
	Array_Stats<float> Data_Array(_Measurement_Array, _Read_Count);

	// End Function
	return(Data_Array.Arithmetic_Average());

}
float MAX17055::Average_Current(void) {

	// Define Statistical Variables
	uint8_t _Read_Count = 5;

	// Define Measurement Read Array
	float _Measurement_Array[_Read_Count];

	// Read Loop For Read Count
	for (uint8_t _Read_ID = 0; _Read_ID < _Read_Count; _Read_ID++) {

		// Define Data Variable
		uint8_t MAX17055_Data[2];

		// Get Data from IC
		I2C.Read_Multiple_Register(__I2C__MAX17055__Addr__, 0x0B, MAX17055_Data, 2, false);

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
		_Measurement_Array[_Read_ID] = (((double)Measurement_Raw * 1.5625) / _Sense_Resistor) / 1000;
		
		// Assign Signiture
		if (_Signiture) _Measurement_Array[_Read_ID] *= -1;

	}
	
	// Construct Object
	Array_Stats<float> Data_Array(_Measurement_Array, _Read_Count);

	// End Function
	return(Data_Array.Arithmetic_Average());

}
float MAX17055::Charge_Termination_Current(void) {

	// Define Statistical Variables
	uint8_t _Read_Count = 1;

	// Define Measurement Read Array
	float _Measurement_Array[_Read_Count];

	// Read Loop For Read Count
	for (uint8_t _Read_ID = 0; _Read_ID < _Read_Count; _Read_ID++) {

		// Define Data Variable
		uint8_t MAX17055_Data[2];

		// Get Data from IC
		I2C.Read_Multiple_Register(__I2C__MAX17055__Addr__, 0x1E, MAX17055_Data, 2, false);

		// Combine Read Bytes
		uint16_t Measurement_Raw = ((uint16_t)MAX17055_Data[1] << 8) | (uint16_t)MAX17055_Data[0];

		// Calculate Data
		_Measurement_Array[_Read_ID] = (((double)Measurement_Raw * 1.5625) / _Sense_Resistor) / 1000;
		
	}
	
	// Construct Object
	Array_Stats<float> Data_Array(_Measurement_Array, _Read_Count);

	// End Function
	return(Data_Array.Arithmetic_Average());

}
bool MAX17055::Set_Charge_Termination_Current(void) {

	// Set Empty Raw
	uint32_t _Raw_Termination_Voltage = 0x0280;

	// Declare Default Data Array
	uint8_t _Data[2];

	// Set Data Low/High Byte
	_Data[0] = ((_Raw_Termination_Voltage & (uint16_t)0x00FF));
	_Data[1] = ((_Raw_Termination_Voltage & (uint16_t)0xFF00) >> 8);

	// Set Register
	bool _Result = I2C.Write_Multiple_Register(__I2C__MAX17055__Addr__, 0x1E, _Data, 2);

	// End Function
	return(_Result);

}

// State of Charge Functions
float MAX17055::State_Of_Charge(void) {

	// Define Statistical Variables
	uint8_t _Read_Count = 1;

	// Define Measurement Read Array
	float _Measurement_Array[_Read_Count];

	// Read Loop For Read Count
	for (uint8_t _Read_ID = 0; _Read_ID < _Read_Count; _Read_ID++) {

		// Define Data Variable
		uint8_t MAX17055_Data[2];

		// Get Data from IC
		I2C.Read_Multiple_Register(__I2C__MAX17055__Addr__, 0x06, MAX17055_Data, 2, false);

		// Calculate Measurement
		_Measurement_Array[_Read_ID] = ((double)MAX17055_Data[1] + (double)MAX17055_Data[0] / 256);

	}
	
	// Construct Object
	Array_Stats<float> Data_Array(_Measurement_Array, _Read_Count);

	// End Function
	return(Data_Array.Arithmetic_Average());

}
float MAX17055::Average_State_Of_Charge(void) {

	// Define Statistical Variables
	uint8_t _Read_Count = 5;

	// Define Measurement Read Array
	float _Measurement_Array[_Read_Count];

	// Read Loop For Read Count
	for (uint8_t _Read_ID = 0; _Read_ID < _Read_Count; _Read_ID++) {

		// Define Data Variable
		uint8_t MAX17055_Data[2];

		// Get Data from IC
		I2C.Read_Multiple_Register(__I2C__MAX17055__Addr__, 0x0E, MAX17055_Data, 2, false);

		// Calculate Measurement
		_Measurement_Array[_Read_ID] = ((double)MAX17055_Data[1] + (double)MAX17055_Data[0] / 256);

	}
	
	// Construct Object
	Array_Stats<float> Data_Array(_Measurement_Array, _Read_Count);

	// End Function
	return(Data_Array.Arithmetic_Average());

}

// Capacity Functions
uint16_t MAX17055::Instant_Capacity(void) {

	// Define Data Variable
	uint8_t MAX17055_Data[2];

	// Get Data from IC
	I2C.Read_Multiple_Register(__I2C__MAX17055__Addr__, 0x05, MAX17055_Data, 2, false);

	// Combine Read Bytes
	uint16_t Measurement_Raw = ((uint16_t)MAX17055_Data[1] << 8) | (uint16_t)MAX17055_Data[0];

	// Calculate Data
	uint16_t _Measurement = Measurement_Raw * 5 / 1000 / _Sense_Resistor;

	// End Function
	return(_Measurement);

}
uint16_t MAX17055::Design_Capacity(void) {

	// Define Data Variable
	uint8_t MAX17055_Data[2];

	// Get Data from IC
	I2C.Read_Multiple_Register(__I2C__MAX17055__Addr__, 0x18, MAX17055_Data, 2, false);

	// Combine Read Bytes
	uint16_t Measurement_Raw = ((uint16_t)MAX17055_Data[1] << 8) | (uint16_t)MAX17055_Data[0];

	// Calculate Data
	uint16_t _Measurement = Measurement_Raw * 5 / 1000 / _Sense_Resistor;

	// End Function
	return(_Measurement);

}
uint16_t MAX17055::Full_Capacity(void) {

	// Define Data Variable
	uint8_t MAX17055_Data[2];

	// Get Data from IC
	I2C.Read_Multiple_Register(__I2C__MAX17055__Addr__, 0x35, MAX17055_Data, 2, false);

	// Combine Read Bytes
	uint16_t Measurement_Raw = ((uint16_t)MAX17055_Data[1] << 8) | (uint16_t)MAX17055_Data[0];

	// Calculate Data
	uint16_t _Measurement = Measurement_Raw * 5 / 1000 / _Sense_Resistor;

	// End Function
	return(_Measurement);

}
bool MAX17055::Set_Design_Capacity(const uint16_t _Capacity) {

	// Set Raw
	uint16_t _Raw_Cap = (uint16_t)_Capacity * 2;

	// Declare Default Data Array
	uint8_t _Data[2];

	// Set Data Low/High Byte
	_Data[0] = ((_Raw_Cap & (uint16_t)0x00FF));
	_Data[1] = ((_Raw_Cap & (uint16_t)0xFF00) >> 8);

	// Set Register
	bool _Result = I2C.Write_Multiple_Register(__I2C__MAX17055__Addr__, 0x18, _Data, 2);

	// End Function
	return(_Result);

}

// Temperature Functions
float MAX17055::Temperature(void) {

	// Define Statistical Variables
	uint8_t _Read_Count = 5;

	// Define Measurement Read Array
	float _Measurement_Array[_Read_Count];

	// Read Loop For Read Count
	for (size_t _Read_ID = 0; _Read_ID < _Read_Count; _Read_ID++) {

		// Define Data Variable
		uint8_t MAX17055_Data[2];

		// Get Data from IC
		I2C.Read_Multiple_Register(__I2C__MAX17055__Addr__, 0x08, MAX17055_Data, 2, false);

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

// Time Functions
uint16_t MAX17055::Time_To_Empty(void) {

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
		I2C.Read_Multiple_Register(__I2C__MAX17055__Addr__, 0x11, MAX17055_Data, 2, false);

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
uint16_t MAX17055::Time_To_Full(void) {

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
		I2C.Read_Multiple_Register(__I2C__MAX17055__Addr__, 0x20, MAX17055_Data, 2, false);

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

// Battery Properties Functions
uint16_t MAX17055::Battery_Age(void) {

	// Define Data Variable
	uint8_t MAX17055_Data[2];

	// Get Data from IC
	I2C.Read_Multiple_Register(__I2C__MAX17055__Addr__, 0x07, MAX17055_Data, 2, false);

	// Combine Read Bytes
	uint16_t Measurement_Raw = ((uint16_t)MAX17055_Data[1] << 8) | (uint16_t)MAX17055_Data[0];

	// End Function
	return(Measurement_Raw);

}
uint16_t MAX17055::Charge_Cycle(void) {

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
		I2C.Read_Multiple_Register(__I2C__MAX17055__Addr__, 0x17, MAX17055_Data, 2, false);

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

// Control Functions
bool MAX17055::is_Power_on_Reset(void) {
	
	// Get Status Bit
	bool _Result = I2C.Read_Register_Bit(__I2C__MAX17055__Addr__, 0x00, 0x01);

	// Reset Bit
	I2C.Clear_Register_Bit(__I2C__MAX17055__Addr__, 0x00, 0x01, false);

	// End Function
	return(_Result);
	
}
bool MAX17055::is_Min_Current(void) {
	
	// Get Status Bit
	bool _Result = I2C.Read_Register_Bit(__I2C__MAX17055__Addr__, 0x00, 0x02);

	// Reset Bit
	I2C.Clear_Register_Bit(__I2C__MAX17055__Addr__, 0x00, 0x02, false);

	// End Function
	return(_Result);
	
}
bool MAX17055::is_Max_Current(void) {
	
	// Get Status Bit
	bool _Result = I2C.Read_Register_Bit(__I2C__MAX17055__Addr__, 0x00, 0x06);

	// Reset Bit
	I2C.Clear_Register_Bit(__I2C__MAX17055__Addr__, 0x00, 0x06, false);

	// End Function
	return(_Result);
	
}
bool MAX17055::is_Min_Voltage(void) {
	
	// Get Status Bit
	bool _Result = I2C.Read_Register_Bit(__I2C__MAX17055__Addr__, 0x00, 0x08);

	// Reset Bit
	I2C.Clear_Register_Bit(__I2C__MAX17055__Addr__, 0x00, 0x08, false);

	// End Function
	return(_Result);
	
}
bool MAX17055::is_Max_Voltage(void) {
	
	// Get Status Bit
	bool _Result = I2C.Read_Register_Bit(__I2C__MAX17055__Addr__, 0x00, 0x12);

	// Reset Bit
	I2C.Clear_Register_Bit(__I2C__MAX17055__Addr__, 0x00, 0x12, false);

	// End Function
	return(_Result);
	
}
bool MAX17055::is_Min_Temperature(void) {
	
	// Get Status Bit
	bool _Result = I2C.Read_Register_Bit(__I2C__MAX17055__Addr__, 0x00, 0x09);

	// Reset Bit
	I2C.Clear_Register_Bit(__I2C__MAX17055__Addr__, 0x00, 0x09, false);

	// End Function
	return(_Result);
	
}
bool MAX17055::is_Max_Temperature(void) {
	
	// Get Status Bit
	bool _Result = I2C.Read_Register_Bit(__I2C__MAX17055__Addr__, 0x00, 0x13);

	// Reset Bit
	I2C.Clear_Register_Bit(__I2C__MAX17055__Addr__, 0x00, 0x13, false);

	// End Function
	return(_Result);
	
}
bool MAX17055::is_Min_SOC(void) {
	
	// Get Status Bit
	bool _Result = I2C.Read_Register_Bit(__I2C__MAX17055__Addr__, 0x00, 0x10);

	// Reset Bit
	I2C.Clear_Register_Bit(__I2C__MAX17055__Addr__, 0x00, 0x10, false);

	// End Function
	return(_Result);
	
}
bool MAX17055::is_Max_SOC(void) {
	
	// Get Status Bit
	bool _Result = I2C.Read_Register_Bit(__I2C__MAX17055__Addr__, 0x00, 0x14);

	// Reset Bit
	I2C.Clear_Register_Bit(__I2C__MAX17055__Addr__, 0x00, 0x14, false);

	// End Function
	return(_Result);
	
}
bool MAX17055::is_Battery_Present(void) {
	
	// Get Status Bit
	bool _Result = I2C.Read_Register_Bit(__I2C__MAX17055__Addr__, 0x00, 0x03);

	// Reset Bit
	I2C.Clear_Register_Bit(__I2C__MAX17055__Addr__, 0x00, 0x03, false);

	// End Function
	return(_Result);
	
}
bool MAX17055::is_SOC_Change(void) {
	
	// Get Status Bit
	bool _Result = I2C.Read_Register_Bit(__I2C__MAX17055__Addr__, 0x00, 0x07);

	// Reset Bit
	I2C.Clear_Register_Bit(__I2C__MAX17055__Addr__, 0x00, 0x07, false);

	// End Function
	return(_Result);
	
}
bool MAX17055::is_Battery_Insertion(void) {
	
	// Get Status Bit
	bool _Result = I2C.Read_Register_Bit(__I2C__MAX17055__Addr__, 0x00, 0x11);

	// Reset Bit
	I2C.Clear_Register_Bit(__I2C__MAX17055__Addr__, 0x00, 0x11, false);

	// End Function
	return(_Result);
	
}
bool MAX17055::is_Battery_Removal(void) {
	
	// Get Status Bit
	bool _Result = I2C.Read_Register_Bit(__I2C__MAX17055__Addr__, 0x00, 0x15);

	// Reset Bit
	I2C.Clear_Register_Bit(__I2C__MAX17055__Addr__, 0x00, 0x15, false);

	// End Function
	return(_Result);
	
}

// Config Functions
bool MAX17055::Set_dQAcc(const uint16_t _Capacity) {

	// Declare Default Data Array
	uint8_t _Data[2];

	// Set Data Low/High Byte
	_Data[0] = (((_Capacity / 32) & (uint16_t)0x00FF));
	_Data[1] = (((_Capacity / 32) & (uint16_t)0xFF00) >> 8);

	// Set Register
	bool _Result = I2C.Write_Multiple_Register(__I2C__MAX17055__Addr__, 0x45, _Data, 2);

	// End Function
	return(_Result);

}
bool MAX17055::Set_dPAcc(const uint16_t _Capacity) {

	// Declare dQAcc Variable
	uint8_t _dQAcc[2];

	// Get dQAcc Register
	I2C.Read_Multiple_Register(__I2C__MAX17055__Addr__, 0x45, _dQAcc, 2, false);

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
	bool _Result = I2C.Write_Multiple_Register(__I2C__MAX17055__Addr__, 0x46, _Data, 2);

	// End Function
	return(_Result);

}
bool MAX17055::Set_ModelCfg(const uint8_t _Model_ID, const bool _VChg) {

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
	bool _Result = I2C.Write_Multiple_Register(__I2C__MAX17055__Addr__, 0xDB, _Data, 2);

	// End Function
	return(_Result);

}
bool MAX17055::Set_HibCFG(const uint16_t _Config) {

	// Declare Default Data Array
	uint8_t _Data[2];

	// Set Data Low/High Byte
	_Data[0] = ((_Config & (uint16_t)0x00FF));
	_Data[1] = ((_Config & (uint16_t)0xFF00) >> 8);

	// Set Register
	bool _Result = I2C.Write_Multiple_Register(__I2C__MAX17055__Addr__, 0xBA, _Data, 2);

	// End Function
	return(_Result);

}
bool MAX17055::Set_Config(const uint8_t _Channel, const uint16_t _Config) {

	// Declare Default Data Array
	uint8_t _Data[2];

	// Declare Result Variable
	bool _Result;

	// Set Data Low/High Byte
	_Data[0] = ((_Config & (uint16_t)0x00FF));
	_Data[1] = ((_Config & (uint16_t)0xFF00) >> 8);

	// Config1 Setting
	if (_Channel == 1) _Result = I2C.Write_Multiple_Register(__I2C__MAX17055__Addr__, 0x1D, _Data, 2);
	if (_Channel == 2) _Result = I2C.Write_Multiple_Register(__I2C__MAX17055__Addr__, 0xBB, _Data, 2);

	// End Function
	return(_Result);

}

// Misc Functions
String MAX17055::Serial_ID(void) {

	// Declare Serial ID Variable
	String _Serial_ID = "";

	// Define Data Variable
	uint8_t MAX17055_Data[2];

	// Get Data from IC
	memset(MAX17055_Data, '\0', 2);
	I2C.Read_Multiple_Register(__I2C__MAX17055__Addr__, 0xDF, MAX17055_Data, 2, false);
	uint16_t Measurement_WORD_7 = ((uint16_t)MAX17055_Data[1] << 8) | (uint16_t)MAX17055_Data[0];

	// Get Data from IC
	memset(MAX17055_Data, '\0', 2);
	I2C.Read_Multiple_Register(__I2C__MAX17055__Addr__, 0xDE, MAX17055_Data, 2, false);
	uint16_t Measurement_WORD_6 = ((uint16_t)MAX17055_Data[1] << 8) | (uint16_t)MAX17055_Data[0];

	// Get Data from IC
	memset(MAX17055_Data, '\0', 2);
	I2C.Read_Multiple_Register(__I2C__MAX17055__Addr__, 0xDD, MAX17055_Data, 2, false);
	uint16_t Measurement_WORD_5 = ((uint16_t)MAX17055_Data[1] << 8) | (uint16_t)MAX17055_Data[0];

	// Get Data from IC
	memset(MAX17055_Data, '\0', 2);
	I2C.Read_Multiple_Register(__I2C__MAX17055__Addr__, 0xDC, MAX17055_Data, 2, false);
	uint16_t Measurement_WORD_4 = ((uint16_t)MAX17055_Data[1] << 8) | (uint16_t)MAX17055_Data[0];

	// Get Data from IC
	memset(MAX17055_Data, '\0', 2);
	I2C.Read_Multiple_Register(__I2C__MAX17055__Addr__, 0xDA, MAX17055_Data, 2, false);
	uint16_t Measurement_WORD_3 = ((uint16_t)MAX17055_Data[1] << 8) | (uint16_t)MAX17055_Data[0];

	// Get Data from IC
	memset(MAX17055_Data, '\0', 2);
	I2C.Read_Multiple_Register(__I2C__MAX17055__Addr__, 0xD9, MAX17055_Data, 2, false);
	uint16_t Measurement_WORD_2 = ((uint16_t)MAX17055_Data[1] << 8) | (uint16_t)MAX17055_Data[0];

	// Get Data from IC
	memset(MAX17055_Data, '\0', 2);
	I2C.Read_Multiple_Register(__I2C__MAX17055__Addr__, 0xD5, MAX17055_Data, 2, false);
	uint16_t Measurement_WORD_1 = ((uint16_t)MAX17055_Data[1] << 8) | (uint16_t)MAX17055_Data[0];

	// Get Data from IC
	memset(MAX17055_Data, '\0', 2);
	I2C.Read_Multiple_Register(__I2C__MAX17055__Addr__, 0xD4, MAX17055_Data, 2, false);
	uint16_t Measurement_WORD_0 = ((uint16_t)MAX17055_Data[1] << 8) | (uint16_t)MAX17055_Data[0];

	// Combine Variable 7
	if (Measurement_WORD_7 < 0x1000) _Serial_ID += String("0");
	if (Measurement_WORD_7 < 0x0100) _Serial_ID += String("0");
	if (Measurement_WORD_7 < 0x0010) _Serial_ID += String("0");
	_Serial_ID += String(Measurement_WORD_7, HEX);
	
	// Combine Variable 6
	if (Measurement_WORD_6 < 0x1000) _Serial_ID += String("0");
	if (Measurement_WORD_6 < 0x0100) _Serial_ID += String("0");
	if (Measurement_WORD_6 < 0x0010) _Serial_ID += String("0");
	_Serial_ID += String(Measurement_WORD_6, HEX);

	// Combine Variable 5
	if (Measurement_WORD_5 < 0x1000) _Serial_ID += String("0");
	if (Measurement_WORD_5 < 0x0100) _Serial_ID += String("0");
	if (Measurement_WORD_5 < 0x0010) _Serial_ID += String("0");
	_Serial_ID += String(Measurement_WORD_5, HEX);

	// Combine Variable 4
	if (Measurement_WORD_4 < 0x1000) _Serial_ID += String("0");
	if (Measurement_WORD_4 < 0x0100) _Serial_ID += String("0");
	if (Measurement_WORD_4 < 0x0010) _Serial_ID += String("0");
	_Serial_ID += String(Measurement_WORD_4, HEX);

	// Combine Variable 3
	if (Measurement_WORD_3 < 0x1000) _Serial_ID += String("0");
	if (Measurement_WORD_3 < 0x0100) _Serial_ID += String("0");
	if (Measurement_WORD_3 < 0x0010) _Serial_ID += String("0");
	_Serial_ID += String(Measurement_WORD_3, HEX);

	// Combine Variable 2
	if (Measurement_WORD_2 < 0x1000) _Serial_ID += String("0");
	if (Measurement_WORD_2 < 0x0100) _Serial_ID += String("0");
	if (Measurement_WORD_2 < 0x0010) _Serial_ID += String("0");
	_Serial_ID += String(Measurement_WORD_2, HEX);

	// Combine Variable 1
	if (Measurement_WORD_1 < 0x1000) _Serial_ID += String("0");
	if (Measurement_WORD_1 < 0x0100) _Serial_ID += String("0");
	if (Measurement_WORD_1 < 0x0010) _Serial_ID += String("0");
	_Serial_ID += String(Measurement_WORD_1, HEX);

	// Combine Variable 0
	if (Measurement_WORD_0 < 0x1000) _Serial_ID += String("0");
	if (Measurement_WORD_0 < 0x0100) _Serial_ID += String("0");
	if (Measurement_WORD_0 < 0x0010) _Serial_ID += String("0");
	_Serial_ID += String(Measurement_WORD_0, HEX);

	// Capital
	_Serial_ID.toUpperCase();

	// End Function
	return(_Serial_ID);

}

MAX17055 Battery;
