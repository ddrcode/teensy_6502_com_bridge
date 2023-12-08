#include <iostream>
#include <unistd.h>

#include "runner.hpp"
#include "pin_utils.hpp"

using namespace std;

Runner::Runner(int device, Memory *mem)
{
    this->device = device;
    this->mem = mem;
    this->phase = false;
    this->cycle = 0;
}

bool Runner::step()
{
    this->read_serial();
    auto addr = decode_addr(this->buff);
    if (this->is_write()) {
        this->mem->write_byte(addr, decode_data(this->buff));
    } else {
        set_data(this->buff, this->mem->read_byte(addr));
    }

    this->print_state();
    this->advance_cycles();

    return this->cycle < 20;
}

void Runner::run()
{
    uint8_t b[] = {
        0b11111110,
        0b00000010,
        0b11101111,
        0b00000001,
        0b00010010
    };

    cout << (uint16_t)pin(b, 9) << endl;

    while (this->step());
}

void Runner::print_state()
{
    cout << "[Cycle " << dec << this->cycle
         << "] Pins: " << pins_to_string(this->buff)
         << ", Addr: " << hex << decode_addr(this->buff)
         << ", Addr: " << addr_to_binary_str(decode_addr(this->buff))
         << ", Data: " << hex << (uint16_t)decode_data(this->buff)
         << ", Flags: " << (this->is_write() ? "W" : "R")
         << endl;
}

void Runner::read_serial()
{
    for (int n=0; !n;) {
        n = read(this->device, this->buff, BUFFSIZE);
    }
}

void Runner::write_serial()
{
    auto _ = write(this->device, this->buff, BUFFSIZE);
}

void Runner::advance_cycles()
{
    if (this->phase) {
        ++this->cycle;
    }
    this->phase = !this->phase;
}

inline bool Runner::is_write()
{
    // write is active when signal is low
    return pin(this->buff, 33) == 0;
}
