# Onion Omega2 Wii classic controller library

### Description
This library enables the Omega2(+) to communicate with a WiiClassic controller via I2C.

### Hardware Wireup
You will need:
* an Onion Omega2 or Omega2+, preferably with the extension dock
* a Wii classic controller 
* an adapter board for the controller's connector (e.g., [Adafruit's nunchucky][adafruit-nunchucky])
* breadboard and wires

From the adapter board, connect GND to Omega's GND, VCC to 3.3V, Data to SDL and Clock to SCL. Do not insert the adapter the wrong way up, or else GND and 3.3V will be exchanged and your controller will be damaged through reverse polarity. 

### Library Usage

First, you need to initialize the controller by calling `WiiClassic_Init()`. The functions returns a `bool` which indicates the presence of the controler. For reading data, allocate a `wiiclassic_status_t` object and give a pointer to it to `WiiClassic_ReadStatus()`. Optionally printing the controler values by calling `WiiClassic_PrintStatus()` or access the members of the `wiiclassic_status_t` object directly. 

```c
/* Initializes the WiiClassic controller */
bool WiiClassic_Init();
/* reads the status of the WiiClass controller into the structure */
bool WiiClassic_ReadStatus(wiiclassic_status_t* status);
/* prints the status to stdout */
void WiiClassic_PrintStatus(wiiclassic_status_t* status);
``` 
For an example, see the `main.c` in this repository.

### Example output
```
Buttons: A B
Shoulder: L 
DPAD: UP 
Analog stick left/right: (1.000000, 0.500000), (0.500000, 0.500000)
```

### Compilation

This project was designed for cross-compliation. Compile your toolchain according to https://docs.onion.io/omega2-docs/cross-compiling.html, change the paths in the `Makefile` and do a `make all`. Optionally, `make upload` will attempt to use `sshpass` with `scp` to transfer the compiled binary to your Omega Onion2 system. Simply change the IP address and the password if you whish to use this feature.
For resons of simplicity, the header and library files for the I2C functionality has been included in this project. You can also compile the project by yourself (https://github.com/OnionIoT/i2c-exp-driver/).

After executing `make all`, the library file `libwiiclassic.so` and a executable file `main` will be created. If you wish to to use the dynamic library in your own project, you have to:
1. set the include path to where the `WiiClassic.h` is (`-I somedir`)
2. set the library path to where the `libwiiclassic.so` and the dependency `libonioni2c.so` and `liboniondebug.so` is
3. set the linker flags to link your executable against the prementioned dependencies (`-lwiiclassic -lonioni2c -loniondebug`) 

### Compatibility
This library has only been tested with an original Nintendo Wii classic controller. Clones might or might not work.

### Credits
* Maximilian Gerhardt, library porter
* Andrew Mascolo, [original library][github-wiiclassic]
* Frank Zhao, for the [instructable on the Wii classic][instr-wiiclassic]

![hardware setup](https://raw.githubusercontent.com/gamer-cndg/omega2-wii-classic-controller/master/hw.jpg)
![example execution](https://raw.githubusercontent.com/gamer-cndg/omega2-wii-classic-controller/master/shell.png)


[adafruit-nunchucky]: <https://www.adafruit.com/product/345>
[github-wiiclassic]: <https://github.com/AndrewMascolo/WiiClassicController>
[instr-wiiclassic]: <http://www.instructables.com/id/USB-Wii-Classic-Controller/>