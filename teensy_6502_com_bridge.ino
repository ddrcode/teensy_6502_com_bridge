// This code makes Teensy 4.1 a bridge betwen COM port and 65C02 processor.
// Created with Teensyduino

#include <_Teensy.h>
#include "configuration.h"

//--------------------------------------------------------------------------
// TYPES

typedef struct t_w64c02_pins
{
    // cpu control
    int ready;
    int ml;  // memory lock
    int be;  // bus enable
    int so;  // set overflow
    int reset;

    // clock
    int phi1o;
    int phi2;
    int phi2o;

    // interrupts
    int irq;
    int nmi;

    // data
    int rw;
    int addr[16];
    int data[8];

    // monitoring
    int sync;
    int vp;  // vector pull
} pins_t;

pins_t setup_pins(void);

//--------------------------------------------------------------------------
// GLOBAL DATA

int pin_ids[40];
auto pins = setup_pins();

void setup()
{
    Serial.begin(9600);
    pinMode(pins.rw, INPUT);
    // pinMode(pins.irq, OUTPUT);
    // pinMode(pins.nmi, OUTPUT);

    // clock
    // pinMode(pins.phi1o, INPUT);
    pinMode(pins.phi2, OUTPUT);
    // pinMode(pins.phi2o, INPUT);

    // data
    set_data_pins(pins.data, INPUT);
    for (int i = 0; i < 16; ++i) {
        pinMode(pins.addr[i], INPUT);
    }
}

//--------------------------------------------------------------------------
// PUBLIC API

void loop()
{
    digitalToggle(pins.phi2);
    delay(CYCLE_DURATION);
    handle_cycle(pins);
    //print_status(pins);
    pins_to_num();
}

void print_status(pins_t &pins)
{
    Serial.print("RW=");
    Serial.print(digitalRead(pins.rw));
    Serial.print(", Addr=");
    Serial.print(get_val_from_pins(pins.addr, 16), BIN);
    Serial.print(", Data=");
    Serial.print(get_val_from_pins(pins.data, 8), BIN);
    Serial.print(", All pins=");
    //Serial.print(pins_to_num());
    Serial.println();
}

//--------------------------------------------------------------------------
// LOCAL FUNCTIONS

void handle_cycle(pins_t &pins)
{
    auto rw = digitalRead(pins.rw);
    set_data_pins(pins.data, rw == HIGH ? OUTPUT : INPUT);
}

void set_data_pins(int data[8], int direction)
{
    for (int i = 0; i < 8; ++i) {
        pinMode(data[i], direction);
    }
}

int get_val_from_pins(int addr_pins[], int len)
{
    int addr = 0;
    for (int i = 0; i < len; ++i) {
        addr |= digitalRead(addr_pins[i]) == HIGH ? (1 << i) : 0;
    }
    return addr;
}

//            +------------+
//     VP <-- |  1      40 | <-- RES
//    RDY <-> |  2      39 | --> PHI2O
//  PHI1O <-- |  3      38 | <-- SO
//    IRQ --> |  4      37 | <-- PHI2
//     ML <-- |  5     @36 | <-- BE
//    NMI --> |  6      35 | --- NC
//   SYNC <-- |  7     *34 | --> RW
//    VDD --> |  8     *33 | <-> D0
//     A0 <-- |  9*    *32 | <-> D1
//     A1 <-- | 10*    *31 | <-> D2
//     A2 <-- | 11*    *30 | <-> D3
//     A3 <-- | 12*    *29 | <-> D4
//     A4 <-- | 13*    *28 | <-> D5
//     A5 <-- | 14*    *27 | <-> D6
//     A6 <-- | 15*    *26 | <-> D7
//     A7 <-- | 16*    *25 | --> A15
//     A8 <-- | 17*    *24 | --> A14
//     A9 <-- | 18*    *23 | --> A13
//    A10 <-- | 19*    *22 | --> A12
//    A11 <-- | 20*     21 | --> GND
//            +------------+
// void set_buff(pins_t &pins, int buff2[40]) {
//   int buff[] = {
//     0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//     0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//     0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//     0, 0, 0, 0, 0, 0, 0, 0, 0, 0
//   };
// }

void pins_to_num()
{
    char buff[] = { 0,0,0,0,0 };
    int i = 0;
    for (int i=0; i<40; ++i) {
        buff[i/8] |= digitalRead(pin_ids[i]) == HIGH ? (1<<(i/8)) : 0;
    }
    for (i=4; i>=0; --i) {
        Serial.print(buff[i], BIN);
    }
    Serial.println();
}

pins_t setup_pins(void)
{
    set_pin_ids(pin_ids);
    auto p = pin_ids;

    pins_t pins = {
        .ready = p[1],
        .ml = p[4],
        .be = p[35],
        .so = p[37],
        .reset = p[39],
        .phi1o = p[2],
        .phi2 = p[36],
        .phi2o = p[38],
        .irq = p[3],
        .nmi = p[5],
        .rw = p[33],
        .addr = { p[9], p[10], p[11], p[12], p[13], p[14], p[15], p[16], p[17], p[18], p[19], p[21], p[22], p[23], p[24] },
        .data = { p[32], p[31], p[30], p[29], p[28], p[27], p[26], p[25] },
        .sync = p[6],
        .vp = p[0]
    };
    return pins;
}

void set_pin_ids(int ids[40])
{
    int x[40] = { PINS_MAP };
    for (int i = 0; i < 40; i += 2) {
        ids[i / 2] = x[i];
        ids[39 - i / 2] = x[i + 1];
    }
}
