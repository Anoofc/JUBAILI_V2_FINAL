#define DEBUG 0

#define BTDEVICE_NAME "JBros"
#define BAUDRATE 115200

#define LED_PIN_1 22    // SUN
#define LED_PIN_2 21    // SOLAR
#define LED_PIN_3 23    // INVERTER
#define LED_PIN_4 33    // GENSET
#define LED_PIN_5 14    // ENERGY UP
#define LED_PIN_6 25    // ENERGY DOWN
#define LED_PIN_7 26    // LOAD
#define LED_PIN_8 32    // UTILITY/MAIN 

#define LED_COUNT_1 21    
#define LED_COUNT_2 20
#define LED_COUNT_3 40
#define LED_COUNT_4 34
#define LED_COUNT_5 43
#define LED_COUNT_6 65
#define LED_COUNT_7 56
#define LED_COUNT_8 29

#define SUNBOX      1
#define SOLARBOX    2
#define INVERTERBOX 3
#define GENSETBOX   4
#define TOTALBOX    5
#define ENERGYBOX   6
#define LOADBOX     7
#define MAINBOX     8


#define RED   255,0,0
#define GREEN 0,255,0
#define BLUE  0,0,255
#define WARM  255,70,0

#define BRIGHTNESS 255

#define NUMBER_OF_ON_LED_FOR_STRIP_1 4
#define NUMBER_OF_OFF_LED_FOR_STRIP_1 8
#define TIME_DELAY_FOR_RUNNING_SPEED_FOR_STRIP_1_IN_us 70000

#define NUMBER_OF_ON_LED_FOR_STRIP_2 4
#define NUMBER_OF_OFF_LED_FOR_STRIP_2 8
#define TIME_DELAY_FOR_RUNNING_SPEED_FOR_STRIP_2_IN_us 90000

#define NUMBER_OF_ON_LED_FOR_STRIP_3 4
#define NUMBER_OF_OFF_LED_FOR_STRIP_3 8
#define TIME_DELAY_FOR_RUNNING_SPEED_FOR_STRIP_3_IN_us 70000

#define NUMBER_OF_ON_LED_FOR_STRIP_4 4
#define NUMBER_OF_OFF_LED_FOR_STRIP_4 8
#define TIME_DELAY_FOR_RUNNING_SPEED_FOR_STRIP_4_IN_us 70000

#define NUMBER_OF_ON_LED_FOR_STRIP_5 4
#define NUMBER_OF_OFF_LED_FOR_STRIP_5 8
#define TIME_DELAY_FOR_RUNNING_SPEED_FOR_STRIP_5_IN_us 70000

#define NUMBER_OF_ON_LED_FOR_STRIP_6 4
#define NUMBER_OF_OFF_LED_FOR_STRIP_6 8
#define TIME_DELAY_FOR_RUNNING_SPEED_FOR_STRIP_6_IN_us 70000

#define NUMBER_OF_ON_LED_FOR_STRIP_7 4
#define NUMBER_OF_OFF_LED_FOR_STRIP_7 8
#define TIME_DELAY_FOR_RUNNING_SPEED_FOR_STRIP_7_IN_us 70000

#define NUMBER_OF_ON_LED_FOR_STRIP_8 4
#define NUMBER_OF_OFF_LED_FOR_STRIP_8 8
#define TIME_DELAY_FOR_RUNNING_SPEED_FOR_STRIP_8_IN_us 70000


#include <Arduino.h>

#include <SparkFunDMX.h>
#include <Adafruit_NeoPixel.h>
#include "BluetoothSerial.h"

uint8_t sunflag = 0;
uint8_t utilityflag = 0;
uint8_t gensetflag = 0;
uint8_t defaultflag = 0;

bool genset_status = 0;
bool utility_status = 0;
bool energystorage_status = 0;
bool sun_status = 0;

uint32_t brightness = 255;

uint32_t current_led_number_1 = 0;
uint32_t current_led_number_2 = 0;
uint32_t current_led_number_3 = 0;
uint32_t current_led_number_4 = 0;
uint32_t current_led_number_5 = 0;
uint32_t current_led_number_6 = 0;
uint32_t current_led_number_7 = 0;
uint32_t current_led_number_8 = 0;

uint32_t last_updated_time_for_strip_1 = 0;
uint32_t last_updated_time_for_strip_2 = 0;
uint32_t last_updated_time_for_strip_3 = 0;
uint32_t last_updated_time_for_strip_4 = 0;
uint32_t last_updated_time_for_strip_5 = 0;
uint32_t last_updated_time_for_strip_6 = 0;
uint32_t last_updated_time_for_strip_7 = 0;
uint32_t last_updated_time_for_strip_8 = 0;



BluetoothSerial SerialBT;
SparkFunDMX dmx;

Adafruit_NeoPixel strip_1(LED_COUNT_1, LED_PIN_1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip_2(LED_COUNT_2, LED_PIN_2, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip_3(LED_COUNT_3, LED_PIN_3, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip_4(LED_COUNT_4, LED_PIN_4, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip_5(LED_COUNT_5, LED_PIN_5, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip_6(LED_COUNT_6, LED_PIN_6, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip_7(LED_COUNT_7, LED_PIN_7, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip_8(LED_COUNT_8, LED_PIN_8, NEO_GRB + NEO_KHZ800);

void reset_values_for_strip()
{
  current_led_number_1 = 0;
  current_led_number_2 = 0;
  current_led_number_3 = 0;
  current_led_number_4 = 0;
  current_led_number_5 = 0;
  current_led_number_6 = 0;
  current_led_number_7 = 0;
  current_led_number_8 = 0;
}

void resetFlag() {
  sunflag = 0;
  utilityflag = 0;
  gensetflag = 0;
  defaultflag = 0;
}

// SUN

void sun_run_led(uint8_t r, uint8_t g, uint8_t b)
{
  if (micros() - last_updated_time_for_strip_1 < TIME_DELAY_FOR_RUNNING_SPEED_FOR_STRIP_1_IN_us) {
    return;
  }
  if (current_led_number_1 < (LED_COUNT_1 + NUMBER_OF_ON_LED_FOR_STRIP_1)) {
    current_led_number_1 += 1;
  } else {
    current_led_number_1 = (LED_COUNT_1 + NUMBER_OF_ON_LED_FOR_STRIP_1) - (NUMBER_OF_ON_LED_FOR_STRIP_1 + NUMBER_OF_OFF_LED_FOR_STRIP_1 - 1);
  }
  for (uint16_t k = 1; k <= current_led_number_1; k++) {
    strip_1.setPixelColor(k, strip_1.Color(0, 0, 0));
  }
  for (int32_t j = current_led_number_1; j > 0; j -= (NUMBER_OF_ON_LED_FOR_STRIP_1 + NUMBER_OF_OFF_LED_FOR_STRIP_1))
  {
    for (int32_t i = j; i > (j - NUMBER_OF_ON_LED_FOR_STRIP_1); i--)
    {
      uint16_t m = i; if (m < 1) {
        m = 1;
      }
            uint8_t red = map(m,j,(j- NUMBER_OF_ON_LED_FOR_STRIP_1),r,0);
            uint8_t green = map(m,j,(j- NUMBER_OF_ON_LED_FOR_STRIP_1),g,0);
            uint8_t blue = map(m,j,(j- NUMBER_OF_ON_LED_FOR_STRIP_1),b,0);
      strip_1.setPixelColor(m, strip_1.Color(r, g, b));
    }
  }
  strip_1.show();
  last_updated_time_for_strip_1 = micros();
}

// SOLAR PANEL

void solar_panel(uint8_t r, uint8_t g, uint8_t b)
{
  if (micros() - last_updated_time_for_strip_2 < TIME_DELAY_FOR_RUNNING_SPEED_FOR_STRIP_2_IN_us) {
    return;
  }
  if (current_led_number_2 < (LED_COUNT_2 + NUMBER_OF_ON_LED_FOR_STRIP_2)) {
    current_led_number_2 += 1;
  } else {
    current_led_number_2 = (LED_COUNT_2 + NUMBER_OF_ON_LED_FOR_STRIP_2) - (NUMBER_OF_ON_LED_FOR_STRIP_2 + NUMBER_OF_OFF_LED_FOR_STRIP_2 - 1);
  }
  for (uint16_t k = 1; k <= current_led_number_2; k++) {
    strip_2.setPixelColor(k, strip_2.Color(0, 0, 0));
  }
  for (int32_t j = current_led_number_2; j > 0; j -= (NUMBER_OF_ON_LED_FOR_STRIP_2 + NUMBER_OF_OFF_LED_FOR_STRIP_2))
  {
    for (int32_t i = j; i > (j - NUMBER_OF_ON_LED_FOR_STRIP_2); i--)
    {
      uint16_t m = i; if (m < 1) {
        m = 1;
      }
            uint8_t red = map(m,j,(j- NUMBER_OF_ON_LED_FOR_STRIP_1),r,0);
            uint8_t green = map(m,j,(j- NUMBER_OF_ON_LED_FOR_STRIP_1),g,0);
            uint8_t blue = map(m,j,(j- NUMBER_OF_ON_LED_FOR_STRIP_1),b,0);
      strip_2.setPixelColor(m, strip_2.Color(r, g, b));
    }
  }
  strip_2.show();
  last_updated_time_for_strip_2 = micros();
}

void inverter_led(uint8_t r, uint8_t g, uint8_t b)
{
  if (micros() - last_updated_time_for_strip_3 < TIME_DELAY_FOR_RUNNING_SPEED_FOR_STRIP_3_IN_us) {
    return;
  }
  if (current_led_number_3 < (LED_COUNT_3 + NUMBER_OF_ON_LED_FOR_STRIP_3)) {
    current_led_number_3 += 1;
  } else {
    current_led_number_3 = (LED_COUNT_3 + NUMBER_OF_ON_LED_FOR_STRIP_3) - (NUMBER_OF_ON_LED_FOR_STRIP_3 + NUMBER_OF_OFF_LED_FOR_STRIP_3 - 1);
  }
  for (uint16_t k = 1; k <= current_led_number_3; k++) {
    strip_3.setPixelColor(k, strip_3.Color(0, 0, 0));
  }
  for (int32_t j = current_led_number_3; j > 0; j -= (NUMBER_OF_ON_LED_FOR_STRIP_3 + NUMBER_OF_OFF_LED_FOR_STRIP_3))
  {
    for (int32_t i = j; i > (j - NUMBER_OF_ON_LED_FOR_STRIP_3); i--)
    {
      uint16_t m = i; if (m < 1) {
        m = 1;
            uint8_t red = map(m,j,(j- NUMBER_OF_ON_LED_FOR_STRIP_1),r,0);
            uint8_t green = map(m,j,(j- NUMBER_OF_ON_LED_FOR_STRIP_1),g,0);
            uint8_t blue = map(m,j,(j- NUMBER_OF_ON_LED_FOR_STRIP_1),b,0);
      }
      strip_3.setPixelColor(m, strip_3.Color(r, g, b));
    }
  }
  strip_3.show();
  last_updated_time_for_strip_3 = micros();
}

void gen_set(uint8_t r, uint8_t g, uint8_t b)
{
  if (micros() - last_updated_time_for_strip_4 < TIME_DELAY_FOR_RUNNING_SPEED_FOR_STRIP_4_IN_us) {
    return;
  }
  if (current_led_number_4 < (LED_COUNT_4 + NUMBER_OF_ON_LED_FOR_STRIP_4)) {
    current_led_number_4 += 1;
  } else {
    current_led_number_4 = (LED_COUNT_4 + NUMBER_OF_ON_LED_FOR_STRIP_4) - (NUMBER_OF_ON_LED_FOR_STRIP_4 + NUMBER_OF_OFF_LED_FOR_STRIP_4 - 1);
  }
  for (uint16_t k = 1; k <= current_led_number_4; k++) {
    strip_4.setPixelColor(k, strip_4.Color(0, 0, 0));
  }
  for (int32_t j = current_led_number_4; j > 0; j -= (NUMBER_OF_ON_LED_FOR_STRIP_4 + NUMBER_OF_OFF_LED_FOR_STRIP_4))
  {
    for (int32_t i = j; i > (j - NUMBER_OF_ON_LED_FOR_STRIP_4); i--)
    {
      uint16_t m = i; if (m < 1) {
        m = 1;
      }
      strip_4.setPixelColor(m, strip_4.Color(r, g, b));
    }
  }
  strip_4.show();
  last_updated_time_for_strip_4 = micros();
}

void energyup(uint8_t r, uint8_t g, uint8_t b)
{
  if (micros() - last_updated_time_for_strip_5 < TIME_DELAY_FOR_RUNNING_SPEED_FOR_STRIP_5_IN_us) {
    return;
  }
  if (current_led_number_5 < (LED_COUNT_5 + NUMBER_OF_ON_LED_FOR_STRIP_5)) {
    current_led_number_5 += 1;
  } else {
    current_led_number_5 = (LED_COUNT_5 + NUMBER_OF_ON_LED_FOR_STRIP_5) - (NUMBER_OF_ON_LED_FOR_STRIP_5 + NUMBER_OF_OFF_LED_FOR_STRIP_5 - 1);
  }
  for (uint16_t k = 1; k <= current_led_number_5; k++) {
    strip_5.setPixelColor(k, strip_5.Color(0, 0, 0));
  }
  for (int32_t j = current_led_number_5; j > 0; j -= (NUMBER_OF_ON_LED_FOR_STRIP_5 + NUMBER_OF_OFF_LED_FOR_STRIP_5))
  {
    for (int32_t i = j; i > (j - NUMBER_OF_ON_LED_FOR_STRIP_5); i--)
    {
      uint16_t m = i; if (m < 1) {
        m = 1;
      }
      strip_5.setPixelColor(m, strip_5.Color(r, g, b));
    }
  }
  strip_5.show();
  last_updated_time_for_strip_5 = micros();
}

void energydown(uint8_t r, uint8_t g, uint8_t b)
{
  if (micros() - last_updated_time_for_strip_6 < TIME_DELAY_FOR_RUNNING_SPEED_FOR_STRIP_6_IN_us) {
    return;
  }
  if (current_led_number_6 < (LED_COUNT_6 + NUMBER_OF_ON_LED_FOR_STRIP_6)) {
    current_led_number_6 += 1;
  } else {
    current_led_number_6 = (LED_COUNT_6 + NUMBER_OF_ON_LED_FOR_STRIP_6) - (NUMBER_OF_ON_LED_FOR_STRIP_6 + NUMBER_OF_OFF_LED_FOR_STRIP_6 - 1);
  }
  for (uint16_t k = 1; k <= current_led_number_6; k++) {
    strip_6.setPixelColor(k, strip_6.Color(0, 0, 0));
  }
  for (int32_t j = current_led_number_6; j > 0; j -= (NUMBER_OF_ON_LED_FOR_STRIP_6 + NUMBER_OF_OFF_LED_FOR_STRIP_6))
  {
    for (int32_t i = j; i > (j - NUMBER_OF_ON_LED_FOR_STRIP_6); i--)
    {
      uint16_t m = i; if (m < 1) {
        m = 1;
      }
      strip_6.setPixelColor(m, strip_6.Color(r, g, b));
    }
  }
  strip_6.show();
  last_updated_time_for_strip_6 = micros();
}

void load(uint8_t r, uint8_t g, uint8_t b)
{
  if (micros() - last_updated_time_for_strip_7 < TIME_DELAY_FOR_RUNNING_SPEED_FOR_STRIP_7_IN_us) {
    return;
  }
  if (current_led_number_7 < (LED_COUNT_7 + NUMBER_OF_ON_LED_FOR_STRIP_7)) {
    current_led_number_7 += 1;
  } else {
    current_led_number_7 = (LED_COUNT_7 + NUMBER_OF_ON_LED_FOR_STRIP_7) - (NUMBER_OF_ON_LED_FOR_STRIP_7 + NUMBER_OF_OFF_LED_FOR_STRIP_7 - 1);
  }
  for (uint16_t k = 1; k <= current_led_number_7; k++) {
    strip_7.setPixelColor(k, strip_7.Color(0, 0, 0));
  }
  for (int32_t j = current_led_number_7; j > 0; j -= (NUMBER_OF_ON_LED_FOR_STRIP_7 + NUMBER_OF_OFF_LED_FOR_STRIP_7))
  {
    for (int32_t i = j; i > (j - NUMBER_OF_ON_LED_FOR_STRIP_7); i--)
    {
      uint16_t m = i; if (m < 1) {
        m = 1;
      }
      strip_7.setPixelColor(m, strip_7.Color(r, g, b));
    }
  }
  strip_7.show();
  last_updated_time_for_strip_7 = micros();
}
void mainline(uint8_t r, uint8_t g, uint8_t b)
{
  if (micros() - last_updated_time_for_strip_8 < TIME_DELAY_FOR_RUNNING_SPEED_FOR_STRIP_8_IN_us) {
    return;
  }
  if (current_led_number_8 < (LED_COUNT_8 + NUMBER_OF_ON_LED_FOR_STRIP_8)) {
    current_led_number_8 += 1;
  } else {
    current_led_number_8 = (LED_COUNT_8 + NUMBER_OF_ON_LED_FOR_STRIP_8) - (NUMBER_OF_ON_LED_FOR_STRIP_8 + NUMBER_OF_OFF_LED_FOR_STRIP_8 - 1);
  }
  for (uint16_t k = 1; k <= current_led_number_8; k++) {
    strip_8.setPixelColor(k, strip_8.Color(0, 0, 0));
  }
  for (int32_t j = current_led_number_8; j > 0; j -= (NUMBER_OF_ON_LED_FOR_STRIP_8 + NUMBER_OF_OFF_LED_FOR_STRIP_8))
  {
    for (int32_t i = j; i > (j - NUMBER_OF_ON_LED_FOR_STRIP_8); i--)
    {
      uint16_t m = i; if (m < 1) {
        m = 1;
      }
      strip_8.setPixelColor(m, strip_8.Color(r, g, b));
    }
  }
  strip_8.show();
  last_updated_time_for_strip_8 = micros();
}

uint8_t sflag = 0; 
void sunButton() {
  
  if(sflag==0){dmx.write(SUNBOX, brightness); dmx.update(); sflag = 1;}
  sun_run_led(WARM);
  if (sunflag > 0) {
    solar_panel(BLUE);
  }
//  if (sunflag > 1) {
//    inverter_led(BLUE);
//  }
  if (sflag >2) {
    inverter_led(BLUE);
  }
  if (sflag > 3) {
    energyup(GREEN);
    load(BLUE);
  }
  if (current_led_number_1 == LED_COUNT_1) {
    sunflag = 1;
    if(sflag==1){dmx.write(SOLARBOX, brightness);dmx.update();  sflag =2;}
    
  }
  if (current_led_number_2 == LED_COUNT_2) {
    if(sflag==2){dmx.write(INVERTERBOX, brightness);dmx.update();  sflag =3;}  
    sunflag = 2;
  }
  if (current_led_number_3 == LED_COUNT_3) {
    sflag=4;
//    dmx.write(TOTALBOX, BRIGHTNESS);
    sunflag = 3;
  }
//  if (current_led_number_5 == LED_COUNT_5) {
//    dmx.write(ENERGYBOX, BRIGHTNESS);
//    sunflag = 4;
//  }
//  if (current_led_number_7 == LED_COUNT_7) {
//    dmx.write(LOADBOX, BRIGHTNESS);
//    sunflag = 5;
//  }
  
}

uint8_t uflag = 0; 
void utilityButton() {

  
  if (uflag ==0){ dmx.write(MAINBOX, BRIGHTNESS);dmx.update();  uflag =1;}
  mainline(BLUE);
  if (utilityflag > 0) {
    energyup(GREEN);
    load(BLUE);
    
  }
  if (current_led_number_8 == LED_COUNT_8) {
    
    if (uflag ==1){dmx.write(TOTALBOX, BRIGHTNESS); dmx.update();  utilityflag = 1; uflag =2; }
  }
  if (current_led_number_5 == LED_COUNT_5) {
    
    if (uflag ==2){dmx.write(ENERGYBOX, BRIGHTNESS); dmx.update();  uflag = 3; utilityflag = 2;}
  }
  if (current_led_number_7 == LED_COUNT_7) {
    
    if (uflag ==3){dmx.write(LOADBOX, BRIGHTNESS); dmx.update(); utilityflag = 3; uflag = 4;}
  }
 
}
uint8_t gflag = 0;
void genSetButton() {
  if (gflag ==0){ dmx.write(GENSETBOX, BRIGHTNESS); dmx.update();  gflag = 1; }
  gen_set(BLUE);
  if (gensetflag > 0) {
    energyup(GREEN);
    load(BLUE);
    
    
  }
  if (current_led_number_4 == LED_COUNT_4) {
    
    if (gflag ==1){ dmx.write(TOTALBOX, BRIGHTNESS); dmx.update();  gflag =2;gensetflag = 1;}
  }
  if (current_led_number_5 == LED_COUNT_5) {
    
    if (gflag == 2){ dmx.write(ENERGYBOX, BRIGHTNESS); dmx.update();  gflag =3;gensetflag = 2;}
  }
  if (current_led_number_7 == LED_COUNT_7) {
    
    if (gflag ==3){ dmx.write(LOADBOX, BRIGHTNESS); dmx.update();  gflag = 4; gensetflag = 3;}
  }
}

uint8_t iflag = 0;
void idleState() {
  
  if (iflag == 0){ dmx.write(ENERGYBOX, BRIGHTNESS); dmx.update();  iflag = 1;}
  energydown(RED);
  if (defaultflag > 0) {
    load(BLUE);
  }
  if (current_led_number_6 == LED_COUNT_6) {
    defaultflag = 1;
    if (iflag == 1){ dmx.write(TOTALBOX, BRIGHTNESS); dmx.update();  iflag = 2;}
  }
  if (current_led_number_7 == LED_COUNT_7) {
    defaultflag = 2;
    if (iflag == 2){ dmx.write(LOADBOX, BRIGHTNESS);dmx.update();  iflag =3;}
  }
}


void setmaxbrightness() {
//  brightness = BRIGHTNESS;
  strip_1.setBrightness(BRIGHTNESS);
  strip_2.setBrightness(BRIGHTNESS);
  strip_3.setBrightness(BRIGHTNESS);
  strip_4.setBrightness(BRIGHTNESS);
  strip_5.setBrightness(BRIGHTNESS);
  strip_6.setBrightness(BRIGHTNESS);
  strip_7.setBrightness(BRIGHTNESS);
  strip_8.setBrightness(BRIGHTNESS);
}

void strip_light_setup() {
  strip_1.begin();
  strip_2.begin();
  strip_3.begin();
  strip_4.begin();
  strip_5.begin();
  strip_6.begin();
  strip_7.begin();
  strip_8.begin();

  setmaxbrightness();
}

void all_dmx_off() {
  dmx.write(1, 0);
  dmx.write(2, 0);
  dmx.write(3, 0);
  dmx.write(4, 0);
  dmx.write(5, 0);
  dmx.write(6, 0);
  dmx.write(7, 0);
  dmx.write(8, 0);
  dmx.update();
}


void allstripClear() {
  strip_1.clear();
  strip_2.clear();
  strip_3.clear();
  strip_4.clear();
  strip_5.clear();
  strip_6.clear();
  strip_7.clear();
  strip_8.clear();
  strip_1.show();
  strip_2.show();
  strip_3.show();
  strip_4.show();
  strip_5.show();
  strip_6.show();
  strip_7.show();
  strip_8.show();
}


void process_data(String btdata) {

  if (DEBUG) {Serial.println(btdata);}
  if (btdata == "A1") {
    gensetflag=0;
    genset_status = 1;
  }
  else if (btdata == "B1") {
    utilityflag=0;
    utility_status = 1;
  }
  else if (btdata == "C1") {
    energystorage_status = 1;
  }
  else if (btdata == "D1") {
    if (genset_status == 1 || utility_status == 1 || energystorage_status == 1){ sun_status = 1;}
  }
  else if (btdata == "A2") {
    gensetflag=0;
    gflag = 0;
    genset_status = 0;
    if (utility_status == 1){ current_led_number_4 = 0; strip_4.clear(); strip_4.show(); dmx.write(GENSETBOX, 0); dmx.update(); }
    if (energystorage_status == 1){ current_led_number_4 = 0; strip_4.clear(); strip_4.show(); current_led_number_5 = 0;  strip_5.clear(); strip_5.show();  dmx.write(GENSETBOX, 0); dmx.update(); }
    if (energystorage_status == 0 && utility_status == 0){ sun_status = 0; sflag =0; reset_values_for_strip(); resetFlag(); allstripClear(); all_dmx_off();  }
  }
  else if (btdata == "B2") {
    utilityflag=0;
    uflag = 0;
    utility_status = 0;
    
    if (genset_status == 1){ current_led_number_8 = 0; strip_8.clear(); strip_8.show(); dmx.write(MAINBOX, 0); dmx.update(); }
    if (energystorage_status == 1){ current_led_number_8 = 0; strip_8.clear(); strip_8.show();  dmx.write(MAINBOX, 0); dmx.update(); if (sun_status==0){current_led_number_5 = 0; strip_5.clear(); strip_5.show();  }}
    if (energystorage_status == 0 && genset_status == 0){ sun_status = 0; sflag =0; reset_values_for_strip(); resetFlag(); allstripClear(); all_dmx_off(); }
  }
  else if (btdata == "C2") {
    defaultflag=0;
    iflag = 0;
    energystorage_status = 0;
    if (utility_status == 1){ current_led_number_6 = 0; strip_6.clear(); strip_6.show(); dmx.update(); }
    if (genset_status == 1){ current_led_number_6 = 0; strip_6.clear(); strip_6.show(); dmx.update(); }
    if (genset_status == 0 && utility_status == 0){ sun_status = 0; sflag =0; reset_values_for_strip(); resetFlag(); allstripClear(); all_dmx_off(); }
  }
  else if (btdata == "D2") {
    sflag =0;
    sun_status = 0;
    sunflag=0;
    if (utility_status == 1){ current_led_number_1 = 0; strip_1.clear(); strip_1.show(); dmx.write(SUNBOX, 0); current_led_number_2 = 0; strip_2.clear(); strip_2.show(); dmx.write(SOLARBOX, 0); current_led_number_3 = 0; strip_3.clear(); strip_3.show();dmx.write(INVERTERBOX, 0); dmx.update(); }
    if (genset_status == 1){ current_led_number_1 = 0; strip_1.clear(); strip_1.show(); dmx.write(SUNBOX, 0); current_led_number_2 = 0; strip_2.clear(); strip_2.show(); dmx.write(SOLARBOX, 0); current_led_number_3 = 0; strip_3.clear(); strip_3.show(); dmx.write(INVERTERBOX, 0); dmx.update(); }
    if (energystorage_status == 1 && utility_status == 0 && genset_status == 0){ current_led_number_1 = 0; strip_1.clear(); strip_1.show(); dmx.write(SUNBOX, 0); current_led_number_2 = 0; strip_2.clear(); strip_2.show(); dmx.write(SOLARBOX, 0); current_led_number_3 = 0; strip_3.clear(); strip_3.show(); dmx.write(INVERTERBOX, 0); current_led_number_5 = 0; strip_5.clear(); strip_5.show(); dmx.update(); }
    
  }

}


void updateBrightness(uint16_t brightness) {
  if (sun_status == 1) {
    strip_1.setBrightness(brightness);
//    strip_2.setBrightness(brightness);
//    strip_3.setBrightness(brightness);
//    strip_5.setBrightness(brightness);
//    strip_7.setBrightness(brightness);

    dmx.write(SUNBOX, brightness);
//    if (sunflag > 0) {
//      dmx.write(SOLARBOX, brightness);
//    }
//    if (sunflag > 1) {
//      dmx.write(INVERTERBOX, brightness);
//    }
//    if (sunflag > 2) {
//      dmx.write(TOTALBOX, brightness);
//    }
//    if (sunflag > 3) {
//      dmx.write(ENERGYBOX, brightness);
//    }
//    if (sunflag > 4) {
//      dmx.write(LOADBOX, brightness);
//    }
  }
  dmx.update();
}

void process_bt_data(String recieved_data)
{
  uint8_t seperation_index = recieved_data.lastIndexOf("S");
  //  if (DEBUG){Serial.println("SI: " + String(seperation_index));}
  if (seperation_index == 255) {
    process_data(recieved_data);
  }
  else if (seperation_index == 0) {
    String required_data = recieved_data.substring(seperation_index + 1);
    brightness = required_data.toInt();
    updateBrightness(brightness);
    if (DEBUG) {
      Serial.println("BRIGHTNESS: " + String(brightness));
    }
  }
}

void read_bt_data()
{
  if (SerialBT.available()) {
    String recieved_data = SerialBT.readString();
    recieved_data.trim();
    process_bt_data(recieved_data);
  }
}


void setup() {
  Serial.begin(BAUDRATE);
  Serial.setTimeout(1);
  SerialBT.begin(BTDEVICE_NAME); // Bluetooth device name
  SerialBT.setTimeout(1);
  delay(1000);

  dmx.initWrite(500);

  all_dmx_off();
  strip_light_setup();

}

void loop() {
  
  read_bt_data();
//  Serial.println( "EG: " + String(iflag) + "\t SUN: "+ String(sflag) + "\t GENE: " + String(gflag) + "\t UT: " + String(uflag)  );
  if (sun_status == 1) {
    sunButton();
  }
  if (utility_status == 1) {
    utilityButton();
  }
  if (genset_status == 1) {
    genSetButton();
  }
  if (energystorage_status == 1) {
    idleState();
  }
//  if (energystorage_status == 0 && utility_status == 0 && genset_status == 0){
//    sun_status = 0;
//    resetFlag(); allstripClear(); all_dmx_off();
//    }

}
