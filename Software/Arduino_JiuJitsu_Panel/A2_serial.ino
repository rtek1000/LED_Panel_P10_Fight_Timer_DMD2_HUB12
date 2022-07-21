//void serialEvent(void) {
//  while (Serial.available()) {
//    // get the new byte:
//    char inChar = (char) Serial.read();
//    if (inChar == '\n') {
//      //DateTime now = rtc.now();
//
//      if ((inputString.length() == 2) & (inputString[1] == 0x0D))
//        inputString[1] = 0; // remove "\r" 0x0D
//
//      if (inputString == "1") {
//        input_ctrl_right_serial = false;
//      } else if (inputString == "2") {
//        input_ctrl_left_serial = false;
//      } else if (inputString == "3") {
//        input_ctrl_cent_serial = false;
//      }
//
//      send_data_start = true;
//
//      //      if (inputString.length() > 0) {
//      //        for (int i = 0; i < inputString.length(); i++) {
//      //          Serial.print("0x");
//      //          if (inputString[i] < 16) Serial.print("0");
//      //          Serial.print(inputString[i], HEX);
//      //          Serial.print(" ");
//      //        }
//      //      }
//      //      Serial.println();
//      inputString = "";
//    } else {
//      if (inputString.length() < 20) {
//        inputString += inChar;
//      }
//    }
//  }
//}
//
//void send_data(void) {
//  int L10_tmp = L10, L1_tmp = L1, R10_tmp = R10, R1_tmp = R1;
//  uint8_t state10 = state / 10;
//  uint8_t state1 = state - (state10 * 10);
//  uint8_t stateAux = 0;
//  uint8_t stateAux10 = 0;
//  uint8_t stateAux1 = 0;
//
//  if ((state == func_normal) | (state == func_interval)
//      | (state == func_menu_set_round_time_mode)
//      | (state == func_menu_set_round10) | (state == func_menu_set_round1)
//      | (state == func_menu_set_round_time_min10)
//      | (state == func_menu_set_round_time_min1)
//      | (state == func_menu_set_round_time_sec10)
//      | (state == func_menu_set_round_time_sec1)) {
//    if ((state == func_normal) | (state == func_interval)) {
//      stateAux = pause_state;
//    } else {
//      stateAux = time_sel;
//    }
//    stateAux10 = round_cnt10;
//    stateAux1 = round_cnt1;
//  } else if (state == func_menu_bell_ding_mode) {
//    stateAux = bell_ding_mode_val;
//  } else if (state == func_menu_set_volume_beep) {
//    stateAux10 = beep_volume / 10;
//    stateAux1 = beep_volume - (stateAux10 * 10);
//  } else if (state == func_menu_set_volume_music) {
//    stateAux10 = music_volume / 10;
//    stateAux1 = music_volume - (stateAux10 * 10);
//  } else if (state == func_menu_set_volume_voice) {
//    stateAux10 = voice_volume / 10;
//    stateAux1 = voice_volume - (stateAux10 * 10);
//  } else if (state == func_clock) {
//    L10_tmp = iHour / 10;
//    L1_tmp = (iHour - (L10_tmp * 10));
//    R10_tmp = iMin / 10;
//    R1_tmp = (iMin - (R10_tmp * 10));
//
//    //        wr_rtc_yy = now.year();
//    //        wr_rtc_mm = now.month();
//    //        wr_rtc_dd = now.day();
//    //
//    //        hour1 = String(now.hour());
//    //
//    //        min1 = String(now.minute());
//  } else if (state == func_date) {
//    //        L10_tmp = now.day() / 10;
//    //        L1_tmp = (now.day() - (L10_tmp * 10));
//    //        R10_tmp = now.month() / 10;
//    //        R1_tmp = (now.month() - (R10_tmp * 10));
//
//    //        wr_rtc_yy = now.year();
//    //        wr_rtc_mm = now.month();
//    //        wr_rtc_dd = now.day();
//    //
//    //        hour1 = String(now.hour());
//    //
//    //        min1 = String(now.minute());
//  }
//
//  Serial.print(state10, DEC);
//  Serial.print(state1, DEC);
//  Serial.print(",");
//  Serial.print(stateAux, DEC);
//  Serial.print(",");
//  Serial.print(stateAux10, DEC);
//  Serial.print(stateAux1, DEC);
//  Serial.print(",");
//  Serial.print(L10_tmp, DEC);
//  Serial.print(L1_tmp, DEC);
//  Serial.print(",");
//  Serial.print(R10_tmp, DEC);
//  Serial.println(R1_tmp, DEC);
//}
