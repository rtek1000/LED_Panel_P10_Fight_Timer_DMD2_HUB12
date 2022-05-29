// https://github.com/rtek1000/Fight_Timer_DMD_HUB12
// - Training (fight) time count and interval count between trainings.
// Note: There is a bug that makes the screen blank related to the input_ctrl_left command (serial command: 2)

#include <avr/wdt.h>

#include <SPI.h>

#include "DMD2.h"

#include "fonts/SystemFont5x7.h"

#include "fonts/Arial14.h"

#include <EEPROM.h>

#include <Wire.h>

#include "RTClib.h"

#include <SoftwareSerial.h>

//#define language_EN 1
//#define language_PT_BR 1
#define skip_init_msg 1

SoftwareSerial softSerial(5, 4);// RX, TX

//------ Navigations & Menu - start

#define func_normal 0
#define func_interval 1
#define func_clock 2
#define func_date 3
#define func_menu_set_round_recall_normal_min10 4
#define func_menu_set_round_recall_normal_min1 5
#define func_menu_set_round_recall_normal_sec10 6
#define func_menu_set_round_recall_normal_sec1 7
#define func_menu_set_round_recall_interval_min10 8
#define func_menu_set_round_recall_interval_min1 9
#define func_menu_set_round_recall_interval_sec10 10
#define func_menu_set_round_recall_interval_sec1 11
#define func_menu_set_round_time_mode 12
#define func_menu_set_round10 13
#define func_menu_set_round1 14
#define func_menu_set_round_time_min10 15
#define func_menu_set_round_time_min1 16
#define func_menu_set_round_time_sec10 17
#define func_menu_set_round_time_sec1 18
#define func_menu_set_hour1 19
#define func_menu_set_hour10 20
#define func_menu_set_min1 21
#define func_menu_set_min10 22
#define func_menu_set_time_date_bright 23
#define func_menu_set_day1 24
#define func_menu_set_day10 25
#define func_menu_set_month1 26
#define func_menu_set_month10 27
#define func_menu_set_year1 28
#define func_menu_set_year10 29
#define func_menu_reset_round 30
#define func_menu_set_round 31
#define func_menu_set_round_recall_normal 32
#define func_menu_set_round_recall_interval 33
#define func_menu_bell_ding_mode 34
#define func_menu_set_volume_beep 35
#define func_menu_set_volume_music 36
#define func_menu_set_volume_voice 37

//------ Navigations & Menu - end

//------ I/O - start
// RF remote control 433MHz
#define input_ctrl_left_pin A3 // btn 1 - A3 (Left)
#define input_ctrl_cent_pin 2 // btn 2 - 2  (Center)
#define input_ctrl_right_pin 3 // btn 3 - 3  (Right)

// Temperature sensor
#define LM35 A0 // To LM35

// Bell for warnings
#define Bell A1 // output

// Unused
//#define  A2
//#define  0
//#define  1
//#define  4
//#define  5
//#define  10
//#define  12

// For P10 panel (These pins are mapped in the library DMD)
// See DMD2.cpp file
/*
  SPIDMD::SPIDMD(byte panelsWide, byte panelsHigh)
  #ifdef ESP8266
  : BaseDMD(panelsWide, panelsHigh, 15, 16, 12, 0)
  #else
  : BaseDMD(panelsWide, panelsHigh, 9, 6, 7, 8)
  #endif
  {
  }

  // Create a DMD display using a custom pinout for all the non-SPI pins (SPI pins set by hardware)
  SPIDMD::SPIDMD(byte panelsWide, byte panelsHigh, byte pin_noe, byte pin_a, byte pin_b, byte pin_sck)
  : BaseDMD(panelsWide, panelsHigh, pin_noe, pin_a, pin_b, pin_sck)
  {
  }
*/

//#define  P10-A      6
//#define  P10-B      7
//#define  P10-SCK    8
//#define  P10-OE     9
//#define  P10-DATA  11
//#define  P10-CLK   13
//------ I/O - end
//------ EEPROM address - start
#define normal_recall_min10_addr 0
#define normal_recall_min1_addr 1
#define normal_recall_sec10_addr 2
#define normal_recall_sec1_addr 3

#define interval_recall_min10_addr 4
#define interval_recall_min1_addr 5
#define interval_recall_sec10_addr 6
#define interval_recall_sec1_addr 7

#define round_display_bright10_addr 8
#define round_display_bright1_addr 9

#define clock_display_bright10_addr 10
#define clock_display_bright1_addr 11

#define bell_start_num_addr 12
#define bell_stop_num_addr 13

#define show_sel_time_date_addr 14
#define show_sel_time_date_bright_addr 15

#define bell_ding_enabled_addr 16

#define beep_volume_addr 17
#define music_volume_addr 18
#define voice_volume_addr 19
//------ EEPROM address - end

//------ Display P10 line position - start
#define line1 0
#define line2 9
//------ Display P10 line position - end

#define set_menu_timeout 60 // tempo de timeout para abandonar menus, decremento em 500ms

//#ifdef language_PT_BR
//char daysOfTheWeek[7][6] = {"Domin", "Segun", "Terca", "Quart", "Quint", "Sexta", "Sabad"};
//#else
char daysOfTheWeek[7][6] = { "Sunda", "Monda", "Tuesd", "Wedne", "Thurs",
                             "Frida", "Satur"
                           };
//#endif

boolean input_ctrl_left = true;
boolean input_ctrl_cent = true;
boolean input_ctrl_right = true;

boolean input_ctrl_left_serial = true;
boolean input_ctrl_cent_serial = true;
boolean input_ctrl_right_serial = true;

boolean input_ctrl_left_old = false;
boolean input_ctrl_cent_old = false;
boolean input_ctrl_right_old = false;

int bell_ding_mode_val = 0;

byte show_menu_timeout = 0;

//unsigned int show_round_stop_timeout = 0;

byte show_sel_time_date; // value from eeprom

byte show_sel_time_date_bright; // value from eeprom

float temperature;
float temperature_array[50];
byte temperature_index = 0;

byte state = 0; // 0 = func_normal; //clock
//byte state_menu_timeout = 0;

byte time_sel = func_normal;

byte round_recall_min10; // round
byte round_recall_min1; // round
byte round_recall_sec10; // round
byte round_recall_sec1; // round

byte interval_recall_min10; // interval round
byte interval_recall_min1; // interval round
byte interval_recall_sec10; // interval round
byte interval_recall_sec1; // interval round

byte L10;
byte L1;
byte R10;
byte R1;

boolean second_old = false;

byte clock_show_cnt = 0;
byte date_show_cnt = 0;

boolean pause_state = false; //true
boolean pause_state_previous = false; //true

boolean blink_state = false;
boolean blink_alternate = false;
byte blink_position_X = false;
byte blink_position_Y = false;
String blink_string1;

byte round_cnt10 = 0;
byte round_cnt1 = 1;

unsigned long previousMillis = 0; // will store last time LED was updated
unsigned long currentMillis;
unsigned int intervalMillis = 500; // interval at which to blink (milliseconds)
unsigned long previousMillis_meter = 0; // will store last time LED was updated

unsigned int wr_rtc_yy;
byte wr_rtc_mm;
byte wr_rtc_dd;
byte wr_rtc_HH;
byte wr_rtc_MM;

byte HH_1_temp;

byte dd_1_temp;
byte mm_1_temp;

RTC_DS3231 rtc; //RTC_DS3231

SoftDMD dmd(1, 1); // DMD controls the entire display

DMD_TextBox box( dmd); // "box" provides a text box to automatically write to/scroll the display

String inputString = "";

//int timeout1 = 0;

int beep_volume = 20;
int music_volume = 20;
int voice_volume = 30;

void setup(void);
void show(void);
void loop(void);
void delayWdt(unsigned long dly);
void text_scroll(String MESSAGE);
void controller(void);
void show_round_and_time(void);
void show_round(void);
void show_time_round(void);
void show_recal_round_normal(void);
void show_recal_round_interval(void);
void decrement_time(void);
void update_recall(void);
void ctrl_left_cmd(void);
void ctrl_cent_cmd(void);
void ctrl_right_cmd(void);
void blink_preset(void);
void blink_show(void);
void show_bell_state(void);
void show(void);
void show_clock_and_temperature(void);
void update_rtc(void);
void show_date(void);
void show_date2(void);
void show_clock(void);
void show_clock2(void);
void show_temperature(void);
void temperature_init(void);
void temperature_read(void);
void bell_stop(void);
void bell_start(void);
void musicRandStart(void);
void Bell_ding(void);
void serialEvent(void);
void dfPlayMP3(uint16_t music);
void dfPlayFolder(uint8_t folder, uint8_t music);
void dfVolume(uint8_t volume);
void dfStop1(void);
void dfReset(void);
void waitDfPlayer(void);


void setup(void) {
  delayWdt(300);

  inputString.reserve(20);

  // initialize serial:
  Serial.begin(9600);

  softSerial.begin(9600);

  Serial.println("Panel start");

  randomSeed((analogRead(6) + analogRead(7)) / 2); //

  delayWdt(300);

  byte loop_setup;

  digitalWrite(input_ctrl_left_pin, HIGH);
  digitalWrite(input_ctrl_cent_pin, HIGH);
  digitalWrite(input_ctrl_right_pin, HIGH);

  pinMode(Bell, INPUT_PULLUP); //OUTPUT

  pinMode(input_ctrl_left_pin, INPUT_PULLUP);
  pinMode(input_ctrl_cent_pin, INPUT_PULLUP);
  pinMode(input_ctrl_right_pin, INPUT_PULLUP);

  digitalWrite(Bell, LOW);

  delayWdt(100);

  dfReset();

  //dfPlayMP3(1); // mp3/file

  delayWdt(100);

  //  dfPlayFolder(2, random(1, 100)); // musica aleatória

  int timeout = 0;

  while (!digitalRead(12)) {
    dfStop1();

    delayWdt(100);

    timeout++;

    if (timeout > 100)
      break;
  }

  musicRandStart();
  //
  //  dfPlayFolder(2, 4); // folder, file

  delayWdt(100);

  dfVolume(30);

  dmd.begin();

  dmd.clearScreen();

  dmd.selectFont(SystemFont5x7);

  currentMillis = millis();

  interval_recall_min10 = EEPROM.read(interval_recall_min10_addr); // interval round
  interval_recall_min1 = EEPROM.read(interval_recall_min1_addr); // interval round
  interval_recall_sec10 = EEPROM.read(interval_recall_sec10_addr); // interval round
  interval_recall_sec1 = EEPROM.read(interval_recall_sec1_addr); // interval round

  round_recall_min10 = EEPROM.read(normal_recall_min10_addr); // round
  round_recall_min1 = EEPROM.read(normal_recall_min1_addr); // round
  round_recall_sec10 = EEPROM.read(normal_recall_sec10_addr); // round
  round_recall_sec1 = EEPROM.read(normal_recall_sec1_addr); // round

  //show_sel_time_date = EEPROM.read(show_sel_time_date_addr) & 3; // 00000011

  show_sel_time_date_bright = EEPROM.read(show_sel_time_date_bright_addr);

  bell_ding_mode_val = EEPROM.read(bell_ding_enabled_addr) & 1;

  //  if (show_sel_time_date_bright > 50)
  //  {
  //    show_sel_time_date_bright = 50;
  //  }
  if (round_recall_min10 > 9) {
    round_recall_min10 = 0;
  }
  if (round_recall_min1 > 9) {
    round_recall_min1 = 0;
  }
  if (round_recall_sec10 > 5) {
    round_recall_sec10 = 0;
  }
  if (round_recall_sec1 > 9) {
    round_recall_sec1 = 0;
  }

  if (interval_recall_min10 > 9) {
    interval_recall_min10 = 0;
  }
  if (interval_recall_min1 > 9) {
    interval_recall_min1 = 0;
  }
  if (interval_recall_sec10 > 5) {
    interval_recall_sec10 = 0;
  }
  if (interval_recall_sec1 > 9) {
    interval_recall_sec1 = 0;
  }

  L10 = round_recall_min10;
  L1 = round_recall_min1;
  R10 = round_recall_sec10;
  R1 = round_recall_sec1;

  dmd.setBrightness(5);

  dmd.drawFilledBox(0, 0, 31, 15);

  delayWdt(2000);

  dmd.clearScreen();

  dmd.clearScreen();

  loop_setup = 1;

  while (loop_setup < (show_sel_time_date_bright)) {
    dmd.setBrightness(loop_setup * 5);
    loop_setup++;
    dmd.drawString(2, 4, String(loop_setup * 5));
    delayWdt(200);
  }

  dmd.setBrightness(20);

  Serial.println(F("Fight_Timer_DMD_HUB12"));
  Serial.println(F("by Rtek1000 (2018-2022)"));

  //#ifdef language_PT_BR
  //  text_scroll(F("PAINEL LED P10-32x16 PROGRAMAVEL"));
  //  text_scroll(F("PAINEL LED P10 - HUB12"));
  //
  //  text_scroll(F("For Brother"));
  //
  //  text_scroll(F("BY Rtek1000 - 2018"));
  //#else
  //  text_scroll(F("PROGRAMMABLE PANEL LED P10-32x16"));
  // text_scroll(F("P10 PANEL LED - HUB12"));
  //
  //  text_scroll(F("For Brother"));
  //
  //  text_scroll(F("by Rtek1000 - 2018-2022"));/
  //#endif

  dmd.setBrightness(show_sel_time_date_bright * 5);

#ifdef skip_init_msg
  dmd.drawString(2, line1, "Fight");
  dmd.drawString(2, line2, "Timer");

  delayWdt(3000);

  dmd.clearScreen();

  dmd.drawString(2, line1, "DMD2");
  dmd.drawString(2, line2, "HUB12");

  delayWdt(3000);

  // text_scroll(F("Fight_Timer_DMD_HUB12"));

  dmd.clearScreen();

  String text1 = String(show_sel_time_date_bright * 5);

  //text_scroll(text1);

  dmd.drawString(2, line1, "BRT:");
  dmd.drawString(2, line2, text1);

  delayWdt(2000);

  // text_scroll(F("Fight_Timer_DMD_HUB12"));

  dmd.clearScreen();

  dmd.clearScreen();

  text1 = "Let's go champions!";

  text_scroll(text1);

  dmd.clearScreen();
#endif

  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");

    //#ifdef language_PT_BR
    //    text_scroll(F("FALHA1: RTC DS3231 NAO ENCONTRADO"));
    //#else
    text_scroll(F("FAULT 1: RTC DS3231 NOT FOUND"));
    //#endif

  }

  //  if (! rtc.isrunning())
  //  {
  //    Serial.println("RTC is NOT running!");
  // #ifdef language_PT_BR
  //    text_scroll(F("FALHA2: RTC DS3231 PARADO"));
  //    text_scroll(F("POR FAVOR: VERIFIQUE A BATERIA CR2032 3V"));
  // #else
  //    text_scroll(F("FAULT 2: RTC DS3231 STOPPED"));
  //    text_scroll(F("PLEASE: CHECK THE CR2032 3V BATTERY"));
  // #endif

  //    // following line sets the RTC to the date & time this sketch was compiled
  //    //rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); // __TIME__ 15:12:18
  //    rtc.adjust(DateTime(2016, 05, 27, 12, 00, 00)); // yyyy,mm,dd,HH,MM,SS
  //    // This line sets the RTC with an explicit date & time, for example to set
  //    // January 21, 2014 at 3am you would call:
  //    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  //  }

  //rtc.adjust(DateTime(2016,05,16,03,01,56)); // yyyy,mm,dd,HH,MM,SS
  analogReference(INTERNAL); // arduino uno/mini/nano

  temperature_init();

  dmd.clearScreen();

  show_clock_and_temperature();

  delayWdt(2000);
  dmd.clearScreen();

  show_date();

  delayWdt(2000);

  dmd.clearScreen();

  //  loop_setup = 3;
  //
  for (int i = 0; i < 5; i++) {
    dmd.drawString(2, 4, "FIGHT");
    delayWdt(150);

    dmd.clearScreen();
    delayWdt(150);
  }

  show_round_and_time();

  dfStop1();

  delayWdt(1000);

  //dmd.setBrightness(show_sel_time_date_bright * 5);

  //wdt_reset();  //  reset watchdog
  //wdt_enable(WDTO_8S);  // watchdog

  bell_start();

  //  dfPlayFolder(1, 251); // rodada
  //
  //  waitDfPlayer();
  //
  //  dfPlayFolder(1, 1); // 1
  //
  //  delayWdt(100);

  //  waitDfPlayer();
  //
  //  dfPlayFolder(2, random(1, 100)); // musica aleatória
  //
  //  delayWdt(100);
}

void loop(void) {
  wdt_reset(); //  reset watchdog

  controller();

  currentMillis = millis();

  if ((currentMillis - previousMillis) >= intervalMillis) {
    previousMillis = currentMillis;

    show();

    if (blink_state == true) {
      blink_show();
    }

    temperature_read();

    if (second_old == false) {
      second_old = true;

      decrement_time();

      if ((state != func_normal) & (state != func_interval)
          & (state != func_clock) & (state != func_date)) {
        if (show_menu_timeout > 0) {
          show_menu_timeout--;

          if (show_menu_timeout == 0) {
            ctrl_left_cmd();
            show_menu_timeout = 0;
          }
        }

        //        if ((state == func_normal) | (state == func_interval))
        //        {
        //          if (show_round_stop_timeout < 10)
        //          {
        //            show_round_stop_timeout++;
        //          }
        //          else
        //          {
        //            show_round_stop_timeout = 0;
        //            ctrl_left_cmd();
        //          }
        //        }
      }
    } else {
      second_old = false;
    }
  }

  //  DateTime now = rtc.now();
  //
  //  if (second_old != now.second())
  //  {
  //    second_old = now.second();
  //
  //    decrement_time();
  //  }

  //delayWdt(50);
}

void delayWdt(unsigned long dly) {
  while (dly--) {
    wdt_reset(); //  reset watchdog
    delay(1);
  }
}

void text_scroll(String MESSAGE) {
  //String MESSAGE = "Menu de ajustes";
  //String MESSAGE = "Settings menu";
  MESSAGE = "     " + MESSAGE;
  MESSAGE = MESSAGE + "     ";
  byte next = MESSAGE.length() + 1;

  dmd.clearScreen();

  dmd.selectFont(SystemFont5x7);

  while (next--) {
    //Serial.print(MESSAGE[MESSAGE.length() - next]);
    //box.print(MESSAGE[MESSAGE.length() - next]);
    dmd.drawString(2, 4, MESSAGE.substring(MESSAGE.length() - next));
    delayWdt(200);
    //next++;
  }

  delayWdt(500);
  dmd.clearScreen();
  dmd.selectFont(SystemFont5x7);
}
void controller(void) {
  input_ctrl_left = false;
  input_ctrl_cent = false;
  input_ctrl_right = false;

  if ((!digitalRead(input_ctrl_left_pin)) | (!input_ctrl_left_serial)) // HIGH
  {
    input_ctrl_left = true;
  } else if ((!digitalRead(input_ctrl_cent_pin)) | (!input_ctrl_cent_serial)) // HIGH
  {
    input_ctrl_cent = true;
  } else if ((!digitalRead(input_ctrl_right_pin))
             | (!input_ctrl_right_serial)) // HIGH
  {
    input_ctrl_right = true;
  }

  if ((input_ctrl_left == true) & (input_ctrl_left_old == false)) {
    while ((digitalRead(input_ctrl_left_pin) == HIGH)
           & (input_ctrl_left_serial))
      ;
    input_ctrl_left_old = true;
    ctrl_left_cmd();
    delayWdt(300);
  } else if ((input_ctrl_left == false) & (input_ctrl_left_old == true)) {
    input_ctrl_left_old = false;
  }

  if ((input_ctrl_cent == true) & (input_ctrl_cent_old == false)) {
    while ((digitalRead(input_ctrl_cent_pin) == HIGH)
           & (input_ctrl_cent_serial))
      ;
    input_ctrl_cent_old = true;
    ctrl_cent_cmd();
    delayWdt(300);
  } else if ((input_ctrl_cent == false) & (input_ctrl_cent_old == true)) {
    input_ctrl_cent_old = false;
  }

  if ((input_ctrl_right == true) & (input_ctrl_right_old == false)) {
    while ((digitalRead(input_ctrl_right_pin) == HIGH)
           & (input_ctrl_right_serial))
      ;
    input_ctrl_right_old = true;
    ctrl_right_cmd();
    delayWdt(300);
  } else if ((input_ctrl_right == true) & (input_ctrl_right_old == true)) {
    input_ctrl_right_old = false;
  }

  if (!input_ctrl_left_serial)
    input_ctrl_left_old = false;
  if (!input_ctrl_cent_serial)
    input_ctrl_cent_old = false;
  if (!input_ctrl_right_serial)
    input_ctrl_right_old = false;

  input_ctrl_left_serial = true;
  input_ctrl_cent_serial = true;
  input_ctrl_right_serial = true;
}
void show_round_and_time(void) {
  show_round();
  show_time_round();
}
void show_round(void) {
  //dmd.drawString(2, 0, " :" + String(round_cnt10) + String(round_cnt1));
  dmd.drawString(14, 0, ":" + String(round_cnt10) + String(round_cnt1));
}
void show_time_round(void) {
  //  if ((state == func_normal) | (time_sel == func_interval))
  //  {
  if (time_sel == func_normal) {
    //#ifdef language_PT_BR
    //    dmd.drawString(2, 0, "Lt"); // "N"
    //#else
    dmd.drawString(2, 0, "Ft"); // "N"
    //#endif
    //  dmd.drawString(26, 0, " "); // "N"
  } else if (time_sel == func_interval) {
    dmd.drawString(2, 0, "In");
    //    dmd.drawString(26, 0, "I");
  }
  //
  //    str1 = String(L10)+String(L1);
  //    str2 = ":"+String(R10)+String(R1);

  dmd.drawString(2, line2,
                 String(L10) + String(L1) + ":" + String(R10) + String(R1));
  //dmd.drawString(15, line2, ":" + String(R10) + String(R1));
  //  }
}
void show_recal_round_normal(void) {
  dmd.drawString(2, line2,
                 String(round_recall_min10) + String(round_recall_min1) + ":"
                 + String(round_recall_sec10) + String(round_recall_sec1));
}
void show_recal_round_interval(void) {
  dmd.drawString(2, line2,
                 String(interval_recall_min10) + String(interval_recall_min1) + ":"
                 + String(interval_recall_sec10)
                 + String(interval_recall_sec1));
}
void decrement_time(void) {
  //  if ((state == func_normal) | (state == func_interval))
  //  {
  if (((L10 != 0) | (L1 != 0) | (R10 != 0) | (R1 != 0))
      & (pause_state == false)) {
    if ((bell_ding_mode_val == true) & (state == func_interval)) {
      if (digitalRead(12)) { // dfPlayer off?
        musicRandStart();
      }
    }

    if (R1 > 0) {
      R1--;
    } else {
      R1 = 9;

      if (R10 > 0) {
        R10--;
      } else {
        R10 = 5;

        if (L1 > 0) {
          L1--;
        } else {
          L1 = 9;

          if (L10 > 0) {
            L10--;
          }
        }
      }
    }
  }

  if ((state == func_normal) | (state == func_interval)) {
    show_round_and_time();
  }
  if ((L10 == 0) & (L1 == 0) & (R10 == 0) & (R1 == 0)
      & (state == func_normal)) {
    //    if ((state == func_normal) | (state == func_interval))
    //    {
    show_round_and_time();
    delayWdt(1000);
    state = func_interval;
    //    }
    time_sel = state;
    L10 = interval_recall_min10;
    L1 = interval_recall_min1;
    R10 = interval_recall_sec10;
    R1 = interval_recall_sec1;

    //    if ((state == func_normal) | (state == func_interval))
    //    {
    blink_state = true;
    blink_alternate = false;

    dmd.clearScreen();
    delayWdt(50);
    show_round_and_time();
    delayWdt(500);
    //    }

    bell_stop();

  } else if ((L10 == 0) & (L1 == 0) & (R10 == 0) & (R1 == 0)
             & (state == func_interval)) {
    //    if ((state == func_normal) | (state == func_interval))
    //    {
    show_round_and_time();
    delayWdt(1000);
    state = func_normal;
    //    }
    time_sel = state;
    L10 = round_recall_min10;
    L1 = round_recall_min1;
    R10 = round_recall_sec10;
    R1 = round_recall_sec1;

    //    if ((state == func_normal) | (state == func_interval))
    //    {
    blink_state = false;
    dmd.clearScreen();
    delayWdt(50);
    //    }
    if (round_cnt1 < 9) {
      round_cnt1++;
    } else {
      if (round_cnt10 < 9) {
        round_cnt1 = 0;
        round_cnt10++;
      } else {
        round_cnt1 = 1;
        round_cnt10 = 0;
      }
    }
    //    if ((state == func_normal) | (state == func_interval))
    //    {
    show_round_and_time();
    delayWdt(500);
    //    }

    bell_start();
  }
  //  }
}
void update_recall(void) {
  EEPROM.update(interval_recall_min10_addr, interval_recall_min10); // interval round
  EEPROM.update(interval_recall_min1_addr, interval_recall_min1); // interval round
  EEPROM.update(interval_recall_sec10_addr, interval_recall_sec10); // interval round
  EEPROM.update(interval_recall_sec1_addr, interval_recall_sec1); // interval round

  EEPROM.update(normal_recall_min10_addr, round_recall_min10); // round
  EEPROM.update(normal_recall_min1_addr, round_recall_min1); // round
  EEPROM.update(normal_recall_sec10_addr, round_recall_sec10); // round
  EEPROM.update(normal_recall_sec1_addr, round_recall_sec1); // round
}
void ctrl_left_cmd(void) {
  dmd.clearScreen();
  delayWdt(50);

  if ((state == func_menu_reset_round) | (state == func_menu_set_round)
      | (state == func_menu_set_round_recall_normal)
      | (state == func_menu_set_round_recall_interval)
      | (state == func_menu_set_round_recall_normal_min10)
      | (state == func_menu_set_round_recall_normal_min1)
      | (state == func_menu_set_round_recall_normal_sec10)
      | (state == func_menu_set_round_recall_normal_sec1)
      | (state == func_menu_set_round_recall_interval_min10)
      | (state == func_menu_set_round_recall_interval_min1)
      | (state == func_menu_set_round_recall_interval_sec10)
      | (state == func_menu_set_round_recall_interval_sec1)
      | (state == func_menu_set_round10) | (state == func_menu_set_round1)
      | (state == func_menu_set_round_time_mode)
      | (state == func_menu_set_round_time_min10)
      | (state == func_menu_set_round_time_min1)
      | (state == func_menu_set_round_time_sec10)
      | (state == func_menu_set_round_time_sec1)
      | (state == func_menu_bell_ding_mode)
      | (state == func_menu_set_volume_beep)
      | (state == func_menu_set_volume_music)
      | (state == func_menu_set_volume_voice)) {
    show_menu_timeout = 0;
    blink_state = false;
    pause_state = pause_state_previous;
    state = time_sel;
    delayWdt(100);
    show_round_and_time();
    update_recall();
  } else if ((state == func_clock) | (state == func_date)) {
    state = time_sel;
    if (time_sel == func_interval) {
      blink_state = true;
    } else {
      blink_state = false;
    }
    show_round_and_time();
  } else if ((state == func_normal) | (state == func_interval)) {
    time_sel = state;
    blink_state = false;
    state = func_clock;
    show_clock_and_temperature();
  } else if ((state == func_menu_set_hour1) | (state == func_menu_set_hour10)
             | (state == func_menu_set_min1) | (state == func_menu_set_min10)
             | (state == func_menu_set_time_date_bright)) //(state == "menu_set_time_date")
  {
    show_menu_timeout = 0;
    clock_show_cnt = 0;
    state = func_clock;

    blink_state = false;
    show_clock_and_temperature();
  } else if ((state == func_menu_set_day1) | (state == func_menu_set_day10)
             | (state == func_menu_set_month1) | (state == func_menu_set_month10)
             | (state == func_menu_set_year1)
             | (state == func_menu_set_year10)) {
    show_menu_timeout = 0;
    date_show_cnt = 0;
    state = func_date;

    blink_state = false;
    show_date();
  }

  show_menu_timeout = set_menu_timeout;
  //  show_round_stop_timeout = 0;
}
void ctrl_cent_cmd(void) {
  byte HH_10;
  byte HH_1;
  byte MM_10;
  byte MM_1;

  byte dd_10;
  byte dd_1;
  byte mm_10;
  byte mm_1;
  byte yy_10;
  byte yy_1;

  dmd.clearScreen();
  delayWdt(50);

  if (state == func_menu_reset_round) {
    round_cnt10 = 0;
    round_cnt1 = 1;

    L10 = round_recall_min10;
    L1 = round_recall_min1;
    R10 = round_recall_sec10;
    R1 = round_recall_sec1;

    dmd.drawString(2, line1, "Reset");
    dmd.drawString(2, line2, "Ok");

    delayWdt(2000);

    dmd.clearScreen();

    state = func_normal;
    time_sel = func_normal;

    pause_state = true;
    blink_state = false;
    show_round_and_time();
  } else if (state == func_menu_bell_ding_mode) {
    if (bell_ding_mode_val < 7) {
      bell_ding_mode_val++;
    } else {
      bell_ding_mode_val = 0;
    }
    EEPROM.update(bell_ding_enabled_addr, bell_ding_mode_val);
  } else if (state == func_menu_set_round) {
    state = func_menu_set_round_time_mode;
    blink_state = true;
    show_round_and_time();
  } else if (state == func_menu_set_round_recall_normal) {
    state = func_menu_set_round_recall_normal_min10;
    blink_state = true;
    dmd.drawString(2, line1, "Fight");
    show_recal_round_normal();
  } else if (state == func_menu_set_round_recall_interval) {
    state = func_menu_set_round_recall_interval_min10;
    blink_state = true;
    dmd.drawString(2, line1, "Inter");
    show_recal_round_interval();
  } else if (state == func_menu_set_round10) {
    if (round_cnt10 < 9) {
      round_cnt10++;
    } else {
      round_cnt10 = 0;
    }
  } else if (state == func_menu_set_round1) {
    if (round_cnt1 < 9) {
      round_cnt1++;
    } else {
      if (round_cnt10 == 0) {
        round_cnt1 = 1;
      } else {
        round_cnt1 = 0;
      }
    }
  } else if (state == func_menu_set_round_time_mode) {
    if (time_sel == func_normal) {
      time_sel = func_interval;
      //      dmd.drawString(2, line1, "Inter");
      //      dmd.drawString(2, line2, "valo ");
      //      delayWdt(2000);
    } else //if(time_sel == func_interval)
    {
      time_sel = func_normal;
      //      dmd.drawString(2, line1, "Luta ");
      //      dmd.drawString(2, line2, "     ");
      //      delayWdt(2000);
    }
  } else if (state == func_menu_set_round_time_min10) {
    if (L10 < 9) {
      L10++;
    } else {
      L10 = 0;
    }
  } else if (state == func_menu_set_round_time_min1) {
    if (L1 < 9) {
      L1++;
    } else {
      L1 = 0;
    }
  } else if (state == func_menu_set_round_time_sec10) {
    if (R10 < 5) {
      R10++;
    } else {
      R10 = 0;
    }
  } else if (state == func_menu_set_round_time_sec1) {
    if (R1 < 9) {
      R1++;
    } else {
      R1 = 0;
    }
  } else if (state == func_menu_set_round_recall_normal_min10) {
    if (round_recall_min10 < 9) {
      round_recall_min10++;
    } else {
      round_recall_min10 = 0;
    }
    //  EEPROM.update(interval_recall_min10_addr, interval_recall_min10);           // interval round
    //  EEPROM.update(interval_recall_min1_addr, interval_recall_min1);           // interval round
    //  EEPROM.update(interval_recall_sec10_addr, interval_recall_sec10);           // interval round
    //  EEPROM.update(interval_recall_sec1_addr, interval_recall_sec1);           // interval round
    //
    //  EEPROM.update(normal_recall_min10_addr, round_recall_min10);           // round
    //  EEPROM.update(normal_recall_min1_addr, round_recall_min1);           // round
    //  EEPROM.update(normal_recall_sec10_addr, round_recall_sec10);           // round
    //  EEPROM.update(normal_recall_sec1_addr, round_recall_sec1);           // round
  } else if (state == func_menu_set_round_recall_normal_min1) {
    if (round_recall_min1 < 9) {
      round_recall_min1++;
    } else {
      round_recall_min1 = 0;
    }
  } else if (state == func_menu_set_round_recall_normal_sec10) {
    if (round_recall_sec10 < 5) {
      round_recall_sec10++;
    } else {
      round_recall_sec10 = 0;
    }
  } else if (state == func_menu_set_round_recall_normal_sec1) {
    if (round_recall_sec1 < 9) {
      round_recall_sec1++;
    } else {
      round_recall_sec1 = 0;
    }
  } else if (state == func_menu_set_round_recall_interval_min10) {
    if (interval_recall_min10 < 9) {
      interval_recall_min10++;
    } else {
      interval_recall_min10 = 0;
    }
  } else if (state == func_menu_set_round_recall_interval_min1) {
    if (interval_recall_min1 < 9) {
      interval_recall_min1++;
    } else {
      interval_recall_min1 = 0;
    }
  } else if (state == func_menu_set_round_recall_interval_sec10) {
    if (interval_recall_sec10 < 5) {
      interval_recall_sec10++;
    } else {
      interval_recall_sec10 = 0;
    }
  } else if (state == func_menu_set_round_recall_interval_sec1) {
    if (interval_recall_sec1 < 9) {
      interval_recall_sec1++;
    } else {
      interval_recall_sec1 = 0;
    }
  } else if (state == func_normal) {
    if (pause_state == true) {
      pause_state = false;
      show_round_and_time();
      bell_start();
    } else {
      pause_state = true;
      show_round_and_time();
      bell_stop();
    }

    show_round_and_time();
  } else if (state == func_interval) {
    if (pause_state == true) {
      pause_state = false;
    } else {
      pause_state = true;
    }

    show_round_and_time();
  } else if (state == func_clock) {
    clock_show_cnt = 0; // clear the counter, for alternating clock and date
    state = func_date;
    dmd.clearScreen();
    show_date();
  } else if (state == func_date) {
    date_show_cnt = 0; // clear the counter, for alternating clock and date
    state = func_clock;
    dmd.clearScreen();
    show_clock_and_temperature();
  } else if (state == func_menu_set_hour1) {
    HH_10 = wr_rtc_HH / 10;
    HH_1 = wr_rtc_HH - (HH_10 * 10);

    if (HH_10 == 2) {
      if (HH_1 < 3) {
        HH_1++;
      } else {
        HH_1 = 0;
      }
    } else {
      if (HH_1 < 9) {
        HH_1++;
      } else {
        HH_1 = 0;
      }
    }

    wr_rtc_HH = (HH_10 * 10) + HH_1;

    update_rtc();
  } else if (state == func_menu_set_hour10) {
    HH_10 = wr_rtc_HH / 10;
    HH_1 = wr_rtc_HH - (HH_10 * 10);

    if (HH_10 < 2) {
      HH_10++;
      HH_1_temp = HH_1;
    } else {
      HH_10 = 0;
      HH_1 = HH_1_temp;
    }

    wr_rtc_HH = (HH_10 * 10) + HH_1;

    if (wr_rtc_HH > 23) {
      wr_rtc_HH = 23;
    }

    update_rtc();
  } else if (state == func_menu_set_min1) {
    MM_10 = wr_rtc_MM / 10;
    MM_1 = wr_rtc_MM - (MM_10 * 10);

    if (MM_1 < 9) {
      MM_1++;
    } else {
      MM_1 = 0;
    }

    wr_rtc_MM = (MM_10 * 10) + MM_1;

    update_rtc();
  } else if (state == func_menu_set_min10) {
    MM_10 = wr_rtc_MM / 10;
    MM_1 = wr_rtc_MM - (MM_10 * 10);

    if (MM_10 < 5) {
      MM_10++;
    } else {
      MM_10 = 0;
    }

    wr_rtc_MM = (MM_10 * 10) + MM_1;

    if (wr_rtc_MM > 59) {
      wr_rtc_MM = 59;
    }

    update_rtc();
  }
  //  else if (state == "menu_set_time_date")
  //  {
  //    if (show_sel_time_date < 2)
  //    {
  //      show_sel_time_date++;
  //    }
  //    else
  //    {
  //      show_sel_time_date = 0;
  //    }
  //
  //    EEPROM.write(show_sel_time_date_addr, show_sel_time_date);
  //
  //    show_clock();
  //  }
  else if (state == func_menu_set_time_date_bright) {
    if (show_sel_time_date_bright < 51) {
      show_sel_time_date_bright++;
    } else {
      show_sel_time_date_bright = 1;
    }

    EEPROM.write(show_sel_time_date_bright_addr, show_sel_time_date_bright);

    dmd.setBrightness(show_sel_time_date_bright * 5);

    show_clock2();
  } else if (state == func_menu_set_day1) {
    dd_10 = wr_rtc_dd / 10;
    dd_1 = wr_rtc_dd - (dd_10 * 10);

    mm_10 = wr_rtc_mm / 10;
    mm_1 = wr_rtc_mm - (mm_10 * 10);

    // J f M a M j J A s O  n  D
    // 1 2 3 4 5 6 7 8 9 10 11 12
    if (wr_rtc_mm == 2) {
      if (wr_rtc_dd < 29) {
        if (dd_1 < 9) {
          dd_1++;
        } else {
          dd_1 = 0;
        }
      } else {
        dd_1 = 0;
      }
    } else if ((wr_rtc_mm == 4) | (wr_rtc_mm == 6) | (wr_rtc_mm == 9)
               | (wr_rtc_mm == 11)) {
      if (wr_rtc_dd < 30) {
        if (dd_1 < 9) {
          dd_1++;
        } else {
          dd_1 = 0;
        }
      } else {
        dd_1 = 0;
      }
    } else {
      if (wr_rtc_dd < 31) {
        if (dd_1 < 9) {
          dd_1++;
        } else {
          dd_1 = 0;
        }
      } else {
        dd_1 = 0;
      }
    }

    wr_rtc_dd = (dd_10 * 10) + dd_1;

    // J f M a M j J A s O  n  D
    // 1 2 3 4 5 6 7 8 9 10 11 12
    if ((wr_rtc_dd > 30)
        & ((wr_rtc_mm == 4) | (wr_rtc_mm == 6) | (wr_rtc_mm == 9)
           | (wr_rtc_mm == 11))) {
      wr_rtc_dd = 30;
    } else if ((wr_rtc_dd > 29) & (wr_rtc_mm == 2)) {
      wr_rtc_dd = 29;
    } else if (wr_rtc_dd > 31) {
      wr_rtc_dd = 31;
    } else if (wr_rtc_dd == 0) {
      wr_rtc_dd = 1;
    }
    update_rtc();
  } else if (state == func_menu_set_day10) {
    dd_10 = wr_rtc_dd / 10;
    dd_1 = wr_rtc_dd - (dd_10 * 10);

    mm_10 = wr_rtc_mm / 10;
    mm_1 = wr_rtc_mm - (mm_10 * 10);

    if (((dd_10 < 3) & (wr_rtc_mm != 2))
        | ((dd_10 < 2) & (wr_rtc_mm == 2))) {
      dd_10++;
      dd_1_temp = dd_1;
    } else {
      dd_10 = 0;
      dd_1 = dd_1_temp;
    }

    wr_rtc_dd = (dd_10 * 10) + dd_1;

    // J f M a M j J A s O  n  D
    // 1 2 3 4 5 6 7 8 9 10 11 12
    if ((wr_rtc_dd > 30)
        & ((wr_rtc_mm == 4) | (wr_rtc_mm == 6) | (wr_rtc_mm == 9)
           | (wr_rtc_mm == 11))) {
      wr_rtc_dd = 30;
    } else if ((wr_rtc_dd > 29) & (wr_rtc_mm == 2)) {
      wr_rtc_dd = 29;
    } else if (wr_rtc_dd > 31) {
      wr_rtc_dd = 31;
    } else if (wr_rtc_dd == 0) {
      wr_rtc_dd = 1;
    }

    update_rtc();
  } else if (state == func_menu_set_month1) {
    mm_10 = wr_rtc_mm / 10;
    mm_1 = wr_rtc_mm - (mm_10 * 10);

    if ((mm_1 < 9) & (wr_rtc_mm < 12)) {
      mm_1++;
    } else {
      mm_1 = 0;
    }

    wr_rtc_mm = (mm_10 * 10) + mm_1;

    if (wr_rtc_mm > 12) {
      wr_rtc_mm = 12;
    } else if (wr_rtc_mm == 0) {
      wr_rtc_mm = 1;
    }

    update_rtc();
  } else if (state == func_menu_set_month10) {
    mm_10 = wr_rtc_mm / 10;
    mm_1 = wr_rtc_mm - (mm_10 * 10);

    if (mm_10 < 1) {
      mm_10++;
      mm_1 = mm_1_temp;
    } else {
      mm_10 = 0;
    }

    wr_rtc_mm = (mm_10 * 10) + mm_1;

    if (wr_rtc_mm > 12) {
      wr_rtc_mm = 12;
    } else if (wr_rtc_mm == 0) {
      mm_1_temp = mm_1;
      wr_rtc_mm = 1;
    }

    update_rtc();
  } else if (state == func_menu_set_year1) {
    yy_10 = (wr_rtc_yy - 2000) / 10;
    yy_1 = (wr_rtc_yy - 2000) - (yy_10 * 10);

    if (yy_1 < 9) {
      yy_1++;
    } else {
      yy_1 = 0;
    }

    wr_rtc_yy = 2000 + ((yy_10 * 10) + yy_1);

    update_rtc();
  } else if (state == func_menu_set_year10) {
    yy_10 = (wr_rtc_yy - 2000) / 10;
    yy_1 = (wr_rtc_yy - 2000) - (yy_10 * 10);

    if (yy_10 < 9) {
      yy_10++;
    } else {
      yy_10 = 0;
    }

    wr_rtc_yy = 2000 + ((yy_10 * 10) + yy_1);

    update_rtc();
  }

  blink_alternate = true;

  show_menu_timeout = set_menu_timeout;
  //  show_round_stop_timeout = 0;
}
void ctrl_right_cmd(void) {
  dmd.clearScreen();
  delayWdt(50);

  if ((state == func_normal) | (state == func_interval)) {
    pause_state_previous = pause_state;
    pause_state = true;
    state = func_menu_reset_round;
    blink_state = false;
    dmd.drawString(2, line1, "Reset");
    //#ifdef language_PT_BR
    //    dmd.drawString(2, line2, "Cont?");
    //#else
    dmd.drawString(2, line2, "Cnt?");
    //#endif
  } else if (state == func_menu_reset_round) {
    state = func_menu_set_round;
    dmd.drawString(2, line1, "Set");
    dmd.drawString(2, line2, "Cnt?");
  } else if (state == func_menu_set_round) {
    state = func_menu_set_round_recall_normal;
    dmd.drawString(2, line1, "Set");
    dmd.drawString(2, line2, "Ft?");
  } else if (state == func_menu_set_round_recall_normal) {
    state = func_menu_set_round_recall_interval;
    dmd.drawString(2, line1, "Set");
    dmd.drawString(2, line2, "Intv?");
  } else if (state == func_menu_set_round_recall_interval) {
    blink_state = true;
    state = func_menu_bell_ding_mode;
  } else if (state == func_menu_bell_ding_mode) {
    blink_state = false;
    state = func_menu_set_volume_beep;
    dmd.drawString(2, line1, "Volum");
    dmd.drawString(2, line2, "B: " + String(music_volume));
  } else if (state == func_menu_set_volume_beep) {
    blink_state = false;
    state = func_menu_set_volume_music;
    dmd.drawString(2, line1, "Volum");
    dmd.drawString(2, line2, "M: " + String(music_volume));
  } else if (state == func_menu_set_volume_music) {
    blink_state = false;
    state = func_menu_set_volume_voice;
    dmd.drawString(2, line1, "Volum");
    dmd.drawString(2, line2, "V: " + String(voice_volume));
  } else if (state == func_menu_set_volume_voice) {
    state = func_menu_reset_round; // retorna
    blink_state = false;
    dmd.drawString(2, line1, "Reset");
    dmd.drawString(2, line2, "?");
  } else if (state == func_menu_set_round_time_mode) {
    state = func_menu_set_round10;
  } else if (state == func_menu_set_round10) {
    state = func_menu_set_round1;
  } else if (state == func_menu_set_round1) {
    state = func_menu_set_round_time_min10;
  } else if (state == func_menu_set_round_time_min10) {
    state = func_menu_set_round_time_min1;
  } else if (state == func_menu_set_round_time_min1) {
    state = func_menu_set_round_time_sec10;
  } else if (state == func_menu_set_round_time_sec10) {
    state = func_menu_set_round_time_sec1;
  } else if (state == func_menu_set_round_time_sec1) {
    state = func_menu_set_round_time_mode; // return
  } else if (state == func_menu_set_round_recall_normal_min10) {
    state = func_menu_set_round_recall_normal_min1;
  } else if (state == func_menu_set_round_recall_normal_min1) {
    state = func_menu_set_round_recall_normal_sec10;
  } else if (state == func_menu_set_round_recall_normal_sec10) {
    state = func_menu_set_round_recall_normal_sec1;
  } else if (state == func_menu_set_round_recall_normal_sec1) {
    state = func_menu_set_round_recall_normal_min10; // return
    //    state = time_sel;
    //    blink_state = false;
    //    show_round_and_time();
  } else if (state == func_menu_set_round_recall_interval_min10) {
    state = func_menu_set_round_recall_interval_min1;
  } else if (state == func_menu_set_round_recall_interval_min1) {
    state = func_menu_set_round_recall_interval_sec10;
  } else if (state == func_menu_set_round_recall_interval_sec10) {
    state = func_menu_set_round_recall_interval_sec1;
  } else if (state == func_menu_set_round_recall_interval_sec1) {
    state = func_menu_set_round_recall_interval_min10; // return
    //    state = time_sel;
    //    blink_state = false;
    //    show_round_and_time();
  } else if (state == func_clock) {
    state = func_menu_set_time_date_bright;
    blink_state = true;
  } else if (state == func_menu_set_time_date_bright) {
    state = func_menu_set_hour10;
  } else if (state == func_menu_set_hour10) {
    state = func_menu_set_hour1;
  } else if (state == func_menu_set_hour1) {
    state = func_menu_set_min10;
  } else if (state == func_menu_set_min10) {
    state = func_menu_set_min1;
  } else if (state == func_menu_set_min1) {
    state = func_menu_set_time_date_bright; // return
  } else if (state == func_date) {
    state = func_menu_set_day10;
    blink_state = true;
  } else if (state == func_menu_set_day10) {
    state = func_menu_set_day1;
  } else if (state == func_menu_set_day1) {
    state = func_menu_set_month10;
  } else if (state == func_menu_set_month10) {
    state = func_menu_set_month1;
  } else if (state == func_menu_set_month1) {
    state = func_menu_set_year10;
  } else if (state == func_menu_set_year10) {
    state = func_menu_set_year1;
  } else if (state == func_menu_set_year1) {
    state = func_menu_set_day10; // return
  }

  blink_alternate = true;

  show_menu_timeout = set_menu_timeout;
  //  show_round_stop_timeout = 0;
}
void blink_preset(void) {
  blink_string1 = "_";

  if ((state == func_interval) | (state == func_menu_bell_ding_mode)) {
    blink_position_X = 0;
    blink_position_Y = line2;
    blink_string1 = "      ";
  }
  if (state == func_menu_set_round10) {
    blink_position_X = 20;
    blink_position_Y = line1;
  } else if (state == func_menu_set_round1) {
    blink_position_X = 26;
    blink_position_Y = line1;
  } else if (state == func_menu_set_round_time_mode) {
    blink_position_X = 2; // 26
    blink_position_Y = line1;
    blink_string1 = "  ";
  } else if ((state == func_menu_set_round_time_min10)
             | (state == func_menu_set_round_recall_normal_min10)
             | (state == func_menu_set_round_recall_interval_min10)) {
    blink_position_X = 2;
    blink_position_Y = line2;
  } else if ((state == func_menu_set_round_time_min1)
             | (state == func_menu_set_round_recall_normal_min1)
             | (state == func_menu_set_round_recall_interval_min1)) {
    blink_position_X = 8;
    blink_position_Y = line2;
  } else if ((state == func_menu_set_round_time_sec10)
             | (state == func_menu_set_round_recall_normal_sec10)
             | (state == func_menu_set_round_recall_interval_sec10)) {
    blink_position_X = 20;
    blink_position_Y = line2;
  } else if ((state == func_menu_set_round_time_sec1)
             | (state == func_menu_set_round_recall_normal_sec1)
             | (state == func_menu_set_round_recall_interval_sec1)) {
    blink_position_X = 26;
    blink_position_Y = line2;
  } else if (state == func_menu_set_hour1) {
    blink_position_X = 8;
    blink_position_Y = line2;
  } else if (state == func_menu_set_hour10) {
    blink_position_X = 2;
    blink_position_Y = line2;
  } else if (state == func_menu_set_min1) {
    blink_position_X = 25;
    blink_position_Y = line2;
  } else if (state == func_menu_set_min10) {
    blink_position_X = 19;
    blink_position_Y = line2;
  } else if (state == func_menu_set_day1) {
    blink_position_X = 8;
    blink_position_Y = line1;
  } else if (state == func_menu_set_day10) {
    blink_position_X = 2;
    blink_position_Y = line1;
  } else if (state == func_menu_set_month1) {
    blink_position_X = 25;
    blink_position_Y = line1;
  } else if (state == func_menu_set_month10) {
    blink_position_X = 19;
    blink_position_Y = line1;
  } else if (state == func_menu_set_time_date_bright) {
    blink_position_X = 14;
    blink_position_Y = line1;
    blink_string1 = "   ";
  }
  //  else if (state == "menu_set_time_date")
  //  {
  //    blink_position_X = 0;
  //    blink_position_Y = line2;
  //    blink_string1 = "      ";
  //  }
  else if (state == func_menu_set_year1) {
    blink_position_X = 22;
    blink_position_Y = line2;
  } else if (state == func_menu_set_year10) {
    blink_position_X = 16;
    blink_position_Y = line2;
  }
}
void blink_show(void) {
  blink_preset();

  if (blink_alternate == false) {
    blink_alternate = true;
    if (!((state == func_interval) & (pause_state == true))) {
      dmd.drawString(blink_position_X, blink_position_Y, blink_string1);
    }
  } else {
    blink_alternate = false;

    if (state == func_menu_bell_ding_mode) {
      show_bell_state();
    } else if ((state == func_menu_set_round_recall_normal_min10)
               | (state == func_menu_set_round_recall_normal_min1)
               | (state == func_menu_set_round_recall_normal_sec10)
               | (state == func_menu_set_round_recall_normal_sec1)) {

      //#ifdef language_PT_BR
      //      dmd.drawString(2, line1, "Luta ");
      //#else
      dmd.drawString(2, line1, "Fight");
      //#endif

      show_recal_round_normal();
    } else if ((state == func_menu_set_round_recall_interval_min10)
               | (state == func_menu_set_round_recall_interval_min1)
               | (state == func_menu_set_round_recall_interval_sec10)
               | (state == func_menu_set_round_recall_interval_sec1)) {
      dmd.drawString(2, line1, "Inter");
      show_recal_round_interval();
    } else if ((state == func_menu_set_round10)
               | (state == func_menu_set_round1)
               | (state == func_menu_set_round_time_mode)
               | (state == func_menu_set_round_time_min10)
               | (state == func_menu_set_round_time_min1)
               | (state == func_menu_set_round_time_sec10)
               | (state == func_menu_set_round_time_sec1)) {
      show_round_and_time();
    } else if ((state == func_menu_set_hour1)
               | (state == func_menu_set_hour10)
               | (state == func_menu_set_min1) | (state == func_menu_set_min10)
               | (state == func_menu_set_time_date_bright)) //(state == "menu_set_time_date")
    {
      show_clock2();
    } else if ((state == func_menu_set_day1)
               | (state == func_menu_set_day10)
               | (state == func_menu_set_month1)
               | (state == func_menu_set_month10)
               | (state == func_menu_set_year1)
               | (state == func_menu_set_year10)) {
      show_date2();
    }
  }
}
void show_bell_state(void) {
  dmd.drawString(2, line1, "Notif");

  if (bell_ding_mode_val == 0) {
    dmd.drawString(2, line2, "Beep ");
  } else if (bell_ding_mode_val == 1) {
    dmd.drawString(2, line2, "Music");
  } else if (bell_ding_mode_val == 2) {
    dmd.drawString(2, line2, "Voice");
  } else if (bell_ding_mode_val == 3) {
    dmd.drawString(2, line2, "B+M  ");
  } else if (bell_ding_mode_val == 4) {
    dmd.drawString(2, line2, "B+V  ");
  } else if (bell_ding_mode_val == 5) {
    dmd.drawString(2, line2, "M+V  ");
  } else if (bell_ding_mode_val == 6) {
    dmd.drawString(2, line2, "B+M+V");
  } else if (bell_ding_mode_val == 7) {
    dmd.drawString(2, line2, "Off  ");
  }

  //    if (bell_ding_mode_val == true)
  //    {
  //      dmd.drawString(2, line2, "<On> ");
  //    }
  //    else
  //    {
  //      dmd.drawString(2, line2, "<Off>");
  //    }
}
void show(void) {
  if (state == func_clock) {
    if (clock_show_cnt < 19) {
      clock_show_cnt++;
    } else {
      clock_show_cnt = 0;

      //      if (show_sel_time_date == 2)
      //      {
      state = func_date;
      dmd.drawString(0, line1, "      ");
      dmd.drawString(0, line2, "      ");
      show_date();
      //      }
    }
  } else if (state == func_date) {
    if (date_show_cnt < 9) {
      date_show_cnt++;
    } else {
      date_show_cnt = 0;

      if (pause_state == false) {
        state = time_sel;
        if (time_sel == func_interval) {
          blink_state = true;
        }
        show_round_and_time();
      } else {
        state = func_clock;
      }

      dmd.drawString(0, line1, "      ");
      dmd.drawString(0, line2, "      ");
      show_clock_and_temperature();
    }
  }
}
void show_clock_and_temperature(void) {
  show_clock();
  show_temperature();
}
void update_rtc(void) {
  rtc.adjust(
    DateTime(wr_rtc_yy, wr_rtc_mm, wr_rtc_dd, wr_rtc_HH, wr_rtc_MM, 0)); // yyyy,mm,dd,HH,MM,SS
}
void show_date(void) {
  String year1;
  String month1;
  String day1;

  DateTime now = rtc.now();

  wr_rtc_HH = now.hour();
  wr_rtc_MM = now.minute();

  wr_rtc_yy = now.year();
  wr_rtc_mm = now.month();
  wr_rtc_dd = now.day();

  day1 = String(now.day());

  if (now.day() < 10) {
    day1 = "0" + String(now.day());
  }

  if (now.day() > 31) {
    day1 = "--";
  }

  month1 = String(now.month());

  if (now.month() < 10) {
    month1 = "0" + String(now.month());
  }

  if (now.month() > 31) {
    month1 = "--";
  }

  year1 = String(now.year());

  if (now.year() > 2099) {
    year1 = "----";
  }

  dmd.drawString(2, line1, day1 + "/");
  dmd.drawString(19, line1, month1);
  //dmd.drawString(4, line2, year1);
  dmd.drawString(2, line2, daysOfTheWeek[now.dayOfTheWeek()]);

}
void show_date2(void) {
  String year1;
  String month1;
  String day1;

  DateTime now = rtc.now();

  wr_rtc_HH = now.hour();
  wr_rtc_MM = now.minute();

  wr_rtc_yy = now.year();
  wr_rtc_mm = now.month();
  wr_rtc_dd = now.day();

  day1 = String(now.day());

  if (now.day() < 10) {
    day1 = "0" + String(now.day());
  }

  if (now.day() > 31) {
    day1 = "--";
  }

  month1 = String(now.month());

  if (now.month() < 10) {
    month1 = "0" + String(now.month());
  }

  if (now.month() > 31) {
    month1 = "--";
  }

  year1 = String(now.year());

  if (now.year() > 2099) {
    year1 = "----";
  }

  dmd.drawString(2, line1, day1 + "/");
  dmd.drawString(19, line1, month1);
  dmd.drawString(4, line2, year1);
  //dmd.drawString(2, line2, daysOfTheWeek[now.dayOfTheWeek()]);

}
int iHour;
int iMin;

void show_clock(void) {
  String hour1;
  String min1;

  DateTime now = rtc.now();

  iHour = now.hour();
  iMin = now.minute();

  wr_rtc_HH = now.hour();
  wr_rtc_MM = now.minute();

  wr_rtc_yy = now.year();
  wr_rtc_mm = now.month();
  wr_rtc_dd = now.day();

  hour1 = String(now.hour());

  if (now.hour() < 10) {
    hour1 = "0" + String(now.hour());
  }

  if (now.hour() > 23) {
    hour1 = "--";
  }

  min1 = String(now.minute());

  if (now.minute() < 10) {
    min1 = "0" + String(now.minute());
  }

  if (now.minute() > 59) {
    min1 = "--";
  }

  dmd.drawString(2, line1, hour1);
  dmd.drawString(14, line1, ":");
  dmd.drawString(19, line1, min1);
}
void show_clock2(void) {
  String hour1;
  String min1;

  DateTime now = rtc.now();

  wr_rtc_HH = now.hour();
  wr_rtc_MM = now.minute();

  wr_rtc_yy = now.year();
  wr_rtc_mm = now.month();
  wr_rtc_dd = now.day();

  hour1 = String(now.hour());

  if (now.hour() < 10) {
    hour1 = "0" + String(now.hour());
  }

  if (now.hour() > 23) {
    hour1 = "--";
  }

  min1 = String(now.minute());

  if (now.minute() < 10) {
    min1 = "0" + String(now.minute());
  }

  if (now.minute() > 59) {
    min1 = "--";
  }

  dmd.drawString(2, line2, hour1);
  dmd.drawString(14, line2, ":");
  dmd.drawString(19, line2, min1);

  //  if ((state == func_menu_set_hour1) |
  //      (state == func_menu_set_hour10) |
  //      (state == func_menu_set_min1) |
  //      (state == func_menu_set_min10) |
  //      (state == func_menu_set_time_date_bright)) //(state == "menu_set_time_date") |
  //  {
  dmd.drawString(2, line1, "b:" + String(show_sel_time_date_bright * 5));
  //  }
}
void show_temperature(void) {
  String temp_string;
  String temp_dec_string;

  temp_dec_string = String(int((temperature * 10) - int(temperature) * 10));

  if (temperature == 0) {
    temp_string = "--";
    temp_dec_string = "-";
  } else if (temperature < 10) {
    temp_string = "0" + String(int(temperature));
  } else {
    temp_string = String(int(temperature));
  }

  dmd.drawString(2, 9, temp_string);
  dmd.drawString(14, 9, ".");
  dmd.drawString(19, 9, temp_dec_string);
  //dmd.drawCircle(27, 11, 2);

  dmd.drawLine(25, 10, 25, 11); //°
  dmd.drawLine(26, 9, 27, 9);
  dmd.drawLine(28, 10, 28, 11);
  dmd.drawLine(26, 12, 27, 12);

  //    dmd.drawString(23,9,"C");
}
void temperature_init(void) {
  byte loop_init1;
  //  float calc_temp1;

  for (loop_init1 = 0; loop_init1 <= 49; loop_init1++) {
    temperature_read();
    //temperature_array[loop1] = int((5.0 * float(analogRead(LM35)) * 100.0) / 1024.0); //int(float(analogRead(LM35))/ 9.31);
    //    temperature_array[loop1] = analogRead(LM35) / 9.31;
  }

  //  calc_temp1 = 0;
  //
  //  for (loop1 = 0; loop1 <= 49; loop1++)
  //  {
  //    calc_temp1 = calc_temp1 + temperature_array[loop1];
  //  }
  //
  //  //temperature = (float(calc_temp1) / 50.0) +1;
  //  temperature = calc_temp1 / 50.0;

}
void temperature_read(void) {
  byte loop1;
  float calc_temp1;

  //temperature_array[temperature_index] = int((5.0 * float(analogRead(LM35)) * 100.0) / 1024.0);
  //temperature_array[temperature_index] = int((1.1 * float(analogRead(LM35)) * 100.0) / 1024.0);
  temperature_array[temperature_index] = analogRead(LM35) / 9.31;

  if (temperature_index <= 49) {
    temperature_index++;
  } else {
    temperature_index = 0;
  }

  calc_temp1 = 0;

  for (loop1 = 0; loop1 <= 49; loop1++) {
    calc_temp1 = calc_temp1 + temperature_array[loop1];
  }

  //  temperature = temperature_array[temperature_index];

  temperature = calc_temp1 / 50.0;
}
void bell_stop(void) {
  return;

  if (bell_ding_mode_val == true) {
    Bell_ding();

    delayWdt(300);

    for (int i = 0; i < 3; i++) {
      dfPlayMP3(33); // beep

      delayWdt(50);

      waitDfPlayer();
    }

    dfPlayFolder(1, 246); // interval

    delayWdt(50);

    waitDfPlayer();

    if (pause_state == false) {
      musicRandStart();
    }
  }
}

void bell_start(void) {
  return;

  if (bell_ding_mode_val == true) {
    Bell_ding();

    Bell_ding();

    delayWdt(300);

    for (int i = 0; i < 3; i++) {
      dfPlayMP3(11); // beep

      delayWdt(50);

      waitDfPlayer();
    }

    dfPlayFolder(1, 251); // round

    delayWdt(50);

    waitDfPlayer();

    dfPlayFolder(1, ((round_cnt10 * 10) + round_cnt1)); // round

    delayWdt(50);
  }
}

void musicRandStart(void) {
  //  int timeout = 0;
  //  while (digitalRead(12)) { // waiting to find valid music
  //    dfPlayFolder(random(2, 7), random(1, 255)); // random music
  //
  //    delayWdt(50);
  //
  //    timeout++;
  //
  //    if (timeout > 200) break;
  //  }
}

void Bell_ding(void) {
  digitalWrite(Bell, HIGH);

  delayWdt(10);

  digitalWrite(Bell, LOW);

  delayWdt(100);

  digitalWrite(Bell, HIGH);

  delayWdt(10);

  digitalWrite(Bell, LOW);

  delayWdt(200);
}
void serialEvent(void) {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char) Serial.read();
    if (inChar == '\n') {
      //DateTime now = rtc.now();
      int L10_tmp = L10, L1_tmp = L1, R10_tmp = R10, R1_tmp = R1;

      if ((inputString.length() == 2) & (inputString[1] == 0x0D))
        inputString[1] = 0; // remove "\r" 0x0D

      if (inputString == "0") {
        //        input_ctrl_right_serial = false;
        //        Serial.println("ctrl_dir");
      } else if (inputString == "2") {
        //if (pause_state != true) {
        input_ctrl_left_serial = false;
        //}
        //        Serial.println("ctrl_esq");
      } else if (inputString == "1") {
        input_ctrl_right_serial = false;
        //        Serial.println("ctrl_dir");
      } else if (inputString == "3") {
        input_ctrl_cent_serial = false;
        //        Serial.println("ctrl_cent");
      }

      uint8_t state10 = state / 10;
      uint8_t state1 = state - (state10 * 10);
      uint8_t stateAux = 0;
      uint8_t stateAux10 = 0;
      uint8_t stateAux1 = 0;

      //#define func_menu_set_round_time_mode 12
      //#define func_menu_set_round10 13
      //#define func_menu_set_round1 14
      //#define func_menu_set_round_time_min10 15
      //#define func_menu_set_round_time_min1 16
      //#define func_menu_set_round_time_sec10 17
      //#define func_menu_set_round_time_sec1 18
      //time_sel

      if ((state == func_normal) | (state == func_interval)
          | (state == func_menu_set_round_time_mode)
          | (state == func_menu_set_round10) | (state == func_menu_set_round1)
          | (state == func_menu_set_round_time_min10)
          | (state == func_menu_set_round_time_min1)
          | (state == func_menu_set_round_time_sec10)
          | (state == func_menu_set_round_time_sec1)) {
        if ((state == func_normal) | (state == func_interval)) {
          stateAux = pause_state;
        } else {
          stateAux = time_sel;
        }
        stateAux10 = round_cnt10;
        stateAux1 = round_cnt1;
      } else if (state == func_menu_bell_ding_mode) {
        stateAux = bell_ding_mode_val;
      } else if (state == func_menu_set_volume_beep) {
        stateAux10 = beep_volume / 10;
        stateAux1 = beep_volume - (stateAux10 * 10);
      } else if (state == func_menu_set_volume_music) {
        stateAux10 = music_volume / 10;
        stateAux1 = music_volume - (stateAux10 * 10);
      } else if (state == func_menu_set_volume_voice) {
        stateAux10 = voice_volume / 10;
        stateAux1 = voice_volume - (stateAux10 * 10);
      } else if (state == func_clock) {
        L10_tmp = iHour / 10;
        L1_tmp = (iHour - (L10_tmp * 10));
        R10_tmp = iMin / 10;
        R1_tmp = (iMin - (R10_tmp * 10));

        //        wr_rtc_yy = now.year();
        //        wr_rtc_mm = now.month();
        //        wr_rtc_dd = now.day();
        //
        //        hour1 = String(now.hour());
        //
        //        min1 = String(now.minute());
      } else if (state == func_date) {
        //        L10_tmp = now.day() / 10;
        //        L1_tmp = (now.day() - (L10_tmp * 10));
        //        R10_tmp = now.month() / 10;
        //        R1_tmp = (now.month() - (R10_tmp * 10));

        //        wr_rtc_yy = now.year();
        //        wr_rtc_mm = now.month();
        //        wr_rtc_dd = now.day();
        //
        //        hour1 = String(now.hour());
        //
        //        min1 = String(now.minute());
      }

      Serial.print(state10, DEC);
      Serial.print(state1, DEC);
      Serial.print(",");
      Serial.print(stateAux, DEC);
      Serial.print(",");
      Serial.print(stateAux10, DEC);
      Serial.print(stateAux1, DEC);
      Serial.print(",");
      Serial.print(L10_tmp, DEC);
      Serial.print(L1_tmp, DEC);
      Serial.print(",");
      Serial.print(R10_tmp, DEC);
      Serial.println(R1_tmp, DEC);

      //      if (inputString.length() > 0) {
      //        for (int i = 0; i < inputString.length(); i++) {
      //          Serial.print("0x");
      //          if (inputString[i] < 16) Serial.print("0");
      //          Serial.print(inputString[i], HEX);
      //          Serial.print(" ");
      //        }
      //      }
      //      Serial.println();
      inputString = "";
    } else {
      if (inputString.length() < 20) {
        inputString += inChar;
      }
    }
  }
}

void dfPlayMP3(uint16_t music) {
  // play 1: 7E FF 06 12 00 00 01 FE E8 EF
  // checksum: 0 - (FF+06+12+00+00+01)
  uint8_t m1[10] = { 0x7E, 0xFF, 0x06, 0x12, 0x00, 0x00, 0, 0, 0, 0xEF };

  m1[5] = (music >> 8) & 0xFF;
  m1[6] = music & 0xFF;

  doSum(m1, 7);

  for (uint8_t i = 0; i < 10; i++) {
    softSerial.write(m1[i]);
  }
}

void dfPlayFolder(uint8_t folder, uint8_t music) {
  // play 1: 7E FF 06 12 00 00 01 FE E8 EF
  // checksum: 0 - (FF+06+12+00+00+01)
  uint8_t m1[10] = { 0x7E, 0xFF, 0x06, 0x0F, 0x00, 0x00, 0, 0, 0, 0xEF };

  if (folder > 99)
    folder = 99; // it is made to support maximum 99 folders and
  // each folder maximum 255 songs supported.

  m1[5] = folder;
  m1[6] = music;

  doSum(m1, 7);

  for (uint8_t i = 0; i < 10; i++) {
    softSerial.write(m1[i]);
  }

  delayWdt(10);

  //  Serial.println();
  //
  //  for (uint8_t i = 0; i < 10; i++) {
  //    Serial.print(m1[i], HEX);
  //    Serial.print(", ");
  //  }
  //  Serial.println();
}

void dfVolume(uint8_t volume) {
  // volume 15: 7E FF 06 06 00 00 0F FF D5 EF
  // checksum: 0 - (FF+06+06+00+00+0F)
  uint8_t m1[10] = { 0x7E, 0xFF, 0x06, 0x06, 0x00, 0x00, 0, 0, 0, 0xEF };

  m1[6] = volume;

  doSum(m1, 7);

  for (uint8_t i = 0; i < 10; i++) {
    softSerial.write(m1[i]);
  }
}

void dfStop1(void) {
  // 7E FF 06 16 00 00 00 FE E5 EF
  uint8_t m1[10] = { 0x7E, 0xFF, 0x06, 0x16, 0, 0, 0, 0xFE, 0xE5, 0xEF };

  for (uint8_t i = 0; i < 10; i++) {
    softSerial.write(m1[i]);
  }
}

void dfReset(void) {
  // 7E FF 06 0C 00 00 00 FE EF EF
  uint8_t m1[10] = { 0x7E, 0xFF, 0x06, 0x0C, 0, 0, 0, 0xFE, 0xEF, 0xEF };

  for (uint8_t i = 0; i < 10; i++) {
    softSerial.write(m1[i]);
  }
}

void waitDfPlayer(void) {
  int timeout1 = 0;

  while (digitalRead(12)) {
    //wdt_reset();  //  reset watchdog
    delayWdt(50);
    timeout1++;
    if (timeout1 > 50)
      break;
  }

  timeout1 = 0;

  while (!digitalRead(12)) {
    //wdt_reset();  //  reset watchdog
    delayWdt(50);
    timeout1++;
    if (timeout1 > 50)
      break;
  }
}

void doSum(uint8_t *frame, uint8_t len) {
  uint16_t xorsum = 0;
  uint8_t i = 0;

  for (i = 1; i < len; i++) {
    xorsum = xorsum + (frame[i]);
  }

  xorsum = 0 - xorsum;
  frame[i] = (xorsum >> 8) & 0xFF;
  frame[i + 1] = xorsum & 0xFF;
}
