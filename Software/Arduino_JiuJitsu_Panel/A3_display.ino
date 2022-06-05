
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

void show_round_and_time(void) {
  show_round();
  show_time_round();
}
void show_round(void) {
  //dmd.drawString(2, 0, " :" + String(round_cnt10) + String(round_cnt1));
  dmd.drawString(14, 0, ":" + String(round_cnt10) + String(round_cnt1));
}
void show_time_round(void) {
  if (time_sel == func_normal) {
    dmd.drawString(2, 0, "Ft"); // "N"
  } else if (time_sel == func_interval) {
    dmd.drawString(2, 0, "In");
  }

  dmd.drawString(2, line2,
                 String(L10) + String(L1) + ":" + String(R10) + String(R1));
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
  if (((L10 != 0) | (L1 != 0) | (R10 != 0) | (R1 != 0))
      & (pause_state == false)) {
    if ((bell_ding_mode_val == true) & (state == func_interval)) {
      if (digitalRead(dfPlayerStatus)) { // dfPlayer off?
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
    delayWdt(50);
    show_round_and_time();
    delayWdt(500);

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
    delayWdt(500);

    bell_start();
  }
}

void blink_preset(void) {
  blink_string1 = "_";

  if ((state == func_interval) | (state == func_menu_bell_ding_mode) | (state == func_menu_bell_ding_mode_intv)) {
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
  } else if (state == func_menu_set_year1) {
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
    } else if (state == func_menu_bell_ding_mode_intv) {
      show_bell_state_intv();
    } else if ((state == func_menu_set_round_recall_normal_min10)
               | (state == func_menu_set_round_recall_normal_min1)
               | (state == func_menu_set_round_recall_normal_sec10)
               | (state == func_menu_set_round_recall_normal_sec1)) {
      dmd.drawString(2, line1, "Fight");

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
  dmd.drawString(2, line1, "N. Ft");

  switch (bell_ding_mode_val) {
    case 0:
      dmd.drawString(2, line2, "Beep ");
      break;
    case 1:
      dmd.drawString(2, line2, "Music");
      break;
    case 2:
      dmd.drawString(2, line2, "Voice");
      break;
    case 3:
      dmd.drawString(2, line2, "B+M  ");
      break;
    case 4:
      dmd.drawString(2, line2, "B+V  ");
      break;
    case 5:
      dmd.drawString(2, line2, "M+V  ");
      break;
    case 6:
      dmd.drawString(2, line2, "B+M+V");
      break;
    case 7:
      dmd.drawString(2, line2, "Off  ");
      break;
    default:
      // comando(s)
      break;
  }
}

void show_bell_state_intv(void) {
  dmd.drawString(2, line1, "N. In");

  switch (bell_ding_mode_val_intv) {
    case 0:
      dmd.drawString(2, line2, "Beep ");
      break;
    case 1:
      dmd.drawString(2, line2, "Music");
      break;
    case 2:
      dmd.drawString(2, line2, "Voice");
      break;
    case 3:
      dmd.drawString(2, line2, "B+M  ");
      break;
    case 4:
      dmd.drawString(2, line2, "B+V  ");
      break;
    case 5:
      dmd.drawString(2, line2, "M+V  ");
      break;
    case 6:
      dmd.drawString(2, line2, "B+M+V");
      break;
    case 7:
      dmd.drawString(2, line2, "Off  ");
      break;
    default:
      // comando(s)
      break;
  }
}

void show(void) {
  if (state == func_clock) {
    if (clock_show_cnt < 19) {
      clock_show_cnt++;
    } else {
      clock_show_cnt = 0;

      state = func_date;
      dmd.drawString(0, line1, "      ");
      dmd.drawString(0, line2, "      ");
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

      dmd.drawString(0, line1, "      ");
      dmd.drawString(0, line2, "      ");
      show_clock_and_temperature();
    }
  }
}

void show_clock_and_temperature(void) {
  dmd.clearScreen();
  show_clock();
  show_temperature();
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

  dmd.drawString(2, line1, day1 + "/");
  dmd.drawString(19, line1, month1);
  dmd.drawString(4, line2, year1);
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

  dmd.drawString(2, line1, day1 + "/");
  dmd.drawString(19, line1, month1);
  dmd.drawString(4, line2, year1);
}

void show_clock(void) {
  String hour1;
  String min1;

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

  dmd.drawString(2, line1, hour1);
  dmd.drawString(14, line1, ":");
  dmd.drawString(19, line1, min1);
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

  dmd.drawString(2, line2, hour1);
  dmd.drawString(14, line2, ":");
  dmd.drawString(19, line2, min1);
  dmd.drawString(2, line1, "b:" + String(show_sel_time_date_bright * 5));
}

void show_temperature(void) {
  String temp_string;
  String temp_dec_string;
  bool neg_sig = false;

  //temperature = 111.9;

  if (temperature < 0) {
    temperature *= -1;

    neg_sig = true;
  }

  temp_dec_string = String(int((temperature * 10) - int(temperature) * 10));

  if ((neg_sig == true) && (temperature == 127)) { // DEVICE_DISCONNECTED_C -127
    temp_string = "--";
    temp_dec_string = "-";
  } else {
    temp_string = String(int(temperature));

    if ((temperature < 10) && (neg_sig == false)) {
      temp_string = ' ' + temp_string;
    }

    //    if (temperature < 10) {
    //      x_add = -3;
    //    }
  }

  if (neg_sig == false) {
    dmd.drawString(2, 9, temp_string);
    if (temperature < 100) {
      dmd.drawString(14, 9, ".");
      dmd.drawString(19, 9, temp_dec_string);
    }
  } else {
    if (temperature == 127) {  // DEVICE_DISCONNECTED_C -127
      dmd.drawString(2, 9, temp_string);
      dmd.drawString(14, 9, ".");
      dmd.drawString(19, 9, temp_dec_string);
    } else {
      dmd.drawString(2, 9, "-");
      dmd.drawString(8, 9, temp_string);
      if (temperature < 10) {
        dmd.drawString(14, 9, ".");
        dmd.drawString(19, 9, temp_dec_string);
      }
    }
  }

  dmd.drawLine(25, 10, 25, 11); //°
  dmd.drawLine(26, 9, 27, 9);
  dmd.drawLine(28, 10, 28, 11);
  dmd.drawLine(26, 12, 27, 12);
}
