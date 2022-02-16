#include "MAX17055.h"

void setup() {

	// Serial Communication Start
	Serial.begin(115200);

	// Set Multiplexer
	I2C.Set_Multiplexer(__ADDR_TCA9548__ ,__CH4__);

	// Start Battery Gauge
	Battery.Begin(2000, 0.01);

	// Header
	Serial.println("     Battery Abstract     ");
	Serial.println("--------------------------");

}

void loop() {

	// Set Start Time
	uint32_t Time = millis();

	// Measure
	float _IV = Battery.Instant_Voltage();
	float _AV = Battery.Average_Voltage();
	float _EV = Battery.Empty_Voltage();
	float _RV = Battery.Recovery_Voltage();

	float _SOC = Battery.State_Of_Charge();

	float _T = Battery.Temperature();

	uint16_t _TTE = Battery.Time_To_Empty();
	uint16_t _TTF = Battery.Time_To_Full();

	uint16_t _AGE = Battery.Battery_Age();

	float _IC = Battery.Instant_Current();
	float _AC = Battery.Average_Current();
	float _CTC = Battery.Charge_Termination_Current();

	uint16_t _IBC = Battery.Instant_Capacity();
	uint16_t _DBC = Battery.Design_Capacity();
	uint16_t _FBC = Battery.Full_Capacity();

	// Calculate Delta Time
	uint32_t _DT = millis() - Time;

	// Serial Print Data
	Serial.print("Temperature                : "); Serial.print(_T); Serial.println(" C");
	Serial.print("Instant Voltage            : "); Serial.print(_IV); Serial.println(" V");
	Serial.print("Average Voltage            : "); Serial.print(_AV); Serial.println(" V");
	Serial.print("Empty Voltage              : "); Serial.print(_EV); Serial.println(" V");
	Serial.print("Recovery Voltage           : "); Serial.print(_RV); Serial.println(" V");
	Serial.print("Instant Current            : "); Serial.print(_IC); Serial.println(" mA");
	Serial.print("Average Current            : "); Serial.print(_AC); Serial.println(" mA");
	Serial.print("Charge Termination Current : "); Serial.print(_CTC); Serial.println(" mA");
	Serial.print("State of Charge            : "); Serial.print(_SOC, 2); Serial.println(" %");
	Serial.print("Instant Capacity           : "); Serial.print(_IBC); Serial.println(" mAh");
    Serial.print("Full Capacity              : "); Serial.print(_FBC); Serial.println(" mAh");
    Serial.print("Design Capacity            : "); Serial.print(_DBC); Serial.println(" mAh");
    Serial.print("Time to Empty              : "); Serial.print(_TTE); Serial.println(" h");
    Serial.print("Time to Full               : "); Serial.print(_TTF); Serial.println(" h");
    Serial.print("Battery Age                : "); Serial.print(_AGE); Serial.println(" ");

	Serial.print("Function Time              : "); Serial.print(_DT); Serial.println(" mS");

	Serial.print("Serial ID                  : "); Serial.print(Battery.Serial_ID()); Serial.println("");
	Serial.println("--------------------------------");

	// Loop Delay
	delay(5000);

}
