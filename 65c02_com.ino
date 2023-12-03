// This script makes Teensy 4.1 a bridge betwen COM port and 65C02 processor.

#include <_Teensy.h>

#define CYCLE_DURATION 500

// PIN CONFIGURATION
// For each W65C02 PIN (as in the comments), provide Teensy 4.1 pin
#define PINS_MAP \
  0,   /*     VP <-- |  1      40 | <-- RES   */     0, \
  0,   /*    RDY <-> |  2      39 | --> PHI2  */     0, \
  0,   /*  PHI1O <-- |  3      38 | <-- SO    */     0, \
  0,   /*    IRQ --> |  4      37 | <-- PHI2  */     23, \
  0,   /*     ML <-- |  5     @36 | <-- BE    */     0, \
  0,   /*    NMI --> |  6      35 | --- NC    */     0, \
  0,   /*   SYNC <-- |  7     *34 | --> RW    */     21, \
  0,   /*    VDD --> |  8     *33 | <-> D0    */     20, \
  10,  /*     A0 <-- |  9*    *32 | <-> D1    */     19, \
  11,  /*     A1 <-- | 10*    *31 | <-> D2    */     18, \
  12,  /*     A2 <-- | 11*    *30 | <-> D3    */     17, \
  24,  /*     A3 <-- | 12*    *29 | <-> D4    */     16, \
  25,  /*     A4 <-- | 13*    *28 | <-> D5    */     15, \
  26,  /*     A5 <-- | 14*    *27 | <-> D6    */     14, \
  27,  /*     A6 <-- | 15*    *26 | <-> D7    */     13, \
  28,  /*     A7 <-- | 16*    *25 | --> A15   */     36, \
  29,  /*     A8 <-- | 17*    *24 | --> A14   */     35, \
  30,  /*     A9 <-- | 18*    *23 | --> A13   */     34, \
  31,  /*    A10 <-- | 19*    *22 | --> A12   */     33, \
  32,  /*    A11 <-- | 20*     21 | --> GND   */     0


typedef struct t_w64c02_pins {
  // cpu control
  int ready;
  int ml; // memory lock
  int be; // bus enable
  int so; // set overflow
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
  int vp; // vector pull
} pins_t;

pins_t setup_pins(void);

auto pins = setup_pins();

void setup() {
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
  for (int i=0; i<16; ++i) {
    pinMode(pins.addr[i], INPUT);
  }
}

void loop() {
  digitalToggle(pins.phi2);
  delay(CYCLE_DURATION);
  handle_cycle(pins);
  print_status(pins);
}

void print_status(pins_t &pins) {
  Serial.print("RW=");
  Serial.print(digitalRead(pins.rw));
  Serial.print(", Addr=");
  Serial.print(get_val_from_pins(pins.addr, 16), BIN);
  Serial.print(", Data=");
  Serial.print(get_val_from_pins(pins.data, 8), BIN);
  Serial.println();
}

void handle_cycle(pins_t &pins) {
  auto rw = digitalRead(pins.rw);
  set_data_pins(pins.data, rw == HIGH ? OUTPUT : INPUT);
}

void set_data_pins(int data[8], int direction) {
  for (int i=0; i<8; ++i) {
    pinMode(data[i], direction);
  }
}

int get_val_from_pins(int addr_pins[], int len) {
  int addr = 0;
  for (int i=0; i<len; ++i) {
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

pins_t setup_pins(void) {
  int x[40] = { PINS_MAP };
  int p[40];
  for (int i=0; i<40; i+=2) {
    p[i/2] = x[i];
    p[39-i/2] = x[i+1];
  }
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