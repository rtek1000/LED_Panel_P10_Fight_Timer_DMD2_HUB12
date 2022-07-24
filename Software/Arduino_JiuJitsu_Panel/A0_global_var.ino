#include <avr/wdt.h> // Arduino IDE

//// Data wire is plugged into port 2 on the Arduino
//#define ONE_WIRE_BUS A0 // from LM35 to DS18B20
//
//// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
//OneWire oneWire(ONE_WIRE_BUS);
//
//// Pass our oneWire reference to Dallas Temperature.
//DallasTemperature sensors(&oneWire);

//#define language_EN 1
//#define language_PT_BR 1
//#define skip_init_msg 1

//#define skip_LM35 1

//SoftwareSerial softSerial(5, 4);// RX, TX

// #define dfPlayerStatus 3 // input

#define DEBUG 1

//------ Navigations & Menu - end

#define input_ctrl_logic_high 1

//------ I/O - start
// RF remote control 433MHz
#define input_ctrl_left_pin A3 // A3 // btn 1 - A3 (Left)
#define input_ctrl_cent_pin A2 // 2 // btn 2 - 2  (Center)
#define input_ctrl_right_pin A1 // 3 // btn 3 - 3  (Right)

// Temperature sensor
#define LM35 A6 // To LM35/

// Corrective factor:
// 28.3°C (Arduino) / 24.7°C (0.247V using Multimeter) = 1.145748988
// 126°C (Using hot air from JCD 8898) / 100.0°C (Arduino) = 1.26

// 0.48828125 / 1.145748988 = 0.426167734
// 0.48828125 / 1.26 = 0.387524802
#define Corrective_Factor 0.387524802 // 0.426167734

// Bell defines (for warnings)
// MP3 files located on DFPlayer Mini's SD card
// The sound must be on the SD card warning the start of the round (1.mp3)
// The sound must be on the SD card warning the start of the interval (2.mp3)
#define beep_folder 1 // folder 01

#define beep_start_num 1 // file 001.mp3
#define beep_stop_num 2 // file 002.mp3

#define beep_start_time 1500 // 1500ms
#define beep_stop_time 1500 // 1500ms
//#define Bell 2/ //A1 // output

// Voice defines (for warnings)
// MP3 files located on DFPlayer Mini's SD card
// Voices must be on the SD card speaking the numbers 1 to 99 (from 1.mp3 to 99.mp3)
// The voice must be on the SD card speaking the round start warning (250.mp3)
// The voice must be on the SD card speaking the interval start warning (252.mp3)

#define voice_folder 2 // folder 02

#define voice_001 250 // file 001.mp3
#define voice_099 250 // file 099.mp3
#define voice_round 250 // file 250.mp3
#define voice_interval 252 // file 252.mp3

#define voice_001_099_time 2000 // 2000ms
#define voice_round_time 1500 // 1500ms
#define voice_interval_time 1500 // 1500ms

// RTC SQW
#define SQW_interruptPin 2

byte state_sqw = LOW;
byte state_sqw_old = LOW;

// Unused

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

//------ Display P10 line position - start
#define line1 0
#define line2 9
#define dig1 2
#define dig2 8
#define dig3 14
#define dig4 20
#define dig5 26

//------ Display P10 line position - end

//------ Navigations & Menu - start

enum {
  func_normal = 0,
  func_interval,
  func_clock,
  func_date,
  func_menu_set_round_recall_normal_min10,
  func_menu_set_round_recall_normal_min1,
  func_menu_set_round_recall_normal_sec10,
  func_menu_set_round_recall_normal_sec1,
  func_menu_set_round_recall_interval_min10,
  func_menu_set_round_recall_interval_min1,
  func_menu_set_round_recall_interval_sec10,
  func_menu_set_round_recall_interval_sec1,
  func_menu_set_round_time_mode,
  func_menu_set_round10,
  func_menu_set_round1,
  func_menu_set_round_time_min10,
  func_menu_set_round_time_min1,
  func_menu_set_round_time_sec10,
  func_menu_set_round_time_sec1,
  func_menu_set_hour1,
  func_menu_set_hour10,
  func_menu_set_min1,
  func_menu_set_min10,
  func_menu_set_time_date_bright,
  func_menu_set_day1,
  func_menu_set_day10,
  func_menu_set_month1,
  func_menu_set_month10,
  func_menu_set_year1,
  func_menu_set_year10,
  func_menu_reset_round,
  func_menu_set_round,
  func_menu_set_round_recall_normal,
  func_menu_set_round_recall_interval,
  func_menu_bell_ding_mode,
  func_menu_bell_ding_mode_intv,
  func_menu_set_volume_beep,
  // func_menu_set_volume_music,
  func_menu_set_volume_voice
};

enum {
  audio_off = 0,
  audio_beep,
  audio_voice,
  audio_beep_voice,
  audio_music,
  audio_beep_music,
  audio_music_voice,
  audio_beep_music_voice
};

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

boolean input_ctrl_left_pin_old = true;
boolean input_ctrl_cent_pin_old = true;
boolean input_ctrl_right_pin_old = true;

int bell_ding_mode_val = 0;
int bell_ding_mode_val_intv = 0;

byte show_menu_timeout = 0;

byte show_sel_time_date_bright; // value from eeprom

#define max_array_temp 5

float temperature = 0;
float temperature_array[max_array_temp + 1] = {0}; // to resolve this warning: invokes undefined behavior (+1)
byte temperature_index = 0;

bool blink_temp = false;
unsigned long millis_temp = 0;
unsigned long millis_high_temp = 0;
float rtc_temp = 0;

byte state = 0; // 0 = func_normal; //clock

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

int iHour;
int iMin;

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
#define intervalMillis 499 // interval at which to blink (milliseconds)
unsigned long previousMillis_meter = 0; // will store last time LED was updated

unsigned int wr_rtc_yy;
byte wr_rtc_mm;
byte wr_rtc_dd;
byte wr_rtc_HH;
byte wr_rtc_MM;

byte HH_1_temp;

byte dd_1_temp;
byte mm_1_temp;


String inputString = "";

//int timeout1 = 0;

//#define beep_volume_addr 17
//#define music_volume_addr 18
//#define voice_volume_addr 19

int beep_volume = 20;
int music_volume = 20;
int voice_volume = 30;

bool send_data_start = false;

uint32_t millisTemp = 0;

uint32_t millisAutoPlay = 0;

uint32_t millisControl = 0;

uint32_t continuous_play_millis = 0;

RTC_DS3231 rtc; //RTC_DS3231

SoftDMD dmd(1, 1); // DMD controls the entire display

DMD_TextBox box( dmd); // "box" provides a text box to automatically write to/scroll the display

DateTime now;

DFRobotDFPlayerMini DFPlayer1;

void delayWdt(unsigned long dly) {
  while (dly--) {
    wdt_reset(); //  reset watchdog
    delay(1);
  }
}
