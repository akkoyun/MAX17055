// =====================================================================
// Backward Compatibility: Define Macros (DEPRECATED - use namespaces)
// =====================================================================

#ifndef __MAX17055_Design_Capacity__
	#define __MAX17055_Design_Capacity__	2000	// mAh
#endif

#ifndef __MAX17055_Resistor__
	#define __MAX17055_Resistor__			0.01	// Ohms
#endif

#ifndef __MAX17055_Min_Voltage__
	#define __MAX17055_Min_Voltage__		3.8		// V
#endif

#ifndef __MAX17055_Max_Voltage__
	#define __MAX17055_Max_Voltage__		4.3		// V
#endif

#ifndef __MAX17055_Empty_Voltage__
	#define __MAX17055_Empty_Voltage__		3.0		// V
#endif

#ifndef __MAX17055_Recovery_Voltage__
	#define __MAX17055_Recovery_Voltage__	3.5		// V
#endif

#ifndef __MAX17055_Max_Current__
	#define __MAX17055_Max_Current__		1.5		// A
#endif

#ifndef __MAX17055_Termination_Current__
	#define __MAX17055_Termination_Current__	0.1	// A
#endif

#ifndef __MAX17055_Min_Temperature__
	#define __MAX17055_Min_Temperature__	-20		// C
#endif

#ifndef __MAX17055_Max_Temperature__
	#define __MAX17055_Max_Temperature__	60		// C
#endif

#ifndef __MAX17055_Battery_Model__
	#define __MAX17055_Battery_Model__		2		// Model ID
#endif

#ifndef __MAX17055_Min_SOC__
	#define __MAX17055_Min_SOC__			20		// %
#endif

#ifndef __MAX17055_Max_SOC__
	#define __MAX17055_Max_SOC__			90		// %
#endif

// =====================================================================
// Register Map & Configuration
// =====================================================================

namespace MAX17055_Registers {
	// Status Register
	constexpr uint8_t STATUS			= 0x00;
	constexpr uint8_t VALRTTH			= 0x01;
	constexpr uint8_t TALRTTH			= 0x02;
	constexpr uint8_t SALRTTH			= 0x03;
	
	// Measurement Registers
	constexpr uint8_t VCELL				= 0x09;
	constexpr uint8_t AVGVCELL			= 0x19;
	constexpr uint8_t CURRENT			= 0x0A;
	constexpr uint8_t AVGCURRENT		= 0x0B;
	constexpr uint8_t TEMPERATURE		= 0x08;
	
	// Capacity Registers
	constexpr uint8_t DESIGNCAP			= 0x18;
	constexpr uint8_t REMCAP_REP		= 0x05;
	constexpr uint8_t FULLCAP_REP		= 0x35;
	
	// State of Charge
	constexpr uint8_t REPCAP			= 0x05;
	constexpr uint8_t REPSOC			= 0x06;
	constexpr uint8_t AVGSOC			= 0x0E;
	
	// Time Estimates
	constexpr uint8_t TTE				= 0x11;
	constexpr uint8_t TTF				= 0x20;
	
	// Battery Age & Cycles
	constexpr uint8_t AGE				= 0x07;
	constexpr uint8_t CYCLES			= 0x17;
	
	// Voltage Configuration
	constexpr uint8_t VEMPTY			= 0x3A;
	
	// Current & Charge Config
	constexpr uint8_t ICHGTERM			= 0x1E;
	
	// Model Configuration
	constexpr uint8_t MODELCFG			= 0xDB;
	constexpr uint8_t CONFIG			= 0x1D;
	constexpr uint8_t CONFIG2			= 0xBB;
	constexpr uint8_t HIBCFG			= 0xBA;
	
	// I2C Address
	constexpr uint8_t I2C_ADDRESS		= 0x36;
}

// =====================================================================
// Design Parameters
// =====================================================================

namespace MAX17055_Config {
	// Battery Capacity
	constexpr uint16_t DESIGN_CAPACITY		= __MAX17055_Design_Capacity__;
	constexpr float    RESISTOR				= __MAX17055_Resistor__;
	
	// Voltage Thresholds
	constexpr float    MIN_VOLTAGE			= __MAX17055_Min_Voltage__;
	constexpr float    MAX_VOLTAGE			= __MAX17055_Max_Voltage__;
	constexpr float    EMPTY_VOLTAGE		= __MAX17055_Empty_Voltage__;
	constexpr float    RECOVERY_VOLTAGE		= __MAX17055_Recovery_Voltage__;
	
	// Current Limits
	constexpr float    MAX_CURRENT			= __MAX17055_Max_Current__;
	constexpr float    TERMINATION_CURRENT	= __MAX17055_Termination_Current__;
	
	// Temperature Thresholds
	constexpr int8_t   MIN_TEMPERATURE		= __MAX17055_Min_Temperature__;
	constexpr int8_t   MAX_TEMPERATURE		= __MAX17055_Max_Temperature__;
	
	// Battery Model
	constexpr uint8_t  BATTERY_MODEL		= __MAX17055_Battery_Model__;
	
	// SOC Limits
	constexpr uint8_t  MIN_SOC				= __MAX17055_Min_SOC__;
	constexpr uint8_t  MAX_SOC				= __MAX17055_Max_SOC__;
}


// Config1 (0x1D) Configuration
//----------------------------------------------

// Enable alert on battery removal when the IC is mounted host side. 
// When Ber = 1, a battery-removal condition, as detected by the AIN pin voltage, triggers an alert.
#ifndef MAX17055_Ber
	#define MAX17055_Ber		0
#endif

// Enable alert on battery insertion when the IC is mounted host side. 
// When Bei = 1, a battery-insertion condition, as detected by the AIN pin voltage, triggers an alert.
#ifndef MAX17055_Bei
	#define MAX17055_Bei		0
#endif

// When Aen = 1, violation of any of the alert threshold register values
// by temperature, voltage, or SOC triggers an alert. This bit 
// affects the ALRT pin operation only. The Smx, Smn, Tmx, 
// Tmn, Vmx, Vmn, Imx, and Imn bits of the Status register
// (00h) are not disabled.
#ifndef MAX17055_Aen
	#define MAX17055_Aen		1
#endif

// This allows the host to control the bias of the thermistor switch or
// enable fast detection of battery removal. Set FTHRM = 1
// to always enable the thermistor bias switch. With a standard 10kΩ thermistor, 
// this adds an  additional ~200μA to the current drain of the circuit.
#ifndef MAX17055_FTHRM
	#define MAX17055_FTHRM		0
#endif

// Set to logic 1 to enable the automatic THRM output bias and AIN measurement.
#ifndef MAX17055_ETHRM
	#define MAX17055_ETHRM		0
#endif

// Set to logic 1 to force the device to enter shutdown mode if both
// SDA and SCL are held low for more than timeout of the
// ShdnTimer register.
#ifndef MAX17055_COMMSH
	#define MAX17055_COMMSH		0
#endif

// Write this bit to logic 1 to force a shutdown of the device after timeout of the ShdnTimer register (default 45s delay). 
#ifndef MAX17055_SHDN
	#define MAX17055_SHDN		0
#endif

// When set to 1, the fuel gauge requires external temperature measurements to be
// written from the host. When set to 0, the IC's own measurements as used as selected by Config.TSEL.
#ifndef MAX17055_Tex
	#define MAX17055_Tex		0
#endif

// Set to 1 and set ETHRM or FTHRM to 1 to enable temperature measurements selected by Config.TSel.
#ifndef MAX17055_Ten
	#define MAX17055_Ten		1
#endif

// Set to 1 to enable device shutdown when the IC is mounted host side and the battery is removed. 
#ifndef MAX17055_AINSH
	#define MAX17055_AINSH		0
#endif

// When IS = 1, current alerts can only be cleared through software. When IS = 0, 
// current alerts are cleared automatically when the thresholdis no longer exceeded.
#ifndef MAX17055_IS
	#define MAX17055_IS			0
#endif

// When VS = 1, voltage alerts can only be cleared through software. When VS = 0, 
// voltage alerts are cleared automatically when the threshold is no longer exceeded.
#ifndef MAX17055_VS
	#define MAX17055_VS			0
#endif

// When TS = 1, temperature alerts can only be cleared through software. WhenTS = 0, 
// temperature alerts are cleared automatically whenthe threshold is no longer exceeded.
#ifndef MAX17055_TS
	#define MAX17055_TS			0
#endif

// When SS = 1, SOC alerts can only be cleared through software. When SS = 0, 
// SOC alerts are cleared automatically when the threshold is no longer exceeded.
#ifndef MAX17055_SS
	#define MAX17055_SS			0
#endif

// 0 to use internal die temperature. 1 to use temperature information from thermistor. 
// ETHRM bit should be set to 1 when TSel is 1.
#ifndef MAX17055_TSel
	#define MAX17055_TSel		0
#endif

// Config1 (0xBB) Configuration
//----------------------------------------------

// Set to 1 to enable constant-power mode. If it is set to 0,
// AtRate/AvgCurrent is used for (At)TTE/(At) QResidual/(At)AvSOC/(At)AvCap.
#ifndef MAX17055_CPMode
	#define MAX17055_CPMode		0
#endif

// Host sets this bit to 1 in order to initiate firmware to finish processing a newly loaded model. 
// Firmware clears this bit to zero to indicate that model loading is finished.
#ifndef MAX17055_LDMDL
	#define MAX17055_LDMDL		0
#endif

// Set this bit to 1 to enable temperature based alerts. 
// Write this bit to 0 to disable temperature alerts. This bit is set to 1 at power-up.
#ifndef MAX17055_TAIrtEN
	#define MAX17055_TAIrtEN	1
#endif

// Set this bit to 1 to enable alert output with the Status.dSOCi bit function.
// Write this bit to 0 to disable alert output with the Status. dSOCi bit. This bit is set to 0 at power-up.
#ifndef MAX17055_dSOCen
	#define MAX17055_dSOCen		0
#endif

// When this bit is set to 0, Dynamic Power calculations are disabled and registers 
// MaxPeakPower/SusPeakPower/MPPCurrent/ SPPCurrent can be used as general purpose memory.
#ifndef MAX17055_DPEn
	#define MAX17055_DPEn		0
#endif

// When this bit is set to 0, AtRate calculations are disabled and registers 
// AtQResidual/AtTTE/AtAvSOC/AtAvCap can be used as general purpose memory.
#ifndef MAX17055_AtRateEn
	#define MAX17055_AtRateEn	0
#endif
