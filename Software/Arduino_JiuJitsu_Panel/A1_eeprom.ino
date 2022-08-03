//------ EEPROM address - start
enum {
  normal_recall_min10_addr = 0,
  normal_recall_min1_addr,
  normal_recall_sec10_addr,
  normal_recall_sec1_addr,

  interval_recall_min10_addr,
  interval_recall_min1_addr,
  interval_recall_sec10_addr,
  interval_recall_sec1_addr,

  round_display_bright10_addr,
  round_display_bright1_addr,

  clock_display_bright10_addr,
  clock_display_bright1_addr,

  bell_start_num_addr,
  bell_stop_num_addr,

  show_sel_time_date_addr,
  show_sel_time_date_bright_addr,

  bell_ding_enabled_addr,
  bell_ding_enabled_intv_addr,

  beep_volume_addr,
  music_volume_addr,
  voice_volume_addr,
  beep_type_start_addr,
  beep_type_stop_addr
};
//------ EEPROM address - end

void update_recall(void) {
  EEPROM.update(interval_recall_min10_addr, interval_recall_min10); // interval round
  EEPROM.update(interval_recall_min1_addr, interval_recall_min1); // interval round
  EEPROM.update(interval_recall_sec10_addr, interval_recall_sec10); // interval round
  EEPROM.update(interval_recall_sec1_addr, interval_recall_sec1); // interval round

  EEPROM.update(normal_recall_min10_addr, round_recall_min10); // round
  EEPROM.update(normal_recall_min1_addr, round_recall_min1); // round
  EEPROM.update(normal_recall_sec10_addr, round_recall_sec10); // round
  EEPROM.update(normal_recall_sec1_addr, round_recall_sec1); // round

  EEPROM.update(show_sel_time_date_bright_addr, show_sel_time_date_bright); //

  EEPROM.update(bell_ding_enabled_addr, bell_ding_mode_val); //
  EEPROM.update(bell_ding_enabled_intv_addr, bell_ding_mode_val_intv); //

  EEPROM.update(beep_volume_addr, beep_volume); //
  EEPROM.update(music_volume_addr, music_volume); //
  EEPROM.update(voice_volume_addr, voice_volume); //

  EEPROM.update(beep_type_start_addr, beep_type_start); //
  EEPROM.update(beep_type_stop_addr, beep_type_stop); //
}

void get_data_from_eeprom(void) {
  interval_recall_min10 = EEPROM.read(interval_recall_min10_addr); // interval round
  interval_recall_min1 = EEPROM.read(interval_recall_min1_addr); // interval round
  interval_recall_sec10 = EEPROM.read(interval_recall_sec10_addr); // interval round
  interval_recall_sec1 = EEPROM.read(interval_recall_sec1_addr); // interval round

  round_recall_min10 = EEPROM.read(normal_recall_min10_addr); // round
  round_recall_min1 = EEPROM.read(normal_recall_min1_addr); // round
  round_recall_sec10 = EEPROM.read(normal_recall_sec10_addr); // round
  round_recall_sec1 = EEPROM.read(normal_recall_sec1_addr); // round

  //show_sel_time_date = EEPROM.read(show_sel_time_date_addr) & 3; // 00000011

  show_sel_time_date_bright = EEPROM.read(show_sel_time_date_bright_addr);

  bell_ding_mode_val = EEPROM.read(bell_ding_enabled_addr) & 0x7;
  bell_ding_mode_val_intv = EEPROM.read(bell_ding_enabled_intv_addr) & 0x7;

  beep_volume = EEPROM.read(beep_volume_addr);

  if (beep_volume > 30) beep_volume = 1;
  if (beep_volume < 1) beep_volume = 1;

  music_volume = EEPROM.read(music_volume_addr);

  if (music_volume > 30) music_volume = 1;
  if (music_volume < 1) music_volume = 1;

  voice_volume = EEPROM.read(voice_volume_addr);

  if (voice_volume > 30) voice_volume = 1;
  if (voice_volume < 1) voice_volume = 1;

  beep_type_start = EEPROM.read(beep_type_start_addr);

  if (beep_type_start > 40) beep_type_start = 1;
  if (beep_type_start < 1) beep_type_start = 1;

  beep_type_stop = EEPROM.read(beep_type_stop_addr);

  if (beep_type_stop > 40) beep_type_stop = 1;
  if (beep_type_stop < 1) beep_type_stop = 1;

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
}
