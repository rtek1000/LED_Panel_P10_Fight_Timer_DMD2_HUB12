void setup(void) {
  delayWdt(300);

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

  byte loop_setup;

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

  delayWdt(100);

  //  dfPlayFolder(2, random(1, 100)); // musica aleatória

  int timeout = 0;

  //  while (!digitalRead(dfPlayerStatus)) {
  //    // dfStop1();
  //    DFPlayer1.stop();
  //
  //    delayWdt(100);
  //
  //    timeout++;
  //
  //    if (timeout > 100)
  //      break;
  //  }

  // musicRandStart();
  //
  //  dfPlayFolder(2, 4); // folder, file

  //pinMode(13, OUTPUT);

  //initialize TimerOne's interrupt/CPU usage used to scan and refresh the display
  //  Timer1.initialize( 5000 );           //period in microseconds to call ScanDMD. Anything longer than 5000 (5ms) and you can see flicker.
  //  Timer1.attachInterrupt( ScanDMD );   //attach the Timer1 interrupt to ScanDMD which goes to dmd.scanDisplayBySPI()

  //seed_rand();

  dmd.begin();

  dmd.clearScreen();

  dmd.selectFont(SystemFont5x7);

  currentMillis = millis();

  get_data_from_eeprom();

  if (show_sel_time_date_bright < 1)
  {
    show_sel_time_date_bright = 1;
  }

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

  // dfVolume(music_volume);

  //  while (1) {

  //dfPlayFolder(2, random(1, 99)); // random music

  // dfVolume(music_volume);

  //    delayWdt(100);

  //    waitDfPlayer(60);

  //  }

  delayWdt(2000);

  //  DFPlayer1.volume(music_volume);

  delayWdt(100);

  //  DFPlayer1.volume(15);

  uint32_t millis_test = 0;

  //  while (1) {
  //    if (millis() > (millis_test + 500)) {
  //      millis_test = millis();
  //
  //      Serial.print("DFPlayer1.readVolume(): ");
  //      Serial.println(DFPlayer1.readVolume());
  //    }
  //
  //    if (DFPlayer1.available()) {
  //      //printDetail(DFPlayer1.readType(), DFPlayer1.read()); //Print the detail message from DFPlayer to handle different errors and states.
  //    }
  //
  //    wdt_reset(); //  reset watchdog
  //  }

  //  musicRandStart();

  dmd.clearScreen();

  //  loop_setup = 1;
  //
  //  while (loop_setup < (show_sel_time_date_bright)) {
  //    dmd.setBrightness(loop_setup * 5);
  //    loop_setup++;
  //    dmd.drawString(2, 4, String(loop_setup * 5));
  //    delayWdt(200);
  //  }
  //
  //  dmd.setBrightness(50);

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

  // Timer1.stop();

  if (! rtc.begin())
  {
    // Serial.println("Couldn't find RTC");
    //text_scroll(F("FAIL1: RTC DS3231 NOT FOUND"));

    dmd.drawString(2, line1, "RTC");
    dmd.drawString(2, line2, "FAIL");

    delayWdt(2000);

    dmd.drawString(2, line1, "Check");
    dmd.drawString(2, line2, "Batt");

    delayWdt(2000);
  }

  dmd.stop();

  Ds3231SqwPinMode mode = rtc.readSqwPinMode();
  if ( mode != DS3231_SquareWave1Hz ) {
    rtc.writeSqwPinMode( DS3231_SquareWave1Hz );
  }

  dmd.resume();

  //  Timer1.resume();

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
  // analogReference(INTERNAL); // arduino uno/mini/nano

#ifndef skip_LM35
  temperature_init();
#endif

#ifndef skip_init_msg

  dmd.clearScreen();

  show_clock_and_temperature();

  delayWdt(2000);
  dmd.clearScreen();

  show_date();

  delayWdt(2000);

  dmd.clearScreen();
  //  dmd.clearScreen();

  //  loop_setup = 3;
  //
  for (int i = 0; i < 5; i++) {
    dmd.drawString(2, 4, "FIGHT");
    delayWdt(150);

    dmd.clearScreen();
    delayWdt(150);
  }

  // DFPlayer1.playFolder(14, 12);

  show_round_and_time();

  bell_start();

  // dfStop1();
  //  DFPlayer1.stop();

  delayWdt(1000);

  //dmd.setBrightness(show_sel_time_date_bright * 5);

  //wdt_reset();  //  reset watchdog
  //wdt_enable(WDTO_8S);  // watchdog

  //  bell_start();

#endif
}
