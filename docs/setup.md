## Installation instructions

# Setup the Raspberry Pi

1. Install the newest Raspberry Pi OS on a SD card. You can find a guide at [www.raspberrypi.com](https://www.raspberrypi.com/software/).
1. Insert the SD card and boot your Raspberry Pi. Setup the desktop, setup wifi and install your favorite software.

# Install SmallBASIC

Right now there aren't any packages or executable files for the Raspberry Pi. You have to compile SmallBASIC by yourself. Don't be afraid, it is fairly easy.

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
make install
```
The "make" command will take quite a while. Don't get unpatient. After you finished the installation, you can type "sbasicg" in the console and SmallBASIC should start.
  
