# CH559sdccUSBHost

> Modified from upstream for building on non-Windows machines, and to use 115200 BPS instead of 400000. 
> This is to make it usable with rosco_m68k and our usual build envs / toolchains / etc.

## Building

You'll need a programmer. Clone this repo: https://github.com/frank-zago/isp55e0 
Make sure you have libusb installed (via brew on macOS, or your package manager
on Linux - be sure to have the `devel` package if applicable).

Then do:

```shell
cd isp55e0
clang -O2 -Wall -Werror $(pkg-config --cflags --libs libusb-1.0) -o isp55e0 isp55e0.c
```

You can put the resulting `isp55e0` somewhere in your path if you like (I'll 
assume you have in the following commands).

Now, get sdcc installed:

```shell
brew install sdcc
```

Or you can build it from source if you like, it's not painful. The code is on
Sourceforge, have a google for it. 

Again, make sure it's in your path (or provide a `SDCC_PATH=path/to/sdcc/` option 
to `make` below - **NOTE the trailing slash is important because I'm lazy**).

You should now be all set to build. In this directory, do:

```shell
make all
```

Which should churn away for a bit and output some weird messages about dogs 
or something, and in the end you should have `CH559USB.bin` ready to go.

Now connect up your CH559 adapter - you'll _probably_ need a host-to-host 
USB cable (which is just two USB-A plugs wired back to back, i.e. with both 
data and  power wires crossed) and you'll need to set a jumper or push a 
button to get it into bootloader mode. 

**See your adapter's docs for details!** If you blow up your adapter because
you couldn't be bothered to RTFM, that's totally on you - don't come crying to 
me because I promise you I do not care.

Once it's connected, you should be able to program it. Adjust the path
to the `isp55e0` program if you didn't add it to your path.

```shell
isp55e0 -f CH559USB.bin
```

## Original README follows

Code for the CH559 chip from WCH to use as a USB host, you can connect a mouse or keyboard to it and it will output a simple protocol via serial port 1.

you can edit the files and compile it via the compile.bat on windows, sdcc is included, so no extra tools needed.

This repo is made together with this explanation video:(click on it)
[![YoutubeVideo](https://img.youtube.com/vi/Th88RiSmj2w/0.jpg)](https://www.youtube.com/watch?v=Th88RiSmj2w)

NEW: the chflasher.exe can be used to upload a bin firmware to ch55x chips, it will be uploaded directly after compile, no WCH tool needed anymore.
Please install the libusb-win32 driver with the zadig tool ( https://zadig.akeo.ie/ ) to enable access to usb for the tool.

Including Arduino Example code to either output the data to serial or onto an st7789 display.

