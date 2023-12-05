// CONFIGURATION
// Edit this section to adjust the config to your project.

#define CYCLE_DURATION 500

// PIN CONFIGURATION
// For each W65C02 PIN (as in the comments), provide Teensy 4.1 pin no.
// Pins 8, 21 and 35 are not data pins, so they can be ignored.
// Pins 4, 6, 36 and 40 can be skipped if connected directly to power.
// Pin 2 can be skipped if connected to power via resistor.

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
  