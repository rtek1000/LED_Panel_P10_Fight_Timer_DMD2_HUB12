void controller(void) {
  bool state_one = true;
  bool state_zero = false;

  input_ctrl_left = false;
  input_ctrl_cent = false;
  input_ctrl_right = false;

#ifndef input_ctrl_logic_high
  state_one = false;
  state_zero = true;
#endif

  //  if (((digitalRead(input_ctrl_left_pin) == state_one) && (digitalRead(input_ctrl_cent_pin) == state_one)) ||
  //      ((digitalRead(input_ctrl_left_pin) == state_one) && (digitalRead(input_ctrl_right_pin) == state_one)) ||
  //      ((digitalRead(input_ctrl_cent_pin) == state_one) && (digitalRead(input_ctrl_right_pin) == state_one))) {
  //    return;
  //  }

  if ((digitalRead(input_ctrl_left_pin) == state_one) && (input_ctrl_left_pin_old == true)) // HIGH
  {
    input_ctrl_left_pin_old = false;

    input_ctrl_left = true;

  } else if (digitalRead(input_ctrl_left_pin) == state_zero) {
    input_ctrl_left_pin_old = true;
  }

  if ((digitalRead(input_ctrl_cent_pin) == state_one) && (input_ctrl_cent_pin_old == true)) // HIGH
  {
    input_ctrl_cent_pin_old = false;

    input_ctrl_cent = true;
  } else if (digitalRead(input_ctrl_cent_pin) == state_zero) {
    input_ctrl_cent_pin_old = true;
  }

  if ((digitalRead(input_ctrl_right_pin) == state_one) && (input_ctrl_right_pin_old == true)) // HIGH
  {
    input_ctrl_right_pin_old = false;

    input_ctrl_right = true;
  } else if (digitalRead(input_ctrl_right_pin) == state_zero) {
    input_ctrl_right_pin_old = true;
  }

  if (input_ctrl_left_serial == false) { // HIGH
    input_ctrl_left_serial = true;

    input_ctrl_left = true;
  } else if (input_ctrl_cent_serial == false) { // HIGH
    input_ctrl_cent_serial = true;

    input_ctrl_cent = true;
  } else if (input_ctrl_right_serial == false) { // HIGH
    input_ctrl_right_serial = true;

    input_ctrl_right = true;
  }

  if ((input_ctrl_left == true) & (input_ctrl_left_old == false)) {
    //    while ((digitalRead(input_ctrl_left_pin) == HIGH)
    //           & (input_ctrl_left_serial)) {}
    input_ctrl_left_old = true;
    ctrl_left_cmd();
    delayWdt(300);
  } else if ((input_ctrl_left == false) & (input_ctrl_left_old == true)) {
    input_ctrl_left_old = false;
  }

  if ((input_ctrl_cent == true) & (input_ctrl_cent_old == false)) {
    //    while ((digitalRead(input_ctrl_cent_pin) == HIGH)
    //           & (input_ctrl_cent_serial)){}
    input_ctrl_cent_old = true;
    ctrl_cent_cmd();
    delayWdt(300);
  } else if ((input_ctrl_cent == false) & (input_ctrl_cent_old == true)) {
    input_ctrl_cent_old = false;
  }

  if ((input_ctrl_right == true) & (input_ctrl_right_old == false)) {
    //    while ((digitalRead(input_ctrl_right_pin) == HIGH)
    //           & (input_ctrl_right_serial)){}
    input_ctrl_right_old = true;
    ctrl_right_cmd();
    delayWdt(300);
  } else if ((input_ctrl_right == true) & (input_ctrl_right_old == true)) {
    input_ctrl_right_old = false;
  }

  input_ctrl_left = false;
  input_ctrl_cent = false;
  input_ctrl_right = false;
}

//void ctrl_left_cmd(void) {
//  dmd.clearScreen();
//  delayWdt(50);
//
//  if ((state == func_menu_reset_round) | (state == func_menu_set_round)
//      | (state == func_menu_set_round_recall_normal)
//      | (state == func_menu_set_round_recall_interval)
//      | (state == func_menu_set_round_recall_normal_min10)
//      | (state == func_menu_set_round_recall_normal_min1)
//      | (state == func_menu_set_round_recall_normal_sec10)
//      | (state == func_menu_set_round_recall_normal_sec1)
//      | (state == func_menu_set_round_recall_interval_min10)
//      | (state == func_menu_set_round_recall_interval_min1)
//      | (state == func_menu_set_round_recall_interval_sec10)
//      | (state == func_menu_set_round_recall_interval_sec1)
//      | (state == func_menu_set_round10) | (state == func_menu_set_round1)
//      | (state == func_menu_set_round_time_mode)
//      | (state == func_menu_set_round_time_min10)
//      | (state == func_menu_set_round_time_min1)
//      | (state == func_menu_set_round_time_sec10)
//      | (state == func_menu_set_round_time_sec1)
//      | (state == func_menu_bell_ding_mode)
//      | (state == func_menu_set_volume_beep)
//      | (state == func_menu_set_volume_music)
//      | (state == func_menu_set_volume_voice)) {
//    show_menu_timeout = 0;
//    blink_state = false;
//    pause_state = pause_state_previous;
//    state = time_sel;
//    delayWdt(100);
//    show_round_and_time();
//    update_recall();
//  } else if ((state == func_clock) | (state == func_date)) {
//    state = time_sel;
//    if (time_sel == func_interval) {
//      blink_state = true;
//    } else {
//      blink_state = false;
//    }
//    show_round_and_time();
//  } else if ((state == func_normal) | (state == func_interval)) {
//    time_sel = state;
//    blink_state = false;
//    state = func_clock;
//    show_clock_and_temperature();
//  } else if ((state == func_menu_set_hour1) | (state == func_menu_set_hour10)
//             | (state == func_menu_set_min1) | (state == func_menu_set_min10)
//             | (state == func_menu_set_time_date_bright)) { //(state == "menu_set_time_date")
//    show_menu_timeout = 0;
//    clock_show_cnt = 0;
//    state = func_clock;
//
//    blink_state = false;
//    show_clock_and_temperature();
//  } else if ((state == func_menu_set_day1) | (state == func_menu_set_day10)
//             | (state == func_menu_set_month1) | (state == func_menu_set_month10)
//             | (state == func_menu_set_year1)
//             | (state == func_menu_set_year10)) {
//    show_menu_timeout = 0;
//    date_show_cnt = 0;
//    state = func_date;
//
//    blink_state = false;
//    show_date();
//  }
//
//  show_menu_timeout = set_menu_timeout;
//}

void ctrl_left_cmd(void) {
  dmd.clearScreen();
  delayWdt(50);

  switch (state) {
    case func_menu_reset_round:
    
    case func_menu_set_round:    
    case func_menu_set_round10:
    case func_menu_set_round1:

    case func_menu_set_round_time_mode:
    case func_menu_set_round_time_min10:
    case func_menu_set_round_time_min1:
    case func_menu_set_round_time_sec10:
    case func_menu_set_round_time_sec1:

    case func_menu_set_round_recall_normal:
    case func_menu_set_round_recall_normal_min10:
    case func_menu_set_round_recall_normal_min1:
    case func_menu_set_round_recall_normal_sec10:
    case func_menu_set_round_recall_normal_sec1:

    case func_menu_set_round_recall_interval:
    case func_menu_set_round_recall_interval_min10:
    case func_menu_set_round_recall_interval_min1:
    case func_menu_set_round_recall_interval_sec10:
    case func_menu_set_round_recall_interval_sec1:

    case func_menu_bell_ding_mode:
    case func_menu_bell_ding_mode_intv:

    case func_menu_set_volume_beep:
    case func_menu_set_volume_music:
    case func_menu_set_volume_voice:
      show_menu_timeout = 0;
      blink_state = false;
      pause_state = pause_state_previous;
      state = time_sel;
      delayWdt(100);
      show_round_and_time();
      update_recall();
      break;

    case func_clock:
    case func_date:
      state = time_sel;
      if (time_sel == func_interval) {
        blink_state = true;
      } else {
        blink_state = false;
      }
      show_round_and_time();
      break;
    case func_normal:
    case func_interval:
      time_sel = state;
      blink_state = false;
      state = func_clock;
      show_clock_and_temperature();
      break;

    case func_menu_set_hour1:
    case func_menu_set_hour10:
    case func_menu_set_min1:
    case func_menu_set_min10:
    case func_menu_set_time_date_bright:
      show_menu_timeout = 0;
      clock_show_cnt = 0;
      state = func_clock;

      blink_state = false;
      show_clock_and_temperature();
      break;

    case func_menu_set_day1:
    case func_menu_set_day10:
    case func_menu_set_month1:
    case func_menu_set_month10:
    case func_menu_set_year1:
    case func_menu_set_year10:
      show_menu_timeout = 0;
      date_show_cnt = 0;
      state = func_date;

      blink_state = false;
      show_date();
      break;
    default:
      break;
  }

  show_menu_timeout = set_menu_timeout;
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
  //delayWdt(50);

  switch (state) {
    case func_menu_reset_round:
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
      break;
    case func_menu_bell_ding_mode:
      if (bell_ding_mode_val < 7)
        bell_ding_mode_val++;
      else
        bell_ding_mode_val = 0;
      update_recall();
      break;
    case func_menu_bell_ding_mode_intv:
      if (bell_ding_mode_val_intv < 7)
        bell_ding_mode_val_intv++;
      else
        bell_ding_mode_val_intv = 0;
      update_recall();
      break;
    case func_menu_set_round:
      state = func_menu_set_round_time_mode;
      blink_state = true;
      show_round_and_time();
      break;
    case func_menu_set_round_recall_normal:
      state = func_menu_set_round_recall_normal_min10;
      blink_state = true;
      dmd.drawString(2, line1, "Fight");
      show_recal_round_normal();
      break;
    case func_menu_set_round_recall_interval:
      state = func_menu_set_round_recall_interval_min10;
      blink_state = true;
      dmd.drawString(2, line1, "Inter");
      show_recal_round_interval();
      break;
    case func_menu_set_round10:
      if (round_cnt10 < 9)
        round_cnt10++;
      else
        round_cnt10 = 0;
      update_recall();
      break;
    case func_menu_set_round1:
      if (round_cnt1 < 9) {
        round_cnt1++;
      } else {
        if (round_cnt10 == 0)
          round_cnt1 = 1;
        else
          round_cnt1 = 0;
      }
      update_recall();
      break;
    case func_menu_set_round_time_mode:
      if (time_sel == func_normal)
        time_sel = func_interval;
      else
        time_sel = func_normal;
      break;
    default:
      // statements
      break;
  }

  switch (state) {
    case func_menu_set_round_time_min10:
      if (L10 < 9)
        L10++;
      else
        L10 = 0;
      break;
    case func_menu_set_round_time_min1:
      if (L1 < 9)
        L1++;
      else
        L1 = 0;
      break;
    case func_menu_set_round_time_sec10:
      if (R10 < 5)
        R10++;
      else
        R10 = 0;
      break;
    case func_menu_set_round_time_sec1:
      if (R1 < 9)
        R1++;
      else
        R1 = 0;
      break;
    default:
      // statements
      break;
  }

  switch (state) {
    case func_menu_set_round_recall_normal_min10:
      if (round_recall_min10 < 9)
        round_recall_min10++;
      else
        round_recall_min10 = 0;
      update_recall();
      break;
    case func_menu_set_round_recall_normal_min1:
      if (round_recall_min1 < 9)
        round_recall_min1++;
      else
        round_recall_min1 = 0;
      update_recall();
      break;
    case func_menu_set_round_recall_normal_sec10:
      if (round_recall_sec10 < 5)
        round_recall_sec10++;
      else
        round_recall_sec10 = 0;
      update_recall();
      break;
    case func_menu_set_round_recall_normal_sec1:
      if (round_recall_sec1 < 9)
        round_recall_sec1++;
      else
        round_recall_sec1 = 0;
      update_recall();
      break;
    default:
      // statements
      break;
  }

  switch (state) {
    case func_menu_set_round_recall_interval_min10:
      if (interval_recall_min10 < 9)
        interval_recall_min10++;
      else
        interval_recall_min10 = 0;
      update_recall();
      break;
    case func_menu_set_round_recall_interval_min1:
      if (interval_recall_min1 < 9)
        interval_recall_min1++;
      else
        interval_recall_min1 = 0;
      update_recall();
      break;
    case func_menu_set_round_recall_interval_sec10:
      if (interval_recall_sec10 < 5)
        interval_recall_sec10++;
      else
        interval_recall_sec10 = 0;
      update_recall();
      break;
    case func_menu_set_round_recall_interval_sec1:
      if (interval_recall_sec1 < 9)
        interval_recall_sec1++;
      else
        interval_recall_sec1 = 0;
      update_recall();
      break;
    default:
      // statements
      break;
  }

  switch (state) {
    case func_normal:
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
      break;
    case func_interval:
      if (pause_state == true) {
        pause_state = false;
      } else {
        pause_state = true;
      }
      show_round_and_time();
      break;
    case func_clock:
      clock_show_cnt = 0; // clear the counter, for alternating clock and date
      state = func_date;
      dmd.clearScreen();
      show_date();
      break;
    case func_date:
      date_show_cnt = 0; // clear the counter, for alternating clock and date
      state = func_clock;
      dmd.clearScreen();
      show_clock_and_temperature();
      break;
    default:
      // statements
      break;
  }

  switch (state) {
    case func_menu_set_hour1:
      HH_10 = wr_rtc_HH / 10;
      HH_1 = wr_rtc_HH - (HH_10 * 10);
      if (HH_10 == 2) {
        if (HH_1 < 3)
          HH_1++;
        else
          HH_1 = 0;
      } else {
        if (HH_1 < 9)
          HH_1++;
        else
          HH_1 = 0;
      }
      wr_rtc_HH = (HH_10 * 10) + HH_1;
      update_rtc();
      break;
    case func_menu_set_hour10:
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
      if (wr_rtc_HH > 23)
        wr_rtc_HH = 23;
      update_rtc();
      break;
    case func_menu_set_min1:
      MM_10 = wr_rtc_MM / 10;
      MM_1 = wr_rtc_MM - (MM_10 * 10);
      if (MM_1 < 9)
        MM_1++;
      else
        MM_1 = 0;
      wr_rtc_MM = (MM_10 * 10) + MM_1;
      update_rtc();
      break;
    case func_menu_set_min10:
      MM_10 = wr_rtc_MM / 10;
      MM_1 = wr_rtc_MM - (MM_10 * 10);
      if (MM_10 < 5)
        MM_10++;
      else
        MM_10 = 0;
      wr_rtc_MM = (MM_10 * 10) + MM_1;
      if (wr_rtc_MM > 59)
        wr_rtc_MM = 59;
      update_rtc();
      break;
    case func_menu_set_time_date_bright:
      if (show_sel_time_date_bright < 20)
        show_sel_time_date_bright++;
      else
        show_sel_time_date_bright = 1;
      if ((show_sel_time_date_bright * 5) > 254)
        dmd.setBrightness(254);
      else
        dmd.setBrightness(show_sel_time_date_bright * 5);
      update_recall();
      show_clock2();
      break;
    default:
      // statements
      break;
  }

  switch (state) {
    case func_menu_set_day1:
      dd_10 = wr_rtc_dd / 10;
      dd_1 = wr_rtc_dd - (dd_10 * 10);

      mm_10 = wr_rtc_mm / 10;
      mm_1 = wr_rtc_mm - (mm_10 * 10);

      // J f M a M j J A s O  n  D
      // 1 2 3 4 5 6 7 8 9 10 11 12
      if (wr_rtc_mm == 2) {
        if (wr_rtc_dd < 29) {
          if (dd_1 < 9)
            dd_1++;
          else
            dd_1 = 0;
        } else {
          dd_1 = 0;
        }
      } else if ((wr_rtc_mm == 4) | (wr_rtc_mm == 6) | (wr_rtc_mm == 9)
                 | (wr_rtc_mm == 11)) {
        if (wr_rtc_dd < 30) {
          if (dd_1 < 9)
            dd_1++;
          else
            dd_1 = 0;
        } else {
          dd_1 = 0;
        }
      } else {
        if (wr_rtc_dd < 31) {
          if (dd_1 < 9)
            dd_1++;
          else
            dd_1 = 0;
        } else {
          dd_1 = 0;
        }
      }

      wr_rtc_dd = (dd_10 * 10) + dd_1;

      // J f M a M j J A s O  n  D
      // 1 2 3 4 5 6 7 8 9 10 11 12
      if ((wr_rtc_dd > 30)
          & ((wr_rtc_mm == 4) | (wr_rtc_mm == 6) | (wr_rtc_mm == 9)
             | (wr_rtc_mm == 11)))
        wr_rtc_dd = 30;
      else if ((wr_rtc_dd > 29) & (wr_rtc_mm == 2))
        wr_rtc_dd = 29;
      else if (wr_rtc_dd > 31)
        wr_rtc_dd = 31;
      else if (wr_rtc_dd == 0)
        wr_rtc_dd = 1;
      update_rtc();
      break;
    case func_menu_set_day10:
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
             | (wr_rtc_mm == 11)))
        wr_rtc_dd = 30;
      else if ((wr_rtc_dd > 29) & (wr_rtc_mm == 2))
        wr_rtc_dd = 29;
      else if (wr_rtc_dd > 31)
        wr_rtc_dd = 31;
      else if (wr_rtc_dd == 0)
        wr_rtc_dd = 1;
      update_rtc();
      break;
    case func_menu_set_month1:
      mm_10 = wr_rtc_mm / 10;
      mm_1 = wr_rtc_mm - (mm_10 * 10);
      if ((mm_1 < 9) & (wr_rtc_mm < 12))
        mm_1++;
      else
        mm_1 = 0;
      wr_rtc_mm = (mm_10 * 10) + mm_1;
      if (wr_rtc_mm > 12)
        wr_rtc_mm = 12;
      else if (wr_rtc_mm == 0)
        wr_rtc_mm = 1;
      update_rtc();
      break;
    case func_menu_set_month10:
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
      break;
    case func_menu_set_year1:
      yy_10 = (wr_rtc_yy - 2000) / 10;
      yy_1 = (wr_rtc_yy - 2000) - (yy_10 * 10);
      if (yy_1 < 9)
        yy_1++;
      else
        yy_1 = 0;
      wr_rtc_yy = 2000 + ((yy_10 * 10) + yy_1);
      update_rtc();
      break;
    case func_menu_set_year10:
      yy_10 = (wr_rtc_yy - 2000) / 10;
      yy_1 = (wr_rtc_yy - 2000) - (yy_10 * 10);
      if (yy_10 < 9)
        yy_10++;
      else
        yy_10 = 0;
      wr_rtc_yy = 2000 + ((yy_10 * 10) + yy_1);
      update_rtc();
      break;
    default:
      // statements
      break;
  }

  switch (state) {
    case func_menu_set_volume_beep:
      if (beep_volume < 30)
        beep_volume++;
      else
        beep_volume = 1;
      dmd.drawString(2, line1, "Volum");
      dmd.drawString(2, line2, "B: " + String(beep_volume));
      break;
    case func_menu_set_volume_music:
      if (music_volume < 30)
        music_volume++;
      else
        music_volume = 1;
      dmd.drawString(2, line1, "Volum");
      dmd.drawString(2, line2, "M: " + String(music_volume));
      break;
    case func_menu_set_volume_voice:
      if (voice_volume < 30)
        voice_volume++;
      else
        voice_volume = 1;
      dmd.drawString(2, line1, "Volum");
      dmd.drawString(2, line2, "V: " + String(voice_volume));
      break;
    default:
      // statements
      break;
  }

  blink_alternate = true;

  show_menu_timeout = set_menu_timeout;
}

void ctrl_right_cmd(void) {
  dmd.clearScreen();
  //  delayWdt(50);

  switch (state) {
    case func_normal:
    case func_interval:
      pause_state_previous = pause_state;
      pause_state = true;
      state = func_menu_reset_round;
      blink_state = false;
      dmd.drawString(2, line1, "Reset");
      dmd.drawString(2, line2, "Cnt?");
      break;
    case func_menu_reset_round:
      state = func_menu_set_round;
      dmd.drawString(2, line1, "Set");
      dmd.drawString(2, line2, "Cnt?");
      break;
    case func_menu_set_round:
      state = func_menu_set_round_recall_normal;
      dmd.drawString(2, line1, "Set");
      dmd.drawString(2, line2, "Ft?");
      break;
    case func_menu_set_round_recall_normal:
      state = func_menu_set_round_recall_interval;
      dmd.drawString(2, line1, "Set");
      dmd.drawString(2, line2, "Intv?");
      break;
    case func_menu_set_round_recall_interval:
      blink_state = true;
      state = func_menu_bell_ding_mode;
      break;
    case func_menu_bell_ding_mode:
      blink_state = true;
      state = func_menu_bell_ding_mode_intv;
      break;
    case func_menu_bell_ding_mode_intv:
      blink_state = false;
      state = func_menu_set_volume_beep;
      dmd.drawString(2, line1, "Volum");
      dmd.drawString(2, line2, "B: " + String(beep_volume));
      break;
    case func_menu_set_volume_beep:
      blink_state = false;
      state = func_menu_set_volume_music;
      dmd.drawString(2, line1, "Volum");
      dmd.drawString(2, line2, "M: " + String(music_volume));
      break;
    case func_menu_set_volume_music:
      blink_state = false;
      state = func_menu_set_volume_voice;
      dmd.drawString(2, line1, "Volum");
      dmd.drawString(2, line2, "V: " + String(voice_volume));
      break;
    case func_menu_set_volume_voice:
      state = func_menu_reset_round; // retorna
      blink_state = false;
      ctrl_left_cmd(); // exit
      break;
    default:
      //
      break;
  }

  switch (state) {
    case func_menu_set_round_time_mode:
      state = func_menu_set_round10;
      break;
    case func_menu_set_round10:
      state = func_menu_set_round1;
      break;
    case func_menu_set_round1:
      state = func_menu_set_round_time_min10;
      break;
    case func_menu_set_round_time_min10:
      state = func_menu_set_round_time_min1;
      break;
    case func_menu_set_round_time_min1:
      state = func_menu_set_round_time_sec10;
      break;
    case func_menu_set_round_time_sec10:
      state = func_menu_set_round_time_sec1;
      break;
    case func_menu_set_round_time_sec1:
      state = func_menu_set_round_time_mode;
      break;
    default:
      //
      break;
  }

  switch (state) {
    case func_menu_set_round_recall_normal_min10:
      state = func_menu_set_round_recall_normal_min1;
      break;
    case func_menu_set_round_recall_normal_min1:
      state = func_menu_set_round_recall_normal_sec10;
      break;
    case func_menu_set_round_recall_normal_sec10:
      state = func_menu_set_round_recall_normal_sec1;
      break;
    case func_menu_set_round_recall_normal_sec1:
      state = func_menu_set_round_recall_normal_min10;
      break;
      break;
    default:
      //
      break;
  }

  switch (state) {
    case func_menu_set_round_recall_interval_min10:
      state = func_menu_set_round_recall_interval_min1;
      break;
    case func_menu_set_round_recall_interval_min1:
      state = func_menu_set_round_recall_interval_sec10;
      break;
    case func_menu_set_round_recall_interval_sec10:
      state = func_menu_set_round_recall_interval_sec1;
      break;
    case func_menu_set_round_recall_interval_sec1:
      state = func_menu_set_round_recall_interval_min10;
      break;
    default:
      //
      break;
  }

  switch (state) {
    case func_clock:
      state = func_menu_set_time_date_bright;
      blink_state = true;
      break;
    case func_date:
      state = func_menu_set_day10;
      blink_state = true;
      break;
    default:
      //
      break;
  }

  switch (state) {
    case func_menu_set_time_date_bright:
      state = func_menu_set_hour10;
      break;
    case func_menu_set_hour10:
      state = func_menu_set_hour1;
      break;
    case func_menu_set_hour1:
      state = func_menu_set_min10;
      break;
    case func_menu_set_min10:
      state = func_menu_set_min1;
      break;
    case func_menu_set_min1:
      state = func_menu_set_time_date_bright;
      break;
    default:
      //
      break;
  }

  switch (state) {
    case func_menu_set_day10:
      state = func_menu_set_day1;
      break;
    case func_menu_set_day1:
      state = func_menu_set_month10;
      break;
    case func_menu_set_month10:
      state = func_menu_set_month1;
      break;
    case func_menu_set_month1:
      state = func_menu_set_year10;
      break;
    case func_menu_set_year10:
      state = func_menu_set_year1;
      break;
    case func_menu_set_year1:
      state = func_menu_set_day10;
      break;
    default:
      //
      break;
  }

  blink_alternate = true;

  show_menu_timeout = set_menu_timeout;
}
