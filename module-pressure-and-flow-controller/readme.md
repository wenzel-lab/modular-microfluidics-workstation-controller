## Pressure and flow controller module

This is the core piece of the workstation. 
This electronic controller module aims to regulate and distrbute the pressure (originating from the pressure source module) into the sample tubes, so that the samples (aqueous, gel pre-cursors and oils) flow onto the microfluidic chip and the collection resarvoirs.
We aim to regulate pressures enough for ultra-high throughput agarose bead generation (several bar), in a smooth fashion that avoids pulsing especially at steady state.
This controller requires a custom cuircuit board in addition to connectors and mechanical parts. Nothing in this controller should be manual.

The flow controller is a modular part of the circuit board of the pressure controller. Populating this part of the board with components is optional.
The flow controller and its flow-sensors do not have to be used at all times, but rather for dynamic protocols and for establishing new experiments.
Once desired flowrates and required pressures are known, it is enough to use the pressure controller unit without setting up the flow sensors.

More detail to be filled in on our approach and approaches taken in the literature.

### PCB Components

<img src="images/pcb_copper_top.jpg" width=50%><img src="images/pcb_copper_bottom.jpg" width=50%>

|Qty|Name|Component|Description|
|-|-|-|-|
|7|C1-C5,C8-C9|100nf Capacitor 0603|Decoupling Capacitors|
|2|C6,C10|10uF Capacitor 0603|Bypass,Bulk Capacitor|
|1|C7|Not fitted||
|1|C2|dsPIC33CK256MP502-I/SS SSOP28|PIC Microprocessor|
|1|C3|AD5624R|DAC|
|1|C4|ADS1115|ADC|
|1|C5|PCA9544APW,112|I2C Multiplexer|
|1|C6|25AA040ST|EEPROM|
|12|R1-R10,R12-R13|1k Resistor 0603||
|1|R11|10k Resistor 0603||
|8|X1-X8|Picoblade 53047-0410|Connectors|

