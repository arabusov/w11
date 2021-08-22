# Install Cypress FX2 Support

The Nexys2 and Nexys3 board feature a Cypress FX2 USB interface. It allows
to configure the FPGA and to transfer between FPGA and a PC. The retro
project uses a custom firmware in the FX2, this writeup describes the
installation of tools, environment setup and generation of the FX2 firmware.

### Table of content

- [System requirements](#user-content-sysreq)
- [Setup environment variables](#user-content-envvar)
- [Setup USB access](#user-content-usb-access)
- [Rebuild Cypress FX2 firmware](#user-content-fx2-firmware)

### <a id="sysreq">System requirements</a>
  
the download contains pre-build firmware images for the Cypress FX2
USB Interface. Re-building them requires
- Small Device C Compiler -> package: `sdcc` `sdcc-ucsim`
- for FX2 firmware download and jtag programming over USB one needs
  - fxload -> package: `fxload`
  - urjtag -> package: `urjtag` for Ubuntu 12.04 or 14.04,
      is **broken in Ubuntu 16.04**.
      See [INSTALL_urjtag.md](INSTALL_urjtag.md) for installation from sources !!

### <a id="envvar">Setup environment variables</a>

The default USB VID and PID is defined by two environment variables. 
For internal lab use one can use

    export RETRO_FX2_VID=16c0
    export RETRO_FX2_PID=03ef

> **Carefully read the disclaimer about usage of USB VID/PID numbers  
> in the file [README_USB-VID-PID.md](README_USB-VID-PID.md). You'll be responsible for a  
> misuse of the defaults provided with the project sources.  
> Usage of this VID/PID in any commercial product is forbidden.**

### <a id="usb-access">Setup USB access</a>

For using the Cypress FX2 USB interface on Digilent Nexys2, Nexys3 and
Atlys boards `udev` rules must be setup to allow user level access to
these devices. A set of rules is provided under

    $RETROBASE/tools/fx2/sys

Follow the [README.md](../tools/fx2/sys/README.md) file in this directory.

Notes:
- the provided udev rules use the VID/PID for **internal lab use** as
  described above. If a different VID/PID is used the file must be modified.
- your user account must be in group `plugdev` (should be the default).

### <a id="fx2-firmware">Rebuild Cypress FX2 firmware</a>

The download includes pre-build firmware images for the Cypress FX2
USB interface used on the Digilent Nexys2, Nexys3 and Atlys Boards.
These firmware images are under

    $RETROBASE/tools/fx2/bin

To re-build them, e.g. because a different USB VID/PID is to be used

    cd $RETROBASE/tools/fx2/src
    make clean
    make
    make install

Note: The default build assumes that sdcc with a version 3.x is installed.
In case sdcc 2.x is installed use

    make SDCC29=1

instead. See also [tools/fx2/src/README.md](../tools/fx2/src/README.md).

Please read [README_USB-VID-PID.md](README_USB-VID-PID.md) carefully to
understand the usage of USB VID and PID.

