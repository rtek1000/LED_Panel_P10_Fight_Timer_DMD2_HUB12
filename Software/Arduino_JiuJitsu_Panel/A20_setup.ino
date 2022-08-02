void setup(void) {
  pinMode(PanelOnOffPin, OUTPUT);
  digitalWrite(PanelOnOffPin, HIGH);
  
  delayWdt(500);

  wdt_enable(WDTO_4S);

  inputString.reserve(20);

  // initialize serial:
  //  Serial.begin(115200);
  Serial.begin(9600);

  Serial.println();
  Serial.println(F("LED Panel P10 Fight Timer DMD2 HUB12"));
  Serial.println(F("by Rtek1000 (2018-2022)"));
  Serial.println(F("(ref.: GitHub Rtek1000)"));
  Serial.println();

  //  Serial.println(F("SD card tree:"));
  //  Serial.println(F("01 Folder: 001 - 099_number_voice_file.mp3"));
  //  Serial.println(F("02 Folder: 001 - 099_music_file.mp3"));
  //  Serial.println(F("03 Folder: 002 - 012_music_file.mp3"));
  //  Serial.println(F("04 Folder: 001 - 020_music_file.mp3"));
  //  Serial.println(F("05 Folder: 001 - 059_music_file.mp3"));
  //  Serial.println(F("ADVERT Folder: 001 - 010_audio_file.mp3"));
  //  Serial.println(F("mp3 Folder: 0001 - 0172_audio_file.mp3"));

  //  softSerial.begin(9600);

  // DFPlayer1.begin(softSerial);
  DFPlayer1.begin(Serial);

  //  dfReset();
  DFPlayer1.reset();

  DFPlayer1.volume(30);

  //  DFPlayer1.playMp3Folder(12);

  //  DFPlayer1.playMp3Folder(131);
  //
  //  delayWdt(2000);
  //
  //  DFPlayer1.playMp3Folder(130);

  //delayWdt(300);

  pinMode(SQW_interruptPin, INPUT);
  //attachInterrupt(digitalPinToInterrupt(SQW_interruptPin), blink_sqw, CHANGE);

  digitalWrite(input_ctrl_left_pin, HIGH);
  digitalWrite(input_ctrl_cent_pin, HIGH);
  digitalWrite(input_ctrl_right_pin, HIGH);

  //  pinMode(Bell, INPUT_PULLUP); //OUTPUT

  pinMode(input_ctrl_left_pin, INPUT_PULLUP);
  pinMode(input_ctrl_cent_pin, INPUT_PULLUP);
  pinMode(input_ctrl_right_pin, INPUT_PULLUP);

  //  digitalWrite(Bell, LOW);

  //  pinMode(dfPlayerStatus, OUTPUT);
  //  digitalWrite(dfPlayerStatus, LOW);

  digitalWrite(PanelOnOffPin, LOW); // Panel ON

  delayWdt(100);

  //seed_rand();

  dmd.begin();

  dmd.clearScreen();

  dmd.selectFont(SystemFont5x7);

  currentMillis = millis();

  get_data_from_eeprom();

  dmd.setBrightness(5);

  dmd.drawFilledBox(0, 0, 31, 15);

  delayWdt(2000);

  dmd.clearScreen();

  if ((show_sel_time_date_bright * 5) > 254) {
    dmd.setBrightness(254);
  } else {
    dmd.setBrightness(show_sel_time_date_bright * 5);
  }

#ifndef skip_init_msg
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

  dmd.drawString(2, line1, "Brt:");
  dmd.drawString(2, line2, text1);

  delayWdt(2000);

  // text_scroll(F("Fight_Timer_DMD_HUB12"));

  //  dmd.clearScreen();
  //
  //  dmd.clearScreen();
  //
  //  text1 = "Let's go champions!";
  //
  //  text_scroll(text1);

  dmd.clearScreen();
#endif

  dmd.stop();

  if (! rtc.begin())
  {
    dmd.resume();
    // Serial.println("Couldn't find RTC");
    //text_scroll(F("FAIL1: RTC DS3231 NOT FOUND"));

    dmd.drawString(2, line1, "RTC  ");
    dmd.drawString(2, line2, "FAIL ");

    delayWdt(2000);

    dmd.drawString(2, line1, "Check");
    dmd.drawString(2, line2, "Batt ");

    delayWdt(2000);
  }

  dmd.resume();

  get_rtc();

#ifndef skip_LM35
  temperature_init();
#endif

#ifndef skip_init_msg

  dmd.clearScreen();

  show_clock_and_temperature();

  delayWdt(2000);
  dmd.clearScreen();

  show_rtc_temp();
  delayWdt(2000);
  dmd.clearScreen();

  show_date();

  delayWdt(2000);

  dmd.clearScreen();

  for (int i = 0; i < 5; i++) {
    dmd.drawString(2, 4, "FIGHT");
    delayWdt(150);

    dmd.clearScreen();
    delayWdt(150);
  }

  show_round_and_time();

  bell_start();

  delayWdt(1000);

#endif
}
