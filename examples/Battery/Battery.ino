// İnclude Library
#include "MAX17055.h"

// Create Charger Object
MAX17055 Battery(true, 8);

// Setup Function
void setup() {

	// Serial Communication Start
	Serial.begin(115200);

	// Begin Sensor
	Battery.Begin();

	// Header
	Serial.println(F("     Battery Abstract     "));
	Serial.println(F("--------------------------"));

}

void loop() {

	// Serial Print Data
	Serial.print(F("Temperature                : ")); Serial.print(Battery.IC_Temperature()); Serial.println(F(" C"));
	Serial.print(F("Instant Voltage            : ")); Serial.print(Battery.Instant_Voltage()); Serial.println(F(" V"));
	Serial.print(F("Average Voltage            : ")); Serial.print(Battery.Average_Voltage()); Serial.println(F(" V"));
	Serial.print(F("Empty Voltage              : ")); Serial.print(Battery.Empty_Voltage()); Serial.println(F(" V"));
	Serial.print(F("Recovery Voltage           : ")); Serial.print(Battery.Recovery_Voltage()); Serial.println(F(" V"));
	Serial.print(F("Instant Current            : ")); Serial.print(Battery.Instant_Current()); Serial.println(F(" mA"));
	Serial.print(F("Average Current            : ")); Serial.print(Battery.Average_Current()); Serial.println(F(" mA"));
	Serial.print(F("State of Charge            : ")); Serial.print(Battery.State_Of_Charge(), 2); Serial.println(F(" %"));
	Serial.print(F("Instant Capacity           : ")); Serial.print(Battery.Instant_Capacity()); Serial.println(F(" mAh"));
	Serial.print(F("Full Capacity              : ")); Serial.print(Battery.Full_Capacity()); Serial.println(F(" mAh"));
	Serial.print(F("Design Capacity            : ")); Serial.print(Battery.Design_Capacity()); Serial.println(F(" mAh"));
	Serial.print(F("Time to Empty              : ")); Serial.print(Battery.Time_To_Empty()); Serial.println(F(" h"));
	Serial.print(F("Time to Full               : ")); Serial.print(Battery.Time_To_Full()); Serial.println(F(" h"));
	Serial.print(F("Battery Age                : ")); Serial.print(Battery.Battery_Age()); Serial.println(F(" "));

	// Serial Print Divider
	Serial.println(F("--------------------------------"));

	// Loop Delay
	delay(5000);

}
