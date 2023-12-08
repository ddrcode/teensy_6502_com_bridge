#include <iostream>
#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <sstream>

#include "configuration.hpp"
#include "runner.hpp"

int main(int argc, char *argv[])
{
    // open COM port device
    int device = open(std::string(PORT).c_str(), O_RDWR | O_NOCTTY | O_SYNC);

    // instantiate memory emulation and load example program
    Memory mem(PROGRAM_ADDR);
    mem.load_program(PROGRAM_FILE);

    // execute program on connected CPU with emulated RAM
    Runner runner = Runner(device, &mem);
    runner.run();

    return 0;
}
