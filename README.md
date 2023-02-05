# Open Microfluidics Workstation

Repository for the development of the *compact* *modules* of a free and *open source* microfluidic workstation, for *high-throughput droplet microfluidic* biological assays. It addresses gas-pressure control to push microfluidic samples onto chips, pressure and flow measurement with feedback controll, sample holders with heating and stirring, and imaging of fast droplet generation processes with open source microscopy and stroboscopic illumination.

The aim is to create a prototype of a *compact* working station that is based on *connectable*, *open*, modern and *low-cost* components (Rasberry Pi, Arduino, 3D printing, on-board components, open or at least accessible design software and operation software - python). This workstation is aimed to be a fully functional research grade equipment with good specifications, such as fast reaction times and low pressure fluctuations. It is modular, so that parts of the workstation can be repurposed and improved in the open source hardware sense, and also easily be combined, exchanged or used on their own in challenging environments such as an anaerobic chamber. This is an open collaborative project and *your participation* (comments, inputs, contributions) are explicitly welcome! Please submit your message as an issue in this repository to participate.

> **Please Note:** This technical index is very much under construction, and is at present an incomplete summary of the various documents that exist elsewhere in the Open Microfluidics Workstation repositories on GiHub, and elsewhere.

## Table of Contents

### Description and assessment

* **TO DO**: Specifications

* **TO DO**: General description
    * **TO DO**: One-page description of the OM Workstation
    * **TO DO**: Block diagram
    * **TO DO**: Wiring/circuit diagrams
        * **TO DO**: Pi Hat
        * **TO DO**: Fast Imaging Module
        * **TO DO**: Pressure and Flow Controller Module
        * **TO DO**: Sample Holders Module
    * **TO DO**: Drawings
    * **TO DO**: Control philosophy/logic
        * **TO DO**: Software (it should cover the software architecture and technologies employed)
        * **TO DO**: Overview paper
        * **TO DO**: Flowcharts or descriptions of important procedures e.g. droplet generation
    * **TO DO**: Datasheets for critical sub-assemblies
        * **TO DO**: Pi
        * **TO DO**: Pi Hat
        * **TO DO**: Fast Imaging Module
        * **TO DO**: Pressure and Flow Controller Module
        * **TO DO**: Sample Holders Module
        * **TO DO**: Sample Reservoirs

* **TO DO**: List of standards applied
    * **TO DO**: Records of assessments to standard

* **TO DO**: Risk
    * **TO DO**: Failure Mode and Effects Analysis (FMEA)
    * **TO DO**: Risk-related meeting and generate minutes
    * **TO DO**: Extract documents from our risk register that better match the format of documents expected by regulators/auditors.
    * **TO DO**: Add required **records** relating to risk

* **TO DO**: Verification and validation
    * **TO DO**: Analytical/Technical performance
        * **TO DO**: Closed loop
        * **TO DO**: Stability
    * **TO DO**: Scientific validity

### Instructions

* **TO DO**: Assembly instructions
    * **TO DO**: Parts list
    * **TO DO**: Software installation
    * **TO DO**: Go all the way to being ready to image and/or calibrate

* **TO DO**: Usage instructions
	* **TO DO**: Software getting-started
    * **TO DO**: Software usage
	* **TO DO**: First-run instructions (which should also be in assembly instructions)
	* **TO DO**: Basic fault-finding and operation
	* **TO DO**: Maintenance instructions
	* **TO DO**: Installation instructions

* **TO DO**: Calibration/commissioning procedures
    * **TO DO**: Document the automated procedures
    * **TO DO**: Document the manual checks
    * **TO DO**: Implement and document self-tests

* **TO DO**: Quality control
    * **TO DO**: Visual guide to check prints and assembly
    * **TO DO**: Checking output of calibration/commissioning scripts

### Design files and source code

* **TO DO**: Hardware designs
    * **TO DO**: OM Workstation hardware and assembly instructions (source)
    * **TO DO**: Pull in specific versions of STLs
    * **TO DO**: Pi Hat
    * [Fast Imaging Module](https://github.com/wenzel-lab/open-microfluidics-workstation/tree/master/module-fast-imaging)
    * [Pressure and Flow Controller Module](https://github.com/wenzel-lab/open-microfluidics-workstation/tree/master/module-pressure-and-flow-controller)
    * [Sample Holders Module](https://github.com/wenzel-lab/open-microfluidics-workstation/tree/master/module-sample-holders)
    * [Sample Reservoirs](https://github.com/wenzel-lab/open-microfluidics-workstation/tree/master/module_sample_reservoirs)
 
* **TO DO**: Software source code
    * **TO DO**: OM Workstation server (This code runs on the built-in Raspberry Pi, and includes the web application for the user interface.)
    * **TO DO**: OM Workstation operating system (This collates all software and builds a pre-written SD card image for the Raspberry Pi)

## Modules Wish List
 * Microfluidic droplet sorting workstation driven by the droplet workstation tools describes here. This is beeing developed in a seperate repository: https://github.com/MakerTobey/Open_FPGA_control_for_FADS
 * High Pressure Source Module in sub-replository https://github.com/wenzel-lab/open-microfluidics-workstation/tree/master/module-high-pressure-source
 * Anaerobic Chamber Module (this module might have its own repository)
 * Note, there is also a previous general open source microfluidics repository with re-builds and design considerations: https://github.com/MakerTobey/OpenMicrofluidics

## Acknowledgment
This technical index is based on the [OpenFlexure technical overview](https://gitlab.com/openflexure/microscope-technical-overview/-/tree/main) and discussions with Richard Bowman. 
