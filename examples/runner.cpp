#include <iostream>
#include <unistd.h>

#include "pins.hpp"
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

/**
 * CPU reset procedure. It requires to keep the CPU reset pin (40)
 * low for two cycles. After that this pin is set high.
 */
void Runner::reset()
{
    uint8_t b[] = {
        0b00001010, // BE, RW high
        0b00000000,
        0b00000000,
        0b00000000,
        0b10101000  // IRQ, NMI, VDD high
    };
    this->pins = W65C02Pins(b);

    this->step();
    this->step();

    this->pins.reset = true;
    this->pins.ready = true;
    this->pins.set_overflow = true;
}

bool Runner::step()
{
    this->pins.phi2 = this->phase;
    this->write_serial();
    usleep(100000);
    this->advance_cycles();

    this->read_serial();
    if (this->pins.is_write()) {
        this->mem->write_byte(this->pins.addr, this->pins.data);
    } else {
        this->pins.data = this->mem->read_byte(this->pins.addr);
    }
    this->print_state();

    usleep(100000);
    this->advance_cycles();

    return this->cycle < 500;
}

void Runner::run()
{
    this->reset();
    while (this->step()) {
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
         << ", VP: " << (int)this->pins.vector_pull
         << ", SYNC " << (int)this->pins.sync
         << ", IRQ " << (int)this->pins.irq
         << ", NMI " << (int)this->pins.nmi
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
