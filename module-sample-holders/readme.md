# Sample Holder Module

<img src="images/sample_holder_with_insert.jpg" width=50%>

The sample holder module comprises a PCB, two cartridge heaters, temperature sensor and stirring motor and magnets, all integrated into an aluminium shell.  Aluminium inserts that slot into the top can be machined to accommodate sample bottles of various shapes and sizes.  The sample holder connects to a Raspberry Pi via SPI, from which the sample holder temperature and stir speed are set and monitored.

The sample holder has been tested to 100 degrees Celcius and provides a stable (but uncalibrated) readout to 0.01 degrees Celcius.

## PCB

<img src="images/sample_holder_pcb_top.jpg" width=50%><img src="images/sample_holder_pcb_bottom.jpg" width=50%>

The sample holder PCB is the heart of the sample holder and contains a PIC microprocessor that reads the temperature sensor, controls the heaters and stirrer motor and offers an SPI interface to a host.

The PCB has been designed for dual use.  It can be mounted in a sample holder with heaters, temperature sensor and stir motor wires soldered to the PCB.  Alternatively, it can also be used with external heater and temperature sensor attached via the secondary connector as shown below.

<img src="images/sample_holder_pcb_wired.jpg" width=50%>

The connector on the left is the primary connector and provides 12V power for the heaters, 3.3V power for the microprocessor and SPI communications with the Raspberry Pi.

The connector on the right is the secondary connector that connects to the external heater (black wires) and temperature sensor (white wires).  The polarity of heater and temperature sensor does not matter.  The connector also provides debug serial output and ICSP interface for reprogramming the PIC microprocessor.

### Primary Connector : Power and SPI Interface

<img src="images/sample_holder_pcb_connector.jpg" width=10%>

|Pin|Description|
|-|-|
|1|SPI Clock|
|2|SPI Slave Select|
|3|Vdd 3.3V|
|4|SPI MOSI|
|5|Vss (Digital Ground)|
|6|SPI MISO|
|7|Heater 0V|
|8|Heater 12V|
|9|Heater 0V|
|10|Heater 12V|

**Note:** Vss(GND) must be connected with Heater 0V, and in a way that prevents ground loops.

### Secondary Connector : External Heater, Temperature Sensor, Debug Serial and Programming Interface (ICSP)

<img src="images/sample_holder_pcb_connector.jpg" width=10%>

|Pin|Description|
|-|-|
|1|Heater Out 12V|
|2|ICSP MCLR|
|3|Heater Out 0V|
|4|ICSP 3.3V|
|5|Temp Sensor (Vss)|
|6|ICSP GND (Vss)|
|7|Temp Sensor +|
|8|ICSP PGD|
|9|Serial Debug TX|
|10|ICSP PGC|

**Note:** Do not supply ICSP 3.3V while also supplying Vdd 3.3V as they are connected.

### FET Mounting

<img src="images/sample_holder_pcb_side.jpg" width=50%>

A power FET is mounted on the bottom of the PCB and drives the heaters.  It is intended to touch the metal the PCB is mounted on, with thermal paste between FET and metal.

### BOM and Placement

<img src="images/sample_holder_pcb_placement_top.jpg" width=50%><img src="images/sample_holder_pcb_placement_bottom.jpg" width=50%>

|Qty|Name|Component|Description|
|-|-|-|-|
|1|IC2|dsPIC33CK256MP502-I/SS SSOP28|PIC Microprocessor|
|1|IC1|25AA128-I/ST TSSOP8|SPI EEPROM|
|1|Q1|TO229P239X654X978-3P|MOSFET|
|2|T1,T3|MMBT3904LT1 SOT23|NPN Transistor|
|2|T2,T4|MMBT3906LT1 SOT23|PNP Transistor|
|1|LED1|LTST-S270KGKT 0603|Green Side View LED|
|2|J1,J2|10 Pin 2 Row Angled Header||
|1|R7|3.3k Resistor|Temperature Sensor Reference|
|6|R1-R6|1k Resistor 0603||
|1|R9|10k Resistor 0603|PIC MCLR Pull Up|
|1|R8|120R Resistor 0603|LED Resistor|
|4|C1,C2,C4,C6|100nf Capacitor 0603|Decoupling Capacitors|
|1|C3|1nf Capacitor 0603|Motor Drive Capacitor|
|1|C5|10uf Capacitor 1206|Bulk Capacitor|

## Heaters and Sensors

<img src="images/sample_holder_heaters_and_sensors.jpg" width=50%>

The heaters are 3.3 Ohm (rated 12V, 40W) stainless steel cartridges that insert into holes drilled into the top section of the sample holder.  A temperature sensor is inserted into a hole drilled into the same section.  The temperature sensor is a negative coefficient thermister (NTC).  These heaters and temperature sensors are commonly used in 3D printers and can be found on eBay or Amazon.

## Stirrer Motor and Magnets

The stirrer motor rotates two attached magnets, which in turn rotate an optional stirrer magnet in the sample bottle inserted into the top of the sample holder.

The stirrer motor is a standard 40mm x 40mm x 10mm, three wire fan.  The rotating side points upward and has two 6mm diameter x 2mm thick Neodymium magnets mounted flat next to each other (opposing poles same side) in the centre of the fan.  The magnets have to be raised off the fan such that their fields do not interfere with the operation of the fan, be close enough to the stirrer magnet, while not touching the opposing surface.  We find that raising it 3-4mm with layers of double sided tape is sufficient and provides enough strength to spin the stirrer magnet.  The fan wire colours are typically black=ground, red=12V, yellow=sense/tachometer.

## Power Considerations

The heaters can draw quite a lot of power, so an adequate 12V power supply must be provided.  At full power with two 3.3 Ohm heaters, the required current can be calculated as:
I = ( 12V / 3.3 Ohm ) * 2 heaters = 7.3A
The maximum power can be limited by storing a power limit percentage to the sample holder via SPI.  Alternatively, these 40W heater cartridges are also made for 24V, which have around 4x the resistance (P=V^2/R, 40=24^2/R, R=14.4 Ohm) and will thus consume 1/4 as much power, but heat more slowly.

## Tobey's text

This is an important, but less high-tech part of the workstation and needs to be particularly adaptable to accomodate using a variety of samples.
The sample holder module attaches to the pressure control module and clamps the sample tubes into place, so that pressure inlets and thin sample outlet tubing can be attached securely, before pressures up to several bar can be applied to the sample tubes (e.g. eppendorf tubes, falcon tubes, glass bottles).

Nanoport connections for PTFE tubing...
