#include <iostream>
#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <sstream>

#include "configuration.h"
#include "runner.h"

int main(int argc, char *argv[])
{
    int device = open(std::string(PORT).c_str(), O_RDWR | O_NOCTTY | O_SYNC);
    Memory mem;
    Runner runner = Runner(device, &mem);
    runner.run();
    return 0;
}
