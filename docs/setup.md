[Back to main page](./index.html)

# Installation instructions

## Setup the Raspberry Pi

1. Install the newest Raspberry Pi OS on a SD card. 
2. Insert the SD card and boot your Raspberry Pi. Setup the desktop, login to wifi and install your favorite software.
3. You can find a beginners guide here: [www.raspberrypi.org](https://projects.raspberrypi.org/en/projects/raspberry-pi-setting-up). 

## Install SmallBASIC

Right now there aren't any SmallBasic packages or executable files for the Raspberry Pi. You have to compile SmallBASIC by yourself. Don't be afraid, it is fairly easy.

Open a console and copy paste the following commands one at a time to build the SDL2-version:
  
```
cd ~
sudo apt-get install git autotools-dev automake gcc g++ libsdl2-dev libfreetype6-dev libfontconfig1-dev xxd libdrm-dev libgbm-dev
git clone https://github.com/smallbasic/SmallBASIC.git
cd SmallBASIC
git submodule update --init
sh autogen.sh
./configure --enable-sdl
make
sudo make install
```
The "make" command will take quite a while. Don't get unpatient. After you finished the installation, you can type "sbasicg" in the console and SmallBASIC should start.

Usually it is quite nice to use the console version of SmallBASIC, especially if you don't have any graphical output to the computer screen. To build the console version copy and paste the following commands one at a time after you build the SDL2-version:

```
./configure
make
sudo make install
```

## Install SmallBasicPIGPIO

The SmallBasicPIGPIO plugin is already compiled for 32 bit system and available as a library. Since you used already "git" to install SmallBASIC, we will again use it to install the plugin. Open again a console and copy paste the following commands one at a time:

```
cd ~
git clone https://github.com/Joe7M/SmallBasicPIGPIO.git
cd SmallBasicPIGPIO
ls
```
Now you can see the following directories: 

- bin: it containes the plugin library libSmallBasicPIGPIO.so
- examples: some example basic files
- src: the source code. 

If you want to compile the plugin by yourself if you have Raspberry Pi OS 64bit, just enter this directory and type `make`.

## Start SmallBasic with the plugin

Start a console and type:

```
sudo sbasicg --module-path=/home/pi/SmallBasicPIGPIO/bin/ -r 'file'
```
For example you want to execute led.bas in the example folder:
```
cd ~
cd SmallBasicPIGPIO/examples
sudo sbasicg --module-path=/home/pi/SmallBasicPIGPIO/bin/ -r led.bas
```

**To access the GPIO pins you need admin rights. Therefore start "sbasicg" always with the "sudo" command.**

Now you can start coding your own programs. Check out the examples on this website for helpfull informations.

[Back to main page](./index.html)
