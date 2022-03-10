# MAX17055 Library <sup>V2.1</sup>

![GitHub release (latest by date)](https://img.shields.io/github/v/release/akkoyun/MAX17055) ![arduino-library-badge](https://www.ardu-badge.com/badge/MAX17055.svg?) ![Visits Badge](https://badges.pufler.dev/visits/akkoyun/MAX17055) ![GitHub stars](https://img.shields.io/github/stars/akkoyun/MAX17055?style=flat&logo=github) ![Updated Badge](https://badges.pufler.dev/updated/akkoyun/MAX17055) [![PlatformIO Registry](https://badges.registry.platformio.org/packages/akkoyun/library/MAX17055.svg)](https://registry.platformio.org/libraries/akkoyun/MAX17055)
[![Check Arduino](https://github.com/akkoyun/MAX17055/actions/workflows/check-arduino.yml/badge.svg)](https://github.com/akkoyun/MAX17055/actions/workflows/check-arduino.yml) [![Compile Examples](https://github.com/akkoyun/MAX17055/actions/workflows/compile-examples.yml/badge.svg)](https://github.com/akkoyun/MAX17055/actions/workflows/compile-examples.yml) [![Spell Check](https://github.com/akkoyun/MAX17055/actions/workflows/spell-check.yml/badge.svg)](https://github.com/akkoyun/MAX17055/actions/workflows/spell-check.yml)

	Battery Gauge Sensor - 02.01.05

---

**General Description**

The MAX17055 is a low 7μA operating current fuel gauge that implements Maxim ModelGaugeTM m5 EZ algorithm. ModelGauge m5 EZ makes fuel gauge implementation easy by eliminating battery characterization requirements and simplifying host software interaction. The ModelGauge m5 EZ robust algorithm provides tolerance against battery diversity for most lithium batteries and applications.

ModelGauge m5 EZ algorithm combines the short-term accuracy and linearity of a coulomb counter with the longterm stability of a voltage-based fuel gauge, along with temperature compensation to provide industry-leading fuel gauge accuracy. The MAX17055 automatically compensates for cell aging, temperature, and discharge rate, and provides accurate state of charge (SOC in %) and remaining capacity in milliampere-hours (mAh). As the battery approaches the critical region near empty, the ModelGauge m5 algorithm invokes a special compensa- tion that eliminates any error. It also provides three methods for reporting the age of the battery: reduction in capacity, increase in battery resistance, and cycle odometer.

The MAX17055 provides precision measurements of current, voltage, and temperature. Temperature of the battery pack is measured using an internal temperature measurement or external thermistor. A 2-wire I2C interface provides access to data and control registers.

The MAX17055 is available in a tiny, lead-free 0.4mm pitch 1.4mm x 1.5mm, 9-pin WLP package, and a 2mm x 2.5mm, 10-pin TDFN package.

**Benefits and Features**

* ModelGauge m5 EZ
	* No Characterization Required for EZ Performance
	* Robust Against Battery Variation
	* Eliminates Error Near Empty Voltage
	* Eliminates Coulomb Counter Drift
	* Current, Temperature, and Age Compensated
	* Does Not Require Empty, Full, or Idle States
* Low 7μA Operating Current
* Wide Sense Resistor Range
	* 1mΩ to 1000mΩ
	* PCB Metal Sensing + Temperature Compensation
* Supports Li+ and Variants Including LiFePO4
* ±1°C Internal Temperature or Thermistor
* Dynamic Power Estimates Power Capability During Discharge
* Time-to-Empty and Time-to-Full Estimation
	* Constant Power or Constant Current
* Predicts Remaining Capacity Under Theoretical Load
* Precision Measurement System
	* No Calibration Required
* Alert Indicator for Voltage, SOC, Temperature, Current and 1% SOC Change

**Measured Parameters**

* Gauge Serial ID
* Instant Voltage
* Average Voltage
* Empty Voltage
* Charge Recovery Voltage
* Instant Current
* Average Current
* Charge Termination Current
* State Of Charge
* Average State Of Charge
* Instant Capacity
* Design Capacity
* Full Capacity
* Temperature
* Time To Empty
* Time To Full
* Battery Age
* Charge Cycle
* Control for Power on Reset
* Control for Min Current
* Control for Max Current
* Control for Min Voltage
* Control for Max Voltage
* Control for Min Temperature
* Control for Max Temperature
* Control for Min SOC
* Control for Max SOC
* Control for Battery Present
* Control for SOC Change
* Control for Battery Insertion
* Control for Battery Removal

---

[![Support me](https://img.shields.io/badge/Support-PATREON-GREEN.svg)](https://www.patreon.com/bePatron?u=62967889) ![Twitter Follow](https://img.shields.io/twitter/follow/gunceakkoyun?style=social) ![YouTube Channel Views](https://img.shields.io/youtube/channel/views/UCIguQGdaBT1GnnVMz5qAZ2Q?style=social) ![Repos Badge](https://badges.pufler.dev/repos/akkoyun) [![E-Mail](https://img.shields.io/badge/E_Mail-Mehmet_Gunce_Akkoyun-blue.svg)](mailto:akkoyun@me.com) ![GitHub](https://img.shields.io/github/license/akkoyun/Statistical) 
