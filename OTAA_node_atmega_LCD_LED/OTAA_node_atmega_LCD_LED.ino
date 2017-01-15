/*******************************************************************************
   Copyright (c) 2015 Thomas Telkamp and Matthijs Kooijman

   Permission is hereby granted, free of charge, to anyone
   obtaining a copy of this document and accompanying files,
   to do whatever they want with them without any restriction,
   including, but not limited to, copying, modification and redistribution.
   NO WARRANTY OF ANY KIND IS PROVIDED.

   This example sends a valid LoRaWAN packet with payload "Hello,
   world!", using frequency and encryption settings matching those of
   the The Things Network.

   This uses OTAA (Over-the-air activation), where where a DevEUI and
   application key is configured, which are used in an over-the-air
   activation procedure where a DevAddr and session keys are
   assigned/generated for use with all further communication.

   Note: LoRaWAN per sub-band duty-cycle limitation is enforced (1% in
   g1, 0.1% in g2), but not the TTN fair usage policy (which is probably
   violated by this sketch when left running for longer)!

   To use this sketch, first register your application and device with
   the things network, to set or generate an AppEUI, DevEUI and AppKey.
   Multiple devices can use the same AppEUI, but each device has its own
   DevEUI and AppKey.

   Do not forget to define the radio type correctly in config.h.

 *******************************************************************************/

#include <lmic.h>
#include <hal/hal.h>
#include <SPI.h>
#include <PCD8544.h>
#include "LowPower.h"

#define LED_PIN_GREEN A1
#define LED_PIN_RED A3

// This EUI must be in little-endian format, so least-significant-byte
// first. When copying an EUI from ttnctl output, this means to reverse
// the bytes. For TTN issued EUIs the last bytes should be 0xD5, 0xB3,
// 0x70. (LSB)
static const u1_t PROGMEM APPEUI[8] = { 0x21, 0x1F, 0x00, 0xF0, 0x7E, 0xD5, 0xB3, 0x70 };
void os_getArtEui (u1_t* buf) {
  memcpy_P(buf, APPEUI, 8);
}

// This should also be in little endian format, see above.(LSB)
static const u1_t PROGMEM DEVEUI[8] = { 0x4D, 0x8E, 0x85, 0xE1, 0xB4, 0x37, 0x26, 0x00 };
void os_getDevEui (u1_t* buf) {
  memcpy_P(buf, DEVEUI, 8);
}

// This key should be in big endian format (or, since it is not really a
// number but a block of memory, endianness does not really apply). In
// practice, a key taken from ttnctl can be copied as-is.
// The key shown here is the semtech default key. (MSB)
static const u1_t PROGMEM APPKEY[16] = { 0x8D, 0xF4, 0x75, 0xCA, 0x78, 0x81, 0x82, 0x81, 0x6A, 0x3B, 0x92, 0x85, 0xB4, 0x50, 0x1B, 0xA6 };
void os_getDevKey (u1_t* buf) {
  memcpy_P(buf, APPKEY, 16);
}

static uint8_t mydata[] = "!";
static osjob_t sendjob;

// Schedule TX every this many seconds (might become longer due to duty
// cycle limitations).
const unsigned TX_INTERVAL = 60;

// Pin mapping atmega328p
const lmic_pinmap lmic_pins = {
  .nss = 10,
  .rxtx = LMIC_UNUSED_PIN,
  .rst = 4,
  .dio = {2, 3, LMIC_UNUSED_PIN},
};

static PCD8544 lcd(9, 8, 7, 5, 6); // SCLK, MOSI, DC, RESET, CS
int cursorPos = -1;

int debugParams[4];
bool LED;

void onEvent (ev_t ev) {
  String str_os_getTime = String(os_getTime());
  display(str_os_getTime);
  display("");

  switch (ev) {
    case EV_SCAN_TIMEOUT:
      display("EV_SCAN_TIMEOUT");
      break;
    case EV_BEACON_FOUND: // beacon found by scanning
      display("EV_BEACON_FOUND");
      // LMIC_sendAlive(); // send empty frame up to notify server of ping mode and interval!
      break;
    case EV_BEACON_MISSED:
      display("EV_BEACON_MISSED");
      break;
    case EV_BEACON_TRACKED:
      display("EV_BEACON_TRACKED");
      break;
    case EV_JOINING:
      Serial.println(F("EV_JOINING"));
      display("EV_JOINING");
      break;
    case EV_JOINED:
      display("EV_JOINED");
      // Disable link check validation (automatically enabled
      // during join, but not supported by TTN at this time).
      LMIC_setLinkCheckMode(0);
      // enable pinging mode, start scanning... // (set local ping interval configuration to 2^1 == 2 sec)
      //     LMIC_setPingable(1);

      break;
    case EV_RFU1:
      display("EV_RFU1");
      break;
    case EV_JOIN_FAILED:
      display("EV_JOIN_FAILED");
      break;
    case EV_REJOIN_FAILED:
      display("EV_REJOIN_FAILED");
      break;
      break;
    case EV_TXCOMPLETE:
      display("EV_TXCOMPLETE (includes waiting for RX windows)");
      if (LMIC.txrxFlags & TXRX_ACK)
        Serial.println(F("Received ack"));
      display("Received ack");
      if (LMIC.dataLen) {
        display("Received ");
        String str_LMICdataLen = String(LMIC.dataLen);
        display(str_LMICdataLen);
        display(" bytes of payload");
      }
      // Schedule next transmission
      os_setTimedCallback(&sendjob, os_getTime() + sec2osticks(TX_INTERVAL), do_send);

      break;
    case EV_LOST_TSYNC:
      Serial.println(F("EV_LOST_TSYNC"));
      display("EV_LOST_TSYNC");
      break;
    case EV_RESET:
      display("EV_RESET");
      break;
    case EV_RXCOMPLETE:      // data received in ping slot
      // log frame data
      debugParams[0] = LMIC.frame; // frame
      debugParams[1] = LMIC.dataBeg; // data begin?
      debugParams[2] = LMIC.dataLen; // data length
      debugParams[3] = LMIC.frame + LMIC.dataBeg; // frame  + databegin

      if (LMIC.dataLen == 1) { // set LED state if exactly one byte is received
        display("reveived 1 byte");
      }
      display("EV_RXCOMPLETE");
      Serial.print("frame; ");
      Serial.print(debugParams[0]);
      Serial.print("  data begin: ");
      Serial.print(debugParams[1]);
      Serial.print("  data length: ");
      Serial.println(debugParams[2]);


      // set LED;
      if (debugParams[0] == 1) {
        digitalWrite(LED_PIN_GREEN, 1);
        digitalWrite(LED_PIN_RED, 0);
      } else if (debugParams[0] == 0) {
        digitalWrite(LED_PIN_GREEN, 0);
        digitalWrite(LED_PIN_RED, 1);
      }
      break;

    case EV_LINK_DEAD:
      display("EV_LINK_DEAD");
      break;
    case EV_LINK_ALIVE:
      display("EV_LINK_ALIVE");
      break;
    default:
      display("unknown event");
      break;
  }
}




void do_send(osjob_t* j) {
  // Check if there is not a current TX/RX job running
  if (LMIC.opmode & OP_TXRXPEND) {
    Serial.println(F("OP_TXRXPEND, not sending"));
    display("OP_TXRXPEND, not sending");
  } else {
    // Prepare upstream data transmission at the next possible time.
    LMIC_setTxData2(1, mydata, sizeof(mydata) - 1, 0);
    display("Packet queued");
  }
  // Next TX is scheduled after TX_COMPLETE event.
}


void setup() {
  delay(100);
  Serial.begin(9600);
  Serial.println(F("Starting"));


  pinMode(LED_PIN_GREEN, OUTPUT);
  pinMode(LED_PIN_RED, OUTPUT);

  ledStartup();
  lcd.begin(84, 48);
  display("starting");
#ifdef VCC_ENABLE
  // For Pinoccio Scout boards
  pinMode(VCC_ENABLE, OUTPUT);
  digitalWrite(VCC_ENABLE, HIGH);
  delay(1000);
#endif

  // LMIC init
  os_init();
  // Reset the MAC state. Session and pending data transfers will be discarded.
  LMIC_reset();
  // LMIC_startJoining(); // described in LMIC 1.5 manual
  // Start job (sending automatically starts OTAA too)
  do_send(&sendjob);
}

void loop() {
  os_runloop_once();
  // LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
  // LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
  // LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
}

void display(String text) {
  Serial.println(text);
  if (cursorPos < 4) {
    cursorPos++;
  } else {
    cursorPos = -1;
    lcd.clear();
  }
  lcd.setCursor(0, cursorPos);
  lcd.print(text);
}


void ledStartup() {
  bool m = 0;
  for (int i = 0; i < 6; i++) {
    bool n = m;
    m = !m;
    digitalWrite(LED_PIN_GREEN, m);
    digitalWrite(LED_PIN_RED, n);
    delay(200);
  }
  digitalWrite(LED_PIN_RED, LOW);
}
