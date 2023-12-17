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

void Runner::reset() {
    // set_data();
}

bool Runner::step()
{
    this->read_serial();
    // auto addr = decode_addr(this->buff);
    // if (this->is_write()) {
    //     this->mem->write_byte(addr, decode_data(this->buff));
    // } else {
    //     set_data(this->buff, this->mem->read_byte(addr));
    // }

    this->print_state();
    this->advance_cycles();

    return this->cycle < 20;
}

void Runner::run()
{
    this->reset();

    uint8_t b[] = {
        0b11111110,
        0b00000010,
        0b11101111,
        0b00000001,
        0b00010010
    };

    cout << (uint16_t)pin(b, 9) << endl;

    while (this->step()) {
        usleep(250000);
    }
}

void Runner::print_state()
{
    cout << "[Cycle " << dec << this->cycle
         // << "] Pins: " << pins_to_string(this->buff)
         << ", Addr:" << hex << this->pins.addr
         << " (" << addr_to_binary_str(this->pins.addr)
         << "), Data: " << hex << (uint16_t)this->pins.data
         << ", R/W: " << (this->pins.rw ? "R" : "W")
         << endl;
}

void Runner::read_serial()
{
    uint8_t buff[5];
    for (int n=0; !n;) {
        n = read(this->device, buff, BUFFSIZE);
    }
    this->pins.set_pins(buff);
}

void Runner::write_serial()
{
    uint8_t buff[5];
    this->pins.set_buff(buff);
    auto _ = write(this->device, buff, BUFFSIZE);
}

void Runner::advance_cycles()
{
    if (this->phase) {
        ++this->cycle;
    }
    this->phase = !this->phase;
}

