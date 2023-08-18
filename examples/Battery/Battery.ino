#include "MAX17055.h"

void setup() {

	// Serial Communication Start
	Serial.begin(115200);

	// Header
	Serial.println("     Battery Abstract     ");
	Serial.println("--------------------------");

}

void loop() {

	// Create Charger Object
	MAX17055 Battery(true, 4);

	// Set Start Time
	uint32_t Time = millis();

	// Serial Print Data
	//Serial.print("Temperature                : "); Serial.print(Battery.Temperature()); Serial.println(" C");
	Serial.print("Instant Voltage            : "); Serial.print(Battery.Instant_Voltage()); Serial.println(" V");
	Serial.print("Average Voltage            : "); Serial.print(Battery.Average_Voltage()); Serial.println(" V");
	Serial.print("Empty Voltage              : "); Serial.print(Battery.Empty_Voltage()); Serial.println(" V");
	Serial.print("Recovery Voltage           : "); Serial.print(Battery.Recovery_Voltage()); Serial.println(" V");
	Serial.print("Instant Current            : "); Serial.print(Battery.Instant_Current()); Serial.println(" mA");
	Serial.print("Average Current            : "); Serial.print(Battery.Average_Current()); Serial.println(" mA");
	Serial.print("State of Charge            : "); Serial.print(Battery.State_Of_Charge(), 2); Serial.println(" %");
	Serial.print("Instant Capacity           : "); Serial.print(Battery.Instant_Capacity()); Serial.println(" mAh");
    Serial.print("Full Capacity              : "); Serial.print(Battery.Full_Capacity()); Serial.println(" mAh");
    Serial.print("Design Capacity            : "); Serial.print(Battery.Design_Capacity()); Serial.println(" mAh");
    Serial.print("Time to Empty              : "); Serial.print(Battery.Time_To_Empty()); Serial.println(" h");
    Serial.print("Time to Full               : "); Serial.print(Battery.Time_To_Full()); Serial.println(" h");
    Serial.print("Battery Age                : "); Serial.print(Battery.Battery_Age()); Serial.println(" ");

    Serial.println(" ");

	// Serial Print Time
	Serial.print("Is Power On Reset          : "); Serial.println(Battery.is_Power_on_Reset());
	Serial.print("Is Min Current             : "); Serial.println(Battery.is_Min_Current());
	Serial.print("Is Max Current             : "); Serial.println(Battery.is_Max_Current());
	Serial.print("Is Min Voltage             : "); Serial.println(Battery.is_Min_Voltage());
	Serial.print("Is Max Voltage             : "); Serial.println(Battery.is_Max_Voltage());
	Serial.print("Is Min Temperature         : "); Serial.println(Battery.is_Min_Temperature());
	Serial.print("Is Max Temperature         : "); Serial.println(Battery.is_Max_Temperature());
	Serial.print("Is Min SOC                 : "); Serial.println(Battery.is_Min_SOC());
	Serial.print("Is Max SOC                 : "); Serial.println(Battery.is_Max_SOC());
	Serial.print("Is Battery Present         : "); Serial.println(Battery.is_Battery_Present());
	Serial.print("Is SOC Change              : "); Serial.println(Battery.is_SOC_Change());
	Serial.print("Is Battery Insertion       : "); Serial.println(Battery.is_Battery_Insertion());
	Serial.print("Is Battery Removal         : "); Serial.println(Battery.is_Battery_Removal());

	// Calculate Delta Time
	uint32_t _DT = millis() - Time;

	// Serial Print Divider
	Serial.println("--------------------------------");

	// Loop Delay
	delay(5000);

}
