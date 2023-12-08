#include <cstdint>
#include <string>

class Memory
{
private:
    uint8_t data[1<<16];
    uint16_t program_addr;

public:
    Memory(const uint16_t program_addr);
    uint8_t read_byte(uint16_t addr);
    void write_byte(uint16_t addr, uint8_t val);
    void load_program(std::string filename);
};