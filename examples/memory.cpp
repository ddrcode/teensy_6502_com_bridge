#include "memory.h"

Memory::Memory()
{
    // hardcode reset vector to $0200
    this->write_byte(0xfffd, 0x02);
    this->write_byte(0xfffc, 0x00);
}

uint8_t Memory::read_byte(uint16_t addr)
{
    return this->data[addr];
}

void Memory::write_byte(uint16_t addr, uint8_t val)
{
    this->data[addr] = val;
}
