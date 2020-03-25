
/* 
 * Code to receive commands from a custom 3D printed joystick using cheap 433MHz RF Modules, Sep 2019
 * Prof. Thiago Borges de Oliveira
 * 
 */

#include <Arduino.h>

// connect a receiver RF module signal to pin 11
#define RADIO_PIN 11

// radio is at Arduino Pin 11 (PORTB, PB3)
#define RCV_PORT PINB
#define RCV_PIN PB3

struct pressed_button_t {
  union {
    uint16_t all_cmds;
    struct {
      uint16_t UP:1;
      uint16_t DOWN:1;
      uint16_t RIGHT:1;
      uint16_t LEFT:1;
      uint16_t BWD:1;
      uint16_t FWD:1;
      uint16_t TURC:1; // turn clockwise
      uint16_t TURCC:1; // turn counter-clockwise
      uint16_t AUX1:1;
      uint16_t AUX2:1;
      uint16_t ID:6;
    };
  };
} __attribute__ ((packed)) ;

struct pressed_button_t get_cmd();

void setup()
{  
  pinMode(RADIO_PIN, INPUT);

  Serial.begin(57600);
  Serial.println("Starting");  
}

void loop()
{

    // receive the commands pressed
    pressed_button_t cmd = get_cmd();
    if (cmd.all_cmds == 0)
       return; // no button pressed
       
    Serial.println("Command received: ");
    Serial.print("\tDOWN: ");  Serial.println(cmd.DOWN);
    Serial.print("\tUP: ");    Serial.println(cmd.UP);
    Serial.print("\tLEFT: ");  Serial.println(cmd.LEFT);
    Serial.print("\tRIGHT: "); Serial.println(cmd.RIGHT);
    Serial.print("\tFWD: ");   Serial.println(cmd.FWD);
    Serial.print("\tBWD: ");   Serial.println(cmd.BWD);
    Serial.print("\tOPEN: ");  Serial.println(cmd.TURC);
    Serial.print("\tCLOSE: "); Serial.println(cmd.TURCC);
    Serial.print("\tAUX1: ");  Serial.println(cmd.AUX1);
    Serial.print("\tAUX2: ");  Serial.println(cmd.AUX2);
    Serial.println();
}

//#define DEBUG_PROTOCOL
#define DEBUG_PROTOCOL_END

#define set_bit(var, bit) (var |= (_BV(bit)))
#define clr_bit(var, bit) (var &= ~(_BV(bit)))

void receive_rf_packet(uint8_t *rxdat, uint8_t len);

uint8_t crc8(uint8_t *data, uint8_t len)
{
    uint8_t crc = 0xff;
    uint8_t i, j;
    for (i = 0; i < len; i++) {
        crc ^= data[i];
        for (j = 0; j < 8; j++) {
            if ((crc & 0x80) != 0)
                crc = (uint8_t)((crc << 1) ^ 0x31);
            else
                crc <<= 1;
        }
    }
    return crc;
}

struct pressed_button_t get_cmd() {
  uint8_t rxcmd[3] = {0, 0, 0};
  receive_rf_packet(rxcmd, 3);  
  
  uint8_t crc = crc8(rxcmd, 2);
  
  // debug command received
  #ifdef DEBUG_PROTOCOL_END
  uint16_t v = *(uint16_t*)rxcmd;
  if (v > 0) {
    for(int i = 15; i >= 0; i--) {
      if (v & (1U << i))
        Serial.print("1");
      else
        Serial.print("0");
    }
    Serial.print(" crc: ");
    Serial.print(rxcmd[2]);
    Serial.print(":");
    Serial.println(crc);
  }
  #endif

  if (crc != rxcmd[2]) { // crc mismatch
    rxcmd[0] = 0;
    rxcmd[1] = 0;
  }

  pressed_button_t rxcmd2 = *(pressed_button_t*)rxcmd;
  return rxcmd2;
}

#ifdef DEBUG_PROTOCOL
  struct data_t {
    uint16_t high;
    uint16_t low;
  };

  struct data_t data[27];
  int data_idx = 0;
#endif

void print_data_received(uint8_t c, uint8_t di) {
  #ifdef DEBUG_PROTOCOL
  Serial.print("\nDecode error ");
  Serial.print((char)c);
  Serial.println(":");
  for(int i = 0; i < di; i++) {
     Serial.print(data[i].low);
     Serial.print('\t');
     Serial.println(data[i].high);
  }
  #endif
}
//=============================================================================
//   RECEIVE_RF_PACKET
//   Based in the code from https://www.romanblack.com/RF/cheapRFmodules.htm
//=============================================================================
void receive_rf_packet(uint8_t *rxdat, uint8_t len)
{
  unsigned char rrp_data;
  uint16_t rrp_period;
  unsigned char rrp_bits;
  unsigned char rrp_bytes;

  rrp_bytes = 0;
  while(rrp_bytes < len)   // loop until it has received (len) contiguous RF bytes
  {
    // wait for a start pulse >200uS
    unsigned long start;
    unsigned long middle;
    unsigned long end;
    while(1)
    {
      start = micros();
      while (!bit_is_set(RCV_PORT, RCV_PIN)) { // wait for input / edge
        __asm__ __volatile__ ("nop\n\t");
      }
      middle = micros();
      while (bit_is_set(RCV_PORT, RCV_PIN)) { // wait for input \ edge
        __asm__ __volatile__ ("nop\n\t");
      }
      end = micros();
      rrp_period = end - start;                      // grab the pulse period!

      uint8_t high = end - middle;
      
      #ifdef DEBUG_PROTOCOL
        uint8_t di = rrp_bytes * 9;
        data[di].high = high;
        data[di].low = middle - start;
      #endif

      start = end;
      
      if (rrp_period < 170 || high < 60) {
        rrp_bytes = 0;           // clear bytecount if still receiving noise
        continue;
      }
      else {  // valid pulse. stop and read bytes.
        break;
      }
    }

    // now we had a start pulse, record 8 bits
    #ifdef DEBUG_PROTOCOL
    uint8_t di;
    #endif
    rrp_bits = 8;
    while (rrp_bits) {
      while (!bit_is_set(RCV_PORT, RCV_PIN)) { // wait for input / edge
        __asm__ __volatile__ ("nop\n\t");
      }
      middle = micros();
      while (bit_is_set(RCV_PORT, RCV_PIN)) { // wait for input \ edge
        __asm__ __volatile__ ("nop\n\t");
      }
      end = micros();
      rrp_period = end - start;                      // grab the pulse period!

      uint8_t high = end - middle;
      
      #ifdef DEBUG_PROTOCOL
      di = rrp_bytes * 9 + 1 + (8-rrp_bits);
      data[di].high = high;
      data[di].low = middle - start;
      #endif

      start = end;

      if (rrp_period >= 180 || high < 60) {
        rxdat[0] = 0;
        rxdat[1] = 0;
        break;
      }

      rrp_data <<= 1;
      if (rrp_period < 140)
        clr_bit(rrp_data, 0);
      else
        set_bit(rrp_data, 0);
      rrp_bits--;                   // record 1 more good bit done
    }

    // gets to here after 8 good bits OR after an error (unexpected start pulse)
    if (rrp_bits) {    // if error
      rrp_bytes = 0;  // reset bytes, must run from start of a new packet again!
    } else {          // else 8 good bits were received
      rxdat[rrp_bytes] = rrp_data;  // so save the received byte into array
      rrp_bytes++;                  // record another good byte was saved
    }
  }
}

