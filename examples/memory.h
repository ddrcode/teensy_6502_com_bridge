#include <cstdint>
#include <string>

class Memory
{
private:
    uint8_t data[1<<16];

public:
    Memory();
    uint8_t read_byte(uint16_t addr);
    void write_byte(uint16_t addr, uint8_t val);
    void load_program(std::string filename, uint16_t start_addr);
};
