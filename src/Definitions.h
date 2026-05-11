#define MAX17055_POR			0
#define MAX17055_IMin			1
#define MAX17055_IMax			2
#define MAX17055_VMin			3
#define MAX17055_VMax			4
#define MAX17055_TMin			5
#define MAX17055_TMax			6
#define MAX17055_SOC_Min		7
#define MAX17055_SOC_Max		8
#define MAX17055_SOC_Change		9
#define MAX17055_Bat_Status		10
#define MAX17055_Bat_Insert		11
#define MAX17055_Bat_Remove		12

// =====================================================================
// Error Code Enumerations (for detailed diagnostics)
// =====================================================================

namespace MAX17055_Error {
	enum ErrorCode_t : uint8_t {
		// Success codes
		SUCCESS					= 0x00,		// Operation successful
		
		// I2C errors
		I2C_ERROR				= 0x10,		// General I2C bus error
		I2C_TIMEOUT				= 0x11,		// I2C communication timeout
		I2C_NACK				= 0x12,		// I2C NACK received
		I2C_BUS_BUSY			= 0x13,		// I2C bus is busy
		I2C_DEVICE_NOT_DETECTED	= 0x14,		// Device not found on bus
		
		// Device errors
		DEVICE_NOT_READY		= 0x20,		// Device not initialized
		DEVICE_NOT_DETECTED		= 0x21,		// Device not detected
		INVALID_REGISTER		= 0x22,		// Invalid register address
		
		// Parameter errors
		INVALID_PARAMETER		= 0x30,		// Invalid function parameter
		INVALID_VOLTAGE			= 0x31,		// Voltage out of valid range
		INVALID_CURRENT			= 0x32,		// Current out of valid range
		INVALID_TEMPERATURE		= 0x33,		// Temperature out of range
		INVALID_CAPACITY		= 0x34,		// Capacity out of range
		INVALID_SOC				= 0x35,		// SOC out of valid range
		
		// Data errors
		INVALID_DATA			= 0x40,		// Invalid data received
		DATA_CHECKSUM_ERROR		= 0x41,		// CRC/Checksum mismatch
		DATA_CORRUPTION			= 0x42,		// Data corruption detected
		
		// System errors
		BUFFER_OVERFLOW			= 0x50,		// Buffer overflow detected
		MEMORY_ERROR			= 0x51,		// Memory allocation error
		SYSTEM_ERROR			= 0x52,		// General system error
		
		// Generic error
		UNKNOWN_ERROR			= 0xFF		// Unknown error
	};
	
	// Error message lookup
	const char* Get_Error_Message(ErrorCode_t code) {
		switch (code) {
			case SUCCESS:				return "Success";
			case I2C_ERROR:				return "I2C bus error";
			case I2C_TIMEOUT:			return "I2C timeout";
			case I2C_NACK:				return "I2C NACK";
			case I2C_BUS_BUSY:			return "I2C bus busy";
			case I2C_DEVICE_NOT_DETECTED:	return "Device not detected";
			case DEVICE_NOT_READY:		return "Device not ready";
			case INVALID_PARAMETER:		return "Invalid parameter";
			case INVALID_VOLTAGE:		return "Invalid voltage";
			case DATA_CHECKSUM_ERROR:	return "Checksum error";
			default:					return "Unknown error";
		}
	}
}

// =====================================================================
// Status Bit Enumerations & Definitions
// =====================================================================

namespace MAX17055_Status {
	enum Status_t : uint8_t {
		POR			= 0,	// Power-On-Reset
		IMin		= 1,	// Minimum Current Alert
		IMax		= 2,	// Maximum Current Alert
		VMin		= 3,	// Minimum Voltage Alert
		VMax		= 4,	// Maximum Voltage Alert
		TMin		= 5,	// Minimum Temperature Alert
		TMax		= 6,	// Maximum Temperature Alert
		SOC_Min		= 7,	// Minimum SOC Alert
		SOC_Max		= 8,	// Maximum SOC Alert
		SOC_Change	= 9,	// SOC Change Alert
		Bat_Status	= 10,	// Battery Status
		Bat_Insert	= 11,	// Battery Insertion
		Bat_Remove	= 12	// Battery Removal
	};
}

// =====================================================================
// Configuration Bit Enumerations
// =====================================================================

namespace MAX17055_ConfigBits {
	// Config1 Register Bits
	enum Config1_t : uint8_t {
		Ber		= 0,	// Battery Removal Enable
		Bei		= 1,	// Battery Insertion Enable
		Aen		= 2,	// Alert Enable
		FTHRM	= 3,	// Fast Thermistor
		ETHRM	= 4,	// External Thermistor
		COMMSH	= 6,	// Communication Shutdown
		SHDN	= 7		// Shutdown
	};
	
	// Config1 High Byte Bits
	enum Config1_H_t : uint8_t {
		Tex		= 0,	// External Temperature
		Ten		= 1,	// Temperature Enable
		AINSH	= 2,	// AIN Shutdown
		IS		= 3,	// Current Software
		VS		= 4,	// Voltage Software
		TS		= 5,	// Temperature Software
		SS		= 6,	// SOC Software
		TSel	= 7		// Temperature Select
	};
	
	// Config2 Register Bits
	enum Config2_t : uint8_t {
		CPMode		= 1,	// Constant Power Mode
		LDMDL		= 5,	// Load Model
		TAIrtEN		= 6,	// Temperature Alert
		dSOCen		= 7		// dSOC Enable
	};
	
	// Config2 High Byte Bits
	enum Config2_H_t : uint8_t {
		DPEn		= 4,	// Dynamic Power Enable
		AtRateEn	= 5		// AtRate Enable
	};
}
