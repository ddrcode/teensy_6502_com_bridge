#include "memory.h"

#define BUFFSIZE 5

class Runner
{
private:
    Memory *mem;
    bool phase;
    int device;
    uint64_t cycle;
    uint8_t buff[BUFFSIZE];

    void read_serial();
    void write_serial();
    void advance_cycles();

public:
    Runner(int device, Memory *mem);
    bool step();
    void run();
    void print_state();

    bool is_write();
};
