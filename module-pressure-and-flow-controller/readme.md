## Pressure and flow controller module

This is the core piece of the workstation. 
This electronic controller module aims to regulate and distrbute the pressure (originating from the pressure source module) into the sample tubes, so that the samples (aqueous, gel pre-cursors and oils) flow onto the microfluidic chip and the collection resarvoirs.
We aim to regulate pressures enough for ultra-high throughput agarose bead generation (several bar), in a smooth fashion that avoids pulsing especially at steady state.
This controller requires a custom cuircuit board in addition to connectors and mechanical parts. Nothing in this controller should be manual.

The flow controller is a modular part of the circuit board of the pressure controller. Populating this part of the board with components is optional.
The flow controller and its flow-sensors do not have to be used at all times, but rather for dynamic protocols and for establishing new experiments.
Once desired flowrates and required pressures are known, it is enough to use the pressure controller unit without setting up the flow sensors.

More detail to be filled in on our approach and approaches taken in the literature.

Proportional valve approach?

PID controll of solenoid outlet approach? - with vent

Regulator modules...
