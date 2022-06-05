/*
   Maximum capacity: 32GB
   Supported disk formats: FAT16 or FAT32
   Maximum number of supported folders: 100
   Maximum number of audio files per folder: 255 (except for mp3 folder)

   https://www.tinytronics.nl/shop/en/audio/audio-sources/dfrobot-dfplayer-mini-mp3-module
*/

void bell_stop(void) {
  dfStop1();

  delayWdt(1000);

  if (pause_state == true) {
    dfVolume(beep_volume);

    delayWdt(100);

    dfPlayMP3(11); // beep

    delayWdt(100);

    waitDfPlayer(10);
  } else {
    if ((bell_ding_mode_val == 0) || (bell_ding_mode_val == 3) || (bell_ding_mode_val == 4) || (bell_ding_mode_val == 6)) {
      //Bell_ding();
      dfVolume(beep_volume);

      delayWdt(100);

      for (int i = 0; i < 2; i++) {
        dfPlayMP3(11); // beep

        delayWdt(100);

        waitDfPlayer(10);
      }
    }

    if ((bell_ding_mode_val == 2) || (bell_ding_mode_val == 4) || (bell_ding_mode_val == 5) || (bell_ding_mode_val == 6)) {

      dfVolume(voice_volume);

      delayWdt(50);

      dfPlayFolder(1, 246); // interval

      delayWdt(50);

      waitDfPlayer(10);

      if ((bell_ding_mode_val == 1) || (bell_ding_mode_val == 3) || (bell_ding_mode_val == 5) || (bell_ding_mode_val == 6)) {
        dfVolume(music_volume); // music_volume
        delayWdt(100);

        musicRandStart();
      }
    }
  }
}

void bell_start(void) {
  dfStop1();

  delayWdt(1000);

  if (pause_state == true) {
    dfVolume(beep_volume);

    delayWdt(100);

    dfPlayMP3(11); // beep

    delayWdt(100);

    waitDfPlayer(10);
  } else {
    if ((bell_ding_mode_val == 0) || (bell_ding_mode_val == 3) || (bell_ding_mode_val == 4) || (bell_ding_mode_val == 6)) {
      //Bell_ding();

      //Bell_ding();

      //    delayWdt(300);

      dfVolume(beep_volume);

      delayWdt(100);

      for (int i = 0; i < 2; i++) {
        //      beep_volume = EEPROM.read(beep_volume_addr);
        //      music_volume = EEPROM.read(music_volume_addr);
        //      voice_volume = EEPROM.read(voice_volume_addr);

        dfPlayMP3(11); // beep

        delayWdt(100);

        waitDfPlayer(10);
      }

    }

    if ((bell_ding_mode_val == 2) || (bell_ding_mode_val == 4) || (bell_ding_mode_val == 5) || (bell_ding_mode_val == 6)) {

      dfVolume(voice_volume);

      delayWdt(100);

      dfVolume(voice_volume);

      delayWdt(100);

      dfPlayFolder(1, 251); // round

      delayWdt(100);

      waitDfPlayer(10);

      dfPlayFolder(1, ((round_cnt10 * 10) + round_cnt1)); // round

      delayWdt(100);

      waitDfPlayer(60);
    }

    //  Serial.print("state: ");
    //  Serial.println(state, DEC);

    if (pause_state == false) {
      if ((bell_ding_mode_val == 1) || (bell_ding_mode_val == 3) || (bell_ding_mode_val == 5) || (bell_ding_mode_val == 6)) {
        dfVolume(music_volume); // music_volume
        delayWdt(100);

        musicRandStart();
      }
    }
  }
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


void musicRandStart(void) {
  for (int i = 0; i < 25; i++) {
    // dfPlayFolder(2, random(1, 99)); // musica aleatória
    //dfPlayMP3(random(1, 173)); // musica aleatória
    int p = random(2, 6);

    int f_min = 1;
    int f_max = 99; // (the maximum quantity is 255 per folder)

    if (p == 2) f_max = 54; // (the maximum quantity is 255 per folder)
    if (p == 3) {
      f_max = 12; // (the maximum quantity is 255 per folder)
      f_min = 2;  // (the maximum quantity is 255 per folder)
    }
    if (p == 4) f_max = 20; // (the maximum quantity is 255 per folder)
    if (p == 5) f_max = 59; // (the maximum quantity is 255 per folder)
    if (p == 6) f_max = 172; // (the maximum quantity is 9999 for mp3 folder)

    int f = random(f_min, f_max);

    Serial.print("Try to play path: ");
    if (p == 6) {
      Serial.print("mp3");
    } else {
      Serial.print(p, DEC);
    }
    Serial.print(" file: ");
    Serial.println(f);

    if (p == 6) {
      dfPlayMP3(f);
    } else {
      dfPlayFolder(p, f); // random music
    }

    delayWdt(100);

    if (waitDfPlayer(1) == true) {
      break;
    }
  }
}

void continuous_play(void) {
  if (state == func_normal) {
    if (pause_state == false) {
      if ((bell_ding_mode_val == 1) || (bell_ding_mode_val == 3) || (bell_ding_mode_val == 5) || (bell_ding_mode_val == 6)) {
        if (digitalRead(dfPlayerStatus) == true) {
          dfVolume(music_volume); // music_volume
          delayWdt(100);

          musicRandStart();
        }
      }
    }
  } else if (state == func_interval) {
    if (pause_state == false) {
      if ((bell_ding_mode_val_intv == 1) || (bell_ding_mode_val_intv == 3) || (bell_ding_mode_val_intv == 5) || (bell_ding_mode_val_intv == 6)) {
        if (digitalRead(dfPlayerStatus) == true) {
          dfVolume(music_volume); // music_volume
          delayWdt(100);

          musicRandStart();
        }
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
    dmd.stop();
    softSerial.write(m1[i]);
    dmd.resume();

    delayWdt(10);
  }

  delayWdt(10);
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
    dmd.stop();
    softSerial.write(m1[i]);
    dmd.resume();

    delayWdt(10);
  }

  delayWdt(10);
}

void dfVolume(uint8_t volume) {
  // volume 15: 7E FF 06 06 00 00 0F FF D5 EF
  // checksum: 0 - (FF+06+06+00+00+0F)
  uint8_t m1[10] = { 0x7E, 0xFF, 0x06, 0x06, 0x00, 0x00, 0, 0, 0, 0xEF };

  m1[6] = volume;

  doSum(m1, 7);

  for (uint8_t i = 0; i < 10; i++) {
    dmd.stop();
    softSerial.write(m1[i]);
    dmd.resume();

    delayWdt(10);
  }

  delayWdt(10);
}

void dfStop1(void) {
  // 7E FF 06 16 00 00 00 FE E5 EF
  uint8_t m1[10] = { 0x7E, 0xFF, 0x06, 0x16, 0, 0, 0, 0xFE, 0xE5, 0xEF };

  for (uint8_t i = 0; i < 10; i++) {
    dmd.stop();
    softSerial.write(m1[i]);
    dmd.resume();

    delayWdt(10);
  }

  delayWdt(10);
}

void dfReset(void) {
  // 7E FF 06 0C 00 00 00 FE EF EF
  uint8_t m1[10] = { 0x7E, 0xFF, 0x06, 0x0C, 0, 0, 0, 0xFE, 0xEF, 0xEF };

  for (uint8_t i = 0; i < 10; i++) {
    dmd.stop();
    softSerial.write(m1[i]);
    dmd.resume();

    delayWdt(10);
  }

  delayWdt(2000);
}

bool waitDfPlayer(unsigned long timeout_seconds) {
  unsigned long timeout1 = 0;
  unsigned long timeMax = timeout_seconds * 20;
  bool ret_break = false;

  while (digitalRead(dfPlayerStatus)) {
    //wdt_reset();  //  reset watchdog
    delayWdt(50);
    timeout1++;
    if (timeout1 > 50)
      break;
  }

  timeout1 = 0;

  while (!digitalRead(dfPlayerStatus)) {
    //wdt_reset();  //  reset watchdog
    delayWdt(50);
    timeout1++;
    if (timeout1 > timeMax) {
      ret_break = true;
      break;
    }
  }

  return ret_break;
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