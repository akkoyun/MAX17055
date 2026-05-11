// Include Library
#include "MAX17055.h"

// Create Charger Object with multiplexer channel
MAX17055 Battery(true, 8);

// Setup Function
void setup() {

	// Serial Communication Start
	Serial.begin(115200);

	// Begin Sensor
	if (!Battery.Begin()) {
		Serial.println(F("ERROR: Battery gauge not detected!"));
		while(1);
	}

	// Header
	Serial.println(F("     MAX17055 Battery Gauge     "));
	Serial.println(F("--------------------------------"));

}

void loop() {

	// ===== METHOD 1: Individual readings (backward compatible) =====
	
	// Temperature
	Serial.print(F("Temperature              : ")); 
	Serial.print(Battery.IC_Temperature()); 
	Serial.println(F(" C"));
	
	// Voltages
	Serial.print(F("Instant Voltage          : ")); 
	Serial.print(Battery.Instant_Voltage()); 
	Serial.println(F(" V"));
	
	Serial.print(F("Average Voltage          : ")); 
	Serial.print(Battery.Average_Voltage()); 
	Serial.println(F(" V"));
	
	// Currents
	Serial.print(F("Instant Current          : ")); 
	Serial.print(Battery.Instant_Current()); 
	Serial.println(F(" A"));
	
	Serial.print(F("Average Current          : ")); 
	Serial.print(Battery.Average_Current()); 
	Serial.println(F(" A"));
	
	// State of Charge
	Serial.print(F("State of Charge          : ")); 
	Serial.print(Battery.State_Of_Charge(), 2); 
	Serial.println(F(" %"));
	
	Serial.print(F("Average SOC              : ")); 
	Serial.print(Battery.Average_State_Of_Charge(), 2); 
	Serial.println(F(" %"));
	
	// Capacity
	Serial.print(F("Instant Capacity         : ")); 
	Serial.print(Battery.Instant_Capacity()); 
	Serial.println(F(" mAh"));
	
	Serial.print(F("Design Capacity          : ")); 
	Serial.print(Battery.Design_Capacity()); 
	Serial.println(F(" mAh"));
	
	Serial.print(F("Full Capacity            : ")); 
	Serial.print(Battery.Full_Capacity()); 
	Serial.println(F(" mAh"));
	
	// Time Estimates
	Serial.print(F("Time to Empty            : ")); 
	Serial.print(Battery.Time_To_Empty()); 
	Serial.println(F(" h"));
	
	Serial.print(F("Time to Full             : ")); 
	Serial.print(Battery.Time_To_Full()); 
	Serial.println(F(" h"));
	
	// Battery Info
	Serial.print(F("Battery Age              : ")); 
	Serial.println(Battery.Battery_Age());
	
	Serial.print(F("Charge Cycles            : ")); 
	Serial.println(Battery.Charge_Cycle());
	
	// Status
	Serial.print(F("Battery Status           : ")); 
	Serial.println(Battery.Status_Control(MAX17055_Bat_Status) ? F("OK") : F("ERROR"));
	
	Serial.println(F("--------------------------------"));

	// ===== METHOD 2: Batch read (RECOMMENDED - more efficient) =====
	/*
	MAX17055::BatteryData_t battery_data;
	
	if (Battery.Read_Battery_Data(battery_data)) {
		Serial.print(F("Voltage   : ")); Serial.print(battery_data.voltage_mV); Serial.println(F(" mV"));
		Serial.print(F("Current   : ")); Serial.print(battery_data.current_mA); Serial.println(F(" mA"));
		Serial.print(F("SOC       : ")); Serial.print(battery_data.soc_percent); Serial.println(F(" %"));
		Serial.print(F("Temp      : ")); Serial.print(battery_data.temperature_C); Serial.println(F(" C"));
		Serial.print(F("Capacity  : ")); Serial.print(battery_data.capacity_mAh); Serial.println(F(" mAh"));
		Serial.print(F("Cycles    : ")); Serial.println(battery_data.cycles);
	} else {
		Serial.println(F("ERROR: Failed to read battery data"));
	}
	*/

	// Loop Delay
	delay(5000);

}
