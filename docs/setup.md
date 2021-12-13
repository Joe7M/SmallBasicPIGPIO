# Installation instructions

## Setup the Raspberry Pi

1. Install the newest Raspberry Pi OS on a SD card. You can find a guide at [www.raspberrypi.com](https://www.raspberrypi.com/software/).
1. Insert the SD card and boot your Raspberry Pi. Setup the desktop, login to wifi and install your favorite software.

## Install SmallBASIC

Right now there aren't any SmallBasic packages or executable files for the Raspberry Pi. You have to compile SmallBASIC by yourself. Don't be afraid, it is fairly easy.

Open a console and copy paste the following commands one at a time:
  
```
cd ~
sudo apt-get install git autotools-dev automake gcc g++ libsdl2-dev libfreetype6-dev libfontconfig1-dev xxd
git clone https://github.com/smallbasic/SmallBASIC.git
cd SmallBASIC
git submodule update --init
sh autogen.sh
./configure --enable-sdl
make
sudo make install
```
The "make" command will take quite a while. Don't get unpatient. After you finished the installation, you can type "sbasicg" in the console and SmallBASIC should start.

## Install SmallBasicPIGPIO

The SmallBasicPIGPIO plugin is already compile and available as a library. Since you used already git to install SmallBasic, we will again use it to install the plugin. Open again a console and copy paste the following commands ome at a time:

```
cd ~
git clone https://github.com/Joe7M/SmallBasicPIGPIO.git
cd SmallBasicPIGPIO
ls
```
Now you can see the following directories: 

- bin: it containes the plugin library libSmallBasicPIGPIO.so
- examples: some example basic files
- src: the source code. If you want to compile the plugin by yourself, just enter this directory and type "make"

## Start SmallBasic with the plugin

Start a console and type:

```
sbasicg -m /home/pi/SmallBasicPIGPIO/bin/
```

Now you can start coding your own programs. Check out the examples on this website for helpfull informations.
