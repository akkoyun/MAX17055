#include "MAX17055.h"

void setup() {

	// Serial Communication Start
	Serial.begin(115200);

	// Set Multiplexer
	I2C.Set_Multiplexer(0x70, 4);

	// Start Battery Gauge
	Battery.Begin(2000, 0.01);

	// Header
	Serial.println("     Battery Abstract     ");
	Serial.println("--------------------------");

}

void loop() {

	// Set Start Time
	uint32_t Time = millis();

	// Serial Print Data
	Serial.print("Temperature                : "); Serial.print(Battery.Temperature()); Serial.println(" C");
	Serial.print("Instant Voltage            : "); Serial.print(Battery.Instant_Voltage()); Serial.println(" V");
	Serial.print("Average Voltage            : "); Serial.print(Battery.Average_Voltage()); Serial.println(" V");
	Serial.print("Empty Voltage              : "); Serial.print(Battery.Empty_Voltage()); Serial.println(" V");
	Serial.print("Recovery Voltage           : "); Serial.print(Battery.Recovery_Voltage()); Serial.println(" V");
	Serial.print("Instant Current            : "); Serial.print(Battery.Instant_Current()); Serial.println(" mA");
	Serial.print("Average Current            : "); Serial.print(Battery.Average_Current()); Serial.println(" mA");
	Serial.print("Charge Termination Current : "); Serial.print(Battery.Charge_Termination_Current()); Serial.println(" mA");
	Serial.print("State of Charge            : "); Serial.print(Battery.State_Of_Charge(), 2); Serial.println(" %");
	Serial.print("Instant Capacity           : "); Serial.print(Battery.Instant_Capacity()); Serial.println(" mAh");
    Serial.print("Full Capacity              : "); Serial.print(Battery.Full_Capacity()); Serial.println(" mAh");
    Serial.print("Design Capacity            : "); Serial.print(Battery.Design_Capacity()); Serial.println(" mAh");
    Serial.print("Time to Empty              : "); Serial.print(Battery.Time_To_Empty()); Serial.println(" h");
    Serial.print("Time to Full               : "); Serial.print(Battery.Time_To_Full()); Serial.println(" h");
    Serial.print("Battery Age                : "); Serial.print(Battery.Battery_Age()); Serial.println(" ");

	// Calculate Delta Time
	uint32_t _DT = millis() - Time;

	// Serial Print Time
	Serial.print("Function Time              : "); Serial.print(_DT); Serial.println(" mS");

	// Serial Print Divider
	Serial.println("--------------------------------");

	// Loop Delay
	delay(5000);

}
