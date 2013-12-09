DivRGBCube
==========

8x8x8 RGB Cube Project Resources

Aiming to supply other makers with resources to build my 8^3 RGB LED cube design. Some of the design choices were inspired by others and have been credited appropriately, but this is mostly my own custom design and built completely from scratch. I've provided a custom board layout designed in eagle, which has some minor flaws (nothing I would consider a fault however) which I've noted accordingly, but otherwise I'm using the as-designed board and have yet to encounter a problem. I've also written the software to satisfy my own needs and knowledge level.

All code is written in standard C using Microchip's MPLAB X IDE. I am not a seasoned C veteran by any means so please feel free to suggest alternative or more efficient ways of doing things, I am sure there are plenty of areas which could use some improving and optimization. All build files are included in addition to the source.

The target microcontroller platform is a PIC32. There should be only minimal hassles to port the code to other platforms - I believe the only major portion that would require rewrites would be the interrupt handlers and of course the I/O pin definitions and latch functions (for example the TRIS and LATx macros). The PIC32 was chosen due to its fast speed given that I'm using shift registers for each LED cathode and handling the color fading in software using BCM (aka BAM) thus requiring a very fast refresh rate to avoid flickering artifacts and to maintain a good frame rate. Credits to Nick Schulze of www.hownottoengineer.com for this particular design inspiration. The implementation of software BCM was a fun challenge.

This is still a work in progress project so I'm constantly adding new features so stay tuned for new builds and updates.

