# "Rio controller" the modular microfluidics controller [![Open Source Love](https://badges.frapsoft.com/os/v1/open-source.svg?v=103)](https://github.com/ellerbrock/open-source-badges/)

This repository contains the free and *open-source* design and documentation for the *brain of a microfluidics workstation* to enable *high-throughput droplet microfluidic* biological assays. The design contains an electronics board "hat" that plugs onto a Raspberry Pi single-board computer and interfaces with our *compact* *modules* e.g. for gas-pressure control to push microfluidic samples onto chips, pressure, and flow measurement with feedback control, sample holders with heating and stirring, and imaging of fast droplet generation processes with open-source microscopy and stroboscopic illumination

Our aim is to create a prototype of a *compact* working station that is based on *connectable*, *open*, modern, and *low-cost* components (Rasberry Pi, Arduino, 3D printing, on-board components, open or at least accessible design software and operation software - python). This workstation is aimed to be fully functional research-grade equipment with good specifications, such as fast reaction times and low-pressure fluctuations. It is modular so that parts of the workstation can be repurposed and improved in the open-source hardware sense and easily combined, exchanged, or used independently in challenging environments such as an anaerobic chamber.

This is an open, collaborative project by the Wenzel Lab in Chile, and *your participation* (comments, inputs, contributions) are explicitly welcome! Please submit your message as an issue in this repository to participate.

> **Please Note:** This technical index is very much under construction and is at present an incomplete summary of the various documents that exist elsewhere in the Open Microfluidics Workstation repositories on GitHub and elsewhere.

Follow us! [#twitter](https://twitter.com/WenzelLab), [#YouTube](https://www.youtube.com/@librehub), [#LinkedIn](https://www.linkedin.com/company/92802424), [#instagram](https://www.instagram.com/wenzellab/), [#Printables](https://www.printables.com/@WenzelLab), [#LIBREhub website](https://librehub.github.io), [#IIBM website](https://ingenieriabiologicaymedica.uc.cl/en/people/faculty/821-tobias-wenzel)


<!--- ## Table of Contents --->

<!--- ## Background --->

## Usage

* Installation instructions
    * [Software installation](https://github.com/wenzel-lab/moldular-microfluidics-workstation-controller/wiki/Install-the-Software)
* Our modules are used in our [flow-microscopy-platform, see repostiory for documentation and instructions](https://github.com/wenzel-lab/flow-microscopy-platform)
* And it also powers our [strobe-enhanced microscopy stage](https://wenzel-lab.github.io/strobe-enhanced-microscopy-stage/) see also [it's repository](https://github.com/wenzel-lab/strobe-enhanced-microscopy-stage) 

## Design files and source code

* Hardware designs
    * [Fast imaging module](https://github.com/wenzel-lab/moldular-microfluidics-workstation-controller/tree/master/module-fast-imaging)
    * [Pressure and flow controller module](https://github.com/wenzel-lab/moldular-microfluidics-workstation-controller/tree/master/module-pressure-and-flow-controller)
    * [Sample holders module](https://github.com/wenzel-lab/moldular-microfluidics-workstation-controller/tree/master/module-sample-holders)
 
* Software source code
    * [Microfluidics workstation server](https://github.com/wenzel-lab/moldular-microfluidics-workstation-controller/blob/master/module-pi/webapp.zip) (This code runs on the built-in Raspberry Pi and includes the web application for the user interface.

### Modules Wish List
* Microfluidic droplet sorting workstation driven by the droplet workstation tools is described [here](https://github.com/MakerTobey/Open_FPGA_control_for_FADS) (This is being developed in a separate repository).
* High-pressure source module in [sub-repository](https://github.com/wenzel-lab/moldular-microfluidics-workstation-controller/tree/master/module-high-pressure-source)
* Anaerobic chamber module (this module might have its own repository)

> **Please Note:** There is also a previous general [open-source microfluidics repository](https://github.com/MakerTobey/OpenMicrofluidics) with re-builds and design considerations. 

## Contribute

Feel free to dive in! [Open an issue](https://github.com/wenzel-lab/moldular-microfluidics-workstation-controller/issues/new).
For interactions in our team and with the community applies the [GOSH Code of Conduct](https://openhardware.science/gosh-2017/gosh-code-of-conduct/).

## License

[CERN OHL 2W](LICENSE) © Tobias Wenzel, Christie Nel, and Pierre Padilla-Huamantinco. This project is Open-Source Hardware - please acknowledge us when using the hardware or sharing modifications.
