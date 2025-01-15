# Software 

The [software of the platform](src/webapp) follows a client-server architecture and runs on a Raspberry Pi 32-bit operating system (OS). Therefore, the physical hardware is controlled by a server application written in Python, where specific scripts handle each module:

- Core Board: `pi_webapp.py`, `picommon.py`
- Strobe Imaging Module: `camera_pi.py`, `pistrobe.py`, `pistrobecam`
- Pressure and Flow Control Module: `piflow.py`, `piflow_web.py`
- Heating and Stirring Module: `piholder.py`, `piholder_web.py`

The Graphical User Interface (GUI) is the client application. It is based on the HTTP method and [HTML framework](src/webapp/templates) and allows simple interaction in a web environment.

Using this software approach, local and remote control is possible through Internet protocol (IP) networks, and new systems can be implemented easily using the Python script approach. 

## Installation

### Simple installation
The simplest way to set up the platform is to download the pre-built SD card image that contains a full operating system with all the necessary software pre-installed:

<details>
  <summary>Instructions</summary>

#### STEP 1: Download Custom OS
* Click [here](https://drive.google.com/file/d/1Vi45Qx171UzSz13v-zXnuIWUdMhgsN83/view?usp=sharing) to download the custom image. **Required:** Raspberry Pi 4 Model B (2GB RAM) or better.

#### STEP 2: Write your SD Card
* Connect your MicroSD card to your computer. **Required:** a 8GB MicroSD card or better.
* Use a program such as [Etcher](https://www.balena.io/etcher) or [Raspberry Pi imager](https://www.raspberrypi.com/software/) to flash the .img file onto your MicroSD card.

More detailed information on installing a Raspberry Pi operating system image onto an SD card can be found [here on the Raspberry Pi website](https://www.raspberrypi.com/documentation/computers/getting-started.html#installing-the-operating-system).

#### STEP 3: First boot and use

* Mount the microSD card with the OS on your Pi.
* Connect your Pi to a display, mouse, keyboard, and power source. When the Pi first boots, you could be asked to complete a quick setup or install updates.
* Click on the Desktop shortcut to start using the web UI.

#### STEP 4: Captured images

* Go to this directory `/home/pi/webapp/snapshots` to find and download the captured images.

</details>

### Advanced installation
If you are familiar with command lines, follow the instructions below:

<details>
  <summary>Instructions</summary>

#### STEP 1: Write your SD Card
You must install an operating system first to set up and use the Raspberry Pi board. Bullseye OS is a Debian-based computer operating system for the Raspberry Pi board. Raspberry Pi OS 32-bit must be installed.

* **Before you start, please make sure you have:**

    * A computer with Internet
    * A MicroSD card
    * A SD card reader (OPTIONAL) 
    * A SD card adapter (OPTIONAL)

* **To write your MicroSD card:**

    * Download the recommended .img file from [Raspberry Pi website](https://www.raspberrypi.com/software/operating-systems/) or install [Raspberry Pi imager](https://www.raspberrypi.com/software/) on your computer to access to an image.
    * Use an adapter to connect your MicroSD card to your computer.
    * Use a program such as [Etcher](https://www.balena.io/etcher) or [Raspberry Pi imager](https://www.raspberrypi.com/software/) to flash the .img file onto your MicroSD card.

    More detailed information on installing a Raspberry Pi operating system image onto an SD card can be found [here on the Raspberry Pi website](https://www.raspberrypi.com/documentation/computers/getting-started.html#installing-the-operating-system).

* **First boot:**
    
     Connect your Pi to a display, mouse, keyboard, and power source. When the Pi first boots, you will be asked to complete a quick setup before rebooting.

Once you have installed and configured the Raspberry Pi OS, you can install the User Interface App.

#### STEP 2: Enable SSH, SPI, VNC and Camera

* **Method 1:** Use the graphical tool "Raspberry Pi Configuration". This is found under Menu > Preferences > Raspberry Pi Configuration. Then you must select the "Interfaces" tab and set SSH, SPI, VNC, and Camera to "Enabled".

* **Method 2:** From the command line or Terminal window, start by running `sudo raspi-config`. This will launch the raspi-config utility:

    - Select “Interfacing Options”.
    - Highlight the "SSH" option and activate **Select**.
    - Select and activate **Yes**.
    - Highlight and activate **Ok**.
    - When prompted to reboot highlight and activate **Yes**.
    - Repeat the previous steps to enable "SPI" and "Camera".


#### STEP 3: Edit config.txt

* We need to edit the config file `sudo nano /boot/config.txt`

* Add `disable_camera_led=2` at the end of the file  

* Then, save it with `CTL+O` > `Enter` > `CTL+X` and reboot the system `sudo reboot`.

#### STEP 4: Plug in Camera

How to connect properly a Raspberry Pi camera and more information about the sensor can be found [here on the Raspberry Pi website](https://www.raspberrypi.com/documentation/accessories/camera.html). Use `vcgencmd get_camera` to verify if your camera was detected and `raspistill -o test.jpg` if it works.

>**CAUTION:** Cameras are sensitive to static. Earth yourself before handling the PCB. A sink tap or similar should suffice if you don’t have an earthing strap.

#### STEP 5: Install Libraries

The Python libraries that allow the Webb App to work on your Pi are the followings:

> Use `pip list` to verify which modules are installed and their versions.

```
Flask 1.0.2
Flask-socketio 4.3.2
Werkzeug 0.14.1
Jinja2 2.10
Markupsafe 1.1.0
itsdangerous 0.24
eventlet 0.33.3
spidev 3.6
RPi.GPIO 0.7.1
picamera 1.13
Pillow 9.4.0
```
* **Method 1:** Use the terminal window and install each of the above libraries with `pip install`. Example: `pip install Flask==1.0.2`

* **Method 2:** Download [requirements.txt](https://github.com/wenzel-lab/modular-microfluidics-workstation-controller/blob/documentation/beta/user-interface-software/src/requirements.txt) file and copy it into your Home folder on your Pi. Use the terminal window and install all the libraries with `pip install -r requirements.txt` 

#### STEP 6: Set up the GPIO pin states

GPIO pins states must be modified during the bootup sequence to use them with the peripherals of the workstation. Our own custom `dt-blob.bin` file specifies which pin states should change:

* Download the [dts file](https://github.com/wenzel-lab/modular-microfluidics-workstation-controller/blob/documentation/beta/RPi-HAT-extension-board/others/pi_config/dt-blob.dts) and copy it into your Home folder.
* Install the Device Tree compiler by running `sudo apt install device-tree-compiler`
* Run the `dtc` command `sudo dtc -I dts -O dtb -o /boot/dt-blob.bin dt-blob.dts`

> It is very useful to use the command `raspi-gpio get` to look at the setup of the GPIO pins to check that they are as you expect.

#### STEP 7: Run the Web UI

Download the [folder](https://github.com/wenzel-lab/modular-microfluidics-workstation-controller/tree/documentation/beta/user-interface-software/src/webapp) and copy it into your Home folder.

* **Method 1:** Go to the web app folder and double click on `pi_webapp.py`. A programming editor will open the Python file, then run the code.

* **Method 2:** Use the terminal window, then `cd webapp` and `python pi_webapp.py`

Use a browser and go to `https://0.0.0.0:5000` to use the Web UI

#### STEP 8: Run the Web UI on startup

Once the folder and Python files are in your Home folder, configure the Raspberry Pi to run the web UI on startup.

* We must edit the `rc.local` file `sudo nano /etc/rc.local`

* Add `sudo -H -u pi python3 /home/pi/webapp/pi_webapp.py &` before `exit 0` on the file

* Then, save it with `CTL+O` > `Enter` > `CTL+X`, and reboot the system `sudo reboot`.

When the Raspberry Pi boot again, use a browser and go to `https://0.0.0.0:5000` to open the Web App.

> **Note:** While the program is running in the background, you will not be able to use the camera. First, you must kill the process related to the program. To identify the process (number of PID), use `ps aux`. Then, use `sudo kill -9 PID` where `PID` is the process. If you want to run the program again, repeat **STEP 7** or reboot the system.

</details>
