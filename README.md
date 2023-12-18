# W65C02-COM bridge for Teensy 4.1

This project is a simple bridge between [W65C02 CPU](https://westerndesigncenter.com/wdc/documentation/w65c02s.pdf)
and a COM port, implemented for [Teensy 4.1](https://www.pjrc.com/store/teensy41.html) development board.

The main purpose of this project is to provide cycle-by-cycle debugging capabilities for W65C02, but it can serve
other purpose too, i.e.:

- __Comparative debugging for emulators__:
  You can run your 6502-family emulator in parallel with an actual CPU to compare the results
  in order to measure the accuracy of your emulator.
- __Emulated 8-bit computers with a physical processor__:
  You can implement an emulator of an 8-bit, 6502-based computer (i.e. C64), that will work
  (via this bridge) with a real CPU, while other components of the system (like RAM, ROM,
  video chip) remain emulated
  (something like [Neo 6502](https://www.olimex.com/Products/Retro-Computers/Neo6502/open-source-hardware)).

## Wiring

The CPU is a 40-pin chip, with 37 data pins (2 pins are for
power/ground and pin 35 is unused). As Teensy board is equipped with 38 digital input/output pins,
it seems to be a perfect match for this project.

Below there is a pinout diagram of W65C02 CPU and Teensy 4.1.

```text
                                                   +-------------+
                                                  -| GND     VIN |-
            +------------+                        -|  0      GND |-
    VP/ <-- |  1      40 | <-- RES/               -|  1     3.3V |-
    RDY <-> |  2      39 | --> PHI2O              -|  2       23 |-
  PHI1O <-- |  3      38 | <-- SO/                -|  3       22 |-
   IRQ/ --> |  4      37 | <-- PHI2               -|  4       21 |-
    ML/ <-- |  5     @36 | <-- BE                 -|  5       20 |-
   NMI/ --> |  6      35 | --- NC                 -|  6       19 |-
   SYNC <-- |  7     *34 | --> RW/                -|  7       18 |-
    VDD --> |  8     *33 | <-> D0                 -|  8       17 |-
     A0 <-- |  9*    *32 | <-> D1                 -|  9       16 |-
     A1 <-- | 10*    *31 | <-> D2                 -| 10       15 |-
     A2 <-- | 11*    *30 | <-> D3                 -| 11       14 |-
     A3 <-- | 12*    *29 | <-> D4                 -| 12       13 |-
     A4 <-- | 13*    *28 | <-> D5                 -| 3.3V    GND |-
     A5 <-- | 14*    *27 | <-> D6                 -| 24       41 |-
     A6 <-- | 15*    *26 | <-> D7                 -| 25       40 |-
     A7 <-- | 16*    *25 | --> A15                -| 26       39 |-
     A8 <-- | 17*    *24 | --> A14                -| 27       38 |-
     A9 <-- | 18*    *23 | --> A13                -| 28       37 |-
    A10 <-- | 19*    *22 | --> A12                -| 29       36 |-
    A11 <-- | 20*     21 | --> GND                -| 30       35 |-
            +------------+                        -| 31       34 |-
                                                  -| 32       33 |-
                                                   +-------------+

    * - tri-state pin,   @ - async,  / - active on low
```

### Minimal configuration

In the minimalistic configuration the following CPU pins must be connected to the board:

- `A0-A15` - address bus (input)
- `D0-D7` - data bus (input or output; input by default)
- `RW` - read/write input; it informs whether the data bus is in read (high) or write (low) state
- `PHI2` - clock signal input
- `GND` - ground, should be connected with Teensy's ground pin
- `VDD` - power; should be connected with Teensy's 3.3V pin

In such configuration `RES`, `IRQ`, `NMI` and `BE` pins must be also connected to 3.3V
and `RDY` pin should be connected with power via resistor.

Please note that the `RES` (pin 40) must be kept low for at least two cycles on power on.
The bridge handles it programmatically, but if your intention is not to connect RST pin to
Teensy or to have a physical control over the reset state (i.e. in case of power failure),
then the best option is to connect the pin via
[DS1813](https://www.mouser.co.uk/datasheet/2/609/DS1813-3122044.pdf)

### Default configuration

The table below illustrates the default configuration of this project.
In order to adjust the pin mapping, modify the `PINS_MAP` macro definition in the
[configuration file](./configuration.h).

| Teensy pin | CPU pin | CPU pin name | ←  → | CPU Pin name  | CPU pin | Teensy pin |
| ---------- | ------- | ------------ | ---- | ------------- | ------- | ---------- |
|  2         | 1       | Vector pull  |      | Reset         | 40      | 18         |
| 21         | 2       | Ready        |      | PHI2O         | 39      |  1         |
|  3         | 3       | PHI1O        |      | Set overflow  | 38      | 17         |
| 20         | 4       | IRQ          |      | PHI2          | 37      | 16         |
|  4         | 5       | Memory lock  |      | Bus enable    | 36      | 15         |
| 19         | 6       | NMI          |      | No connection | 35      |            |
|  5         | 7       | SYNC         |      | Read/Write    | 34      | 14         |
| 3.3V       | 8       | VDD          |      | D0            | 33      | 40         |
|  6         | 9       | A0           |      | D1            | 32      | 39         |
|  7         | 10      | A1           |      | D2            | 31      | 38         |
|  8         | 11      | A2           |      | D3            | 30      | 37         |
|  9         | 12      | A3           |      | D4            | 29      | 36         |
| 10         | 13      | A4           |      | D5            | 28      | 35         |
| 11         | 14      | A5           |      | D6            | 27      | 34         |
| 12         | 15      | A6           |      | D7            | 26      | 33         |
| 24         | 16      | A7           |      | A15           | 25      | 29         |
| 25         | 17      | A8           |      | A14           | 24      | 30         |
| 26         | 18      | A9           |      | A13           | 23      | 31         |
| 27         | 19      | A10          |      | A12           | 22      | 32         |
| 28         | 20      | A11          |      | Ground        | 21      | GND        |

### Example

![Example wiring](./assets/board.jpg)
The photo demonstrates the complete configuration, as per table above (all CPU pins are
connected to Teensy, apart pin 35).
The CPU pin 8 (VDD) is connected via [decoupling capacitor](https://en.wikipedia.org/wiki/Decoupling_capacitor).
Additionally there are some LEDs to indicate some of the signals.

### Warning

Incorrect connection may destroy the CPU. Please take extra attention to the
correctness of your wiring, and double-check it before powering up your board.

## Compilation and execution

### With Arduino IDE and Teensyduino

1. Install [Arduino IDE](https://www.arduino.cc/en/software)
1. Install and configure [Teensyduino](https://www.pjrc.com/teensy/teensyduino.html)
1. Clone this project into your Arduino sketches folder (usually `~/Arduino/sketches`)
1. Click menu `Sketch / Compile` to compile
1. Click menu `Sketch / Upload` tu upload to Teensy board

### With Arduino CLI

Follow [this post](https://forum.pjrc.com/index.php?threads/arduino-cli-and-ide-now-released-teensy-supported.53548/page-5#post-299430) to see how to configure Arduino CLI with Teensy.

- to compile: `make`
- to upload `make upload`

Make sure that the port name in `Makefile` is the correct one. To find the port name execute
`arduino-cli board list`. You must have Teensy connected with your computer to have the port visible.

##### Install Arduino CLI and dependencies with Nix and Direnv

If you are not using Nix packages, then you don't know how much you miss in terms of convenience and reproducible dev environments - [start](https://nixos.org/download) today :-)

1. Clone the repo and cd project's directory
1. If you use [Direnv](https://direnv.net/), just execute `direnv allow`, otherwise type `nix-shell`
1. Wait until all dependencies download and configure
1. I added [`treefmt`](https://github.com/numtide/treefmt) to dependencies, so you can format the
   code after changes by just executing `treefmt`

## Example

There is complete example program in the [examples folder](./examples), that demonstrates
how to execute 6502 binary with the bridge and RAM emulated on the host machine
(not on Teensy). See the [Readme file](.examples/README.md) for details.

## Working with other CPUs from the 6502 family

This project is meant to work specifically with W65C02 CPU. The main reason
is that W65C02 is static, that means it can be easily step-by-step cycled at any speed,
that is very useful in case of debugging. Some other CPUs, like C64's MOS6510, have
limitations of a minimum speed (~100kHz). This bridge would still work with these CPUs
(after some adjustments - see below), but more in a "runtime" mode, rather than for debugging.

Perhaps a good workflow could be to test/debug your project with W65C02 and then
switch to your desired CPU.

Please note that other processors from the 6502 family have different pin layout,
so this code needs to be adjusted in order to make them work.

## References

- [W65C02 datasheet](https://westerndesigncenter.com/wdc/documentation/w65c02s.pdf)
- [Teensy 4.1](https://www.pjrc.com/store/teensy41.html)
- [6502 Primer: What do I do with the "mystery" pins](https://wilsonminesco.com/6502primer/MysteryPins.html)
