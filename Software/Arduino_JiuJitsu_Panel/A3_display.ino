
//void text_scroll(String MESSAGE) {
//  //String MESSAGE = "Menu de ajustes";
//  //String MESSAGE = "Settings menu";
//  MESSAGE = "     " + MESSAGE;
//  MESSAGE = MESSAGE + "     ";
//  byte next = MESSAGE.length() + 1;
//
//  dmd.clearScreen();
//
//  dmd.selectFont(SystemFont5x7);
//
//  while (next--) {
//    //Serial.print(MESSAGE[MESSAGE.length() - next]);
//    //box.print(MESSAGE[MESSAGE.length() - next]);
//    dmd.drawString(dig1, 4, MESSAGE.substring(MESSAGE.length() - next));
//    delayWdt(200);
//    //next++;
//  }
//
//  delayWdt(500);
//  dmd.clearScreen();
//  dmd.selectFont(SystemFont5x7);
//}

void show_round_and_time(void) {
  show_round();
  show_time_round();
}
void show_round(void) {
  //dmd.drawString(dig1, line1, " :" + String(round_cnt10) + String(round_cnt1));
  dmd.drawString(dig3, line1, ":" + String(round_cnt10) + String(round_cnt1));
}
void show_time_round(void) {
  if (time_sel == func_normal) {
    dmd.drawString(dig1, line1, "Rd"); // "N"
  } else if (time_sel == func_interval) {
    dmd.drawString(dig1, line1, "In");
  }

  dmd.drawString(dig1, line2,
                 String(L10) + String(L1) + ":" + String(R10) + String(R1));
}

void show_recal_round_normal(void) {
  dmd.drawString(dig1, line2,
                 String(round_recall_min10) + String(round_recall_min1) + ":"
                 + String(round_recall_sec10) + String(round_recall_sec1));
}

void show_recal_round_interval(void) {
  dmd.drawString(dig1, line2,
                 String(interval_recall_min10) + String(interval_recall_min1) + ":"
                 + String(interval_recall_sec10)
                 + String(interval_recall_sec1));
}

void decrement_time_and_show(void) {
  if (rtc_temp > 45) { // rtc_temp, temperature
    if ((millis() - millis_high_temp) >= 30000) { // 30000
      millis_high_temp = millis();

      for (int i = 0; i < 3; i++) {
        dmd.drawString(dig1, line1, "HIGH ");
        dmd.drawString(dig1, line2, " TEMP");

        delayWdt(200);
        dmd.drawString(dig1, line1, "     ");
        dmd.drawString(dig1, line2, "     ");

        delayWdt(200);
      }

      dmd.drawString(dig1, line1, "T.int");
      dmd.drawString(dig1, line2, ">45");
      draw_degree(dig4, line2);

      delayWdt(2000);

      dmd.drawString(dig1, line1, "Batt.");
      dmd.drawString(dig1, line2, ">Risk");
      draw_degree(dig4, line2);

      delayWdt(2000);
    }
  }

  if (((L10 != 0) | (L1 != 0) | (R10 != 0) | (R1 != 0))
      & (pause_state == false)) {
    //    if ((bell_ding_mode_val == true) & (state == func_interval)) {
    //      if (digitalRead(dfPlayerStatus)) { // dfPlayer off?
    //        musicRandStart();
    //      }
    //    }

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

          dmd.clearScreen();

          delayWdt(3);

          get_rtc(); // Read RTC temperature to protect the bank power battery
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
    show_round_and_time();
    delayWdt(1000);
    state = func_interval;

    time_sel = state;
    L10 = interval_recall_min10;
    L1 = interval_recall_min1;
    R10 = interval_recall_sec10;
    R1 = interval_recall_sec1;

    blink_state = true;
    blink_alternate = false;

    dmd.clearScreen();
    delayWdt(5);
    get_rtc(); // Read RTC temperature to protect the bank power battery
    millis_temp = millis();
    delayWdt(50);
    show_round_and_time();
    dmd.drawString(dig3, line1, ".");
    // delayWdt(500);

    bell_stop();

  } else if ((L10 == 0) & (L1 == 0) & (R10 == 0) & (R1 == 0)
             & (state == func_interval)) {
    show_round_and_time();
    delayWdt(1000);
    state = func_normal;

    time_sel = state;
    L10 = round_recall_min10;
    L1 = round_recall_min1;
    R10 = round_recall_sec10;
    R1 = round_recall_sec1;

    blink_state = false;
    dmd.clearScreen();
    get_rtc(); // Read RTC temperature to protect the bank power battery
    millis_temp = millis();
    delayWdt(50);

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

    show_round_and_time();
    dmd.drawString(dig3, line1, ".");
    // delayWdt(500);

    bell_start();
  }
}

void blink_preset(void) {
  blink_string1 = "_";

  switch (state) {
    case func_interval:
    case func_menu_bell_ding_mode:
    case func_menu_bell_ding_mode_intv:
      blink_position_X = 0;
      blink_position_Y = line2;
      blink_string1 = "      ";
      break;
    case func_menu_set_round10:
      blink_position_X = dig4;
      blink_position_Y = line1;
      break;
    case func_menu_set_round1:
      blink_position_X = dig5;
      blink_position_Y = line1;
      break;
    case func_menu_set_round_time_mode:
      blink_position_X = dig1; // 26
      blink_position_Y = line1;
      blink_string1 = "  ";
      break;
    case func_menu_set_round_time_min10:
    case func_menu_set_round_recall_normal_min10:
    case func_menu_set_round_recall_interval_min10:
      blink_position_X = dig1;
      blink_position_Y = line2;
      break;
    case func_menu_set_round_time_min1:
    case func_menu_set_round_recall_normal_min1:
    case func_menu_set_round_recall_interval_min1:
      blink_position_X = dig2;
      blink_position_Y = line2;
      break;
    case func_menu_set_round_time_sec10:
    case func_menu_set_round_recall_normal_sec10:
    case func_menu_set_round_recall_interval_sec10:
      blink_position_X = dig4;
      blink_position_Y = line2;
      break;
    case func_menu_set_round_time_sec1:
    case func_menu_set_round_recall_normal_sec1:
    case func_menu_set_round_recall_interval_sec1:
      blink_position_X = dig5;
      blink_position_Y = line2;
      break;
    case func_menu_set_hour1:
      blink_position_X = dig2;
      blink_position_Y = line2;
      break;
    case func_menu_set_hour10:
      blink_position_X = dig1;
      blink_position_Y = line2;
      break;
    case func_menu_set_min1:
      blink_position_X = dig5;
      blink_position_Y = line2;
      break;
    case func_menu_set_min10:
      blink_position_X = dig4;
      blink_position_Y = line2;
      break;
    case func_menu_set_day1:
      blink_position_X = dig2;
      blink_position_Y = line1;
      break;
    case func_menu_set_day10:
      blink_position_X = dig1;
      blink_position_Y = line1;
      break;
    case func_menu_set_month1:
      blink_position_X = dig5;
      blink_position_Y = line1;
      break;
    case func_menu_set_month10:
      blink_position_X = dig4;
      blink_position_Y = line1;
      break;
    case func_menu_set_time_date_bright:
      blink_position_X = dig3;
      blink_position_Y = line1;
      blink_string1 = "   ";
      break;
    case func_menu_set_year1:
      blink_position_X = dig4 + 2;
      blink_position_Y = line2;
      break;
    case func_menu_set_year10:
      blink_position_X = dig3 + 2;
      blink_position_Y = line2;
      break;
    case func_menu_set_volume_beep:
    case func_menu_set_volume_voice:
      blink_position_X = dig4;
      blink_position_Y = line2;
      blink_string1 = "   ";
      break;
    default:
      // statements
      break;
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
    } else if (state == func_menu_bell_ding_mode_intv) {
      show_bell_state_intv();
    } else if ((state == func_menu_set_round_recall_normal_min10)
               | (state == func_menu_set_round_recall_normal_min1)
               | (state == func_menu_set_round_recall_normal_sec10)
               | (state == func_menu_set_round_recall_normal_sec1)) {
      dmd.drawString(dig1, line1, "Round");

      show_recal_round_normal();
    } else if ((state == func_menu_set_round_recall_interval_min10)
               | (state == func_menu_set_round_recall_interval_min1)
               | (state == func_menu_set_round_recall_interval_sec10)
               | (state == func_menu_set_round_recall_interval_sec1)) {
      dmd.drawString(dig1, line1, "Inter");
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
  dmd.drawString(dig1, line1, "Nt Rd"); // Notifications for Round

  switch (bell_ding_mode_val) {
    case audio_off:
      dmd.drawString(dig1, line2, "Off  ");
      break;
    case audio_beep:
      dmd.drawString(dig1, line2, "Beep ");
      break;
    case audio_voice:
      dmd.drawString(dig1, line2, "Voice");
      break;
    case audio_beep_voice:
      dmd.drawString(dig1, line2, "B+V  ");
      break;
    case audio_music:
      dmd.drawString(dig1, line2, "Music");
      break;
    case audio_beep_music:
      dmd.drawString(dig1, line2, "B+M  ");
      break;
    case audio_music_voice:
      dmd.drawString(dig1, line2, "M+V  ");
      break;
    case audio_beep_music_voice:
      dmd.drawString(dig1, line2, "B+M+V");
    default:
      //
      break;
  }
}

void show_bell_state_intv(void) {
  dmd.drawString(dig1, line1, "Nt In"); // Notifications for Interval

  switch (bell_ding_mode_val_intv) {
    case audio_off:
      dmd.drawString(dig1, line2, "Off  ");
      break;
    case audio_beep:
      dmd.drawString(dig1, line2, "Beep ");
      break;
    case audio_voice:
      dmd.drawString(dig1, line2, "Voice");
      break;
    case audio_beep_voice:
      dmd.drawString(dig1, line2, "B+V  ");
      break;
    case audio_music:
      dmd.drawString(dig1, line2, "Music");
      break;
    case audio_beep_music:
      dmd.drawString(dig1, line2, "B+M  ");
      break;
    case audio_music_voice:
      dmd.drawString(dig1, line2, "M+V  ");
      break;
    case audio_beep_music_voice:
      dmd.drawString(dig1, line2, "B+M+V");
      break;
    default:
      //
      break;
  }
}

void show_date_time(void) {
  if (state == func_clock) {
    if (clock_show_cnt < 19) {
      clock_show_cnt++;
    } else {
      clock_show_cnt = 0;

      state = func_date;

      dmd.clearScreen();

      show_rtc_temp();
      delayWdt(2500);

      dmd.clearScreen();

      show_date();
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

      dmd.clearScreen();
      show_clock_and_temperature();
    }
  }
}

void show_clock_and_temperature(void) {
  dmd.clearScreen();
  show_clock();

  show_temperature(temperature);

  if (temperature > 45.0) {
    for (int i = 0; i < 5; i++) {
      dmd.drawString(0, line2, "      ");
      delayWdt(200);
      show_temperature(temperature);
      delayWdt(200);
    }
  }
}

void show_date(void) {
  String year1;
  String month1;
  String day1;

  get_rtc();

  wr_rtc_HH = now.hour();
  wr_rtc_MM = now.minute();

  wr_rtc_yy = now.year();
  wr_rtc_mm = now.month();
  wr_rtc_dd = now.day();

  day1 = String(wr_rtc_dd);

  if (wr_rtc_dd < 10) {
    day1 = "0" + String(wr_rtc_dd);
  }

  if (wr_rtc_dd > 31) {
    day1 = "--";
  }

  month1 = String(wr_rtc_mm);

  if (wr_rtc_mm < 10) {
    month1 = "0" + String(wr_rtc_mm);
  }

  if (wr_rtc_mm > 31) {
    month1 = "--";
  }

  year1 = String(wr_rtc_yy);

  if (wr_rtc_yy > 2099) {
    year1 = "----";
  }

  dmd.drawString(dig1, line1, day1 + "/");
  dmd.drawString(dig4, line1, month1);
  dmd.drawString(dig1 + 2, line2, year1);
}

void show_date2(void) {
  String year1;
  String month1;
  String day1;

  get_rtc();

  wr_rtc_HH = now.hour();
  wr_rtc_MM = now.minute();

  wr_rtc_yy = now.year();
  wr_rtc_mm = now.month();
  wr_rtc_dd = now.day();

  day1 = String(wr_rtc_dd);

  if (wr_rtc_dd < 10) {
    day1 = "0" + String(wr_rtc_dd);
  }

  if (wr_rtc_dd > 31) {
    day1 = "--";
  }

  month1 = String(wr_rtc_mm);

  if (wr_rtc_mm < 10) {
    month1 = "0" + String(wr_rtc_mm);
  }

  if (wr_rtc_mm > 31) {
    month1 = "--";
  }

  year1 = String(wr_rtc_yy);

  if (wr_rtc_yy > 2099) {
    year1 = "----";
  }

  dmd.drawString(dig1, line1, day1 + "/");
  dmd.drawString(dig4, line1, month1);
  dmd.drawString(dig1 + 2, line2, year1);
}

void show_clock(void) {
  String hour1 = "";
  String min1 = "";

  get_rtc();

  wr_rtc_HH = now.hour();
  wr_rtc_MM = now.minute();

  wr_rtc_yy = now.year();
  wr_rtc_mm = now.month();
  wr_rtc_dd = now.day();

  iHour = wr_rtc_HH;
  iMin = wr_rtc_MM;

  hour1 = String(wr_rtc_HH);

  if (wr_rtc_HH < 10) {
    hour1 = "0" + String(wr_rtc_HH);
  }

  if (wr_rtc_HH > 23) {
    hour1 = "--";
  }

  min1 = String(wr_rtc_MM);

  if (wr_rtc_MM < 10) {
    min1 = "0" + String(wr_rtc_MM);
  }

  if (wr_rtc_MM > 59) {
    min1 = "--";
  }

  dmd.drawString(dig1, line1, hour1);
  dmd.drawString(dig3, line1, "h");
  dmd.drawString(dig4, line1, min1);
}

void show_clock2(void) {
  String hour1;
  String min1;

  get_rtc();

  wr_rtc_HH = now.hour();
  wr_rtc_MM = now.minute();

  wr_rtc_yy = now.year();
  wr_rtc_mm = now.month();
  wr_rtc_dd = now.day();

  hour1 = String(wr_rtc_HH);

  if (wr_rtc_HH < 10) {
    hour1 = "0" + String(wr_rtc_HH);
  }

  if (wr_rtc_HH > 23) {
    hour1 = "--";
  }

  min1 = String(wr_rtc_MM);

  if (wr_rtc_MM < 10) {
    min1 = "0" + String(wr_rtc_MM);
  }

  if (wr_rtc_MM > 59) {
    min1 = "--";
  }

  dmd.drawString(dig1, line2, hour1);
  dmd.drawString(dig3, line2, "h");
  dmd.drawString(dig4, line2, min1);

  dmd.drawString(dig1, line1, "b:" + String(show_sel_time_date_bright * 5));
}

void show_rtc_temp(void) {
  dmd.drawString(dig1, line1, "T.int");
  show_temperature(rtc_temp);

  if (rtc_temp > 45.0) {
    for (int i = 0; i < 5; i++) {
      dmd.drawString(0, line2, "      ");
      delayWdt(200);
      show_temperature(rtc_temp);
      delayWdt(200);
    }
  }
}

void show_temperature(int temper) {
  String temp_string = "";
  bool neg_sig = false;

  if (temper < 0) {
    temper *= -1;

    neg_sig = true;
  }

  temp_string = String(temper);

  if (neg_sig == false) {
    if (temper < 100) {
      if (temper < 10) {
        dmd.drawString(dig2, line2, temp_string);
        draw_degree(dig3, line2);
      } else {
        dmd.drawString(dig1 + 3, line2, temp_string);
        draw_degree(dig3 + 3, line2);
      }
    } else {
      dmd.drawString(dig1, line2, temp_string);
      draw_degree(dig4, line2);
    }
  } else {
    if (temper < 100) { // LM35 neg. max.: -55°C
      if (temper < 10) {
        dmd.drawString(dig1 + 3, line2, "-");
        dmd.drawString(dig2 + 3, line2, temp_string);
        draw_degree(dig3 + 3, line2);
      } else {
        dmd.drawString(dig1, line2, "-");
        dmd.drawString(dig2, line2, temp_string);
        draw_degree(dig4, line2);
      }
    }
  }
}

void draw_degree(int dig, int line) {
  dmd.drawLine(0 + dig, 1 + line, 0 + dig, 2 + line); //°
  dmd.drawLine(1 + dig, 0 + line, 2 + dig, 0 + line);
  dmd.drawLine(3 + dig, 1 + line, 3 + dig, 2 + line);
  dmd.drawLine(1 + dig, 3 + line, 2 + dig, 3 + line);

  dmd.drawString(dig + 6, line2, "C");
}
