#include <iostream>
#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <sstream>

void testSerialComm()
{
    std::string port = "/dev/cu.usbmodem122057101";
    int device = open(port.c_str(), O_RDWR | O_NOCTTY | O_SYNC);

    uint8_t buffer[5];

    do {
        int n = read(device, buffer, sizeof buffer);

        // if (n > 0) {
            std::cout << buffer;
        // }

    } while (buffer[0] != 'X'); // 'X' means end of transmission

    close(device);
}

int main(int argc, char *argv[])
{
    testSerialComm();
    return 0;
}
