# Example - program runner

The program in this folder executes 6502 binary, using the
Teensy bridge. As 6502 needs RAM for functioning, the memory
is emulated with a simple byte array in the program.

## Compilation and execution

1. Plug your Teensy with running Bridge app from this repo.
   It's sufficient to have the CPU connected only with address, data,
   Phi2 and Read/Write pins.
1. Check Teensy's port name (`arduino-cli board list`)
   and update `configuration.hpp` file, if needed.
1. Compile the example: `g++ -o example.out *.cpp`
1. Execute the example app: `./example.out`

## How does the example work?

## Test program
