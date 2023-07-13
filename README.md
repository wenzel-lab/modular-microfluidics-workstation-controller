# Open Microfluidics Workstation

Repository for developing the *compact* *modules* of a free and *open-source* microfluidic workstation for *high-throughput droplet microfluidic* biological assays. It addresses gas-pressure control to push microfluidic samples onto chips, pressure, and flow measurement with feedback control, sample holders with heating and stirring, and imaging of fast droplet generation processes with open-source microscopy and stroboscopic illumination.

The aim is to create a prototype of a *compact* working station that is based on *connectable*, *open*, modern, and *low-cost* components (Rasberry Pi, Arduino, 3D printing, on-board components, open or at least accessible design software and operation software - python). This workstation is aimed to be fully functional research-grade equipment with good specifications, such as fast reaction times and low-pressure fluctuations. It is modular so that parts of the workstation can be repurposed and improved in the open-source hardware sense and easily combined, exchanged, or used independently in challenging environments such as an anaerobic chamber.

This is an open, collaborative project, and *your participation* (comments, inputs, contributions) are explicitly welcome! Please submit your message as an issue in this repository to participate.

> **Please Note:** This technical index is very much under construction and is at present an incomplete summary of the various documents that exist elsewhere in the Open Microfluidics Workstation repositories on GitHub and elsewhere.

## Table of Contents

### Design files and source code

* Hardware designs
    * [Fast Imaging Module](https://github.com/wenzel-lab/open-microfluidics-workstation/tree/master/module-fast-imaging)
    * [Pressure and Flow Controller Module](https://github.com/wenzel-lab/open-microfluidics-workstation/tree/master/module-pressure-and-flow-controller)
    * [Sample Holders Module](https://github.com/wenzel-lab/open-microfluidics-workstation/tree/master/module-sample-holders)
    * [Sample Reservoirs](https://github.com/wenzel-lab/open-microfluidics-workstation/tree/master/module_sample_reservoirs)
 
* Software source code
    * [OM Workstation server](https://github.com/wenzel-lab/open-microfluidics-workstation/blob/master/module-pi/webapp.zip) (This code runs on the built-in Raspberry Pi and includes the web application for the user interface.)

### Instructions

* Assembly instructions
    * [Software installation](https://github.com/wenzel-lab/open-microfluidics-workstation/wiki/Install-the-Software)
 
### Applications
* Strobe-enhanced microscopy stage: [assembly instructions](https://librehub.github.io/3_Levels_Stage/) and [repository](https://github.com/LIBREhub/3_Levels_Stage) 

## Modules Wish List
* Microfluidic droplet sorting workstation driven by the droplet workstation tools is described [here](https://github.com/MakerTobey/Open_FPGA_control_for_FADS) (This is being developed in a separate repository).
* High-Pressure Source Module in [sub-repository](https://github.com/wenzel-lab/open-microfluidics-workstation/tree/master/module-high-pressure-source)
* Anaerobic Chamber Module (this module might have its own repository)

> **Please Note:** There is also a previous general [open-source microfluidics repository](https://github.com/MakerTobey/OpenMicrofluidics) with re-builds and design considerations. 

## Acknowledgment
This technical index is based on the [OpenFlexure technical overview](https://gitlab.com/openflexure/microscope-technical-overview/-/tree/main) and discussions with Richard Bowman. 
