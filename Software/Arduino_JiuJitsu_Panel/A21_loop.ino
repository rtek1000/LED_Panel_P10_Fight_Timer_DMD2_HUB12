void blink_sqw() {
  state_sqw = !state_sqw;
}

void loop (void) {
  if (millis() > (millisControl + 100)) {
    millisControl = millis();

    controller();
  }

  // currentMillis = millis();

  //if ((currentMillis - previousMillis) >= intervalMillis) {
  //if (state_sqw != state_sqw_old) {
  state_sqw = digitalRead(SQW_interruptPin);

  if (state_sqw != state_sqw_old) {
    // previousMillis = currentMillis;
    state_sqw_old = state_sqw;
    //state_sqw_old = digitalRead(SQW_interruptPin);

    show_date_time();

    if (blink_state == true) {
      blink_show();
    }

#ifndef skip_LM35
    temperature_read();
#endif

    if (second_old == false) {
      second_old = true;

      decrement_time_and_show();

      if ((state != func_normal) & (state != func_interval)
          & (state != func_clock) & (state != func_date)) {
        if (show_menu_timeout > 0) {
          show_menu_timeout--;

          if (show_menu_timeout == 0) {
            ctrl_left_cmd();
            show_menu_timeout = 0;
          }
        }
      }
    } else {
      second_old = false;
    }

    //    if (send_data_start == false) {
    //      //send_data();
    //    }
  }

  //  if (millis() > (millisAutoPlay + 1000)) {
  //    millisAutoPlay = millis();
  //
  ////    continuous_play();
  //  }

  //  if (send_data_start == true) {
  //    send_data_start = false;
  //    send_data();
  //  }

  wdt_reset(); //  reset watchdog
}
