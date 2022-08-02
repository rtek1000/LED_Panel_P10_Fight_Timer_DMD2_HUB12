void loop (void) {
  if ((millis() - millisControl) >= 100) {
    millisControl = millis();

    controller();

    if (power_off_enabled == true) {
      dmd.setBrightness(3);
      
      dmd.clearScreen();

      if ((millis() - power_off_timeout_blink) >= 10000) {
        power_off_timeout_blink = millis();

        dmd.setPixel(0, 0);

        delayWdt(100);

        dmd.clearScreen();
      }
    }
  }

  if (power_off_enabled == false) {
    state_sqw = digitalRead(SQW_interruptPin);

    if (state_sqw != state_sqw_old) {
      state_sqw_old = state_sqw;

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
