/* *******************************************************************************
 *  Copyright (C) 2014-2019 Mehmet Gunce Akkoyun Can not be copied and/or
 *	distributed without the express permission of Mehmet Gunce Akkoyun
 *
 *	Library				: Battery Management Library
 *	Code Developer		: Mehmet Gunce Akkoyun (akkoyun@me.com)
 *********************************************************************************/

#include "MAX17055.h"

// Configuration Functions
bool MAX17055::Begin(uint16_t _Design_Capacity, float _Resistor) {

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
	Set_Empty_Recovery_Voltage(3.0, 4.0);

	// IChgTerm Register
	Set_Charge_Termination_Current();

	// Set Resistor Variable
	_Sense_Resistor = _Resistor;

	// I2C Delay
	delay(5);
	
}

// Voltage Functions
float MAX17055::Instant_Voltage(void) {

	// Define Statistical Objects
	Statistical _DataSet;

	// Define Statistical Variables
	uint8_t _Read_Count = 1;
	uint8_t _Average_Type = 1;

	// Define Measurement Read Array
	float _Measurement_Array[_Read_Count];

	// Read Loop For Read Count
	for (uint8_t _Read_ID = 0; _Read_ID < _Read_Count; _Read_ID++) {

		// Define Data Variable
		uint8_t MAX17055_Data[2];

		// Get Data from IC
		I2C.Read_Multiple_Register(I2C.MAX17055.I2C_Address, __MAX17055_VCell__, MAX17055_Data, 2, false);

		// Combine Read Bytes
		uint16_t Measurement_Raw = ((uint16_t)MAX17055_Data[1] << 8) | (uint16_t)MAX17055_Data[0];

		// Calculate Measurement
		_Measurement_Array[_Read_ID] = ((double)Measurement_Raw * 1.25 / 16) / 1000;

	}
	
	// Calculate Data
	float _Average = _DataSet.Array_Average(_Measurement_Array, _Read_Count, _Average_Type);

	// End Function
	return(_Average);

}
float MAX17055::Average_Voltage(void) {

	// Define Statistical Objects
	Statistical _DataSet;

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
		I2C.Read_Multiple_Register(I2C.MAX17055.I2C_Address, __MAX17055_AvgVCell__, MAX17055_Data, 2, false);

		// Combine Read Bytes
		uint16_t Measurement_Raw = ((uint16_t)MAX17055_Data[1] << 8) | (uint16_t)MAX17055_Data[0];

		// Calculate Measurement
		_Measurement_Array[_Read_ID] = ((double)Measurement_Raw * 1.25 / 16) / 1000;

	}
	
	// Calculate Data
	float _Average = _DataSet.Array_Average(_Measurement_Array, _Read_Count, _Average_Type);

	// End Function
	return(_Average);

}
float MAX17055::Empty_Voltage(void) {

	// Define Statistical Objects
	Statistical _DataSet;

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
		I2C.Read_Multiple_Register(I2C.MAX17055.I2C_Address, __MAX17055_VEmpty__, MAX17055_Data, 2, false);

		// Combine Read Bytes
		uint16_t Measurement_Raw = ((uint16_t)MAX17055_Data[1] << 8) | (uint16_t)MAX17055_Data[0];
		Measurement_Raw = ((Measurement_Raw & 0xFF80) >> 7);

		// Calculate Measurement
		_Measurement_Array[_Read_ID] = ((double)Measurement_Raw * 10) / 1000;

	}
	
	// Calculate Data
	float _Average = _DataSet.Array_Average(_Measurement_Array, _Read_Count, _Average_Type);

	// End Function
	return(_Average);

}
float MAX17055::Recovery_Voltage(void) {

	// Define Statistical Objects
	Statistical _DataSet;

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
		I2C.Read_Multiple_Register(I2C.MAX17055.I2C_Address, __MAX17055_VEmpty__, MAX17055_Data, 2, false);

		// Combine Read Bytes
		uint16_t Measurement_Raw = ((uint16_t)MAX17055_Data[1] << 8) | (uint16_t)MAX17055_Data[0];
		Measurement_Raw = (Measurement_Raw & 0x7F);

		// Calculate Measurement
		_Measurement_Array[_Read_ID] = ((double)Measurement_Raw * 40) / 1000;

	}
	
	// Calculate Data
	float _Average = _DataSet.Array_Average(_Measurement_Array, _Read_Count, _Average_Type);

	// End Function
	return(_Average);

}
bool MAX17055::Set_Empty_Recovery_Voltage(float _Empty_Voltage, float _Recovery_Voltage) {

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
	bool _Result = I2C.Write_Multiple_Register(I2C.MAX17055.I2C_Address, __MAX17055_VEmpty__, _Data, 2);

	// End Function
	return(_Result);

}
bool MAX17055::Set_Max_Min_Voltage(float _Max_Voltage, float _Min_Voltage) {

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
	bool _Result = I2C.Write_Multiple_Register(I2C.MAX17055.I2C_Address, __MAX17055_VEmpty__, _Data, 2);

	// End Function
	return(_Result);

}

// Current Functions
float MAX17055::Instant_Current(void) {

	// Define Statistical Objects
	Statistical _DataSet;

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
		I2C.Read_Multiple_Register(I2C.MAX17055.I2C_Address, __MAX17055_Current__, MAX17055_Data, 2, false);

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
	
	// Calculate Data
	float _Average = _DataSet.Array_Average(_Measurement_Array, _Read_Count, _Average_Type);

	// End Function
	return(_Average);

}
float MAX17055::Average_Current(void) {

	// Define Statistical Objects
	Statistical _DataSet;

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
		I2C.Read_Multiple_Register(I2C.MAX17055.I2C_Address, __MAX17055_AvgCurrent__, MAX17055_Data, 2, false);

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
	
	// Calculate Data
	float _Average = _DataSet.Array_Average(_Measurement_Array, _Read_Count, _Average_Type);

	// End Function
	return(_Average);

}
float MAX17055::Charge_Termination_Current(void) {

	// Define Statistical Objects
	Statistical _DataSet;

	// Define Statistical Variables
	uint8_t _Read_Count = 1;
	uint8_t _Average_Type = 1;

	// Define Measurement Read Array
	float _Measurement_Array[_Read_Count];

	// Read Loop For Read Count
	for (uint8_t _Read_ID = 0; _Read_ID < _Read_Count; _Read_ID++) {

		// Define Data Variable
		uint8_t MAX17055_Data[2];

		// Get Data from IC
		I2C.Read_Multiple_Register(I2C.MAX17055.I2C_Address, __MAX17055_IChgTerm__, MAX17055_Data, 2, false);

		// Combine Read Bytes
		uint16_t Measurement_Raw = ((uint16_t)MAX17055_Data[1] << 8) | (uint16_t)MAX17055_Data[0];

		// Calculate Data
		_Measurement_Array[_Read_ID] = (((double)Measurement_Raw * 1.5625) / _Sense_Resistor) / 1000;
		
	}
	
	// Calculate Data
	float _Average = _DataSet.Array_Average(_Measurement_Array, _Read_Count, _Average_Type);

	// End Function
	return(_Average);

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
	bool _Result = I2C.Write_Multiple_Register(I2C.MAX17055.I2C_Address, __MAX17055_IChgTerm__, _Data, 2);

	// End Function
	return(_Result);

}

// State of Charge Functions
float MAX17055::State_Of_Charge(void) {

	// Define Statistical Objects
	Statistical _DataSet;

	// Define Statistical Variables
	uint8_t _Read_Count = 1;
	uint8_t _Average_Type = 1;

	// Define Measurement Read Array
	float _Measurement_Array[_Read_Count];

	// Read Loop For Read Count
	for (uint8_t _Read_ID = 0; _Read_ID < _Read_Count; _Read_ID++) {

		// Define Data Variable
		uint8_t MAX17055_Data[2];

		// Get Data from IC
		I2C.Read_Multiple_Register(I2C.MAX17055.I2C_Address, __MAX17055_RepSOC__, MAX17055_Data, 2, false);

		// Calculate Measurement
		_Measurement_Array[_Read_ID] = ((double)MAX17055_Data[1] + (double)MAX17055_Data[0] / 256);

	}
	
	// Calculate Data
	float _Average = _DataSet.Array_Average(_Measurement_Array, _Read_Count, _Average_Type);

	// End Function
	return(_Average);

}
float MAX17055::Average_State_Of_Charge(void) {

	// Define Statistical Objects
	Statistical _DataSet;

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
		I2C.Read_Multiple_Register(I2C.MAX17055.I2C_Address, __MAX17055_AvSOC__, MAX17055_Data, 2, false);

		// Calculate Measurement
		_Measurement_Array[_Read_ID] = ((double)MAX17055_Data[1] + (double)MAX17055_Data[0] / 256);

	}
	
	// Calculate Data
	float _Average = _DataSet.Array_Average(_Measurement_Array, _Read_Count, _Average_Type);

	// End Function
	return(_Average);

}

// Capacity Functions
uint16_t MAX17055::Instant_Capacity(void) {

	// Define Data Variable
	uint8_t MAX17055_Data[2];

	// Get Data from IC
	I2C.Read_Multiple_Register(I2C.MAX17055.I2C_Address, __MAX17055_RepCap__, MAX17055_Data, 2, false);

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
	I2C.Read_Multiple_Register(I2C.MAX17055.I2C_Address, __MAX17055_DesignCap__, MAX17055_Data, 2, false);

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
	I2C.Read_Multiple_Register(I2C.MAX17055.I2C_Address, __MAX17055_FullCap__, MAX17055_Data, 2, false);

	// Combine Read Bytes
	uint16_t Measurement_Raw = ((uint16_t)MAX17055_Data[1] << 8) | (uint16_t)MAX17055_Data[0];

	// Calculate Data
	uint16_t _Measurement = Measurement_Raw * 5 / 1000 / _Sense_Resistor;

	// End Function
	return(_Measurement);

}
bool MAX17055::Set_Design_Capacity(uint16_t _Capacity) {

	// Set Raw
	uint16_t _Raw_Cap = (uint16_t)_Capacity * 2;

	// Declare Default Data Array
	uint8_t _Data[2];

	// Set Data Low/High Byte
	_Data[0] = ((_Raw_Cap & (uint16_t)0x00FF));
	_Data[1] = ((_Raw_Cap & (uint16_t)0xFF00) >> 8);

	// Set Register
	bool _Result = I2C.Write_Multiple_Register(I2C.MAX17055.I2C_Address, __MAX17055_DesignCap__, _Data, 2);

	// End Function
	return(_Result);

}

// Temperature Functions
float MAX17055::Temperature(void) {

	// Define Statistical Objects
	Statistical _DataSet;

	// Define Statistical Variables
	uint8_t _Read_Count = 1;
	uint8_t _Average_Type = 1;

	// Define Measurement Read Array
	float _Measurement_Array[_Read_Count];

	// Read Loop For Read Count
	for (uint8_t _Read_ID = 0; _Read_ID < _Read_Count; _Read_ID++) {

		// Define Data Variable
		uint8_t MAX17055_Data[2];

		// Get Data from IC
		I2C.Read_Multiple_Register(I2C.MAX17055.I2C_Address, __MAX17055_Temp__, MAX17055_Data, 2, false);

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

		// Command Delay
		delay(10);

	}
	
	// Calculate Data
	float _Average = _DataSet.Array_Average(_Measurement_Array, _Read_Count, _Average_Type);

	// End Function
	return(_Average);

}

// Time Functions
uint16_t MAX17055::Time_To_Empty(void) {

	// Define Statistical Objects
	Statistical _DataSet;

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
		I2C.Read_Multiple_Register(I2C.MAX17055.I2C_Address, __MAX17055_TTE__, MAX17055_Data, 2, false);

		// Combine Read Bytes
		uint16_t Measurement_Raw = ((uint16_t)MAX17055_Data[1] << 8) | (uint16_t)MAX17055_Data[0];

		// Calculate Data
		_Measurement_Array[_Read_ID] = (uint16_t)Measurement_Raw * 5.625 / 60 / 60;

	}
	
	// Calculate Data
	uint16_t _Average = _DataSet.Array_Average(_Measurement_Array, _Read_Count, _Average_Type);

	// End Function
	return(_Average);

}
uint16_t MAX17055::Time_To_Full(void) {

	// Define Statistical Objects
	Statistical _DataSet;

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
		I2C.Read_Multiple_Register(I2C.MAX17055.I2C_Address, __MAX17055_TTF__, MAX17055_Data, 2, false);

		// Combine Read Bytes
		uint16_t Measurement_Raw = ((uint16_t)MAX17055_Data[1] << 8) | (uint16_t)MAX17055_Data[0];

		// Calculate Data
		_Measurement_Array[_Read_ID] = (uint16_t)Measurement_Raw * 5.625 / 60 / 60;

	}
	
	// Calculate Data
	uint16_t _Average = _DataSet.Array_Average(_Measurement_Array, _Read_Count, _Average_Type);

	// End Function
	return(_Average);

}

// Battery Properties Functions
uint16_t MAX17055::Battery_Age(void) {

	// Define Data Variable
	uint8_t MAX17055_Data[2];

	// Get Data from IC
	I2C.Read_Multiple_Register(I2C.MAX17055.I2C_Address, __MAX17055_Age__, MAX17055_Data, 2, false);

	// Combine Read Bytes
	uint16_t Measurement_Raw = ((uint16_t)MAX17055_Data[1] << 8) | (uint16_t)MAX17055_Data[0];

	// End Function
	return(Measurement_Raw);

}
uint16_t MAX17055::Charge_Cycle(void) {

	// Define Statistical Objects
	Statistical _DataSet;

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
		I2C.Read_Multiple_Register(I2C.MAX17055.I2C_Address, __MAX17055_Cycles__, MAX17055_Data, 2, false);

		// Combine Read Bytes
		uint16_t Measurement_Raw = ((uint16_t)MAX17055_Data[1] << 8) | (uint16_t)MAX17055_Data[0];

		// Calculate Data
		_Measurement_Array[_Read_ID] = (uint16_t)Measurement_Raw;

	}
	
	// Calculate Data
	uint16_t _Average = _DataSet.Array_Average(_Measurement_Array, _Read_Count, _Average_Type);

	// End Function
	return(_Average);

}

// Control Functions
bool MAX17055::is_Power_on_Reset(void) {
	
	// Get Status Bit
	bool _Result = I2C.Read_Register_Bit(I2C.MAX17055.I2C_Address, __MAX17055_Status__, __MAX17055_POR__);

	// Reset Bit
	I2C.Clear_Register_Bit(I2C.MAX17055.I2C_Address, __MAX17055_Status__, __MAX17055_POR__, false);

	// End Function
	return(_Result);
	
}
bool MAX17055::is_Min_Current(void) {
	
	// Get Status Bit
	bool _Result = I2C.Read_Register_Bit(I2C.MAX17055.I2C_Address, __MAX17055_Status__, __MAX17055_Imn__);

	// End Function
	return(_Result);
	
}
bool MAX17055::is_Max_Current(void) {
	
	// Get Status Bit
	bool _Result = I2C.Read_Register_Bit(I2C.MAX17055.I2C_Address, __MAX17055_Status__, __MAX17055_Imx__);

	// End Function
	return(_Result);
	
}
bool MAX17055::is_Min_Voltage(void) {
	
	// Get Status Bit
	bool _Result = I2C.Read_Register_Bit(I2C.MAX17055.I2C_Address, __MAX17055_Status__, __MAX17055_Vmn__);

	// End Function
	return(_Result);
	
}
bool MAX17055::is_Max_Voltage(void) {
	
	// Get Status Bit
	bool _Result = I2C.Read_Register_Bit(I2C.MAX17055.I2C_Address, __MAX17055_Status__, __MAX17055_Vmx__);

	// End Function
	return(_Result);
	
}
bool MAX17055::is_Min_Temperature(void) {
	
	// Get Status Bit
	bool _Result = I2C.Read_Register_Bit(I2C.MAX17055.I2C_Address, __MAX17055_Status__, __MAX17055_Tmn__);

	// End Function
	return(_Result);
	
}
bool MAX17055::is_Max_Temperature(void) {
	
	// Get Status Bit
	bool _Result = I2C.Read_Register_Bit(I2C.MAX17055.I2C_Address, __MAX17055_Status__, __MAX17055_Tmx__);

	// End Function
	return(_Result);
	
}
bool MAX17055::is_Min_SOC(void) {
	
	// Get Status Bit
	bool _Result = I2C.Read_Register_Bit(I2C.MAX17055.I2C_Address, __MAX17055_Status__, __MAX17055_Smn__);

	// End Function
	return(_Result);
	
}
bool MAX17055::is_Max_SOC(void) {
	
	// Get Status Bit
	bool _Result = I2C.Read_Register_Bit(I2C.MAX17055.I2C_Address, __MAX17055_Status__, __MAX17055_Smx__);

	// End Function
	return(_Result);
	
}
bool MAX17055::is_Battery_Present(void) {
	
	// Get Status Bit
	bool _Result = I2C.Read_Register_Bit(I2C.MAX17055.I2C_Address, __MAX17055_Status__, __MAX17055_Bst__);

	// End Function
	return(_Result);
	
}
bool MAX17055::is_SOC_Change(void) {
	
	// Get Status Bit
	bool _Result = I2C.Read_Register_Bit(I2C.MAX17055.I2C_Address, __MAX17055_Status__, __MAX17055_dSOCi__);

	// End Function
	return(_Result);
	
}
bool MAX17055::is_Battery_Insertion(void) {
	
	// Get Status Bit
	bool _Result = I2C.Read_Register_Bit(I2C.MAX17055.I2C_Address, __MAX17055_Status__, __MAX17055_Bi__);

	// End Function
	return(_Result);
	
}
bool MAX17055::is_Battery_Removal(void) {
	
	// Get Status Bit
	bool _Result = I2C.Read_Register_Bit(I2C.MAX17055.I2C_Address, __MAX17055_Status__, __MAX17055_Br__);

	// End Function
	return(_Result);
	
}

// Config Functions
bool MAX17055::Set_dQAcc(uint16_t _Capacity) {

	// Handle Capacity
	_Capacity /= 32;

	// Declare Default Data Array
	uint8_t _Data[2];

	// Set Data Low/High Byte
	_Data[0] = ((_Capacity & (uint16_t)0x00FF));
	_Data[1] = ((_Capacity & (uint16_t)0xFF00) >> 8);

	// Set Register
	bool _Result = I2C.Write_Multiple_Register(I2C.MAX17055.I2C_Address, __MAX17055_dQAcc__, _Data, 2);

	// End Function
	return(_Result);

}
bool MAX17055::Set_dPAcc(uint16_t _Capacity) {

	// Declare dQAcc Variable
	uint8_t _dQAcc[2];

	// Get dQAcc Register
	I2C.Read_Multiple_Register(I2C.MAX17055.I2C_Address, __MAX17055_dQAcc__, _dQAcc, 2, false);

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
	bool _Result = I2C.Write_Multiple_Register(I2C.MAX17055.I2C_Address, __MAX17055_dPAcc__, _Data, 2);

	// End Function
	return(_Result);

}
bool MAX17055::Set_ModelCfg(uint8_t _Model_ID, bool _VChg) {

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
	bool _Result = I2C.Write_Multiple_Register(I2C.MAX17055.I2C_Address, __MAX17055_ModelCfg__, _Data, 2);

	// End Function
	return(_Result);

}
bool MAX17055::Set_HibCFG(uint16_t _Config) {

	// Declare Default Data Array
	uint8_t _Data[2];

	// Set Data Low/High Byte
	_Data[0] = ((_Config & (uint16_t)0x00FF));
	_Data[1] = ((_Config & (uint16_t)0xFF00) >> 8);

	// Set Register
	bool _Result = I2C.Write_Multiple_Register(I2C.MAX17055.I2C_Address, __MAX17055_HibCfg__, _Data, 2);

	// End Function
	return(_Result);

}
bool MAX17055::Set_Config(uint8_t _Channel, uint16_t _Config) {

	// Declare Default Data Array
	uint8_t _Data[2];

	// Declare Result Variable
	bool _Result;

	// Set Data Low/High Byte
	_Data[0] = ((_Config & (uint16_t)0x00FF));
	_Data[1] = ((_Config & (uint16_t)0xFF00) >> 8);

	// Config1 Setting
	if (_Channel == 1) _Result = I2C.Write_Multiple_Register(I2C.MAX17055.I2C_Address, __MAX17055_Config__, _Data, 2);
	if (_Channel == 2) _Result = I2C.Write_Multiple_Register(I2C.MAX17055.I2C_Address, __MAX17055_Config2__, _Data, 2);

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
	I2C.Read_Multiple_Register(I2C.MAX17055.I2C_Address, 0xDF, MAX17055_Data, 2, false);
	uint16_t Measurement_WORD_7 = ((uint16_t)MAX17055_Data[1] << 8) | (uint16_t)MAX17055_Data[0];

	// Get Data from IC
	memset(MAX17055_Data, '\0', 2);
	I2C.Read_Multiple_Register(I2C.MAX17055.I2C_Address, 0xDE, MAX17055_Data, 2, false);
	uint16_t Measurement_WORD_6 = ((uint16_t)MAX17055_Data[1] << 8) | (uint16_t)MAX17055_Data[0];

	// Get Data from IC
	memset(MAX17055_Data, '\0', 2);
	I2C.Read_Multiple_Register(I2C.MAX17055.I2C_Address, 0xDD, MAX17055_Data, 2, false);
	uint16_t Measurement_WORD_5 = ((uint16_t)MAX17055_Data[1] << 8) | (uint16_t)MAX17055_Data[0];

	// Get Data from IC
	memset(MAX17055_Data, '\0', 2);
	I2C.Read_Multiple_Register(I2C.MAX17055.I2C_Address, 0xDC, MAX17055_Data, 2, false);
	uint16_t Measurement_WORD_4 = ((uint16_t)MAX17055_Data[1] << 8) | (uint16_t)MAX17055_Data[0];

	// Get Data from IC
	memset(MAX17055_Data, '\0', 2);
	I2C.Read_Multiple_Register(I2C.MAX17055.I2C_Address, 0xDA, MAX17055_Data, 2, false);
	uint16_t Measurement_WORD_3 = ((uint16_t)MAX17055_Data[1] << 8) | (uint16_t)MAX17055_Data[0];

	// Get Data from IC
	memset(MAX17055_Data, '\0', 2);
	I2C.Read_Multiple_Register(I2C.MAX17055.I2C_Address, 0xD9, MAX17055_Data, 2, false);
	uint16_t Measurement_WORD_2 = ((uint16_t)MAX17055_Data[1] << 8) | (uint16_t)MAX17055_Data[0];

	// Get Data from IC
	memset(MAX17055_Data, '\0', 2);
	I2C.Read_Multiple_Register(I2C.MAX17055.I2C_Address, 0xD5, MAX17055_Data, 2, false);
	uint16_t Measurement_WORD_1 = ((uint16_t)MAX17055_Data[1] << 8) | (uint16_t)MAX17055_Data[0];

	// Get Data from IC
	memset(MAX17055_Data, '\0', 2);
	I2C.Read_Multiple_Register(I2C.MAX17055.I2C_Address, 0xD4, MAX17055_Data, 2, false);
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
