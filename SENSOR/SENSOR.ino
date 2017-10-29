/*
  ardu_328p 8Mhz 3.3V
  dht22  AM2302
  OLED 128*64 i2c
*/
//#include <avr/pgmspace.h>

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <stdint.h>
#include "LowPower.h" //LP
#include <avr/wdt.h>
#include "DHT.h"

#define IM_SENSOR_NUM 1  //1..5

//=========================================== NRF24 =====================================================
#define NRF24_CE_PIN 9
#define NRF24_CSN_PIN 10 //if use SPI, d10=hardware SS SPI only
const uint64_t pipes[6] = {   //'static' - no need
  0xDEADBEEF00LL,  //pipe0 is SYSTEM_pipe, avoid openReadingPipe(0, );
  0xDEADBEEF01LL,
  0xDEADBEEF02LL, 
  0xDEADBEEF03LL,
  0xDEADBEEF04LL,
  0xDEADBEEF05LL
};
RF24 NRF24_radio(NRF24_CE_PIN, NRF24_CSN_PIN);


//============================================ OLED ===========================================
#include <OLED_I2C.h>
OLED  myOLED(SDA, SCL);  //OLED SDA A4, SCL A5
extern uint8_t SmallFont[]; //6*8px
extern uint8_t MediumNumbers[]; //12*16px
extern uint8_t BigNumbers[]; //14*24px

//======================================= SENSOR DATA ==========================================
int16_t humidity = 0;
int16_t temperature = 0;
float batteryVoltage = 0.0;

//====================================== BUTTON ================================================
#define BUTTON_WAKEUP_PIN 2 //only d2 interrupt unSleep

//====================================== SYS =====================================================
#define BATT_CONTROL_PIN_1V1 A0 //hardcoded in PCB voltage divider
#define BATT_max 4.20
#define BATT_min 3.34

//======================================= DHT humidity sensor =====================================
DHT dht;
#define DHT22_DATA_PIN 3

void setup() {
  SYS_init();
  NRF24_init();
  DHT_init();
  OLED_init();  
}

void loop() {
  //wdt_enable (WDTO_8S);
  //try to have time < 8s, else autoreset by watchdog
  NRF24_sendDataToBase();
 // wdt_reset();
  //wdt_disable();

  // Allow wake up pin to trigger interrupt on low.
  attachInterrupt(0, wakeUp, LOW); //D2
  uint8_t countSleep = 0;
  while (countSleep < 20) {
    LowPower.powerDown(SLEEP_1S, ADC_OFF, BOD_OFF);
    countSleep++;
  }
  detachInterrupt(0);

  //reset base after 1 day uptime
  if ((int) millis() > 86400000L) {
    wdt_enable(WDTO_2S);
    delay(2500);
  }

  //LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF); //43mkA

}

void wakeUp() {
  detachInterrupt(0);
  OLED_display();
}


